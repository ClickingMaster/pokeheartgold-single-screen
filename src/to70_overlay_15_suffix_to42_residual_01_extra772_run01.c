#pragma force_active on
#include "global.h"
typedef struct {u8 p[20];void *pixels;} Data;extern void *BagApp_LoadListIconChars(int,Data**);extern void BlitBitmapRectToWindow(void*,void*,int,int,int,int,int,int,int,int);extern void Heap_FreeExplicit(int,void*);
void BagApp_DrawRegisteredItemIcon(int,void*,int,int);
void BagApp_DrawRegisteredItemIcon(int id,void*win,int y,int flag) {Data*d;void*raw=BagApp_LoadListIconChars(id,&d);if(flag==0)BlitBitmapRectToWindow(win,d->pixels,24,0,104,16,0,(u16)y,40,16);else BlitBitmapRectToWindow(win,d->pixels,64,0,104,16,0,(u16)y,40,16);Heap_FreeExplicit(6,raw);}
