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

int ov74_02232398(u8 *base, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    int box = *(int *)(base + 0xE884);
    return AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 0x41, NULL) == 0x19c;
}

int ov74_022323D0(u8 *base, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    int box = *(int *)(base + 0xE884);
    u8 *mon = boxes + 4 + box * 0x960 + slot * 0x50;
    int i;
    u32 j;
    for (i = 0; i < 4; i++) {
        u32 value = AGB_GetBoxMonData(mon, i + 13, NULL);
        for (j = 0; j < 8; j++) {
            if (value == ov74_0223C998[j]) {
                return 1;
            }
        }
    }
    return 0;
}

int ov74_02232424(u8 *base, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    int box = *(int *)(base + 0xE884);
    u32 value = AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 12, NULL);
    if (ov74_0223C9A8[24] != 0) {
        const u16 *list = ov74_0223C9D8;
        do {
            if (value == *list) {
                return 1;
            }
            list++;
        } while (*list != 0);
    }
    return 0;
}

int ov74_02232474(u8 *base, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    int box = *(int *)(base + 0xE884);
    return ov74_02231764(AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 11, NULL));
}
