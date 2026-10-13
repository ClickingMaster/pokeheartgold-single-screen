#include "overlay_06.h"

#include "global.h"

#include "palette.h"
#include "swap_3d_buffers.h"

const u16 *GetMoveTypePalette(int param_1) {
    if (param_1 != -1) {
        return sMoveTypePalettes[param_1];
    }
    return sMoveTypePaletteNone;
}

void LoadMoveTypePalette(PaletteData *palData, int param_2, int unused, PaletteBufferId bufferId, u32 paletteSlot) {
    const u16 *srcData = GetMoveTypePalette(param_2);

    PaletteData_LoadPalette(palData, srcData, bufferId, paletteSlot * 16, 32);
}

SPLEmitter *BattleParticleSystem_New(enum HeapID heapID) {
    void *particleHeap;
    SPLEmitter *pPVar1;
    Camera *pGVar2;

    particleHeap = Heap_Alloc(heapID, 0x4800);
    pPVar1 = ParticleSystem_New(BattleParticleSystem_AllocTexVram, BattleParticleSystem_AllocPlttVram, particleHeap, 0x4800, TRUE, heapID);
    pGVar2 = ParticleSystem_GetCamera(pPVar1);
    if (pGVar2 != NULL) {
        Camera_SetPerspectiveClippingPlane(1 * FX32_ONE, 900 * FX32_ONE, pGVar2);
    }
    return pPVar1;
}

SPLEmitter *BattleParticleSystem_NewSmallHeap(enum HeapID heapID) {
    void *particleHeap;
    SPLEmitter *pPVar1;
    Camera *pGVar2;

    particleHeap = Heap_Alloc(heapID, 0x4200);
    if (particleHeap == NULL) {
        return NULL;
    }
    pPVar1 = ParticleSystem_New(BattleParticleSystem_AllocTexVram, BattleParticleSystem_AllocPlttVram, particleHeap, 0x4200, TRUE, heapID);
    pGVar2 = ParticleSystem_GetCamera(pPVar1);
    if (pGVar2 != NULL) {
        Camera_SetPerspectiveClippingPlane(1 * FX32_ONE, 900 * FX32_ONE, pGVar2);
    }
    return pPVar1;
}

void BattleParticleSystem_Free(SPLEmitter *param_1) {
    void *pvVar1;

    pvVar1 = ParticleSystem_GetHeap(param_1);
    ParticleSystem_Free(param_1);
    Heap_Free(pvVar1);
}

void BattleParticleSystem_DrawAll(void) {
    int iVar1;

    Thunk_G3X_Reset();
    iVar1 = ParticleSystem_CountActive();
    if (iVar1) {
        iVar1 = ParticleSystem_DrawAll();
        if (iVar1 > 0) {
            Thunk_G3X_Reset();
            NNS_G2dSetupSoftwareSpriteCamera();
        }
        ParticleSystem_CalcAll();
    }
}

u32 BattleParticleSystem_AllocTexVram(u32 szByte, BOOL is4x4comp) {
    NNSGfdTexKey texKey;

    texKey = NNS_GfdDefaultFuncAllocTexVram(szByte, is4x4comp, 0);
    ParticleSystem_RegisterTexKey(texKey);
    return (texKey << 16) >> 13;
}

u32 BattleParticleSystem_AllocPlttVram(u32 szByte, BOOL is4pltt) {
    NNSGfdPlttKey plttKey;

    plttKey = NNS_GfdDefaultFuncAllocPlttVram(szByte, is4pltt, 0);
    ParticleSystem_RegisterPlttKey(plttKey);
    return (plttKey << 16) >> 13;
}
