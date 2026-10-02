#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Template {u32 v[2];s32 x,y;u32 rest[8];} Template;
extern void ov74_02231D48(Template*,void*,void*,int);
extern void *Sprite_CreateAffine(Template*);
extern void Sprite_SetAnimActiveFlag(void*,int);
extern void Sprite_SetAnimCtrlSeq(void*,int);
extern void Sprite_SetPriority(void*,int);
extern void Sprite_SetDrawFlag(void*,int);
extern void Sprite_SetDrawPriority(void*,int);
void ov74_02232154(u8*);
extern void ov74_02231DDC(void*,int,int,int,int);
void ov74_022322D8(u8*);
void *ov74_02232284(u8*,int,int,int,int);
void *ov74_02232284(u8 *w,int x,int y,int anim,int draw){Template t;ov74_02231D48(&t,w,w+0x184,1);t.x=x<<12;t.y=y<<12;t.rest[5]=10;void *sprite=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(sprite,1);Sprite_SetAnimCtrlSeq(sprite,anim);Sprite_SetPriority(sprite,1);Sprite_SetDrawFlag(sprite,draw);return sprite;}

void ov74_022322D8(u8*w){ov74_02231DDC(w+0x388,228,176,50,32);*(void**)(w+0x398)=ov74_02232284(w,228,176,6,1);ov74_02231DDC(w+0x390,176,176,32,32);*(void**)(w+0x3a8)=ov74_02232284(w,176,176,3,1);ov74_02231DDC(w+0x38c,24,176,32,32);*(void**)(w+0x3b8)=ov74_02232284(w,24,176,0,1);w[0x394]=255;int i=0;int x=0;u8*p=w;for(;i<6;i++,x+=36,p+=12){*(void**)(p+0x3c8)=ov74_02232284(w,x,64,9,0);*(s32*)(p+0x3cc)=-1;*(s32*)(p+0x3d0)=14;}*(u32*)(w+0x410)=0;}
