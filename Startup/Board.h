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

#ifndef __BOARD_H
#define __BOARD_H

#define Board_DASN

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drivers/ADC.h>
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

#include <ti/drivers/Board.h>

#define Board_initGeneral()     Board_init()  /* deprecated */

#include "DASN.h"

#define Board_shutDownExtFlash() DASN_shutDownExtFlash()
#define Board_wakeUpExtFlash() DASN_wakeUpExtFlash()

/* These #defines allow us to reuse TI-RTOS across other device families */

#define Board_ADC0              DASN_ADC0
#define Board_ADC1              DASN_ADC1

#define Board_ADCBUF0           DASN_ADCBUF0
#define Board_ADCBUF0CHANNEL0   DASN_ADCBUF0CHANNEL0
#define Board_ADCBUF0CHANNEL1   DASN_ADCBUF0CHANNEL1

#define Board_CRYPTO0           DASN_CRYPTO0
#define Board_AESCCM0           DASN_AESCCM0
#define Board_AESGCM0           DASN_AESGCM0
#define Board_AESCBC0           DASN_AESCBC0
#define Board_AESCTR0           DASN_AESCTR0
#define Board_AESECB0           DASN_AESECB0
#define Board_AESCTRDRBG0       DASN_AESCTRDRBG0

#define Board_DIO0              DASN_DIO0
#define Board_DIO1_RFSW         DASN_DIO1_RFSW
#define Board_DIO12             DASN_DIO12
#define Board_DIO15             DASN_DIO15
#define Board_DIO16_TDO         DASN_DIO16_TDO
#define Board_DIO17_TDI         DASN_DIO17_TDI
#define Board_DIO21             DASN_DIO21
#define Board_DIO22             DASN_DIO22

#define Board_DIO23_ANALOG      DASN_DIO23_ANALOG
#define Board_DIO24_ANALOG      DASN_DIO24_ANALOG
#define Board_DIO25_ANALOG      DASN_DIO25_ANALOG
#define Board_DIO26_ANALOG      DASN_DIO26_ANALOG
#define Board_DIO27_ANALOG      DASN_DIO27_ANALOG
#define Board_DIO28_ANALOG      DASN_DIO28_ANALOG
#define Board_DIO29_ANALOG      DASN_DIO29_ANALOG
#define Board_DIO30_ANALOG      DASN_DIO30_ANALOG

#define Board_GPIO_BUTTON0      DASN_GPIO_S1
#define Board_GPIO_BUTTON1      DASN_GPIO_S2
#define Board_GPIO_BTN1         DASN_GPIO_S1
#define Board_GPIO_BTN2         DASN_GPIO_S2
#define Board_GPIO_LED0         DASN_GPIO_LED_RED
#define Board_GPIO_LED1         DASN_GPIO_LED_GREEN
#define Board_GPIO_LED2         DASN_GPIO_LED_RED
#define Board_GPIO_RLED         DASN_GPIO_LED_RED
#define Board_GPIO_GLED         DASN_GPIO_LED_GREEN
#define Board_GPIO_LED_ON       DASN_GPIO_LED_ON
#define Board_GPIO_LED_OFF      DASN_GPIO_LED_OFF
#define Board_GPIO_TMP116_EN    DASN_GPIO_TMP116_EN

#define Board_GPTIMER0A         DASN_GPTIMER0A
#define Board_GPTIMER0B         DASN_GPTIMER0B
#define Board_GPTIMER1A         DASN_GPTIMER1A
#define Board_GPTIMER1B         DASN_GPTIMER1B
#define Board_GPTIMER2A         DASN_GPTIMER2A
#define Board_GPTIMER2B         DASN_GPTIMER2B
#define Board_GPTIMER3A         DASN_GPTIMER3A
#define Board_GPTIMER3B         DASN_GPTIMER3B

#define Board_I2C0              DASN_I2C0
#define Board_I2C_TMP           Board_I2C0

#define Board_I2S0              DASN_I2S0
#define Board_I2S_ADO           DASN_I2S_ADO
#define Board_I2S_ADI           DASN_I2S_ADI
#define Board_I2S_BCLK          DASN_I2S_BCLK
#define Board_I2S_MCLK          DASN_I2S_MCLK
#define Board_I2S_WCLK          DASN_I2S_WCLK

#define Board_NVSINTERNAL       DASN_NVSCC26XX0
#define Board_NVSEXTERNAL       DASN_NVSSPI25X0

#define Board_PIN_BUTTON0       DASN_PIN_BTN1
#define Board_PIN_BUTTON1       DASN_PIN_BTN2
#define Board_PIN_BTN1          DASN_PIN_BTN1
#define Board_PIN_BTN2          DASN_PIN_BTN2
#define Board_PIN_LED0          DASN_PIN_RLED
#define Board_PIN_LED1          DASN_PIN_GLED
#define Board_PIN_LED2          DASN_PIN_RLED
#define Board_PIN_RLED          DASN_PIN_RLED
#define Board_PIN_GLED          DASN_PIN_GLED

#define Board_PWM0              DASN_PWM0
#define Board_PWM1              DASN_PWM1
#define Board_PWM2              DASN_PWM2
#define Board_PWM3              DASN_PWM3
#define Board_PWM4              DASN_PWM4
#define Board_PWM5              DASN_PWM5
#define Board_PWM6              DASN_PWM6
#define Board_PWM7              DASN_PWM7

#define Board_SD0               DASN_SDSPI0

#define Board_SPI0              DASN_SPI0
#define Board_SPI1              DASN_SPI1
#define Board_SPI_FLASH_CS      DASN_SPI_FLASH_CS
#define Board_FLASH_CS_ON       0
#define Board_FLASH_CS_OFF      1

#define Board_SPI_MASTER        DASN_SPI0
#define Board_SPI_SLAVE         DASN_SPI0
#define Board_SPI_MASTER_READY  DASN_SPI_MASTER_READY
#define Board_SPI_SLAVE_READY   DASN_SPI_SLAVE_READY
#define Board_UART0             DASN_UART0

#define Board_WATCHDOG0         DASN_WATCHDOG0

/*
 * These macros are provided for backwards compatibility.
 * Please use the <Driver>_init functions directly rather
 * than Board_init<Driver>.
 */
#define Board_initADC()         ADC_init()
#define Board_initADCBuf()      ADCBuf_init()
#define Board_initGPIO()        GPIO_init()
#define Board_initPWM()         PWM_init()
#define Board_initSPI()         SPI_init()
#define Board_initUART()        UART_init()
#define Board_initWatchdog()    Watchdog_init()

/*
 * These macros are provided for backwards compatibility.
 * Please use the 'Board_PIN_xxx' macros to differentiate
 * them from the 'Board_GPIO_xxx' macros.
 */
#define Board_BUTTON0           Board_PIN_BUTTON0
#define Board_BUTTON1           Board_PIN_BUTTON1
#define Board_BTN1              Board_PIN_BTN1
#define Board_BTN2              Board_PIN_BTN2
#define Board_LED_ON            Board_GPIO_LED_ON
#define Board_LED_OFF           Board_GPIO_LED_OFF
#define Board_LED0              Board_PIN_LED0
#define Board_LED1              Board_PIN_LED1
#define Board_LED2              Board_PIN_LED2
#define Board_RLED              Board_PIN_RLED
#define Board_GLED              Board_PIN_GLED
#define Board_ADCBUFCHANNEL0    Board_ADCBUF0CHANNEL0
#define Board_ADCBUFCHANNEL1    Board_ADCBUF0CHANNEL1

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
