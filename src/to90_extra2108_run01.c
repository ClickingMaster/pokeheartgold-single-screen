#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
typedef struct {u8 pad[0x30];Window windows[5];} Panel;
typedef struct {u8 pad[0x34];int enabled;} Config;
typedef struct {u8 pad[0x54];Window window;u8 pad64[0x1c0];Panel *panel;u32 pad228;Config *config;u8 pad230[0x570];MsgData *messages;u8 pad7a4[8];String *string;} View;
void sub_0208D520(View *v);
void sub_0208D520(View *v){
    ScheduleWindowCopyToVram(&v->window);
    if(v->config->enabled){
        FillWindowPixelBuffer(&v->panel->windows[0],0);
        FillWindowPixelBuffer(&v->panel->windows[1],0);
        FillWindowPixelBuffer(&v->panel->windows[2],0);
        FillWindowPixelBuffer(&v->panel->windows[3],0);
        FillWindowPixelBuffer(&v->panel->windows[4],0);
        ReadMsgDataIntoString(v->messages,187,v->string);
        AddTextPrinterParameterizedWithColor(&v->panel->windows[0],0,v->string,4,0,255,0x10200,NULL);
        ReadMsgDataIntoString(v->messages,188,v->string);
        AddTextPrinterParameterizedWithColor(&v->panel->windows[1],0,v->string,4,0,255,0x10200,NULL);
        ReadMsgDataIntoString(v->messages,189,v->string);
        AddTextPrinterParameterizedWithColor(&v->panel->windows[2],0,v->string,4,0,255,0x10200,NULL);
        ReadMsgDataIntoString(v->messages,190,v->string);
        AddTextPrinterParameterizedWithColor(&v->panel->windows[3],0,v->string,4,0,255,0x10200,NULL);
        ReadMsgDataIntoString(v->messages,191,v->string);
        AddTextPrinterParameterizedWithColor(&v->panel->windows[4],0,v->string,4,0,255,0x10200,NULL);
        ScheduleWindowCopyToVram(&v->panel->windows[0]);
        ScheduleWindowCopyToVram(&v->panel->windows[1]);
        ScheduleWindowCopyToVram(&v->panel->windows[2]);
        ScheduleWindowCopyToVram(&v->panel->windows[3]);
        ScheduleWindowCopyToVram(&v->panel->windows[4]);
    }
}
