#include "overlay93_arm_slop_r1_internal.h"

// global.h selects Thumb by default. This include must remain after the project
// header so this translation unit is compiled as the original ARM code.
// clang-format off
#include <nitro/code32.h>
// clang-format on

#define OV93_PACK_TEXCOORD(s, t) ((u16)(fx16)((s) >> (FX32_SHIFT - 4)) | ((u32)(u16)(fx16)((t) >> (FX32_SHIFT - 4)) << 16))
#define OV93_PACK_VTX16_XY(x, y) ((u32)(u16)(x) | ((u32)(u16)(y) << 16))
#define OV93_NORMAL_FRONT        0x1FF00000

void ov93_0225EFAC(Ov93SlopR1VtxSet *ctx) {
    s16 x;
    s16 y;
    s32 i;

    for (i = 0; i < 8; i++) {
        u32 begin = GX_BEGIN_QUADS;

        NNS_G3dGeBufferOP_N(G3OP_BEGIN, &begin, G3OP_BEGIN_NPARAMS);
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->lo[i].a, ctx->lo[i].b, &x, &y);
            texcoord = OV93_PACK_TEXCOORD(ctx->hi[i].a, ctx->hi[i].b);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = OV93_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = OV93_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->lo[i].c, ctx->lo[i].d, &x, &y);
            texcoord = OV93_PACK_TEXCOORD(ctx->hi[i].c, ctx->hi[i].d);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = OV93_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = OV93_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->lo[i].g, ctx->lo[i].h, &x, &y);
            texcoord = OV93_PACK_TEXCOORD(ctx->hi[i].g, ctx->hi[i].h);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = OV93_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = OV93_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            ov93_0225F268(ctx->lo[i].e, ctx->lo[i].f, &x, &y);
            texcoord = OV93_PACK_TEXCOORD(ctx->hi[i].e, ctx->hi[i].f);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = OV93_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = OV93_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        NNS_G3dGeBufferOP_N(G3OP_END, NULL, G3OP_END_NPARAMS);
    }
}
