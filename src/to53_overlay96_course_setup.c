#include "to53_overlay96_course_setup_private.h"

void ov96_021F5EC4(void *course) {
    u8 *data;
    u8 *work;
    int i;
    u8 *entry;

    data = (void *)ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    for (i = 0, entry = work; i < 4; i++, entry += 4) {
        if (i == ov96_021E5F24(course)) {
            if (W18_U8(work, 0x142) != 0 && ov96_021EB57C(W18_PTR(entry, 0x68)) == FALSE) {
                ov96_021EB564(W18_PTR(entry, 0x68), i + 13);
            }
        } else if (data[i] != 0 && ov96_021EB57C(W18_PTR(entry, 0x68)) == FALSE) {
            ov96_021EB564(W18_PTR(entry, 0x68), i + 13);
        }
    }
}
