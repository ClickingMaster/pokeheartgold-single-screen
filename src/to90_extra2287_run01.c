#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 heap;u8 p[0x17c];void *narc;} Work;
typedef struct{int heap;void *data;} Data;
typedef struct{int heap;void *data;int active;} Palette;
typedef struct{Data *a;u32 b;Palette *palette;u32 c,d;void *manager;} Output;
extern void *GfGfxLoader_LoadFromOpenNarc(void *,int,int,int,int),*ov41_022463DC(Work *,void *,u32);
extern void GF2dGfxRawResMan_AllocObj(void *,void *,int);
static inline Data *entry(Output *o,int i){return (Data *)((u32)o->a+i*8);}
static inline Palette *palette(Output *o,int i){return (Palette *)((u32)o->palette+i*12);}
void ov41_02246BEC(Work *,void *);
void ov41_02246BEC(Work *w,void *output){int narc;Output *out=output;int i=0;narc=135;for(;i<18;i++){void *raw=GfGfxLoader_LoadFromOpenNarc(w->narc,i+206,0,14,1);u32 offset=(i+100)*8;void *processed=ov41_022463DC(w,raw,i+100);((Data *)((u32)out->a+offset))->data=processed;((Data *)((u32)out->a+offset))->heap=w->heap;int id=i+1;raw=GfGfxLoader_LoadFromOpenNarc(w->narc,narc,0,14,1);GF2dGfxRawResMan_AllocObj(out->manager,raw,id);u32 palOffset=id*12;NNS_G2dGetUnpackedPaletteData(raw,(NNSG2dPaletteData **)&((Palette *)((u32)out->palette+palOffset))->data);if(!((Palette *)((u32)out->palette+palOffset))->data)GF_AssertFail();((Palette *)((u32)out->palette+palOffset))->heap=w->heap;((Palette *)((u32)out->palette+palOffset))->active=1;narc+=4;}}
