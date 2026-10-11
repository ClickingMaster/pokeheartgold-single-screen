#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[4];} Window;typedef struct {u8 pad[12];Window windows[101];void *messages;} Work;
extern void FillWindowPixelBuffer(Window *,int),ScheduleWindowCopyToVram(Window *);extern int GetWindowWidth(Window *),PokedexApp_GetFormNameMsg(Work *,int);extern void PokedexApp_PrintFormattedMessage(Work *,void *,int,int,int,int,int,u32,int);
void PokedexApp_DrawCenteredFormName(Work *,int,int);
void PokedexApp_DrawCenteredFormName(Work *w,int index,int value){int message,width;FillWindowPixelBuffer(&w->windows[index],0);message=PokedexApp_GetFormNameMsg(w,value);width=GetWindowWidth(&w->windows[index]);PokedexApp_PrintFormattedMessage(w,w->messages,index,message,(width*8)/2,0,0,0x50900,2);ScheduleWindowCopyToVram(&w->windows[index]);}
