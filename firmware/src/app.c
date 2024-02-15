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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "gfx/legato/string/legato_fixedstring.h"
#include "gfx/canvas/gfx_canvas_api.h"
#include "peripheral/tc/plib_tc1.h"
#include <stdio.h>

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
#define MAX_SPEED_STRING_LEN     3
#define FADE_IN_START_ALPHA      0
#define FADE_IN_END_ALPHA        255

#define BLUR_ALPHA_LOW         100
#define BLUR_ALPHA_HIGH        255


#define MAX_SPEED                156
#define MIN_SPEED                16

APP_DATA appData;
needleObj speedoNeedles[121];



static uint32_t prev_tick, tick =0;
static uint32_t prev_sec_tick, sec_tick =0;

//Legato string object
leFixedString p_speedstring;


//Legato Char buffer
static leChar p_legatoSpeedBuff[MAX_SPEED_STRING_LEN] = {0};


//C character buffer
static char p_speedcharbuff[MAX_SPEED_STRING_LEN];

static bool blur_alpha_high;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/
void Screen0_OnShow(void)
{                      
 
    gfxcShowCanvas(BACKGROUND_CANVAS_ID);
    gfxcSetWindowAlpha(BACKGROUND_CANVAS_ID, 0);
    gfxcCanvasUpdate(BACKGROUND_CANVAS_ID); 
    
    gfxcShowCanvas(GPS_CANVAS_ID);
    gfxcSetWindowAlpha(GPS_CANVAS_ID, 0);
    gfxcCanvasUpdate(GPS_CANVAS_ID);   
    
    gfxcShowCanvas(BLUR_CANVAS_ID);
    gfxcSetWindowAlpha(BLUR_CANVAS_ID, 0);
    gfxcCanvasUpdate(BLUR_CANVAS_ID); 
    
    TC1_CH0_TimerStart();
}

void Screen0_OnUpdate(void)
{
  
}

void TC1_CH1_TimerInterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    tick++;
}

void TC1_CH0_TimerInterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    sec_tick++;
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void appSetNeedleImageData(leImage * imgAst,
                           uint32_t index,
                           uint32_t x,
                           uint32_t y,
                           uint32_t angle)
{
    speedoNeedles[index].x = x;
    speedoNeedles[index].y = y;
    speedoNeedles[index].imgAst = imgAst;
    speedoNeedles[index].speed = angle;
}

void updateNeedle(uint8_t nIndex)
{
    gfxcSetPixelBuffer(NEEDLE_CANVAS_ID,
    speedoNeedles[nIndex].imgAst->buffer.size.width,
    speedoNeedles[nIndex].imgAst->buffer.size.height,
    GFX_COLOR_MODE_RGBA_8888,
    (void *) speedoNeedles[nIndex].imgAst->buffer.pixels);

    gfxcSetWindowPosition(NEEDLE_CANVAS_ID, speedoNeedles[nIndex].x, speedoNeedles[nIndex].y);
    gfxcCanvasUpdate(NEEDLE_CANVAS_ID);
            
}
void updateSpeedLabel(uint8_t nIndex)
{
    memset(p_speedcharbuff, 0, sizeof(p_speedcharbuff));
    sprintf(p_speedcharbuff, "%d", speedoNeedles[nIndex].speed);
    p_speedstring.fn->setFromCStr(&p_speedstring, p_speedcharbuff);
    Screen0_SpeedLabel->fn->setString(Screen0_SpeedLabel, (leString*)&p_speedstring);
    gfxcCanvasUpdate(GPS_CANVAS_ID);   
}


void pulseBlur(void)
{
    if(blur_alpha_high == true)
    {
        blur_alpha_high = false;
        gfxcStartEffectFade(BLUR_CANVAS_ID,
                                BLUR_ALPHA_HIGH,
                                BLUR_ALPHA_LOW,
                                10);
    }
    else
    {
        blur_alpha_high = true;
        gfxcStartEffectFade(BLUR_CANVAS_ID,
                                BLUR_ALPHA_LOW,
                                BLUR_ALPHA_HIGH,
                                10);
    }
}

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
    blur_alpha_high = true;
    APP_InitDrive();
    leFixedString_Constructor(&p_speedstring,  p_legatoSpeedBuff, MAX_SPEED_STRING_LEN *2);
    p_speedstring.fn->setFont(&p_speedstring, (leFont*)& NotoSansBoldI_48);
    
    
    TC1_CH0_TimerCallbackRegister(TC1_CH0_TimerInterruptHandler, (uintptr_t) NULL); 
    TC1_CH1_TimerCallbackRegister(TC1_CH1_TimerInterruptHandler, (uintptr_t) NULL); 
    
    
    /* Initialize canvas objects */
    gfxcSetLayer(BACKGROUND_CANVAS_ID, BACKGROUND_LAYER_ID);
    gfxcSetLayer(NEEDLE_CANVAS_ID, NEEDLE_LAYER_ID);
    gfxcSetLayer(GPS_CANVAS_ID, GPS_LAYER_ID);
    gfxcSetLayer(BLUR_CANVAS_ID, BLUR_LAYER_ID);
    

    
    gfxcSetWindowPosition(BACKGROUND_CANVAS_ID, 0, 0);
    gfxcSetWindowPosition(GPS_CANVAS_ID, 195, 265);
    gfxcSetWindowPosition(BLUR_CANVAS_ID, 0, 0);
    
    gfxcSetWindowSize(BACKGROUND_CANVAS_ID, 720, 720);
    gfxcSetWindowSize(NEEDLE_CANVAS_ID, 201, 195);
    gfxcSetWindowSize(GPS_CANVAS_ID, 340, 363);
    gfxcSetWindowSize(BLUR_CANVAS_ID, 720, 720);
    
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    static uint8_t  sec2=0;
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            
            if (appInitialized)
            {  
                if(sec_tick >2)
                {
                    sec_tick = 0;
                    appData.state = APP_STATE_SHOW_BASE;
                    TC1_CH0_TimerStop();
                    
                }
            }
            break;
        }
        case APP_STATE_SHOW_BASE:
        {
            gfxcStartEffectFade(BACKGROUND_CANVAS_ID,
                                FADE_IN_START_ALPHA,
                                FADE_IN_END_ALPHA,
                                10);
            gfxcStartEffectFade(GPS_CANVAS_ID,
                                FADE_IN_START_ALPHA,
                                FADE_IN_END_ALPHA,
                                10);
            gfxcStartEffectFade(BLUR_CANVAS_ID,
                                FADE_IN_START_ALPHA,
                                FADE_IN_END_ALPHA,
                                10);

            appData.state = APP_STATE_INIT_NEEDLE_SHOW;
          
            break;
        } 
        case APP_STATE_INIT_NEEDLE_SHOW:
        {
            gfxcShowCanvas(NEEDLE_CANVAS_ID); 
            gfxcSetPixelBuffer(NEEDLE_CANVAS_ID,
            speedoNeedles[0].imgAst->buffer.size.width,
            speedoNeedles[0].imgAst->buffer.size.height,
            GFX_COLOR_MODE_RGBA_8888,
            (void *) speedoNeedles[0].imgAst->buffer.pixels);
    
            gfxcSetWindowPosition(NEEDLE_CANVAS_ID, speedoNeedles[0].x, speedoNeedles[0].y);
            gfxcCanvasUpdate(NEEDLE_CANVAS_ID);    
            sec_tick = 0;
            //tick = 0;
            //prev_tick = 0;
            TC1_CH0_TimerStart();
            appData.state = APP_STATE_NEEDLE_TWIRL;   
            appData.nstate = TWIRL_ACCELERATE_START;
            break;
        }
        case APP_STATE_NEEDLE_TWIRL:
        {    
            if(sec_tick >1)
            {               
                TC1_CH0_TimerStop();               
                TC1_CH1_TimerStart();
                sec_tick=0;
            }
            if (tick != prev_tick)
            {               
                prev_tick = tick; 
                APP_ProcessNeedle();
            }
            break;
        }  
        case APP_STATE_FADEOUT_ICON:
        {            
            gfxcStartEffectFade(GPS_LAYER_ID,
                                FADE_IN_END_ALPHA,
                                FADE_IN_START_ALPHA,
                                50);
            
            Screen0_MCHPLogoImage->fn->setVisible(Screen0_MCHPLogoImage, LE_FALSE);    
            Screen0_InstructLabel->fn->setVisible(Screen0_InstructLabel, LE_FALSE);
            TC1_CH0_TimerStart();
            appData.state = APP_STATE_SPEED_INIT1;

            break;
        }
        case APP_STATE_SPEED_INIT1:
        {
            if(sec_tick > 0 )
            {                               
                sec_tick=0;
                Screen0_SpeedUnitLabel->fn->setVisible(Screen0_SpeedUnitLabel, LE_TRUE);
                Screen0_SpeedLabel->fn->setVisible(Screen0_SpeedLabel, LE_TRUE);
                gfxcStartEffectFade(GPS_LAYER_ID,
                                FADE_IN_START_ALPHA,
                                FADE_IN_END_ALPHA,
                                10);
                
                
                appData.state = APP_STATE_SPEED_INIT2;
                
            }
          
            break;
        }
        case APP_STATE_SPEED_INIT2:
        {
            if(sec_tick > 1 )
            {   
                appData.nstate = DRIVE_START;
                appData.state = APP_STATE_DRIVE;
                
            }
            break;
        }

        case APP_STATE_DRIVE:
        {
            APP_ProcessNeedle();
            
            if (sec_tick != prev_sec_tick)
            {   sec2++;            
                prev_sec_tick = sec_tick; 
                APP_ProcessMap();
                checkNeedleAnime();
                if(sec2>1)
                {
                    sec2=0;
                    pulseBlur();
                }
            }
            break;
        }
        case APP_STATE_PAUSE:
        {
            sec_tick = 0;
            appData.state = APP_STATE_REACHED;
            if(blur_alpha_high==false)
            {
                blur_alpha_high = true;
                gfxcStartEffectFade(BLUR_CANVAS_ID,
                                        BLUR_ALPHA_LOW,
                                        BLUR_ALPHA_HIGH,
                                        50);
            }
            break;
        }
        case APP_STATE_REACHED:
        {
            if(sec_tick > 2 )
            {
                /* Reset the demo so we can restart everything */
                sec_tick = 0;
                gfxcStartEffectFade(BACKGROUND_CANVAS_ID,
                                    FADE_IN_END_ALPHA,
                                    FADE_IN_START_ALPHA,
                                    10);
                gfxcStartEffectFade(GPS_CANVAS_ID,
                                    FADE_IN_END_ALPHA,
                                    FADE_IN_START_ALPHA,
                                    10);
                gfxcStartEffectFade(BLUR_CANVAS_ID,
                                    FADE_IN_END_ALPHA,
                                    FADE_IN_START_ALPHA,
                                    10);

                appData.state = APP_STATE_LOOPBACK;
                Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_FALSE);
                gfxcHideCanvas(NEEDLE_CANVAS_ID); 
                gfxcCanvasUpdate(NEEDLE_CANVAS_ID);    

            }
            break;
        }
        case APP_STATE_LOOPBACK:
        {
            if(sec_tick > 2 )
            {
                Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Instruction);
                Screen0_MCHPLogoImage->fn->setVisible(Screen0_MCHPLogoImage, LE_TRUE);
                Screen0_SpeedLabel->fn->setVisible(Screen0_SpeedLabel, LE_FALSE);
                Screen0_SpeedUnitLabel->fn->setVisible(Screen0_SpeedUnitLabel, LE_FALSE);
                appData.state = APP_STATE_SHOW_BASE;
                TC1_CH0_TimerStop();
                TC1_CH1_TimerStop();
            }

        }
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
