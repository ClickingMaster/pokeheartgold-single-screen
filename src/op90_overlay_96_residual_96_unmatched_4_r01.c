#pragma force_active on
#include "global.h"
#include "pokeathlon/pokeathlon.h"

typedef struct Entry {
    u32 words[9];
} Entry;

typedef struct Work {
    enum HeapID heap;
    PokeathlonCourseData *course;
    Entry entries[3];
    u32 count;
} Work;

void ov96_0220C9F4(Entry *entry, u8 index, PokeathlonCourseData *course);
Work *ov96_0220C93C(PokeathlonCourseData *course, enum HeapID heap);

// @fn ov96_0220C93C
Work *ov96_0220C93C(PokeathlonCourseData *course, enum HeapID heap) {
    u32 i;
    Entry *e;
    int n = PokeathlonCourse_GetParticipantCount(course);
    Work *w = Heap_Alloc(heap, sizeof(Work));

    MI_CpuFill8(w, 0, sizeof(Work));
    w->heap = heap;
    w->course = course;
    w->count = 4 - n;
    GF_ASSERT(w->count <= 3);
    i = 0;
    if (i < w->count) {
        e = w->entries;
        do {
            ov96_0220C9F4(e, n, course);
            n++;
            i++;
            e++;
        } while (i < w->count);
    }
    return w;
}
