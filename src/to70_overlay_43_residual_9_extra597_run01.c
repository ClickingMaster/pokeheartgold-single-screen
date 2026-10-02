#pragma force_active on
#include "global.h"
#include "yes_no_prompt.h"
extern void Sprite_SetPriority(void *,int);
extern void Sprite_SetDrawPriority(void *,int);
extern void StartBrightnessTransition(int,int,int,int,int);
typedef struct {u8 p[0x1fc];void *sprite;} Work;
typedef struct {BgConfig *bg;u8 pad[0x5c];YesNoPrompt *prompt;} Config;
void ov43_0222AD98(Work *w,int a,int b);
void ov43_0222ADB8(int *w,Config *c,int cursor);
void ov43_0222AD98(Work *w,int a,int b){Sprite_SetPriority(w->sprite,a);Sprite_SetDrawPriority(w->sprite,b);}
void ov43_0222ADB8(int *w,Config *c,int cursor){YesNoPromptTemplate t;MI_CpuFill8(&t,0,sizeof(t));t.bgConfig=c->bg;t.bgId=1;t.tileStart=10;t.plttSlot=13;t.x=25;t.y=9;t.ignoreTouchFlag=(u8)*w;t.initialCursorPos=(u8)cursor;t.shapeParam=0;YesNoPrompt_InitFromTemplate(c->prompt,&t);StartBrightnessTransition(4,8,0,61,1);}
