#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 bytes[20];} Small;
typedef struct {u8 bytes[21];} Large;
extern int ov00_0220FB6C(const void*);
extern void *ov00_0220F9A8(u32);
extern void ov00_022104D4(void*,const Small*,void*),ov00_02210590(void*,const Small*,void*),ov00_02210658(void*,const Large*,void*);
static inline u32 swap(u32 id){return ((id>>24)&0xff)|((id>>8)&0xff00)|((id<<8)&0xff0000)|((id<<24)&0xff000000);}
void ov00_0221075C(const u8*,int,void*);
#pragma force_active on
void ov00_0221075C(const u8 *packet,int size,void *arg){struct {Large large;u8 padding[3];Small small;} locals;int kind;void *owner;
 if(!ov00_0220FB6C(packet))return;kind=packet[7];
 if(kind==5 || kind==7){if(size<20)return;locals.small=*(const Small*)packet;owner=ov00_0220F9A8(swap(*(u32*)(locals.small.bytes+8)));if(!owner)return;if(kind==5)ov00_022104D4(owner,&locals.small,arg);else ov00_02210590(owner,&locals.small,arg);}
 else{if(size<21)return;locals.large=*(const Large*)packet;owner=ov00_0220F9A8(swap(*(u32*)(locals.large.bytes+8)));if(!owner)return;ov00_02210658(owner,&locals.large,arg);}
}
