#pragma force_active on
#include "global.h"
typedef struct {void *bg;u8 p[0x58];void *menu,*prompt;u8 win[16];void *str[4];} W;
extern void LoadFontPal1(int,int,int);extern void LoadUserFrameGfx2(void*,int,int,int,int,int);extern void InitWindow(void*);extern void AddWindowParameterized(void*,void*,int,int,int,int,int,int,int);extern void *String_New(int,int);extern void *TouchscreenListMenuSpawner_Create(int,int);extern void *YesNoPrompt_Create(int);
void ov43_0222A8C0(W*,u8*,int);
void ov43_0222A8C0(W*w,u8*opt,int heap) {LoadFontPal1(0,352,heap);LoadFontPal1(4,448,heap);LoadUserFrameGfx2(w->bg,1,70,12,opt[13],heap);InitWindow(w->win);AddWindowParameterized(w->bg,w->win,1,1,0,24,3,11,100);w->str[0]=String_New(128,heap);w->str[1]=String_New(128,heap);w->str[2]=String_New(128,heap);w->str[3]=String_New(128,heap);w->menu=TouchscreenListMenuSpawner_Create(heap,0);w->prompt=YesNoPrompt_Create(heap);}
