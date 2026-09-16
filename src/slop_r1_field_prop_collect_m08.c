#include "slop_r1_field_prop_collect_m08.h"

#include "heap.h"
#include "map_matrix.h"

int ov01_021F652C(MapLoadManager *mapLoadManager, u8 index);
BOOL sub_02054A9C(MapProp *mapProp, fx32 *rect, VecFx32 *offset);
void sub_02054DC8(int index, int width, VecFx32 *offset);

int *sub_02054D10(FieldSystem *fieldSystem, u32 heapId, int capacity, fx32 *rect, u32 fill) {
    u8 outer;
    int *array = Heap_AllocAtEnd((enum HeapID)heapId, capacity * sizeof(int));
    int i;
    u8 count;

    for (i = 0; i < capacity; i++) {
        array[i] = fill;
    }
    count = 0;
    for (outer = 0; outer < 4; outer++) {
        MapPropManager *manager;
        ov01_021F630C(outer, fieldSystem->mapLoadManager, &manager);
        if (manager != NULL) {
            int index = ov01_021F652C(fieldSystem->mapLoadManager, outer);
            VecFx32 offset;
            sub_02054DC8(index, MapMatrix_GetWidth(fieldSystem->mapMatrix), &offset);
            u8 inner;
            for (inner = 0; inner < 32; inner++) {
                MapProp *prop = MapPropManager_GetMapPropByIndex(manager, inner);
                if (sub_02054A9C(prop, rect, &offset)) {
                    int model = MapProp_GetBuildModel(prop);
                    if (model != 0) {
                        if ((u32)count >= (u32)capacity) {
                            GF_AssertFail();
                            return array;
                        }
                        array[count++] = model;
                    }
                }
            }
        }
    }
    return array;
}
