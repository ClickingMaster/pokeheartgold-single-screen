#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
#include "string_util.h"
typedef struct {u8 pad[0x2c];u8 values[4];} Parent;
typedef struct {Parent *parent;u8 pad4[12];MsgData *messages;MessageFormat *formatter;String *string;u8 pad1c[16];Window *windows;u8 pad30[0x111d];u8 selected;u8 pad114e[6];u8 values[4];} View;
u32 FontID_String_GetWidth(int,const String*,int);
void ov08_022237C4(View*);
void ov08_022237C4(View *v){
 Window *windows;String *first,*second,*third;u32 width,center;
 FillWindowPixelBuffer(&v->windows[25],0);
 windows=v->windows;first=NewString_ReadMsgData(v->messages,28);
 width=FontID_String_GetWidth(0,first,0);center=(GetWindowWidth((Window*)((u32)windows+400))*8-width)/2;
 AddTextPrinterParameterizedWithColor((Window*)((u32)windows+400),0,first,center,4,255,0x10200,NULL);String_Delete(first);
 second=NewString_ReadMsgData(v->messages,29);BufferIntegerAsString(v->formatter,0,v->values[v->selected]+1,2,PRINTING_MODE_LEFT_ALIGN,1);StringExpandPlaceholders(v->formatter,v->string,second);
 AddTextPrinterParameterizedWithColor((Window*)((u32)windows+400),0,v->string,center+width,4,255,0x10200,NULL);String_Delete(second);
 third=NewString_ReadMsgData(v->messages,30);BufferIntegerAsString(v->formatter,0,v->parent->values[v->selected]+1,2,PRINTING_MODE_LEFT_ALIGN,1);StringExpandPlaceholders(v->formatter,v->string,third);
 AddTextPrinterParameterizedWithColor((Window*)((u32)windows+400),0,v->string,center-FontID_String_GetWidth(0,v->string,0),4,255,0x10200,NULL);String_Delete(third);ScheduleWindowCopyToVram((Window*)((u32)windows+400));
}
