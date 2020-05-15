#ifndef _RTL8195A_FT_ADC_DAC_H_
#define _RTL8195A_FT_ADC_DAC_H_

//--------- Verification control flag >----------------------------------------
// Enable SPI DAC
#define SPI_DAC_OUTPUT      1

// To reduce harmonic for better SNR result
#define FFT_REDUCE_HARMONIC 1
#define FFT_HARMONIC_ORDER  6

// Enable FFT debug msg
#define FFT_DBG_INFO        0

// Engineer verification only
#define FLOW_VERIFICATION   0

//--------- Verification control flag <----------------------------------------


#define ADDA_VERI_SUBJECT_NO    2
typedef enum ADDA_VEFI_TYPE {
        DAC_SRC_SPI_DMA = 0,
        DAC_SRC_I2C_DMA = 1,
};

#define I2C_DAC_FFT_CRITERION     (float32_t)60.000
#define EXTER_DAC_FFT_CRITERION     (float32_t)56.000

extern int FTAdcFFTTest(IN  unsigned char TestItem);

#endif
