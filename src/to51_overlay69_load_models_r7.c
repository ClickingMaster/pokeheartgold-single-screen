#include "global.h"

#include "filesystem.h"
#include "gf_3d_render.h"

#define PTR(w, o) (*(void **)((u8 *)(w) + (o)))
#define U32(w, o) (*(u32 *)((u8 *)(w) + (o)))

void ov69_021E6E88(u8 *work, NARC *narc);

void ov69_021E6E88(u8 *work, NARC *narc) {
    PTR(work, 0xC0DC) = NARC_AllocAndReadWholeMember(narc, 0, (enum HeapID)U32(work, 0));
    GF3dRender_InitObjFromHeader((NNSG3dRenderObj *)(work + 0xC084), (NNSG3dResMdl **)(work + 0xC0D8), (NNSG3dResFileHeader **)(work + 0xC0DC));
    PTR(work, 0xC2A4) = NARC_AllocAndReadWholeMember(narc, 1, (enum HeapID)U32(work, 0));
    GF3dRender_InitObjFromHeader((NNSG3dRenderObj *)(work + 0xC1DC), (NNSG3dResMdl **)(work + 0xC290), (NNSG3dResFileHeader **)(work + 0xC2A4));
    PTR(work, 0xC2A8) = NARC_AllocAndReadWholeMember(narc, 2, (enum HeapID)U32(work, 0));
    GF3dRender_InitObjFromHeader((NNSG3dRenderObj *)(work + 0xC230), (NNSG3dResMdl **)(work + 0xC294), (NNSG3dResFileHeader **)(work + 0xC2A8));
    PTR(work, 0xC29C) = NARC_AllocAndReadWholeMember(narc, 3, (enum HeapID)U32(work, 0));
    GF3dRender_InitObjFromHeader((NNSG3dRenderObj *)(work + 0xC134), (NNSG3dResMdl **)(work + 0xC288), (NNSG3dResFileHeader **)(work + 0xC29C));
    PTR(work, 0xC2A0) = NARC_AllocAndReadWholeMember(narc, 4, (enum HeapID)U32(work, 0));
    GF3dRender_InitObjFromHeader((NNSG3dRenderObj *)(work + 0xC188), (NNSG3dResMdl **)(work + 0xC28C), (NNSG3dResFileHeader **)(work + 0xC2A0));
}
