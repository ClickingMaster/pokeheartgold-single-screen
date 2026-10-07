#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "renderer_joint_family_private.h"
static inline void *Materials(const NNSG3dResMdl *m){if(m){u32 off=m->ofsMat;if(off)return(u8*)m+off;}return 0;}
static inline void *Shapes(const NNSG3dResMdl *m){if(m){u32 off=m->ofsShp;if(off)return(u8*)m+off;}return 0;}
void G3dDrawInternal_(MdlRenderState*w,NNSG3dRenderObj*p){void*commands;MIi_CpuClearFast(0,(u32*)w,0x188);w->scaleFlags[0]=1;w->flags=1;commands=p->ptrUserSbc;if(commands==0)commands=(u8*)p->resMdl+p->resMdl->ofsSbc;w->command=commands;w->input=p;w->nodes=p->resMdl?&p->resMdl->nodeInfo:0;w->materials=Materials(p->resMdl);w->shapes=Shapes(p->resMdl);w->scale=_02110A80[p->resMdl->info.scalingRule];w->matrixHandler=_02110A74[p->resMdl->info.scalingRule];w->textureHandler=_02110A8C[p->resMdl->info.texMtxMode];w->extra1=p->resMdl->info.posScale;w->extra2=p->resMdl->info.invPosScale;if(p->cbFunc && p->cbCmd<32){w->slots[p->cbCmd]=p->cbFunc;w->timing[p->cbCmd]=p->cbTiming;}if(p->flag&1)w->flags|=0x80;if(p->flag&2)w->flags|=0x100;if(p->flag&4)w->flags|=0x200;if(p->flag&8)w->flags|=0x400;if(p->cbInitFunc)p->cbInitFunc(w);G3dDrawInternal_Loop_(w);p->flag&=~1;}
