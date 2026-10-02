#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void G3X_Init(void), NNS_G3dGlbInit(void),NNS_G3dGeWaitSendDL(void),NNS_G3dGeFlushBuffer(void);
extern void MIi_CpuSend32(const void*,void*,u32);
extern int G3X_GetClipMtx(void*),G3X_GetVectorMtx(void*);
extern void MTX_Copy44To43_(void*,void*);
extern u32 NNS_G3dAnmObjCalcSizeRequired(void*,void*);
extern void *NNS_FndAllocFromAllocator(void*,u32);
extern void NNS_FndFreeToAllocator(void*,void*);
typedef struct State {u32 *buffer;volatile u32 locked;u32 fast;} State;
extern State _021DD404;
static inline u8 *getentry(u8*dict,u32 idx){if(dict && idx<dict[1]) {u32 off=*(u16*)(dict+6);return dict+off+4+*(u16*)(dict+off)*idx;}return 0;}
static inline u8 *getres(u8*mat,u32 idx){if(mat){u8*entry=getentry(mat+4,idx);if(entry)return mat+*(u32*)entry;}return 0;}
static inline u8 *getmat(u8*mdl,u32 idx){return getres(mdl && *(u32*)(mdl+8)?mdl+*(u32*)(mdl+8):0,idx);}
void NNS_G3dGeFlushBuffer(void);
void NNS_G3dGeWaitSendDL(void);
void simpleUnlock_(u32 *p);
void NNS_G3dGetCurrentMtx(void *m,void *v);
void NNS_G3dInit(void);
void *NNS_G3dAllocAnmObj(void*a,void*b,void*c);
void NNS_G3dFreeAnmObj(void*a,void*b);
void NNS_G3dMdlSetMdlDiff(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlAmb(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlSpec(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlEmi(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlLightEnableFlag(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlPolygonMode(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlCullMode(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlPolygonID(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlAlpha(u8*m,u32 i,u32 v);
void NNS_G3dMdlSetMdlFogEnableFlag(u8*m,u32 i,int v);
void NNS_G3dMdlSetMdlDepthTestCond(u8*m,u32 i,int v);
void NNS_G3dMdlSetMdl1Dot(u8*m,u32 i,int v);
void NNS_G3dMdlSetMdlFarClip(u8*m,u32 i,int v);
void NNS_G3dMdlSetMdlXLDepthUpdate(u8*m,u32 i,int v);
u32 NNS_G3dMdlGetMdlLightEnableFlag(u8*m,u32 i);
u32 NNS_G3dMdlGetMdlFogEnableFlag(u8*m,u32 i);
extern u32 GXi_DmaId,_021DD408;
extern void MI_SendGXCommandAsyncFast(u32,const void*,u32,void(*)(u32*),u32*),MI_SendGXCommandAsync(u32,const void*,u32,void(*)(u32*),u32*),MIi_CpuCopyFast(const void*,void*,u32);
void NNS_G3dGeSendDL(u32*,u32);void NNS_G3dGeBufferOP_N(u32,u32*,u32);
void NNS_G3dGeFlushBuffer(void){if(_021DD404.locked)NNS_G3dGeWaitSendDL();if(_021DD404.buffer && _021DD404.buffer[0]){MIi_CpuSend32(_021DD404.buffer+1,(void*)0x4000400,_021DD404.buffer[0]*4);_021DD404.buffer[0]=0;}}
void NNS_G3dGeWaitSendDL(void){while(_021DD404.locked){}}
void simpleUnlock_(u32 *p){*p=0;}
void NNS_G3dGeSendDL(u32*p,u32 n){if(n<256 || GXi_DmaId==(u32)-1){NNS_G3dGeBufferOP_N(p[0],p+1,(n>>2)-1);return;}NNS_G3dGeFlushBuffer();_021DD404.locked=1;if(_021DD404.fast)MI_SendGXCommandAsyncFast(GXi_DmaId,p,n,simpleUnlock_,&_021DD408);else MI_SendGXCommandAsync(GXi_DmaId,p,n,simpleUnlock_,&_021DD408);}
void NNS_G3dGeBufferOP_N(u32 op,u32*p,u32 n){if(_021DD404.buffer){if(_021DD404.locked && _021DD404.buffer[0]+1+n<=192){u32 old=_021DD404.buffer[0]++;_021DD404.buffer[old+1]=op;if(n){MIi_CpuCopyFast(p,_021DD404.buffer+1+_021DD404.buffer[0],n*4);_021DD404.buffer[0]+=n;}return;}if(_021DD404.buffer[0])NNS_G3dGeFlushBuffer();else if(_021DD404.locked)NNS_G3dGeWaitSendDL();}else if(_021DD404.locked)NNS_G3dGeWaitSendDL();*(volatile u32*)0x4000400=op;MIi_CpuSend32(p,(void*)0x4000400,n*4);}
void NNS_G3dGetCurrentMtx(void *m,void *v){u32 tmp[16];NNS_G3dGeFlushBuffer();*(volatile u32*)0x4000440=0;*(volatile u32*)0x4000444=0;*(volatile u32*)0x4000454=0;if(m){while(G3X_GetClipMtx(tmp)){}MTX_Copy44To43_(tmp,m);}if(v){while(G3X_GetVectorMtx(v)){}}*(volatile u32*)0x4000448=1;*(volatile u32*)0x4000440=2;}
void NNS_G3dInit(void){G3X_Init();NNS_G3dGlbInit();*(volatile u32*)0x4000600=(*(volatile u32*)0x4000600 & ~0xc0000000)|0x80000000;}
