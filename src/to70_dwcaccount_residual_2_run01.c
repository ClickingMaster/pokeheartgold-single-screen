#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#pragma force_active on
extern u8 MATH_CalcCRC8(const MATHCRC8Table *,const void *,u32);
extern const s8 _021105F4[];
BOOL DWC_Acc_CheckFriendKey(u64,u32);
int DWC_Acc_FriendKeyToGsProfileId(u64,u32);
void DWCi_Acc_U64ToString32(u64,int,char *);
int DWC_Acc_FriendKeyToGsProfileId(u64 key,u32 game){if(DWC_Acc_CheckFriendKey(key,game))return key&0xffffffffULL;return 0;}
