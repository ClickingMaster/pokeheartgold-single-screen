#include "ov80_2237a70_partial_internal.h"

int ov80_02237F9C(int value) {
    if ((u32)value >= 8) {
        value = 7;
    }
    return value;
}

void ov80_02237FA4(void *save, int mode, int amount) {
    int stat = sub_0205C1F0(mode);
    int current;
    FrontierSave_SubStat(save, stat, sub_0205C268(sub_0205C1F0(mode)), amount);
    stat = sub_0205C218(mode);
    current = FrontierSave_GetStat(save, stat, sub_0205C268(sub_0205C218(mode)));
    current += amount;
    if (current > 9999) {
        stat = sub_0205C218(mode);
        FrontierSave_SetStat(save, stat, sub_0205C268(sub_0205C218(mode)), 9999);
    } else {
        stat = sub_0205C218(mode);
        FrontierSave_AddStat(save, stat, sub_0205C268(sub_0205C218(mode)), amount);
    }
}
