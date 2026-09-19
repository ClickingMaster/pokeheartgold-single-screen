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
void ov18_021F95FC(void *, void *, u32, u32, u32, u32, u32);
void ov18_021EEA84(void *, u32, u32, u32, u32, u32, u32, u32);
void ov18_021EEAE4(void *, u32, u32, u32);
void ov18_021EEB34(void *, u32, u32, u32, u32, u32, u32, u32);
void ov18_021EEB94(void *, u32, u32, u32);
#endif
