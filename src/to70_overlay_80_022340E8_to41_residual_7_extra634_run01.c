#pragma force_active on
#include "global.h"
typedef struct {u8 pad[12];void*data;} Pltt;
extern void *Heap_Alloc(int,u32);extern void Heap_Free(void*);extern void *NARC_New(int,int);extern void NARC_Delete(void*);extern void *GfGfxLoader_GetPlttDataFromOpenNarc(void*,int,Pltt**,int);extern void BlendPalette(void*,void*,int,int,int);extern void DC_FlushRange(const void*,u32);extern void GX_BeginLoadBGExtPltt(void);extern void GX_LoadBGExtPltt(const void*,u32,u32);extern void GX_EndLoadBGExtPltt(void);

void ov80_02234DC4(int color,int mode);

void ov80_02234DC4(int color,int mode){int size,index;void*buffer,*narc,*raw;Pltt*p;if(mode==0){index=162;size=480;}else{index=161;size=288;}buffer=Heap_Alloc(101,8192);memset(buffer,0,8192);narc=NARC_New(183,101);raw=GfGfxLoader_GetPlttDataFromOpenNarc(narc,index,&p,101);BlendPalette(p->data,buffer,4096,color,0);DC_FlushRange(buffer,8192);GX_BeginLoadBGExtPltt();GX_LoadBGExtPltt(buffer,24576,size);GX_EndLoadBGExtPltt();NARC_Delete(narc);Heap_Free(buffer);Heap_Free(raw);}
