#pragma force_active on
#include "global.h"
typedef struct {u16 species,form;u8 pad[3];u8 gender;u32 unused,pid;} Mon;
extern void ov96_021EB408(void*,int,int,int,int),ov96_021EB588(void*,void*),ov96_021EB564(void*,int),ov96_021EB52C(void*,int,int),ov96_021EB630(void*,int),ov96_02203DCC(void*,Mon*),ov96_02203F50(void*,void*),ov96_02203F0C(void*);
extern void *ov96_021EB4F4(void*,int,int);
extern u8 GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
void ov96_02203E30(void*,u8*,Mon*);
void ov96_02203E30(void *ctx,u8 *w,Mon *mon){struct {int x,y,z;} pos;int i;volatile int x;u8 *cur;Mon *m;
for(i=0;i<3;i++)ov96_021EB408(ctx,3,2,101,2);
m=mon;cur=w;i=0;x=16;for(;i<3;i++){*(void**)(cur+84)=ov96_021EB4F4(ctx,101,2);pos.z=0;pos.x=x<<12;pos.y=(GetMonPicHeightBySpeciesGenderForm(m->species,m->gender,0,m->form,m->pid)+376)<<12;ov96_021EB588(*(void**)(cur+84),&pos);ov96_021EB564(*(void**)(cur+84),i+1);ov96_021EB52C(*(void**)(cur+84),1,1);ov96_021EB630(*(void**)(cur+84),19-i);*(void**)(cur+96)=ov96_021EB4F4(ctx,101,5);ov96_021EB564(*(void**)(cur+96),0);pos.y-=65536;ov96_021EB588(*(void**)(cur+96),&pos);ov96_021EB630(*(void**)(cur+96),4);{int next=x+50;m++;cur+=4;x=next;}}
ov96_02203DCC(w,mon);ov96_02203F50(ctx,w);ov96_02203F0C(w);}
