#ifndef TO52_W09_OV75_PRIVATE_H
#define TO52_W09_OV75_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define U32_AT(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR_AT(p,o) (*(void **)((u8 *)(p)+(o)))
extern const u32 _02249B20[];
extern const u32 ov75_02249B24[];
int ov75_02249534(u32);
void ov75_022494CC(void *, void *, u32, u32, u32);
void *ov75_02247890(void *, u32, u32);
int YesNoPrompt_HandleInput(void *);
void YesNoPrompt_Destroy(void *);
void sub_02031780(void *, u32, u32);
void sub_02031734(void *);
void ov75_02247854(void *, u32, u32);
void FillWindowPixelBuffer(void *, u32);
void *NewString_ReadMsgData(void *, u32);
void AddTextPrinterParameterized(void *, u32, void *, u32, u32, u32, u32);
void String_Delete(void *);
void ListMenuUpdateCursorObj(void *, void *, u32, u32);
void CopyWindowToVram(void *);
int ov75_02247A98(void *);
int ov75_02247B98(void *);
void ov75_02247C70(void *);
#endif
