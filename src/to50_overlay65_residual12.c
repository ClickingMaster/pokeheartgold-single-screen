#include "to50_overlay65_residual12_private.h"

int ov65_0221F4D4(void *p) {
    u32 input = TouchscreenListMenu_HandleInput(PTR(p, 0x36c8));
    switch (input) {
    case 0:
        FillBgTilemapRect(PTR(p, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90((u8 *)p + 0x594, 0xf, 1, U32(p, 0x190), PTR(p, 0x184));
        FN(p, 0x2220) = ov65_0221E050;
        ov65_0221F4B0(p);
        ListMenuItems_Delete(PTR(p, 0x668));
        return 3;
    case 1:
    case 0xfffffffe:
        FillBgTilemapRect(PTR(p, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90((u8 *)p + 0x594, 0xf, 1, U32(p, 0x190), PTR(p, 0x184));
        ov65_0221F4B0(p);
        ListMenuItems_Delete(PTR(p, 0x668));
        FN(p, 0x2220) = ov65_0221E050;
        break;
    }
    return 0;
}

void ov65_0221F5A0(void *partyA, void *partyB, u32 indexA, u32 indexB, void *ctx) {
    void *copyA;
    void *copyB;
    void *vars;
    u8 data[2];
    copyA = AllocMonZeroed(0x1a);
    copyB = AllocMonZeroed(0x1a);
    CopyPokemonToPokemon(Party_GetMonByIndex(partyA, indexA), copyA);
    CopyPokemonToPokemon(Party_GetMonByIndex(partyB, indexB), copyB);
    if (GetMonData(copyB, 5, NULL) == 0x1ed) {
        if (GetMonData(copyB, 0x6e, NULL) != 0 || (GetMonData(copyB, 0x99, NULL) == 0x56 && GetMonData(copyB, 0x6e, NULL) == 0)) {
            vars = Save_VarsFlags_Get(PTR(ctx, 0x10));
            if (Save_VarsFlags_GetVar404C(vars) == 0) {
                Save_VarsFlags_SetVar404C(vars, 1);
            }
        }
    }
    SetMonData(copyB, 0x6f, NULL);
    if (GetMonData(copyB, 0x4c, NULL) == 0) {
        data[1] = 0x46;
        data[0] = 0;
        SetMonData(copyB, 9, &data[1]);
        SetMonData(copyB, 0xbb, &data[0]);
    }
    MonSetTrainerMemo(copyB, sub_02034818(sub_0203769C()), 5, 0, 0xb);
    Pokemon_RemoveCapsule(copyB);
    CopyPokemonToPokemon(copyA, PTR(ctx, 0x38));
    CopyPokemonToPokemon(copyB, PTR(ctx, 0x3c));
    PlayerProfile_Copy(sub_02034818(sub_0203769C() ^ 1), PTR(ctx, 0x34));
    U32(ctx, 0x28) = indexA;
    if (Party_HasMon(partyA, 0x1b9) == 0) {
        Chatot_Invalidate(Save_Chatot_Get(PTR(ctx, 0x10)));
    }
    UpdatePokedexWithReceivedSpecies(PTR(ctx, 0x10), copyB);
    CopyPokemonToPokemon(copyB, Party_GetMonByIndex(partyA, indexA));
    CopyPokemonToPokemon(copyA, Party_GetMonByIndex(partyB, indexB));
    Party_ResetMonAprijuiceModifiers(partyA, indexA);
    Party_ResetMonAprijuiceModifiers(partyB, indexB);
    GameStats_Inc(PTR(ctx, 0x1c), 0x14);
    Heap_Free(copyA);
    Heap_Free(copyB);
}
