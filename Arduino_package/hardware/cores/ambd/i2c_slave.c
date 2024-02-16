#include "ameba_soc.h"
#include "PinNames.h"
#include "basic_types.h"
#include "diag.h"
#include "pinmap.h"
#include "i2c_slave.h"
#include "wiring_constants.h"

#define DEBUG 0

#if DEBUG
#define DBG(fmt, args...) \
            do {printf("\r\nFunc-[%s]@Line-%d: \r\n"fmt"\r\n", __func__, __LINE__, ## args); } while (0);
#else
#define DBG(fmt, args...)
#endif

#define I2C_SLAVE_RX_FIFO_THRESHOLD 0    // set HW I2C RX FIFO threshold to 8 bytes

#define ENABLE_DEFAULT_HANDLER 1

#define EVENT_QUEUE_LEN 16

int flag = 0;

// Initialize an instance to be used globally within the file
static i2c_slave_struct_t _i2c_slave = {NULL,          // i2c_t instance
                                        NULL,          // arg
                                        0,             // tx datalength
                                        0,             // rx dataleangth
                                        0,             // event status | NO_EVENT
                                        NULL,          // request callback 
                                        NULL,          // receive callback
                                        NULL,          // RTOS task handle
                                        NULL,          // Event Queue (tx or rx)
                                        NULL,          // RX Queue for passing data from ISR
                                        NULL,          // TX Queue for passing data from ISR
                                        NULL           // Event semaphore
};

I2C_InitTypeDef I2CInitData[2];

u32 I2C_Index_Get(PinName sda) {
    if ((sda == _PA_26) || (sda == _PB_6) || (sda == _PB_0)) {
        return 0;
    } else {
        assert_param(0);
    }
    return 2;
}

void I2CISRHandle(void *data) {
    i2c_slave_struct_t *obj = (i2c_slave_struct_t *) data;
    i2c_slave_queue_event_t event;
    //BaseType_t pxHigherPriorityTaskWoken = false;
    DBG("Enter Handler")

    // Check if pointer passed in is correct
    assert_param(IS_I2C_ALL_PERIPH(obj->i2c_slave_dev->I2Cx));

    u32 intr_status = I2C_GetINT(obj->i2c_slave_dev->I2Cx);

    DBG("Get handler status")
    /* I2C ADDR MATCH Intr*/
    if (intr_status & BIT_IC_INTR_STAT_R_ADDR_1_MATCH) {
        /* Clear I2C interrupt */
        I2C_WakeUp(obj->i2c_slave_dev->I2Cx);
    }
    if (intr_status & BIT_IC_INTR_STAT_R_ADDR_2_MATCH) {
        /* Clear I2C interrupt */
        I2C_WakeUp(obj->i2c_slave_dev->I2Cx);
    }

    /* I2C Activity Intr */
    if (intr_status & BIT_IC_INTR_STAT_R_ACTIVITY) {
        /* Clear I2C interrupt */
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_ACTIVITY);
    }

    /* I2C RX Done Intr */
    if (intr_status & BIT_IC_INTR_STAT_R_RX_DONE) {
        //slave-transmitter and master not ACK it, This occurs on the last byte of
        //the transmission, indicating that the transmission is done. 
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RX_DONE);
    }

    /* I2C TX Abort Intr */ 
    if (intr_status & BIT_IC_INTR_STAT_R_TX_ABRT) {
        // This interrupt is needed to flush the old data in the TX FIFO before we can write new data in the FIFO
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_TX_ABRT);
    }

    /* I2C TX Empty Intr */
    if (intr_status & BIT_IC_INTR_STAT_R_TX_EMPTY) {
        //I2CISRHandleTxEmpty(obj);
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_TX_EMPTY);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //                                                                                //
    //  TX EVENT -- handle read request from master, slave serves as transmitter now  //
    //                                                                                //
    ////////////////////////////////////////////////////////////////////////////////////
    if (intr_status & BIT_IC_INTR_STAT_R_RD_REQ) {
        DBG("RD_REQ interrupt detected!")
        // Send event on Read Request
        obj->event_status = (u32)I2C_SLAVE_TX_EVENT;

        event.event = I2C_SLAVE_TX_EVENT;
        DBG("The event->event is %d", event.event)

        DBG("  Sending TX Event now!!!  ")
        if(xQueueSendFromISR(obj->event_queue, &event, NULL) != pdTRUE) {
            printf("Send Event Queue from ISR failed\n");
        } else {DBG("TX Event sent success")}

        DBG("send TX event finished")
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RD_REQ);
    }

    /* I2C TX Over Run Intr */
    if (intr_status & BIT_IC_INTR_STAT_R_TX_OVER) {
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_TX_OVER);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //                                                                                //
    //  RX EVENT -- handle transmitted data from master, slave serve as receiver now  //
    //                                                                                //
    ////////////////////////////////////////////////////////////////////////////////////
    if (intr_status & BIT_IC_INTR_STAT_R_RX_FULL ||	intr_status & BIT_IC_INTR_STAT_R_RX_OVER) {
        /*I2C RX Over Run Intr*/
        if (intr_status & BIT_IC_INTR_STAT_R_RX_OVER) {
            I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RX_OVER);
            DBG("RX Over triggered\r\n")
        }

        if (intr_status & BIT_IC_INTR_STAT_R_RX_FULL) {
            //I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RX_OVER);
            DBG("RX Full triggered\r\n")
        }

        // Process the full or overflown RX FIFO, and send data to rx_queue which is to be recved on i2c slave thread
        // program would not return until all data are received
        // By default, RX FIFO Threshold is 0 => Just 1 entry of data(1 byte) is allowed and will trigger RX FULL interrupt immediately

        //printf("receiveEvent triggered!\r\n");
        DBG("\r\nRX FIFO Level: %d\r\n", obj->i2c_slave_dev->I2Cx->IC_RXFLR)
        DBG("RX FIFO Thshd: %d\r\n", obj->i2c_slave_dev->I2Cx->IC_RX_TL)

        /* Receive data till the RX FIFO is empty */
        DBG("rx full, reading data out")
        //printf("receivedEvent triggered[%d]\r\n", obj->rx_datalength);
        DelayUs(1000);
        while (I2C_CheckFlagState(obj->i2c_slave_dev->I2Cx, (BIT_IC_STATUS_RFNE | BIT_IC_STATUS_RFF))) {
            u32 data = 0;

            // Read 1 byte data from I2C RX FIFO
            data = (u32)I2C_ReceiveData(obj->i2c_slave_dev->I2Cx);
            obj->rx_datalength++;

            if(pdTRUE != xQueueSendFromISR(obj->rx_queue, &data, NULL)) {
                printf("\r\n rx queue full(max 128 bytes)!");
            }
            if (intr_status & BIT_IC_INTR_STAT_R_STOP_DET){
                break;
            }
        }
        DBG("RX LEN: %d ", obj->rx_datalength)

        //if (intr_status & BIT_IC_INTR_STAT_R_STOP_DET) {
            // Sending RX event only if data are received successfully
            if (obj->rx_datalength) {
                DBG("  Sending RX Event now!!!  ")
                event.event = I2C_SLAVE_RX_EVENT;
                event.stop = 0;
                event.param = obj->rx_datalength;
                DBG("\r\n[rx_datalength]: %d\r\n", event.param);
                DBG("The event->event is %d", event.event)
                if(xQueueSendFromISR(obj->event_queue, &event, NULL) != pdTRUE) {
                    printf("Send Event Queue from ISR failed\n");
                } else {
                    DBG("RX Event sent success")
                }
                // zero RX data count
                obj->rx_datalength = 0;
            } else {
                DBG("didn't recv data in RX FIFO, not sending TX Event")
            }
            /* Clear I2C interrupt */
            I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_STOP_DET);
        //}

        // clear this interrupt once its serviced
        // no need to clear, it clears itself once buffer level goes below the threshold
        // I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RX_FULL);
    }

    // I2C RX Under Run Intr
    if (intr_status & BIT_IC_INTR_STAT_R_RX_UNDER) {
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_RX_UNDER);
        DBG("RX Under triggered\r\n")
    }

    // Handle I2C STOP DET Intr if it is not already done 
    if (intr_status & BIT_IC_INTR_STAT_R_STOP_DET) {
        /* Clear I2C interrupt */
        I2C_ClearINT(obj->i2c_slave_dev->I2Cx, BIT_IC_INTR_STAT_R_STOP_DET);
        //printf("Stop bit detected\r\n");
    }
    DBG("end of ISR")
}


void i2c_slave_attach_callbacks(i2c_slave_request_cb_t onRequestService, i2c_slave_receive_cb_t onReceiveService, void * arg) {
    i2c_slave_struct_t *i2c = &_i2c_slave;
    i2c->arg = arg;      // pointer to the a "Wire" instance from the cpp layer
    i2c->rqst_callback = onRequestService;
    i2c->recv_callback = onReceiveService;

    DBG("attach success!\r\n")
}

// Public API to send user data, available to user via Wire.cpp
// Responsible for transmitting data in slave mode
bool i2cSlaveWrite(const u8 *buffer, size_t len, u32 timeout) {
    i2c_slave_struct_t *i2c = &_i2c_slave;
    i2c->tx_datalength = len;
    //u32 cnt = 0;
    timeout = timeout;

    DBG("i2cSlaveWrite: Writing to master!\r\n")

    DBG("buffer content is %s", (char *)buffer)

    u32 intr_rawState = I2C_GetRawINT(i2c->i2c_slave_dev->I2Cx);

    for (; i2c->tx_datalength > 0; i2c->tx_datalength--) {
        /* Check I2C RD Request flag */
        DBG("In the loop")
        //while((intr_status & BIT_IC_INTR_STAT_R_RD_REQ) == 0); // wait until this flag is set
        DBG("RD REQ set")
        /* Check I2C TX FIFO status */
        while(((I2C_CheckFlagState(i2c->i2c_slave_dev->I2Cx, BIT_IC_STATUS_TFNF)) == 0) && ((intr_rawState & BIT_IC_RAW_INTR_STAT_RX_DONE) == 0));
        DBG("sending now")
        I2C_SlaveSend(i2c->i2c_slave_dev->I2Cx, *buffer);
        buffer++;
    }

    DBG("Writing to master-2!\r\n")

    // wait until TX FIFO is completely empty, then clear this interrupt
    while(((I2C_CheckFlagState(i2c->i2c_slave_dev->I2Cx, BIT_IC_STATUS_TFE)) == 0) &&((intr_rawState & BIT_IC_RAW_INTR_STAT_RX_DONE) == 0));
    i2c->i2c_slave_dev->I2Cx->IC_CLR_INTR;

    DBG("Writing to master-3!\r\n")

    DBG("Finish writing to master!")

    if (0 == i2c->tx_datalength) {
        return true;
    } else {
        return false;
    }
}

// Responsible for reading incoming data from ISR via queue
// Only can be used in this file
static size_t i2cSlaveRead(i2c_slave_struct_t * i2c, u8 * data, size_t len){
    if (!len) {
        return 0;
    }
    u8 d = 0;
    BaseType_t res = pdTRUE;
    for (size_t i = 0; i < len; i++) {
        if (data) {
            // recv 1 byte from RX FULL Handler, max wait for 1 tick(0 is too fast, will cause QueueReceive fail)
            DBG("using data")
            res = xQueueReceive(i2c->rx_queue, &data[i], 5);
        } else {
            // use d just in case malloc() failed in the previous step
            DBG("using d")
            res = xQueueReceive(i2c->rx_queue, &d, 1);
        }
        if (res != pdTRUE) {
            printf("Read Queue(%d) Failed\r\n", i);
            len = i;
            break;
        }
    }
    return (data ? len : 0);
}

void i2c_slave_task_thread(void *param) {
    i2c_slave_struct_t *i2c = (i2c_slave_struct_t *)param;
    i2c_slave_queue_event_t i2c_event;
    size_t len = 0;
    bool stop = false;
    u8 *data = NULL;

    DBG("\r\n @@@ in slave task @@@ \r\n\r\n")
    for (;;) {
        // Wait until received event queue
        DBG("Checking event queue")
        if(xQueueReceive(i2c->event_queue, &i2c_event, portMAX_DELAY) == pdTRUE) {
            DBG("Event recved!")

            // TX Event
            if(i2c_event.event == I2C_SLAVE_TX_EVENT) {
                DBG("TX Event")
                if(i2c->rqst_callback) {
                    i2c->rqst_callback(i2c->arg); // run user onRequest callback function which calls "i2cSlaveWrite()" from CPP
                }
            // RX Event
            } else if (i2c_event.event == I2C_SLAVE_RX_EVENT) {
                DBG("RX Event")
                len = i2c_event.param;
                //printf("len 1 :%d\r\n", len);
                stop = i2c_event.stop;
                data = (len > 0)?(u8*)malloc(len):NULL;

                if (len && data == NULL) {
                    printf("Malloc (%d) Failed\r\n", len);
                }
                len = i2cSlaveRead(i2c, data, len); // read data from rx queue and put into 'data'
                //printf("len 2 :%d\r\n", len);
                if (i2c->recv_callback) {
                    i2c->recv_callback(data, len, stop, i2c->arg);  // run user onReceive callback function
                }
                free(data);
            } else {
                printf("Unidentified event received!\r\n");
            }
        }
    }
    DBG("End of slave task")
    vTaskDelete(NULL);
}

bool i2c_slave_init(i2c_t *obj, PinName SDA, PinName SCL, u8 addr, size_t buffer_len) {
    DBG("Enter slave init")

    // create local instance and initialize it with an existing instance
    i2c_slave_struct_t *i2c = &_i2c_slave;
    i2c->i2c_slave_dev = obj;
    DBG("Initializing slave task")

    // create queues for passing event, HW RX buffer and HW TX buffer data
    i2c->event_queue = xQueueCreate(EVENT_QUEUE_LEN, sizeof(i2c_slave_queue_event_t));
    if (i2c->event_queue == NULL) {
        printf("Event queue create failed\n");
        return false;
    }
    
    i2c->rx_queue = xQueueCreate(buffer_len, sizeof(u8)); // default queue buffer len 128
    if (i2c->rx_queue == NULL) {
        printf("RX queue create failed\n");
        return false;
    }

    i2c->tx_queue = xQueueCreate(buffer_len, sizeof(u8)); // default queue buffer len 128
    if (i2c->tx_queue == NULL) {
        printf("TX queue create failed\n");
        return false;
    }

    i2c->event_sema = xSemaphoreCreateBinary();
    if( i2c->event_sema == NULL ) {
        printf("Event semaphore created failed!\r\n");
        return false;
    }

    // create a task at very high priority to handle user callback
    if (xTaskCreate(i2c_slave_task_thread,                      // task created
                    ((const char*)"i2c_slave_task_thread"),     // task name
                    (4*1024),                                   // stack size - 4KB is enough
                    i2c,                                        // parameter to pass in
                    8,                                          // priority, set it high to be more responsive (max 10)
                    &i2c->task_handle)                          // task handle
        != pdPASS) { 
            printf("\n\r%s xTaskCreate failed", __FUNCTION__);  // fail-safe msg
            return false;
    }

    /////////////////////////////////////////////////////////////
    //                                                         //
    //  Init I2C peripheral as slave and enable i2c interrupt  //
    //                                                         //
    /////////////////////////////////////////////////////////////
    i2c->i2c_slave_dev->i2c_idx = I2C_Index_Get(SDA);
    I2C_StructInit(&I2CInitData[i2c->i2c_slave_dev->i2c_idx]);
    I2CInitData[i2c->i2c_slave_dev->i2c_idx].I2CMaster = I2C_SLAVE_MODE;
    i2c->i2c_slave_dev->I2Cx = I2C_DEV_TABLE[i2c->i2c_slave_dev->i2c_idx].I2Cx;
    I2CInitData[i2c->i2c_slave_dev->i2c_idx].I2CIdx = i2c->i2c_slave_dev->i2c_idx;
    I2CInitData[i2c->i2c_slave_dev->i2c_idx].I2CAckAddr = addr;
    I2CInitData[i2c->i2c_slave_dev->i2c_idx].I2CRXTL = I2C_SLAVE_RX_FIFO_THRESHOLD;

    /* I2C Pin Mux Initialization */
    Pinmux_Config(SDA, PINMUX_FUNCTION_I2C);
    Pinmux_Config(SCL, PINMUX_FUNCTION_I2C);
    PAD_PullCtrl(SDA, GPIO_PuPd_UP);
    PAD_PullCtrl(SCL, GPIO_PuPd_UP);

    // Interrupt enable
    InterruptRegister((IRQ_FUN)I2CISRHandle, I2C0_IRQ_LP, (u32)(i2c), 7);
    InterruptEn(I2C0_IRQ_LP, 7);

    /* I2C HAL Initialization */
    I2C_Init(i2c->i2c_slave_dev->I2Cx, &I2CInitData[i2c->i2c_slave_dev->i2c_idx]);

    /* I2C Enable Module */
    I2C_Cmd(i2c->i2c_slave_dev->I2Cx, ENABLE);

    // Config Interrupt bits and enable them
    I2C_INTConfig(i2c->i2c_slave_dev->I2Cx, (BIT_IC_INTR_MASK_M_TX_ABRT |  //TX related
                                             BIT_IC_INTR_MASK_M_TX_OVER |  //TX related
                                             BIT_IC_INTR_MASK_M_RX_DONE |  //TX related
                                             BIT_IC_INTR_MASK_M_RD_REQ  |  //TX related
                                             BIT_IC_INTR_MASK_M_RX_FULL |  //RX related
                                             BIT_IC_INTR_MASK_M_RX_OVER |  //RX related
                                             BIT_IC_INTR_MASK_M_RX_UNDER|  //RX related
                                             BIT_IC_INTR_MASK_M_STOP_DET), //RX related
                                             ENABLE);
    return true;
}
