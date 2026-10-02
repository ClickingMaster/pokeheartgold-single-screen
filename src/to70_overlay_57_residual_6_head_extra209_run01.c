#pragma force_active on
#include "global.h"
#pragma force_active on
struct Capsule{void*ptr;u32 pad;};struct S{u8 pad8[8];struct Capsule caps[12];u8 p68[24];u8 owned[81];u8 pD1[0x450-0xd1];u8 list[8];};int SealIsOnCapsule(void*,int);void ov57_022388E4(struct S*,int);
void ov57_022388E4(struct S*s,int page){int limit,count=0,eligible=0,i,j;for(i=0;i<8;i++)s->list[i]=0;limit=page*8;for(i=0;i<81;i++){int found;for(j=0;j<12;j++){found=SealIsOnCapsule(s->caps[j].ptr,i);if(found)break;}if(s->owned[i] || found==1){eligible++;if(eligible>limit){s->list[count]=i+1;count++;if(count>=8)break;}}}}
