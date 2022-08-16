/*
 * DASN_UI_Module.h
 *
 *  Created on: Aug 14, 2022
 *      Author: leandroarrieta@gmail.com
 */

#ifndef APPLICATION_DASN_UI_H_
#define APPLICATION_DASN_UI_H_

/*******************************************************************************
 * INCLUDES
 */
#include <Board.h>
#include <ti/drivers/PIN.h>
#include <DASN_app.h>
#include "util.h"
#include <icall.h>
//#include <xdc/runtime/Log.h> // Comment this in to use xdc.runtime.Log
#include <uartlog/UartLog.h>  // Comment out if using xdc Log

// Struct for message about button state
typedef struct
{
    PIN_Id pinId;
    uint8_t state;
} DASN_ButtonState_t;

typedef enum {
    DASN_UI_OK = 0,
    DASN_UI_ERROR1,         // Error initializing board LED pins
    DASN_UI_ERROR2,         // Error initializing button pins
    DASN_UI_ERROR3,
    DASN_UI_PINID_INVALID,
    DASN_UI_STATE_INVALID
}DASN_UI_returns_t;

typedef enum {
    UI_DASN_ON,
    UI_DASN_OFF,
    UI_STAND_BY,
    UI_CONNECTED_BLE,
    UI_CONNECTED_485,

    STATES_COUNT
}DASN_UI_states_t;

#define UI_LED_ON   1
#define UI_LED_OFF  0

static void buttonDebounceSwiFxn(UArg buttonId);
static void buttonCallbackFxn(PIN_Handle handle,PIN_Id pinId);
static void LedSwiFxn(UArg ledId);
DASN_UI_returns_t DASN_UI_init(void);
DASN_UI_returns_t DASN_UI_toogleLed(PIN_Id led);
DASN_UI_returns_t DASN_UI_update(DASN_UI_states_t state);

#endif /* APPLICATION_DASN_UI_H_ */
