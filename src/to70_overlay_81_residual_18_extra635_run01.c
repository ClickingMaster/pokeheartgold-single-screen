#pragma force_active on
#include "global.h"
typedef struct {u32 x,y;void*s;} Work;
typedef struct {u16 a;u16 pad;u32 b,c;void*s;} NewWork;
extern void *Sprite_GetImageProxy(void*);extern void DC_FlushRange(const void*,u32);extern void GX_LoadOBJ(const void*,u32,u32);extern void GXS_LoadOBJ(const void*,u32,u32);extern void *Heap_Alloc(int,u32);extern void *ov81_02242A8C(void*,int,int,int,int,int);extern void ov81_02242F10(NewWork*,u32,u32);extern void Sprite_SetPaletteOverride(void*,int);

void ov81_02242E14(Work*w,void*data,u32 size,int screen);
NewWork *ov81_02242E50(void*w,u32 b,u32 c,int heap);

void ov81_02242E14(Work*w,void*data,u32 size,int screen){u32 addr=NNS_G2dGetImageLocation(Sprite_GetImageProxy(w->s),(NNS_G2D_VRAM_TYPE)screen);DC_FlushRange(data,size);if(screen==1)GX_LoadOBJ(data,addr,size);else GXS_LoadOBJ(data,addr,size);}
NewWork *ov81_02242E50(void*w,u32 b,u32 c,int heap){NewWork*n=Heap_Alloc(heap,16);memset(n,0,16);n->a=0;n->b=b;n->c=c;n->s=ov81_02242A8C(w,0,0,10,0,1);ov81_02242F10(n,b,c);Sprite_SetPaletteOverride(n->s,1);return n;}
