#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
int DWCi_Acc_GetFlag_DataType(void*);u32 DWCi_Acc_GetFlags(void*);void DWCi_Acc_SetFlags(void*,u32);
void DWCi_SetBuddyFriendData(void*);
void DWCi_SetBuddyFriendData(void *friend){if(DWCi_Acc_GetFlag_DataType(friend)==3){u32 flags=DWCi_Acc_GetFlags(friend)|4;DWCi_Acc_SetFlags(friend,flags);}}
