#include "oam_manager.h"

#include "global.h"

#include "sprite.h"

// NNS OAM manager instance (opaque here)
typedef struct OamInstance {
    u32 data[7];
} OamInstance;

typedef struct OamManager {
    OamInstance main;
    OamInstance sub;
    enum HeapID heapID;
} OamManager;

extern OamManager *gOamManager; // asm/oam_manager_data.s

// NitroSystem OAM manager (lib/include/nnsys/g2d_internal.h declares them with its own instance type)
BOOL NNS_G2dGetNewOamManagerInstance(OamInstance *, u16, u16, u16, u16, int);
void NNS_G2dApplyAndResetOamManagerBuffer(OamInstance *);
BOOL NNS_G2dEntryOamManagerOamWithAffineIdx(OamInstance *, const GXOamAttr *, u16);
u16 NNS_G2dEntryOamManagerAffine(OamInstance *, const MtxFx22 *);

// G2d renderer setup (unk_02025C44 group)
void GF_InitG2dRenderSurface(NNSG2dRenderSurface *, NNSG2dViewRect *, NNSG2dOamRegisterFunction, NNSG2dAffineRegisterFunction, NNSG2dRndCellCullingFunction, NNS_G2D_VRAM_TYPE, NNSG2dRendererInstance *);
BOOL GF_G2dIsCellVisible(const NNSG2dCellData *, const MtxFx32 *, const NNSG2dViewRect *);

void OamManager_CreateExact(int first, int count, int affineFirst, int affineCount, int subFirst, int subCount, int subAffineFirst, int subAffineCount, enum HeapID heapID);
BOOL OamManager_EntryMainOam(const GXOamAttr *oam, u16 affine, BOOL doubleAffine);
BOOL OamManager_EntrySubOam(const GXOamAttr *oam, u16 affine, BOOL doubleAffine);
u16 OamManager_EntryMainAffine(const MtxFx22 *matrix);
u16 OamManager_EntrySubAffine(const MtxFx22 *matrix);

void OamManager_Create(int first, int count, int affineFirst, int affineCount, int subFirst, int subCount, int subAffineFirst, int subAffineCount, enum HeapID heapID) {
    int adjustedFirst = first;
    int adjustedAffine;
    if (first < 4) {
        adjustedFirst = 4;
        if (count > 124) {
            first = adjustedFirst - first;
            count -= first;
        }
    }
    if (affineFirst < 1) {
        adjustedAffine = 1;
        if (affineCount > 30) {
            affineFirst = adjustedAffine - affineFirst;
            affineCount -= affineFirst;
        }
    } else {
        adjustedAffine = affineFirst;
    }
    OamManager_CreateExact(adjustedFirst, count, adjustedAffine, affineCount, subFirst, subCount, subAffineFirst, subAffineCount, heapID);
}

void OamManager_CreateExact(int first, int count, int affineFirst, int affineCount, int subFirst, int subCount, int subAffineFirst, int subAffineCount, enum HeapID heapID) {
    GF_ASSERT(gOamManager == NULL);
    gOamManager = Heap_Alloc(heapID, sizeof(OamManager));
    GF_ASSERT(gOamManager != NULL);
    gOamManager->heapID = heapID;
    if (!NNS_G2dGetNewOamManagerInstance(&gOamManager->main, (u16)first, (u16)count, (u16)affineFirst, (u16)affineCount, 0)) {
        GF_ASSERT(FALSE);
    }
    if (!NNS_G2dGetNewOamManagerInstance(&gOamManager->sub, (u16)subFirst, (u16)subCount, (u16)subAffineFirst, (u16)subAffineCount, 1)) {
        GF_ASSERT(FALSE);
    }
}

void OamManager_ApplyAndResetBuffers(void) {
    if (gOamManager) {
        NNS_G2dApplyAndResetOamManagerBuffer(&gOamManager->main);
        NNS_G2dApplyAndResetOamManagerBuffer(&gOamManager->sub);
    }
}

void OamManager_Free(void) {
    GF_ASSERT(gOamManager != NULL);
    thunk_ClearMainOAM(gOamManager->heapID);
    thunk_ClearSubOAM(gOamManager->heapID);
    Heap_Free(gOamManager);
    gOamManager = NULL;
}

void OamManager_InitRenderSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect, NNS_G2D_VRAM_TYPE type, NNSG2dRendererInstance *instance) {
    GF_ASSERT(gOamManager != NULL);
    if (type == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GF_InitG2dRenderSurface(surface, rect, OamManager_EntryMainOam, OamManager_EntryMainAffine, GF_G2dIsCellVisible, type, instance);
    } else {
        GF_InitG2dRenderSurface(surface, rect, OamManager_EntrySubOam, OamManager_EntrySubAffine, GF_G2dIsCellVisible, type, instance);
    }
}

void thunk_ClearMainOAM(enum HeapID heapID) {
    ClearMainOAM(heapID);
}

void thunk_ClearSubOAM(enum HeapID heapID) {
    ClearSubOAM(heapID);
}

BOOL OamManager_EntryMainOam(const GXOamAttr *oam, u16 affine, BOOL doubleAffine) {
    BOOL result = NNS_G2dEntryOamManagerOamWithAffineIdx(&gOamManager->main, oam, affine);
    GF_ASSERT(result);
    return result;
}

BOOL OamManager_EntrySubOam(const GXOamAttr *oam, u16 affine, BOOL doubleAffine) {
    BOOL result = NNS_G2dEntryOamManagerOamWithAffineIdx(&gOamManager->sub, oam, affine);
    GF_ASSERT(result);
    return result;
}

u16 OamManager_EntryMainAffine(const MtxFx22 *matrix) {
    u16 result = NNS_G2dEntryOamManagerAffine(&gOamManager->main, matrix);
    GF_ASSERT(result != 0xFFFE);
    return result;
}

u16 OamManager_EntrySubAffine(const MtxFx22 *matrix) {
    u16 result = NNS_G2dEntryOamManagerAffine(&gOamManager->sub, matrix);
    GF_ASSERT(result != 0xFFFE);
    return result;
}
