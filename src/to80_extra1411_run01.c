#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u32 words[3]; } Friend;
typedef struct { u8 pad[8]; u8 *work; } Work;
extern Work ov00_0221A918;
extern int ov00_021F03BC(int);
extern int DWC_GetFriendDataType(const Friend*);
extern int DWC_IsBuddyFriendData(const Friend*);
extern void DWC_SetGsProfileId(Friend*,int);
extern void DWCi_SetBuddyFriendData(Friend*);
extern void ov00_021F094C(Friend*,int,int);
int ov00_021F0A50(Friend*,int,int);
int ov00_021F0A50(Friend *friends,int count,int desired) {
 int result=-1,id,i;
 for(i=0;i<count;i++) {
  id=ov00_021F03BC(i);
  if(id) {
   int j;
   if(id==desired)result=i;
   for(j=i+1;j<count;j++) {
    if(id==ov00_021F03BC(j)) {
     if(DWC_GetFriendDataType(&friends[i])==2 && DWC_GetFriendDataType(&friends[j])==3) DWC_SetGsProfileId(&friends[i],id);
     if(DWC_IsBuddyFriendData(&friends[j])) DWCi_SetBuddyFriendData(&friends[i]);
     ov00_021F094C(friends,j,i);ov00_0221A918.work[0x1d]=1;
    }
   }
  }
 }
 return result;
}
