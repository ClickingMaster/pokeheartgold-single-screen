#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {void*owner;u8 pad4[8];void*ctx;u32 p10;void*man;u8 pad18[0x2c];void*values[3],*icons[3],*extra[3],*digits[3],*texts[3];} Work;
extern void*ov99_021E5B90(void*),*ov99_021E5B7C(void*,int,int);extern int ov99_021E5BB4(void*,int);extern u32 ov98_0221F120(void*,u8);extern u32 ov99_021E9628[];extern void ManagedSprite_SetAnim(void*,u32),ManagedSprite_SetDrawFlag(void*,int),ov98_0221E7E8(void*,void*,int,int,void*),ov98_0221E970(void*,void*,void*,int,int);
void ov99_021E6530(Work*,int);
void ov99_021E6530(Work*w,int mode){void*a=ov99_021E5B90(w->owner);u8 i;for(i=0;i<3;i++){if(ov99_021E5BB4(w->owner,mode)==1){u16 anim=(u16)ov98_0221F120(a,(u8)(i+1));ManagedSprite_SetAnim(w->digits[i],anim);ManagedSprite_SetAnim(w->icons[i],ov99_021E9628[anim]);ManagedSprite_SetDrawFlag(w->icons[i],1);ManagedSprite_SetDrawFlag(w->digits[i],1);}else{ManagedSprite_SetDrawFlag(w->icons[i],0);ManagedSprite_SetDrawFlag(w->digits[i],0);}}for(i=0;i<3;i++){void*b=ov99_021E5B7C(w->owner,mode,i);if(ov99_021E5BB4(w->owner,mode)==1){ov98_0221E7E8(w->values[i],b,1,0,w->ctx);ManagedSprite_SetDrawFlag(w->values[i],1);ov98_0221E970(w->man,w->texts[i],b,0,1);ManagedSprite_SetDrawFlag(w->texts[i],1);}else ManagedSprite_SetDrawFlag(w->texts[i],0);}for(i=0;i<3;i++)ManagedSprite_SetDrawFlag(w->extra[i],ov99_021E5BB4(w->owner,mode));}
