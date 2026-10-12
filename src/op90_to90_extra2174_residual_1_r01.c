#pragma force_active on
#include "ov01_21f4704_opus_partial_internal.h"
// @fn ov01_021F488C
void ov01_021F488C(MapLoadManager *mapLoadManager, int a1) {
    u32 i;
    MapLoadedChunk *chunk;

    for (i = 0; i < 4; i = (u8)(i + 1)) {
        chunk = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapLoadedChunk));
        MI_CpuFill8(chunk, 0, sizeof(MapLoadedChunk));
        mapLoadManager->maps[i] = chunk;
        mapLoadManager->maps[i]->unk864 = 0;
        MapLoadBuffers_GetPrimary(i, mapLoadManager->slots[0].unk00, &mapLoadManager->maps[i]->unk854);
        MapLoadBuffers_GetSecondary(i, mapLoadManager->slots[0].unk00, &mapLoadManager->maps[i]->unk858);
        mapLoadManager->maps[i]->unk868 = MapPropManager_New(HEAP_ID_FIELD1, mapLoadManager->unkBC);
        mapLoadManager->maps[i]->unk860 = -1;
        MI_CpuFillFast(mapLoadManager->maps[i], -1, 0x800);
        if (a1 != 0) {
            mapLoadManager->maps[i]->unkA70 = Heap_Alloc(HEAP_ID_FIELD1, 0x400);
            MI_CpuFill8(mapLoadManager->maps[i]->unkA70, 0xFF, 0x400);
        }
        MI_CpuClear32(&mapLoadManager->maps[i]->unk86C, sizeof(MapLoadSoundplate));
    }
}
