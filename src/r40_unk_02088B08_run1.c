#include "r40_unk_02088B08_private.h"

int sub_02088B08(void *data) {
    u8 *inner;
    u8 value;

    if (IsPaletteFadeFinished() == 1) {
        sub_0208E444(data);
        sub_02089C50(data);
        inner = *(u8 **)((u8 *)data + 0x22C);
        value = inner[0x12];
        if (value == 2) {
            return 8;
        }
        if (value == 4) {
            return 0x10;
        }
        return 2;
    }
    return 1;
}

int sub_02088B40(void *data) {
    u8 *work = data;
    u32 heldKeys;
    int selection;

    if (((SummaryInputWork *)data)->highNibble == 1) {
        (*(u8 **)(work + 0x22C))[0x17] = 1;
        return 0x15;
    }

    heldKeys = *(u32 *)(gSystem + 0x4C);
    if (heldKeys & 0x20) {
        sub_02089E30(data, -1);
        return 2;
    }
    if (heldKeys & 0x10) {
        sub_02089E30(data, 1);
        return 2;
    }
    if (heldKeys & 0x40) {
        sub_0208A2C0(data, -1);
        return 0x13;
    }
    if (heldKeys & 0x80) {
        sub_0208A2C0(data, 1);
        return 0x13;
    }

    if (*(u32 *)(gSystem + 0x48) & 2) {
        PlaySE(0x940);
        (*(u8 **)(work + 0x22C))[0x17] = 1;
        sub_0208ADB8(data, 0);
        return sub_0208B044(data, 0x15);
    }

    if (*(u32 *)(gSystem + 0x48) & 1) {
        switch (*(s8 *)(work + 0x7BC)) {
        case 1:
            PlaySE(0x69B);
            work[0x7BD] &= ~0xF;
            return 3;
        case 2:
            if (work[0x7C6] != 0) {
                PlaySE(0x5DD);
                work[0x7C4] = 0;
                return 0xA;
            }
            break;
        }
    }

    if (*(s8 *)(work + 0x7BC) == 1) {
        selection = sub_0208ADCC(data);
        if (selection != -1 && *(u16 *)(work + 0x264 + selection * 2) != 0) {
            u8 selection8 = selection;
            PlaySE(0x69B);
            work[0x7BD] = (work[0x7BD] & ~0xF) | (selection8 & 0xF);
            return 3;
        }
    }

    if (*(s8 *)(work + 0x7BC) == 2) {
        selection = sub_0208AEC4(data);
        if (selection != -1 && selection < 9 && selection < work[0x7C6]) {
            PlaySE(0x5DD);
            work[0x7C4] = selection;
            return 0xA;
        }
    }

    if ((*(u8 **)(work + 0x22C))[0x11] == 2) {
        selection = sub_0208AEB4(data);
        if (selection == 0) {
            if (sub_0208A2E0(data, -1) != -1) {
                PlaySE(0x5DD);
                return sub_0208B0B0(data, 0, 0x14);
            }
            return 2;
        }
        if (selection == 1) {
            if (sub_0208A2E0(data, 1) != -1) {
                PlaySE(0x5DD);
                return sub_0208B0B0(data, 1, 0x14);
            }
            return 2;
        }
    }
    return sub_02089E98(data);
}
