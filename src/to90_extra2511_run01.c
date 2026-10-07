#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct{u16 x,y,z,anim;int priority,pal,vram;int res[6];int transfer,draw;} Template;
void*SpriteSystem_NewSprite(void*,void*,const Template*);void ManagedSprite_SetPositionXY(void*,s16,s16),ManagedSprite_SetAnim(void*,int),ManagedSprite_TickFrame(void*),ManagedSprite_SetAffineOverwriteMode(void*,int);int sub_02086D98(u32,u32);void sub_02086758(u8*);
void sub_02086758(u8*d){int inserted=0;void*system=P(d,0x2f0),*manager=P(d,0x2f4);int i,x;u8*special,*digit;u32 value;Template t;t.x=inserted;t.y=inserted;t.z=inserted;t.anim=inserted;t.priority=10;t.transfer=inserted;t.draw=inserted;t.pal=inserted;t.vram=1;t.res[0]=1000;t.res[1]=1000;t.res[2]=1000;t.res[3]=1000;t.res[4]=-1;t.res[5]=-1;value=U(d,0x3e0);
 i=(int)U(d,0x3f0)-1;if(i>=0){u32 cursor=(u32)d+(u32)i*28;do{U((u8*)cursor,0)=value%10+1;value/=10;cursor-=28;i--;}while(i>=0);}
 i=0;if((int)(U(d,0x2d0)+U(d,0x3ec))>0){x=76;special=d;digit=d;do{if(U(d,0x3ec)&&i==(int)(inserted+U(special,0x1c0)+1)){P(special,0x1cc)=SpriteSystem_NewSprite(system,manager,&t);ManagedSprite_SetPositionXY(P(special,0x1cc),(s16)x,24);ManagedSprite_SetAnim(P(special,0x1cc),22);ManagedSprite_TickFrame(P(special,0x1cc));special+=28;inserted++;}else{P(digit,12)=SpriteSystem_NewSprite(system,manager,&t);ManagedSprite_SetPositionXY(P(digit,12),(s16)x,24);ManagedSprite_SetAnim(P(digit,12),sub_02086D98(U(digit,0),U(digit,8)));ManagedSprite_SetAffineOverwriteMode(P(digit,12),2);ManagedSprite_TickFrame(P(digit,12));digit+=28;}i++;x+=8;}while(i<(int)(U(d,0x2d0)+U(d,0x3ec)));}}
