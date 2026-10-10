#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void G3X_Init(void);
extern void NNS_G3dGlbInit(void);
extern void NNS_G3dGeWaitSendDL(void);
extern void NNS_G3dGeFlushBuffer(void);
extern void MIi_CpuSend32(const void *, void *, u32);
extern int G3X_GetClipMtx(void *);
extern int G3X_GetVectorMtx(void *);
extern void MTX_Copy44To43_(void *, void *);
extern u32 NNS_G3dAnmObjCalcSizeRequired(void *, void *);
extern void *NNS_FndAllocFromAllocator(void *, u32);
extern void NNS_FndFreeToAllocator(void *, void *);

typedef struct State {
    u32 *buffer;
    volatile u32 locked;
    u32 fast;
} State;

extern State _021DD404;

static inline u8 *getentry(u8 *dict, u32 idx) {
    if (dict && idx < dict[1]) {
        u32 off = *(u16 *)(dict + 6);
        return dict + off + 4 + *(u16 *)(dict + off) * idx;
    }
    return 0;
}

static inline u8 *getres(u8 *mat, u32 idx) {
    if (mat) {
        u8 *entry = getentry(mat + 4, idx);
        if (entry) {
            return mat + *(u32 *)entry;
        }
    }
    return 0;
}

static inline u8 *getmat(u8 *mdl, u32 idx) {
    return getres(mdl && *(u32 *)(mdl + 8) ? mdl + *(u32 *)(mdl + 8) : 0, idx);
}

void NNS_G3dGeFlushBuffer(void);
void NNS_G3dGeWaitSendDL(void);
void simpleUnlock_(u32 *p);
void NNS_G3dGetCurrentMtx(void *m, void *v);
void NNS_G3dInit(void);
void *NNS_G3dAllocAnmObj(void *a, void *b, void *c);
void NNS_G3dFreeAnmObj(void *a, void *b);
void NNS_G3dMdlSetMdlDiff(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlAmb(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlSpec(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlEmi(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlLightEnableFlag(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlPolygonMode(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlCullMode(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlPolygonID(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlAlpha(u8 *m, u32 i, u32 v);
void NNS_G3dMdlSetMdlFogEnableFlag(u8 *m, u32 i, int v);
void NNS_G3dMdlSetMdlDepthTestCond(u8 *m, u32 i, int v);
void NNS_G3dMdlSetMdl1Dot(u8 *m, u32 i, int v);
void NNS_G3dMdlSetMdlFarClip(u8 *m, u32 i, int v);
void NNS_G3dMdlSetMdlXLDepthUpdate(u8 *m, u32 i, int v);
u32 NNS_G3dMdlGetMdlLightEnableFlag(u8 *m, u32 i);
u32 NNS_G3dMdlGetMdlFogEnableFlag(u8 *m, u32 i);
extern u32 GXi_DmaId;
extern u32 _021DD408;
extern void MI_SendGXCommandAsyncFast(u32, const void *, u32, void (*)(u32 *), u32 *), MI_SendGXCommandAsync(u32, const void *, u32, void (*)(u32 *), u32 *), MIi_CpuCopyFast(const void *, void *, u32);
void NNS_G3dGeSendDL(u32 *, u32);
void NNS_G3dGeBufferOP_N(u32, u32 *, u32);

void *NNS_G3dAllocAnmObj(void *a, void *b, void *c) {
    return NNS_FndAllocFromAllocator(a, NNS_G3dAnmObjCalcSizeRequired(b, c));
}

void NNS_G3dFreeAnmObj(void *a, void *b) {
    NNS_FndFreeToAllocator(a, b);
}
