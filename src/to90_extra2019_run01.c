#pragma force_active on
#include "global.h"
#pragma force_active on
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
#define SH(p,n) (*(s16 *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
typedef struct{int a,b,c,d;}Surface;extern Surface ov43_0222F0FC;extern void (*ov43_0222F0EC[])(void *,void *,s16,s16);
extern void ScheduleSetBgPosText(void *,int,int,int),GF_SetG2dRendererSurface(void *,Surface *),ov43_0222C9A4(void *,void *,void *),ov43_0222C378(void *,void *,void *,s16,s16,int,void *),ov43_0222AD20(void *,int);
int ov43_0222C024(u8 *,void *,u8 *,void *);
int ov43_0222C024(u8 *p,void *arg,u8 *s,void *other){Surface surf;int offset;if(SH(p,0xb0)>=8){ScheduleSetBgPosText(P(s,0),2,0,0);ScheduleSetBgPosText(P(s,0),3,0,0);surf=ov43_0222F0FC;GF_SetG2dRendererSurface(s+8,&surf);ov43_0222C9A4(p+8+0x38*(H(p,0xb2)==2?2:1),s,s+0x200);ov43_0222C378(p,arg,s,SH(p,0),SH(p,4),0,other);ov43_0222AD20(s,1);return 1;}offset=(SH(p,0xb0)*256)/8;if(H(p,0xb2)==2)offset=-offset;ScheduleSetBgPosText(P(s,0),2,0,offset);ScheduleSetBgPosText(P(s,0),3,0,offset);surf=ov43_0222F0FC;surf.a+=((u32)offset<<12);GF_SetG2dRendererSurface(s+8,&surf);if(SH(p,0xb0)%2){void (*cb)(void *,void *,s16,s16)=ov43_0222F0EC[SH(p,0xb0)/2];if(cb)cb(p,s,SH(p,0),SH(p,2));}SH(p,0xb0)++;return 0;}
