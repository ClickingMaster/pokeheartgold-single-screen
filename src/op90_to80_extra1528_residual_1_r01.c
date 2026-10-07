#pragma force_active on
#include "global.h"
#include "assert.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "location_gmm_dat.h"
#include "filesystem.h"

typedef struct Ov45CountryEntry {
    u16 type;
    u16 unk2;
    s16 value;
} Ov45CountryEntry;

typedef struct Ov45RegionEntry {
    s16 unk0;
    s16 value;
} Ov45RegionEntry;

typedef struct Ov45Param {
    u32 value;
} Ov45Param;

typedef union Ov45Result {
    u32 raw;
    struct {
        u8 hour;
        u8 unk1[3];
    } fields;
} Ov45Result;

void ov45_0222D524(Ov45Result *result, Ov45Param *param);
Ov45Result ov45_02230F94(int country, u16 region, Ov45Param param, enum HeapID heapID);
void ov45_02231018(NARC *narc, enum HeapID heapID, int country, u16 region, s32 *out);

// @fn ov45_02230F94
Ov45Result ov45_02230F94(int country, u16 region, Ov45Param param, enum HeapID heapID) {
    s32 base;
    s32 local;
    int hour;
    NARC *narc = NARC_New(NARC_a_0_9_8, heapID);

    ov45_02231018(narc, heapID, 219, 3, &base);
    ov45_02231018(narc, heapID, country, region, &local);
    hour = (u16)(base - local) * 24 / 0xFFFF;
    if (hour < 0) {
        hour += 24;
    }
    if (hour >= 24) {
        hour %= 24;
    }
    {
    Ov45Result result = {0};
    result.fields.hour = hour;
    ov45_0222D524(&result, &param);
    NARC_Delete(narc);
    return result;
    }
}

// @fn ov45_02231018
void ov45_02231018(NARC *narc, enum HeapID heapID, int country, u16 region, s32 *out) {
    BOOL found = FALSE;
    u32 size;
    u32 regionSize;
    Ov45CountryEntry *countries;
    Ov45RegionEntry *regions;

    countries = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 18, FALSE, heapID, FALSE, &size);
    GF_ASSERT(country < (int)(size / sizeof(Ov45CountryEntry)));
    if (countries[country].type != 2) {
        *out = countries[country].value;
        found = TRUE;
    }
    Heap_Free(countries);
    if (!found) {
        regions = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, LocationGmmDatGetEarthPlaceDatId(LocationGmmDatIndexGetByCountryMsgNo(country)), FALSE, heapID, FALSE, &regionSize);
        if (region < (int)(regionSize / sizeof(Ov45RegionEntry))) {
            *out = regions[region].value;
        } else {
            GF_AssertFail();
            *out = regions[0].value;
        }
        Heap_Free(regions);
    }
}
