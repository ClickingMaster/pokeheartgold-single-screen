#include "overlay18_pokedex_search_middle_private.h"

void ov18_021F5EFC(Ov18MidWork *work, int arg1, int arg2) {
    u8 b;
    u8 a;
    int first;
    int second;
    int height;
    ov18_021F3CA8(work, arg1, &a, &b);
    if (!work->flag) {
        first = 1;
        second = 2;
        ov18_021F11C0(work, 3, 0);
        ov18_021F11C0(work, 4, 0);
    } else {
        first = 3;
        second = 4;
        ov18_021F11C0(work, 1, 0);
        ov18_021F11C0(work, 2, 0);
    }
    work->flag ^= 1;
    ov18_021F1A7C(work, work->species, a, b, 2, first, arg2);
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)work + 0x670 + first * 4), 0x40, 0x78, 0x200000);
    ov18_021F1A7C(work, work->species, a, b, 0, second, arg2);
    height = GetMonPicHeightBySpeciesGenderForm(work->species, b, 0, a, 0);
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)work + 0x670 + second * 4), 0xc0, height + 0x78, 0x200000);
    ov18_021F11C0(work, first, 1);
    ov18_021F11C0(work, second, 1);
}

void ov18_021F5FFC(Ov18MidWork *work, int arg, int offset) {
    int value;
    int copy;
    int form;
    int base = 0x18a4;
    value = *(u8 *)((u8 *)work + offset + base);
    copy = value;
    if (copy & 0x80) {
        int species = *(u16 *)((u8 *)work + base - 2);
        form = value ^ 0x80;
        if (species == 0xac) {
            if (form == 2) {
                form = 1;
            } else {
                form = 0;
            }
        }
    } else {
        form = 0;
    }
    ov18_021F14FC(work, *(u16 *)((u8 *)work + 0x18a2), form, arg);
}

void ov18_021F6038(Ov18MidWork *work) {
    int i = 0;
    int y = 0xe;
    u32 entry;
    do {
        entry = work->selected + i - 2;
        if (entry >= (u32)work->count) {
            ov18_021F11C0(work, i + 0xe, 0);
        } else {
            ov18_021F11C0(work, i + 0xe, 1);
            ov18_021F5FFC(work, i + 0xe, entry);
        }
        ov18_021F1294(work, i + 0xe, 0x30, (s16)y, 0);
        i++;
        y += 0x18;
    } while ((u32)i < 6);
}

void ov18_021F609C(Ov18MidWork *work, int direction) {
    int i = 0;
    int zero = 0;
    s16 x;
    s16 y;
    do {
        ov18_021F12C8(work, i + 0xe, &x, &y, zero);
        if (y == -10 || y == 0x86) {
            break;
        }
        i++;
    } while ((u32)i < 6);
    if (direction < 0) {
        ov18_021F1294(work, i + 0xe, 0x30, 0x86, 0);
        if (work->selected + 2 >= work->count) {
            i += 0xe;
            ov18_021F11C0(work, i, 0);
        } else {
            ov18_021F11C0(work, i + 0xe, 1);
            i += 0xe;
            ov18_021F5FFC(work, i, work->selected + 2);
        }
    } else {
        ov18_021F1294(work, i + 0xe, 0x30, -10, 0);
        if (work->selected - 2 < 0) {
            i += 0xe;
            ov18_021F11C0(work, i, 0);
        } else {
            ov18_021F11C0(work, i + 0xe, 1);
            i += 0xe;
            ov18_021F5FFC(work, i, work->selected - 2);
        }
    }
}
