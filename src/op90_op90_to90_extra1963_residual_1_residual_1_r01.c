#pragma force_active on
#include "global.h"
typedef struct { u8 raw[0x28]; } Block;
typedef struct {
    u16 x0[3];
    u8 x6;
    u8 x7;
    u8 x8;
    u8 x9;
    u16 xa[3];
    u16 x10[3];
    u8 x16[0x24 - 0x16];
} Payload;
typedef struct {
    u32 flag;
    u16 arr[4][3];
} Status;
typedef struct {
    u16 xe0;
    u8 pad[6];
    u8 xe8;
    u8 rest[0x6c - 9];
} Entry;
typedef struct {
    u8 pad[0xe0];
    Entry entries[3];
    u8 pad2[0x230 - 0xe0 - 3 * 0x6c];
    u32 x230;
} Heap;
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void *PokeathlonCourse_GetDataCopyArea(void *course);
int PokeathlonCourse_GetParticipantCount(void *course);
void *ov96_021E8A20(void *block);
BOOL ov96_021E5F24(void *course);
void ov96_021FB400(void *course);
void ov96_021FB0F4(void *course);
void ov96_021FAF1C(void *course);
// @fn ov96_021FAF1C
void ov96_021FAF1C(void *course) {
    Heap *heap = PokeathlonCourse_GetHeapAllocPtr4(course);
    Block *blocks = PokeathlonCourse_GetDataCopyArea(course);
    Payload *payload = ov96_021E8A20(blocks);
    int i;
    for (i = 0; i < 3; i++) {
        payload->x0[i] = heap->entries[i].xe0;
        if (heap->entries[i].xe8 && payload->x10[i] == 0) {
            payload->x10[i] = 1;
            payload->xa[i] = heap->x230;
        }
    }
    if (!ov96_021E5F24(course)) {
        u8 count = 0;
        Status *status = ov96_021E8A20(&blocks[1]);
        Payload *dst = ov96_021E8A20(&blocks[2]);
        int j;
        *dst = *(Payload *)ov96_021E8A20(&blocks[0]);
        for (i = 0, blocks += 2; i < 4; blocks++, i++) {
            payload = ov96_021E8A20(blocks);
            for (j = 0; j < 3; j++) {
                status->arr[i][j] = payload->x0[j];
            }
            if (payload->x9) {
                count++;
            }
        }
        if (count == PokeathlonCourse_GetParticipantCount(course)) {
            status->flag = 1;
        }
        ov96_021FB400(course);
    }
    ov96_021FB0F4(course);
}
