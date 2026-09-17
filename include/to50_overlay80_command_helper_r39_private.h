#ifndef TO50_OV80_R39_PRIVATE_H
#define TO50_OV80_R39_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *Frontier_GetLaunchArgs(void *);
extern void *NewMsgDataFromNarc(u32, u32, u32, u32);
extern void ov80_0222E268(void *, void *, u16, u32, void *);
extern void DestroyMsgData(void *);
extern void *Save_PlayerData_GetOptionsAddr(void *);
extern u32 Options_GetTextFrameDelay(void *);
extern void ov80_0222E3B8(void *, u32, u16, u16, u16, s16, u32);
extern void FrontierScriptContext_Pause(void *, BOOL (*)(void *));
extern BOOL ov80_0222F4D0(void *);
void ov80_0222F458(void *, const u16 *, u32);
#endif
