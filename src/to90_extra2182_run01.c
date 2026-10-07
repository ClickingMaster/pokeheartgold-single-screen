#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 values[4];} Window;
typedef struct {u8 values[8];} Template;
typedef struct {void *save;u8 pad4[0x3c];void *bg;u8 pad44[0x24];Window windows[4];void *prompt;} Work;
extern const Template ov113_021E6C90[];
extern void AddWindow(void*,Window*,const Template*);
extern void FillWindowPixelBuffer(Window*,int);
extern void *YesNoPrompt_Create(void*);
void ov113_021E671C(Work*);
void ov113_021E671C(Work *work) {const Template *template=ov113_021E6C90;Window *window=(Window*)work;int i;i=0;window=(Window*)((u8*)window+0x68);for(;i<4;i++,template++,window++){AddWindow(work->bg,window,template);FillWindowPixelBuffer(window,0);}work->prompt=YesNoPrompt_Create(work->save);}
