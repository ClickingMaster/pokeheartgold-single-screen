#pragma force_active on
#include "global.h"
extern void Heap_Free(void*);
typedef struct {void*bg;}B;
extern void ov49_0225B438(void*),ov49_0225BA40(void*,B*,int,int),BgFillTilemapBufferAndSchedule(void*,int,int),ov49_0225C3C0(int*),ov49_0225BB14(B*,void*,int,int,int,int),ov45_0222A844(void*,void*,int),ov49_0225C3DC(int*,void*,int,int,int,int,int),ov49_0225B3A8(void*,u32,int,int,int),ov49_0225C414(int*,void*,int,int,int,int,int),ov49_0225C470(int*,void*,void*),ov49_0225C460(int*,int),ov49_0225C480(int*,void*,void*),ov49_0225C4B0(int*,void*,int),ov49_0225C4CC(int*,B*,void*,int,void*,void*),ov49_0225BFF0(int*,B*,void*,int,int,int),ov49_0225C180(int*,B*,void*,int,int),ov49_0225BEA0(void*,int*,B*,int,void*);
extern void *PlayerProfile_New(int);extern u32 PlayerProfile_GetTrainerID_VisibleHalf(void*);extern int ov45_0222A9CC(void*),ov45_0222AA84(void*),ov45_0222AAC8(void*),ov45_0222AA5C(void*);
void ov49_0225BBD0(void*,void*,int*,B*,void*,int,void*,int,void*,void*,int,int,int,int);
void ov49_0225BBD0(void*owner,void*unused,int*ui,B*bg,void*fmt,int flag,void*narc,int heap,void*net,void*other,int mode,int enabled,int value,int last){int color,n;void*prof;
ov49_0225B438(fmt);if(!ov45_0222A9CC(net)){color=0x70800;if(!mode)ov49_0225BA40(owner,bg,86,heap);else ov49_0225BA40(owner,bg,88,heap);}else{color=0x30400;if(!mode)ov49_0225BA40(owner,bg,87,heap);else ov49_0225BA40(owner,bg,88,heap);}
BgFillTilemapBufferAndSchedule(bg->bg,4,0);BgFillTilemapBufferAndSchedule(bg->bg,5,0);BgFillTilemapBufferAndSchedule(bg->bg,6,0);ov49_0225C3C0(ui);ov49_0225BB14(bg,narc,92,4,0,heap);prof=PlayerProfile_New(heap);ov45_0222A844(net,prof,heap);
ov49_0225C3DC(ui,fmt,0,44,0,0,color);ov49_0225B3A8(fmt,PlayerProfile_GetTrainerID_VisibleHalf(prof),5,0,2);ov49_0225C414(ui,fmt,0,49,122,0,0x10200);ov49_0225C3DC(ui,fmt,0,45,0,16,color);ov49_0225C470(ui,fmt,prof);ov49_0225C414(ui,fmt,0,50,122,16,0x10200);ov49_0225C460(ui,0);ov49_0225C3DC(ui,fmt,1,46,0,0,color);
if(!ov45_0222AA84(net)){ov49_0225C3DC(ui,fmt,1,55,32,16,0x10200);}else{ov49_0225C480(ui,fmt,net);ov49_0225C3DC(ui,fmt,1,51,32,16,0x10200);}ov49_0225C460(ui,1);
if(enabled){*ui=1;ov49_0225C470(ui,fmt,prof);ov49_0225C3DC(ui,fmt,4,56,8,0,color);ov49_0225C4B0(ui,fmt,value);ov49_0225C3DC(ui,fmt,4,59,6,24,0x10200);ov49_0225C3DC(ui,fmt,4,60,90,24,0x10200);ov49_0225C3DC(ui,fmt,4,61,6,44,0x10200);ov49_0225C3DC(ui,fmt,4,62,90,44,0x10200);ov49_0225C460(ui,4);}else{*ui=0;ov49_0225C3DC(ui,fmt,4,48,8,0,color);ov49_0225C460(ui,4);ov49_0225C4CC(ui,bg,narc,heap,net,other);}
ov49_0225BFF0(ui,bg,narc,heap,ov45_0222AAC8(net),last);if(flag==1){if(ov45_0222A9CC(net)==1)n=97;else n=0;}else n=ov45_0222AA5C(net);ov49_0225C180(ui,bg,narc,heap,n);ov49_0225BEA0(owner,ui,bg,heap,net);Heap_Free(prof);}
