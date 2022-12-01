/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "Mc32DriverLcd.h"
#include "Mc32DriverAdc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;


// *****************************************************************************
/* Application Data

  Summary:
    Holds leds informations

  Description:
    This structure holds the leds variables data.

  Remarks:
    This structure should be used in BSP_LED... functions
*/

BSP_LED appLeds[8] = {
    BSP_LED_0,
    BSP_LED_1,
    BSP_LED_2,
    BSP_LED_3,
    BSP_LED_4,
    BSP_LED_5,
    BSP_LED_6,
    BSP_LED_7
};


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Iterance through the array that store LEDS values */
    static uint8_t i_leds = 0;
    
    /* Flag the 1st cycle of service tasks */
    static bool FLAG_1ST_CYCLE = true;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Custom libraries init */
            lcd_init();
            BSP_InitADC10();
            
            /* Timer 1 start */
            DRV_TMR0_Start();

            /* Initial text display */
            lcd_gotoxy(1, 1);
            printf_lcd("TP0 LED+AD 2022-23");
            lcd_gotoxy(1, 2);
            printf_lcd("Miguel Santos");
            lcd_bl_on();           
            
            /* Set all leds ON */
            for(i_leds = 0; i_leds <= 7; i_leds++)
                BSP_LEDOn(appLeds[i_leds]);

            /* Change to next state */
            appData.state = APP_STATE_WAIT;
            break;
        }
        
        case APP_STATE_WAIT:
        {
            /* Nothing should be done here */
            break;
        }

        /* Application's actions state */
        case APP_STATE_SERVICE_TASKS:
        {    
            appData.AdcRes = BSP_ReadAllADC();
            lcd_gotoxy(1, 3);
            printf_lcd("CH0 %.4d CH1 %.4d", appData.AdcRes.Chan0, appData.AdcRes.Chan1);
            
            /* Code to execute only the first cycle */
            if(FLAG_1ST_CYCLE)
            {
                for(i_leds = 0; i_leds <= 7; i_leds++)
                    BSP_LEDOff(appLeds[i_leds]);
                
                FLAG_1ST_CYCLE = false;
            }
            
            /* Turn OFF previous LED */
            BSP_LEDOff(appLeds[i_leds]);
            
            /* Increment LED */
            if(i_leds >= 7)
                i_leds = 0;
            else
                i_leds++;
            
            /* Turn ON current LED*/
            BSP_LEDOn(appLeds[i_leds]);
            
            /* Change to next state */
            appData.state = APP_STATE_WAIT;
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/******************************************************************************
  Function:
    void APP_UpdateState( APP_STATES NewState )

  Remarks:
    See prototype in app.h.
 */

void APP_UpdateState(APP_STATES NewState)
{
    appData.state = NewState;
}

/*******************************************************************************
 End of File
 */
