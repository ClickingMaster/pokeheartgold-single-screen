#pragma force_active on
#include "global.h"
#pragma force_active on
void ov96_021EEA94(void*,int,int,int);void ov96_021EEB84(void*,int,int,int);void ov96_021EEA88(void**,int,int,int);void ov96_021EEB74(void***,int,int,int);
typedef struct {u8 size,count;u16 pad,other;u16 offset;} Dict;
extern void *NARC_New(int,int),*NARC_AllocAndReadWholeMember(void *,int,int),*Heap_AllocAtEnd(int,int),*Sprite_GetPaletteProxy(void *);
extern void NARC_Delete(void *),Heap_Free(void *),thunk_Sprite_SetDrawFlag(void *,int),sub_020145B4(void *,int,int,int,int,int,void *),ov96_021EED64(void *,void *,int);
extern NNS_G2D_VRAM_TYPE Sprite_GetVramType(void *);
static inline void *Data(void *tex,u32 index){Dict *d;if(tex){d=(Dict *)((u32)tex+0x3c);if(d&&index<*(u8 *)((u32)tex+0x3d)){u16 *table=(u16 *)((u32)d+*(u16 *)((u32)tex+0x42));return (void *)((u32)table+4+table[0]*index);}return 0;}return 0;}
void ov96_021EEA94(void *sprite,int member,int index,int heap){void *narc,*raw,*tex,*buf;u8 *data;int offset,location,palette;NNS_G2D_VRAM_TYPE vram;thunk_Sprite_SetDrawFlag(sprite,0);narc=NARC_New(81,heap);raw=NARC_AllocAndReadWholeMember(narc,member,heap);tex=NNS_G3dGetTex(raw);offset=(*(u32 *)Data(tex,index)&65535)<<3;data=(u8 *)tex+*(u32 *)((u8 *)tex+0x14);buf=Heap_AllocAtEnd(heap,512);sub_020145B4(data+offset,4,0,0,4,4,buf);ov96_021EED64(sprite,buf,512);Heap_Free(buf);vram=Sprite_GetVramType(sprite);palette=*(u32 *)((u8 *)tex+0x38);DC_FlushRange((u8 *)tex+palette,32);location=NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite),vram);if(vram==1)GX_LoadOBJPltt((u8 *)tex+palette,location,32);else GXS_LoadOBJPltt((u8 *)tex+palette,location,32);Heap_Free(raw);NARC_Delete(narc);thunk_Sprite_SetDrawFlag(sprite,1);}
