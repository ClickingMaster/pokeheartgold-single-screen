#pragma force_active on
#include "global.h"
#include "heap.h"
#pragma force_active on
typedef struct {NNSG3dResFileHeader *file;NNSG3dResMdlSet *set;void *model;NNSG3dResTex *tex;} Model;
extern void *GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,int);
extern void ov89_0225AA24(void*,void*,void*,void*,int);
extern int ov89_0225A9B4(void*),ov89_0225A958(void*,void*);
extern BOOL GF3dRender_BindModelSet(NNSG3dResFileHeader*,const NNSG3dResTex*);
BOOL ov89_0225A878(void*,Model*,void*,void*,void*,int);
static inline u8 *GetEntry(u8 *dict,u32 index){if(dict&&index<dict[1])return dict+*(u16*)(dict+6)+4+index*4;return 0;}
static inline u8 *GetModel(u8 *set){if(set){u8 *entry=GetEntry(set+8,0);if(entry)return set+*(u32*)entry;}return 0;}
BOOL ov89_0225A878(void *state,Model *m,void *narc,void *special,void *item,int mode){m->file=GfGfxLoader_LoadFromOpenNarc(narc,29,0,125,0);GF_ASSERT(m->file);m->set=NNS_G3dGetMdlSet(m->file);m->model=GetModel((u8*)m->set);m->tex=NNS_G3dGetTex(m->file);if(m->tex){int loaded;ov89_0225AA24(m->tex,narc,special,item,mode);loaded=ov89_0225A9B4(m->tex);if(!loaded||!ov89_0225A958(state,m->tex)){if(loaded==1){NNSG3dTexKey normal,compressed;NNS_G3dTexReleaseTexKey(m->tex,&normal,&compressed);NNS_GfdDefaultFuncFreeTexVram(normal);NNS_GfdDefaultFuncFreeTexVram(compressed);NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(m->tex));}if(m->file)Heap_Free(m->file);memset(m,0,16);return 0;}GF3dRender_BindModelSet(m->file,m->tex);}return 1;}
