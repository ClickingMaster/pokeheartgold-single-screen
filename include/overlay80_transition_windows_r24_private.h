#ifndef TO50_OV80_R24_PRIVATE_H
#define TO50_OV80_R24_PRIVATE_H
#include "global.h"
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct Window Window;
void ov80_0223AC24(u32, u32, s32, void *, u32);
void ResetVisibleHardwareWindows(u32);
void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
BOOL IsPaletteFadeFinished(void);
Window *AllocWindows(u32, u32);
void AddWindowParameterized(void *, Window *, u32, u32, u32, u32, u32, u32, u32);
void PaletteData_FillPaletteInBuffer(void *, u32, u32, u32, u32, u32);
void FillWindowPixelBuffer(Window *, u32);
void ScheduleWindowCopyToVram(Window *);
void *ov80_0223AF30(u32);
void ov80_0223AF80(void *, u32, u32, Window *, u32);
BOOL ov80_0223AFC4(void *);
BOOL ov80_0223B1D4(void *);
void ov80_0223AF60(void *);
void ClearWindowTilemapAndCopyToVram(Window *);
void RemoveWindow(Window *);
void WindowArray_Delete(Window *, u32);
void SetMasterBrightnessByColor(u32, u32);
void BG_ClearCharDataRange(u32, u32, u32, u32);
void BgClearTilemapBufferAndCommit(void *, u32);
BOOL ov80_0222D390(void *);
BOOL ov80_0222D404(void *);
BOOL ov80_0222D520(void *);
#endif
