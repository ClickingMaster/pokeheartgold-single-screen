#include "to45_overlay96_r82_private.h"

u32 ov96_02201B18(void *course) {
    void *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    ov96_02203CE4(PTR_AT(work, 0x5e0));
    PokeathlonCourseSpritePool_Draw(PTR_AT(work, 0x48));
    return 1;
}

u32 ov96_02201B34(void *course) {
    void *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < 4; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 0x12b);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    ov96_0221A56C(gfx, 8);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 8);
    return 1;
}
