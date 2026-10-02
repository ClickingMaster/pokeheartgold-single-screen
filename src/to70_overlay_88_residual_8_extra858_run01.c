#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
#include "string_util.h"
#include "font.h"
typedef struct {u8 active,kind;u16 position,speed,width;String *text;Window window;u8 colors[4];} Work;
extern void ov88_022592B8(void *,void *,int);
void ov88_02259560(Work *,String *,u16,u8 *,void *);
void ov88_02259560(Work *w,String *s,u16 speed,u8 *colors,void *data){String_Copy(w->text,s);w->active=1;w->position=0;w->speed=speed;w->width=FontID_String_GetWidth(1,s,0);w->width+=256;GF_ASSERT(w->width<=1440);FillWindowPixelBuffer(&w->window,0);AddTextPrinterParameterized(&w->window,1,w->text,0,0,255,NULL);w->colors[0]=colors[0];w->colors[1]=colors[1];w->colors[2]=colors[2];w->colors[3]=colors[3];ov88_022592B8(w->colors,data,w->kind);}
