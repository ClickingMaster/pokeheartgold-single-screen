#ifndef TO57_R3_02_PRIVATE_H
#define TO57_R3_02_PRIVATE_H

#include "global.h"

extern u32 ov13_0224D258[256];

void *ov13_022208E8(s32 size);
void ov13_022208F8(void *ptr);
u8 ov13_02222708(const void *data, s32 size);
u32 ov13_02222730(u32 crc, const u8 *data, s32 size, s32 init, u32 *table);
void ov13_02222768(s32 unused, u32 *table);
s32 ov13_022227A0(void *dst, s32 size, const s8 *key, s32 keySize);
void ov13_0222281C(s32 which, s8 *dst, s32 size, const s8 *key, s32 keySize);
void ov13_02222850(const s8 *src, u8 *dst, s32 size);
void ov13_02222874(u8 *dst, s32 size, u8 *tmp);
void ov13_02222968(void *dst, const void *src, u32 size);

#endif
