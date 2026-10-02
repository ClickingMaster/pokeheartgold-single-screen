#pragma force_active on
#include "global.h"
#pragma force_active on
void ov96_021EEA94(void*,int,int,int);void ov96_021EEB84(void*,int,int,int);void ov96_021EEA88(void**,int,int,int);void ov96_021EEB74(void***,int,int,int);
typedef struct {u8 size,count;u16 pad;u16 offset;} Dict;
extern void *NARC_New(int,int),*NARC_AllocAndReadWholeMember(void *,int,int),*Heap_AllocAtEnd(int,int),*Sprite_GetPaletteProxy(void *);
extern void NARC_Delete(void *),Heap_Free(void *),thunk_Sprite_SetDrawFlag(void *,int),sub_020145B4(void *,int,int,int,int,int,void *),ov96_021EED64(void *,void *,int);
extern NNS_G2D_VRAM_TYPE Sprite_GetVramType(void *);
static inline void *Data(void *tex,int index){Dict *d;if(tex){d=(Dict *)((u8 *)tex+0x3c);if(d&&index<d->count){u16 *table=(u16 *)((u8 *)d+d->offset);return (u8 *)table+4+table[0]*index;}}return 0;}
void ov96_021EEB74(void***a,int b,int c,int d){ov96_021EEB84(*a[c],b,c,d);}
