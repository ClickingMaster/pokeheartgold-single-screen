#include "overlay96_course_touch_state_r108_private.h"

BOOL ov96_02211AF0(void *course) {
    u8 *alloc = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *area = PokeathlonCourse_GetDataCopyArea(course);
    Ov96TouchRec *rec = ov96_021E8A20(area);
    Ov96FlagState *flags;

    rec->unk00 = 0;
    flags = ov96_021E8A20(area + 0xF0);
    u8 f = 1;
    f &= flags->word >> 24;
    if (f) {
        ov96_021EB63C(*(void **)alloc);
        ov96_021EB144(*(void **)(alloc + 0x748), 1);
        ov96_021E65A4(course);
        ov96_021E8324(course, ov96_02214618);
        return TRUE;
    }
    if (System_GetTouchNew()) {
        ov96_021E8228(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        rec->unk04 = (u8)gSystem.touchX;
        rec->unk05 = (u8)gSystem.touchY;
        rec->unk00 = 1;
    }
    ov96_02211F38(course);
    return FALSE;
}
