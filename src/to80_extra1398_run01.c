#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { char magic[4]; int kind; u8 type,size; u16 val; int a,b; } Packet;
typedef struct { u8 pad[21]; u8 state; } Work;
extern int strncmp(const char*,const char*,u32);
extern const char ov00_02217B54[];
extern int ov00_021EE568(void);
extern Work *ov00_021F955C(void);
extern int ov00_021F4740(int,int,int,int,void*,int);
void ov00_021F8FF4(void*,u32);
void ov00_021F8FF4(void *src,u32 size) {
 Packet pkt; int data[32]; u32 used=0;
 if(ov00_021EE568()!=5) {
  if(ov00_021EE568()!=6) return;
  if(ov00_021F955C()->state!=2 && ov00_021F955C()->state!=3) return;
 }
 if(size<20) return;
 do {
  MI_CpuCopy8(src,&pkt,20);
  if(strncmp(pkt.magic,ov00_02217B54,4)!=0 || pkt.kind!=3) return;
  MI_CpuCopy8((char*)src+20,data,pkt.size);
  if(!ov00_021F4740(pkt.type,pkt.b,pkt.a,pkt.val,data,pkt.size>>2)) return;
  used+=pkt.size+20;
 } while(used+20<=size);
}
