#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_Image.h>

extern void GX_BeginLoadTexPltt(void);
extern void GX_LoadTexPltt(const void *, u32, u32);
extern void GX_EndLoadTexPltt(void);
extern void NNS_G2dSetImagePaletteLocation(NNSG2dImagePaletteProxy *, NNS_G2D_VRAM_TYPE, u32);

extern void NNS_G2dSetImageLocation(NNSG2dImageProxy *, NNS_G2D_VRAM_TYPE, u32);

void NNS_G2dLoadImageVramTransfer(const NNSG2dCharacterData *src, u32 addr, NNS_G2D_VRAM_TYPE type, NNSG2dImageProxy *proxy) {
    u32 converted;
    u32 mapping;
    DC_FlushRange(src->pRawData, src->szByte);
    mapping = *(volatile const u32 *)&src->mapingType;
    switch (type) {
    case NNS_G2D_VRAM_TYPE_3DMAIN:
        break;
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & 0xFFCFFFEF) | mapping;
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        *(volatile u32 *)0x04001000 = (*(volatile u32 *)0x04001000 & 0xFFCFFFEF) | mapping;
        break;
    default:
        break;
    }
    if (src->mapingType == 0) {
        switch (src->W) {
        case 1:
            converted = 0;
            break;
        case 2:
            converted = 1;
            break;
        case 4:
            converted = 2;
            break;
        case 8:
            converted = 3;
            break;
        case 16:
            converted = 4;
            break;
        case 32:
            converted = 5;
            break;
        default:
            converted = 0;
            break;
        }
        proxy->attr.sizeS = (GXTexSizeS)converted;
        switch (src->H) {
        case 1:
            converted = 0;
            break;
        case 2:
            converted = 1;
            break;
        case 4:
            converted = 2;
            break;
        case 8:
            converted = 3;
            break;
        case 16:
            converted = 4;
            break;
        case 32:
            converted = 5;
            break;
        default:
            converted = 0;
            break;
        }
        proxy->attr.sizeT = (GXTexSizeT)converted;
    } else {
        proxy->attr.sizeS = (GXTexSizeS)src->W;
        proxy->attr.sizeT = (GXTexSizeT)src->H;
    }
    proxy->attr.fmt = src->pixelFmt;
    proxy->attr.bExtendedPlt = 0;
    proxy->attr.plttUse = (GXTexPlttColor0)1;
    proxy->attr.mappingType = src->mapingType;
    NNS_G2dSetImageLocation(proxy, type, addr);
}

void NNS_G2dLoadPalette(const NNSG2dPaletteData *src, u32 addr, NNS_G2D_VRAM_TYPE type, NNSG2dImagePaletteProxy *proxy) {
    void *data = src->pRawData;
    u32 size = src->szByte;
    DC_FlushRange(data, size);
    switch (type) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        if (src->bExtendedPlt) {
            GX_BeginLoadOBJExtPltt();
            GX_LoadOBJExtPltt(data, addr, size);
            GX_EndLoadOBJExtPltt();
        } else {
            GX_LoadOBJPltt(data, addr, size);
        }
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        if (src->bExtendedPlt) {
            GXS_BeginLoadOBJExtPltt();
            GXS_LoadOBJExtPltt(data, addr, size);
            GXS_EndLoadOBJExtPltt();
        } else {
            GXS_LoadOBJPltt(data, addr, size);
        }
        break;
    case NNS_G2D_VRAM_TYPE_3DMAIN:
        GX_BeginLoadTexPltt();
        GX_LoadTexPltt(data, addr, size);
        GX_EndLoadTexPltt();
        break;
    default:
        break;
    }
    proxy->fmt = src->fmt;
    proxy->bExtendedPlt = src->bExtendedPlt;
    NNS_G2dSetImagePaletteLocation(proxy, type, addr);
}

void NNS_G2dLoadPaletteEx(const NNSG2dPaletteData *src, const NNSG2dPaletteCompressInfo *info, u32 addr, NNS_G2D_VRAM_TYPE type, NNSG2dImagePaletteProxy *proxy) {
    u32 unit;
    u16 count;
    u32 sourceOffset;
    void *data;
    u32 targetOffset;
    u16 i;
    if (src->fmt == 3) {
        unit = 0x20;
    } else {
        unit = 0x200;
    }
    count = info->numPalette;
    for (i = 0; i < count; i++) {
        sourceOffset = unit * i;
        targetOffset = unit * ((u16 *)info->pPlttIdxTbl)[i];
        data = src->pRawData;
        DC_FlushRange(data, src->szByte);
        switch (type) {
        case NNS_G2D_VRAM_TYPE_2DMAIN:
            if (src->bExtendedPlt) {
                GX_BeginLoadOBJExtPltt();
                GX_LoadOBJExtPltt((u8 *)data + sourceOffset, addr + targetOffset, unit);
                GX_EndLoadOBJExtPltt();
            } else {
                GX_LoadOBJPltt((u8 *)data + sourceOffset, addr + targetOffset, unit);
            }
            break;
        case NNS_G2D_VRAM_TYPE_2DSUB:
            if (src->bExtendedPlt) {
                GXS_BeginLoadOBJExtPltt();
                GXS_LoadOBJExtPltt((u8 *)data + sourceOffset, addr + targetOffset, unit);
                GXS_EndLoadOBJExtPltt();
            } else {
                GXS_LoadOBJPltt((u8 *)data + sourceOffset, addr + targetOffset, unit);
            }
            break;
        case NNS_G2D_VRAM_TYPE_3DMAIN:
            GX_BeginLoadTexPltt();
            GX_LoadTexPltt((u8 *)data + sourceOffset, addr + targetOffset, unit);
            GX_EndLoadTexPltt();
            break;
        default:
            break;
        }
    }
    proxy->fmt = src->fmt;
    proxy->bExtendedPlt = src->bExtendedPlt;
    NNS_G2dSetImagePaletteLocation(proxy, type, addr);
}
