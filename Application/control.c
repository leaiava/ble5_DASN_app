/*******************************************************************************
 * INCLUDES
 */
#include <control.h>
#include <ti/sysbios/knl/Task.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Task configuration
#define CONTROL_TASK_PRIORITY                     1

#ifndef CONTROL_TASK_STACK_SIZE
#define CONTROL_TASK_STACK_SIZE                   2048
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Task configuration
Task_Struct controlTask;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(appTaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t appTaskStack[CONTROL_TASK_STACK_SIZE];

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void control_taskFxn(void);
/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
static void control_taskFxn(void)
{

}

/**
 * @Brief Función para crear la tarea del modulo de control
 */
void control_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = appTaskStack;
    taskParams.stackSize = CONTROL_TASK_STACK_SIZE;
    taskParams.priority = CONTROL_TASK_PRIORITY;

    Task_construct(&controlTask, control_taskFxn, &taskParams, NULL);
}
