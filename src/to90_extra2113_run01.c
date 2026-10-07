#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 words[3];} Friend;
typedef struct {u8 pad[20];int count;Friend *friends;u8 pad2;u8 changed;} List;
typedef struct {u32 a,b;List *list;} Global;
typedef struct {int status,profile;u8 pad[0x86];char name[1];} Reply;
extern Global ov00_0221A918;
extern int DWC_GetFriendDataType(Friend *);
extern void *ov00_021EF3A4(void);
extern void DWC_LoginIdToUserName(void *,Friend *,char *);
extern void DWC_SetGsProfileId(Friend *,int);
extern void DWCi_SetBuddyFriendData(Friend *);
extern int DWC_GetGsProfileId(void *,Friend *);
extern int DWC_IsBuddyFriendData(Friend *);
extern void ov00_021F0A50(Friend *,int,int);
extern void ov00_021F0550(void);
void ov00_021F10A8(void *,Reply *);
void ov00_021F10A8(void *unused,Reply *reply){
 int i,updated=0,buddy=0;char name[24];
 if(reply->status)return;
 for(i=0;i<ov00_0221A918.list->count;i++){
  if(DWC_GetFriendDataType(&ov00_0221A918.list->friends[i])==1){
   DWC_LoginIdToUserName(ov00_021EF3A4(),&ov00_0221A918.list->friends[i],name);
   if(strcmp(name,reply->name)==0){
    DWC_SetGsProfileId(&ov00_0221A918.list->friends[i],reply->profile);
    DWCi_SetBuddyFriendData(&ov00_0221A918.list->friends[i]);updated=1;
   }
  }else if(DWC_GetFriendDataType(&ov00_0221A918.list->friends[i])==3 || DWC_GetFriendDataType(&ov00_0221A918.list->friends[i])==2){
   if(DWC_GetGsProfileId(ov00_021EF3A4(),&ov00_0221A918.list->friends[i])==reply->profile){
    if(DWC_IsBuddyFriendData(&ov00_0221A918.list->friends[i])==1)buddy=1;
    else {DWC_SetGsProfileId(&ov00_0221A918.list->friends[i],reply->profile);DWCi_SetBuddyFriendData(&ov00_0221A918.list->friends[i]);updated=1;}
   }
  }
 }
 if(updated){ov00_021F0A50(ov00_0221A918.list->friends,ov00_0221A918.list->count,reply->profile);if(!buddy)ov00_021F0550();ov00_0221A918.list->changed=1;}
}
