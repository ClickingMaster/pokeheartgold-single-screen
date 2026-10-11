#ifndef W13_R3_PRIVATE_H
#define W13_R3_PRIVATE_H
#include "global.h"
void *NewMsgDataFromNarc(u32, u32, u32, u32);
void *NewString_ReadMsgData(void *, u32);
void String_Delete(void *);
void DestroyMsgData(void *);
int GetDexHeightMsgBank(void);
int GetDexWeightMsgBank(void);
void FillWindowPixelBuffer(void *, u32);
void ScheduleWindowCopyToVram(void *);
void PokedexApp_PrintString(void *, void *, u32, u32, u32, u32, u32);
void PokedexApp_PrintHeight(void *, u32, u32, u32, u32, u32, u32, u32);
void PokedexApp_DrawHeight(void *, u32, u32, u32);
void PokedexApp_PrintWeight(void *, u32, u32, u32, u32, u32, u32, u32);
void PokedexApp_DrawWeight(void *, u32, u32, u32);
#endif
