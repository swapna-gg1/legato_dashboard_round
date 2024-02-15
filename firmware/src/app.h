/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "gfx/legato/generated/le_gen_init.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

//Layer and canvas ID
#define BACKGROUND_LAYER_ID 0
#define NEEDLE_LAYER_ID 1
#define GPS_LAYER_ID 2
#define BLUR_LAYER_ID 3
    
#define BACKGROUND_CANVAS_ID 0
#define NEEDLE_CANVAS_ID 1
#define GPS_CANVAS_ID 2
#define BLUR_CANVAS_ID 3

    
#define FADE_IN_START_ALPHA 0
#define FADE_IN_END_ALPHA 255    
    
#define SWIPEX_THRESHOLD_PIXELS 10
#define SWIPEY_THRESHOLD_PIXELS 10
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    leImage * imgAst;
    uint16_t x;
    uint16_t y;
    uint16_t speed;
} needleObj;    


typedef enum
{
    TWIRL_ACCELERATE_START=0,
    TWIRL_ACCELERATE_PROGRESS,
    TWIRL_DECELERATE_PROGRESS,
    DRIVE_START,
    DRIVE_PROGRESS_ACCEL,
    DRIVE_PROGRESS_DECEL,

} NEEDLE_STATE;

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/
typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,
    APP_STATE_SHOW_BASE,
    APP_STATE_INIT_NEEDLE_SHOW,
    APP_STATE_NEEDLE_TWIRL,
    APP_STATE_FADEOUT_ICON,
    APP_STATE_SPEED_INIT1,   
    APP_STATE_SPEED_INIT2,
    APP_STATE_DRIVE,
    APP_STATE_PAUSE,
    APP_STATE_REACHED,
    APP_STATE_LOOPBACK,

} APP_STATES;

typedef enum
{
    RIGHT=1,
    LEFT,
    DESTINATION,
} DIRECTION;

typedef enum
{
    NEXT=0,
    STRAIGHT,
    NO_CHANGE,
    REACHED,
}DIRECTION_STATUS;

typedef struct
{
    float dist;
    float prev_dist;
    float current_dist;
    uint8_t nextDirection;
} locationOnMap;  


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;
    NEEDLE_STATE nstate;

    /* TODO: Define any additional data used by the application. */

} APP_DATA;

extern needleObj speedoNeedles[121];
extern APP_DATA appData;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );


void APP_InitDrive( void );

void appSetNeedleImageData(leImage * imgAst,
                           uint32_t index,
                           uint32_t x,
                           uint32_t y,
                           uint32_t angle);
void APP_ProcessNeedle(void);
void APP_ProcessMap(void);
void updateNeedle(uint8_t nIndex);
void updateSpeedLabel(uint8_t nIndex);
void checkNeedleAnime(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

