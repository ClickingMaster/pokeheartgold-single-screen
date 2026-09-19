#ifndef POKEHEARTGOLD_OVERLAY83_PARTY_MENU_OPEN_R10_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY83_PARTY_MENU_OPEN_R10_PRIVATE_H
#include "global.h"
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct Ov83R10Bits {
    u8 pad[0xE];
    u8 low : 3;
    u8 active : 1;
    u8 high : 4;
} Ov83R10Bits;
void ov83_02241354(void *);
void ClearWindowTilemapAndScheduleTransfer(void *);
void ov83_0223FB6C(void *);
void ov83_022477EC(int, int, void *);
void ov83_02240B54(void *, void *);
void ov83_02247A18(void *);
void ListMenuItems_Delete(void *);
void ov83_0224755C(void *, int);
void ov83_0224759C(void *, int, int);
void ov83_022403C0(void *, void *);
#endif
