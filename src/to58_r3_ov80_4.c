#include "overlay_80_command_helpers_internal.h"
#include "screen_fade.h"

int FrtCmd_019(void *c);

int FrtCmd_019(void *c) {
    u16 a = FrontierScriptContext_ReadHalfWord(c);
    u16 b = FrontierScriptContext_ReadHalfWord(c);
    u16 d = FrontierScriptContext_ReadHalfWord(c);
    u16 e = FrontierScriptContext_ReadHalfWord(c);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)d, (enum FadeType)d, e, a, b, (enum HeapID)11);
    ResetVisibleHardwareWindows((PMLCDTarget)0);
    ResetVisibleHardwareWindows((PMLCDTarget)1);
    return 0;
}
