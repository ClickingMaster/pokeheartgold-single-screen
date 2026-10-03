#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
#include "yes_no_prompt.h"
typedef struct {u32 pad;BgConfig*bg;Window windows[15];u8 p[32];YesNoPrompt*prompt;} Work;
extern const WindowTemplate ov68_021E7DFC[];
extern void FontID_Alloc(int,int);
void ov68_021E5E48(Work*);
void ov68_021E5E48(Work*w){u32 i;FontID_Alloc(4,66);for(i=0;i<15;i++){AddWindow(w->bg,&w->windows[i],&ov68_021E7DFC[i]);FillWindowPixelBuffer(&w->windows[i],0);}ClearWindowTilemapAndScheduleTransfer(&w->windows[8]);w->prompt=YesNoPrompt_Create((enum HeapID)66);}
