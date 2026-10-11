#ifndef W08_OV15_PRIVATE_H
#define W08_OV15_PRIVATE_H
#include "global.h"
#include "constants/items.h"
#include "constants/sndseq.h"
void BufferPlayersName(void *, u32, void *);
void BufferItemName(void *, u32, u32);
void *NewString_ReadMsgData(void *, s32);
void BagApp_SetFlute(void *, u8);
void *BagApp_TryUseRepel(void *, u16);
void *BagApp_ToggleGBSounds(void *, u16);
void StringExpandPlaceholders(void *, void *, void *);
void String_Delete(void *);
void *BagApp_GetSaveRoamers(void *);
BOOL RoamerSave_RepelNotInUse(void *);
s32 GetItemAttr(u16, u16, int);
void BagApp_SetRepelStepCount(void *, u8);
BOOL PlaySE(u16);
BOOL BagApp_TryUseItemInPlace(void *, u16);
#endif
