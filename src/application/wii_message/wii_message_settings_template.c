#include "global.h"

#include "application/wii_message/wii_message_settings_internal.h"

const OverlayManagerTemplate gApp_MainMenu_SelectOption_WiiMessageSettings = {
    .init = WiiMessageSettings_Init,
    .exec = WiiMessageSettings_Main,
    .exit = WiiMessageSettings_Exit,
    .ovy_id = FS_OVERLAY_ID_NONE,
};
