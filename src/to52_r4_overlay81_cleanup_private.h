#ifndef TO52_OVERLAY81_TEXT_CLEANUP_PRIVATE_H
#define TO52_OVERLAY81_TEXT_CLEANUP_PRIVATE_H
#include "global.h"
typedef struct Work {
    u8 pad00[0x12];
    u8 count;
    u8 pad13[9];
    void *msgData;
    void *format;
    void *string24;
    void *string28;
    void *strings[4];
    u8 pad3C[0x10];
    void *unk4C;
    u8 pad50[0x150];
    void *palette;
    void *vramMan;
    void *picManager;
    void *pics[3];
    u8 pad1B8[0x1A8];
    void *objects[1];
} Work;
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define TEXT_COLOR(a,b,c) (((u32)(a) << 24 >> 8) | ((u32)(b) << 24 >> 16) | (c))
void FillWindowPixelBuffer(void *,int);
void *Party_GetMonByIndex(void *,u32);
void *String_New(u32,u32);
void *NewString_ReadMsgData(void *,u32);
void *Mon_GetBoxMon(void *);
void BufferBoxMonSpeciesName(void *,u32,void *);
void StringExpandPlaceholders(void *,void *,void *);
void AddTextPrinterParameterizedWithColor(void *,u32,void *,u32,u32,u32,u32,void *);
void String_Delete(void *);
u32 GetMonData(void *,u32,void *);
u32 GetMonGender(void *);
u32 GetWindowWidth(void *);
void ScheduleWindowCopyToVram(void *);
void *NewMsgDataFromNarc(u32,u32,u32,u32);
void DestroyMsgData(void *);
void String_SetEmpty(void *);
void ReadMsgDataIntoString(void *,u32,void *);
void ov81_0224275C(void *);
void *ov81_02242EA4(void *);
void ov81_02242D74(void *);
void ov81_02241364(void *);
void ov81_02241FC0(void *);
void ov81_02242170(void *);
void WirelessStatusIcon_Destroy(void);
void PaletteData_FreeBuffers(void *,u32);
void PaletteData_Free(void *);
void ov81_02242B38(void *);
void Pokepic_Delete(void *);
void PokepicManager_Delete(void *);
void MessageFormat_Delete(void *);
void FontID_Release(u32);
void ov81_0224300C(void *);
void ov81_02240458(void *);
void NARC_Delete(void *);
void GF_3DVramMan_Delete(void *);
void ov81_022409B0(Work *,void *,u8,int,int,u8,u8,u8,u8,void *);
void ov81_02240AD8(Work *,void *,int,int,u8,u8,u8,u8,u16,u8);
void ov81_02240BB0(Work *);
#endif
