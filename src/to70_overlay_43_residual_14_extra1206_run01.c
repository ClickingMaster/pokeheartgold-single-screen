#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct { s16 a,b,c,d;u8 data[0xec];int timer; } State;
typedef struct {u32 x;void *save;u8 state,x9,xa;s8 index;u8 padc[0xc];u8 slots[0x40];u32 value;}Work;
extern void ov43_0222A420(Work *),ov43_0222BB20(State *,Work *,void *,int),ov43_0222C53C(State *,Work *,int),ov43_0222BFD4(State *,Work *,void *,int);
extern int ov43_0222BFA4(State *,Work *,void *),ov43_0222C024(State *,Work *,void *,int),ov43_0222C620(State *),ov43_0222C228(State *,Work *,void *);
extern void ov43_0222A41C(Work *,int),ov43_0222C148(void *,void *,void *,int),ov43_0222BBB8(State *,Work *,void *),ov43_0222A318(Work *,int,int);
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);extern int IsPaletteFadeFinished(void);
extern void *sub_0202C6F4(void *),*Save_Frontier_GetStatic(void *);
extern int sub_0202C254(void *,int);extern void sub_0202C270(void *,int,u32),sub_0202C338(void *,int),sub_020311AC(void *,int),ov43_0222A414(Work *,int);
extern void ov43_0222C32C(State *,Work *,void *,int),ov43_0222C600(State *),ov43_0222AD00(void *,int),ov43_0222C890(void *,void *,int,int),ov43_0222C65C(State *,void *),ov43_0222C750(State *,void *);
extern int ov43_0222C358(State *,Work *,void *),ov43_0222AE2C(Work *,void *),ov43_0222C714(State *,void *),ov43_0222C2B0(State *,Work *,void *,int),ov43_0222C5D8(State *,Work *);
extern void ov43_0222C378(State *,Work *,void *,int,int,int,int);
extern int System_GetTouchNew(void);
int ov43_0222B5D0(State *,Work *,void *,int);
int ov43_0222B5D0(State *s,Work *w,void *ctx,int heap) {
 int index;void *saved;
 switch(w->state) {
 case 0:s->a=0;s->b=0;s->c=0;s->d=0;
 case 1:ov43_0222A420(w);ov43_0222BB20(s,w,ctx,heap);w->state=2;break;
 case 2:switch(ov43_0222BFA4(s,w,ctx)){case 0:break;case 1:ov43_0222C53C(s,w,2);break;case 2:ov43_0222C53C(s,w,3);break;case 3:w->state=23;break;case 4:s->timer=4;w->state=5;break;}break;
 case 3:ov43_0222BFD4(s,w,ctx,heap);w->state=4;break;
 case 4:if(ov43_0222C024(s,w,ctx,heap)==1)w->state=2;break;
 case 5:if(s->timer--<=0){s->timer=0;ov43_0222A41C(w,ov43_0222C620(s));ov43_0222C148(s,w,ctx,heap);w->state=6;}break;
 case 6:w->state=ov43_0222C228(s,w,ctx);break;
 case 7:s->timer=4;w->state=8;break;
 case 8:s->timer--;if(s->timer<=0){ov43_0222BBB8(s,w,ctx);ov43_0222A318(w,4,0);return 1;}break;
 case 9:s->a=w->index/8;s->c=w->index%8;w->state=1;break;
 case 10:BeginNormalPaletteFade(0,16,16,0,6,1,heap);index=ov43_0222C620(s);saved=sub_0202C6F4(w->save);ov43_0222A414(w,sub_0202C254(saved,w->slots[index]));w->state=11;break;
 case 11:if(IsPaletteFadeFinished()){ov43_0222BBB8(s,w,ctx);ov43_0222A318(w,2,4);return 1;}break;
 case 12:index=ov43_0222C620(s);saved=sub_0202C6F4(w->save);sub_0202C270(saved,w->slots[index],w->value);
 case 13:ov43_0222BB20(s,w,ctx,heap);BeginNormalPaletteFade(0,17,17,0,6,1,heap);w->state=14;break;
 case 14:if(IsPaletteFadeFinished())w->state=2;break;
 case 15:ov43_0222C32C(s,w,ctx,heap);w->state=16;break;
 case 16:if(ov43_0222C358(s,w,ctx)==1)w->state=17;break;
 case 17:switch(ov43_0222AE2C(w,ctx)){case 1:ov43_0222C600(s);w->state=18;break;case 2:ov43_0222C600(s);ov43_0222AD00(ctx,1);ov43_0222C890(s->data,ctx,s->c,0);w->state=2;break;}break;
 case 18:ov43_0222C65C(s,ctx);w->state=19;break;
 case 19:if(ov43_0222C714(s,ctx)==1){saved=sub_0202C6F4(w->save);index=ov43_0222C620(s);sub_0202C338(saved,w->slots[index]);sub_020311AC(Save_Frontier_GetStatic(w->save),w->slots[index]);ov43_0222A420(w);ov43_0222C378(s,w,ctx,s->a,s->c,0,heap);ov43_0222AD00(ctx,1);w->state=20;}break;
 case 20:ov43_0222C750(s,ctx);w->state=2;break;
 case 21:if(ov43_0222C2B0(s,w,ctx,heap)==1)w->state=22;else{ov43_0222AD00(ctx,1);w->state=2;}break;
 case 22:if(ov43_0222C5D8(s,w)&&(gSystem.newKeys||System_GetTouchNew())){ov43_0222AD00(ctx,1);ov43_0222C890(s->data,ctx,s->c,0);w->state=2;ov43_0222C600(s);}break;
 case 23:ov43_0222BBB8(s,w,ctx);ov43_0222A318(w,0,2);return 1;
 }return 0;
}
