#include "overlay_80_command_helpers_internal.h"

extern void thunk_Sprite_SetOamMode(void *, int);
int FrtCmd_048(void *c);

int FrtCmd_048(void *c) {
    void *p = sub_0209680C(APP(c));
    u16 index = FrontierScript_ReadVar(c);
    u16 mode = FrontierScript_ReadVar(c);
    void *sprite = FrontierMap_GetSprite(p, index);
    if (!sprite) {
        GF_AssertFail();
    }
    if (mode == 0) {
        thunk_Sprite_SetOamMode(PTR(sprite, 0), 0);
    } else {
        thunk_Sprite_SetOamMode(PTR(sprite, 0), 1);
    }
    return 0;
}
