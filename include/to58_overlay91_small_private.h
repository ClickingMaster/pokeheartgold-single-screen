#ifndef TO58_W01B_OV91_SMALL_PRIVATE_H
#define TO58_W01B_OV91_SMALL_PRIVATE_H
#include "global.h"
int ov90_0225886C(void *base, int arg);
void ov91_0225CD74(void *object, int arg, int value);
void ov91_0225CD84(void *object, int arg, int value);
void ov91_0225D368(void *object);
int ov91_0225D768(void *object);
int ov91_0225D32C(u8 *object, void *out);
int ov91_0225CCA8(u8 *base, int offset, void *data, int size);
int ov91_0225D2E8(void *object);
u32 ov91_0225D37C(void *object);
void GF_AssertFail(void);
void ov91_0225CB98(u8 *base);
void ov91_0225E23C(void *record);
void ov91_0225E400(void *record);
void Main_SetVBlankIntrCB(void *cb, void *arg);
void HBlankInterruptDisable(void);
void ov90_02258938(void *object);
void ov91_0225CCEC(void *object);
void ov91_0225CE80(void *object);
void GF_DestroyVramTransferManager(void);
void WifiConnection_StopVoiceChat(void);
void ov90_02258A04(void *object);
void ov91_0225CC4C(u8 *base, u8 *arg);
void ov91_0225E294(void *record, void *out);
void ov91_0225E2E4(void *base, void *data, int mode);
void ov91_0225D2EC(u8 *base, u8 *record);
void ov91_0225D3B4(void *object, int arg, int index);
void sub_02037030(int id, int a, int b);
void ov91_0225CAEC(u8 *base, int arg, int index);
int ov91_0225CCF4(u8 *base);
void ov91_0225CB64(u8 *base);
void ov91_0225CA74(u8 *base, int arg, int index);
void ov91_0225CA9C(u8 *base, int arg, int index);
void ov91_0225CABC(u8 *base, u32 *values);
void ov91_0225D694(u8 *base, int index, int unused, void *ptr);
#endif
