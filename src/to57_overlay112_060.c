#include "to57_overlay112_060_private.h"

int ov112_021EAF5C(W12Work *work) {
    int i;

    ov112_021E9C10(work, 1);
    ov112_021E9A78(work, 3);
    FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    ov112_021E7CA4(work, 5, 0x11);
    ov112_021E7CA4(work, 2, 0x17);
    for (i = 0; i < 4; i++) {
        String *string = NewString_ReadMsgData(PTR(work, 0x1E44C), i + 0x98);
        ov112_021E9FA4(work, i, string, 4, 0xF0100);
        String_Delete(string);
    }
    ov112_021E9F40(work, 6);
    U32(work, 0x1EC50) = 0;
    ov112_021EA51C(work, ov112_021FF0AC[U32(work, 0x1EC50)]);
    return 3;
}

int ov112_021EAFFC(W12Work *work) {
    u32 keys = gSystem.newKeys;

    if (keys & PAD_KEY_UP) {
        if (S32(work, 0x1EC50) > 0) {
            S32(work, 0x1EC50)
            --;
        }
        ov112_021EA51C(work, ov112_021FF0AC[U32(work, 0x1EC50)]);
    } else if (keys & PAD_KEY_DOWN) {
        if (S32(work, 0x1EC50) < 5) {
            S32(work, 0x1EC50)
            ++;
        }
        ov112_021EA51C(work, ov112_021FF0AC[U32(work, 0x1EC50)]);
    } else if (keys & PAD_BUTTON_A) {
        switch (U32(work, 0x1EC50)) {
        case 0:
            U32(work, 4) = 2;
            sub_020326A4(PTR(work, 0x1E440), 0, 0);
            ov112_021E9C10(work, 3);
            ov112_021E9A78(work, 1);
            FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
            BgCommitTilemapBufferToVram(PTR(work, 0x18), 1);
            return 0x1A;
        case 1:
            U32(work, 4) = 3;
            *(u16 *)(work->raw + 0x1D77C) = 0;
            U32(work, 0x10) = 0;
            ov112_021E9C10(work, 3);
            ov112_021E9A78(work, 1);
            FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
            BgCommitTilemapBufferToVram(PTR(work, 0x18), 1);
            sub_020326A4(PTR(work, 0x1E440), 0, 0);
            return 4;
        case 2:
            sub_020326A4(PTR(work, 0x1E440), 1, 1);
            *(u16 *)(work->raw + 0x1D77C) = 0;
            U32(work, 0x10) = 0;
            ov112_021E9C10(work, 3);
            ov112_021E9A78(work, 7);
            FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
            BgCommitTilemapBufferToVram(PTR(work, 0x18), 1);
            return 0xF;
        case 3:
            ov112_021E9C10(work, 3);
            ov112_021E9A78(work, 1);
            FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
            BgCommitTilemapBufferToVram(PTR(work, 0x18), 1);
            return 0xA;
        case 4:
            ov112_021E9C10(work, 3);
            ov112_021E9A78(work, 1);
            FillBgTilemapRect(PTR(work, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
            BgCommitTilemapBufferToVram(PTR(work, 0x18), 1);
            return 0xF;
        }
    }
    return 3;
}
