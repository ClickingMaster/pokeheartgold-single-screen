#include "to58_w11c_ov01_area_private.h"
extern const char *const ov01_02206450[5];

AreaLightManager *AreaLightManager_New(ModelAttributes *areaModelAttributes, const u32 archiveID) {
    AreaLightManager *areaLightManager;
    s32 sec;
    u32 i;
    AreaLightEntry *entry;

    GF_ASSERT(archiveID < 5);
    areaLightManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaLightManager));
    areaLightManager->modelAttributes = areaModelAttributes;
    areaLightManager->count = ov01_021EA3E0(ov01_02206450[archiveID], &areaLightManager->entries);
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
    ov01_021EA398(areaLightManager);
    return areaLightManager;
}
