/*
 * UI.c
 *
 *  Created on: Feb 20, 2022
 *      Author: leandro
 */
/*******************************************************************************
 * INCLUDES
 */
#include <DASN_UI.h>

#include <Board.h>
#include <icall.h>
//#include <services/data_service.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
//#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#define DEBUG_ON
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
#define DASN_UI_TASK_PRIORITY                     1

#ifndef DASN_UI_TASK_STACK_SIZE
#define DASN_UI_TASK_STACK_SIZE                   512
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Task configuration
Task_Struct DASN_UI_Task;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(UITaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t UITaskStack[DASN_UI_TASK_STACK_SIZE];
/*********************************************************************
 * LOCAL VARIABLES
 */
Event_Handle UI_event;
static Event_Params eventParams;
static Event_Struct structEvent;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void DASN_UI_taskFxn(UArg a0, UArg a1);
static void UI_init(void);
/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void DASN_UI_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = UITaskStack;
    taskParams.stackSize = DASN_UI_TASK_STACK_SIZE;
    taskParams.priority = DASN_UI_TASK_PRIORITY;

    Task_construct(&DASN_UI_Task, DASN_UI_taskFxn, &taskParams, NULL);

}

static void DASN_UI_taskFxn(UArg a0, UArg a1)
{
    UI_init();
    Log_info0("UI_init START!");
    for(;;)
    {
        uint32_t events;
        uint32_t contador;
        events = Event_pend(UI_event, Event_Id_NONE, UI_ALL_EVENTS, BIOS_WAIT_FOREVER);
        switch(events)
        {
        case UI_ACQ_BLE:
            contador++;
        }

        while(1);
    }
}

static void UI_init(void)
{
    Event_Params_init(&eventParams);
    Event_construct(&structEvent, &eventParams);
    /* It's optional to store the handle */
    UI_event = Event_handle(&structEvent);
}
