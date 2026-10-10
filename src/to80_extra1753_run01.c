#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
typedef struct Render Render;

struct Render {
    u8 *cmd;
    u32 unk4, flags;
    u8 gapC[0x30];
    void (*env)(Render *);
    void (*prj)(Render *);
    u8 gap44[0x54];
    u8 envStage, prjStage;
    u8 gap9a[0x16];
    u8 *mat;
    u8 gapb4[0x24];
    u8 *resources;
};

extern u32 _02110A9C[];
extern u32 _02110AA8[];
extern u32 _02110AA0[];
extern u32 _02110ACC[];
extern void NNS_G3dGetCurrentMtx(MtxFx43 *, MtxFx33 *);
extern void NNS_G3dGeFlushBuffer(void);
extern void NNS_G3dGeSendDL(const void *, u32);
extern MtxFx43 *NNS_G3dGlbGetInvV(void);

static inline u8 *lookup(u8 *base, u32 index, int mat) {
    u8 *dict, *entry, *result;
    if (!base) {
        goto null;
    }
    dict = base + (mat ? 4 : 0);
    if (dict && index < B(base, mat ? 5 : 1)) {
        u16 off = H(base, mat ? 10 : 6), stride = H(dict, off);
        entry = dict + off + 4 + stride * index;
    } else {
        entry = 0;
    }
    if (entry) {
        result = base + W(entry, 0);
        goto done;
    }
null:
    result = 0;
done:
    return result;
}

static inline u8 *matrixData(u8 *mat) {
    u8 *p = mat + 0x2c;
    u16 flags = H(mat, 0x1e);
    if (!(flags & 2)) {
        p += 8;
    }
    if (!(flags & 4)) {
        p += 4;
    }
    if (!(flags & 8)) {
        p += 8;
    }
    return p;
}

void NNSi_G3dFuncSbc_ENVMAP(Render *, u32);

void NNSi_G3dFuncSbc_ENVMAP(Render *s, u32 opt) {
    MtxFx33 normal;
    u32 scale[3], modeStart, coord, modeGet, modeRestore, modeEnd;
    u32 flags, step, skip;
    u8 *mat;
    u16 w, h;
    flags = s->flags;
    if (flags & 0x200) {
        goto end;
    }
    if (!(flags & 1)) {
        goto end;
    }
    if ((W(s->mat, 16) & 0xc0000000) != 0x80000000) {
        W(s->mat, 16) &= ~0xc0000000;
        W(s->mat, 16) |= 0x80000000;
        _02110A9C[3] = W(s->mat, 16);
        NNS_G3dGeBufferOP_N(_02110A9C[2], (const u32 *)(_02110AA8), 1);
    }
    modeStart = 3;
    NNS_G3dGeBufferOP_N(16, (const u32 *)(&modeStart), 1);
    step = s->env ? s->envStage : 0;
    if (step == 1) {
        s->flags &= ~0x40;
        s->env(s);
        step = s->env ? s->envStage : 0;
        skip = s->flags & 0x40;
    } else {
        skip = 0;
    }
    if (!skip) {
        h = H(s->mat, 0x2e);
        w = H(s->mat, 0x2c);
        scale[0] = (u32)w << 15;
        scale[1] = (u32)(-h) << 15;
        scale[2] = 0x10000;
        NNS_G3dGeBufferOP_N(27, (const u32 *)(scale), 3);
        coord = (u16)(s16)(w << 3) | ((u16)(s16)(h << 3) << 16);
        NNS_G3dGeBufferOP_N(34, (const u32 *)(&coord), 1);
    }
    if (step == 2) {
        s->flags &= ~0x40;
        s->env(s);
        step = s->env ? s->envStage : 0;
        skip = s->flags & 0x40;
    } else {
        skip = 0;
    }
    if (!skip) {
        mat = lookup(s->resources, B(s->cmd, 1), 1);
        if (H(mat, 0x1e) & 0x2000) {
            NNS_G3dGeBufferOP_N(24, (const u32 *)(matrixData(mat)), 16);
        }
    }
    if (step == 3) {
        s->flags &= ~0x40;
        s->env(s);
        skip = s->flags & 0x40;
    } else {
        skip = 0;
    }
    if (!skip) {
        modeGet = 2;
        NNS_G3dGeBufferOP_N(16, (const u32 *)(&modeGet), 1);
        NNS_G3dGetCurrentMtx(0, &normal);
        modeRestore = 3;
        NNS_G3dGeBufferOP_N(16, (const u32 *)(&modeRestore), 1);
        flags = NNS_G3dGlb.flag;
        if (flags & 1) {
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&NNS_G3dGlb.cameraMtx), 9);
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&NNS_G3dGlb.prmBaseRot), 9);
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&normal), 9);
        } else if (flags & 2) {
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&NNS_G3dGlb.cameraMtx), 9);
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&normal), 9);
        } else {
            NNS_G3dGeBufferOP_N(26, (const u32 *)(&normal), 9);
        }
    }
    modeEnd = 2;
    NNS_G3dGeBufferOP_N(16, (const u32 *)(&modeEnd), 1);
end:
    s->cmd += 3;
}
