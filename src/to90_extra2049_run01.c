#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[4];void *save;u8 pad2;u8 mode,result;u8 pad3[0x15];void *string;u8 pad4[0x596];u16 points;} Work;
extern void ov83_02244DF4(Work *,u16 *,u16 *,u16 *,u16 *),FillWindowPixelRect(void *,int,u16,u16,int,int),ov83_02244A98(Work *,int,int,int,int),ov83_02245D08(Work *,void *,void *,int,int,int,int,u32,int),ScheduleWindowCopyToVram(void *);
extern int ov80_02237D8C(int),sub_0205C1F0(int),sub_0205C268(int),FrontierSave_GetStat(void *,int,int),Comm_GetLocalNetId(void),ov83_0224484C(Work *,void *,int,u16,u16,int,int,int,int,int);
void ov83_022453DC(void *, void *);
void ov83_022453DC(void *parent,void *window){Work *w=parent;
u16 x,y,a,b;int first,second,stat;
ov83_02244DF4(w,&x,&y,&a,&b);
if(ov80_02237D8C(w->mode)==0){u16 xx=x+72,yy=y+1;FillWindowPixelRect(window,0,xx,yy,48,16);stat=sub_0205C1F0(w->mode);first=FrontierSave_GetStat(w->save,stat,sub_0205C268(sub_0205C1F0(w->mode)));ov83_02244A98(w,0,first,4,0);w->result=ov83_0224484C(w,window,2,xx,yy,255,1,2,0,0);}
else{FillWindowPixelRect(window,0,64,0,48,16);FillWindowPixelRect(window,0,192,0,48,16);
if(Comm_GetLocalNetId()==0){stat=sub_0205C1F0(w->mode);first=FrontierSave_GetStat(w->save,stat,sub_0205C268(sub_0205C1F0(w->mode)));second=w->points;}
else{first=w->points;stat=sub_0205C1F0(w->mode);second=FrontierSave_GetStat(w->save,stat,sub_0205C268(sub_0205C1F0(w->mode)));}
ov83_02244A98(w,0,first,4,0);ov83_02245D08(w,window,w->string,2,112,0,0,0x10200,1);
ov83_02244A98(w,0,second,4,0);ov83_02245D08(w,window,w->string,3,240,0,0,0x10200,1);}
ScheduleWindowCopyToVram(window);
}
