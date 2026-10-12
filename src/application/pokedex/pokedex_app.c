#include <dsprot.h>

#include "application/pokedex/pokedex_internal.h"

#include "dex_mon_measures.h"
#include "overlay_18.h"
#include "sound_system.h"

FS_EXTERN_OVERLAY(ds_protect);

void Pokedex_OnEmulatorDetected(void);
void Pokedex_OnFlashcartDetected(void);
void Pokedex_OnDummyCheckFailed(void);

BOOL Pokedex_Init(OverlayManager *man, int *state) {
    PokedexAppData *appData;

    Heap_Create(HEAP_ID_3, HEAP_ID_POKEDEX_APP, 0x61000);
    appData = OverlayManager_CreateAndGetData(man, sizeof(PokedexAppData), HEAP_ID_POKEDEX_APP);
    MI_CpuClear8(appData, sizeof(PokedexAppData));
    appData->args = OverlayManager_GetArgs(man);
    appData->nextSeq = 5;
    appData->dexMode = UnkStruct_02092BB8_GetUnk2(appData->args->unk_08);
    if (Pokedex_GetNatDexFlag(appData->args->pokedex)) {
        appData->nationalDex = TRUE;
        if (appData->dexMode == 2) {
            appData->dexMode = 1;
        }
    } else {
        appData->nationalDex = FALSE;
        if (appData->dexMode == 2) {
            appData->dexMode = 0;
        }
    }
    if (Pokedex_CheckMonCaughtFlag(appData->args->pokedex, SPECIES_GIRATINA) == TRUE) {
        SetDexBanksByGiratinaForm(Pokedex_GetSeenFormByIdx(appData->args->pokedex, SPECIES_GIRATINA, 0));
    } else {
        SetDexBanksByGiratinaForm(GIRATINA_ALTERED);
    }
    GF_SndHandleSetPlayerVolume(1, 42);
    appData->language = 2;
    return TRUE;
}

BOOL Pokedex_Main(OverlayManager *man, int *state) {
    PokedexAppData *appData = OverlayManager_GetData(man);

    if (!PokedexApp_RunMainSeq(appData, state)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL Pokedex_Exit(OverlayManager *man, int *state) {
    PokedexAppData *appData = OverlayManager_GetData(man);

    FS_LoadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
    if (DSProt_DetectEmulator(Pokedex_OnEmulatorDetected)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    UnkStruct_02092BB8_Set(appData->args->unk_08, PokedexApp_GetCursorSpecies(appData), appData->dexMode);
    if (DSProt_DetectFlashcart(Pokedex_OnFlashcartDetected)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_POKEDEX_APP);
    GF_SndHandleSetPlayerVolume(1, 127);
    sub_02004B10();
    if (!DSProt_DetectNotDummy(Pokedex_OnDummyCheckFailed)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    FS_UnloadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
    return TRUE;
}

void Pokedex_OnEmulatorDetected(void) {
    Heap_AllocAtEnd(HEAP_ID_3, 1000);
}

void Pokedex_OnFlashcartDetected(void) {
    Heap_AllocAtEnd(HEAP_ID_3, 1000);
}

void Pokedex_OnDummyCheckFailed(void) {
}
