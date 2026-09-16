#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224F6AC(FieldSystem *fieldSystem, int a1, int a2, void *work) {
    PartyExtraSub aprijuice;
    struct PokeathlonPerformanceStars stars;
    int code;
    u32 best;
    Party *party = SaveArray_Party_Get(*(SaveData **)((u8 *)fieldSystem + 0xc));
    int idx = GetIdxOfFirstAliveMonInParty_CrashIfNone(party);
    Party_GetMonAprijuiceModifiers(party, &aprijuice, idx);
    CalcMonPokeathlonStars(&stars, Party_GetMonByIndex(party, idx), (s8 *)&aprijuice, HEAP_ID_FIELD2);
    best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s0;
    code = 1;
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s4) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s4;
        code = 2;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s3) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s3;
        code = 4;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s1) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s1;
        code = 3;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s2) {
        code = 5;
    }
    *(u8 *)((u8 *)work + 0x16) = code;
}

WIP_LOCAL void ov02_0224F728(FieldSystem *fieldSystem, void *arg1) {
    switch (MapObject_GetFacingDirection(*(LocalMapObject **)((u8 *)fieldSystem + 0xe4))) {
    case 0:
        *(u8 *)((u8 *)arg1 + 0x17) = 3;
        break;
    case 1:
        *(u8 *)((u8 *)arg1 + 0x17) = 4;
        break;
    case 2:
        *(u8 *)((u8 *)arg1 + 0x17) = 2;
        break;
    case 3:
        *(u8 *)((u8 *)arg1 + 0x17) = 1;
        break;
    default:
        *(u8 *)((u8 *)arg1 + 0x17) = 0;
        break;
    }
}

WIP_LOCAL void ov02_0224F76C(int a0, void *out) {
    void *narc;
    if (a0 <= 0 || a0 > 0x1ed) {
        GF_AssertFail();
        return;
    }
    narc = AllocAtEndAndReadWholeNarcMemberByIdPair((NarcId)0xe9, 0, HEAP_ID_FIELD2);
    *(u8 *)((u8 *)out + 0xa) = ((u8 *)narc)[a0 - 1];
    Heap_Free(narc);
}

WIP_LOCAL int ov02_0224F79C(int a0) {
    switch (a0) {
    case 0:
        return 1;
    case 1:
        return 7;
    case 2:
        return 0xa;
    case 3:
        return 8;
    case 4:
        return 9;
    case 5:
        return 0xd;
    case 6:
        return 0xc;
    case 7:
        return 0xe;
    case 8:
        return 0x11;
    case 10:
        return 2;
    case 11:
        return 3;
    case 12:
        return 5;
    case 13:
        return 4;
    case 14:
        return 0xb;
    case 15:
        return 6;
    case 16:
        return 0xf;
    case 17:
        return 0x10;
    }
    GF_AssertFail();
    return 0;
}

WIP_LOCAL int ov02_0224F820(int a0) {
    switch (a0) {
    case 0:
        return 4;
    case 1:
        return 2;
    case 2:
        return 1;
    case 3:
        return 7;
    case 4:
        return 6;
    case 5:
        return 5;
    case 6:
        return 3;
    default:
        GF_AssertFail();
        return 8;
    }
}
