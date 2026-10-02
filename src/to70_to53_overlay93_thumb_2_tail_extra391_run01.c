#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 fields[12];} Template;
typedef struct {u8 bytes[16];} Window;
typedef struct {void *sprite;VecFx32 dimensions;u16 width;} Result;
extern int FontID_String_GetWidthMultiline(int,void *,int);
extern void InitWindow(Window *),AddTextWindowTopLeftCorner(void *,Window *,u8,u8,int,int),AddTextPrinterParameterizedWithColorAndSpacing(Window *,int,void *,int,int,int,int,int,void *,int),sub_02021AC8(void *,int,int,VecFx32 *),sub_020138B0(void *),sub_020136B4(void *,int,int),RemoveWindow(Window *);
extern void *sub_02013688(Window *,int,int),*SpriteManager_GetSpriteList(void *),*SpriteManager_FindPlttResourceProxy(void *,int),*sub_020135D8(Template *);
void ov93_02261EB8(void *,void *,void *,Result *,void *,int,int,int,int,int,int,int,int,int,int);
void ov93_02261EB8(void *bg,void *mgr,void *fontMgr,Result *out,void *str,int font,int color,int hidden,int palette,int x,int y,int center,int priority,int vram,int height){Template t;Window w;VecFx32 dims;void *sprite;int width=FontID_String_GetWidthMultiline(font,str,0);int tiles=width/8;if(FX_ModS32(width,8)!=0)tiles++;InitWindow(&w);AddTextWindowTopLeftCorner(bg,&w,tiles,height,0,0);AddTextPrinterParameterizedWithColorAndSpacing(&w,font,str,0,0,255,color,0,0,0);sub_02021AC8(sub_02013688(&w,1,117),1,1,&dims);if(center==1)x-=width/2;t.fields[0]=(u32)fontMgr;t.fields[1]=(u32)&w;t.fields[2]=(u32)SpriteManager_GetSpriteList(mgr);t.fields[3]=(u32)SpriteManager_FindPlttResourceProxy(mgr,palette);t.fields[4]=0;t.fields[5]=dims.y;t.fields[6]=x;t.fields[7]=y;t.fields[8]=priority;t.fields[9]=vram;t.fields[10]=1;t.fields[11]=117;sprite=sub_020135D8(&t);if(hidden)sub_020138B0(sprite);sub_020136B4(sprite,x,y);RemoveWindow(&w);out->sprite=sprite;out->dimensions=dims;out->width=width;}
