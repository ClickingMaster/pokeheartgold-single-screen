#include "to52_r6_w02_overlay96_r67_private.h"

void ov96_021F87E0(void *work) {
    int i;
    u8 *entry;

    ov96_021F8980(work);
    ov96_021F8A50(work);
    i = 0;
    entry = (u8 *)work + 0x14;
    for (; i < 5; i++, entry += 0x48) {
        ov96_021F8AFC(work, (u8)i, entry);
    }
    ov96_021F8DD4(work);
    G2dRenderer_SetSubSurfaceCoords((u8 *)work + 0x180, 0, 0x12C000);
    G2dRenderer_SetMainSurfaceCoords((u8 *)work + 0x180, 0, 0);
}

void *ov96_021F8830(void *work, u32 arg1, u32 arg2, Ov96EntryInput *input) {
    u8 *entry;
    s32 *participant;
    BOOL positive;
    u32 index;

    index = S32_AT(work, 0x330);
    entry = (u8 *)work + 0x14 + index * 0x48;
    if (U32_AT(entry, 0x30) != 0) {
        GF_AssertFail();
    }
    participant = PokeathlonCourse_GetParticipantData(PTR_AT(work, 4), input->participant);
    positive = participant[0] > 0;
    U32_AT(entry, 0x38) = arg1;
    U32_AT(entry, 0x3C) = arg2;
    U32_AT(entry, 0x40) = input->participant;
    U16_AT(entry, 0x44) = input->kind;
    ov96_021F8C04(work, input->window, entry);
    ov96_021F8BC0(work, input->participant, entry);
    U32_AT(work, 0x32C) = U16_AT(entry, 0x44) == 0;
    if (S32_AT(entry, 0x40) >= 4) {
        GF_AssertFail();
    }
    if (positive) {
        u32 value = ov96_021EEDCC(ov96_021E8424((u8)participant[0]));
        ov96_021EEA94(PTR_AT(entry, 0xC), value, 11, U32_AT(work, 0));
    } else {
        ov96_021EEB84(PTR_AT(entry, 0xC), PTR_AT(work, 4), (u8)U32_AT(entry, 0x40), 27);
    }
    ov96_021F8C2C(entry);
    ov96_021F8C54(U32_AT(work, 0x32C), TRUE, entry);
    U32_AT(entry, 0x30) = 1;
    S32_AT(work, 0x330) = (S32_AT(work, 0x330) + 1) % 5;
    return entry;
}

void ov96_021F8918(void *entry) {
    if (U32_AT(entry, 0x30) == 0) {
        GF_AssertFail();
    }
    U32_AT(entry, 0x30) = 0;
    ov96_021F8E94((u8 *)entry + 0x10);
    ov96_021F8E94((u8 *)entry + 0x20);
}
