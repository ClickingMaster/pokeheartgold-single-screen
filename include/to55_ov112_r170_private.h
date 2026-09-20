#include "global.h"
#pragma require_prototypes off

extern void FillWindowPixelBuffer(void *, int);
extern void *ReadMsgData_ExpandPlaceholders(void *, void *, int, int);
extern u32 String_GetLength(void *);
extern void *String_New(u32, int);
extern void String_Copy(void *, const void *);
extern void String_AddChar(void *, u16);
extern void String_Cat(void *, const void *);
extern void String_Delete(void *);
extern void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
extern void CopyWindowToVram(void *);
extern void CopyU16ArrayToString(void *, const u16 *);
extern void BufferString(void *, int, void *, int, int, int);
extern void *NewString_ReadMsgData(void *, int);
extern void CopyStringToU16Array(void *, u16 *, int);
extern void BufferIntegerAsString(void *, int, int, int, int, int);
extern int GetWindowWidth(void *);
extern int FontID_String_GetCenterAlignmentX(int, void *, int, int);
extern void ov112_021F13BC(void *, int, int, int);
extern const u16 ov112_021FF2EC[];

#define PTR_AT(p, off) (*(void **)((u8 *)(p) + (off)))
#define U32_AT(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define U16_AT(p, off) (*(u16 *)((u8 *)(p) + (off)))
#define U8_AT(p, off)  (*(u8 *)((u8 *)(p) + (off)))

typedef struct Ov112RecordWork {
    u8 pad00[0xc0];
    void *records[2];
    u8 padC8[0x5c];
    u32 value9;
    u32 value10;
    u8 pad12C[4];
    u32 hour;
    u8 pad134[9];
    u8 recordIndex;
} Ov112RecordWork;

void ov112_021F13CC(void *, int, int);
void ov112_021F1488(void *, const u16 *, int, int);
void ov112_021F1504(void *, u32);
void ov112_021F1548(void *, int, int);
void ov112_021F1584(void *, const u16 *, int, int);
void ov112_021F15CC(void *, int, int, int, int);
void ov112_021F15E4(void *);
void ov112_021F1624(void *, int, int);
