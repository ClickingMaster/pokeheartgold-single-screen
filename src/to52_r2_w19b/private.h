#ifndef TO52_SOL_R2_W19B_PRIVATE_H
#define TO52_SOL_R2_W19B_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define WORD(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void VsRecorder_FreeVideoInfo(void *, void *);
void VsRecorder_FreeProfile(void *, void *);
void GfGfx_EngineATogglePlanes(int, int);
int VsRecorder_StepBlend(void *, void *, int, int);
void VsRecorder_SetVBlankPaused(void *, int);
void VsRecorder_CreateProfile(void *, void *);
void VsRecorder_SetProfileDrawFlag(void *, int);
void VsRecorder_LoadScreen(void *, int, int);
void VsRecorder_CreateVideoInfo(void *, void *);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *, int);
int ov40_02230410(void *);
void GfGfx_EngineBTogglePlanes(int, int);
void ov40_02232FEC(void *, int);
void VsRecorder_SetSubState(void *, int);
void InitWindow(void *);
void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
void FillWindowPixelBuffer(void *, int);
void *NewString_ReadMsgData(void *, int);
int VsRecorder_GetCenteredTextX(void *, void *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void String_Delete(void *);
void ScheduleWindowCopyToVram(void *);
int ov40_022325B0(void *);
void ov40_022326DC(void *);
#endif
