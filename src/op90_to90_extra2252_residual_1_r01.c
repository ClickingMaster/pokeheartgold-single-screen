#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Dex { u8 pad[0x670]; ManagedSprite *sprites[1]; u8 pad2[0x1859 - 0x674]; u8 unk1859; u8 pad3[4]; u8 unk185E; } Dex;
void ov18_021F1598(Dex *, int, int);
void ov18_021F16C4(Dex *d, int base, int dy, u8 row);
// @fn ov18_021F16C4
void ov18_021F16C4(Dex *d, int base, int dy, u8 row) {
s16 x, y; u16 i, cnt; int idx; cnt = 0; for (i = 0; i < 30; i++) { idx = base + i + d->unk185E * 30; ManagedSprite_GetPositionXY(d->sprites[idx], &x, &y); if (y == 0xE0) { y = -16; ov18_021F1598(d, cnt + (d->unk1859 * 15 - (row + 1) * 5), idx); cnt++; } y += dy; ManagedSprite_SetPositionXY(d->sprites[idx], x, y); }
}
