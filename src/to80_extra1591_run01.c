#pragma force_active on
#include "global.h"
extern struct{u8 y[4],x[4];}ov96_0221BFF4;
extern void ov96_021EB408(void*,int,int,int,int),ov96_021EB52C(void*,int,int),Sprite_SetDrawPriority(void*,int),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetMatrix(void*,VecFx32*);
extern void*ov96_021EB4F4(void*,int,int),*ov96_021EB5B8(void*);
#define P(w,o) (*(void**)((w)+(o)))
void ov96_021F3F80(void*,u8*);
void ov96_021F3F80(void*w,u8*base){VecFx32 pos={0,0,0};u8 x[4]={ov96_0221BFF4.x[0],ov96_0221BFF4.x[1],ov96_0221BFF4.x[2],ov96_0221BFF4.x[3]};u8 y[4]={ov96_0221BFF4.y[0],ov96_0221BFF4.y[1],ov96_0221BFF4.y[2],ov96_0221BFF4.y[3]};u8 i;void*s,*s1,*s2,*s3;base=(u8*)((u32)base+104);
ov96_021EB408(w,0,2,105,8);ov96_021EB408(w,0,2,105,9);ov96_021EB408(w,1,2,105,10);
for(i=0;i<2;i++)ov96_021EB408(w,0,2,105,11);
for(i=0;i<4;i++)ov96_021EB408(w,1,2,105,12);
for(i=0;i<4;i++)ov96_021EB408(w,1,2,105,13);
P(base,180)=ov96_021EB4F4(w,105,8);s=ov96_021EB5B8(P(base,180));Sprite_SetDrawPriority(s,4);Sprite_SetAnimCtrlSeq(s,0);pos.x=128<<12;pos.y=544<<12;Sprite_SetMatrix(s,&pos);ov96_021EB52C(P(base,180),1,1);
P(base,184)=ov96_021EB4F4(w,105,9);s=ov96_021EB5B8(P(base,184));Sprite_SetDrawPriority(s,3);Sprite_SetAnimCtrlSeq(s,1);pos.x=152<<12;pos.y=544<<12;Sprite_SetMatrix(s,&pos);ov96_021EB52C(P(base,184),1,1);
P(base,196)=ov96_021EB4F4(w,105,10);s=ov96_021EB5B8(P(base,196));Sprite_SetDrawPriority(s,5);Sprite_SetAnimCtrlSeq(s,13);pos.x=0;pos.y=512<<12;Sprite_SetMatrix(s,&pos);ov96_021EB52C(P(base,196),1,0);
for(i=0;i<2;i++){P(base+i*4,188)=ov96_021EB4F4(w,105,11);s1=ov96_021EB5B8(P(base+i*4,188));Sprite_SetDrawPriority(s1,6);Sprite_SetAnimCtrlSeq(s1,i+11);pos.x=(160*i+48)<<12;pos.y=536<<12;Sprite_SetMatrix(s1,&pos);ov96_021EB52C(P(base+i*4,188),1,1);}
for(i=0;i<4;i++){P(base+i*4,200)=ov96_021EB4F4(w,105,12);s2=ov96_021EB5B8(P(base+i*4,200));Sprite_SetDrawPriority(s2,8);Sprite_SetAnimCtrlSeq(s2,14);pos.x=x[i]<<12;pos.y=(y[i]<<12)+(512<<12);Sprite_SetMatrix(s2,&pos);ov96_021EB52C(P(base+i*4,200),1,1);}
for(i=0;i<4;i++){P(base+i*4,216)=ov96_021EB4F4(w,105,13);s3=ov96_021EB5B8(P(base+i*4,216));Sprite_SetDrawPriority(s3,7);Sprite_SetAnimCtrlSeq(s3,18);pos.x=x[i]<<12;pos.y=(y[i]<<12)+(512<<12);Sprite_SetMatrix(s3,&pos);ov96_021EB52C(P(base+i*4,216),1,0);}}
