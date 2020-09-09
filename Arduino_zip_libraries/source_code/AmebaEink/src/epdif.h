#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>

// Pin definition
#define BUSY_Pin 8
#define RES_Pin 9
#define DC_Pin 10
#define CS_Pin 11
#define SCK_Pin 12
#define SDI_Pin 13

#define EPD_W21_MOSI_0 digitalWrite(SDI_Pin, LOW)
#define EPD_W21_MOSI_1 digitalWrite(SDI_Pin, HIGH)
#define EPD_W21_CLK_0 digitalWrite(SCK_Pin, LOW)
#define EPD_W21_CLK_1 digitalWrite(SCK_Pin, HIGH)
#define EPD_W21_CS_0 digitalWrite(CS_Pin, LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin, HIGH)
#define EPD_W21_DC_0 digitalWrite(DC_Pin, LOW)
#define EPD_W21_DC_1 digitalWrite(DC_Pin, HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin, LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin, HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2
#define RED_MODE 3
// Display resolution
#define EPD_WIDTH 128
#define EPD_HEIGHT 296
#define MAX_LINE_BYTES 16  // =128/8
#define ALLSCREEN_GRAGHBYTES 4736

// EPD2IN9 commands
#define DRIVER_OUTPUT_CONTROL 0x01
#define BOOSTER_SOFT_START_CONTROL 0x0C
#define DEEP_SLEEP_MODE 0x10
#define DATA_ENTRY_MODE_SETTING 0x11
#define MASTER_ACTIVATION 0x20
#define DISPLAY_UPDATE_CONTROL_2 0x22
#define WRITE_RAM 0x24
#define WRITE_VCOM_REGISTER 0x2C
#define WRITE_LUT_REGISTER 0x32
#define SET_DUMMY_LINE_PERIOD 0x3A
#define SET_GATE_TIME 0x3B
#define BORDER_WAVEFORM_CONTROL 0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#define SET_RAM_X_ADDRESS_COUNTER 0x4E
#define SET_RAM_Y_ADDRESS_COUNTER 0x4F
#define TERMINATE_FRAME_READ_WRITE 0xFF

class EpdIf {
public:
    EpdIf(void);
    ~EpdIf(void);
    void EPD_Dis_Part(
        unsigned int x_start,
        unsigned int y_start,
        const unsigned char* datas,
        unsigned int PART_COLUMN,
        unsigned int PART_LINE);
    void EPD_SetFrame(
        const unsigned char* image_buffer,
        int x,
        int y,
        int image_width,
        int image_height);
    void EPD_SetRAMValue_BaseMap(const unsigned char* datas);
    void EPD_SetFrameMemory(const unsigned char* image_buffer);
    void EPD_UpdateDisplay(void);
    void EPD_Part_Update(void);
    void EPD_ClearScreen_White(void);
    void EPD_ClearScreen_Black(void);

    void EPD_Busy(void);
    void EPD_Reset(void);
    void EPD_Sleep(void);

private:
    // Driver
    void driver_delay_us(unsigned int xus);
    void driver_delay_xms(unsigned long xms);
    void DELAY_S(unsigned int delaytime);
    // SPI
    void SpiDelay(unsigned char xrate);
    void SpiTransfer(unsigned char value);
    void SendData(unsigned char command);
    void SendCommand(unsigned char command);
    // EPD initialization
    void EPD_Init(void);  // Hardware initialization
    void EPD_InitText(void);
    void EPD_Part_Init(void);
    void SetLUT(const unsigned char* lut);
};
#endif
