#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { u8 pad[0x2f0]; void *bg; u8 pad2[8]; ManagedSprite *sprites[1]; u8 pad3[0x40B2 - 0x300]; u8 idx[6]; } Inner;
typedef struct { u8 pad[0x34]; Inner *inner; } Work;
typedef struct { u8 x, y; } Pos;
extern Pos ov14_021F808C[];
void BgCellManager_GetCellPosition(void *, int, s8 *, s8 *);
void PCBox_UpdatePartyIconPositions(Work *w);
// @fn PCBox_UpdatePartyIconPositions
void PCBox_UpdatePartyIconPositions(Work *w) {
s8 a[2]; s16 x, y; u16 i; BgCellManager_GetCellPosition(w->inner->bg, 1, &a[1], &a[0]); y = a[0] * 8; x = a[1] * 8; for (i = 0; i < 6; i++) { ManagedSprite_SetPositionXY(w->inner->sprites[w->inner->idx[i]], x + ov14_021F808C[i].x, y + ov14_021F808C[i].y); }
}
