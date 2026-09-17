#ifndef POKEHEARTGOLD_TO50_OV83_R38_PRIVATE_H
#define POKEHEARTGOLD_TO50_OV83_R38_PRIVATE_H

#include "global.h"

#define PTR_AT(p, off) (*(void **)((u8 *)(p) + (off)))
#define U16_AT(p, off) (*(u16 *)((u8 *)(p) + (off)))

typedef struct Ov83R38Flags {
    u8 gender : 7;
    u8 flag : 1;
} Ov83R38Flags;

int ov83_0224777C(void *, int, int);
int ov83_02247768(int, int);
void FillWindowPixelBuffer(void *, int);
void CopyWindowPixelsToVram_TextMode(void *);
void ScheduleWindowCopyToVram(void *);
int GetWindowWidth(void *);
void ov83_022479E4(void *, void *, int, int, int, int, u32, int);
void ov83_02247998(void *, void *, int, int, u32, int, int);
void ov83_02244A98(u8 *, int, int, int, int);
void ov83_02245D08(u8 *, void *, void *, int, int, int, int, u32, int);
void *Mon_GetBoxMon(void *);
void BufferBoxMonSpeciesName(void *, int, void *);
void BufferAbilityName(void *, int, int);
void BufferNatureName(void *, int, int);
void BufferItemName(void *, int, int);
void BufferMoveName(void *, int, int);
void *NewString_ReadMsgData(void *, int);
int FontID_String_GetWidth(int, void *, int);
void String_Delete(void *);
void StringExpandPlaceholders(void *, void *, void *);
void ov83_02246114(u8 *, int);

#endif
