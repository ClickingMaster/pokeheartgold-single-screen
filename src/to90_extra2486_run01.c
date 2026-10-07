#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
#include "msgdata.h"
typedef struct{u8 p[0x24];BgConfig *bg;u8 p28[32];MsgData *msg;u8 p4c[0x814];void *volatile ui;} Work;
typedef struct{u8 p[0x234];Window windows[2];} UI;
typedef struct{u8 x,y,width,height;} Rect;
typedef struct{u32 p[5];u32 messages[2];} LabelData;
typedef struct{u32 messages[2];} Messages;
extern LabelData ov40_02245268;extern Rect ov40_02245274[];
extern int ov40_022306C0(Window *,String *);

void ov40_02237AC0(Work *);
#pragma force_active on
void ov40_02237AC0(Work *w){Messages messages;Window *window;Rect *r;UI *ui=w->ui;u32 tile;int i;tile=1;messages=*(Messages*)ov40_02245268.messages;r=ov40_02245274;i=0;window=ui->windows;for(;i<2;i++){String *str;InitWindow(window);AddWindowParameterized(w->bg,window,6,r->x,r->y,r->width,r->height,14,(u16)tile);FillWindowPixelBuffer(window,0);str=NewString_ReadMsgData(w->msg,messages.messages[i]);AddTextPrinterParameterizedWithColor(window,0,str,ov40_022306C0(window,str),0,255,0xf0d00,0);ScheduleWindowCopyToVram(window);tile+=r->width*r->height;String_Delete(str);window++;r++;}}
