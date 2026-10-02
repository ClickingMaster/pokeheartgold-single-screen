#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{void*course;u8 pad[0x68-4];void*all[7];u8 gap[0xe4-0x84];u8 state,unused;u16 previous;};void ManagedSprite_SetAnim(void*,int);int ov96_021E5F24(void*);int ov96_02210324(struct S*,int,int);void PlaySE_SetPitch(int,int);int ov96_02210390(struct S*,u32,int);
void ManagedSprite_SetPaletteOverride(void*,int);void ov96_0221040C(struct S*,int);
int ov96_02210390(struct S*s,u32 index,int value){int result;GF_ASSERT(s!=NULL);GF_ASSERT(index<4);{struct S*shifted=(struct S*)((u8*)s+((index+3)<<2));ManagedSprite_SetAnim(shifted->all[0],index+4);}if(index==ov96_021E5F24(s->course)&&value!=s->previous){result=ov96_02210324(s,index,value);if(result==1){}else if(result==2)PlaySE_SetPitch(2189,128);else if(result==3)PlaySE_SetPitch(2189,256);else PlaySE_SetPitch(2189,320);return 1;}return 0;}
void ov96_0221040C(struct S*s,int number){u8 rem=number%100;int tens=(u8)(rem/10);int ones=(u8)(rem%10);switch(s->state){case 0:if(rem<=20){int i;for(i=0;i<2;i++)ManagedSprite_SetPaletteOverride(s->all[i+1],2);s->state++;}break;case 1:if(rem<=3){int i;for(i=0;i<2;i++)ManagedSprite_SetPaletteOverride(s->all[i+1],3);s->state++;}break;default:GF_AssertFail();break;case 2:break;}tens+=9;ManagedSprite_SetAnim(s->all[1],tens);ones+=9;ManagedSprite_SetAnim(s->all[2],ones);}
