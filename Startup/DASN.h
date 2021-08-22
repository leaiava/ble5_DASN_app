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
/** ============================================================================
 *  @file       DASN.h
 *
 *  @brief      CC2640R2 LaunchPad Board Specific header file.
 *
 *  This file is responsible for setting up the board specific items for the
 *  DASN board.
 *
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
#ifndef __DASN_BOARD_H__
#define __DASN_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc26x0r2/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#ifndef DASN
  #define DASN
#endif /* DASN */

/*
 *  ============================================================================
 *  RF Front End and Bias configuration symbols for TI reference designs and
 *  kits. This symbol sets the RF Front End configuration in ble_user_config.h
 *  and selects the appropriate PA table in ble_user_config.c.
 *  Other configurations can be used by editing these files.
 *
 *  Define only one symbol:
 *  CC2650EM_7ID    - Differential RF and internal biasing
                      (default for CC2640R2 LaunchPad)
 *  CC2650EM_5XD    – Differential RF and external biasing
 *  CC2650EM_4XS    – Single-ended RF on RF-P and external biasing
 *  CC2640R2DK_CXS  - WCSP: Single-ended RF on RF-N and external biasing
 *                    (Note that the WCSP is only tested and characterized for
 *                     single ended configuration, and it has a WCSP-specific
 *                     PA table)
 *
 *  Note: CC2650EM_xxx reference designs apply to all CC26xx devices.
 *  ==========================================================================
 */
#define CC2650EM_7ID

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                  <pin mapping>
 */

/* Analog Capable DIOs */
#define DASN_DIO23_ANALOG          IOID_23
#define DASN_DIO24_ANALOG          IOID_24
#define DASN_DIO25_ANALOG          IOID_25
#define DASN_DIO26_ANALOG          IOID_26
#define DASN_DIO27_ANALOG          IOID_27
#define DASN_DIO28_ANALOG          IOID_28
#define DASN_DIO29_ANALOG          IOID_29
#define DASN_DIO30_ANALOG          IOID_30

/* Digital IOs */
#define DASN_DIO0                  IOID_0
#define DASN_DIO1_RFSW             IOID_1
#define DASN_DIO12                 IOID_12
#define DASN_DIO15                 IOID_15
#define DASN_DIO16_TDO             IOID_16
#define DASN_DIO17_TDI             IOID_17
#define DASN_DIO21                 IOID_21
#define DASN_DIO22                 IOID_22

/* Discrete Inputs */
#define DASN_PIN_BTN1              IOID_13
#define DASN_PIN_BTN2              IOID_14

/* GPIO */
#define DASN_GPIO_LED_ON           1
#define DASN_GPIO_LED_OFF          0

/* I2C */
#define DASN_I2C0_SCL0             IOID_4
#define DASN_I2C0_SDA0             IOID_5

/* I2S */
#define DASN_I2S_ADO               IOID_0
#define DASN_I2S_ADI               IOID_1
#define DASN_I2S_BCLK              IOID_30
#define DASN_I2S_MCLK              PIN_UNASSIGNED
#define DASN_I2S_WCLK              IOID_29

/* LCD (430BOOST - Sharp96 Rev 1.1) */
#define DASN_LCD_CS                IOID_24 /* SPI chip select */
#define DASN_LCD_EXTCOMIN          IOID_12 /* External COM inversion */
#define DASN_LCD_ENABLE            IOID_22 /* LCD enable */
#define DASN_LCD_POWER             IOID_23 /* LCD power control */
#define DASN_LCD_CS_ON             1
#define DASN_LCD_CS_OFF            0

/* LEDs */
#define DASN_PIN_LED_ON            1
#define DASN_PIN_LED_OFF           0
#define DASN_PIN_RLED              IOID_6
#define DASN_PIN_GLED              IOID_7

/* PWM Outputs */
#define DASN_PWMPIN0               DASN_PIN_RLED
#define DASN_PWMPIN1               DASN_PIN_GLED
#define DASN_PWMPIN2               PIN_UNASSIGNED
#define DASN_PWMPIN3               PIN_UNASSIGNED
#define DASN_PWMPIN4               PIN_UNASSIGNED
#define DASN_PWMPIN5               PIN_UNASSIGNED
#define DASN_PWMPIN6               PIN_UNASSIGNED
#define DASN_PWMPIN7               PIN_UNASSIGNED

/* SPI */
#define DASN_SPI_FLASH_CS          IOID_20
#define DASN_FLASH_CS_ON           0
#define DASN_FLASH_CS_OFF          1

/* SPI Board */
#define DASN_SPI0_MISO             IOID_8          /* RF1.20 */
#define DASN_SPI0_MOSI             IOID_9          /* RF1.18 */
#define DASN_SPI0_CLK              IOID_10         /* RF1.16 */
#define DASN_SPI0_CSN              PIN_UNASSIGNED
#define DASN_SPI1_MISO             PIN_UNASSIGNED
#define DASN_SPI1_MOSI             PIN_UNASSIGNED
#define DASN_SPI1_CLK              PIN_UNASSIGNED
#define DASN_SPI1_CSN              PIN_UNASSIGNED

/* UART Board */
#define DASN_UART_RX               IOID_2          /* RXD */
#define DASN_UART_TX               IOID_3          /* TXD */
#define DASN_UART_CTS              IOID_19         /* CTS */
#define DASN_UART_RTS              IOID_18         /* RTS */

/* PLACA DASN */
#define DASN_PULSADOR              IOID_4

#define DASN_LED1                  IOID_5
#define DASN_LED2                  IOID_6

#define DASN_TRIG_IN               IOID_7
#define DASN_TRIG_OUT              IOID_8

#define DASN_BAT_SENSE             IOID_10      /* Conflicto, esta entrada no es analógica */

#define DASN_EN_RE485              IOID_11
#define DASN_EN_DE485              IOID_12
#define DASN_TX_CC2640             IOID_29
#define DASN_RX_CC2640             IOID_30

#define DASN_EN_plusV              IOID_13
#define DASN_SHDN_minusV           IOID_28

#define DASN_Zs_plusPE             IOID_14
#define DASN_Zs_minusPE            IOID_15

#define DASN_ADS1299_CLKSEL        IOID_18
#define DASN_ADS1299_DRDY          IOID_19
#define DASN_ADS1299_MISO          IOID_20
#define DASN_ADS1299_SCLK          IOID_21
#define DASN_ADS1299_CS            IOID_22
#define DASN_ADS1299_START         IOID_23
#define DASN_ADS1299_CLK           IOID_24
#define DASN_ADS1299_RESET         IOID_25
#define DASN_ADS1299_PWDN          IOID_26
#define DASN_ADS1299_MOSI          IOID_27


/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void DASN_initGeneral(void);

/*!
 *  @brief  Turn off the external flash on LaunchPads
 *
 */
void DASN_shutDownExtFlash(void);

/*!
 *  @brief  Wake up the external flash present on the board files
 *
 *  This function toggles the chip select for the amount of time needed
 *  to wake the chip up.
 */
void DASN_wakeUpExtFlash(void);

/*!
 *  @def    DASN_ADCBufName
 *  @brief  Enum of ADCs
 */
typedef enum DASN_ADCBufName {
    DASN_ADCBUF0 = 0,

    DASN_ADCBUFCOUNT
} DASN_ADCBufName;

/*!
 *  @def    DASN_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum DASN_ADCBuf0ChannelName {
    DASN_ADCBUF0CHANNEL0 = 0,
    DASN_ADCBUF0CHANNEL1,
    DASN_ADCBUF0CHANNEL2,
    DASN_ADCBUF0CHANNEL3,
    DASN_ADCBUF0CHANNEL4,
    DASN_ADCBUF0CHANNEL5,
    DASN_ADCBUF0CHANNEL6,
    DASN_ADCBUF0CHANNEL7,
    DASN_ADCBUF0CHANNELVDDS,
    DASN_ADCBUF0CHANNELDCOUPL,
    DASN_ADCBUF0CHANNELVSS,

    DASN_ADCBUF0CHANNELCOUNT
} DASN_ADCBuf0ChannelName;

/*!
 *  @def    DASN_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum DASN_ADCName {
    DASN_ADC0 = 0,
    DASN_ADC1,
    DASN_ADC2,
    DASN_ADC3,
    DASN_ADC4,
    DASN_ADC5,
    DASN_ADC6,
    DASN_ADC7,
    DASN_ADCDCOUPL,
    DASN_ADCVSS,
    DASN_ADCVDDS,

    DASN_ADCCOUNT
} DASN_ADCName;

/*!
 *  @def    DASN_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum DASN_CryptoName {
    DASN_CRYPTO0 = 0,

    DASN_CRYPTOCOUNT
} DASN_CryptoName;

/*!
 *  @def    DASN_AESCCMName
 *  @brief  Enum of AESCCM names
 */
typedef enum DASN_AESCCMName {
    DASN_AESCCM0 = 0,

    DASN_AESCCMCOUNT
} DASN_AESCCMName;

/*!
 *  @def    DASN_AESGCMName
 *  @brief  Enum of AESGCM names
 */
typedef enum DASN_AESGCMName {
    DASN_AESGCM0 = 0,

    DASN_AESGCMCOUNT
} DASN_AESGCMName;

/*!
 *  @def    DASN_AESCBCName
 *  @brief  Enum of AESCBC names
 */
typedef enum DASN_AESCBCName {
    DASN_AESCBC0 = 0,

    DASN_AESCBCCOUNT
} DASN_AESCBCName;

/*!
 *  @def    DASN_AESCTRName
 *  @brief  Enum of AESCTR names
 */
typedef enum DASN_AESCTRName {
    DASN_AESCTR0 = 0,

    DASN_AESCTRCOUNT
} DASN_AESCTRName;

/*!
 *  @def    DASN_AESECBName
 *  @brief  Enum of AESECB names
 */
typedef enum DASN_AESECBName {
    DASN_AESECB0 = 0,

    DASN_AESECBCOUNT
} DASN_AESECBName;

/*!
 *  @def    DASN_AESCTRDRBGName
 *  @brief  Enum of AESCTRDRBG names
 */
typedef enum DASN_AESCTRDRBGName {
    DASN_AESCTRDRBG0 = 0,

    DASN_AESCTRDRBGCOUNT
} DASN_AESCTRDRBGName;

/*!
 *  @def    DASN_GPIOName
 *  @brief  Enum of GPIO names
 */
typedef enum DASN_GPIOName {
    DASN_GPIO_PULSADOR = 0,
    DASN_GPIO_TRIG_IN,
    DASN_UART_RX_CC2640,
    DASN_GPIO_ADS1299_DRDY,
    DASN_GPIO_ADS1299_MISO,
    DASN_GPIO_ADS1299_MOSI,
    DASN_GPIO_LED1,
    DASN_GPIO_LED2,
    DASN_GPIO_TRIG_OUT,
    DASN_GPIO_BAT_SENSE,
    DASN_UART_EN_RE485,
    DASN_UART_EN_DE485,
    DASN_GPIO_EN_plusV,
    DASN_GPIO_ZS_plusPE,
    DASN_GPIO_ZS_minusPE,
    DASN_GPIO_ADS1299_CLKSEL,
    DASN_GPIO_ADS1299_SCLK,
    DASN_GPIO_ADS1299_CS,
    DASN_GPIO_ADS1299_START,
    DASN_GPIO_ADS1299_CLK,
    DASN_GPIO_ADS1299_RESET,
    DASN_GPIO_ADS1299_PWDN,
    DASN_GPIO_SHDN_minusV,
    DASN_UART_TX_CC2640,

    DASN_GPIO_LCD_CS,
    DASN_GPIO_LCD_POWER,
    DASN_GPIO_LCD_ENABLE,
    DASN_GPIO_SPI_FLASH_CS,
    DASN_SDSPI_CS,

    DASN_GPIOCOUNT
} DASN_GPIOName;

/*!
 *  @def    DASN_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum DASN_GPTimerName {
    DASN_GPTIMER0A = 0,
    DASN_GPTIMER0B,
    DASN_GPTIMER1A,
    DASN_GPTIMER1B,
    DASN_GPTIMER2A,
    DASN_GPTIMER2B,
    DASN_GPTIMER3A,
    DASN_GPTIMER3B,

    DASN_GPTIMERPARTSCOUNT
} DASN_GPTimerName;

/*!
 *  @def    DASN_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum DASN_GPTimers {
    DASN_GPTIMER0 = 0,
    DASN_GPTIMER1,
    DASN_GPTIMER2,
    DASN_GPTIMER3,

    DASN_GPTIMERCOUNT
} DASN_GPTimers;

/*!
 *  @def    DASN_I2CName
 *  @brief  Enum of I2C names
 */
typedef enum DASN_I2CName {
    DASN_I2C0 = 0,

    DASN_I2CCOUNT
} DASN_I2CName;

/*!
 *  @def    DASN_I2SName
 *  @brief  Enum of I2S names
 */
typedef enum DASN_I2SName {
    DASN_I2S0 = 0,

    DASN_I2SCOUNT
} DASN_I2SName;

/*!
 *  @def    DASN_NVSName
 *  @brief  Enum of NVS names
 */
typedef enum DASN_NVSName {
#ifndef Board_EXCLUDE_NVS_INTERNAL_FLASH
    DASN_NVSCC26XX0 = 0,
#endif
#ifndef Board_EXCLUDE_NVS_EXTERNAL_FLASH
    DASN_NVSSPI25X0,
#endif

    DASN_NVSCOUNT
} DASN_NVSName;

/*!
 *  @def    DASN_PWM
 *  @brief  Enum of PWM outputs
 */
typedef enum DASN_PWMName {
    DASN_PWM0 = 0,
    DASN_PWM1,
    DASN_PWM2,
    DASN_PWM3,
    DASN_PWM4,
    DASN_PWM5,
    DASN_PWM6,
    DASN_PWM7,

    DASN_PWMCOUNT
} DASN_PWMName;

/*!
 *  @def    DASN_SDName
 *  @brief  Enum of SD names
 */
typedef enum DASN_SDName {
    DASN_SDSPI0 = 0,

    DASN_SDCOUNT
} DASN_SDName;

/*!
 *  @def    DASN_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum DASN_SPIName {
    DASN_SPI0 = 0,
    DASN_SPI1,

    DASN_SPICOUNT
} DASN_SPIName;

/*!
 *  @def    DASN_UARTName
 *  @brief  Enum of UARTs
 */
typedef enum DASN_UARTName {
    DASN_UART0 = 0,

    DASN_UARTCOUNT
} DASN_UARTName;

/*!
 *  @def    DASN_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum DASN_UDMAName {
    DASN_UDMA0 = 0,

    DASN_UDMACOUNT
} DASN_UDMAName;

/*!
 *  @def    DASN_WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum DASN_WatchdogName {
    DASN_WATCHDOG0 = 0,

    DASN_WATCHDOGCOUNT
} DASN_WatchdogName;

/*!
 *  @def    CC2650_LAUNCHXL_TRNGName
 *  @brief  Enum of TRNG names on the board
 */
typedef enum DASN_TRNGName {
    DASN_TRNG0 = 0,
    DASN_TRNGCOUNT
} DASN_TRNGName;

#ifdef __cplusplus
}
#endif

#endif /* __DASN_BOARD_H__ */
