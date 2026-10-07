#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p0[8];void*bg;}Context;
typedef struct{u8 p0[8];u8 height;u8 p9[7];}Window;
typedef struct{u8 p0[12];void*str;u8 p10[0x24-0x10];Context*ctx;u32 p28;void*msg;u8 p30[0x48-0x30];u8*mons;u8 p4c[8];Window window;u8 p64[0x78-0x64];void*cursor;Window*active;u8 mode,index;}Work;
extern int WindowIsInUse(Window*);extern void*NewMsgDataFromNarc(int,int,int,int);extern u32 sub_02070D90(void),GetMonData(void*,int,void*);extern void AddWindowParameterized(void*,Window*,int,int,int,int,int,int,int),LoadUserFrameGfx1(void*,int,int,int,int,int),FillWindowPixelBuffer(Window*,int),ReadMsgDataIntoString(void*,int,void*),AddTextPrinterParameterized(Window*,int,void*,u32,u32,u32,void*),DestroyMsgData(void*),FillWindowPixelRect(Window*,int,int,int,u16,u16),ListMenuUpdateCursorObj(void*,Window*,int,int),DrawFrameAndWindow1(Window*,int,int,int);
void sub_02059820(Work*,int);
void sub_02059820(Work*w,int index){Window*window=&w->window;u32 monSize;void*msg;int i,offset,y;if(!WindowIsInUse(window)){msg=NewMsgDataFromNarc(1,27,237,4);monSize=sub_02070D90();AddWindowParameterized(w->ctx->bg,window,3,21,9,10,8,13,10);LoadUserFrameGfx1(w->ctx->bg,3,1,11,0,4);FillWindowPixelBuffer(window,15);i=0;offset=i;y=i;for(;i<3;i++){ReadMsgDataIntoString(msg,GetMonData(w->mons+offset,5,0),w->str);AddTextPrinterParameterized(window,0,w->str,16,y,255,0);offset+=monSize;y+=16;}ReadMsgDataIntoString(w->msg,21,w->str);AddTextPrinterParameterized(window,0,w->str,16,i<<4,255,0);DestroyMsgData(msg);}FillWindowPixelRect(window,15,0,0,16,window->height*8);ListMenuUpdateCursorObj(w->cursor,&w->window,0,index<<4);DrawFrameAndWindow1(&w->window,0,1,11);w->index=index;w->mode=4;w->active=window;}
