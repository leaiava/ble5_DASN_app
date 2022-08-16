/*******************************************************************************
 * INCLUDES
 */
#include <DASN_ADS1299.h>
//#include <DASN_UI.h>

#include <Board.h>
#include <icall.h>
#include <services/data_service.h>
#include <ti/sysbios/knl/Event.h>
//#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#define DEBUG_ON
#ifdef  DEBUG_ON
//#include <xdc/runtime/Log.h> // Comment this in to use xdc.runtime.Log
#include <uartlog/UartLog.h>  // Comment out if using xdc Log
#include <ti/display/AnsiColor.h>
#endif
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Task configuration
#define DASN_ADS1299_TASK_PRIORITY                     1

#ifndef DASN_ADS1299_TASK_STACK_SIZE
#define DASN_ADS1299_TASK_STACK_SIZE                   512
#endif

#define DASN_ADS1299_TOTAL_REGISTERS                    24
#define DASN_ADS1299_TRANSACTION_HEADER                 3
#define DASN_ADS1299_READALL_LEN    DASN_ADS1299_TOTAL_REGISTERS+DASN_ADS1299_TRANSACTION_HEADER
#define DASN_ADS1299_POWER_ON_STARTUP                   1000

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Task configuration
Task_Struct DASN_ADS1299Task;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(ads1299TaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t ads1299TaskStack[DASN_ADS1299_TASK_STACK_SIZE];


/*********************************************************************
 * LOCAL VARIABLES
 */
Event_Handle ads1299_event;
static Event_Params eventParams;
static Event_Struct structEvent; /* Memory allocated at build time */
static SPI_Handle ads1299handle;
static SPI_Transaction ads1299transaction;
static transaction_t tipo;
static PIN_State powerPinState; //***
static PIN_State DRDYPinState; //***
static PIN_Handle DRDYPinHandle; //***
//static Semaphore_Handle ads1299_sem;
const PIN_Config powerPinList[] = {
                   DASN_EN_plusV | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL,
                   DASN_SHDN_minusV | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,
                   PIN_TERMINATE
       };
/*
 * Application Data Ready pin configuration table:
 *   - DRDY pin interrupt is configured to trigger on falling edge.
 */
const PIN_Config DRDYPinTable[] = {
    Board_nDRDY | PIN_INPUT_EN | PIN_IRQ_NEGEDGE, PIN_TERMINATE
};

uint8_t regID = 1;
ads1299_registers_t ads1299_registers;
uint8_t txbuf[DS_STREAM_LEN];
uint8_t rxbuf[DS_STREAM_LEN];
uint8_t data[DS_STREAM_LEN];
uint8_t counter=0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void transferCallback(SPI_Handle ads1299handle, SPI_Transaction* ptrTransaction);
static void ads1299_error_handling(uint8_t errorNumber);
/* DRDY handling function*/
static void DRDYCallbackFxn(PIN_Handle handle,PIN_Id pinId);
static void DASN_ADS1299_taskFxn(UArg a0, UArg a1);
static void ADS1299_init(void);
static void ADS1299_power_on(void);
static void ADS1299_power_off(void);
/*********************************************************************
 * EXTERN FUNCTIONS
 */
bStatus_t DASN_enqueueMsg(uint8_t event, void *pData);
extern Event_Handle UI_event;
/*********************************************************************
 * PROFILE CALLBACKS
 */

static void transferCallback(SPI_Handle ads1299handle, SPI_Transaction* ptrTransaction)
{
    //transaction_t tipo = *(transaction_t*)transaction->arg;
    switch(tipo)
    {
    case READ_REG_ALL:
    {
        ads1299_registers_t *registers = (ads1299_registers_t*)(ptrTransaction->rxBuf);
        regID = 0;
        //Log_info1(ANSI_COLOR(BG_RED)"regID = %d"ANSI_COLOR(ATTR_RESET),regID );
        regID = registers->id;
        DASN_enqueueMsg(DASN_TRANSFER_COMPLETE, 0);
        break;
    }
    case WRITE_REG:
        DASN_enqueueMsg(DASN_TRANSFER_COMPLETE, 0);
        break;
    case READ_ALL_CH:
    {
        uint8_t *newdata =(uint8_t *)ICall_malloc(sizeof(rxbuf));
        memcpy(newdata, rxbuf,DS_STREAM_LEN );
        DASN_enqueueMsg(DASN_NEW_DATA_EVT, newdata);//TODO: Manejar el error
    }
        break;
    case SEND_CMD:
        DASN_enqueueMsg(DASN_TRANSFER_COMPLETE, 0);
        break;
    default:
        break;
    }

    return;
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void DASN_ADS1299_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = ads1299TaskStack;
    taskParams.stackSize = DASN_ADS1299_TASK_STACK_SIZE;
    taskParams.priority = DASN_ADS1299_TASK_PRIORITY;

    Task_construct(&DASN_ADS1299Task, DASN_ADS1299_taskFxn, &taskParams, NULL);
}

static void DASN_ADS1299_taskFxn(UArg a0, UArg a1)
{
    bool ch_config_flag = false;
    bool frec_config_flag = false;

    ADS1299_init();
    Log_info0("ADS1299_init START!");
    mis_estados_t ads1299_estado = ESTADO_STANDBY;
    // ads1299 main loop
    for(;;)
    {
        uint32_t events;

        // Waits for an event to be posted
        events = Event_pend(ads1299_event, Event_Id_NONE, ADS1299_ALL_EVENTS, BIOS_WAIT_FOREVER);

        Log_info1("Events =  %d",events);
        // No importa el estado, si llega este comando reseteo, vuelve a STANDBY
        if (events == ADS1299_CMD_RESET)
        {
            ch_config_flag = false;
            frec_config_flag = false;
            ads1299_estado = ESTADO_STANDBY;
            ads1299_sendCommand(RESET_CMD);
            //ADS1299_power_off();
        }
        switch (ads1299_estado)
        {
        case ESTADO_STANDBY:
            Log_info0("ESTADO_STANDBY");
            if (events == ADS1299_CMD_HOLA)
            {
                // TODO:Contesto OK
                registers_t reg;
                ADS1299_power_on();
                Task_sleep(2000);

                ads1299_writeRegister(CONFIG3 ,
                                      ADS1299_CONFIG3_INTERNAL_REF_BUF_EN |
                                      ADS1299_CONFIG3_BIASREF_SIGNAL_INTERNALLY |
                                      ADS1299_CONFIG3_BIAS_BUFFER_ENABLE
                                      );
                Task_sleep(1000);
                reg = CH1SET;
                for(uint8_t i=0; i<8 ; i++)
                {

                    ads1299_writeRegister(reg,
                                          ADS1299_CH_N_SET_SETUP_NO |
                                          ADS1299_CH_N_PGA_GAIN_1 |
                                          ADS1299_CH_N_SRB2_OPEN |
                                          ADS1299_CH_N_NORMAL_ELECTRODE_INPUT
                                          );
                    reg++;
                }
                ads1299_writeRegister(LOFF,
                                      ADS1299_LOFF_TH_85 |
                                      ADS1299_LOFF_CURRENT_24U |
                                      ADS1299_LOFF_FREQ_31
                                      );
                ads1299_readAllRegisters();
                Task_sleep(1000);

                ads1299_estado = ESTADO_CONECTADO;
                Log_info0("ESTADO_CONECTADO");
            }
            else
            {
                // TODO: Contesto NO_Ok
            }

            break;
        case ESTADO_CONECTADO:
            if (ch_config_flag && frec_config_flag)
            {
                ads1299_estado = ESTADO_LISTO_PARA_ADQUIRIR;
                Log_info0("ESTADO_LISTO_PARA_ADQUIRIR");
            }
            if( events == ADS1299_CMD_CONFIG_INICIAR)
            {
                // todo: Contesto OK
                ads1299_estado = ESTADO_CONFIGURANDO;
                Log_info0("ESTADO_CONFIGURANDO");
            }
            else if (events == ADS1299_CMD_CHAU)
            {
                // todo: Contesto OK
                ch_config_flag = false;
                frec_config_flag = false;
                ads1299_estado = ESTADO_STANDBY;
                ADS1299_power_off();
                Log_info0("ESTADO_STANDBY");
            }
            else
            {
                ;//TODO: contesto NO_OK
            }

            break;
        case ESTADO_CONFIGURANDO:

            Log_info0("ESTADO_CONFIGURANDO");
            if (events == ADS1299_CMD_CONFIG_TERMINAR)
            {
                ads1299_estado = ESTADO_CONECTADO;
                Event_post(ads1299_event, ADS1299_CMD_WAKE_UP); //envío para actualizar la MEF
            }

            //verifico si es un comando de configuracion todos canales_ON
            else if (events == ADS1299_CMD_CONFIG_CH_ALL_ON ||
                    events == ADS1299_CMD_CONFIG_CH_ALL_OFF)
            {
                uint8_t parameter;
                if (events == ADS1299_CMD_CONFIG_CH_ALL_ON)
                    parameter = ADS1299_CH_N_SET_SETUP_NO |
                                ADS1299_CH_N_PGA_GAIN_1;
                else
                    parameter = ADS1299_CH_N_POWER_DONE;
                ads1299_registers.ch1set = parameter;
                ads1299_writeRegister(CH1SET, ads1299_registers.ch1set);
                ads1299_registers.ch2set = parameter;
                ads1299_writeRegister(CH2SET, ads1299_registers.ch2set);
                ads1299_registers.ch3set = parameter;
                ads1299_writeRegister(CH3SET, ads1299_registers.ch3set);
                ads1299_registers.ch4set = parameter;
                ads1299_writeRegister(CH4SET, ads1299_registers.ch4set);
                ads1299_registers.ch5set = parameter;
                ads1299_writeRegister(CH5SET, ads1299_registers.ch5set);
                ads1299_registers.ch6set = parameter;
                ads1299_writeRegister(CH6SET, ads1299_registers.ch6set);
                ads1299_registers.ch7set = parameter;
                ads1299_writeRegister(CH7SET, ads1299_registers.ch7set);
                ads1299_registers.ch8set = parameter;
                ads1299_writeRegister(CH8SET, ads1299_registers.ch8set);
                ch_config_flag = true;
            }
            //verifico si es un comando de configuracion de canales
            else if (events >= ADS1299_CMD_CONFIG_CH1_ON &&
                    events <= ADS1299_CMD_CONFIG_CH8_ON )
            {
                registers_t reg;
                switch (events)
                {
                    case ADS1299_CMD_CONFIG_CH1_ON: reg = CH1SET;break;
                    case ADS1299_CMD_CONFIG_CH2_ON: reg = CH2SET;break;
                    case ADS1299_CMD_CONFIG_CH3_ON: reg = CH3SET;break;
                    case ADS1299_CMD_CONFIG_CH4_ON: reg = CH4SET;break;
                    case ADS1299_CMD_CONFIG_CH5_ON: reg = CH5SET;break;
                    case ADS1299_CMD_CONFIG_CH6_ON: reg = CH6SET;break;
                    case ADS1299_CMD_CONFIG_CH7_ON: reg = CH7SET;break;
                    case ADS1299_CMD_CONFIG_CH8_ON: reg = CH8SET;break;
                }
                //configuro chanels
                //todo: Mejorar esto, debería actualizar la copia del registro interno

                ads1299_writeRegister(reg, ADS1299_CH_N_SET_SETUP_NO | ADS1299_CH_N_PGA_GAIN_1);
                ch_config_flag = true;
                // todo: Contesto OK

            }
            else if (events >= ADS1299_CMD_CONFIG_FREC_1 &&
                    events <= ADS1299_CMD_CONFIG_FREC_7)
            {
                // pongo en cero los bits del DATA_RATE
                Log_info1("CONFIG1:           %d",ads1299_registers.config1);
                ads1299_registers.config1 &= ~DATA_RATE_MASK;
                Log_info1("CONFIG1 with MASK: %d",ads1299_registers.config1);
                // aplico la configuracion del DATA_RATE sin modificar el resto del registro
                ads1299_registers.config1 |= ADS1299_CONFIG1_DATA_RATE_16000 + events - ADS1299_CMD_CONFIG_FREC_1;
                ads1299_writeRegister(CONFIG1 , ads1299_registers.config1);
                Log_info1("CONFIG1 MODIFIED:  %d",ads1299_registers.config1);
                Task_sleep(1000);
                frec_config_flag = true;
            }
            else if (events == ADS1299_CMD_ZSIGNAL_ON)
            {
                ads1299_writeRegister(LOFF_SENSN, ADS1299_LOFFM_EN_ALL);
                ads1299_writeRegister(LOFF_SENSP, ADS1299_LOFFP_EN_ALL);
            }
            else if (events == ADS1299_CMD_ZSIGNAL_OFF)
            {
                ads1299_writeRegister(LOFF_SENSN, ADS1299_LOFFM_DIS_ALL);
                ads1299_writeRegister(LOFF_SENSP, ADS1299_LOFFP_DIS_ALL);
            }
            else
            {
                // todo: Contesto NO_OK
            }

            break;
        case ESTADO_LISTO_PARA_ADQUIRIR:
            if(events == ADS1299_CMD_CONFIG_INICIAR)
            {
                // todo: Contesto OK
                ads1299_estado = ESTADO_CONFIGURANDO;
                Log_info0("ESTADO_CONFIGURANDO");
            }
            else if(events == ADS1299_CMD_ADQUIRIR)
            {
                // todo: Contesto OK
                ads1299_startConversion();
                ads1299_estado = ESTADO_ADQUIRIENDO;
                Log_info0("ESTADO_ADQUIRIENDO");
                //Event_post(UI_event, UI_ACQ_BLE);
            }
            else if(events == ADS1299_CMD_CHAU)
            {
                // todo: Contesto OK
                ch_config_flag = false;
                frec_config_flag = false;
                ads1299_estado = ESTADO_STANDBY;
                ADS1299_power_off();
                Log_info0("ESTADO_STANDBY");
            }
            break;
        case ESTADO_ADQUIRIENDO:
            if (events == ADS1299_CMD_PARAR)
            {
                ads1299_estado = ESTADO_LISTO_PARA_ADQUIRIR;
                ADS1299_STOP();
                Log_info0("ESTADO_LISTO_PARA_ADQUIRIR");
            }
            else
                // todo: Contesto Busy

            break;
        default:;
        }

    }

}

static void ADS1299_init(void)
{
    Event_Params_init(&eventParams);
    Event_construct(&structEvent, &eventParams);
    /* It's optional to store the handle */
    ads1299_event = Event_handle(&structEvent);

    // Inicializo los buffers del SPI en 0
    for (uint8_t i=0 ; i<DS_STREAM_LEN ; i++)
    {
        txbuf[i] = 0;
        rxbuf[i] = 0;
    }

    // Open power pins
    if (!PIN_open(&powerPinState, powerPinList))
    {
        //Log_error0("DRDY pin, Handle allocation error");
        Task_exit();
    }

    //ADS1299_power_on(); lo hago por comando, TODO: Borrar

    /*
    // Semaphore Initialization
    Semaphore_Params semParams;
    Semaphore_Struct structSem; // Memory allocated at build time
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&structSem, 0, &semParams);
    ads1299_sem = Semaphore_handle(&structSem);
    */

    // Configuro SPI
    SPI_Params params;
    ads1299transaction.arg = &tipo;
    SPI_init();  // Initialize the SPI driver

    // Init SPI and specify non-default parameters
    SPI_Params_init(&params);
    params.bitRate              = 4000000;
    params.dataSize             = 8;
    params.frameFormat          = SPI_POL0_PHA1;
    params.mode                 = SPI_MASTER;
    params.transferMode         = SPI_MODE_CALLBACK;
    params.transferCallbackFxn  = transferCallback;

    // Open the SPI and initiate the first transfer
    if( !(ads1299handle = SPI_open(DASN_SPI0, &params)))
        ads1299_error_handling(0x01);  // Handle allocation error

    //TODO: Borrar las proximas lineas, manejado por comandos.
    /*
    ads1299_readAllRegisters(&ads1299transaction);
    Task_sleep(1000);
    ads1299_writeRegister(&ads1299transaction, CONFIG3 , ADS1299_CONFIG3_INTERNAL_REF_BUF_EN);
    Task_sleep(1000);
    ads1299_registers.config1 &= ~DATA_RATE_MASK;  // pongo en cero los bits del DATA_RATE
    ads1299_registers.config1 |= ADS1299_CONFIG1_DATA_RATE_500; // aplico la configuracion del DATA_RATE sin modificar el resto del registro
    ads1299_writeRegister(&ads1299transaction, CONFIG1 , ads1299_registers.config1);
    Task_sleep(1000);

    ads1299_startConversion(&ads1299transaction);
     */
    // Open DRDY pin //***
    DRDYPinHandle = PIN_open(&DRDYPinState, DRDYPinTable);
    if(!DRDYPinHandle)
    {
       //Log_error0("Error initializing DRDY pin");
       Task_exit();
    }
    // Setup callback for DRDY pin
    if(PIN_registerIntCb(DRDYPinHandle, &DRDYCallbackFxn) != 0)
    {
       //Log_error0("Error registering DRDY callback function");
       Task_exit();
    }
}

/**
 * @brief   Escribe todos los regístros internos del ads1299
 *
 */
void ads1299_readAllRegisters(void)
{
    // Configure the transaction
    ads1299transaction.count = DASN_ADS1299_READALL_LEN;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = &ads1299_registers;
    *(transaction_t*)(ads1299transaction.arg) = READ_REG_ALL;

    ((uint8_t*)ads1299transaction.txBuf)[0] = SDATAC_CMD;  // Siempre primero envío comando para detener
    ((uint8_t*)ads1299transaction.txBuf)[1] = RREG_CMD;    // Comando para iniciar lectura de registros
    ((uint8_t*)ads1299transaction.txBuf)[2] = DASN_ADS1299_TOTAL_REGISTERS - 1;
    // Agrego estos últimos para borrar por si había quedado basura en txbuf
    ((uint8_t*)ads1299transaction.txBuf)[3] = NULL_CMD;
    ((uint8_t*)ads1299transaction.txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);
}

void ads1299_readAllChannels(void)
{
    // Configure the transaction
    ads1299transaction.count = 27; //( 8CH + STATUS CH ) * 3 bytes
    ads1299transaction.txBuf = 0;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = READ_ALL_CH;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);
}

/**
 * @brief   Escribe en un regístro interno del ads1299
 *
 * @param   registro    : Registro a escribir del tipo registers_t.
 * @param   data        : Palabra a escribir en el registro
 */
void ads1299_writeRegister(registers_t registro, uint8_t data)
{
    // Configure the transaction
    ads1299transaction.count = 5;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = WRITE_REG;

    ((uint8_t*)ads1299transaction.txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ads1299transaction.txBuf)[1] = (WREG_CMD | registro);   ///< Hago la OR para tener el valor del comando compuesto
    ((uint8_t*)ads1299transaction.txBuf)[2] = NULL_CMD;                ///< Indica que leo solo un registro
    ((uint8_t*)ads1299transaction.txBuf)[3] = data;
    // Agego este extra ya que sino el ads1299 no me tomaba el último byte
    ((uint8_t*)ads1299transaction.txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);

}

/**
 * @brief   Lee un regístro interno del ads1299
 *
 * @param   registro    : Registro a escribir del tipo registers_t.

 */
uint8_t ads1299_readRegister(registers_t registro)
{
/*
    // Configure the transaction
    ads1299transaction.count = 5;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = READ_REG;

    ((uint8_t*)ads1299transaction.txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ads1299transaction.txBuf)[1] = (RREG_CMD | registro);   ///< Hago la OR para tener el valor del comando compuesto
    ((uint8_t*)ads1299transaction.txBuf)[2] = NULL_CMD;                ///< Indica que leo solo un registro
    ((uint8_t*)ads1299transaction.txBuf)[3] = NULL_CMD;
    // Agego este extra ya que sino el ads1299 no me tomaba el último byte
    ((uint8_t*)ads1299transaction.txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);
*/
return 0;
}

void ads1299_sendCommand(commands_t cmd)
{
    // Configure the transaction
    ads1299transaction.count = 2;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = SEND_CMD;

    ((uint8_t*)ads1299transaction.txBuf)[0] = cmd;
    ((uint8_t*)ads1299transaction.txBuf)[1] = NULL_CMD;
    ((uint8_t*)ads1299transaction.txBuf)[2] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);
}
void ads1299_startConversion(void)
{
    // Configure the transaction
    ads1299transaction.count = 4;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = SEND_CMD;

    ((uint8_t*)ads1299transaction.txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ads1299transaction.txBuf)[1] = RDATAC_CMD;              ///< Comando de lectura continua
    ((uint8_t*)ads1299transaction.txBuf)[2] = NULL_CMD;                ///< Dejo un comando vacio para darle tiempo al RDATAC
    ((uint8_t*)ads1299transaction.txBuf)[3] = START_CMD;
    // Agego este extra ya que sino el ads1299 no me tomaba el último byte
    ((uint8_t*)ads1299transaction.txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);

}

/**
 * @brief   Power on the ADS1299
 *
 * @detail  Enable the 2,5V and the -2,V LDOs.
 */
static void ADS1299_power_on(void)
{
    // Habilito +2,5V
    PIN_setOutputValue(&powerPinState , DASN_EN_plusV , 1 );
    // Habilito -2,5V
    PIN_setOutputValue(&powerPinState , DASN_SHDN_minusV , 0 );
    // Delay for LDOs startUp
    Task_sleep(DASN_ADS1299_POWER_ON_STARTUP);
}
/**
 * @brief   Power off the ADS1299
 *
 * @detail  Disable the 2,5V and the -2,V LDOs.
 */
void ADS1299_power_off(void)
{
    // Habilito +2,5V
    PIN_setOutputValue(&powerPinState , DASN_EN_plusV , 0 );
    // Habilito -2,5V
    PIN_setOutputValue(&powerPinState , DASN_SHDN_minusV , 1 );
}
/**
 * @ Brief  Stop de conversion
 *
 * @details Send SDATAC comand to stop de conversion.
 */
void ADS1299_STOP(void)
{
    // Configure the transaction
    ads1299transaction.count = 4;
    ads1299transaction.txBuf = txbuf;
    ads1299transaction.rxBuf = rxbuf;
    *(transaction_t*)(ads1299transaction.arg) = SEND_CMD;

    ((uint8_t*)ads1299transaction.txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ads1299transaction.txBuf)[1] = STOP_CMD;                ///< Comando de STOP conversion
    ((uint8_t*)ads1299transaction.txBuf)[2] = NULL_CMD;                ///< Dejo un comando vacio para darle tiempo al RDATAC
    ((uint8_t*)ads1299transaction.txBuf)[3] = NULL_CMD;
    ((uint8_t*)ads1299transaction.txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, &ads1299transaction);


}

/*********************************************************************
 * @fn     DRDYCallbackFxn
 *
 * @brief  Callback from PIN driver on interrupt
 *
 * @param  handle    The PIN_Handle instance this is about
 * @param  pinId     The pin that generated the interrupt
 */
static void DRDYCallbackFxn(PIN_Handle handle, PIN_Id pinId)
{
    /*
    static uint8_t counter = 0;

    counter+=1;
    if(counter == 10)
    {
        ads1299_readAllChannels();
        counter = 0;
    }
    */
    ads1299_readAllChannels();
}

/**
 *
 */
static void ads1299_error_handling(uint8_t errorNumber)
{
    while(1);
}
