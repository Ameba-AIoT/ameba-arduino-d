#include "amb_ard_pin_check.h"

#ifdef AMEBA_ARDUINO_Pin_Mapping_Check

#ifdef __cplusplus
extern "C" {
#endif

    //g_APinDescription[ard_pin_name].pinname;
    //g_APinDescription[ard_pin_name].ulPinType;
    //g_APinDescription[ard_pin_name].ulPinAttribute;
    //g_APinDescription[ard_pin_name].ulPinMode;
void amb_ard_pin_check_name(int pin) {
    while (pin > TOTAL_GPIO_PIN_NUM) {
        printf("Error %s. The pin: %d beyond the pin mapping!!! \n\r", __FUNCTION__, pin);
        printf("Please check if pin or board is correct \n\r");
        delay(5000);
    }

    while (g_APinDescription[pin].pinname == NC) {
        printf("Error %s. The pin: %d is NC!!! \n\r", __FUNCTION__, pin);
        printf("Please check if pin or board is correct \n\r");
        delay(5000);
    }
}

// pin_type
// TYPE_ANALOG                     (1UL<<21)
// TYPE_DIGITAL                    (1UL<<22)
void amb_ard_pin_check_type(int pin, uint32_t pin_type) {
    // char pin_type_name[] = "";
    uint32_t check_bit = (g_APinDescription[pin].ulPinType & pin_type);

    switch (check_bit) {
        case TYPE_ANALOG:
            // strcpy(pin_type_name, "TYPE_ANALOG");
            break;
        case TYPE_DIGITAL:
            // strcpy(pin_type_name, "TYPE_DIGITAL");
            break;
        default:
            while (1) {
                printf("Error %s. Incorrect pin: %d \n\r", __FUNCTION__, pin);
                printf("Please check if pin or board is correct \n\r");
                delay(5000);
            }
    }
}

// pin_fun
// PIO_GPIO                        (1UL<<1)
// PIO_PWM                         (1UL<<2)
// PIO_I2C                         (1UL<<3)
// PIO_ADC                         (1UL<<4)
// PIO_DAC                         (1UL<<5)
// PIO_GPIO_IRQ                    (1UL<<6)
// PIO_IR                          (1UL<<7)
// PIO_UART                        (1UL<<8)
// PIO_SPI                         (1UL<<9)
void amb_ard_pin_check_fun(int pin, uint32_t pin_fun) {
    // char pin_fun_name[] = "";
    uint32_t check_bit = (g_APinDescription[pin].ulPinAttribute & pin_fun);

    switch (check_bit) {
        case PIO_GPIO:
            // strcpy(pin_fun_name, "PIO_GPIO");
            break;
        case PIO_PWM:
            // strcpy(pin_fun_name, "PIO_PWM");
            break;
        case PIO_I2C:
            // strcpy(pin_fun_name, "PIO_I2C");
            break;
        case PIO_ADC:
            // strcpy(pin_fun_name, "PIO_ADC");
            break;
        case PIO_DAC:
            // strcpy(pin_fun_name, "PIO_DAC");
            break;
        case PIO_GPIO_IRQ:
            // strcpy(pin_fun_name, "PIO_GPIO_IRQ");
            break;
        case PIO_IR:
            // strcpy(pin_fun_name, "PIO_IR");
            break;
        case PIO_UART:
            // strcpy(pin_fun_name, "PIO_UART");
            break;
        case PIO_SPI:
            // strcpy(pin_fun_name, "PIO_SPI");
            break;
        default:
            while (1) {
                printf("Error %s. Incorrect pin: %d \n\r", __FUNCTION__, pin);
                printf("Please check if pin or board is correct \n\r");
                delay(5000);
            }
    }
}

#ifdef __cplusplus
}
#endif

#endif // AMEBA_ARDUINO_Pin_Mapping_Check
