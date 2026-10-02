#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
typedef struct {u8 p[0x10];void*arg;int selected;u8 p2[0x378];void*sprites[4];u8 p3[0xd0];u8 links[4];} Work;
extern int ov27_0225B360(int,int,void*);
extern void PlaySE(int);
extern void Sprite_SetAnimCtrlSeq(void*,int);
extern void ov27_0225B398(Work*,int);
extern u8 ov27_0225C170(Work*,int);
void ov27_0225B404(Work*);
void ov27_0225B404(Work*w){int dir=-1;int selected=w->selected;if(gSystem.newKeys&0x40)dir=0;else if(gSystem.newKeys&0x80)dir=1;else if(gSystem.newKeys&0x20)dir=2;else if(gSystem.newKeys&0x10)dir=3;if(dir>=0)selected=ov27_0225B360(selected,dir,w->links);if(selected!=-1 && w->selected!=selected){w->selected=selected;PlaySE(0x5e0);if((u32)dir<=1)Sprite_SetAnimCtrlSeq(w->sprites[w->selected],3);else if((u32)(dir-2)<=1)Sprite_SetAnimCtrlSeq(w->sprites[w->selected],1);ov27_0225B398(w,w->selected);*((u8*)w->arg+0xd3)=ov27_0225C170(w,w->selected);}}
