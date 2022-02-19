/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ====================== DASN.c =================================
 *  This board file is made for the 7x7 mm QFN package, to convert this board
 *  file to use for other smaller device packages please refer to the table
 *  below which lists the max IOID values supported by each package. All other
 *  unused pins should be set to IOID_UNUSED.
 *
 *  Furthermore the board file is also used
 *  to define a symbol that configures the RF front end and bias.
 *  See the comments below for more information.
 *  For an in depth tutorial on how to create a custom board file, please refer
 *  to the section "Running the SDK on Custom Boards" with in the Software
 *  Developer's Guide.
 *
 *  Refer to the datasheet for all the package options and IO descriptions:
 *  http://www.ti.com/lit/ds/symlink/cc2640r2f.pdf
 *
 *  +-----------------------+------------------+-----------------------+
 *  |     Package Option    |  Total GPIO Pins |   MAX IOID            |
 *  +=======================+==================+=======================+
 *  |     7x7 mm QFN        |     31           |   IOID_30             |
 *  +-----------------------+------------------+-----------------------+
 *  |     5x5 mm QFN        |     15           |   IOID_14             |
 *  +-----------------------+------------------+-----------------------+
 *  |     4x4 mm QFN        |     10           |   IOID_9              |
 *  +-----------------------+------------------+-----------------------+
 *  |     2.7 x 2.7 mm WCSP |     14           |   IOID_13             |
 *  +-----------------------+------------------+-----------------------+
 *  ============================================================================
 */

#include <DASN_Board.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc26x0r2/driverlib/ioc.h>
#include <ti/devices/cc26x0r2/driverlib/udma.h>
#include <ti/devices/cc26x0r2/inc/hw_ints.h>
#include <ti/devices/cc26x0r2/inc/hw_memmap.h>

#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h> // Dejo este include sino algo de crypto no lo reconoce
/*
ADCCC26XX_Object adcCC26xxObjects[DASN_ADCCOUNT];

const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[DASN_ADCCOUNT] = {
    {
        .adcDIO              = DASN_DIO23_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO7,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO24_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO6,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO25_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO26_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO4,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO27_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO3,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO28_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO2,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO29_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO1,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = DASN_DIO30_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO0,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VSS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VDDS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = false
    }
};

const ADC_Config ADC_config[DASN_ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC0], &adcCC26xxHWAttrs[DASN_ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC1], &adcCC26xxHWAttrs[DASN_ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC2], &adcCC26xxHWAttrs[DASN_ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC3], &adcCC26xxHWAttrs[DASN_ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC4], &adcCC26xxHWAttrs[DASN_ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC5], &adcCC26xxHWAttrs[DASN_ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC6], &adcCC26xxHWAttrs[DASN_ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADC7], &adcCC26xxHWAttrs[DASN_ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADCDCOUPL], &adcCC26xxHWAttrs[DASN_ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADCVSS], &adcCC26xxHWAttrs[DASN_ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[DASN_ADCVDDS], &adcCC26xxHWAttrs[DASN_ADCVDDS]},
};

const uint_least8_t ADC_count = DASN_ADCCOUNT;
*/

/*
 *  =============================== Crypto ===============================
 */

#include <ti/drivers/crypto/CryptoCC26XX.h>

CryptoCC26XX_Object cryptoCC26XXObjects[DASN_CRYPTOCOUNT];

const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[DASN_CRYPTOCOUNT] = {
    {
        .baseAddr       = CRYPTO_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_CRYPTO,
        .intNum         = INT_CRYPTO_RESULT_AVAIL_IRQ,
        .intPriority    = ~0,
    }
};

const CryptoCC26XX_Config CryptoCC26XX_config[DASN_CRYPTOCOUNT] = {
    {
         .object  = &cryptoCC26XXObjects[DASN_CRYPTO0],
         .hwAttrs = &cryptoCC26XXHWAttrs[DASN_CRYPTO0]
    },
};

/*
 *  =============================== AESCCM ===============================
 */
#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMCC26XX.h>

AESCCMCC26XX_Object aesccmCC26XXObjects[DASN_AESCCMCOUNT];

const AESCCMCC26XX_HWAttrs aesccmCC26XXHWAttrs[DASN_AESCCMCOUNT] = {
    {
        .intPriority       = ~0,
    }
};

const AESCCM_Config AESCCM_config[DASN_AESCCMCOUNT] = {
    {
         .object  = &aesccmCC26XXObjects[DASN_AESCCM0],
         .hwAttrs = &aesccmCC26XXHWAttrs[DASN_AESCCM0]
    },
};

const uint_least8_t AESCCM_count = DASN_AESCCMCOUNT;


/*
 *  =============================== AESGCM ===============================
 */
#include <ti/drivers/AESGCM.h>
#include <ti/drivers/aesgcm/AESGCMCC26XX.h>

AESGCMCC26XX_Object aesgcmCC26XXObjects[DASN_AESGCMCOUNT];

const AESGCMCC26XX_HWAttrs aesgcmCC26XXHWAttrs[DASN_AESGCMCOUNT] = {
    {
        .intPriority       = ~0,
    }
};

const AESGCM_Config AESGCM_config[DASN_AESGCMCOUNT] = {
    {
         .object  = &aesgcmCC26XXObjects[DASN_AESGCM0],
         .hwAttrs = &aesgcmCC26XXHWAttrs[DASN_AESGCM0]
    },
};

const uint_least8_t AESGCM_count = DASN_AESGCMCOUNT;

/*
 *  =============================== AESCBC ===============================
 */
#include <ti/drivers/AESCBC.h>
#include <ti/drivers/aescbc/AESCBCCC26XX.h>

AESCBCCC26XX_Object aescbcCC26XXObjects[DASN_AESCBCCOUNT];

const AESCBCCC26XX_HWAttrs aescbcCC26XXHWAttrs[DASN_AESCBCCOUNT] = {
    {
        .intPriority       = ~0,
    }
};

const AESCBC_Config AESCBC_config[DASN_AESCBCCOUNT] = {
    {
         .object  = &aescbcCC26XXObjects[DASN_AESCBC0],
         .hwAttrs = &aescbcCC26XXHWAttrs[DASN_AESCBC0]
    },
};

const uint_least8_t AESCBC_count = DASN_AESCBCCOUNT;

/*
 *  =============================== AESCTR ===============================
 */
#include <ti/drivers/AESCTR.h>
#include <ti/drivers/aesctr/AESCTRCC26XX.h>

AESCTRCC26XX_Object aesctrCC26XXObjects[DASN_AESCTRCOUNT];

const AESCTRCC26XX_HWAttrs aesctrCC26XXHWAttrs[DASN_AESCTRCOUNT] = {
    {
        .intPriority       = ~0,
    }
};

const AESCTR_Config AESCTR_config[DASN_AESCTRCOUNT] = {
    {
         .object  = &aesctrCC26XXObjects[DASN_AESCTR0],
         .hwAttrs = &aesctrCC26XXHWAttrs[DASN_AESCTR0]
    },
};

const uint_least8_t AESCTR_count = DASN_AESCTRCOUNT;

/*
 *  =============================== AESECB ===============================
 */
#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBCC26XX.h>

AESECBCC26XX_Object aesecbCC26XXObjects[DASN_AESECBCOUNT];

const AESECBCC26XX_HWAttrs aesecbCC26XXHWAttrs[DASN_AESECBCOUNT] = {
    {
        .intPriority       = ~0,
    }
};

const AESECB_Config AESECB_config[DASN_AESECBCOUNT] = {
    {
         .object  = &aesecbCC26XXObjects[DASN_AESECB0],
         .hwAttrs = &aesecbCC26XXHWAttrs[DASN_AESECB0]
    },
};

const uint_least8_t AESECB_count = DASN_AESECBCOUNT;

/*
 *  =============================== AESCTRDRBG ===============================
 */
#include <ti/drivers/AESCTRDRBG.h>
#include <ti/drivers/aesctrdrbg/AESCTRDRBGXX.h>

AESCTRDRBGXX_Object aesctrdrbgXXObjects[DASN_AESCTRDRBGCOUNT];

const AESCTRDRBGXX_HWAttrs aesctrdrbgXXHWAttrs[DASN_AESCTRDRBGCOUNT] = {
    {
        .aesctrIndex       = DASN_AESCTR0,
    }
};

const AESCTRDRBG_Config AESCTRDRBG_config[DASN_AESCTRDRBGCOUNT] = {
    {
         .object  = &aesctrdrbgXXObjects[DASN_AESCTRDRBG0],
         .hwAttrs = &aesctrdrbgXXHWAttrs[DASN_AESCTRDRBG0]
    },
};

const uint_least8_t AESCTRDRBG_count = DASN_AESCTRDRBGCOUNT;

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in DASN.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array. Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    GPIOCC26XX_DIO_04 | GPIO_DO_NOT_CONFIG, /* PULSADOR */
    GPIOCC26XX_DIO_07 | GPIO_DO_NOT_CONFIG, /* TRIG_IN */
    GPIOCC26XX_DIO_30 | GPIO_DO_NOT_CONFIG, /* RX_CC2640 */
    GPIOCC26XX_DIO_19 | GPIO_DO_NOT_CONFIG, /* ADS1299_DRDY */
    GPIOCC26XX_DIO_20 | GPIO_DO_NOT_CONFIG, /* ADS1299_MISO */
    GPIOCC26XX_DIO_27 | GPIO_DO_NOT_CONFIG, /* ADS1299_MOSI */
    GPIOCC26XX_DIO_05 | GPIO_DO_NOT_CONFIG, /* LED1 */
    GPIOCC26XX_DIO_06 | GPIO_DO_NOT_CONFIG, /* LED2 */
    GPIOCC26XX_DIO_08 | GPIO_DO_NOT_CONFIG, /* TRIG_OUT*/
    GPIOCC26XX_DIO_10 | GPIO_DO_NOT_CONFIG, /* BAT_SENSE */
    GPIOCC26XX_DIO_11 | GPIO_DO_NOT_CONFIG, /* EN_RE485*/
    GPIOCC26XX_DIO_12 | GPIO_DO_NOT_CONFIG, /* EN_DE485 */
    GPIOCC26XX_DIO_13 | GPIO_DO_NOT_CONFIG, /* EN_plusV*/
    GPIOCC26XX_DIO_14 | GPIO_DO_NOT_CONFIG, /* Zs_plusPE*/
    GPIOCC26XX_DIO_15 | GPIO_DO_NOT_CONFIG, /* Zs_minusPE*/
    GPIOCC26XX_DIO_18 | GPIO_DO_NOT_CONFIG, /* ADS1299_CLKSEL*/
    GPIOCC26XX_DIO_21 | GPIO_DO_NOT_CONFIG, /* ADS1299_SCLK */
    GPIOCC26XX_DIO_22 | GPIO_DO_NOT_CONFIG, /* ADS1299_CS */
    GPIOCC26XX_DIO_23 | GPIO_DO_NOT_CONFIG, /* ADS1299_START */
    GPIOCC26XX_DIO_24 | GPIO_DO_NOT_CONFIG, /* ADS1299_CLK */
    GPIOCC26XX_DIO_25 | GPIO_DO_NOT_CONFIG, /* ADS1299_RESET */
    GPIOCC26XX_DIO_26 | GPIO_DO_NOT_CONFIG, /* ADS1299_PWDN */
    GPIOCC26XX_DIO_28 | GPIO_DO_NOT_CONFIG, /* SHDWN_minusV*/
    GPIOCC26XX_DIO_29 | GPIO_DO_NOT_CONFIG, /* TX_CC2640 */

};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC2640R2_LAUNCH.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* PULSADOR */
    NULL,  /* TRIG_IN */
    NULL,  /* RX_CC2640 */
    NULL,  /* ADS1299_DRDY */
};

const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs         = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks          = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = DASN_GPIOCOUNT,
    .numberOfCallbacks  = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority        = (~0)
};

/*
 *  =============================== GPTimer ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/timer/GPTimerCC26XX.h>

GPTimerCC26XX_Object gptimerCC26XXObjects[DASN_GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[DASN_GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1A, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1B, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2A, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2B, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3A, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3B, },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[DASN_GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[DASN_GPTIMER0], &gptimerCC26xxHWAttrs[DASN_GPTIMER0A], GPT_A },
    { &gptimerCC26XXObjects[DASN_GPTIMER0], &gptimerCC26xxHWAttrs[DASN_GPTIMER0B], GPT_B },
    { &gptimerCC26XXObjects[DASN_GPTIMER1], &gptimerCC26xxHWAttrs[DASN_GPTIMER1A], GPT_A },
    { &gptimerCC26XXObjects[DASN_GPTIMER1], &gptimerCC26xxHWAttrs[DASN_GPTIMER1B], GPT_B },
    { &gptimerCC26XXObjects[DASN_GPTIMER2], &gptimerCC26xxHWAttrs[DASN_GPTIMER2A], GPT_A },
    { &gptimerCC26XXObjects[DASN_GPTIMER2], &gptimerCC26xxHWAttrs[DASN_GPTIMER2B], GPT_B },
    { &gptimerCC26XXObjects[DASN_GPTIMER3], &gptimerCC26xxHWAttrs[DASN_GPTIMER3A], GPT_A },
    { &gptimerCC26XXObjects[DASN_GPTIMER3], &gptimerCC26xxHWAttrs[DASN_GPTIMER3B], GPT_B },
};

/*
 *  =============================== I2C ===============================
*/
/*
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

I2CCC26XX_Object i2cCC26xxObjects[DASN_I2CCOUNT];

const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[DASN_I2CCOUNT] = {
    {
        .baseAddr    = I2C0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_I2C0,
        .intNum      = INT_I2C_IRQ,
        .intPriority = ~0,
        .swiPriority = 0,
        .sdaPin      = DASN_I2C0_SDA0,
        .sclPin      = DASN_I2C0_SCL0,
    }
};

const I2C_Config I2C_config[DASN_I2CCOUNT] = {
    {
        .fxnTablePtr = &I2CCC26XX_fxnTable,
        .object      = &i2cCC26xxObjects[DASN_I2C0],
        .hwAttrs     = &i2cCC26xxHWAttrs[DASN_I2C0]
    },
};

const uint_least8_t I2C_count = DASN_I2CCOUNT;
*/

/*
 *  =============================== I2S ===============================
*/
/*
#include <ti/drivers/I2S.h>
#include <ti/drivers/i2s/I2SCC26XX.h>

I2SCC26XX_Object i2sCC26XXObjects[DASN_I2SCOUNT];

const I2SCC26XX_HWAttrs i2sCC26XXHWAttrs[DASN_I2SCOUNT] = {
    {
        .pinSD1      =  DASN_I2S_ADI,
        .pinSD0      =  DASN_I2S_ADO,
        .pinSCK      =  DASN_I2S_BCLK,
        .pinMCLK     =  DASN_I2S_MCLK,
        .pinWS       =  DASN_I2S_WCLK,
        .intPriority = ~0,
    }
};

const I2S_Config I2S_config[DASN_I2SCOUNT] = {
    {
        .object      = &i2sCC26XXObjects[DASN_I2S0],
        .hwAttrs     = &i2sCC26XXHWAttrs[DASN_I2S0]
    },
};

const uint_least8_t I2S_count = DASN_I2SCOUNT;
*/

/*
 *  =============================== PIN ===============================
 */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {

    DASN_PULSADOR | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,          /* Normal Abierto, pull up activado */
    DASN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,           /* LED initially off */
    DASN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,           /* LED initially off */
    DASN_TRIG_IN | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,           /* TRIG_IN normal alto, pullup externo*/
//    DASN_TRIG_OUT | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,       /* TRIG_OUT initially off */
//    DASN_BAT_SENSE | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS | PIN_HYSTERESIS,
    DASN_EN_RE485 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_EN_DE485 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
    DASN_TX_CC2640 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_RX_CC2640 | PIN_INPUT_EN | PIN_PULLDOWN,
    DASN_EN_plusV | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_SHDN_minusV | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
//    DASN_Zs_plusPE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
//    DASN_Zs_minusPE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_ADS1299_CLKSEL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
    DASN_ADS1299_DRDY | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_BOTHEDGES,
    DASN_ADS1299_MISO | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_BOTHEDGES,
    DASN_ADS1299_SCLK | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL |PIN_DRVSTR_MAX,
    DASN_ADS1299_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_ADS1299_START | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
//    DASN_ADS1299_CLK | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
    DASN_ADS1299_RESET | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
    DASN_ADS1299_PWDN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
    DASN_ADS1299_MOSI | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,

    PIN_TERMINATE
};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = true,
#ifdef USE_RCOSC
    .calibrateRCOSC_LF  = true,
#else
    .calibrateRCOSC_LF  = false,
#endif
    .calibrateRCOSC_HF  = true,
};

/*
 *  =============================== PWM ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
/*
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC26XX.h>

PWMTimerCC26XX_Object pwmtimerCC26xxObjects[DASN_PWMCOUNT];

const PWMTimerCC26XX_HwAttrs pwmtimerCC26xxHWAttrs[DASN_PWMCOUNT] = {
    { .pwmPin = DASN_PWMPIN0, .gpTimerUnit = DASN_GPTIMER0A },
    { .pwmPin = DASN_PWMPIN1, .gpTimerUnit = DASN_GPTIMER0B },
    { .pwmPin = DASN_PWMPIN2, .gpTimerUnit = DASN_GPTIMER1A },
    { .pwmPin = DASN_PWMPIN3, .gpTimerUnit = DASN_GPTIMER1B },
    { .pwmPin = DASN_PWMPIN4, .gpTimerUnit = DASN_GPTIMER2A },
    { .pwmPin = DASN_PWMPIN5, .gpTimerUnit = DASN_GPTIMER2B },
    { .pwmPin = DASN_PWMPIN6, .gpTimerUnit = DASN_GPTIMER3A },
    { .pwmPin = DASN_PWMPIN7, .gpTimerUnit = DASN_GPTIMER3B },
};

const PWM_Config PWM_config[DASN_PWMCOUNT] = {
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM0], &pwmtimerCC26xxHWAttrs[DASN_PWM0] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM1], &pwmtimerCC26xxHWAttrs[DASN_PWM1] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM2], &pwmtimerCC26xxHWAttrs[DASN_PWM2] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM3], &pwmtimerCC26xxHWAttrs[DASN_PWM3] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM4], &pwmtimerCC26xxHWAttrs[DASN_PWM4] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM5], &pwmtimerCC26xxHWAttrs[DASN_PWM5] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM6], &pwmtimerCC26xxHWAttrs[DASN_PWM6] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[DASN_PWM7], &pwmtimerCC26xxHWAttrs[DASN_PWM7] },
};

const uint_least8_t PWM_count = DASN_PWMCOUNT;
*/

/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/rf/RF.h>

const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = ~0,       /* Lowest HWI priority */
    .swiPriority        = 0,        /* Lowest SWI priority */
    .xoscHfAlwaysNeeded = true,     /* Keep XOSC dependency while in standby */
    .globalCallback     = NULL,     /* No board specific callback */
    .globalEventMask    = 0         /* No events subscribed to */
};


/*
 *  =============================== SPI DMA ===============================
 */

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>

SPICC26XXDMA_Object spiCC26XXDMAObjects[DASN_SPICOUNT];

/*
 * NOTE: The SPI instances below can be used by the SD driver to communicate
 * with a SD card via SPI.  The 'defaultTxBufValue' fields below are set to 0xFF
 * to satisfy the SDSPI driver requirement.
 */

const SPICC26XXDMA_HWAttrsV1 spiCC26XXDMAHWAttrs[DASN_SPICOUNT] = {
    {
        .baseAddr           = SSI0_BASE,
        .intNum             = INT_SSI0_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue  = 0xFF,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin            = DASN_ADS1299_MOSI,
        .misoPin            = DASN_ADS1299_MISO,
        .clkPin             = DASN_ADS1299_SCLK,
        .csnPin             = DASN_ADS1299_CS,
        .minDmaTransferSize = 10
    }
};

const SPI_Config SPI_config[DASN_SPICOUNT] = {
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[DASN_SPI0],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[DASN_SPI0]
    }
};

const uint_least8_t SPI_count = DASN_SPICOUNT;


/*
 *  =============================== UART ===============================
 */

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

UARTCC26XX_Object uartCC26XXObjects[DASN_UARTCOUNT];

uint8_t uartCC26XXRingBuffer[DASN_UARTCOUNT][32];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[DASN_UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = DASN_TX_CC2640,
        .rxPin          = DASN_RX_CC2640,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED,
        .ringBufPtr     = uartCC26XXRingBuffer[DASN_UART0],
        .ringBufSize    = sizeof(uartCC26XXRingBuffer[DASN_UART0]),
        .txIntFifoThr   = UARTCC26XX_FIFO_THRESHOLD_1_8,
        .rxIntFifoThr   = UARTCC26XX_FIFO_THRESHOLD_4_8,
        .errorFxn       = NULL
    }
};

const UART_Config UART_config[DASN_UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[DASN_UART0],
        .hwAttrs     = &uartCC26XXHWAttrs[DASN_UART0]
    },
};

const uint_least8_t UART_count = DASN_UARTCOUNT;

/*
 *  =============================== UDMA ===============================
 */
#include <ti/drivers/dma/UDMACC26XX.h>

UDMACC26XX_Object udmaObjects[DASN_UDMACOUNT];

const UDMACC26XX_HWAttrs udmaHWAttrs[DASN_UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

const UDMACC26XX_Config UDMACC26XX_config[DASN_UDMACOUNT] = {
    {
         .object  = &udmaObjects[DASN_UDMA0],
         .hwAttrs = &udmaHWAttrs[DASN_UDMA0]
    },
};



/*
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

WatchdogCC26XX_Object watchdogCC26XXObjects[DASN_WATCHDOGCOUNT];

const WatchdogCC26XX_HWAttrs watchdogCC26XXHWAttrs[DASN_WATCHDOGCOUNT] = {
    {
        .baseAddr    = WDT_BASE,
        .reloadValue = 1000 /* Reload value in milliseconds */
    },
};

const Watchdog_Config Watchdog_config[DASN_WATCHDOGCOUNT] = {
    {
        .fxnTablePtr = &WatchdogCC26XX_fxnTable,
        .object      = &watchdogCC26XXObjects[DASN_WATCHDOG0],
        .hwAttrs     = &watchdogCC26XXHWAttrs[DASN_WATCHDOG0]
    },
};

const uint_least8_t Watchdog_count = DASN_WATCHDOGCOUNT;

/*
 *  ========================= TRNG begin ====================================
 */
#include <TRNGCC26XX.h>

/* TRNG objects */
TRNGCC26XX_Object trngCC26XXObjects[DASN_TRNGCOUNT];

/* TRNG configuration structure, describing which pins are to be used */
const TRNGCC26XX_HWAttrs TRNGCC26XXHWAttrs[DASN_TRNGCOUNT] = {
    {
        .powerMngrId    = PowerCC26XX_PERIPH_TRNG,
    }
};

/* TRNG configuration structure */
const TRNGCC26XX_Config TRNGCC26XX_config[] = {
    {
         .object  = &trngCC26XXObjects[0],
         .hwAttrs = &TRNGCC26XXHWAttrs[0]
    },
    {NULL, NULL}
};

/*
 *  ========================= TRNG end ====================================
 */
extern void Board_initHook(void);

/*
 *  ======== DASN_initGeneral ========
 */
void DASN_initGeneral(void)
{
    Power_init();

    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        /* Error with PIN_init */
        while (1);
    }

}

/*
 *  ======== Board_init ========
 */
void Board_init(void)
{
    DASN_initGeneral();
}
