#pragma force_active on
#include "global.h"
extern BOOL GF3dRender_ResTexIsLoaded(NNSG3dResTex*);
void ov01_021F1A48(void*,NNSG3dRenderObj*,NNSG3dResMdl**,NNSG3dResFileHeader**);
void ov01_021F1A48(void*unused,NNSG3dRenderObj*obj,NNSG3dResMdl**out,NNSG3dResFileHeader**file){NNSG3dResTex*tex=NNS_G3dGetTex(*file);if(tex&&!GF3dRender_ResTexIsLoaded(tex)){DC_FlushRange(*file,(*file)->fileSize);if(!NNS_G3dResDefaultSetup(*file))GF_AssertFail();}*out=NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*file),0);GF_ASSERT(out!=0);NNS_G3dRenderObjInit(obj,*out);}
