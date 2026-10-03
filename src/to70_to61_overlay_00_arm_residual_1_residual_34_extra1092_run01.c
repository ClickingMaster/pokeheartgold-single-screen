#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u8 *data; u32 pad4,pad8; int sent; u32 pad10; int total; u32 pad18; u8 active,pad1d[7]; u64 tick; u32 timeout; } Channel;
typedef struct { u8 pad[0x600]; void (*done)(int,int); u32 pad604; void (*expired)(int); u32 pad60c; u16 maximum; } Work;
extern Work *ov00_0221B0A0;
extern int ov00_021EE4AC(u8**);
extern int ov00_021EE530(int);
extern Channel *ov00_021F9CBC(int);
extern int ov00_021EE490(void);
extern int ov00_021F9CD4(int);
extern int ov00_021FA090(int);
extern void ov00_021F9D0C(int,void*,int,int);
void ov00_021F9A8C(void);
void ov00_021F9A8C(void) {
 u8 *clients; int count,i;
 if(!ov00_0221B0A0) return;
 count=ov00_021EE4AC(&clients);
 for(i=0;i<count;i++) {
  int id=clients[i];
  if(ov00_021EE530(id)) {
   Channel *channel=ov00_021F9CBC(id);
   if(ov00_0221B0A0->expired && channel->timeout) {
    u64 now=OS_GetTick();
    if((u32)(((now-channel->tick)<<6)/0x82ea)>channel->timeout) {
     ov00_0221B0A0->expired(id); channel->tick=now;
    }
   }
  }
  if(id!=ov00_021EE490() && ov00_021F9CD4(id)==1) {
   Channel *channel=ov00_021F9CBC(id);
   int size=ov00_0221B0A0->maximum;
   if(channel->total-channel->sent<=size) size=channel->total-channel->sent;
   if(ov00_021FA090(id)>=size) {
    int total;
    ov00_021F9D0C(id,channel->data+channel->sent,size,1);
    channel->sent+=size; total=channel->total;
    if(channel->sent==total) {
     channel->active=0; channel->data=0; channel->sent=0; channel->total=0;
     if(ov00_0221B0A0->done) ov00_0221B0A0->done(total,id);
    }
   }
  }
 }
}
