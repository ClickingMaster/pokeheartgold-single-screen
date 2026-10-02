#pragma force_active on
#include "global.h"
#include "yes_no_prompt.h"
#pragma force_active on
YesNoPrompt *ov75_02247890(BgConfig *,int,int);
YesNoPrompt *ov75_02247890(BgConfig *bg,int tile,int cursor){
 YesNoPrompt *prompt=YesNoPrompt_Create((enum HeapID)116);
 YesNoPromptTemplate t;
 t.bgConfig=bg;t.bgId=0;t.tileStart=tile;t.plttSlot=14;t.x=25;t.y=10;
 t.ignoreTouchFlag=0;t.initialCursorPos=(u8)cursor;t.shapeParam=0;
 YesNoPrompt_InitFromTemplate(prompt,&t);return prompt;
}
