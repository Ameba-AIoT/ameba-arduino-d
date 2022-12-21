/*
  This sketch shows how to use power save deepsleep mode
*/
#include <PowerSave.h>

#include "image.h"
#include "epdif.h"
#include "epdpaint.h"

#include "DHT.h"

// SPI Pin definition
// For all supported boards please chose 2 GPIO pin as EPD_BUSY_Pin and EPD_RES_Pin. And set default SPI/SPI1 pins as EPD_SPI pin.
#define EPD_BUSY_Pin        8
#define EPD_RES_Pin         9
#define EPD_SPI_CS_Pin      SPI_SS
#define EPD_SPI_MOSI_Pin    SPI_MOSI
#define EPD_SPI_MISO_Pin    SPI_MISO
#define EPD_SPI_CLK_Pin     SPI_SCLK

#define COLOR_BLACK        0
#define COLOR_WHITE        1
EpdIf EPD(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);
unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

#define DHTPIN 6                        // DHT data digital pin
#define DHTTYPE DHT11                   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//SET_DS_AON_TIMER_WAKEUP
//SET_DS_RTC_WAKEUP
//For AMB21/22 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA25          // pin 16
    //SET_AON_GPIO_WAKEUP_PA26          // pin 17
    //SET_AON_GPIO_WAKEUP_PA21          // pin 26
    //SET_AON_GPIO_WAKEUP_PA20          // pin 27
    //SET_AON_GPIO_WAKEUP_PA19          // pin 28
//For AMB23 only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA12          // pin 9
    //SET_AON_GPIO_WAKEUP_PA13          // pin 10
    //SET_AON_GPIO_WAKEUP_PA14          // pin 11
    //SET_AON_GPIO_WAKEUP_PA15          // pin 12
    //SET_AON_GPIO_WAKEUP_PA16          // pin 13
    //SET_AON_GPIO_WAKEUP_PA18          // pin 15
    //SET_AON_GPIO_WAKEUP_PA19          // pin 16
    //SET_AON_GPIO_WAKEUP_PA21          // pin 18
//For BW16/BW16-TypeC only the AON GPIO pins listed below should be selected
    //SET_AON_GPIO_WAKEUP_PA25          // pin 7
    //SET_AON_GPIO_WAKEUP_PA26          // pin 8
    //SET_AON_GPIO_WAKEUP_PA15          // pin 9
    //SET_AON_GPIO_WAKEUP_PA14          // pin 10
    //SET_AON_GPIO_WAKEUP_PA13          // pin 11
    //SET_AON_GPIO_WAKEUP_PA12          // pin 12
//For board AW-CU488_ThingPlus
    //SET_AON_GPIO_WAKEUP_PA18          // pin 0
    //SET_AON_GPIO_WAKEUP_PA16          // pin 1
    //SET_AON_GPIO_WAKEUP_PA17          // pin 2
    //SET_AON_GPIO_WAKEUP_PA13          // pin 3
    //SET_AON_GPIO_WAKEUP_PA12          // pin 4
    //SET_AON_GPIO_WAKEUP_PA26          // pin 5
    //SET_AON_GPIO_WAKEUP_PA25          // pin 6
    //SET_AON_GPIO_WAKEUP_PA19          // pin 8
    //SET_AON_GPIO_WAKEUP_PA15          // pin 28
    //SET_AON_GPIO_WAKEUP_PA14          // pin 29
//For board AMB25/AMB26
    //SET_AON_GPIO_WAKEUP_PA12          // pin 3
    //SET_AON_GPIO_WAKEUP_PA13          // pin 2
    //SET_AON_GPIO_WAKEUP_PA14          // pin 1
    //SET_AON_GPIO_WAKEUP_PA15          // pin 0
    //SET_AON_GPIO_WAKEUP_PA25          // pin 7
    //SET_AON_GPIO_WAKEUP_PA26          // pin 6


#define DS_WAKEUP_SOURCE                SET_DS_AON_TIMER_WAKEUP

#define AON_TIMER_SLEEP_DURATION        5000
#define DS_RTC_ALARM_DAY                0
#define DS_RTC_ALARM_HOUR               0
#define DS_RTC_ALARM_MIN                0
#define DS_RTC_ALARM_SEC                10

void DeepSleep_wakeup(void) {
    printf("\r\nDeep sleep wakeuped! \r\n");
    uint32_t wakereason_number = PowerSave.AONWakeReason();

    if (wakereason_number == AONWakeReason_AON_GPIO) {
        //printf("AonWakepin wakeup, wakepin is D%d. Wait 5s sleep again.    \r\n", PowerSave.WakePinCheck());
        //delay(5000);
    } else if (wakereason_number == AONWakeReason_AON_TIMER) {
        //PowerSave.AONTimerCmd();
        //printf("Aontimer wakeup. Wait 5s sleep again.    \r\n");
        //delay(5000);
    } else if (wakereason_number == AONWakeReason_RTC) {
        //printf("RTC wakeup. Wait 5s sleep again.    \r\n");
        //delay(5000);
    }

    PowerSave.AONWakeClear();
}

void DisplayText(float h, float t) {
    String DHT_h_s = String((int)h);
    String DHT_t_s = String((int)t);

    DHT_h_s = String("Humd: " + DHT_h_s);
    DHT_t_s = String("Temp: " + DHT_t_s);
    DHT_h_s = String(DHT_h_s + " %");
    DHT_t_s = String(DHT_t_s + " C");

    char DHT_h_array[20] = {0};
    char DHT_t_array[20] = {0};

    DHT_h_s.toCharArray(DHT_h_array, 20);
    DHT_t_s.toCharArray(DHT_t_array, 20);

    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapNA);

    paint.SetRotate(ROTATE_90);
    paint.SetWidth(30);
    paint.SetHeight(250);

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 4, DHT_t_array, &Font24, COLOR_BLACK);
    EPD.EPD_SetFrame(paint.GetImage(), 30, 25, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 4, DHT_h_array, &Font24, COLOR_BLACK);
    EPD.EPD_SetFrame(paint.GetImage(), 75, 25, paint.GetWidth(), paint.GetHeight());

    EPD.EPD_UpdateDisplay();
}

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // SPI setup
    pinMode(EPD_BUSY_Pin, INPUT);       // EPD_BUSY_Pin
    pinMode(EPD_RES_Pin, OUTPUT);       // EPD_RES_Pin
    pinMode(EPD_SPI_CS_Pin, OUTPUT);    // EPD_SPI_CS_Pin
    pinMode(EPD_SPI_MOSI_Pin, OUTPUT);  // EPD_SPI_MOSI_Pin
    pinMode(EPD_SPI_MISO_Pin, OUTPUT);  // EPD_SPI_MISO_Pin
    pinMode(EPD_SPI_CLK_Pin, OUTPUT);   // EPD_SPI_CLK_Pin

    PowerSave.begin(DEEPSLEEP_MODE);

    if (TRUE == (PowerSave.DsleepWakeStatusGet())) {
        DeepSleep_wakeup();
    }

    dht.begin();                    // initialize the dht
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    DisplayText(h, t);

    PowerSave.AONTimerDuration(0);

    switch (DS_WAKEUP_SOURCE) {
        case SET_DS_AON_TIMER_WAKEUP:
            PowerSave.DS_AON_TIMER_WAKEUP();
            PowerSave.AONTimerDuration(AON_TIMER_SLEEP_DURATION);
            break;
        case SET_DS_RTC_WAKEUP:
            PowerSave.DS_RTC_WAKEUP();
            PowerSave.RTCWakeSetup(DS_RTC_ALARM_DAY, DS_RTC_ALARM_HOUR, DS_RTC_ALARM_MIN, DS_RTC_ALARM_SEC);
            break;
        case SET_AON_GPIO_WAKEUP_PA12:
            PowerSave.AON_WAKEPIN_WAKEUP_PA12();
            break;
        case SET_AON_GPIO_WAKEUP_PA13:
            PowerSave.AON_WAKEPIN_WAKEUP_PA13();
            break;
        case SET_AON_GPIO_WAKEUP_PA14:
            PowerSave.AON_WAKEPIN_WAKEUP_PA14();
            break;
        case SET_AON_GPIO_WAKEUP_PA15:
            PowerSave.AON_WAKEPIN_WAKEUP_PA15();
            break;
        case SET_AON_GPIO_WAKEUP_PA16:
            PowerSave.AON_WAKEPIN_WAKEUP_PA16();
            break;
        case SET_AON_GPIO_WAKEUP_PA17:
            PowerSave.AON_WAKEPIN_WAKEUP_PA17();
            break;
        case SET_AON_GPIO_WAKEUP_PA18:
            PowerSave.AON_WAKEPIN_WAKEUP_PA18();
            break;
        case SET_AON_GPIO_WAKEUP_PA19:
            PowerSave.AON_WAKEPIN_WAKEUP_PA19();
            break;
        case SET_AON_GPIO_WAKEUP_PA20:
            PowerSave.AON_WAKEPIN_WAKEUP_PA20();
            break;
        case SET_AON_GPIO_WAKEUP_PA21:
            PowerSave.AON_WAKEPIN_WAKEUP_PA21();
            break;
        case SET_AON_GPIO_WAKEUP_PA25:
            PowerSave.AON_WAKEPIN_WAKEUP_PA25();
            break;
        case SET_AON_GPIO_WAKEUP_PA26:
            PowerSave.AON_WAKEPIN_WAKEUP_PA26();
            break;
        default:
            printf("Unknown wakeup source.    \r\n");
        break;
    }

    PowerSave.enable();
}

void loop() {
    delay(1000);
}
