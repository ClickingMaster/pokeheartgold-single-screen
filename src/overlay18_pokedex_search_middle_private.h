#ifndef OV18_MID_PRIVATE_H
#define OV18_MID_PRIVATE_H
#include "global.h"
#define U8_AT(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define S8_AT(p,o) (*(s8 *)((u8 *)(p)+(o)))
#define U16_AT(p,o) (*(u16 *)((u8 *)(p)+(o)))
typedef struct Ov18MidWork { u8 pad[0x18a2]; u16 species; u8 values[0x20]; s8 count; s8 selected; u8 pad2; u8 unk_flags:5; u8 flag:1; u8 unk_flags2:2; } Ov18MidWork;
void ov18_021F3CA8(void *, int, u8 *, u8 *);
void ov18_021F11C0(void *, int, int);
void ov18_021F1A7C(void *, u16, u8, u8, int, int, int);
void ov18_021F1294(void *, int, int, int, int);
void ov18_021F14FC(void *, int, int, int);
void ov18_021F12C8(void *, int, s16 *, s16 *, int);
void ov18_021F5EFC(Ov18MidWork *, int, int);
void ov18_021F5FFC(Ov18MidWork *, int, int);
void ov18_021F6038(Ov18MidWork *);
void ov18_021F609C(Ov18MidWork *, int);
int GetMonPicHeightBySpeciesGenderForm(int, int, int, int, int);
typedef struct ManagedSprite ManagedSprite;
void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *, s16, s16, fx32);
#endif
