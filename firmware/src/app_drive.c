#include "app.h"
#include "gfx/legato/string/legato_fixedstring.h"
#include "gfx/canvas/gfx_canvas_api.h"
#include "system/input/sys_input.h"
#include <stdio.h>

#define MAX_DIST_STRING_LEN      6
#define MAX_DIST_TRVL_DIGITS     4
#define DIST_TRAVEL_THRSH_KM     6

const float totDist = 213;
float travlDist = 0;

static SYS_INP_InputListener app_inputListener;

locationOnMap maplocations[8]={{20, 0, 0, RIGHT},{42, 0, 0, LEFT},{22, 0, 0, LEFT},
                                      {36, 0, 0, RIGHT},{16, 0, 0, RIGHT},{32, 0, 0, LEFT},
                                      {20, 0, 0, RIGHT},{25, 0, 0, DESTINATION}};

locationOnMap currentLocation;

uint8_t needleIndex, animeIndex;
uint8_t mapIndex = 0;
uint8_t req_speed;

//Legato string object
leFixedString p_diststring;
leFixedString p_distTravlstring;

//Legato Char buffer
static leChar p_legatoDistBuff[MAX_DIST_STRING_LEN] = {0};
static leChar p_legatoDistTravlBuff[MAX_DIST_TRVL_DIGITS] = {0};

//C character buffer
static char p_distcharbuff[MAX_DIST_STRING_LEN];
static char p_distTrvlcharbuff[MAX_DIST_TRVL_DIGITS];

//bool updateDirection = true;
bool updateDistance = true;
uint8_t directionStatus;
/* Tracking direction Image */
bool dImageOn = true;

static int lastX, lastY;
uint8_t MAP_CNTR = 0, ANIME_CNTR=0;
float sec = 0.0166666666666667;

struct needleAnime 
{
    /* To track required  change and acceleration/deceleration */
    uint8_t regSpeed;           
    uint8_t aChange;       
}; 

struct needleAnime fixedAnime[10]= {{48, DRIVE_PROGRESS_ACCEL}, {72, DRIVE_PROGRESS_ACCEL},
{56, DRIVE_PROGRESS_DECEL}, {64,DRIVE_PROGRESS_ACCEL}, {36, DRIVE_PROGRESS_DECEL},
{24, DRIVE_PROGRESS_DECEL}, {72,DRIVE_PROGRESS_ACCEL}, {77,DRIVE_PROGRESS_ACCEL}, 
{60,DRIVE_PROGRESS_DECEL}, {24,DRIVE_PROGRESS_DECEL}};

static void app_touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
    if(appData.state == APP_STATE_DRIVE)
    {
        lastX = evt->x;
        lastY = evt->y;
        ANIME_CNTR =0;
    }

}

static void app_touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
    if(appData.state == APP_STATE_DRIVE)
    {
        lastX = evt->x;
        lastY = evt->y;
        ANIME_CNTR = 0;
    }  
}

static void app_touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
    ANIME_CNTR = 0;
    //we don't want to increase speed if destination has already been reached
    if((appData.state == APP_STATE_DRIVE)  && (directionStatus != REACHED))
    {
        int diffX = abs(evt->x - lastX);
        int diffY = abs(lastY - evt->y);
        
        //if(((evt->x - lastX) > SWIPEX_THRESHOLD_PIXELS) || ((lastY - evt->y)> SWIPEY_THRESHOLD_PIXELS))      
        //if((evt->x - lastX) > SWIPEX_THRESHOLD_PIXELS)
        if (evt->y < lastY)
        {
            if(needleIndex < 120)           
            {
                //int newIndx = needleIndex+16;
                int newIndx = needleIndex + (diffX + diffY)/3;
                if(newIndx > 120)
                    newIndx = 120;
                req_speed = speedoNeedles[newIndx].speed;
                appData.nstate = DRIVE_PROGRESS_ACCEL;
            }
          
        }
        //if(((lastX - evt->x) > SWIPEX_THRESHOLD_PIXELS) || ((evt->y-lastY)> SWIPEY_THRESHOLD_PIXELS))
        //if((lastX - evt->x) > SWIPEX_THRESHOLD_PIXELS)
        else
        {
            if( needleIndex > 0 )
            {
                //int newIndx = needleIndex-16;
                int newIndx = needleIndex - (diffX + diffY)/3;
                if(newIndx < 0)
                    newIndx = 0;
                req_speed = speedoNeedles[(newIndx)].speed;
                appData.nstate = DRIVE_PROGRESS_DECEL;
            }
        }
        lastX = evt->x;
        lastY = evt->y;
    }
}


void APP_InitMap( void )
{
    currentLocation = maplocations[0];
    mapIndex = 0;
}

void APP_ProcessNeedle(void)
{
    switch ( appData.nstate)
    {
        case TWIRL_ACCELERATE_START:
        {
            directionStatus = STRAIGHT;
            needleIndex = 0;
            req_speed = 77;
            appData.nstate = TWIRL_ACCELERATE_PROGRESS;
            break;
        }
        case TWIRL_ACCELERATE_PROGRESS:
        {

            if(speedoNeedles[needleIndex].speed < req_speed)
            {
                needleIndex++;
                updateNeedle(needleIndex);
            }else
            {
                req_speed = 0;
                appData.nstate = TWIRL_DECELERATE_PROGRESS;
            }
            
            break;
        }
        case TWIRL_DECELERATE_PROGRESS:
        {

            if(speedoNeedles[needleIndex].speed > req_speed)
            {                   
                needleIndex--;
                updateNeedle(needleIndex);
            }
            else
            {
                needleIndex = 0;
                appData.state = APP_STATE_FADEOUT_ICON;
            }

            break;
        }
        case DRIVE_START:
        {
            travlDist = 0;
            animeIndex = 0;
            needleIndex = 0;
            req_speed = 44;
            APP_InitMap();
            appData.nstate = DRIVE_PROGRESS_ACCEL;
            
            sprintf(p_distTrvlcharbuff, "240");
            p_distTravlstring.fn->setFromCStr(&p_distTravlstring, p_distTrvlcharbuff);
            Screen0_RTLabel->fn->setString(Screen0_RTLabel, (leString*)&p_distTravlstring);
        
        
            Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&StA);
            Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_TRUE);
            Screen0_DistLabel->fn->setVisible(Screen0_DistLabel, LE_TRUE);
            Screen0_InstructLabel->fn->setVisible(Screen0_InstructLabel, LE_TRUE);
            Screen0_RTimeLabel->fn->setVisible(Screen0_RTimeLabel, LE_TRUE);
            break;
        }
        case DRIVE_PROGRESS_ACCEL:
        {
            if(speedoNeedles[needleIndex].speed < req_speed)
            {
                needleIndex++;
                updateNeedle(needleIndex);
                updateSpeedLabel(needleIndex);
            }
            break;
        }
        case DRIVE_PROGRESS_DECEL:
        {
            if(speedoNeedles[needleIndex].speed > req_speed)
            {          
                needleIndex--;
                updateNeedle(needleIndex);
                updateSpeedLabel(needleIndex);
            }
            else
            {
                if(directionStatus == REACHED)
                {
                    appData.state = APP_STATE_PAUSE;
                    
                }
            }
            break;
        }
    }
    
}

void updateMapLocation(void)
{
    /* D = S x T */
    currentLocation.prev_dist = currentLocation.current_dist;
    currentLocation.current_dist = currentLocation.prev_dist+ (float)(speedoNeedles[needleIndex].speed*sec);    
    /* Every 3 s update the distance label */
    MAP_CNTR++;
    if(MAP_CNTR > 2)
    {
        MAP_CNTR = 0;
        updateDistance = true;
    }

    if((currentLocation.dist - currentLocation.current_dist) < DIST_TRAVEL_THRSH_KM)
    {
        directionStatus = NEXT;
        updateDistance = true;
    }
    if((currentLocation.dist - currentLocation.current_dist) <= 0)
    {
        /* Go to next path in map unless you have reached destination */
        if(currentLocation.nextDirection != DESTINATION)
        {
            printf("X\r\n");
            travlDist = travlDist + currentLocation.dist;
            currentLocation = maplocations[++mapIndex];
            directionStatus = STRAIGHT;
            updateDistance = true;
        }
        else
        {
            directionStatus = REACHED;
            Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Arrived);
            Screen0_DistLabel->fn->setVisible(Screen0_DistLabel, LE_FALSE);
            Screen0_RTimeLabel->fn->setVisible(Screen0_RTimeLabel, LE_FALSE);
            Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&destinationIcon);
            gfxcCanvasUpdate(GPS_CANVAS_ID);   
            req_speed = 0;
            appData.nstate = DRIVE_PROGRESS_DECEL;

        }
    }
}
float c, a, b, t;

void APP_ProcessMap(void)
{
    if(directionStatus == REACHED)
    {
        Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_TRUE);
        return;
    }
    if(directionStatus == STRAIGHT)
    {
        Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Straight);
        Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&StA);
        directionStatus = NO_CHANGE;
    }
    if(directionStatus == NEXT)
    {
        
        if(currentLocation.nextDirection == RIGHT)
        {
            Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Right);
            Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&RtA);
            directionStatus = NO_CHANGE;
            
        }
        if(currentLocation.nextDirection == LEFT)
        {
            Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Left);
            Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&LtA);
            directionStatus = NO_CHANGE;
            
        }
        
        gfxcCanvasUpdate(GPS_CANVAS_ID);   
    }
    if(updateDistance)
    {
        updateDistance = false;
        memset(p_distcharbuff, 0, sizeof(p_distcharbuff));
        c= currentLocation.dist - currentLocation.current_dist;
        sprintf(p_distcharbuff, "%d km",(int) c);
        
        p_diststring.fn->setFromCStr(&p_diststring, p_distcharbuff);
        Screen0_DistLabel->fn->setString(Screen0_DistLabel, (leString*)&p_diststring);
        a = travlDist + currentLocation.current_dist;
        b = totDist - a;
        //printf("<%f \r\n",a );
        //printf("!%f\r\n", b );
        //update time to destination
        //t = (b /req_speed)* 60;
        t = b;
        //printf("s %d t %d\r\n",req_speed, (int)t);
        memset(p_distTrvlcharbuff, 0, sizeof(p_distTrvlcharbuff));
        sprintf(p_distTrvlcharbuff, "%d",(int) t);       
        p_distTravlstring.fn->setFromCStr(&p_distTravlstring, p_distTrvlcharbuff);
        Screen0_RTLabel->fn->setString(Screen0_RTLabel, (leString*)&p_distTravlstring);
        
        
                
        gfxcCanvasUpdate(GPS_CANVAS_ID);   
        
    }
    updateMapLocation();
    if(dImageOn)
    {
        Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_FALSE);
        dImageOn = false;
    }
    else
    {
        Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_TRUE);
        dImageOn = true;
    }
   
}

void checkNeedleAnime(void)
{
    if(directionStatus != REACHED)
    {
        ANIME_CNTR++;
        if(ANIME_CNTR > 2)
        {
            //printf("*");
            ANIME_CNTR=0;
            req_speed = fixedAnime[animeIndex].regSpeed;
            appData.nstate = fixedAnime[animeIndex].aChange;
            if(++animeIndex > 8)
                animeIndex = 0;
        }
    }  
}

void APP_initialize_needle_lookup_table(void)
{
    memset(speedoNeedles, 0, sizeof(speedoNeedles));
    appSetNeedleImageData(&N0_157_422, 0, 157, 422, 0);
    appSetNeedleImageData(&N2_151_420, 1, 151, 420, 2);
    appSetNeedleImageData(&N4_145_419, 2, 145, 419, 4);
    appSetNeedleImageData(&N6_133_412, 3, 133, 412, 6);
    appSetNeedleImageData(&N8_132_410, 4, 132, 410, 8);
    appSetNeedleImageData(&N10_126_406, 5, 126, 406, 10);
    appSetNeedleImageData(&N12_111_407, 6, 111, 407, 12);
    appSetNeedleImageData(&N14_110_394, 7, 110, 394, 14);
    appSetNeedleImageData(&N16_103_374, 8, 103, 374, 16);
    appSetNeedleImageData(&N18_102_393, 9, 102, 393, 18);
    appSetNeedleImageData(&N20_92_382, 10, 92, 382, 20);
    appSetNeedleImageData(&N22_92_378, 11, 92, 378, 22);
    appSetNeedleImageData(&N24_89_379, 12, 89, 379, 24);
    appSetNeedleImageData(&N26_85_368, 13, 85, 368, 26);
    appSetNeedleImageData(&N28_81_367, 14, 81, 367, 28);
    appSetNeedleImageData(&N30_80_362, 15, 80, 362, 30);
    appSetNeedleImageData(&N32_80_363, 16, 80, 363, 32);
    appSetNeedleImageData(&N34_77_357, 17, 77, 357, 34);
    appSetNeedleImageData(&N36_79_347, 18, 79, 347, 36);
    appSetNeedleImageData(&N38_110_354, 19, 110, 354, 38);
    appSetNeedleImageData(&N40_74_350, 20, 74, 350, 40);
    appSetNeedleImageData(&N42_74_341, 21, 74, 341, 42);
    appSetNeedleImageData(&N44_74_335, 22, 74, 335, 44);
    appSetNeedleImageData(&N46_76_323, 23, 76, 323, 46);
    appSetNeedleImageData(&N48_74_316, 24, 74, 316, 48);
    appSetNeedleImageData(&N50_75_296, 25, 75, 296, 50);
    appSetNeedleImageData(&N52_77_286, 26, 77, 286, 52);
    appSetNeedleImageData(&N54_83_281, 27, 83, 281, 54);
    appSetNeedleImageData(&N56_86_271, 28, 86, 271, 56);
    appSetNeedleImageData(&N58_87_258, 29, 87, 258, 58);
    appSetNeedleImageData(&N60_95_249, 30, 95, 249, 60);
    appSetNeedleImageData(&N62_102_238, 31, 102, 238, 62);
    appSetNeedleImageData(&N64_105_227, 32, 105, 227, 64);
    appSetNeedleImageData(&N66_109_220, 33, 109, 220, 66);
    appSetNeedleImageData(&N68_114_211, 34, 114, 211, 68);
    appSetNeedleImageData(&N70_120_201, 35, 120, 201, 70);
    appSetNeedleImageData(&N72_127_190, 36, 127, 190, 72);
    appSetNeedleImageData(&N74_132_186, 37, 132, 186, 74);
    appSetNeedleImageData(&N76_138_178, 38, 138, 178, 76);
    appSetNeedleImageData(&N78_144_170, 39, 144, 170, 78);
    appSetNeedleImageData(&N80_154_160, 40, 154, 160, 80); 
    appSetNeedleImageData(&N82_164_149, 41, 164, 149, 82); 
    appSetNeedleImageData(&N84_169_142, 42, 169, 142, 84);
    appSetNeedleImageData(&N86_180_138, 43, 180, 138, 86);
    appSetNeedleImageData(&N88_180_123, 44, 180, 123, 88);
    appSetNeedleImageData(&N90_197_124, 45, 197, 124, 90); 
    appSetNeedleImageData(&N92_203_118, 46, 203, 118, 92); 
    appSetNeedleImageData(&N94_215_112, 47, 215, 112, 94);
    appSetNeedleImageData(&N96_224_102, 48, 224, 102, 96); 
    appSetNeedleImageData(&N98_231_103, 49, 231, 103, 98);
    appSetNeedleImageData(&N100_247_95, 50, 247, 95, 100);
    appSetNeedleImageData(&N102_256_95, 51, 256, 95, 102);
    appSetNeedleImageData(&N104_266_92, 52, 266, 92, 104);
    appSetNeedleImageData(&N106_275_89, 53, 275, 89, 106);
    appSetNeedleImageData(&N108_276_79, 54, 276, 79, 108);
    appSetNeedleImageData(&N110_298_79, 55, 298, 79, 110);
    appSetNeedleImageData(&N112_312_75, 56, 312, 75, 112);
    appSetNeedleImageData(&N114_321_74, 57, 321, 74, 114);
    appSetNeedleImageData(&N116_329_76, 58, 329, 76, 116);
    appSetNeedleImageData(&N118_338_74, 59, 338, 74, 118);
    appSetNeedleImageData(&N120_346_71, 60, 346, 71, 120);
    appSetNeedleImageData(&N122_351_73, 61, 351, 73, 122);
    appSetNeedleImageData(&N124_356_72, 62, 356, 72, 124);
    appSetNeedleImageData(&N126_359_74, 63, 359, 74, 126);
    appSetNeedleImageData(&N128_354_73, 64, 354, 73, 128);
    appSetNeedleImageData(&N130_369_76, 65, 369, 76, 130);
    appSetNeedleImageData(&N132_369_76, 66, 369, 76, 132);
    appSetNeedleImageData(&N134_371_76, 67, 371, 76, 134);
    appSetNeedleImageData(&N136_372_78, 68, 372, 78, 136); 
    appSetNeedleImageData(&N138_378_85, 69, 378, 85, 138);
    appSetNeedleImageData(&N140_388_89, 70, 388, 89, 140);
    appSetNeedleImageData(&N142_393_96, 71, 393, 96, 142);
    appSetNeedleImageData(&N144_392_96, 72, 392, 96, 144);
    appSetNeedleImageData(&N146_400_103, 73, 400, 103, 146);
    appSetNeedleImageData(&N148_404_110, 74, 404, 111, 148); 
    appSetNeedleImageData(&N150_411_119, 75, 411, 119, 150);    
    appSetNeedleImageData(&N152_412_121, 76, 412, 121, 152);
    appSetNeedleImageData(&N154_414_126, 77, 414, 126, 154);    
    appSetNeedleImageData(&N156_417_138, 78, 417, 138, 156);                        
    appSetNeedleImageData(&N158_423_146, 79, 423, 146, 158);
    appSetNeedleImageData(&N160_425_154, 80, 425, 154, 160);
    appSetNeedleImageData(&N162_430_162, 81, 430, 162, 162);
    appSetNeedleImageData(&N164_434_173, 82, 434, 173, 164);
    appSetNeedleImageData(&N166_436_179, 83, 436, 179, 166);
    appSetNeedleImageData(&N168_439_188, 84, 439, 188, 168);
    appSetNeedleImageData(&N170_443_197, 85, 443, 197, 170);
    appSetNeedleImageData(&N172_445_206, 86, 445, 206, 172);
    appSetNeedleImageData(&N174_449_215, 87, 449, 215, 174);
    appSetNeedleImageData(&N176_450_220, 88, 450, 220, 176);
    appSetNeedleImageData(&N178_455_235, 89, 455, 235, 178);
    appSetNeedleImageData(&N180_457_245, 90, 457, 245, 180);
    appSetNeedleImageData(&N182_457_254, 91, 457, 254, 182);
    appSetNeedleImageData(&N184_460_265, 92, 460, 265, 184);
    appSetNeedleImageData(&N186_462_275, 93, 462, 275, 186);
    appSetNeedleImageData(&N188_464_284, 94, 464, 284, 188);
    appSetNeedleImageData(&N190_465_296, 95, 465, 296, 190);
    appSetNeedleImageData(&N192_465_304, 96, 465, 304, 192);
    appSetNeedleImageData(&N194_468_318, 97, 468, 318, 194);
    appSetNeedleImageData(&N196_468_326, 98, 468, 326, 196);
    appSetNeedleImageData(&N198_468_337, 99, 468, 337, 198);
    appSetNeedleImageData(&N200_469_350, 100, 469, 350, 200);
    appSetNeedleImageData(&N202_469_352, 101, 469, 352, 202);
    appSetNeedleImageData(&N204_468_358, 102, 468, 358, 204);
    appSetNeedleImageData(&N206_466_363, 103, 466, 363, 206);
    appSetNeedleImageData(&N208_465_367, 104, 465, 367, 208);
    appSetNeedleImageData(&N210_464_373, 105, 464, 373, 210);
    appSetNeedleImageData(&N212_463_377, 106, 463, 377, 212);
    appSetNeedleImageData(&N214_462_380, 107, 462, 380, 214);
    appSetNeedleImageData(&N216_457_386, 108, 457, 386, 216);
    appSetNeedleImageData(&N218_457_388, 109, 457, 388, 218);
    appSetNeedleImageData(&N220_453_389, 110, 453, 389, 220);
    appSetNeedleImageData(&N222_454_394, 111, 454, 394, 222);
    appSetNeedleImageData(&N224_449_401, 112, 449, 401, 224);
    appSetNeedleImageData(&N226_447_406, 113, 447, 406, 226);
    appSetNeedleImageData(&N228_446_408, 114, 446, 408, 228);
    appSetNeedleImageData(&N230_443_411, 115, 443, 411, 230);
    appSetNeedleImageData(&N232_441_415, 116, 441, 415, 232);
    appSetNeedleImageData(&N234_438_417, 117, 438, 417, 234);
    appSetNeedleImageData(&N236_438_417, 118, 438, 417, 236);
    appSetNeedleImageData(&N238_432_424, 119, 432, 424, 238);
    appSetNeedleImageData(&N240_428_428, 120, 428, 428, 240);
}

void APP_InitDrive( void )
{
    APP_initialize_needle_lookup_table();
    
    leFixedString_Constructor(&p_diststring,  p_legatoDistBuff, MAX_DIST_STRING_LEN *2);
    p_diststring.fn->setFont(&p_diststring, (leFont*)& NotoSansB_18);
  
    leFixedString_Constructor(&p_distTravlstring,  p_legatoDistTravlBuff, MAX_DIST_TRVL_DIGITS *2);
    p_distTravlstring.fn->setFont(&p_distTravlstring, (leFont*)& NotoSansB_18);
    
    app_inputListener.handleTouchDown = &app_touchDownHandler;
    app_inputListener.handleTouchUp = &app_touchUpHandler;
    app_inputListener.handleTouchMove = &app_touchMoveHandler;
    SYS_INP_AddListener(&app_inputListener); 
 
}
/*******************************************************************************
 End of File
 */
