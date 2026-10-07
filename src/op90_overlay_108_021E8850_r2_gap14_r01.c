#pragma force_active on
#include "global.h"
#include "heap.h"
#include "unk_02097268.h"
int ov108_021EA700(int, int);
extern u8 ov108_021EAD28[][4];
typedef struct {
    u8 n;
    u8 pad[3];
    u8 *items;
} Desc;
extern Desc ov108_021EAD08[];
typedef struct {
    u8 id;
    u8 value;
    SafariObjectConfig config;
} Record;
Record *ov108_021EA63C(u32 seed, int n, u8 gender, u8 *out, enum HeapID heap);
// @fn ov108_021EA63C
Record *ov108_021EA63C(u32 seed, int n, u8 gender, u8 *out, enum HeapID heap) {
    u8 count = 0, index;
    u32 mask = 0;
    int i, j;
    Record *records;
    u8 *types = ov108_021EAD28[seed % 10];
    for (i = 0; i < n; i++) {
        Desc *desc = &ov108_021EAD08[types[i]];
        for (j = 0; j < desc->n; j++) {
            mask |= 1 << desc->items[j];
            count++;
        }
    }
    records = Heap_Alloc(heap, count * 5);
    MI_CpuFill8(records, 0, count * 5);
    index = 0;
    for (i = 0; i < 24; i++) {
        if (mask & 1) {
            Record *r;
            r = &records[index];
            r->id = i;
            GetSafariObjectConfig(&r->config, i, gender);
            r->value = ov108_021EA700(r->config.width, r->config.height);
            index++;
        }
        mask >>= 1;
    }
    *out = index;
    return records;
}
