#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[9];u8 kind;u8 pad2[0x2aa];void *resource;u8 pad3[0x2a4];void *party;} State;
extern void FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *);
extern int ov80_02237D8C(u8),ov80_02237B58(u8,int);
extern void *Party_GetMonByIndex(void *,int);
extern u32 GetMonData(void *,int,void *);
extern void sub_0200CE7C(void *,int,u32,int,int,void *,int,int),ov83_02244BA8(State *,void *,int,int,int,u8);
void ov83_022449D4(State *,void *);
void ov83_022449D4(State *s,void *window){
 int x,y,i,count;void *mon;u32 value;
 FillWindowPixelBuffer(window,0);
 if(!ov80_02237D8C(s->kind)){x=40;y=80;}else{x=8;y=48;}
 count=ov80_02237B58(s->kind,1);
 for(i=0;i<count;i++,x+=64,y+=64){
  mon=Party_GetMonByIndex(s->party,i);value=GetMonData(mon,161,0);
  sub_0200CE7C(s->resource,1,value,3,0,window,x,1);
  value=GetMonData(mon,111,0);ov83_02244BA8(s,window,y,1,0,(u8)value);
 }
 ScheduleWindowCopyToVram(window);
}
