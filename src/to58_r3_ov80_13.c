#include "overlay_80_command_helpers_internal.h"


int FrtCmd_046(void *c) {
    void *p = sub_0209680C(APP(c));
    u16 index = FrontierScript_ReadVar(c);
    u8 *script = PTR(c, 0x1c);
    PTR(c, 0x1c) = script + 1;
    u8 flag = *script;
    void *sprite = FrontierMap_GetSprite(p, index);
    if (!sprite) {
        GF_AssertFail();
    }
    ManagedSprite_SetDrawFlag(sprite, flag);
    return 0;
}
