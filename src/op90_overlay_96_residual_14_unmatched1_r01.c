#pragma force_active on
#include "global.h"
typedef struct Ov96Tex {
    u8 pad0[0x14];
    u32 texOffset;
    u8 pad18[0x38 - 0x18];
    u32 plttOffset;
} Ov96Tex;
typedef struct Ov96CharData {
    u8 pad0[0x14];
    u8 *rawData;
} Ov96CharData;
typedef struct Ov96Info {
    u16 a;
    u16 b;
    u16 big;
    u8 altPltt;
    u8 c;
} Ov96Info;
typedef struct Ov96Ctx {
    int heapId;
    u8 pad4[4];
    int x8;
    u8 padC[0x144 - 0xC];
    void *charMan;
    void *plttMan;
    void *cellMan;
    void *animMan;
    u8 pad154[4];
    u8 buffer[0x800];
} Ov96Ctx;
void *AddCharResObjFromNarc(void *, int, int, int, int, int, int);
void *AddPlttResObjFromNarc(void *, int, int, int, int, int, int, int);
void *AddCellOrAnimResObjFromNarc(void *, int, int, int, int, int, int);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *);
void SpriteTransfer_CreateExtPlttTransferTask(void *);
NNSG2dImageProxy *SpriteTransfer_GetCharProxy(void *);
NNSG2dImagePaletteProxy *SpriteTransfer_GetPaletteProxy(void *, NNSG2dImageProxy *);
Ov96CharData *GF2DGfxResObj_GetCharDataPtr(void *);
int ov96_021E91B8(u16, u16, u8);
void *AllocAndReadWholeNarcMemberByIdPair(int, int, int);
void sub_020145B4(void *, int, int, int, int, int, void *);
void DC_FlushRange(const void *, u32);
void GX_LoadOBJ(const void *, u32, u32);
void GXS_LoadOBJ(const void *, u32, u32);
void GX_LoadOBJPltt(const void *, u32, u32);
void GXS_LoadOBJPltt(const void *, u32, u32);
void Heap_Free(void *);
void ov96_021E8C70(Ov96Ctx *ctx, void **res, Ov96Info *info, int vram, u8 id, BOOL flag);
// @fn ov96_021E8C70
void ov96_021E8C70(Ov96Ctx *ctx, void **res, Ov96Info *info, int vram, u8 id, BOOL flag) {
    int file2;
    int file3;
    int dim;
    int i;
    void *narcData;
    u8 *texData;
    Ov96Tex *tex;
    NNSG2dImagePaletteProxy *plttProxy;
    NNSG2dImageProxy *charProxy;
    Ov96CharData *charData;
    BOOL main;
    BOOL sub;
    u32 size;
    int charFile;
    u8 *pltt;
    int j;
    u32 offset;
    if (flag) {
        if (info->big) {
            file2 = 10;
            charFile = 11;
            file3 = 9;
        } else {
            file2 = 7;
            charFile = 8;
            file3 = 6;
        }
    } else {
        if (info->big) {
            file2 = 4;
            charFile = 5;
            file3 = 3;
        } else {
            file2 = 1;
            charFile = 2;
            file3 = 0;
        }
    }
    res[0] = AddCharResObjFromNarc(ctx->charMan, 0x95, charFile, 0, id, vram, ctx->heapId);
    res[1] = AddPlttResObjFromNarc(ctx->plttMan, 0x31, 0x1E, 0, id, vram, 1, ctx->heapId);
    res[2] = AddCellOrAnimResObjFromNarc(ctx->cellMan, 0x95, file2, 0, id, 2, ctx->heapId);
    res[3] = AddCellOrAnimResObjFromNarc(ctx->animMan, 0x95, file3, 0, id, 3, ctx->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res[0]);
    SpriteTransfer_CreateExtPlttTransferTask(res[1]);
    main = FALSE;
    sub = FALSE;
    charProxy = SpriteTransfer_GetCharProxy(res[0]);
    plttProxy = SpriteTransfer_GetPaletteProxy(res[1], charProxy);
    charData = GF2DGfxResObj_GetCharDataPtr(res[0]);
    if (vram == 3) {
        main = TRUE;
        sub = TRUE;
    } else if (vram == 1) {
        main = TRUE;
    } else {
        sub = TRUE;
    }
    narcData = AllocAndReadWholeNarcMemberByIdPair(0x51, ov96_021E91B8(info->a, info->b, info->c), ctx->heapId);
    if (info->big) {
        size = 0x800;
        dim = 8;
    } else {
        size = 0x200;
        dim = 4;
    }
    tex = (Ov96Tex *)NNS_G3dGetTex(narcData);
    texData = (u8 *)tex + tex->texOffset;
    for (i = 0, offset = 0; i < 8; i++) {
        sub_020145B4(texData + offset, dim, 0, 0, dim, dim, ctx->buffer);
        if (!flag) {
            if (ctx->x8 && info->big) {
                for (j = 0; j < 0x800; j++) {
                    if (j < 0x800 - dim * 32) {
                        ctx->buffer[j] = ctx->buffer[j + dim * 32];
                    } else {
                        ctx->buffer[j] = 0;
                    }
                }
            }
            DC_FlushRange(ctx->buffer, size);
            if (main) {
                u32 loc = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
                GX_LoadOBJ(ctx->buffer, loc + offset, size);
            }
            if (sub) {
                u32 loc = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
                GXS_LoadOBJ(ctx->buffer, loc + offset, size);
            }
        }
        MIi_CpuCopyFast((u32 *)ctx->buffer, (u32 *)(charData->rawData + offset), size);
        offset += size;
    }
    pltt = (u8 *)tex + tex->plttOffset;
    if (info->altPltt) {
        pltt += 0x20;
    }
    DC_FlushRange(pltt, 0x20);
    if (main) {
        GX_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DMAIN), 0x20);
    }
    if (sub) {
        GXS_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DSUB), 0x20);
    }
    Heap_Free(narcData);
}
