/*
 * UI.h
 *
 *  Created on: Feb 19, 2022
 *      Author: leandro
 */

#ifndef APPLICATION_UI_H_
#define APPLICATION_UI_H_

typedef enum {
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



#endif /* APPLICATION_UI_H_ */
