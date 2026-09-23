#include "overlay108_safari_decor_r2_private.h"

extern void ov108_021E9F94(void *, int);
extern void ov108_021E9F20(void *, int);
extern void ov108_021EA260(void *);
extern void ov108_021EA334(void *, int);
extern void ClearWindowTilemapAndScheduleTransfer(void *);

void ov108_021E940C(void *p, int enable) {
    int draw;
    if (enable == 1) {
        ov108_021E9F94(p, 1);
        ov108_021E9F20(p, 2);
        ov108_021EA260(p);
        ov108_021EA334(p, 0);
        draw = 0;
    } else {
        ov108_021E9F94(p, 0);
        ClearWindowTilemapAndScheduleTransfer((u8 *)p + 0x494);
        ov108_021E9F20(p, 0);
        ov108_021EA040(p);
        ov108_021EA334(p, 2);
        draw = 1;
    }
    Sprite_SetDrawFlag(PTR(p, 0x44c), draw);
    Sprite_SetDrawFlag(PTR(p, 0x450), draw);
    Sprite_SetDrawFlag(PTR(p, 0x444), draw);
}
