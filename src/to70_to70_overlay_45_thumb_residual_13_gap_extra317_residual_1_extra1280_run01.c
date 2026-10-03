#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x1c4];u16 id;u8 state,p2[3],kind;} Work;
extern void *ov45_0222EC90(u16);extern void ov45_0222BE28(Work *,void *),ov45_0222BE00(void *,int),ov45_0222BE48(void *),ov45_0222EF4C(int,void *,void *,int);
void ov45_0222AED8(Work *,int);
void ov45_0222AED8(Work *w,int unused){int flag=0;if(w->kind==1){if(w->state!=2)flag=1;}else if(w->kind==2){if(w->state!=3)flag=1;}if(flag){ov45_0222BE28(w,ov45_0222EC90(w->id));return;}switch(w->kind){case 1:w->state=3;break;case 2:w->state=2;break;default:ov45_0222BE28(w,ov45_0222EC90(w->id));return;}ov45_0222BE00((u8 *)w+0x1c0,unused);ov45_0222EF4C(2,ov45_0222EC90(w->id),(u8 *)w+0x1e0,4);ov45_0222BE48((u8 *)w+0x1c0);}
