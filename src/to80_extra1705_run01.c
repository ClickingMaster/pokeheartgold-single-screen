#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void*window;u8 frames[8][128];u8 backup[128];u16 tile;u8 index;u8 a:7,unuseda:1;u8 b:2,unusedb:6;} Icon;
typedef struct {void*bg;u8 rest[12];} Window;
typedef struct {u8 pad0[0x14];void*raw;} CharData;
extern int BgConfig_GetHeapId(void*),GetWindowBgId(Window*);extern u8*BgGetCharPtr(int);extern void*Heap_Alloc(int,u32);extern void Heap_Free(void*);extern void*GfGfxLoader_GetCharData(int,int,int,CharData**,int);extern void sub_0200EA24(void*,int,int,int,int,void*,int,int,int,int,int,int),sub_0200F3D0(void*,Icon*),sub_0200F1D4(Icon*,int);extern void*SysTask_CreateOnVBlankQueue(void(*)(void*,Icon*),Icon*,int);
Icon*WaitingIcon_New(Window*,int);
Icon*WaitingIcon_New(Window*w,int tile){int heap=BgConfig_GetHeapId(w->bg);u8*chars=BgGetCharPtr(GetWindowBgId(w));Icon*icon=Heap_Alloc(heap,0x48c);u8*tmp;u32 offsetA,offsetB;u8 i;CharData*data;void*allocation;memcpy(icon->backup,chars+(tile+18)*32,128);tmp=Heap_Alloc(heap,128);offsetA=(tile+10)*32;memcpy(tmp,chars+offsetA,32);offsetB=(tile+11)*32;memcpy(tmp+32,chars+offsetB,32);memcpy(tmp+64,chars+offsetA,32);memcpy(tmp+96,chars+offsetB,32);for(i=0;i<8;i++)memcpy(icon->frames[i],tmp,128);Heap_Free(tmp);allocation=GfGfxLoader_GetCharData(38,23,0,&data,heap);sub_0200EA24(data->raw,0,0,16,128,icon->frames,16,128,0,0,16,128);Heap_Free(allocation);icon->window=w;icon->tile=tile;icon->index=0;icon->a=0;icon->b=0;SysTask_CreateOnVBlankQueue(sub_0200F3D0,icon,0);sub_0200F1D4(icon,1);return icon;}
