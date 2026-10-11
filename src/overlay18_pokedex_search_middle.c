#include "overlay18_pokedex_search_middle_private.h"

void PokedexApp_ShowFormMonPics(Ov18MidWork *work, int arg1, int arg2) {
    u8 b;
    u8 a;
    int first;
    int second;
    int height;
    PokedexApp_GetSeenFormAndGender(work, arg1, &a, &b);
    if (!work->flag) {
        first = 1;
        second = 2;
        PokedexApp_SetSpriteVisible(work, 3, 0);
        PokedexApp_SetSpriteVisible(work, 4, 0);
    } else {
        first = 3;
        second = 4;
        PokedexApp_SetSpriteVisible(work, 1, 0);
        PokedexApp_SetSpriteVisible(work, 2, 0);
    }
    work->flag ^= 1;
    PokedexApp_LoadMonPicToSprite(work, work->species, a, b, 2, first, arg2);
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)work + 0x670 + first * 4), 0x40, 0x78, 0x200000);
    PokedexApp_LoadMonPicToSprite(work, work->species, a, b, 0, second, arg2);
    height = GetMonPicHeightBySpeciesGenderForm(work->species, b, 0, a, 0);
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)work + 0x670 + second * 4), 0xc0, height + 0x78, 0x200000);
    PokedexApp_SetSpriteVisible(work, first, 1);
    PokedexApp_SetSpriteVisible(work, second, 1);
}

void PokedexApp_SetFormListIcon(Ov18MidWork *work, int arg, int offset) {
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
    PokedexApp_SetMonIcon(work, *(u16 *)((u8 *)work + 0x18a2), form, arg);
}

void PokedexApp_DrawFormListIcons(Ov18MidWork *work) {
    int i = 0;
    int y = 0xe;
    u32 entry;
    do {
        entry = work->selected + i - 2;
        if (entry >= (u32)work->count) {
            PokedexApp_SetSpriteVisible(work, i + 0xe, 0);
        } else {
            PokedexApp_SetSpriteVisible(work, i + 0xe, 1);
            PokedexApp_SetFormListIcon(work, i + 0xe, entry);
        }
        PokedexApp_SetSpritePos(work, i + 0xe, 0x30, (s16)y, 0);
        i++;
        y += 0x18;
    } while ((u32)i < 6);
}

void PokedexApp_ScrollFormListIcons(Ov18MidWork *work, int direction) {
    int i = 0;
    int zero = 0;
    s16 x;
    s16 y;
    do {
        PokedexApp_GetSpritePos(work, i + 0xe, &x, &y, zero);
        if (y == -10 || y == 0x86) {
            break;
        }
        i++;
    } while ((u32)i < 6);
    if (direction < 0) {
        PokedexApp_SetSpritePos(work, i + 0xe, 0x30, 0x86, 0);
        if (work->selected + 2 >= work->count) {
            i += 0xe;
            PokedexApp_SetSpriteVisible(work, i, 0);
        } else {
            PokedexApp_SetSpriteVisible(work, i + 0xe, 1);
            i += 0xe;
            PokedexApp_SetFormListIcon(work, i, work->selected + 2);
        }
    } else {
        PokedexApp_SetSpritePos(work, i + 0xe, 0x30, -10, 0);
        if (work->selected - 2 < 0) {
            i += 0xe;
            PokedexApp_SetSpriteVisible(work, i, 0);
        } else {
            PokedexApp_SetSpriteVisible(work, i + 0xe, 1);
            i += 0xe;
            PokedexApp_SetFormListIcon(work, i, work->selected - 2);
        }
    }
}
