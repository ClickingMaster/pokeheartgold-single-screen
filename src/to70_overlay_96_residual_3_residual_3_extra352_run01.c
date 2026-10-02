#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x708];void *sprite;u8 tick;u8 pad2[0x1c];u8 state;} Work;
extern void PlaySE(int);
extern void ov96_021E65D8(Work *);
extern int ov96_021E661C(Work *);
extern void sub_020053A8(int,int);
extern void Sprite_SetDrawFlag(void *,int);
extern void Sprite_SetAnimCtrlSeq(void *,int);
int ov96_021E637C(Work *);
int ov96_021E637C(Work *w){int done=0;switch(w->state){
case 0:PlaySE(0x89a);ov96_021E65D8(w);w->state++;break;
case 1:if(ov96_021E661C(w)){w->state++;sub_020053A8(7,0);}break;
case 2:if(w->tick==0){PlaySE(0x89d);Sprite_SetDrawFlag(w->sprite,1);Sprite_SetAnimCtrlSeq(w->sprite,0);}else if(w->tick==21){PlaySE(0x89d);Sprite_SetAnimCtrlSeq(w->sprite,1);}else if(w->tick==42){PlaySE(0x89d);Sprite_SetAnimCtrlSeq(w->sprite,2);}
w->tick++;if(w->tick>63){PlaySE(0x892);Sprite_SetDrawFlag(w->sprite,0);w->tick=0;done=1;}break;
}return done;}
