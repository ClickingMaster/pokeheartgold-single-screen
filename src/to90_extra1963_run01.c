#pragma force_active on
#include "global.h"
typedef struct{u16 time;u8 pad[18];u16 bg0,bg1;u8 row,pad19,old,scroll;int state;}Work;
extern void FillBgTilemapRect(void*,u8,u16,u8,u8,u8,u8,u8),ScheduleBgTilemapBufferTransfer(void*,u8);
void ov96_021FAB24(void*,Work*);
#pragma force_active on
#pragma opt_propagation off
void ov96_021FAB24(void*bg,Work*w){switch(w->state){
case 0:{u16 part,edge;if(w->time<112)return;part=2;edge=(part<<9)+part;
FillBgTilemapRect(bg,(u8)w->bg0,2,(u8)(10*w->row+2),50,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,3,(u8)(10*w->row+3),50,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,edge,(u8)(10*w->row+9),50,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,2,(u8)(10*w->row+2),50,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,3,(u8)(10*w->row+3),50,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,edge,(u8)(10*w->row+9),50,1,2,16);
ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg0);ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg1);w->state=1;w->old=255;}break;
case 1:{int phase=(int)w->time-16;int x;if(phase<3656)return;x=(64-((phase/8)%64))%64-2;if(x<0)x+=64;w->scroll=x;FillBgTilemapRect(bg,(u8)w->bg0,96,(u8)(10*w->row+2),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,97,(u8)(10*w->row+3),w->scroll,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,98,(u8)(10*w->row+9),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,96,(u8)(10*w->row+2),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,97,(u8)(10*w->row+3),w->scroll,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,98,(u8)(10*w->row+9),w->scroll,1,2,16);
ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg0);ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg1);w->state=2;}break;
case 2:{int phase=(int)w->time-16;int x;u16 edge;if(phase<4168)return;x=(64-((phase/8)%64))%64-2;if(x<0)x+=64;w->scroll=x;{u16 part=2;edge=(part<<9)+part;};FillBgTilemapRect(bg,(u8)w->bg0,2,(u8)(10*w->row+2),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,3,(u8)(10*w->row+3),w->scroll,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg0,edge,(u8)(10*w->row+9),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,2,(u8)(10*w->row+2),w->scroll,1,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,3,(u8)(10*w->row+3),w->scroll,6,2,16);
FillBgTilemapRect(bg,(u8)w->bg1,edge,(u8)(10*w->row+9),w->scroll,1,2,16);
ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg0);ScheduleBgTilemapBufferTransfer(bg,(u8)w->bg1);w->state=3;}break;
}}
