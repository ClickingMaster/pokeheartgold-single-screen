#ifndef TO47_OVERLAY83_BATTLE_SUMMARY_R19_PRIVATE_H
#define TO47_OVERLAY83_BATTLE_SUMMARY_R19_PRIVATE_H

#include "global.h"

void ov83_0223FAA8(void *work, int value);
void ov83_0223FB24(void *work, int value);
void ClearWindowTilemapAndScheduleTransfer(void *window);
u32 ov83_02247768(u32 unused, u32 value);
void *Party_GetMonByIndex(void *party, int index);
void ov83_0223FD4C(void *work, void *window, void *mon);
void ov83_0223FF44(void *work, void *window, void *mon);
void ov83_022477EC(int index, int value, u32 *word);
void ov83_02240F7C(u8 *work, u16 *a, u16 *b, u16 *c, u16 *d);
int ov80_02237D8C(int value);
void FillWindowPixelRect(void *window, u8 fill, u16 x, u16 y, u16 width, u16 height);
int sub_0205C1F0(int value);
int sub_0205C268(int value);
int FrontierSave_GetStat(void *save, int stat, int substat);
void ov83_02240C48(u8 *work, int field, int value, int digits, int mode);
int ov83_0223FCB4(void *work, void *window, int msg, int x, int y, int speed, u8 a, u8 b, u8 c, u8 d);
int sub_0203769C(void);
void ov83_02241DD8(u8 *work, void *window, void *msgData, int msgNo, int x, int y, int color, int a, int b);
void ScheduleWindowCopyToVram(void *window);
void ov83_02241770(u8 *work, void *window);
void ov83_02241A60(u8 *work, u32 value);
void ov83_02241ABC(u8 *work, u32 value);
void PlaySE(int seq);
u8 ov83_0224776C(u32 subtrahend, u32 value);
int ov83_02240EF8(u16 value);
void ov83_02240C6C(u8 *work, int field);
void ov80_02237FA4(void *save, int mode, int value);
void ov83_022477C4(void *p, int value);
void ov83_02241730(u8 *work);
void ov83_02241B18(u8 *work);
void ov83_0224042C(u8 *work);
void ov83_02241354(void *window);
int Options_GetFrame(void *options);
void ov83_02247944(void *window, int frame);
void ov83_022415F4(u8 *work, int count, int type);
void ov83_022416A0(u8 *work, int count, int value);
void ov83_022418E8(u8 *work, u32 value, u8 type);
extern const u16 ov83_02247D18[];

#endif
