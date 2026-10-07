#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct PicTemplate {u16 narc,member,field4,field6;u32 extra[2];} PicTemplate;
typedef struct Data {void *pixels;int narc,field8,height;} Data;
typedef struct Work {int selected,other;Data *data[4];void *pic[4];u16 species[4];u8 gender[4],pad34[4],form[4];u32 pid[4];u8 flags[4];} Work;
extern void Pokepic_Pop(void*);
extern PicTemplate *Pokepic_GetTemplate(void*);
extern void sub_02014540(u16,u16,int,void*,u32,int,int,u16);
extern int GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
extern int ov07_02234B5C(u8,int);
extern void Pokepic_SetAttr(void*,int,int);
void ov07_0223494C(Work*,int);
#pragma opt_propagation off
void ov07_0223494C(Work*w,int arg){Pokepic_Pop(w->pic[w->selected]);PicTemplate *t=Pokepic_GetTemplate(w->pic[w->selected]);int species=w->species[w->selected];int gender=w->gender[w->selected];int form=w->form[w->selected];u32 pid=w->pid[w->selected];int facing;if(w->flags[w->selected]&1)facing=2;else facing=0;sub_02014540(t->narc,t->member,arg,w->data[w->selected]->pixels,pid,0,facing,t->field6);w->data[w->selected]->narc=t->narc;w->data[w->selected]->field8=t->field4;w->data[w->selected]->height=GetMonPicHeightBySpeciesGenderForm((u16)species,(u8)gender,(u8)facing,(u8)form,pid);int selected=w->selected;unsigned int scaled=(unsigned int)selected*4;int offset=ov07_02234B5C(w->flags[selected],1);int height=(*(Data**)((u8*)w+scaled+8))->height;Pokepic_SetAttr(*(void**)((u8*)w+scaled+0x18),1,height+offset);}
