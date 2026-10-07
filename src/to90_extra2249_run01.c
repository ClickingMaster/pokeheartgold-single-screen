#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[4];} Window;typedef struct {u8 pad[12];Window windows[101];void *messages;u8 pad2[0x120c];int index;} Work;
extern void FillWindowPixelBuffer(Window *,int);extern int GetWindowWidth(Window *);extern void ov18_021F9648(Window *,void *,int,int,int,int,u32,int);
void ov18_021EFC3C(Work *,int);
void ov18_021EFC3C(Work *w,int index){int message,width;FillWindowPixelBuffer(&w->windows[index],0);message=w->index;if(message==26)message=113;else message+=69;width=GetWindowWidth(&w->windows[index]);ov18_021F9648(&w->windows[index],w->messages,message,(width*8)/2,0,4,0x20100,2);}
