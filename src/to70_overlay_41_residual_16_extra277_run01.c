#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{int heap;u8 pad[0x180-4];void*narc;};struct Item{int heap;void*data;};struct Palette{int heap;NNSG2dPaletteData*data;int type;};struct R{struct Item*items;int a;struct Palette*pal;int b,c;void*manager;};void*GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,int);void*ov41_022463DC(struct S*,void*,int);void GF2dGfxRawResMan_AllocObj(void*,void*,int);void ov41_02246B68(struct S*,struct R*);
void ov41_02246BEC(struct S*,struct R*);
void ov41_02246B68(struct S*s,struct R*r){int i;void*p;for(i=0;i<100;i++){p=GfGfxLoader_LoadFromOpenNarc(s->narc,i+1,0,14,1);GF_ASSERT(p!=NULL);r->items[i].data=ov41_022463DC(s,p,i);r->items[i].heap=s->heap;}p=GfGfxLoader_LoadFromOpenNarc(s->narc,0,0,14,1);GF2dGfxRawResMan_AllocObj(r->manager,p,0);NNS_G2dGetUnpackedPaletteData(p,&r->pal->data);r->pal->heap=s->heap;r->pal->type=3;}
