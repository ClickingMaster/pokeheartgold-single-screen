#include "map_section.h"

#include "global.h"

#include "constants/map_sections.h"

#include "assert.h"

// first mapsec of each MapsecType
static const u16 sMapsecTypeStarts[MAPSECTYPE_MAX] = {
    MAPSEC_MYSTERY_ZONE,
    METLOC_DAY_CARE_COUPLE,
    METLOC_LOVELY_PLACE,
};

int Mapsec_GetType(int mapsec) {
    int i;
    for (i = 0; i < MAPSECTYPE_MAX - 1; i++) {
        if ((u32)mapsec < sMapsecTypeStarts[i + 1]) {
            return i;
        }
    }
    return i;
}

int Mapsec_GetIndexInType(int mapsec) {
    return mapsec - sMapsecTypeStarts[Mapsec_GetType(mapsec)];
}

int Mapsec_FromTypeAndIndex(MapsecType type, int offset) {
    GF_ASSERT(type < MAPSECTYPE_MAX);
    return sMapsecTypeStarts[type] + offset;
}

BOOL LocationIsDiamondPearlCompatible(mapsec_t mapsec) {
    if ((mapsec >= MAPSEC_TWINLEAF_TOWN && mapsec <= MAPSEC_BATTLE_PARK) || (mapsec >= METLOC_DAY_CARE_COUPLE && mapsec <= METLOC_RILEY) || (mapsec >= METLOC_LOVELY_PLACE && mapsec <= METLOC_CONCERT_EVENT)) {
        return TRUE;
    }
    return FALSE;
}
