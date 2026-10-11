#include "to57_overlay112_036_private.h"

int ov112_021E9480(int species, int form, int gender) {
    int base = sub_02070438((u16)species, (u8)gender);
    switch (species) {
    case 0x19c:
        return base + 0x26e;
    case 0x19d:
        return base + 0x271;
    case 0x1a6:
        return base + 0x274;
    case 0x1a7:
        return base + 0x276;
    case 0x1a5:
        return base + 0x278;
    case 0x1ed:
        return base + 0x27a;
    case 0x15f:
        return base + 0x26a;
    case 0x182:
        return base + 0x24a;
    case 0xc9:
        return base + 0x24e;
    case 0x1ec:
        return base + 0x28c;
    case 0x1df:
        return base + 0x28e;
    case 0x1e7:
        return base + 0x294;
    case 0xac:
        return base + 0x296;
    default:
        if (form != 1) {
            form = 0;
        }
        return ov112_021F397E[species][form];
    }
}

void ov112_021E95A0(Ov112State *p) {
    u32 a, points;
    u32 count;
    u32 i;
    sub_02032674(P(p, 0x1e440), &a, &points);
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(P(p, 0x20)))) {
        count = 20;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (sPokewalkerCourses[i].requirement <= points && !Pokewalker_CourseIsUnlocked(P(p, 0x1e440), i)) {
            Pokewalker_UnlockCourse(P(p, 0x1e440), i);
            W(p, 0x1ec78) = i;
            return;
        }
    }
}

int ov112_021E9610(Ov112State *p, int points) {
    int count;
    int i;
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(P(p, 0x20)))) {
        count = 20;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (!Pokewalker_CourseIsUnlocked(P(p, 0x1e440), i)) {
            if (sPokewalkerCourses[i].requirement > points) {
                return sPokewalkerCourses[i].requirement - points;
            }
            return 0;
        }
    }
    return -1;
}
