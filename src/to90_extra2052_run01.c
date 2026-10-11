#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern const s16 ov92_02263BEC[3][3][4];
extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int),InitWindow(void*),AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),FillWindowPixelBuffer(void*,int),CopyWindowToVram(void*),String_Delete(void*),AddTextPrinterParameterizedWithColorAndSpacing(void*,int,void*,int,int,int,int,int,int,void*);
extern void *PlazaGameRoster_GetProfile(void*,int),*PlayerProfile_GetPlayerName_NewString(void*,int);
extern int ov90_022588A4(void*,int),ov92_0225E188(void*,void*);
void ov92_0225E1A8(void*,void*);
#pragma force_active on
void ov92_0225E1A8(void *ctx,void *w){s16 table[3][3][4];int tile=256;int i;ARRAY_ASSIGN(table,ov92_02263BEC);
 switch(W(w,4)){case 2:GfGfxLoader_LoadScrnDataFromOpenNarc(P(P(w,0x14),0),79,P(P(w,0x14),0x10),7,0,0,0,113);break;case 3:GfGfxLoader_LoadScrnDataFromOpenNarc(P(P(w,0x14),0),80,P(P(w,0x14),0x10),7,0,0,0,113);break;case 4:GfGfxLoader_LoadScrnDataFromOpenNarc(P(P(w,0x14),0),81,P(P(w,0x14),0x10),7,0,0,0,113);break;default:GF_AssertFail();break;}
 i=0;if(W(w,4)>0){u8 *window=(u8*)w+0x1ff0;int n=0;do{if(i!=W(w,0)){int flag=ov90_022588A4((u8*)ctx+0x8c,i);void *profile=PlazaGameRoster_GetProfile((u8*)ctx+0x8c,i);InitWindow(window);s16 *coords=(s16*)((u8*)table+W(w,4)*24+n*8);int width=coords[-22],height=coords[-21];AddWindowParameterized(P(P(w,0x14),0x10),window,7,(u8)coords[-24],(u8)coords[-23],(u8)width,(u8)height,14,(u16)tile);tile+=width*height;FillWindowPixelBuffer(window,255);void *name=PlayerProfile_GetPlayerName_NewString(profile,113);int x=ov92_0225E188(window,name);if(flag)AddTextPrinterParameterizedWithColorAndSpacing(window,0,name,x,0,0,0x5060f,0,0,0);else AddTextPrinterParameterizedWithColorAndSpacing(window,0,name,x,0,0,0x1020f,0,0,0);String_Delete(name);CopyWindowToVram(window);window+=16;n++;}i++;}while(i<W(w,4));}
}
