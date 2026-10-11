#pragma force_active on
#include "global.h"
#include "yes_no_prompt.h"
typedef struct {BgConfig*bg;u8 p[0x804-4];YesNoPrompt*prompt;} Work;
void BagApp_CreateYesNoPrompt(Work*);
void BagApp_CreateYesNoPrompt(Work*w){YesNoPromptTemplate t;t.bgConfig=w->bg;t.bgId=5;t.tileStart=129;t.plttSlot=9;t.x=25;t.y=6;t.ignoreTouchFlag=0;t.initialCursorPos=0;t.initialCursorPos=0;t.shapeParam=0;w->prompt=YesNoPrompt_Create((enum HeapID)6);YesNoPrompt_InitFromTemplate(w->prompt,&t);}
