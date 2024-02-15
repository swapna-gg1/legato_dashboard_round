#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// screen member widget declarations
static leWidget* root0;
static leWidget* root1;
static leWidget* root2;
static leWidget* root3;

leWidget* Screen0_BackgroundPanel;
leImageWidget* Screen0_speedoImage;
leLabelWidget* Screen0_DistLabel;
leLabelWidget* Screen0_InstructLabel;
leImageWidget* Screen0_indicatorImage;
leImageWidget* Screen0_MCHPLogoImage;
leLabelWidget* Screen0_SpeedUnitLabel;
leLabelWidget* Screen0_SpeedLabel;
leLabelWidget* Screen0_RTimeLabel;
leLabelWidget* Screen0_RTLabel;
leImageWidget* Screen0_BlueBlurImage;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Screen0(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Screen0(void)
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // layer 0
    root0 = leWidget_New();
    root0->fn->setSize(root0, 720, 720);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);
    root0->flags |= LE_WIDGET_IGNOREEVENTS;
    root0->flags |= LE_WIDGET_IGNOREPICK;

    Screen0_BackgroundPanel = leWidget_New();
    Screen0_BackgroundPanel->fn->setPosition(Screen0_BackgroundPanel, 0, 0);
    Screen0_BackgroundPanel->fn->setSize(Screen0_BackgroundPanel, 720, 720);
    Screen0_BackgroundPanel->fn->setScheme(Screen0_BackgroundPanel, &IndigoScheme);
    root0->fn->addChild(root0, (leWidget*)Screen0_BackgroundPanel);

    Screen0_speedoImage = leImageWidget_New();
    Screen0_speedoImage->fn->setPosition(Screen0_speedoImage, 0, 0);
    Screen0_speedoImage->fn->setSize(Screen0_speedoImage, 720, 720);
    Screen0_speedoImage->fn->setBackgroundType(Screen0_speedoImage, LE_WIDGET_BACKGROUND_NONE);
    Screen0_speedoImage->fn->setBorderType(Screen0_speedoImage, LE_WIDGET_BORDER_NONE);
    Screen0_speedoImage->fn->setImage(Screen0_speedoImage, (leImage*)&Speedo);
    root0->fn->addChild(root0, (leWidget*)Screen0_speedoImage);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGBA_8888);

    // layer 1
    root1 = leWidget_New();
    root1->fn->setSize(root1, 720, 720);
    root1->fn->setBackgroundType(root1, LE_WIDGET_BACKGROUND_NONE);
    root1->fn->setMargins(root1, 0, 0, 0, 0);
    root1->flags |= LE_WIDGET_IGNOREEVENTS;
    root1->flags |= LE_WIDGET_IGNOREPICK;

    leAddRootWidget(root1, 1);
    leSetLayerColorMode(1, LE_COLOR_MODE_RGBA_8888);

    // layer 2
    root2 = leWidget_New();
    root2->fn->setSize(root2, 340, 363);
    root2->fn->setBackgroundType(root2, LE_WIDGET_BACKGROUND_NONE);
    root2->fn->setMargins(root2, 0, 0, 0, 0);
    root2->flags |= LE_WIDGET_IGNOREEVENTS;
    root2->flags |= LE_WIDGET_IGNOREPICK;

    Screen0_DistLabel = leLabelWidget_New();
    Screen0_DistLabel->fn->setPosition(Screen0_DistLabel, 115, 255);
    Screen0_DistLabel->fn->setSize(Screen0_DistLabel, 105, 25);
    Screen0_DistLabel->fn->setScheme(Screen0_DistLabel, &IndigoScheme);
    Screen0_DistLabel->fn->setBackgroundType(Screen0_DistLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_DistLabel->fn->setHAlignment(Screen0_DistLabel, LE_HALIGN_CENTER);
    root2->fn->addChild(root2, (leWidget*)Screen0_DistLabel);

    Screen0_InstructLabel = leLabelWidget_New();
    Screen0_InstructLabel->fn->setPosition(Screen0_InstructLabel, 75, 280);
    Screen0_InstructLabel->fn->setSize(Screen0_InstructLabel, 190, 25);
    Screen0_InstructLabel->fn->setScheme(Screen0_InstructLabel, &IndigoScheme);
    Screen0_InstructLabel->fn->setBackgroundType(Screen0_InstructLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_InstructLabel->fn->setHAlignment(Screen0_InstructLabel, LE_HALIGN_CENTER);
    Screen0_InstructLabel->fn->setVAlignment(Screen0_InstructLabel, LE_VALIGN_TOP);
    Screen0_InstructLabel->fn->setString(Screen0_InstructLabel, (leString*)&string_Instruction);
    root2->fn->addChild(root2, (leWidget*)Screen0_InstructLabel);

    Screen0_indicatorImage = leImageWidget_New();
    Screen0_indicatorImage->fn->setPosition(Screen0_indicatorImage, 47, 255);
    Screen0_indicatorImage->fn->setSize(Screen0_indicatorImage, 50, 50);
    Screen0_indicatorImage->fn->setVisible(Screen0_indicatorImage, LE_FALSE);
    Screen0_indicatorImage->fn->setBackgroundType(Screen0_indicatorImage, LE_WIDGET_BACKGROUND_NONE);
    Screen0_indicatorImage->fn->setBorderType(Screen0_indicatorImage, LE_WIDGET_BORDER_NONE);
    Screen0_indicatorImage->fn->setImage(Screen0_indicatorImage, (leImage*)&StA);
    root2->fn->addChild(root2, (leWidget*)Screen0_indicatorImage);

    Screen0_MCHPLogoImage = leImageWidget_New();
    Screen0_MCHPLogoImage->fn->setPosition(Screen0_MCHPLogoImage, 72, 31);
    Screen0_MCHPLogoImage->fn->setSize(Screen0_MCHPLogoImage, 190, 100);
    Screen0_MCHPLogoImage->fn->setBackgroundType(Screen0_MCHPLogoImage, LE_WIDGET_BACKGROUND_NONE);
    Screen0_MCHPLogoImage->fn->setBorderType(Screen0_MCHPLogoImage, LE_WIDGET_BORDER_NONE);
    Screen0_MCHPLogoImage->fn->setImage(Screen0_MCHPLogoImage, (leImage*)&MicrochipLogo);
    root2->fn->addChild(root2, (leWidget*)Screen0_MCHPLogoImage);

    Screen0_SpeedUnitLabel = leLabelWidget_New();
    Screen0_SpeedUnitLabel->fn->setPosition(Screen0_SpeedUnitLabel, 115, 110);
    Screen0_SpeedUnitLabel->fn->setVisible(Screen0_SpeedUnitLabel, LE_FALSE);
    Screen0_SpeedUnitLabel->fn->setScheme(Screen0_SpeedUnitLabel, &IndigoScheme);
    Screen0_SpeedUnitLabel->fn->setBackgroundType(Screen0_SpeedUnitLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_SpeedUnitLabel->fn->setHAlignment(Screen0_SpeedUnitLabel, LE_HALIGN_CENTER);
    Screen0_SpeedUnitLabel->fn->setString(Screen0_SpeedUnitLabel, (leString*)&string_SpeedUnit);
    root2->fn->addChild(root2, (leWidget*)Screen0_SpeedUnitLabel);

    Screen0_SpeedLabel = leLabelWidget_New();
    Screen0_SpeedLabel->fn->setPosition(Screen0_SpeedLabel, 115, 50);
    Screen0_SpeedLabel->fn->setSize(Screen0_SpeedLabel, 99, 52);
    Screen0_SpeedLabel->fn->setVisible(Screen0_SpeedLabel, LE_FALSE);
    Screen0_SpeedLabel->fn->setScheme(Screen0_SpeedLabel, &IndigoScheme);
    Screen0_SpeedLabel->fn->setBackgroundType(Screen0_SpeedLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_SpeedLabel->fn->setHAlignment(Screen0_SpeedLabel, LE_HALIGN_CENTER);
    Screen0_SpeedLabel->fn->setString(Screen0_SpeedLabel, (leString*)&string_Speed);
    root2->fn->addChild(root2, (leWidget*)Screen0_SpeedLabel);

    Screen0_RTimeLabel = leLabelWidget_New();
    Screen0_RTimeLabel->fn->setPosition(Screen0_RTimeLabel, 75, 310);
    Screen0_RTimeLabel->fn->setSize(Screen0_RTimeLabel, 190, 25);
    Screen0_RTimeLabel->fn->setVisible(Screen0_RTimeLabel, LE_FALSE);
    Screen0_RTimeLabel->fn->setScheme(Screen0_RTimeLabel, &IndigoScheme);
    Screen0_RTimeLabel->fn->setBackgroundType(Screen0_RTimeLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_RTimeLabel->fn->setHAlignment(Screen0_RTimeLabel, LE_HALIGN_CENTER);
    Screen0_RTimeLabel->fn->setString(Screen0_RTimeLabel, (leString*)&string_remainingTime);
    root2->fn->addChild(root2, (leWidget*)Screen0_RTimeLabel);

    Screen0_RTLabel = leLabelWidget_New();
    Screen0_RTLabel->fn->setPosition(Screen0_RTLabel, 98, 0);
    Screen0_RTLabel->fn->setSize(Screen0_RTLabel, 41, 25);
    Screen0_RTLabel->fn->setScheme(Screen0_RTLabel, &IndigoScheme);
    Screen0_RTLabel->fn->setBackgroundType(Screen0_RTLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_RTLabel->fn->setHAlignment(Screen0_RTLabel, LE_HALIGN_CENTER);
    Screen0_RTimeLabel->fn->addChild(Screen0_RTimeLabel, (leWidget*)Screen0_RTLabel);

    leAddRootWidget(root2, 2);
    leSetLayerColorMode(2, LE_COLOR_MODE_RGBA_8888);

    // layer 3
    root3 = leWidget_New();
    root3->fn->setSize(root3, 720, 720);
    root3->fn->setBackgroundType(root3, LE_WIDGET_BACKGROUND_NONE);
    root3->fn->setMargins(root3, 0, 0, 0, 0);
    root3->flags |= LE_WIDGET_IGNOREEVENTS;
    root3->flags |= LE_WIDGET_IGNOREPICK;

    Screen0_BlueBlurImage = leImageWidget_New();
    Screen0_BlueBlurImage->fn->setPosition(Screen0_BlueBlurImage, 0, 0);
    Screen0_BlueBlurImage->fn->setSize(Screen0_BlueBlurImage, 720, 720);
    Screen0_BlueBlurImage->fn->setBackgroundType(Screen0_BlueBlurImage, LE_WIDGET_BACKGROUND_NONE);
    Screen0_BlueBlurImage->fn->setBorderType(Screen0_BlueBlurImage, LE_WIDGET_BORDER_NONE);
    Screen0_BlueBlurImage->fn->setImage(Screen0_BlueBlurImage, (leImage*)&blueBlur);
    root3->fn->addChild(root3, (leWidget*)Screen0_BlueBlurImage);

    leAddRootWidget(root3, 3);
    leSetLayerColorMode(3, LE_COLOR_MODE_RGBA_8888);

    Screen0_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Screen0(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);
    root1->fn->setSize(root1, root1->rect.width, root1->rect.height);
    root2->fn->setSize(root2, root2->rect.width, root2->rect.height);
    root3->fn->setSize(root3, root3->rect.width, root3->rect.height);

    Screen0_OnUpdate(); // raise event
}

void screenHide_Screen0(void)
{

    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Screen0_BackgroundPanel = NULL;
    Screen0_speedoImage = NULL;

    leRemoveRootWidget(root1, 1);
    leWidget_Delete(root1);
    root1 = NULL;

    leRemoveRootWidget(root2, 2);
    leWidget_Delete(root2);
    root2 = NULL;

    Screen0_DistLabel = NULL;
    Screen0_InstructLabel = NULL;
    Screen0_indicatorImage = NULL;
    Screen0_MCHPLogoImage = NULL;
    Screen0_SpeedUnitLabel = NULL;
    Screen0_SpeedLabel = NULL;
    Screen0_RTimeLabel = NULL;
    Screen0_RTLabel = NULL;

    leRemoveRootWidget(root3, 3);
    leWidget_Delete(root3);
    root3 = NULL;

    Screen0_BlueBlurImage = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Screen0(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Screen0(uint32_t lyrIdx)
{
    if(lyrIdx >= LE_LAYER_COUNT)
        return NULL;

    switch(lyrIdx)
    {
        case 0:
        {
            return root0;
        }
        case 1:
        {
            return root1;
        }
        case 2:
        {
            return root2;
        }
        case 3:
        {
            return root3;
        }
        default:
        {
            return NULL;
        }
    }
}

