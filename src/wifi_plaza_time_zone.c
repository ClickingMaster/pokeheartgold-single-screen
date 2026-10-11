#pragma force_active on
#include "global.h"

#include "gf_gfx_loader.h"
#include "location_gmm_dat.h"
#include "wifi_plaza_internal.h"

// Local time of a player's country / region: `time` shifted by the longitude difference to country 219 region 3.
WiFiPlazaTime ov45_02230F94(int country, u16 region, u32 time, enum HeapID heapID) {
    s32 baseLongitude;
    s32 longitude;
    int hour;
    NARC *narc = NARC_New(NARC_a_0_9_8, heapID);

    ov45_02231018(narc, heapID, 219, 3, &baseLongitude);
    ov45_02231018(narc, heapID, country, region, &longitude);
    hour = (u16)(baseLongitude - longitude) * 24 / 0xFFFF;
    if (hour < 0) {
        hour += 24;
    }
    if (hour >= 24) {
        hour %= 24;
    }
    {
        WiFiPlazaTime result = { 0 };
        result.field.hour = hour;
        ov45_0222D524(&result, (WiFiPlazaTime *)&time, &result);
        NARC_Delete(narc);
        return result;
    }
}

// Longitude (y angle) of a country (file 18 of the earth place NARC) or, for countries with regions, of a region.
void ov45_02231018(NARC *narc, enum HeapID heapID, int country, u16 region, s32 *longitude) {
    BOOL found = FALSE;
    u32 size;
    u32 regionSize;
    WiFiPlazaEarthCountry *countries;
    WiFiPlazaEarthRegion *regions;

    countries = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 18, FALSE, heapID, FALSE, &size);
    GF_ASSERT(country < (int)(size / sizeof(WiFiPlazaEarthCountry)));
    if (countries[country].kind != 2) {
        *longitude = countries[country].y;
        found = TRUE;
    }
    Heap_Free(countries);
    if (!found) {
        regions = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, LocationGmmDatGetEarthPlaceDatId(LocationGmmDatIndexGetByCountryMsgNo(country)), FALSE, heapID, FALSE, &regionSize);
        if (region < (int)(regionSize / sizeof(WiFiPlazaEarthRegion))) {
            *longitude = regions[region].y;
        } else {
            GF_AssertFail();
            *longitude = regions[0].y;
        }
        Heap_Free(regions);
    }
}
