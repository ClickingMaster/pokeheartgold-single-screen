#include "to58_overlay74_010_private.h"
extern int ov74_02231764(int);
extern const u32 ov74_0223C998[];
extern const u16 ov74_0223C9A8[];
extern const u16 ov74_0223C9D8[];
extern void Sprite_SetDrawFlag(void *, int);
extern u8 *ov74_0223D338;
extern void Sprite_SetPaletteOverride(void *, int);
void ov74_02231FB0(void);
int ov74_02232398(u8 *base, int slot);
int ov74_022323D0(u8 *base, int slot);
int ov74_02232424(u8 *base, int slot);
int ov74_02232474(u8 *base, int slot);
void ov74_0223262C(u8 *base);
void ov74_02232910(u8 *base, int fadeType, u32 next, int *state);
void ov74_02232940(u8 *base, int fadeType, u32 next, int *state);

void ov74_0223262C(u8 *base) {
    int i;
    u8 *p = base;
    for (i = 0; i < 6; i++, p += 12) {
        if (*(int *)(p + 0x3cc) != -1 && *(u32 *)(base + 0xE884) == *(u32 *)(p + 0x3d0)) {
            Sprite_SetDrawFlag(*(void **)(p + 0x3c8), 1);
        } else {
            Sprite_SetDrawFlag(*(void **)(p + 0x3c8), 0);
        }
    }
}
