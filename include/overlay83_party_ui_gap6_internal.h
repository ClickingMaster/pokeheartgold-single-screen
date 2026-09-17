#ifndef OVERLAY83_PARTY_UI_GAP6_INTERNAL_H
#define OVERLAY83_PARTY_UI_GAP6_INTERNAL_H
#include "global.h"
int ov80_02237D8C(int);
void *Party_GetMonByIndex(void *, int);
u32 GetMonData(void *, int, void *);
void sub_0200CE7C(void *, int, u32, int, int, void *, int, int);
void ov83_02240D64(u8 *, void *, int, int, int, int);
void FillWindowPixelRect(void *, int, u16, int, int, int);
void PrintUIntOnWindow(void *, int, int, int, void *, int, int);
void ScheduleWindowCopyToVram(void *);
u32 ov83_0223FC48(u8 *, void *, int, int, int, int, int, int, int, int);
int Options_GetFrame(void *);
void ov83_02247944(void *, int);
void ov83_022401A4(u8 *, void *, int, int);
void ov83_02240238(u8 *, void *, int, int);
void ov83_02240290(u8 *);
#endif
