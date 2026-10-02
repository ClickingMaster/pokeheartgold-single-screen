#pragma force_active on
#include "global.h"
#include "yes_no_prompt.h"
#include "render_window.h"
#include "font.h"
#include "text.h"
#include "msgdata.h"
#include "string_util.h"
#pragma force_active on
typedef struct W {u8 p[0x20];MessageFormat *format;u8 p24[0x10];String *text;u8 p38[8];u32 printer;Window win;u8 p54[0x38];u32 state;} W;
YesNoPrompt *ov39_02228AC0(BgConfig *,int);void ov39_02228B04(W*,MsgData*,int,int);
YesNoPrompt *ov39_02228AC0(BgConfig *bg,int tile){YesNoPromptTemplate t;YesNoPrompt *p=YesNoPrompt_Create((enum HeapID)124);t.bgConfig=bg;t.bgId=0;t.tileStart=tile;t.plttSlot=14;t.x=25;t.y=10;t.ignoreTouchFlag=0;t.initialCursorPos=0;t.shapeParam=0;YesNoPrompt_InitFromTemplate(p,&t);return p;}
void ov39_02228B04(W*w,MsgData*data,int msg,int speed){String*s=NewString_ReadMsgData(data,msg);StringExpandPlaceholders(w->format,w->text,s);String_Delete(s);FillWindowPixelBuffer(&w->win,15);DrawFrameAndWindow2(&w->win,0,1,10);w->printer=AddTextPrinterParameterized(&w->win,1,w->text,0,0,speed,0);w->state=0;if(speed==255||speed==0)w->printer=255;}
