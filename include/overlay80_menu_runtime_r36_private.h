#ifndef TO50_OV80_R36_PRIVATE_H
#define TO50_OV80_R36_PRIVATE_H
#include "global.h"
#define U8_AT(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U16_AT(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define PTR_AT(p, n) (*(void **)((u8 *)(p) + (n)))
typedef struct Ov80Flags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
} Ov80Flags;
typedef struct String String;
typedef struct SysTask SysTask;
typedef struct Window Window;
typedef struct ListMenu ListMenu;
typedef struct StringTemplate StringTemplate;
extern u8 gSystem[];
BOOL IsPaletteFadeFinished(void);
s32 ListMenu_ProcessInput(ListMenu *);
void ListMenuGetCurrentItemArrayId(ListMenu *, u16 *);
s32 ListMenuGetValueByArrayId(ListMenu *, u16);
BOOL System_GetTouchNew(void);
void PlaySE(u32);
void DestroyListMenu(ListMenu *, void *, void *);
void EraseFrameAndWindow1(void *, u32);
void RemoveWindow(Window *);
void String_Delete(String *);
void DestroyMsgData(void *);
void SysTask_Destroy(SysTask *);
void Heap_Free(void *);
String *String_New(u32, u32);
void FillWindowPixelBuffer(Window *, u8);
void ReadMsgDataIntoString(void *, u32, String *);
void StringExpandPlaceholders(StringTemplate *, String *, String *);
u8 AddTextPrinterParameterized(Window *, u32, String *, u32, u32, u32, void *);
void ov80_0222EC90(SysTask *, void *);
void ov80_0222EDA0(void *, int);
void ov80_0222EE14(void *, u16, u32);
void ov80_0222EE7C(void *);
#endif
