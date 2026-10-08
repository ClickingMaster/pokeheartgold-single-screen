#pragma force_active on
#include "global.h"

typedef struct Record {
    u8 pad[0x1c];
    s32 a;
    s32 b;
} Record;

u8 *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 *PokeathlonCourse_GetDataCopyArea(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_022148E8(void *obj, u8 value, u8 slot);
void ov96_0221236C(void *course);

// @fn ov96_0221236C
void ov96_0221236C(void *course) {
    u8 *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    Record *rec = (Record *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xf0);
    u8 slot = (rec->a >> 28) & 3;
    u8 value = (rec->b >> 25) & 0x7f;

    (work + 0x814)[slot] = value;
    ov96_022148E8(*(void **)(work + 0x750), value, slot);
}
