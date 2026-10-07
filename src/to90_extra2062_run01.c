#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 kind:2;u32 mode:8;u32 ticks:11;u32 rest:11;} Flags;
typedef struct {u8 pad[16];void *main,*shadow,*other;u8 pad2[24];u8 anim[12];Flags flags;} State;
typedef struct {u32 a,b,c;void *sequence;} Control;
typedef struct {u8 pad[4];s16 x,y;} Element;
typedef struct {s16 y,x;} Point;
extern u32 ov96_0221E5A4;
extern void ov96_0220B940(State *,int),ov96_0220B95C(State *),ov96_0220B988(State *),ov96_0220B9A8(State *),ov96_0220C680(State *),ov96_0220BD38(State *),ov96_0220D2AC(void *,int,u8);
extern int ManagedSprite_IsAnimated(void *);
extern Control *Sprite_GetCellAnim(void *);
extern Element *NNS_G2dGetAnimCtrlCurrentElement(Control *);
extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *),ManagedSprite_SetPositionXY(void *,s16,s16);
void ov96_0220B528(State *,int);
void ov96_0220B528(State *s,int arg){
 Control *control;Element *element;Point p;
 GF_ASSERT(s!=0);
 switch(s->flags.mode){case 0:ov96_0220B940(s,arg);break;case 1:ov96_0220B95C(s);break;case 2:ov96_0220B988(s);break;case 3:ov96_0220B9A8(s);break;default:GF_ASSERT(0);break;}
 ov96_0221E5A4++;
 if(arg==0)ov96_0220C680(s);
 ov96_0220BD38(s);ov96_0220D2AC(s->anim,s->flags.ticks,(u8)s->flags.kind);s->flags.ticks++;
 if(ManagedSprite_IsAnimated(s->main)){
  control=Sprite_GetCellAnim(*(void **)s->main);element=NNS_G2dGetAnimCtrlCurrentElement(control);GF_ASSERT(control->sequence!=0);
  ManagedSprite_GetPositionXY(s->main,&p.x,&p.y);p.x+=element->x;p.y+=element->y;
  ManagedSprite_SetPositionXY(s->shadow,p.x,p.y);p.y-=24;ManagedSprite_SetPositionXY(s->other,p.x,p.y);
 }
}
