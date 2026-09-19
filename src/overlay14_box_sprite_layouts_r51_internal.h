#ifndef TO52_R3_W10_PRIVATE_H
#define TO52_R3_W10_PRIVATE_H
#include "global.h"
#define U8_AT(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define PTR_AT(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct Ov14Pos { s16 x, y; } Ov14Pos;
typedef struct Ov14Work { u8 pad000[0x2fc]; void *sprites[6]; } Ov14Work;
extern const Ov14Pos ov14_021F80BC[6];
int Party_GetCount(void *party);
void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
void ov14_021F3190(void *work, u32 idx, u32 value);
void ov14_021F2A18(void *work, u32 slot, u32 value);
void ov14_021F2FDC(void *);
void ov14_021F3044(void *);
void ov14_021F30B0(void *);
void ov14_021F311C(void *);
#endif
