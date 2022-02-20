/*******************************************************************************
 * INCLUDES
 */
#include <DASN_ADS1299.h>

#include <Board.h>
#include <services/data_service.h>
//#include <ti/sysbios/knl/Semaphore.h>
//#include <ti/sysbios/BIOS.h>
#define DEBUG_OFF
#ifdef  DEBUG_ON
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
//#define DASN_ADS1299_TASK_PRIORITY                     1

//#ifndef DASN_ADS1299_TASK_STACK_SIZE
//#define DASN_ADS1299_TASK_STACK_SIZE                   512
//#endif

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
/*
Task_Struct DASN_ADS1299Task;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(appTaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t appTaskStack[DASN_ADS1299_TASK_STACK_SIZE];
*/

/*********************************************************************
 * LOCAL VARIABLES
 */
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
uint8_t txbuf[32];
uint8_t rxbuf[32];
uint8_t data[32];
uint8_t counter=0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void transferCallback(SPI_Handle ads1299handle, SPI_Transaction* ptrTransaction);
static void ads1299_error_handling(uint8_t errorNumber);
/* DRDY handling function*/
static void DRDYCallbackFxn(PIN_Handle handle,PIN_Id pinId);
/*********************************************************************
 * EXTERN FUNCTIONS
 */
bStatus_t DASN_enqueueMsg(uint8_t event, void *pData);

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

        DASN_enqueueMsg(DASN_NEW_DATA_EVT, 0);//TODO: Manejar el error

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
//static void DASN_ADS1299_taskFxn(UArg a0, UArg a1)
void ADS1299_init()
{
    for (uint8_t i=0; i<32;i++)
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

    ADS1299_power_on();

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

    ads1299_readAllRegisters(&ads1299transaction);
    Task_sleep(1000);

    ads1299_writeRegister(&ads1299transaction, CONFIG3 , ADS1299_CONFIG3_INTERNAL_REF_BUF_EN);
    Task_sleep(1000);
    ads1299_registers.config1 &= ~DATA_RATE_MASK;  // pongo en cero los bits del DATA_RATE
    ads1299_registers.config1 |= ADS1299_CONFIG1_DATA_RATE_500; // aplico la configuracion del DATA_RATE sin modificar el resto del registro
    ads1299_writeRegister(&ads1299transaction, CONFIG1 , ads1299_registers.config1);
    Task_sleep(1000);
    ads1299_startConversion(&ads1299transaction);

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
void ads1299_readAllRegisters(SPI_Transaction* ptrTransaction)
{
    // Configure the transaction
    ptrTransaction->count = DASN_ADS1299_READALL_LEN;
    ptrTransaction->txBuf = txbuf;
    ptrTransaction->rxBuf = &ads1299_registers;
    *(transaction_t*)(ptrTransaction->arg) = READ_REG_ALL;

    ((uint8_t*)ptrTransaction->txBuf)[0] = SDATAC_CMD;  // Siempre primero envío comando para detener
    ((uint8_t*)ptrTransaction->txBuf)[1] = RREG_CMD;    // Comando para iniciar lectura de registros
    ((uint8_t*)ptrTransaction->txBuf)[2] = DASN_ADS1299_TOTAL_REGISTERS - 1;
    // Agrego estos últimos para borrar por si había quedado basura en txbuf
    ((uint8_t*)ptrTransaction->txBuf)[3] = NULL_CMD;
    ((uint8_t*)ptrTransaction->txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, ptrTransaction);
}

void ads1299_readAllChannels(SPI_Transaction* ptrTransaction)
{
    // Configure the transaction
    ptrTransaction->count = 27; //( 8CH + STATUS CH ) * 3 bytes
    ptrTransaction->txBuf = 0;
    ptrTransaction->rxBuf = rxbuf;
    *(transaction_t*)(ptrTransaction->arg) = READ_ALL_CH;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, ptrTransaction);
}

/**
 * @brief   Escribe en un regístro interno del ads1299
 *
 * @param   registro    : Registro a escribir del tipo registers_t.
 * @param   data        : Palabra a escribir en el registro
 */
void ads1299_writeRegister(SPI_Transaction* ptrTransaction, registers_t registro, uint8_t data)
{
    // Configure the transaction
    ptrTransaction->count = 5;
    ptrTransaction->txBuf = txbuf;
    ptrTransaction->rxBuf = rxbuf;
    *(transaction_t*)(ptrTransaction->arg) = WRITE_REG;

    ((uint8_t*)ptrTransaction->txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ptrTransaction->txBuf)[1] = (WREG_CMD | registro);   ///< Hago la OR para tener el valor del comando compuesto
    ((uint8_t*)ptrTransaction->txBuf)[2] = NULL_CMD;                ///< Indica que leo solo un registro
    ((uint8_t*)ptrTransaction->txBuf)[3] = data;
    // Agego este extra ya que sino el ads1299 no me tomaba el último byte
    ((uint8_t*)ptrTransaction->txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, ptrTransaction);

}

void ads1299_startConversion(SPI_Transaction* ptrTransaction)
{
    // Configure the transaction
    ptrTransaction->count = 4;
    ptrTransaction->txBuf = txbuf;
    ptrTransaction->rxBuf = rxbuf;
    *(transaction_t*)(ptrTransaction->arg) = SEND_CMD;

    ((uint8_t*)ptrTransaction->txBuf)[0] = SDATAC_CMD;              ///< Necesario si esta en RDATAC mode
    ((uint8_t*)ptrTransaction->txBuf)[1] = RDATAC_CMD;              ///< Comando de lectura continua
    ((uint8_t*)ptrTransaction->txBuf)[2] = NULL_CMD;                ///< Dejo un comando vacio para darle tiempo al RDATAC
    ((uint8_t*)ptrTransaction->txBuf)[3] = START_CMD;
    // Agego este extra ya que sino el ads1299 no me tomaba el último byte
    ((uint8_t*)ptrTransaction->txBuf)[4] = NULL_CMD;

    //inicio transferencia por SPI
    SPI_transfer(ads1299handle, ptrTransaction);

}

/**
 * @brief   Power on the ADS1299
 *
 * @detail  Enable the 2,5V and the -2,V LDOs.
 */
void ADS1299_power_on()
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
void ADS1299_power_off()
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
void ADS1299_STOP(SPI_Transaction* ptrTransaction)
{

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
    counter+=1;
    if(counter == 0)
    {
        ads1299_readAllChannels(&ads1299transaction);
    }

}

/**
 *
 */
static void ads1299_error_handling(uint8_t errorNumber)
{
    while(1);
}
