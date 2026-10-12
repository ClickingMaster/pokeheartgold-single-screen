#include "field/area_light.h"

#include "global.h"

#include "field/model_attributes.h"

#include "ascii_util.h"
#include "gf_rtc.h"
#include "heap.h"
#include "system.h"

#define AREA_LIGHT_FILE_COUNT 5

// One time slot of an area light file (data/*light.txt): until endTime (seconds since midnight / 2), the four
// lights (enabled in lightMask) and the material colors to use.
struct AreaLightEntry {
    u32 endTime;          // 0x00
    u8 lightMask;         // 0x04
    u8 padding05;         // 0x05
    GXRgb lightColors[4]; // 0x06
    s16 lightDirs[4][3];  // 0x0E
    GXRgb diffuse;        // 0x26
    GXRgb ambient;        // 0x28
    GXRgb specular;       // 0x2A
    GXRgb emission;       // 0x2C
    u16 padding2E;        // 0x2E
};

struct AreaLightManager {
    u32 count;                        // 0x00
    AreaLightEntry *entries;          // 0x04
    u32 activeIndex;                  // 0x08
    ModelAttributes *modelAttributes; // 0x0C
    u32 unk10;                        // 0x10
};

// model_attributes_helpers_1.c (no header declares them yet)
void ov01_021EAAB8(ModelAttributes *ma, int idx, s16 x, s16 y, s16 z);
void ov01_021EAAE0(ModelAttributes *ma, int idx, u16 color);
void ov01_021EAAF4(ModelAttributes *ma, u16 v, int a, int b);
void ov01_021EAB08(ModelAttributes *ma, u16 v, int a);
void ov01_021EAB1C(ModelAttributes *ma, u16 v, int a, int b);
void ov01_021EAB30(ModelAttributes *ma, u16 v, int a);

extern const char *const gAreaLightFilePaths[AREA_LIGHT_FILE_COUNT];

AreaLightManager *AreaLightManager_New(ModelAttributes *areaModelAttributes, const u32 archiveID) {
    AreaLightManager *areaLightManager;
    s32 sec;
    u32 i;
    AreaLightEntry *entry;

    GF_ASSERT(archiveID < AREA_LIGHT_FILE_COUNT);
    areaLightManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaLightManager));
    areaLightManager->modelAttributes = areaModelAttributes;
    areaLightManager->count = AreaLight_LoadEntries(gAreaLightFilePaths[archiveID], &areaLightManager->entries);
    areaLightManager->activeIndex = 0;
    sec = GF_RTC_TimeToSec() / 2;
    i = 0;
    if (i < areaLightManager->count) {
        entry = areaLightManager->entries;
        do {
            if (entry->endTime > (u32)sec) {
                areaLightManager->activeIndex = i;
                break;
            }
            i++;
            entry++;
        } while (i < areaLightManager->count);
    }
    areaLightManager->unk10 = 1;
    AreaLightManager_ApplyActiveEntry(areaLightManager);
    return areaLightManager;
}

void AreaLightManager_Free(AreaLightManager **areaLightManager) {
    GF_ASSERT(areaLightManager != NULL);
    AreaLight_FreeEntries(&(*areaLightManager)->entries);
    Heap_FreeExplicit(HEAP_ID_FIELD1, *areaLightManager);
    *areaLightManager = NULL;
}

void AreaLightManager_UpdateActiveTemplate(AreaLightManager *areaLightManager) {
    s32 sec;
    u32 prevEndTime;
    s32 prev;

    GF_ASSERT(areaLightManager != NULL);
    sec = GF_RTC_TimeToSec() / 2;
    if (areaLightManager->count <= 1) {
        return;
    }
    prev = areaLightManager->activeIndex - 1;
    if (prev >= 0) {
        prevEndTime = areaLightManager->entries[prev].endTime;
    } else {
        prevEndTime = 0;
    }
    if (sec >= (s32)areaLightManager->entries[areaLightManager->activeIndex].endTime || sec < (s32)prevEndTime) {
        areaLightManager->activeIndex++;
        if (areaLightManager->activeIndex >= areaLightManager->count) {
            areaLightManager->activeIndex = 0;
        }
        if (areaLightManager->unk10 != 0) {
            AreaLightManager_ApplyActiveEntry(areaLightManager);
        }
    }
}

void AreaLight_ApplyEntry(AreaLightEntry *entry, ModelAttributes *modelAttributes) {
    int i;
    const s16 *dir;
    const u16 *color;

    // dir[7..9] = lightDirs[i], color[3] = lightColors[i]
    i = 0;
    dir = (const s16 *)entry;
    color = (const u16 *)entry;
    for (; i < 4; i++, dir += 3, color++) {
        if (entry->lightMask & (1 << i)) {
            ov01_021EAAB8(modelAttributes, i, dir[7], dir[8], dir[9]);
            ov01_021EAAE0(modelAttributes, i, color[3]);
        } else {
            ov01_021EAAB8(modelAttributes, i, 0, 0, 0);
            ov01_021EAAE0(modelAttributes, i, 0);
        }
    }
    ov01_021EAAF4(modelAttributes, entry->diffuse, 0, 0);
    ov01_021EAB08(modelAttributes, entry->ambient, 1);
    ov01_021EAB1C(modelAttributes, entry->specular, 0, 0);
    ov01_021EAB30(modelAttributes, entry->emission, 1);
}

void AreaLightManager_ApplyActiveEntry(AreaLightManager *areaLightManager) {
    AreaLight_ApplyEntry(&areaLightManager->entries[areaLightManager->activeIndex], areaLightManager->modelAttributes);
}

void ov01_021EA3B0(NNSG3dResMdl *model) {
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_DIFFUSE);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_AMBIENT);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_SPECULAR);
    NNSi_G3dModifyMatFlag(model, FALSE, NNS_G3D_MATFLAG_EMISSION);
}

// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
// Each entry is 10 lines: "endTime,...", 4 light lines, 4 material color lines and one more; the file ends with "EOF".
int AreaLight_LoadEntries(const char *path, AreaLightEntry **entriesOut) {
    s8 line[256];
    s8 field[256];
    const s8 *p;
    long i;
    const s8 *data;
    AreaLightEntry *entry;
    int size;
    int count;
    int j;

    data = Sys_AllocAndReadFile(HEAP_ID_FIELD1, path);
    p = data;
    count = 0;
    do {
        p = Ascii_GetDelim(p, line, '\r');
        if (line[0] != 'E' || line[1] != 'O' || line[2] != 'F') {
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            p = Ascii_GetDelim(p, line, '\r');
            count++;
        }
    } while (line[0] != 'E' || line[1] != 'O' || line[2] != 'F');
    size = count * sizeof(AreaLightEntry);
    *entriesOut = Heap_Alloc(HEAP_ID_FIELD1, size);
    MI_CpuFill8(*entriesOut, 0, size);
    p = data;
    for (i = 0; i < count; i++) {
        entry = &(*entriesOut)[i];
        p = Ascii_GetDelim(p, line, '\r');
        Ascii_GetDelim(line, field, ',');
        entry->endTime = Ascii_StrToL(field);
        for (j = 0; j < 4; j++) {
            p = AreaLight_ParseLight(p, &entry->lightColors[j], entry->lightDirs[j]);
            if (entry->lightColors[j] != 0xFFFF) {
                entry->lightMask |= 1 << j;
            } else {
                entry->lightColors[j] = 0;
            }
        }
        p = AreaLight_ParseColor(p, &entry->diffuse);
        p = AreaLight_ParseColor(p, &entry->ambient);
        p = AreaLight_ParseColor(p, &entry->specular);
        p = AreaLight_ParseColor(p, &entry->emission);
        p = Ascii_GetDelim(p, line, '\r');
    }
    Heap_FreeExplicit(HEAP_ID_FIELD1, (void *)data);
    return count;
}
#pragma force_active reset

void AreaLight_FreeEntries(AreaLightEntry **entries) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, *entries);
    *entries = NULL;
}

// "enabled,r,g,b,x,y,z": color 0xFFFF when the light is off; the direction is clamped to [-1, 1] (fx16).
const s8 *AreaLight_ParseLight(const s8 *src, u16 *colorOut, s16 *dirOut) {
    s8 line[256];
    s8 field[256];
    s32 dir[3];
    u16 rgb[3];
    const s8 *p;
    int i;

    src = Ascii_GetDelim(src, line, '\r');
    p = Ascii_GetDelim(line, field, ',');
    if (Ascii_StrToL(field) == 1) {
        for (i = 0; i < 3; i++) {
            p = Ascii_GetDelim(p, field, ',');
            rgb[i] = Ascii_StrToL(field);
        }
        *colorOut = rgb[0] | (rgb[1] << 5) | (rgb[2] << 10);
        for (i = 0; i < 3; i++) {
            p = Ascii_GetDelim(p, field, ',');
            dir[i] = Ascii_StrToL(field);
        }
        dirOut[0] = dir[0];
        dirOut[1] = dir[1];
        dirOut[2] = dir[2];
        if (dirOut[0] > 0x1000) {
            dirOut[0] = 0x1000;
        }
        if (dirOut[0] < -0x1000) {
            dirOut[0] = -0x1000;
        }
        if (dirOut[1] > 0x1000) {
            dirOut[1] = 0x1000;
        }
        if (dirOut[1] < -0x1000) {
            dirOut[1] = -0x1000;
        }
        if (dirOut[2] > 0x1000) {
            dirOut[2] = 0x1000;
        }
        if (dirOut[2] < -0x1000) {
            dirOut[2] = -0x1000;
        }
    } else {
        *colorOut = 0xFFFF;
    }
    return src;
}

// "r,g,b"
const s8 *AreaLight_ParseColor(const s8 *src, u16 *dest) {
    s8 line[256];
    s8 field[256];
    u16 rgb[3];
    const s8 *p;
    int i;

    src = Ascii_GetDelim(src, line, '\r');
    p = line;
    for (i = 0; i < 3; i++) {
        p = Ascii_GetDelim(p, field, ',');
        rgb[i] = Ascii_StrToL(field);
    }
    *dest = rgb[0] | (rgb[1] << 5) | (rgb[2] << 10);
    return src;
}

void LoadAreaOrDungeonLightTxt(u32 archiveID, ModelAttributes *modelAttributes) {
    AreaLightEntry *entries;
    s32 count;
    s32 sec;
    s32 i;
    u32 index;
    AreaLightEntry *entry;

    GF_ASSERT(archiveID < AREA_LIGHT_FILE_COUNT);
    count = AreaLight_LoadEntries(gAreaLightFilePaths[archiveID], &entries);
    sec = GF_RTC_TimeToSec() / 2;
    i = 0;
    if (count > 0) {
        entry = entries;
        do {
            if (entry->endTime > (u32)sec) {
                index = i;
                break;
            }
            i++;
            entry++;
        } while (i < count);
    }
    AreaLight_ApplyEntry(&entries[index], modelAttributes);
    AreaLight_FreeEntries(&entries);
}
