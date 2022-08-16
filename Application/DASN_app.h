/******************************************************************************

   @file  DASN_app.h

   @brief This file contains the Project Zero sample application
        definitions and prototypes.

   Group: CMCU, LPRF
   Target Device: cc2640r2

 ******************************************************************************
   
 Copyright (c) 2015-2021, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
   
   
 *****************************************************************************/

#ifndef DASN_APP_H
#define DASN_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <ti/sysbios/knl/Queue.h>

#include <bcomdef.h>

/*********************************************************************
 *  EXTERNAL VARIABLES
 */

/*********************************************************************
 * CONSTANTS
 */
// Types of messages that can be sent to the user application task from other
// tasks or interrupts. Note: Messages from BLE Stack are sent differently.
#define DASN_SERVICE_WRITE_EVT   0  /* A characteristic value has been written     */
#define DASN_SERVICE_CFG_EVT     1  /* A characteristic configuration has changed  */
#define PZ_UPDATE_CHARVAL_EVT    2  /* Request from ourselves to update a value    */
#define DASN_BUTTON_DEBOUNCED_EVT 3 /* A button has been debounced with new value  */
#define PZ_PAIRSTATE_EVT         4  /* The pairing state is updated                */
#define PZ_PASSCODE_EVT          5  /* A pass-code/PIN is requested during pairing */
#define PZ_ADV_EVT               6  /* A subscribed advertisement activity         */
#define PZ_START_ADV_EVT         7  /* Request advertisement start from task ctx   */
#define PZ_SEND_PARAM_UPD_EVT    8  /* Request parameter update req be sent        */
#define PZ_CONN_EVT              9  /* Connection Event End notice                 */
//#define DASN_NEW_DATA_EVT      10 /* New data from ADS1299, defined in DASN_ADS1299.h */
//#define DASN_TRANSFER_COMPLETE 11

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * FUNCTIONS
 */
bStatus_t DASN_enqueueMsg(uint8_t event, void *pData);
/*
 * Task creation function for the Project Zero.
 */
void DASN_createTask(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* APP_H */
