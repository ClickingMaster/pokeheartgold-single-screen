#pragma force_active on
#include "global.h"
typedef struct {u8 pad[4];void *bg;} Parent;
typedef struct {u8 state,id;volatile u8 timer;s8 reset;u32 pad;Parent *parent;u8 delay,toggle,mode,flag;} Work;
extern u8 ov96_0221D1F4[];
extern void ov96_02214044(Parent *,int,int),BgTilemapRectChangePalette(void *,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void *,int);
void ov96_02214278(void *,Work *);
#pragma force_active on
void ov96_02214278(void *unused,Work *w) {
 switch(w->state) {
 case 0:break;
 case 1:
  if(w->mode) {if(w->flag)ov96_02214044(w->parent,w->id,1);else ov96_02214044(w->parent,w->id,2);}else ov96_02214044(w->parent,w->id,3);
  w->timer=0;w->toggle=0;w->delay=0;w->state=2;break;
 case 2:
  w->timer++;if(w->timer>30){BgTilemapRectChangePalette(w->parent->bg,1,(w->id&31)*8,0,8,2,1);ScheduleBgTilemapBufferTransfer(w->parent->bg,1);ov96_02214044(w->parent,w->id,0);w->reset=-1;w->state=0;break;}
  if(w->delay==0){struct{u8 first[2],second[2];}palette;u8 *second=palette.second;palette.second[0]=ov96_0221D1F4[2];palette.second[1]=ov96_0221D1F4[3];u8 x=ov96_0221D1F4[0];u8*first=palette.first;palette.first[0]=x;palette.first[1]=ov96_0221D1F4[1];int pal;
   w->toggle=(w->toggle+1)%2;w->delay=5;if(w->mode)pal=second[w->toggle];else pal=first[w->toggle];
   BgTilemapRectChangePalette(w->parent->bg,1,(w->id&31)*8,0,8,2,pal);ScheduleBgTilemapBufferTransfer(w->parent->bg,1);
  }else w->delay--;
  break;
 }
}
