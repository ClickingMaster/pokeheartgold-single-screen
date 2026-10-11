#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
typedef struct{u8 pad[20];u8 *data;}CharData;
extern void *GfGfxLoader_GetCharDataFromOpenNarc(void *,int,int,CharData **,int);extern void BlitBitmapRect(void *,void *,u16,u16,u16,u16,u16,u16,u16,u16,u16),Heap_Free(void *),PokedexApp_PrintMessage(void *,void *,int,int,int,int,u32,int),CopyWindowPixelsToVram_TextMode(void *);extern int GetWindowWidth(void *);
void PokedexApp_DrawSearchButton(u8 *,int,int);
void PokedexApp_DrawSearchButton(u8 *p,int win,int text){u8 *base=p+12;int offset=win*16;CharData *desc;void *buf=GfGfxLoader_GetCharDataFromOpenNarc(P(p,0x854),4,1,&desc,37);u8 *pixels=desc->data;u32 i;int x=0;for(i=0;i<8;i++,x+=8){BlitBitmapRect(base+offset,pixels+0xc40,0,0,8,8,(u16)x,0,8,8,255);BlitBitmapRect(base+offset,pixels+0xca0,0,0,8,8,(u16)x,8,8,8,255);}Heap_Free(buf);{int width=GetWindowWidth(base+offset);x=width*8;PokedexApp_PrintMessage(base+offset,P(p,0x65c),text,x/2,0,4,0x20100,2);}CopyWindowPixelsToVram_TextMode(base+offset);}
