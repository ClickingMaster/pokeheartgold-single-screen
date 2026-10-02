#pragma force_active on
#include "global.h"
typedef struct {NNSG3dResFileHeader*file;NNSG3dRenderObj obj;NNSG3dResMdlSet*set;NNSG3dResMdl*model;NNSG3dResTex*tex;int arg,x,y,z;u8 p[0x88-0x74];int flags;} Model;
typedef struct {u8 p[0x18];Model*models;} Work;
extern void*GfGfxLoader_LoadFromNarc(int,int,int,int,int);extern u32 GetNarcMemberSizeByIdPair(int,int);extern BOOL GF3dRender_AllocAndLoadTexResources(NNSG3dResTex*),GF3dRender_BindModelSet(NNSG3dResFileHeader*,const NNSG3dResTex*);
Model*ov71_02247610(Work*,int,int,int,int,int,int,int);
Model*ov71_02247610(Work*w,int idx,int narc,int member,int x,int y,int z,int arg){Model*base=w->models;Model*m=&base[idx];base[idx].file=GfGfxLoader_LoadFromNarc(narc,member,0,57,1);DC_FlushRange(base[idx].file,GetNarcMemberSizeByIdPair(narc,member));if(base[idx].file){m->set=NNS_G3dGetMdlSet(base[idx].file);m->model=NNS_G3dGetMdlByIdx(m->set,0);m->tex=NNS_G3dGetTex(m->file);GF3dRender_AllocAndLoadTexResources(m->tex);GF3dRender_BindModelSet(m->file,m->tex);NNS_G3dRenderObjInit(&m->obj,m->model);m->x=x;m->y=y;m->z=z;m->flags=31;m->arg=arg;}return m;}
