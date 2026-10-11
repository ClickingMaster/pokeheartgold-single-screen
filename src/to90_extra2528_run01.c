#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
#include "message_format.h"
#include "msgdata.h"
#pragma force_active on
typedef struct{u16 item;u16 pad;}Item;typedef struct{Item entries[36];}Row;typedef struct{u8 p[0x10];MsgData*msg;MessageFormat*fmt;String*output;u8 gap[16];Window*window;u8 gap2[12];Row rows[5];u8 gap3[0x114d-0x3c-5*144];u8 row;}Work;
void BattleBagMenu_PrintUseItemName(Work*,int);
void BattleBagMenu_PrintUseItemName(Work*w,int index){Window*window=w->window;String*str=NewString_ReadMsgData(w->msg,9);Row*row=&w->rows[w->row];BufferItemName(w->fmt,0,row->entries[index].item);StringExpandPlaceholders(w->fmt,w->output,str);FillWindowPixelBuffer(window,0);AddTextPrinterParameterizedWithColor(window,0,w->output,0,0,255,0x10200,NULL);String_Delete(str);ScheduleWindowCopyToVram(window);}
