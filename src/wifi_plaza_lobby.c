#include "global.h"

#include "wifi_plaza_internal.h"

void *ov45_0222AB1C(WiFiPlazaWork *work) {
    return work->unk_508.profile;
}

int ov45_0222AB28(WiFiPlazaWork *work, int a1) {
    return ov45_0222C95C(work->unk_3cc, a1);
}

void ov45_0222AB38(WiFiPlazaWork *work, void *dest) {
    MI_CpuCopy8(work->unk_3cc, dest, sizeof(work->unk_3cc));
}

int ov45_0222AB48(WiFiPlazaWork *work, int a1) {
    return ov45_0222C9D0(&work->wordSet, a1);
}

u16 *ov45_0222AB58(WiFiPlazaWork *work, int a1) {
    return ov45_0222C9EC(&work->wordSet, a1);
}

u32 ov45_0222AB68(void) {
    u32 pair[2];
    ov45_0222EC3C(pair);
    return pair[0];
}

int ov45_0222AB78(void *unused, int a1) {
    int index = ov45_0222ECA4(a1);
    int result = -1;
    if (index != -1) {
        result = ov45_0222EC68(index);
    }
    return result;
}

void ov45_0222AB94(void *unused, u32 slot0, u32 slot1) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    msg.type = 0;
    msg.ids[0] = ov45_0222EC90(slot0);
    msg.ids[1] = ov45_0222EC90(slot1);
    msg.count = 2;
    ov45_0222EEF0(4, &msg, sizeof(msg));
}

void ov45_0222ABD0(void *unused, u32 slot0, u32 slot1, u8 value) {
    WiFiPlazaLobbyMessage msg = { 0 };
    msg.type = 1;
    msg.ids[0] = ov45_0222EC90(slot1);
    msg.ids[1] = ov45_0222EC90(slot0);
    msg.count = 2;
    msg.unk_12 = value;
    ov45_0222EEF0(4, &msg, sizeof(msg));
}

#pragma force_active on

void ov45_0222AC14(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag) {
    WiFiPlazaLobbyMessage msg = { 0 };
    u32 type;
    (void)unused;
    switch ((u32)selector) {
    case 0:
    case 1:
    case 2:
        type = 2;
        break;
    case 3:
    case 4:
        type = 3;
        break;
    case 5:
        type = 4;
        break;
    case 6:
        type = 5;
        break;
    default:
        return;
    }
    msg.type = type;
    msg.ids[0] = ov45_0222EC90(slot0);
    msg.ids[1] = ov45_0222EC90(slot1);
    msg.ids[2] = ov45_0222EC90(slot2);
    msg.ids[3] = ov45_0222EC90(slot3);
    msg.count = count;
    msg.unk_13_0 = selector;
    msg.unk_13_7 = flag;
    ov45_0222EEF0(4, &msg, 20);
}

#pragma force_active reset

void ov45_0222ACB8(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3) {
    WiFiPlazaLobbyMessage msg;
    u32 *dst = (u32 *)&msg;
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        msg.type = 8;
        msg.ids[0] = ov45_0222EC90(slot0);
        msg.ids[1] = ov45_0222EC90(slot1);
        msg.ids[2] = ov45_0222EC90(slot2);
        msg.ids[3] = ov45_0222EC90(slot3);
        msg.count = kind;
        msg.unk_13_0 = mode;
        msg.unk_13_7 = 0;
        ov45_0222EEF0(4, &msg, sizeof(msg));
        break;
    }
}

int ov45_0222AD2C(WiFiPlazaWork *work) {
    int value = ov45_0222ECDC(3);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int ov45_0222AD3C(WiFiPlazaWork *work) {
    int value = ov45_0222ECDC(2);
    if ((u32)value >= 5) {
        value = 0;
    }
    return value;
}

int ov45_0222AD4C(WiFiPlazaWork *work) {
    return ov45_0222ECDC(5) + 30;
}

u8 ov45_0222AD58(WiFiPlazaWork *work, u32 index) {
    if (index >= 20) {
        GF_AssertFail();
    }
    return work->unk_1e8[index];
}

void ov45_0222AD70(void *unused, u32 value) {
    u32 data[1];
    data[0] = value;
    ov45_0222EEF0(5, data, sizeof(data));
}

int ov45_0222AD80(WiFiPlazaWork *work, int a1) {
    return ov45_0222C4E4(&work->unk_20c, a1);
}

int ov45_0222AD90(WiFiPlazaWork *work, int a1) {
    return ov45_0222C4FC(&work->unk_20c, a1);
}

int ov45_0222ADA0(void) {
    return 1200;
}

int ov45_0222ADA8(WiFiPlazaWork *work, int a1) {
    return ov45_0222C5B4(&work->unk_20c, a1);
}

void ov45_0222ADB8(WiFiPlazaWork *work, int a1, int a2) {
    ov45_0222C408(&work->unk_20c, a1, a2);
}

void ov45_0222ADC8(WiFiPlazaWork *work, int a1) {
    ov45_0222C480(&work->unk_20c, a1);
}

void ov45_0222ADD8(WiFiPlazaWork *work, int a1) {
    ov45_0222C514(&work->unk_20c, a1);
}

void ov45_0222ADE8(WiFiPlazaWork *work, int a1) {
    ov45_0222C54C(&work->unk_20c, a1);
}

void ov45_0222ADF8(WiFiPlazaWork *work, int a1) {
    ov45_0222C580(&work->unk_20c, a1);
}

void ov45_0222AE08(u32 value, u32 *quotient, u32 *remainder) {
    *quotient = value / 3;
    *remainder = value % 3;
}

void ov45_0222AE24(WiFiPlazaWork *work, int a1, int a2) {
    ov45_0222C5E8(&work->unk_20c, a1, a2);
}

void ov45_0222AE34(WiFiPlazaWork *work, int a1) {
    ov45_0222C658(&work->unk_20c, a1);
}

void ov45_0222AE44(void) {
    u32 data;
    ov45_0222EEF0(6, &data, 4);
}

void ov45_0222AE54(void) {
    u32 data;
    ov45_0222EEF0(7, &data, 4);
}

void ov45_0222AE64(WiFiPlazaWork *work) {
    ov45_0222BD4C(&work->unk_1c0);
}

#pragma force_active on

int ov45_0222AE74(WiFiPlazaWork *work, int slot) {
    if (work->unk_1c0.unk_0a) {
        return 0;
    }
    if (ov45_0222A920(WiFiPlazaWork_GetPlayer(work, slot)) != 1) {
        return 0;
    }
    work->unk_1c0.unk_04 = slot;
    work->unk_1c0.unk_06 = 1;
    work->unk_1c0.unk_0a = 1;
    ov45_0222BE00(&work->unk_1c0, 0);
    ov45_0222EF4C(0, ov45_0222EC90(slot), &work->unk_1c0.unk_20, 4);
    ov45_0222BE48(&work->unk_1c0);
    return 1;
}

void ov45_0222AED8(WiFiPlazaWork *work, int a1) {
    int flag = 0;
    if (work->unk_1c0.unk_0a == 1) {
        if (work->unk_1c0.unk_06 != 2) {
            flag = 1;
        }
    } else if (work->unk_1c0.unk_0a == 2) {
        if (work->unk_1c0.unk_06 != 3) {
            flag = 1;
        }
    }
    if (flag) {
        ov45_0222BE28(work, ov45_0222EC90(work->unk_1c0.unk_04));
        return;
    }
    switch (work->unk_1c0.unk_0a) {
    case 1:
        work->unk_1c0.unk_06 = 3;
        break;
    case 2:
        work->unk_1c0.unk_06 = 2;
        break;
    default:
        ov45_0222BE28(work, ov45_0222EC90(work->unk_1c0.unk_04));
        return;
    }
    ov45_0222BE00(&work->unk_1c0, a1);
    ov45_0222EF4C(2, ov45_0222EC90(work->unk_1c0.unk_04), &work->unk_1c0.unk_20, 4);
    ov45_0222BE48(&work->unk_1c0);
}

#pragma force_active reset

void ov45_0222AF80(WiFiPlazaWork *work) {
    int id;

    if (work->unk_1c0.unk_06 != 0 && work->unk_1c0.unk_0a == 1) {
        work->unk_1c0.unk_22 = 0;
        id = ov45_0222EC90(work->unk_1c0.unk_04);
        ov45_0222EF4C(3, id, &work->unk_1c0.unk_20, 4);
        ov45_0222BD4C(&work->unk_1c0);
    }
}

void ov45_0222AFC4(WiFiPlazaWork *work) {
    int id;

    if (work->unk_1c0.unk_06 != 0) {
        id = ov45_0222EC90(work->unk_1c0.unk_04);
        work->unk_1c0.unk_06 = 4;
        work->unk_1c0.unk_22 = 4;
        ov45_0222EF4C(2, id, &work->unk_1c0.unk_20, 4);
    }
}

BOOL ov45_0222AFF8(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_0a != 0;
}

BOOL ov45_0222B00C(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_0a == 2;
}

u16 ov45_0222B020(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_04;
}

u8 ov45_0222B028(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_0b;
}

u8 ov45_0222B034(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_06;
}

u16 ov45_0222B040(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_08;
}

BOOL ov45_0222B048(WiFiPlazaWork *work, u32 index) {
    GF_ASSERT(index < 20);
    if (work->unk_1c0.unk_0c[index] >= 6) {
        return FALSE;
    }
    return TRUE;
}

BOOL ov45_0222B06C(WiFiPlazaWork *work) {
    if (work->unk_1c0.unk_06 == 4) {
        return TRUE;
    }
    if (ov45_0222BE74(&work->unk_1c0) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void ov45_0222B094(WiFiPlazaWork *work) {
    ov45_0222BE94(&work->unk_1c0);
}

void ov45_0222B0A4(WiFiPlazaWork *work) {
    work->unk_1c0.unk_07 = 1;
}

u8 ov45_0222B0B0(WiFiPlazaWork *work) {
    return work->unk_1c0.unk_07;
}
