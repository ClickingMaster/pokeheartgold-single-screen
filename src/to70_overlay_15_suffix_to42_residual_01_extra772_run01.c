#pragma force_active on
#include "global.h"
typedef struct {u8 p[20];void *pixels;} Data;extern void *ov15_021FE990(int,Data**);extern void BlitBitmapRectToWindow(void*,void*,int,int,int,int,int,int,int,int);extern void Heap_FreeExplicit(int,void*);
void ov15_021FE9F0(int,void*,int,int);
void ov15_021FE9F0(int id,void*win,int y,int flag) {Data*d;void*raw=ov15_021FE990(id,&d);if(flag==0)BlitBitmapRectToWindow(win,d->pixels,24,0,104,16,0,(u16)y,40,16);else BlitBitmapRectToWindow(win,d->pixels,64,0,104,16,0,(u16)y,40,16);Heap_FreeExplicit(6,raw);}
