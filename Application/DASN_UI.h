/*
 * UI.h
 *
 *  Created on: Feb 19, 2022
 *      Author: leandro
 */

#ifndef APPLICATION_DASN_UI_H_
#define APPLICATION_DASN_UI_H_
#include <ti/sysbios/knl/Task.h>

void DASN_UI_createTask(void);

// List of events to manage
#define UI_ALL_EVENTS          0x1F
typedef enum {

    UI_ACQ_BLE = 1, //!<  1-
    UI_2,      //!<  2-
    UI_3,      //!<  3-
    UI_4,      //!<  4-
    UI_5,      //!<  5-
    UI_6,      //!<  6-
    UI_7,      //!<  7-
    UI_8,      //!<  8-
    UI_9,      //!<  9-
    UI_A,      //!<  A-
    UI_B,      //!<  B-
    UI_C,      //!<  C-
    UI_D,      //!<  D-
    UI_E,      //!<  E-
    UI_F,      //!<  F-
    UI_10,      //!< 10-
    UI_11,      //!< 11-
    UI_12,      //!< 12-
    UI_13,      //!< 13-
    UI_14,      //!< 14-
    UI_15,      //!< 15-
    UI_16,      //!< 16-
    UI_17,      //!< 17-
    UI_18,      //!< 18-
    UI_19,      //!< 19-
    UI_1A,      //!< 1A-
    UI_1B,      //!< 1B-
    UI_1C,      //!< 1C-
    UI_1D,      //!< 1D-
    UI_1E,      //!< 1E-
    UI_1F,      //!< 1F-

}UIcmd_t;

#endif /* APPLICATION_DASN_UI_H_ */
