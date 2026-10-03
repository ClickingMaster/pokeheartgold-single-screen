#pragma force_active on
#include "global.h"
#pragma force_active on
#include "yes_no_prompt.h"
typedef struct{YesNoPrompt*prompt;int state;BgConfig*bg;} Work;
extern void ov83_02247988(u16*,u16*);
void ov83_02247864(Work*,BgConfig*);
void ov83_02247864(Work*w,BgConfig*bg){u16 a,b;YesNoPromptTemplate t;w->bg=bg;ov83_02247988(&a,&b);t.bgConfig=bg;t.bgId=0;t.tileStart=a;t.plttSlot=11;t.x=25;t.y=10;t.ignoreTouchFlag=0;t.initialCursorPos=0;t.shapeParam=0;YesNoPrompt_InitFromTemplate(w->prompt,&t);w->state=1;}
