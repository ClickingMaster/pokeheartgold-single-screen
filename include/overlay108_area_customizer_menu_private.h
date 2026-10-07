#ifndef POKEHEARTGOLD_OVERLAY108_AREA_CUSTOMIZER_MENU_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY108_AREA_CUSTOMIZER_MENU_PRIVATE_H
#include "global.h"
typedef struct Ov108Flags {
    u8 low : 3;
    u8 high : 5;
} Ov108Flags;
typedef struct Ov108Work2 {
    u32 unk00, unk04, unk08, unk0C, unk10;
    u8 pad14[0x4BC - 0x14];
    void *menu;
    u8 pad4C0[0x184DF - 0x4C0];
    u8 selectedA, selectedB, unk184E1;
    Ov108Flags flags;
} Ov108Work2;
s32 TouchscreenListMenu_HandleInput(void *);
void TouchscreenListMenu_Destroy(void *);
void ov108_021E61E8(Ov108Work2 *);
void ov108_021E6238(Ov108Work2 *);
void ov108_021E79A8(void *, u8, int, int);
void ov108_021E6804(Ov108Work2 *);
u32 ov108_021E5DB8(Ov108Work2 *);
u32 ov108_021E5E68(Ov108Work2 *);
void ov108_021E7F7C(Ov108Work2 *);
int ov108_021E80F4(Ov108Work2 *);
void ov108_021E7700(Ov108Work2 *, int, int, int);
void ov108_021E7EB0(Ov108Work2 *);
void ov108_021E7CD8(Ov108Work2 *, int);
#endif
