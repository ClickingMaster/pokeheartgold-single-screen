#pragma force_active on
#include "global.h"
#include <nnsys/g3d.h>
extern void NARC_ReadFile(void *,int,void *);extern int GF3dRender_ResTexIsLoaded(void *),GF3dRender_BindModelSet(void *,void *);NNSG3dResMdl *ov01_021F67B4(void *,int,NNSG3dRenderObj *,void **,void *);
NNSG3dResMdl *ov01_021F67B4(void *narc,int file,NNSG3dRenderObj *obj,void **buffer,void *tex){NNSG3dResMdlSet *set;NNSG3dResMdl *mdl;NARC_ReadFile(narc,file,*buffer);if(tex&&GF3dRender_ResTexIsLoaded(tex)==1){if(!GF3dRender_BindModelSet(*buffer,tex))GF_AssertFail();}set=NNS_G3dGetMdlSet(*buffer);if(set->dict.numEntry!=1)GF_AssertFail();set=NNS_G3dGetMdlSet(*buffer);mdl=NNS_G3dGetMdlByIdx(set,0);if(!mdl)GF_AssertFail();NNS_G3dRenderObjInit(obj,mdl);return mdl;}
