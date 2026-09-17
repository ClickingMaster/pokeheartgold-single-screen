#include "to47_overlay83_battle_summary_r19_private.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))

void ov83_02241770(u8 *work, void *window) {
    u16 a;
    u16 b;
    u16 c;
    u16 d;
    int stat;
    u16 y;
    u16 x;

    ov83_02240F7C(work, &a, &b, &c, &d);
    if (ov80_02237D8C(work[9]) == 0) {
        x = a + 0x48;
        FillWindowPixelRect(window, 0, x, y = b + 1, 0x30, 0x10);
        stat = sub_0205C1F0(work[9]);
        stat = FrontierSave_GetStat(PTR_AT(work, 4), stat, sub_0205C268(sub_0205C1F0(work[9])));
        ov83_02240C48(work, 0, stat, 4, 0);
        work[0xA] = ov83_0223FCB4(work, window, 2, x, y, 0xFF, 1, 2, 0, 0);
    } else {
        int right;
        int left;

        FillWindowPixelRect(window, 0, 0x40, 0, 0x30, 0x10);
        FillWindowPixelRect(window, 0, 0xC0, 0, 0x30, 0x10);
        if (sub_0203769C() == 0) {
            stat = sub_0205C1F0(work[9]);
            left = FrontierSave_GetStat(PTR_AT(work, 4), stat, sub_0205C268(sub_0205C1F0(work[9])));
            right = U16_AT(work, 0x802);
        } else {
            left = U16_AT(work, 0x802);
            stat = sub_0205C1F0(work[9]);
            right = FrontierSave_GetStat(PTR_AT(work, 4), stat, sub_0205C268(sub_0205C1F0(work[9])));
        }
        ov83_02240C48(work, 0, left, 4, 0);
        ov83_02241DD8(work, window, PTR_AT(work, 0x20), 2, 0x70, 0, 0, 0x10200, 1);
        ov83_02240C48(work, 0, right, 4, 0);
        ov83_02241DD8(work, window, PTR_AT(work, 0x20), 3, 0xF0, 0, 0, 0x10200, 1);
    }
    ScheduleWindowCopyToVram(window);
}

void ov83_022418E8(u8 *work, u32 value, u8 type) {
    u32 count;
    u32 old;
    int cost;

    PlaySE(0x5E3);
    old = work[0x15];
    count = ov83_0224776C(old, value);
    switch (type) {
    case 1:
    case 2:
    case 3: {
        const u16 *costs = ov83_02247D18;
        cost = costs[type - 1];
        break;
    }
    case 6:
    case 7:
        cost = ov83_02240EF8(U16_AT(work, 0x10));
        break;
    case 9:
    case 10:
        cost = 0;
        break;
    }

    if (sub_0203769C() == 0) {
        if (value < old) {
            ov83_02240C6C(work, 5);
            ov80_02237FA4(PTR_AT(work, 4), work[9], cost);
        } else {
            ov83_022477C4(PTR_AT(work, 0x24), 5);
            U16_AT(work, 0x802) -= cost;
        }
    } else {
        if (value < old) {
            ov83_022477C4(PTR_AT(work, 0x24), 5);
            U16_AT(work, 0x802) -= cost;
        } else {
            ov83_02240C6C(work, 5);
            ov80_02237FA4(PTR_AT(work, 4), work[9], cost);
        }
    }

    ov83_02241770(work, work + 0x50);
    ov83_02241730(work);
    ov83_02241B18(work);
    ov83_0224042C(work);
    ov83_022477EC(2, 0, (u32 *)(work + 0x868));
    ov83_02241354(work + 0xB0);

    switch (type) {
    case 1:
    case 2:
    case 3:
        ov83_02247944(work + 0xB0, Options_GetFrame(PTR_AT(work, 0x508)));
        ov83_022415F4(work, count, type);
        break;
    case 6:
    case 7:
        ov83_02247944(work + 0xB0, Options_GetFrame(PTR_AT(work, 0x508)));
        ov83_022416A0(work, count, U16_AT(work, 0x10));
        break;
    case 9:
        ov83_02241A60(work, count);
        break;
    case 10:
        ov83_02241ABC(work, count);
        break;
    }
}

void ov83_02241A60(u8 *work, u32 value) {
    void *mon;

    ov83_0223FAA8(work, 2);
    ClearWindowTilemapAndScheduleTransfer(work + 0xB0);
    ClearWindowTilemapAndScheduleTransfer(work + 0xA0);
    ClearWindowTilemapAndScheduleTransfer(work + 0x140);
    mon = Party_GetMonByIndex(PTR_AT(work, 0x7A4), ov83_02247768(work[0x14], value));
    ov83_0223FD4C(work, work + 0x90, mon);
    ov83_022477EC(2, 1, (u32 *)(work + 0x868));
}

void ov83_02241ABC(u8 *work, u32 value) {
    void *mon;

    ov83_0223FB24(work, 2);
    ClearWindowTilemapAndScheduleTransfer(work + 0xB0);
    ClearWindowTilemapAndScheduleTransfer(work + 0xA0);
    ClearWindowTilemapAndScheduleTransfer(work + 0x140);
    mon = Party_GetMonByIndex(PTR_AT(work, 0x7A4), ov83_02247768(work[0x14], value));
    ov83_0223FF44(work, work + 0x90, mon);
    ov83_022477EC(2, 1, (u32 *)(work + 0x868));
}
