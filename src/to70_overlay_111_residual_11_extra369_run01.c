#pragma force_active on
#include "global.h"
#pragma force_active on
u8 AddTextPrinterParameterizedWithColor(void *,int,void *,u32,u32,u32,u32,void *);void FillWindowPixelBuffer(void *,int),String_Delete(void *),ov111_021E6B30(void *,void *,void *,void *,int,int,u8,u8,u32,int);
void *ReadMsgData_ExpandPlaceholders(void *,void *,int,int);
void AddWindowParameterized(void *,void *,u8,int,int,int,int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),DrawFrameAndWindow2(void *,int,int,int);int GetWindowBgId(void *),Options_GetFrame(void *),Options_GetTextFrameDelay(void *);u8 ov111_021E6A74(void *,void **,void **,void *,void *,int,u8,int,void *);
u8 ov111_021E6A74(void *bg,void **win,void **out,void *fmt,void *msg,int id,u8 bgid,int heap,void *options){int delay;u8 printer;if(bg==NULL)GF_AssertFail();if(options==NULL)GF_AssertFail();if(*win==NULL){int index;AddWindowParameterized(bg,win,bgid,2,19,27,4,15,1);index=GetWindowBgId(win);LoadUserFrameGfx2(bg,index,978,13,(u8)Options_GetFrame(options),heap);}delay=Options_GetTextFrameDelay(options);FillWindowPixelBuffer(win,15);*out=ReadMsgData_ExpandPlaceholders(fmt,msg,id,heap);printer=AddTextPrinterParameterizedWithColor(win,1,*out,0,0,delay,0x1020f,NULL);DrawFrameAndWindow2(win,0,978,13);return printer;}
void ov111_021E6B30(void *unused,void *win,void *fmt,void *msg,int id,int unused2,u8 x,u8 y,u32 color,int heap){void*s;FillWindowPixelBuffer(win,0);s=ReadMsgData_ExpandPlaceholders(fmt,msg,id,heap);AddTextPrinterParameterizedWithColor(win,0,s,x,y,0,color,NULL);String_Delete(s);}
