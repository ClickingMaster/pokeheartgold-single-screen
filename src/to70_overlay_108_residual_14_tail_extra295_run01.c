#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Flags{u8 a:1,dir:1,rest:6;}Flags;
void ManagedSprite_OffsetPositionXY(void *,s16,s16),ov108_021E78F4(void *,int,int),ov108_021E7EB0(void *),ov108_021E77D4(void *);
int ov108_021E8270(u8 *);
int ov108_021E8270(u8 *w){int i;u8 *p;int x=((Flags *)(w+0x184e2))->dir==0?-32:32;i=0;p=w;do{ManagedSprite_OffsetPositionXY(*(void **)(p+876),(s16)x,0);i++;p+=4;}while(i<12);w[0x184e1]++;if(w[0x184e1]>=8){w[0x184e1]=0;ov108_021E78F4(w,1,w[0x184e0]);ov108_021E7EB0(w);ov108_021E77D4(w);return 1;}return 0;}
