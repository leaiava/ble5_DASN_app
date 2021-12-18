/*******************************************************************************
 * INCLUDES
 */
#include <myADC.h>


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Task configuration
#define MYADC_TASK_PRIORITY                     1

#ifndef MYADC_TASK_STACK_SIZE
#define MYADC_TASK_STACK_SIZE                   2048
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Task configuration
Task_Struct myADCTask;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(appTaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t appTaskStack[MYADC_TASK_STACK_SIZE];

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void myADC_taskFxn(UArg a0, UArg a1);
/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
static void myADC_taskFxn(UArg a0, UArg a1)
{

}

/**
 * @Brief Función para crear la tarea del modulo de myADC
 */
void myADC_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = appTaskStack;
    taskParams.stackSize = MYADC_TASK_STACK_SIZE;
    taskParams.priority = MYADC_TASK_PRIORITY;
    taskParams.arg0 = NULL;

    Task_construct(&myADCTask, myADC_taskFxn, &taskParams, NULL);
}
