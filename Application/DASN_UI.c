/*
 * DASN_UI_Module.c
 *
 *  Created on: Aug 14, 2022
 *      Author: leandroarrieta@gmail.com
 */

#include <DASN_UI.h>

/* Pin driver handles */
static PIN_Handle buttonPinHandle;
static PIN_Handle ledPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State buttonPinState;
static PIN_State ledPinState;

/*
 * Initial LED pin configuration table
 *   - LEDs Board_PIN_LED0 & Board_PIN_LED1 are off.
 */
PIN_Config ledPinTable[] = {
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL |
    PIN_DRVSTR_MAX,
    Board_PIN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL |
    PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/*
 * Application button pin configuration table:
 *   - Button interrupt is configured to trigger on falling edge.
 */
PIN_Config buttonPinTable[] = {
    Board_PIN_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};

// Clock objects for debouncing the buttons
static Clock_Struct button0DebounceClock;
static Clock_Handle button0DebounceClockHandle;

// State of the button
static uint8_t button0State = 0;

// Clock objects for Led1 timing.
static Clock_Struct Led1Clock;
static Clock_Handle Led1ClockHandle;

/*********************************************************************
 * @fn DASN_UI_init
 *
 * @brief   Inicializa la interfaz de usuario, los leds y el pulsador
 *
 * @param   No tiene
 *
 * @return  Devuelve DASN_UI_OK si salió todo bien, sino devuelve el
 *          codigo de error correspondiente.
 */
DASN_UI_returns_t DASN_UI_init(void)
{
    // Open LED pins
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle)
    {
        Log_error0("Error initializing board LED pins");
        return DASN_UI_ERROR1;
    }

    // Open button pins
    buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
    if(!buttonPinHandle)
    {
        Log_error0("Error initializing button pins");
        return DASN_UI_ERROR2;
    }

    // Setup callback for button pins
    if(PIN_registerIntCb(buttonPinHandle, &buttonCallbackFxn) != 0)
    {
        Log_error0("Error registering button callback function");
        return DASN_UI_ERROR3;
    }

    // Create the debounce clock objects for Button 0
    button0DebounceClockHandle = Util_constructClock(&button0DebounceClock,
                                                     buttonDebounceSwiFxn, 50,
                                                     0,
                                                     0,
                                                     Board_PIN_BUTTON0);
    // Create clock objects for Led1 timing
    Led1ClockHandle = Util_constructClock(&Led1Clock,
                                          LedSwiFxn, 200,
                                          0,
                                          0,
                                          Board_PIN_LED1);


    return DASN_UI_OK;
}

/*********************************************************************
 * @fn     LedSwiFxn
 *
 * @brief  Callback from Clock module on timeout
 *
 *         Toogle state led after clock finish
 *
 * @param  LedId    The Led being used
 */
static void LedSwiFxn(UArg ledId)
{
    DASN_UI_toogleLed(ledId);
    Util_startClock((Clock_Struct *)Led1ClockHandle);
}

/*********************************************************************
 * @fn     buttonDebounceSwiFxn
 *
 * @brief  Callback from Clock module on timeout
 *
 *         Determines new state after debouncing
 *
 * @param  buttonId    The pin being debounced
 */
static void buttonDebounceSwiFxn(UArg buttonId)
{
    Log_info0("Entro a buttonDebounceSwiFxn ");

    // Used to send message to app
    DASN_ButtonState_t buttonMsg = { .pinId = buttonId };
    uint8_t sendMsg = FALSE;

    // Get current value of the button pin after the clock timeout
    Log_info1("button0State = %d", button0State );
    uint8_t buttonPinVal = PIN_getInputValue(buttonId);

    Log_info1("buttonPinVal = %d", buttonPinVal );


    // Set interrupt direction to opposite of debounced state
    // If button is now released (button is active low, so release is high)
    if(buttonPinVal)
    {
        // Enable negative edge interrupts to wait for press
        PIN_setConfig(buttonPinHandle, PIN_BM_IRQ, buttonId | PIN_IRQ_NEGEDGE);
    }
    else
    {
        // Enable positive edge interrupts to wait for relesae
        PIN_setConfig(buttonPinHandle, PIN_BM_IRQ, buttonId | PIN_IRQ_POSEDGE);
    }

    switch(buttonId)
    {
    case Board_PIN_BUTTON0:
        // If button is now released (buttonPinVal is active low, so release is 1)
        // and button state was pressed (buttonstate is active high so press is 1)
        if(buttonPinVal && button0State)
        {
            // Button was released
            Log_info0("SWI: Button was released");
            buttonMsg.state = button0State = 0;
            sendMsg = TRUE;
        }
        else if(!buttonPinVal && !button0State)
        {
            // Button was pressed
            Log_info0("SWI: Button was pressed");
            buttonMsg.state = button0State = 1;
            sendMsg = TRUE;
        }
        break;
    }

    if(sendMsg == TRUE)
    {
        Log_info0("SWI: Mando ICall");
        DASN_ButtonState_t *pButtonState = ICall_malloc(sizeof(DASN_ButtonState_t));
        if(pButtonState != NULL)
        {
            *pButtonState = buttonMsg;
            if(DASN_enqueueMsg(DASN_BUTTON_DEBOUNCED_EVT, pButtonState) != SUCCESS)
            {
              ICall_free(pButtonState);
            }
        }
    }
}

/*********************************************************************
 * @fn     buttonCallbackFxn
 *
 * @brief  Callback from PIN driver on interrupt
 *
 *         Sets in motion the debouncing.
 *
 * @param  handle    The PIN_Handle instance this is about
 * @param  pinId     The pin that generated the interrupt
 */
static void buttonCallbackFxn(PIN_Handle handle, PIN_Id pinId)
{
    Log_info1("Button interrupt: %s",
              (uintptr_t)((pinId == Board_PIN_BUTTON0) ? "Button 0" : "Button 1"));

    // Disable interrupt on that pin for now. Re-enabled after debounce.
    PIN_setConfig(handle, PIN_BM_IRQ, pinId | PIN_IRQ_DIS);

    // Start debounce timer
    switch(pinId)
    {
    case Board_PIN_BUTTON0:
        Util_startClock((Clock_Struct *)button0DebounceClockHandle);
        break;

    }
}

/*********************************************************************
 * @fn      DASN_UI_toogleLed
 *
 * @brief   Invierte el estado del led
 *
 * @param   led     led a invertir el estado, el led tiene que ser uno
 *                  de los definidos en Board.h
 *
 * @return  Devuelve DASN_UI_OK si salió todo bien, sino devuelve el
 *          codigo de error correspondiente.
 */
static DASN_UI_returns_t DASN_UI_toogleLed(PIN_Id ledId)
{
    if (ledId == Board_PIN_LED1 || ledId == Board_PIN_LED2)
    {
        PIN_setOutputValue(ledPinHandle, ledId, !PIN_getOutputValue(ledId));
        return DASN_UI_OK;
    }
    else
        return DASN_UI_PINID_INVALID;

}

/*********************************************************************
 * @fn      DASN_UI_update
 *
 * @brief   Actualiza el estado de la UI.
 *
 * @param   state   Le indico en que estado quiero la UI
 *
 * @return  Devuelve DASN_UI_OK si salió todo bien, sino devuelve el
 *          codigo de error correspondiente.
 */
DASN_UI_returns_t DASN_UI_update(DASN_UI_states_t state)
{
    if(state > STATES_COUNT)
        return DASN_UI_STATE_INVALID;

    switch(state)
    {
    case UI_STAND_BY:
        Util_stopClock((Clock_Struct *)Led1ClockHandle);
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, UI_LED_OFF);
        break;

    case UI_CONNECTED_BLE:
        Util_startClock((Clock_Struct *)Led1ClockHandle);
        break;

    case UI_CONNECTED_485:
        Util_stopClock((Clock_Struct *)Led1ClockHandle);
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, UI_LED_ON);
        break;

    case UI_DASN_ON:
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED2, UI_LED_ON);
        break;

    case UI_DASN_OFF:
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED2, UI_LED_OFF);
        Util_stopClock((Clock_Struct *)Led1ClockHandle);
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED1, UI_LED_OFF);
        break;

    case UI_TEST_LED1:
        DASN_UI_toogleLed(Board_PIN_LED1);
        break;

    default:
        return DASN_UI_STATE_INVALID;
    }

    return DASN_UI_OK;
}

