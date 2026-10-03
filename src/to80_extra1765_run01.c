#pragma force_active on
#include "global.h"
typedef struct{u8 p[32];void *manager;}Host;typedef struct{u8 p[24];void *res;}Man;void *GF3dGfxRawResMan_GetObjById(void*,u32);void *GF3dGfxRawResObj_GetTex(void*);void *ov01_021F1AD4(Host*,u32);
void *ov01_021F1AD4(Host*h,u32 id){void*tex;void*obj;obj=GF3dGfxRawResMan_GetObjById(((Man*)h->manager)->res,id);tex=NULL;GF_ASSERT(obj);if(obj)tex=GF3dGfxRawResObj_GetTex(obj);return tex;}
