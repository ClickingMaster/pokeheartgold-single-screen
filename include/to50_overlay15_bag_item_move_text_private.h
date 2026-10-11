#ifndef TO50_OV15_R25_PRIVATE_H
#define TO50_OV15_R25_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *String_New(u32, u32);
extern void GetItemDescIntoString(void *, u16, u32);
extern void *NewString_ReadMsgData(void *, u32);
extern void AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, u32);
extern void String_Delete(void *);
extern u16 TMHMGetMove(u16);
extern u32 GetMoveMaxPP(u16, u32);
extern u32 GetMoveAttr(u16, u32);
extern void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void ScheduleWindowCopyToVram(void *);
void BagApp_PrintItemDescription(void *, u16);
void BagApp_PrintTMHMMoveInfo(void *, u16);
#endif
