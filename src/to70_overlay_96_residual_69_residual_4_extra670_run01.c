#pragma force_active on
#include "global.h"
typedef struct {int a,b,c;} Block;typedef struct {void *font,*window,*list,*pal,*sprite;u32 offset;int x,y,z,a,vram,heap;} Template;
typedef struct {int heap;u8 p[12];void *font;u8 q[0x168];void *list;u8 r[0x144];void *pal;} W;
typedef struct {void *text;Block block;} Out;
extern void FillWindowPixelBufferText_AssumeTileSize32(void*,int);extern void AddTextPrinterParameterizedWithColorAndSpacing(void*,int,void*,int,int,int,int,int,int,int);extern void sub_02021AC8(int,int,int,Block*);extern void *SpriteTransfer_GetPaletteProxy(void*,int);extern void *FontOAM_Create(Template*);extern void TextOBJ_SetPaletteNum(void*,int);
void ov96_021F8DF4(W*,void*,void*,int,int,int,void*,Out*);
void ov96_021F8DF4(W*w,void*window,void*str,int x,int a,int b,void*s,Out*out) {Template t;Block block;void*text;FillWindowPixelBufferText_AssumeTileSize32(window,0);AddTextPrinterParameterizedWithColorAndSpacing(window,0,str,x,0,255,0x10200,0,0,0);sub_02021AC8(*(int*)((u8*)window+20),1,1,&block);t.font=w->font;t.window=window;t.list=w->list;t.pal=SpriteTransfer_GetPaletteProxy(w->pal,0);t.sprite=s;t.offset=block.b;t.x=a;t.y=b;t.z=0;t.a=3;t.vram=1;t.heap=w->heap;text=FontOAM_Create(&t);TextOBJ_SetPaletteNum(text,0);out->text=text;out->block=block;}
