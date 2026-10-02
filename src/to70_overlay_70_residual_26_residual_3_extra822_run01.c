#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#pragma force_active on
typedef struct Choice{s16 species;s8 value,a,b;u8 pad[0x13];void*p,*q,*r;}Choice;typedef struct W{u8 p[0x2c];int state;u8 p30[0xb54];Choice choice;}W;extern int ov70_02242144(void*),ov70_0223EDE4(Choice*,void*),ov70_0223F864(s8,s8,int);extern void ov70_0223F828(Choice*,int,int),ov70_0223F3D8(void*,void*,void*,void*,s16,s8,int);int ov70_0223F008(W*);
int ov70_0223F008(W*w){u32 selected=ov70_02242144(*(void**)((u8*)w+0x11a8));switch(selected){case 12:case -2:ClearFrameAndWindow2((Window*)((u8*)w+0xf18),0);if(ov70_0223EDE4(&w->choice,*(void**)(*(u8**)((u8*)w+0x11c4)+0x20)))w->state=0;else w->state=7;break;case -1:break;default:ov70_0223F828(&w->choice,selected,0);w->state=13;{int x=ov70_0223F864(w->choice.a,w->choice.b,0);ov70_0223F3D8(w->choice.q,w->choice.r,w->choice.p,(u8*)w+0x1058,w->choice.species,w->choice.value,x);}break;}return 3;}
