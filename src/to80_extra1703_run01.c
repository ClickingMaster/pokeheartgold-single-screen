#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Desc { int power, other; u8 pad[8]; int altPower, altOther; } Desc;
typedef struct Info { void *pad; Desc *desc; } Info;
typedef struct E E;
struct E { void *root; Info *info; u8 p8[8]; E *link; int kind, prior; u8 p1c[0x28]; VecFx32 vec; u8 p50[13]; u8 action; u8 p5e[2]; struct { u32 count:12; u32 level:2; u32 style:2; u32 pad:4; u32 color:2; u32 channel:2; u32 pad2:3; u32 hit:1; u32 pad3:3; u32 final:1; } flags; int timer; };
extern int ov96_0221785C(int,int);
extern void ov96_021E8228(void *,u8,u8,int,int);
extern void ov96_02215958(VecFx32 *, int);
extern void ov96_02217868(E *, VecFx32 *);
extern int ov96_02217890(int);
extern void ov96_02219174(E *,int);
int ov96_022178A0(E *, E *, VecFx32 *);
int ov96_022178A0(E *target, E *source, VecFx32 *direction) {
 int power=source->info->desc->power;
 int other=source->info->desc->other;
 if(ov96_0221785C(target->kind, source->kind)) {
 if(source->kind==6) {
 int n;
 power=(int)(3.0f*(float)power); other=(int)(2.0f*(float)other);
 n=(int)source->flags.count+1; if(n>200)n=200; source->flags.count=n;
 ov96_021E8228(source->root,(u8)source->flags.color,(u8)source->flags.channel,3,1);
 ov96_021E8228(source->root,(u8)source->flags.color,(u8)source->flags.channel,4,1);
 } else if(source->kind==11) {
 int n;
 power=source->info->desc->altPower;other=source->info->desc->altOther;
 n=(int)source->flags.count+2;if(n>200)n=200;source->flags.count=n;
 ov96_021E8228(source->root,(u8)source->flags.color,(u8)source->flags.channel,3,1);
 ov96_021E8228(source->root,(u8)source->flags.color,(u8)source->flags.channel,4,1);
 } else power=3<<12;
 VEC_MultAdd(power,direction,&target->vec,&target->vec);
 ov96_02215958(&target->vec,3<<14);
 ov96_02217868(source,direction);
 if(ov96_02217890(source->kind)) { target->action=4; source->action=4; ov96_02219174(target,other); }
 if(!target->link || ov96_02217890(source->kind)) { target->link=source;if(source->link && source->link!=target)target->link=source->link; }
 target->flags.hit=1;target->prior=source->kind;
 if(ov96_02217890(source->kind)) {
 u32 value;
 if(target->flags.style!=2) { switch(source->kind) {case 6: target->flags.style=1;break;case 11:target->flags.style=2;break;} }
 value=source->flags.color;if(value<target->flags.color)value++;target->flags.level=value;target->timer=4;
 } else target->flags.final=1;
 GF_ASSERT(source->kind!=3 && source->kind!=4);
 GF_ASSERT(target->kind!=3 && target->kind!=4 && target->kind!=11);
 return 1;
 } return 0;
}
