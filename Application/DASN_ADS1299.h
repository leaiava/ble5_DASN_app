
#ifndef DASN_ADS1299_H
#define DASN_ADS1299_H

#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/dma/UDMACC26XX.h>

// Event Handle global para recibir los eventos.

// List of events to manage
#define ADS1299_ALL_EVENTS          0x1F
#define ADS1299_test_event          1

typedef enum {
    ESTADO_STANDBY,             //!< Estado inicial luego del reset
    ESTADO_CONECTADO,           //!< Cuando establece conexión con un dispositivo externo
    ESTADO_CONFIGURANDO,        //!< Estado para configurar cantidad de canales y frecuencia de sampling
    ESTADO_LISTO_PARA_ADQUIRIR, //!< Si tiene configurado una frecuencia de muestreo y canales a usar queda en este estado
    ESTADO_ADQUIRIENDO          //!< Adquiriendo y enviando datos por la UART.
}mis_estados_t;

/*
typedef enum {
    ADS1299_CMD_HOLA = 1,

    DASN_START_ACQ,
    DASN_STOP_ACQ,
    DASN_POWER_OFF,
    DASN_FREC1,             //   65 Hz
    DASN_FREC2,             //  131 Hz
    DASN_FREC3,             //  262 Hz
    DASN_FREC4,             //  524 Hz
    DASN_FREC5,             // 1048 Hz
    DASN_FREC6,             // 2096 Hz
    DASN_FREC7,             // 4193 Hz
    DASN_TRIGGER_OUT1,      //  0,1 ms
    DASN_TRIGGER_OUT2,      //  0,5 ms
    DASN_TRIGGER_OUT3,      //    1 ms
    DASN_TRIGGER_OUT4,      //    5 ms
    DASN_TRIGGER_OUT5,      //   10 ms
    DASN_TRIGGER_IN,        // Trigger IN Enable
    DASN_TRIGGER_DISABLE,   // Trigger module disable
    DASN_Z_SIGNAL1_EN,      //  7,8 Hz
    DASN_Z_SIGNAL2_EN,      // 31,2 Hz
    DASN_Z_SIGNAL_OFF,      // No Impedance signal injected

}DASN_Commands_t;
*/

typedef enum {

    ADS1299_CMD_HOLA = 1,           //!<  1-Iniciar la comunicación.
    ADS1299_CMD_OK,                 //!<  2-Se interpreto bien el comando recibido
    ADS1299_CMD_NO_OK,              //!<  3-No se interpreto el comando recibido
    ADS1299_CMD_CHAU,               //!<  4-Terminar comunicacion
    ADS1299_CMD_CONFIG_INICIAR,     //!<  5-Inicia configuracion
    ADS1299_CMD_CONFIG_TERMINAR,    //!<  6-Termina configuracion
    ADS1299_CMD_CONFIG_CH_ALL_ON,   //!<  7-Todos los canales habilitados
    ADS1299_CMD_CONFIG_CH_ALL_OFF,  //!<  8-Todos los canales deshabilitados
    ADS1299_CMD_CONFIG_CH1_ON,      //!<  9-Canal 1 habilitdo
    ADS1299_CMD_CONFIG_CH2_ON,      //!<  A-Canal 2 habilitdo
    ADS1299_CMD_CONFIG_CH3_ON,      //!<  B-Canal 3 habilitdo
    ADS1299_CMD_CONFIG_CH4_ON,      //!<  C-Canal 4 habilitdo
    ADS1299_CMD_CONFIG_CH5_ON,      //!<  D-Canal 5 habilitdo
    ADS1299_CMD_CONFIG_CH6_ON,      //!<  E-Canal 6 habilitdo
    ADS1299_CMD_CONFIG_CH7_ON,      //!<  F-Canal 7 habilitdo
    ADS1299_CMD_CONFIG_CH8_ON,      //!< 10-Canal 8 habilitdo
    ADS1299_CMD_CONFIG_FREC_1,      //!< 11-Configura ADC con frecuencia 1 (16KHz)
    ADS1299_CMD_CONFIG_FREC_2,      //!< 12-Configura ADC con frecuencia 2 ( 8KHz)
    ADS1299_CMD_CONFIG_FREC_3,      //!< 13-Configura ADC con frecuencia 3 ( 4KHz)
    ADS1299_CMD_CONFIG_FREC_4,      //!< 14-Configura ADC con frecuencia 4 ( 2KHz)
    ADS1299_CMD_CONFIG_FREC_5,      //!< 15-Configura ADC con frecuencia 5 ( 1KHz)
    ADS1299_CMD_CONFIG_FREC_6,      //!< 16-Configura ADC con frecuencia 6 (500Hz)
    ADS1299_CMD_CONFIG_FREC_7,      //!< 17-Configura ADC con frecuencia 7 (250Hz)
    ADS1299_CMD_ADQUIRIR,           //!< 18-Inicia la adquisición
    ADS1299_CMD_PARAR,              //!< 19-Para la adquisición
    ADS1299_CMD_OCUPADO,            //!< 1A-Respuesta si no puede atender un comando
    ADS1299_CMD_LEER_ESTADO,        //!< 1B-Pregunta en que estado se encuentra
    ADS1299_CMD_WAKE_UP,            //!< 1C-Sirve para actualizar la MEF interna del modulo
    ADS1299_CMD_ZSIGNAL_ON,         //!< 1D-Inyecta la señal de impedancia
    ADS1299_CMD_ZSIGNAL_OFF,        //!< 1E-Apaga la señal de impedancia
    ADS1299_CMD_RESET,              //!< 1F

}ADS1299cmd_t;

typedef enum {
    EXTERNAL_CLOCK=0,
    INTERNAL_CLOCK=1
}clkSel_t;

typedef enum {
    ID,
    CONFIG1,
    CONFIG2,
    CONFIG3,
    LOFF,
    CH1SET,
    CH2SET,
    CH3SET,
    CH4SET,
    CH5SET,
    CH6SET,
    CH7SET,
    CH8SET,
    BIAS_SENSP,
    BIAS_SENSN,
    LOFF_SENSP,
    LOFF_SENSN,
    LOFF_FLIP,
    LOFF_STATP,
    LOFF_STATN,
    GPIO,
    MISC1,
    MISC2,
    CONFIG4,
}registers_t;

typedef enum {
    //System Commands
    NULL_CMD    = 0x00,
    WAKEUP_CMD  = 0x02,     /*!< Wake-up from standby mode */
    STANDBY_CMD = 0x04,     /*!< Enter standby mode */
    RESET_CMD   = 0x06,     /*!< Reset the device */
    START_CMD   = 0x08,     /*!< Start and restart (synchronize) conversions */
    STOP_CMD    = 0x0A,     /*!< Stop conversion */

    //Data Read Commands
    RDATAC_CMD  = 0x10,     /*!< Enable Read Data Continuous mode.
                             *  This mode is the default mode at power-up */

    SDATAC_CMD  = 0x11,     /*!< Stop Read Data Continuously mode */
    RDATA_CMD   = 0x12,     /*!< Read data by command; supports multiple read back */

    //Register Read Commands
    RREG_CMD    = 0x20,
    WREG_CMD    = 0x40,
}commands_t;

typedef enum {
    READ_REG_ALL,
    WRITE_REG,
    READ_REG,
    SEND_CMD,
    READ_ALL_CH
}transaction_t;

typedef struct {
    uint8_t scrap1;
    uint8_t scrap2;
    uint8_t scrap3;
    uint8_t id;
    uint8_t config1;
    uint8_t config2;
    uint8_t config3;
    uint8_t loff;
    uint8_t ch1set;
    uint8_t ch2set;
    uint8_t ch3set;
    uint8_t ch4set;
    uint8_t ch5set;
    uint8_t ch6set;
    uint8_t ch7set;
    uint8_t ch8set;
    uint8_t bias_sensp;
    uint8_t bias_sensn;
    uint8_t loff_sensp;
    uint8_t loff_sensn;
    uint8_t loff_flip;
    uint8_t loff_statp;
    uint8_t loff_statn;
    uint8_t gpio;
    uint8_t misc1;
    uint8_t misc2;
    uint8_t config4;
}ads1299_registers_t;

void DASN_ADS1299_createTask(void);

void ads1299_readAllRegisters(void);
void ads1299_readAllChannels(void);
void ads1299_writeRegister(registers_t registro, uint8_t data);
uint8_t ads1299_readRegister(registers_t registro);
void ads1299_sendCommand(commands_t cmd);
void ads1299_startConversion(void);
void ADS1299_STOP(void);

#define DASN_NEW_DATA_EVT       10
#define DASN_TRANSFER_COMPLETE  11
/* Register settings */
/* ------ CONFIG1 settings ------ */
#define ADS1299_CONFIG1_DATA_RATE_250           (0x96)    // 250 SPS
#define ADS1299_CONFIG1_DATA_RATE_500           (0x95)    // 500 SPS
#define ADS1299_CONFIG1_DATA_RATE_1000          (0x94)    // 1 kSPS
#define ADS1299_CONFIG1_DATA_RATE_2000          (0x93)    // 2 kSPS
#define ADS1299_CONFIG1_DATA_RATE_4000          (0x92)    // 4 kSPS
#define ADS1299_CONFIG1_DATA_RATE_8000          (0x91)    // 8 kSPS
#define ADS1299_CONFIG1_DATA_RATE_16000         (0x90)    // 16 kSPS
#define ADS1299_CONFIG1_CLK_OUTPUT_ENABLE       (0xB0)
#define ADS1299_CONFIG1_CLK_OUTPUT_DISABLE      (0x90)
#define ADS1299_CONFIG1_DAISY_CHAIN_MODE        (0x90)
#define ADS1299_CONFIG1_MULT_READBACK_MODE      (0xD0)
#define DATA_RATE_MASK                          (0x07)

/* ------ CONFIG2 settings ------ */
#define ADS1299_CONFIG2_INTERNAL_TEST_SIGNAL    (0xD0)    // Test signals are generated internally
#define ADS1299_CONFIG2_EXTERNAL_TEST_SIGNAL    (0xC0)    // Test signals are driven externally
#define ADS1299_CONFIG2_TEST_SIGNAL_AMP_X1      (0xC0)    // Test signal amplitude 1 × –(VREFP – VREFN) / 2400
#define ADS1299_CONFIG2_TEST_SIGNAL_AMP_X2      (0xC4)    // Test signal amplitude 2 × –(VREFP – VREFN) / 2400
#define ADS1299_CONFIG2_TEST_SIGNAL_FREC_1_95   (0xC0)    // Pulsed at fCLK / 2^21
#define ADS1299_CONFIG2_TEST_SIGNAL_FREC_0_98   (0xC1)    // Pulsed at fCLK / 2^20
#define ADS1299_CONFIG2_TEST_SIGNAL_FREC_DC     (0xC3)    // At DC

/* ------ CONFIG3 settings ------ */
#define ADS1299_CONFIG3_BIAS_STAT_NOT_CONNECTED (0x61)    // BIAS is not connected
#define ADS1299_CONFIG3_BIAS_STAT_CONNECTED     (0x60)    // BIAS is connected
#define ADS1299_CONFIG3_BIAS_LOFF_SENS_ENABLE   (0x62)    // BIAS sense is enabled
#define ADS1299_CONFIG3_BIAS_LOFF_SENS_DISABLE  (0x60)    // BIAS sense is disabled
#define ADS1299_CONFIG3_BIAS_BUFFER_ENABLE      (0x64)    // BIAS buffer is enabled
#define ADS1299_CONFIG3_BIAS_BUFFER_DISABLE     (0x60)    // BIAS buffer is powered down
#define ADS1299_CONFIG3_BIASREF_SIGNAL_INTERNALLY (0x68)  // BIASREF signal (AVDD + AVSS) / 2 generated internally
#define ADS1299_CONFIG3_BIASREF_SIGNAL_EXTERNALLY (0x60)  // BIASREF signal fed externally
#define ADS1299_CONFIG3_BIAS_MEAS_OPEN          (0x60)    // OPEN
#define ADS1299_CONFIG3_BIAS_MEAS_BIAS_IN       (0x70)    // BIAS_IN signal is routed to the channel that has the MUX_Setting 010 (VREF)
#define ADS1299_CONFIG3_INTERNAL_REF_BUF_EN     (0xE0)    // Enable internal reference buffer
#define ADS1299_CONFIG3_INTERNAL_REF_BUF_DIS    (0x60)    // Power-down internal reference buffer

/* ------ CONFIG4 settings ------ */
#define ADS1299_CONFIG4_LOFF_COMPARATOR_EN      (0x02)    // Lead-Off comparator enabled
#define ADS1299_CONFIG4_SINGLE_SHOT_MODE        (0x08)    // Single-shot mode

/* ------ Lead-Off settings ------ */
#define ADS1299_LOFF_TH_95                      (0x00)    // Lead-off comp threshold (positive 95%)
#define ADS1299_LOFF_TH_92                      (0x20)    // Lead-off comp threshold (positive 92.5%)
#define ADS1299_LOFF_TH_90                      (0x40)    // Lead-off comp threshold (positive 90%)
#define ADS1299_LOFF_TH_87                      (0x60)    // Lead-off comp threshold (positive 87%)
#define ADS1299_LOFF_TH_85                      (0x80)    // Lead-off comp threshold (positive 85%)
#define ADS1299_LOFF_TH_80                      (0xA0)    // Lead-off comp threshold (positive 80%)
#define ADS1299_LOFF_TH_75                      (0xC0)    // Lead-off comp threshold (positive 75%)
#define ADS1299_LOFF_TH_70                      (0xE0)    // Lead-off comp threshold (positive 70%)
#define ADS1299_LOFF_CURRENT_6N                 (0x00)    // Lead-off curr magnitude 6 nA
#define ADS1299_LOFF_CURRENT_24N                (0x04)    // Lead-off curr magnitude 24 nA
#define ADS1299_LOFF_CURRENT_6U                 (0x08)    // Lead-off curr magnitude 6 uA
#define ADS1299_LOFF_CURRENT_24U                (0x0C)    // Lead-off curr magnitude 24 uA
#define ADS1299_LOFF_FREQ_DC                    (0x00)    // DC lead-off detection
#define ADS1299_LOFF_FREQ_7                     (0x01)    // AC lead-off detection at 7.8 Hz (fCLK / 2^18)
#define ADS1299_LOFF_FREQ_31                    (0x02)    // AC lead-off detection at 31.2 Hz (fCLK / 2^16)
#define ADS1299_LOFF_FREQ_DR                    (0x03)    // AC lead-off detection at fDR / 4

/* ------ CHnSET settings ------ */
#define ADS1299_CH_N_SET_SETUP_NO               (0x00)    // Normal operation
#define ADS1299_CH_N_POWER_DONE                 (0x80)    // Channel power-down
#define ADS1299_CH_N_PGA_GAIN_24                (0x60)    // PGA gain 24
#define ADS1299_CH_N_GPA_GAIN_12                (0x50)    // PGA gain 12
#define ADS1299_CH_N_GPA_GAIN_8                 (0x40)    // PGA gain 8
#define ADS1299_CH_N_PGA_GAIN_6                 (0x30)    // PGA gain 6
#define ADS1299_CH_N_PGA_GAIN_4                 (0x20)    // PGA gain 4
#define ADS1299_CH_N_PGA_GAIN_2                 (0x10)    // PGA gain 2
#define ADS1299_CH_N_PGA_GAIN_1                 (0x00)    // PGA gain 1
#define ADS1299_CH_N_SRB2_OPEN                  (0x00)    // SRB2 Open
#define ADS1299_CH_N_SRB2_CLOSED                (0x08)    // SRB2 Closed
#define ADS1299_CH_N_NORMAL_ELECTRODE_INPUT     (0x00)    // Normal electrode input
#define ADS1299_CH_N_INPUT_SHORTED              (0x01)    // Input shorted
#define ADS1299_CH_N_BIAS_MEASUREMENTS          (0x02)    // Used in conjunction with BIAS_MEAS bit (CONFIG3 bit4)
#define ADS1299_CH_N_MVDD                       (0x03)    // MVDD for supply measurement
#define ADS1299_CH_N_TEMP_SENSOR                (0x04)    // Temperature sensor
#define ADS1299_CH_N_TEST_SIGNAL                (0x05)    // Test signal
#define ADS1299_CH_N_BIAS_DRP                   (0x06)    // BIAS_DRP
#define ADS1299_CH_N_BIAS_DRN                   (0x07)    // BIAS_DRN
#define CH_GAIN_MASK                            (0X70)

/* ------ BIAS_SENSP settings ------ */
#define ADS1299_BIASP8_EN                       (0x80)    // Route channel 8 positive signal into BIAS derivation
#define ADS1299_BIASP7_EN                       (0x40)    // Route channel 7 positive signal into BIAS derivation
#define ADS1299_BIASP6_EN                       (0x20)    // Route channel 6 positive signal into BIAS derivation
#define ADS1299_BIASP5_EN                       (0x10)    // Route channel 5 positive signal into BIAS derivation
#define ADS1299_BIASP4_EN                       (0x08)    // Route channel 4 positive signal into BIAS derivation
#define ADS1299_BIASP3_EN                       (0x04)    // Route channel 3 positive signal into BIAS derivation
#define ADS1299_BIASP2_EN                       (0x02)    // Route channel 2 positive signal into BIAS derivation
#define ADS1299_BIASP1_EN                       (0x01)    // Route channel 1 positive signal into BIAS derivation
#define ADS1299_BIASP_EN_ALL                    (0xFF)    // Route all channel positive signal into BIAS derivation

/* ------ BIAS_SENSN settings ------ */
#define ADS1299_BIASN8_EN                       (0x80)    // Route channel 8 negative signal into BIAS derivation
#define ADS1299_BIASN7_EN                       (0x40)    // Route channel 7 negative signal into BIAS derivation
#define ADS1299_BIASN6_EN                       (0x20)    // Route channel 6 negative signal into BIAS derivation
#define ADS1299_BIASN5_EN                       (0x10)    // Route channel 5 negative signal into BIAS derivation
#define ADS1299_BIASN4_EN                       (0x08)    // Route channel 4 negative signal into BIAS derivation
#define ADS1299_BIASN3_EN                       (0x04)    // Route channel 3 negative signal into BIAS derivation
#define ADS1299_BIASN2_EN                       (0x02)    // Route channel 2 negative signal into BIAS derivation
#define ADS1299_BIASN1_EN                       (0x01)    // Route channel 1 negative signal into BIAS derivation
#define ADS1299_BIASN_EN_ALL                    (0xFF)    // Route all channel negative signal into BIAS derivation

/* ------ LOFF_SENSP settings ------ */
#define ADS1299_LOFFP8_EN                       (0x80)    // Enable lead-off detection on IN8P
#define ADS1299_LOFFP7_EN                       (0x40)    // Enable lead-off detection on IN7P
#define ADS1299_LOFFP6_EN                       (0x20)    // Enable lead-off detection on IN6P
#define ADS1299_LOFFP5_EN                       (0x10)    // Enable lead-off detection on IN5P
#define ADS1299_LOFFP4_EN                       (0x08)    // Enable lead-off detection on IN4P
#define ADS1299_LOFFP3_EN                       (0x04)    // Enable lead-off detection on IN3P
#define ADS1299_LOFFP2_EN                       (0x02)    // Enable lead-off detection on IN2P
#define ADS1299_LOFFP1_EN                       (0x01)    // Enable lead-off detection on IN1P
#define ADS1299_LOFFP_EN_ALL                    (0xFF)    // Enable lead-off detection on all INxP
#define ADS1299_LOFFP_DIS_ALL                   (0x00)    // Disable lead-off detection on all INxP

/* ------ LOFF_SENSN settings ------ */
#define ADS1299_LOFFM8_EN                       (0x80)    // Enable lead-off detection on IN8N
#define ADS1299_LOFFM7_EN                       (0x40)    // Enable lead-off detection on IN7N
#define ADS1299_LOFFM6_EN                       (0x20)    // Enable lead-off detection on IN6N
#define ADS1299_LOFFM5_EN                       (0x10)    // Enable lead-off detection on IN5N
#define ADS1299_LOFFM4_EN                       (0x08)    // Enable lead-off detection on IN4N
#define ADS1299_LOFFM3_EN                       (0x04)    // Enable lead-off detection on IN3N
#define ADS1299_LOFFM2_EN                       (0x02)    // Enable lead-off detection on IN2N
#define ADS1299_LOFFM1_EN                       (0x01)    // Enable lead-off detection on IN1N
#define ADS1299_LOFFM_EN_ALL                    (0xFF)    // Enable lead-off detection on all INxN
#define ADS1299_LOFFM_DIS_ALL                   (0x00)    // Disable lead-off detection on all INxN

/* ------ LOFF_FLIP settings ------ */
#define ADS1299_LOFF_FLIP_DIS_ALL               (0x00)    // All INxP are pulled to AVDD and INxN pulled to AVSS
#define ADS1299_LOFF_FLIP8_EN                   (0x80)    // IN8P is pulled to AVSS and IN8N pulled to AVDD
#define ADS1299_LOFF_FLIP7_EN                   (0x40)    // IN7P is pulled to AVSS and IN7N pulled to AVDD
#define ADS1299_LOFF_FLIP6_EN                   (0x20)    // IN6P is pulled to AVSS and IN6N pulled to AVDD
#define ADS1299_LOFF_FLIP5_EN                   (0x10)    // IN5P is pulled to AVSS and IN5N pulled to AVDD
#define ADS1299_LOFF_FLIP4_EN                   (0x08)    // IN4P is pulled to AVSS and IN4N pulled to AVDD
#define ADS1299_LOFF_FLIP3_EN                   (0x04)    // IN3P is pulled to AVSS and IN3N pulled to AVDD
#define ADS1299_LOFF_FLIP2_EN                   (0x02)    // IN2P is pulled to AVSS and IN2N pulled to AVDD
#define ADS1299_LOFF_FLIP1_EN                   (0x01)    // IN1P is pulled to AVSS and IN1N pulled to AVDD
#define ADS1299_LOFF_FLIP_EN_ALL                (0xFF)    // All INxP are pulled to AVSS and all INxN pulled to AVDD

/* ------ GPIO settings ------ */
#define ADS1299_GPIO_ALL_OUTPUT                 (0x00)    // All GPIOs pin are output
#define ADS1299_GPIO1_CONTROL_INPUT             (0x01)    // GPIO1 is an input
#define ADS1299_GPIO2_CONTROL_INPUT             (0x02)    // GPIO2 is an input
#define ADS1299_GPIO3_CONTROL_INPUT             (0x04)    // GPIO3 is an input
#define ADS1299_GPIO4_CONTROL_INPUT             (0x08)    // GPIO4 is an input
#define ADS1299_GPIO1_DATA_HI                   (0x10)
#define ADS1299_GPIO2_DATA_HI                   (0x20)
#define ADS1299_GPIO3_DATA_HI                   (0x40)
#define ADS1299_GPIO4_DATA_HI                   (0x80)

/* ------ MISC1 settings ------ */
#define ADS1299_MISC1_SRB1_SWITCHES_OPEN        (0X00)    // Switches open
#define ADS1299_MISC1_SRB1_SWITCHES_CLOSED      (0X20)    // Switches closed: Connects the SRB1 to all channels inverting inputs

#endif
