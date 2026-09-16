#include "global.h"

#include "constants/heap.h"

#include "heap.h"

typedef struct UnkStruct_Ov01_021FAC44 {
    u8 *slices[4];
    u8 *buffers[4];
} UnkStruct_Ov01_021FAC44;

u8 ov01_02209B70[4][0xF000];

void ov01_021FACF8(int i, u32 size1, u32 size2, UnkStruct_Ov01_021FAC44 *s);
UnkStruct_Ov01_021FAC44 *ov01_021FAC44(int arg);

UnkStruct_Ov01_021FAC44 *ov01_021FAC44(int arg) {
    u8 i;
    UnkStruct_Ov01_021FAC44 *s = Heap_Alloc(HEAP_ID_FIELD1, 0x20);
    u32 size2;
    i = 0;
    do {
        s->slices[i] = ov01_02209B70[i];
        if (s->slices[i] == NULL) {
            return NULL;
        }
        if (arg) {
            s->buffers[i] = Heap_Alloc(HEAP_ID_FIELD1, 0x9000);
            if (s->buffers[i] == NULL) {
                return NULL;
            }
            size2 = 0x9000;
        } else {
            s->buffers[i] = NULL;
            size2 = 0;
        }
        ov01_021FACF8(i, 0xF000, size2, s);
        i++;
    } while (i < 4);
    return s;
}
