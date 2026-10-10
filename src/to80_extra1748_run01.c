#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 key;
    u16 size, pad, flags, pad2;
    u32 offset;
} TexInfo;

typedef struct {
    u8 pad[8];
    TexInfo normal, compressed;
    u32 indexOffset;
} Texture;

extern void GX_BeginLoadTex(void);
extern void GX_EndLoadTex(void);
extern void GX_LoadTex(const void *, u32, u32);
void NNS_G3dTexLoad(NNSG3dResTex *, int);
#pragma force_active on

void NNS_G3dTexLoad(NNSG3dResTex *res, int beginEnd) {
    Texture *texture = (Texture *)res;
    if (beginEnd) {
        GX_BeginLoadTex();
    }
    {
        u32 size = texture->normal.size << 3;
        if (size) {
            GX_LoadTex((u8 *)texture + texture->normal.offset, (texture->normal.key & 65535) << 3, size);
            texture->normal.flags |= 1;
        }
    }
    u32 indexOffset;
    u32 compressedSize = texture->compressed.size << 3;
    u32 address;
    if (compressedSize) {
        address = (texture->compressed.key & 65535) << 3;
        indexOffset = texture->indexOffset;
        GX_LoadTex((u8 *)texture + texture->compressed.offset, address, compressedSize);
        GX_LoadTex((u8 *)texture + indexOffset, ((address & 0x1ffff) >> 1) + 0x20000 + ((address & 0x40000) >> 2), compressedSize >> 1);
        texture->compressed.flags |= 1;
    }
    if (beginEnd) {
        GX_EndLoadTex();
    }
}
