#include "frontier_stat_ids.h"

#include "frontier_save_access.h"

// The frontier stat ids of each facility record, by challenge type (and level mode).
// BUG: the switches have no default: an unknown challenge type returns an undefined stat id.

u32 BattleFactory_GetWinStreakStat(u32 levelMode, u32 type) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0xB;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x13;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x1B;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x73;
        break;
    }

    return base + levelMode * 4;
}

u32 BattleFactory_GetBestStreakStat(u32 levelMode, u32 type) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0xA;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x12;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x1A;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x72;
        break;
    }

    return base + levelMode * 4;
}

u32 BattleFactory_GetTradeCountStat(u32 levelMode, u32 type) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0xD;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x15;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x1D;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x75;
        break;
    }

    return base + levelMode * 4;
}

u32 BattleFactory_GetBestTradeCountStat(u32 levelMode, u32 type) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0xC;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x14;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x1C;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x74;
        break;
    }

    return base + levelMode * 4;
}

u32 BattleHall_GetTypeRankStat(u32 type, u32 cell) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0x25;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x31;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x3D;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x7D;
        break;
    }

    return base + cell / 2;
}

u32 BattleHall_GetWinStreakStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x23;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x2F;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x3B;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x7B;
        break;
    }

    return ret;
}

u32 BattleHall_GetBestStreakStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x22;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x2E;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x3A;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x7A;
        break;
    }

    return ret;
}

u32 BattleHall_GetSpeciesStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x24;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x30;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x3C;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x7C;
        break;
    }

    return ret;
}

u32 BattleHall_GetSpeciesRecordsType(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 1;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 2;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 2;
        GF_AssertFail();
        break;
    }

    return ret;
}

u32 BattleCastle_GetRankStat(u32 type, u32 index) {
    u32 base;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        base = 0x4B;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        base = 0x53;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        base = 0x5B;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        base = 0x8B;
        break;
    }

    return base + index;
}

u32 BattleCastle_GetWinStreakStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x47;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x4F;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x57;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x87;
        break;
    }

    return ret;
}

u32 BattleCastle_GetBestStreakStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x46;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x4E;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x56;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x86;
        break;
    }

    return ret;
}

u32 BattleCastle_GetPointsStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x48;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x50;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x58;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x88;
        break;
    }

    return ret;
}

u32 BattleCastle_GetSpentPointsStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x49;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x51;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x59;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x89;
        break;
    }

    return ret;
}

u32 BattleCastle_GetBestStreakPointsStat(u32 type) {
    u32 ret;

    switch (type) {
    case FRONTIER_CHALLENGE_SINGLE:
        ret = 0x4A;
        break;
    case FRONTIER_CHALLENGE_DOUBLE:
        ret = 0x52;
        break;
    case FRONTIER_CHALLENGE_MULTI:
        ret = 0x5A;
        break;
    case FRONTIER_CHALLENGE_LINK_MULTI:
        ret = 0x8A;
        break;
    }

    return ret;
}
