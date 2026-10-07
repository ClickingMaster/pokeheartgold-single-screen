#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct{void* bg;} Window;
typedef struct{u8 p[20];void*raw;} CharData;
int BgConfig_GetHeapId(void*);
u8 GetWindowBgId(Window*);
void*Heap_Alloc(int,u32);
void*BgGetCharPtr(int);
void*GfGfxLoader_GetCharData(int,int,int,CharData**,int);
void sub_0200EA24(void*,u8,u8,u8,u8,void*,u8,u8,u8,u8,u8,u8);
void BG_LoadCharTilesData(void*,int,void*,u32,u32);
void Heap_Free(void*);
void sub_0200EA68(Window*,int,int,int,u8,u8);
#pragma force_active on
void sub_0200EA68(Window*w,int member,int tile,int count,u8 a,u8 b){
 struct{volatile int tile,count;u32 bg;volatile u32 size;void*loaded,*raw;u32 first,second;CharData*cd;} locals;
 int heap;locals.tile=tile;locals.count=count;heap=BgConfig_GetHeapId(w->bg);u8*chars,*dst;u8 i,width,height;
 locals.bg=GetWindowBgId(w);locals.size=locals.count<<7;dst=Heap_Alloc(heap,locals.size);chars=BgGetCharPtr(locals.bg);
 locals.loaded=GfGfxLoader_GetCharData(38,member,0,&locals.cd,heap);locals.raw=locals.cd->raw;
 i=0;if(locals.count>0){locals.first=(locals.tile+10)<<5;locals.second=(locals.tile+11)<<5;
 do{memcpy(dst+(i<<7),chars+locals.first,32);memcpy(dst+((i<<7)+32),chars+locals.second,32);memcpy(dst+((i<<7)+64),chars+locals.first,32);memcpy(dst+((i<<7)+96),chars+locals.second,32);i++;}while(i<locals.count);}
 height=(16-b)*locals.count;width=16-a;sub_0200EA24(locals.raw,a,b,width,height,dst,width,height,0,0,width,height);{u32 index=locals.tile;u32 size=locals.size;index+=18;locals.tile=index;BG_LoadCharTilesData(w->bg,locals.bg,dst,size,index);}Heap_Free(locals.loaded);Heap_Free(dst);}
