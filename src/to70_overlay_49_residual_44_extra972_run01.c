#pragma force_active on
#include "global.h"
#pragma force_active on
#include <nnsys/g3d/binres/res_struct_accessor_inline.h>
typedef struct{NNSG3dResFileHeader *res;NNSG3dResMdlSet*set;NNSG3dResMdl*model;NNSG3dResTex*tex;}Work;
extern void WiFiPlazaMap_LoadModelFile(Work*);
extern void GF3dRender_BindModelSet(void*,void*);

void ov49_0225D528(Work*w);

void ov49_0225D528(Work*w){WiFiPlazaMap_LoadModelFile(w);w->set=NNS_G3dGetMdlSet(w->res);w->model=NNS_G3dGetMdlByIdx(w->set,0);w->tex=NNS_G3dGetTex(w->res);GF3dRender_BindModelSet(w->res,w->tex);}
