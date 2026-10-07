#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[4];} Window;
typedef struct {u8 pad[0xc];Window windows[101];void *message;u8 pad2[0x1f4];void *narc;} Work;
typedef struct {u8 pad[0x14];u8 *data;} Chars;
extern void *GfGfxLoader_GetCharDataFromOpenNarc(void *,int,int,Chars **,int);
extern void BlitBitmapRect(Window *,const void *,u16,u16,u16,u16,u16,u16,u16,u16,u16),Heap_Free(void *);
extern void ov18_021F9648(Window *,void *,u32,int,int,int,u32,int);
void ov18_021EE44C(Work *,int,u32);
void ov18_021EE44C(Work *w,int index,u32 value){
Chars *chars;void *allocation=GfGfxLoader_GetCharDataFromOpenNarc(w->narc,1,1,&chars,37);u8 *data=chars->data;u32 i;int x;
i=0;x=0;data+=32;for(;i<12;i++,x+=8){BlitBitmapRect(&w->windows[index],data,0,0,8,8,(u16)x,0,8,8,255);BlitBitmapRect(&w->windows[index],data,0,0,8,8,(u16)x,8,8,8,255);}
ov18_021F9648(&w->windows[index],w->message,value,96,0,0,0xf0800,1);Heap_Free(allocation);
}
