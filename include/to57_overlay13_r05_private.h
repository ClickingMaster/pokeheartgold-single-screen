#ifndef TO57_R3_10_PRIVATE_H
#define TO57_R3_10_PRIVATE_H
#include "global.h"
typedef struct Ov13GlobalR05 {
    u8 pad00[0x34];
    int unk34;
    int unk38;
} Ov13GlobalR05;
extern Ov13GlobalR05 _0224CF98;
void ov13_02222968(void *dst, const void *src, int size);
void ov13_02222978(void *dst, int value, int size);
int ov13_02222A84(int value);
int ov13_02222A5C(int value);
int ov13_02221D34(const u8 *src, int length);
int ov13_02221C78(const u8 *src, u8 *dst);
int ov13_02221D58(const u8 *src, u8 *dst);
int ov13_02221EAC(const u8 *src, u8 *dst);
#endif
