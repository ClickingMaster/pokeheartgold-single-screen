#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Win {void *bg;u8 id,x,y,width,height,pal;u16 base:15,mode:1;void *pixels;} Win;
typedef struct Out {u32 a,b,c;Win win;} Out;
typedef struct Context {void *bg,*other,*msg;} Context;
typedef struct Names {u32 pad[2];u16 *names[4];u32 flags[4];} Names;
extern const u32 ov91_02261DAC[];
extern void AddWindowParameterized(void*,Win*,u8,u8,u8,u8,u8,u8,u16);
extern void FillWindowPixelRect(Win*,u8,u8,u8,u8,u8);
extern void *String_New(u32,int);
extern void ReadMsgDataIntoString(void*,u32,void*);
extern void AddTextPrinterParameterizedWithColor(Win*,u32,void*,u32,u32,u32,u32,void*);
extern void String_Delete(void*);
extern void GfGfxLoader_LoadCharDataFromOpenNarc(void*,u32,void*,u32,u32,u32,u32,int);
extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,u32,void*,u32,u32,u32,u32,int);
extern void GfGfxLoader_GXLoadPalFromOpenNarc(void*,u32,u32,u32,u32,int);
extern void InitWindow(Win*);
extern void BgTilemapRectChangePalette(void*,u8,u8,u8,u8,u8,u8);
extern void FillWindowPixelBuffer(Win*,u8);
extern void PlayerName_FlatToString(u16*,void*);
extern void SetWindowX(Win*,u8);
extern void SetWindowY(Win*,u8);
extern u32 FontID_String_GetWidth(u32,void*,u32);
extern void RemoveWindow(Win*);
extern void GfGfx_EngineATogglePlanes(int,int);
void ov91_0225D8F0(Out*,Context*,Names*,u32,u32,void*,int);
void ov91_0225D8F0(Out*out,Context*ctx,Names*names,u32 count,u32 selected,void*narc,int heap){u8*p=(u8*)out;u32 size=sizeof(*out);do{*p++=0;}while(--size);AddWindowParameterized(ctx->bg,&out->win,1,2,19,28,4,5,10);FillWindowPixelRect(&out->win,15,0,0,224,32);void *str=String_New(128,heap);ReadMsgDataIntoString(ctx->msg,0,str);AddTextPrinterParameterizedWithColor(&out->win,0,str,0,0,255,0x1020f,0);String_Delete(str);GfGfxLoader_LoadCharDataFromOpenNarc(narc,18,ctx->bg,2,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,count+18,ctx->bg,2,0,0,0,heap);GfGfxLoader_GXLoadPalFromOpenNarc(narc,19,0,0,128,heap);Win local;InitWindow(&local);AddWindowParameterized(ctx->bg,&local,2,0,0,8,2,5,256);u32 x;u32 base=256;str=String_New(128,heap);u32 i;for(i=0;i<count;i++){const u32 *table=ov91_02261DAC+selected*32+count*8+i*2;if(i!=selected){x=table[-8];u32 y=table[-7];BgTilemapRectChangePalette(ctx->bg,2,(u8)(x-1),(u8)(y-1),10,4,(u8)i);FillWindowPixelBuffer(&local,15);u32 color=names->flags[i]==1?0x5060f:0x1020f;PlayerName_FlatToString(names->names[i],str);SetWindowX(&local,(u8)x);SetWindowY(&local,(u8)y);local.base=(u16)base;u32 width=FontID_String_GetWidth(0,str,0);AddTextPrinterParameterizedWithColor(&local,0,str,(64-width)>>1,0,0,color,0);base+=16;}}String_Delete(str);RemoveWindow(&local);out->b=0;GfGfx_EngineATogglePlanes(2,0);GfGfx_EngineATogglePlanes(4,0);}
