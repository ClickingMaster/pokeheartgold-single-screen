#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 width,height;u8 p[8];u8 data[1];} Image;typedef struct {u8 p[0x10];void *bg;u8 p2[0x4c];Image *img;} Work;
extern void CopyToBgTilemapRect(void *,int,u8,u8,u8,u8,void *,u8,u8,u8,u8);void ov102_021EB570(Work *,int,int);
void ov102_021EB570(Work *w,int a,int b){Image *img=w->img;CopyToBgTilemapRect(w->bg,3,(u8)(a*17+2),0,11,6,img->data,0,(u8)(b*6),(u8)(img->width>>3),(u8)(img->height>>3));}
