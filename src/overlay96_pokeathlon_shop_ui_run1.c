#include "overlay96_pokeathlon_shop_ui_private.h"

u32 ov96_021F08F4(void *course) {
    void *gfx;
    int i;
    PokeathlonCourse_GetHeapAllocPtr4(course);
    gfx = PokeathlonCourse_GetGraphicsSystem(course);
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < 4; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 0x125);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    ov96_0221A56C(gfx, 2);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 2);
    return 1;
}
