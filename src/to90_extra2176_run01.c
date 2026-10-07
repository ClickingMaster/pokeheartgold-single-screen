#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x198];void *sprites[18];} Work;
extern int Party_GetCount(void *);extern void *Party_GetMonByIndex(void *,int);extern u32 GetMonData(void *,int,void *);extern int Pokemon_GetStatusIconId(void *);extern void ManagedSprite_SetAnim(void *,int);
void ov05_0221D414(Work *,void *,int,int);
void ov05_0221D414(Work *w,void *party,int limit,int first){
u8 i,written,count,status;void *mon;count=Party_GetCount(party);written=0;
for(i=0;i<limit;i++){ManagedSprite_SetAnim(w->sprites[first+i],0);if(count>i){mon=Party_GetMonByIndex(party,i);if(GetMonData(mon,5,0)!=0&&GetMonData(mon,76,0)==0){status=Pokemon_GetStatusIconId(mon);if(status==6)ManagedSprite_SetAnim(w->sprites[first+written],3);else if(status!=7)ManagedSprite_SetAnim(w->sprites[first+written],2);else ManagedSprite_SetAnim(w->sprites[first+written],1);written++;}}}
}
