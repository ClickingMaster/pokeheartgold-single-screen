#pragma force_active on
#include "global.h"
extern void FillBgTilemapRect(void *,int,int,u8,u8,int,int,int);
extern void ScheduleBgTilemapBufferTransfer(void *,int);
extern void ReadMsgDataIntoString(void *,int,void *);
extern void StringExpandPlaceholders(void *,void *,void *);
extern u32 FontID_String_GetWidth(int,void *,int);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
typedef struct {void *bg;int pad[2];void *windows[4];} Work;
typedef struct {u8 pad[0x50];void *fmt;void *msg;} Text;
void ov43_0222EC14(Work *,s16 *);
void ov43_0222EC58(Work *,int,int,int,Text *,int,int,int,u32,void *,void *,int);
void ov43_0222EC14(Work *w,s16 *p){int y=p[1],x;if(y<0)y=-y;x=p[0];if(x<0)x=-x;FillBgTilemapRect(w->bg,3,0,(u8)x,(u8)y,32,24,0);ScheduleBgTilemapBufferTransfer(w->bg,3);}
void ov43_0222EC58(Work *w,int a,int b,int unused,Text *t,int id,int x,int y,u32 color,void *out,void *tmp,int align){ReadMsgDataIntoString(t->msg,id,tmp);StringExpandPlaceholders(t->fmt,out,tmp);switch(align){case 1:x-=(FontID_String_GetWidth(1,out,0)+1)/2;break;case 2:x-=FontID_String_GetWidth(1,out,0);break;}AddTextPrinterParameterizedWithColor((u8*)w->windows[a]+b*16,1,out,x,y,255,color,0);}
