#ifndef TO52_SOL_R2_W19B_PRIVATE_H
#define TO52_SOL_R2_W19B_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define WORD(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void ov40_0223064C(void *, void *);
void ov40_0222E7B8(void *, void *);
void GfGfx_EngineATogglePlanes(int, int);
int ov40_0222DA00(void *, void *, int, int);
void ov40_02230964(void *, int);
void ov40_0222E79C(void *, void *);
void ov40_0222E7DC(void *, int);
void ov40_022307DC(void *, int, int);
void ov40_02230638(void *, void *);
void ov40_022306A0(void *, int);
int ov40_02230410(void *);
void GfGfx_EngineBTogglePlanes(int, int);
void ov40_02232FEC(void *, int);
void ov40_0222BF80(void *, int);
void InitWindow(void *);
void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
void FillWindowPixelBuffer(void *, int);
void *NewString_ReadMsgData(void *, int);
int ov40_022306C0(void *, void *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void String_Delete(void *);
void ScheduleWindowCopyToVram(void *);
int ov40_022325B0(void *);
void ov40_022326DC(void *);
#endif
