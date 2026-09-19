#include "private.h"

BOOL ov96_022093D0(void *course) {
    void *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 participant;
    s32 count;
    u32 offset;
    u16 result;

    if (IsPaletteFadeFinished()) {
        participant = ov96_021E5F24(course);
        result = ov96_0220AD4C(PTR_AT(work, 0x40));
        PokeathlonCourse_SetField5E0_AtIndex(course, (u8)participant, result);
        if (participant == 0) {
            count = PokeathlonCourse_GetParticipantCount(course);
            if (count < 4) {
                offset = count * 0x28;
                do {
                    void *entry = (u8 *)PokeathlonCourse_GetDataCopyArea(course) + 0x50 + offset;
                    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)count, ((R92Bits *)ov96_021E8A20(entry))->value);
                    count++;
                    offset += 0x28;
                } while (count < 4);
            }
        }
        return TRUE;
    }
    return FALSE;
}

