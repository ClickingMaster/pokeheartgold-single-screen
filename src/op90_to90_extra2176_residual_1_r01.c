#pragma force_active on
#include "global.h"
typedef struct { u8 v[4]; } Tbl4;
extern const Tbl4 _0221EA54;
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
typedef struct {
    u8 pad[0x198];
    void *sprites[12];
    u8 gap[0xb64 - 0x1c8];
    u8 x[4];
    u8 y[4];
} View;
void ov05_0221D3AC(View *view, u8 index, u16 x, s32 y);
// @fn ov05_0221D3AC
void ov05_0221D3AC(View *view, u8 index, u16 x, s32 y) {
    Tbl4 tbl = _0221EA54;
    u32 i;

    for (i = 0; i < 3; i++) {
        ManagedSprite_SetPositionXY(view->sprites[tbl.v[index] + i], view->x[index] + i * 19 + x, view->y[index] + y);
    }
}
