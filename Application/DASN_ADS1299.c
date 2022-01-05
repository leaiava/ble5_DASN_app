/*******************************************************************************
 * INCLUDES
 */
#include <DASN_ADS1299.h>


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Task configuration
#define DASN_ADS1299_TASK_PRIORITY                     1

#ifndef DASN_ADS1299_TASK_STACK_SIZE
#define DASN_ADS1299_TASK_STACK_SIZE                   2048
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Task configuration
Task_Struct DASN_ADS1299Task;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(appTaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t appTaskStack[DASN_ADS1299_TASK_STACK_SIZE];

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void DASN_ADS1299_taskFxn(UArg a0, UArg a1);
/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
static void DASN_ADS1299_taskFxn(UArg a0, UArg a1)
{

}

/**
 * @Brief Funcion para crear la tarea del modulo de myADC
 */
void DASN_ADS1299_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = appTaskStack;
    taskParams.stackSize = DASN_ADS1299_TASK_STACK_SIZE;
    taskParams.priority = DASN_ADS1299_TASK_PRIORITY;
    taskParams.arg0 = NULL;

    Task_construct(&DASN_ADS1299Task, DASN_ADS1299_taskFxn, &taskParams, NULL);
}
