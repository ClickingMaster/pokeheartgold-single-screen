#include "to52_ov96_r86_gap3_tail_private.h"

BOOL ov96_02205C94(void *course) {
    u8 *work;
    u8 *copy;
    u8 *touch;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    copy = PokeathlonCourse_GetDataCopyArea(course);
    touch = ov96_021E8A20(copy);
    *(u32 *)touch = 0;
    copy = ov96_021E8A20(copy + 0xf0);
    {
        u8 flag = 1;
        flag &= (*(u32 *)(copy + 0x10) >> 31);
        if (flag) {
            ov96_021EB63C(PTR8(work, 0x20));
            ov96_021EB144(PTR8(work, 0x348), 1);
            ov96_021E8324(course, ov96_02207C64);
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        ov96_021E8228(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        touch[4] = *(u16 *)(gSystem + 0x60);
        touch[5] = *(u16 *)(gSystem + 0x62);
        *(u32 *)touch = 1;
    }
    ov96_02205D30(course);
    return FALSE;
}
