#include "global.h"

#include "wifi_plaza_internal.h"

// Advances the plaza time; group i starts counting (state 1) once the time reaches ov45_02254A3C[i] and ends (state 2)
// after 1200 more updates.
void ov45_0222C3B0(WiFiPlazaUnk20C *groups) {
    int i;
    if (groups->active != 0) {
        for (i = 0; i < 9; i++) {
            if (groups->time == ov45_02254A3C[i]) {
                groups->states[i] = 1;
            }
            if (groups->states[i] == 1) {
                if (groups->counters[i] + 1 <= 1200) {
                    groups->counters[i]++;
                } else {
                    groups->states[i] = 2;
                }
            }
        }
        if (groups->time + 1 <= 4320) {
            groups->time++;
        }
    }
}

#pragma force_active on

// Puts id into the first free place of a group that has not ended and whose unk_bc count allows value; returns the
// place (group * 3 + seat) or -1.
int ov45_0222C408(WiFiPlazaUnk20C *groups, u32 id, u32 value) {
    int seat;
    int base;
    int group;
    if (ov45_0222C5B4(groups, (s32)id) == -1) {
        base = 0;
        group = 0;
        for (; group < 9; group++, base += 3) {
            if (ov45_0222C4E4(groups, group) != 2) {
                for (seat = 0; seat < 3; seat++) {
                    if (ov45_0222C61C(groups, seat + base, value) == 0 && ov45_0222C4B4(groups, seat + base) == -1) {
                        groups->ids[group][seat] = id;
                        return seat + base;
                    }
                }
            }
        }
    }
    return -1;
}

#pragma force_active reset

void ov45_0222C480(WiFiPlazaUnk20C *groups, u32 id) {
    int place = ov45_0222C5B4(groups, id);
    if (place != -1) {
        u32 group = (u32)place / 3, seat = (u32)place % 3;
        groups->ids[group][seat] = (u32)-1;
    }
}

int ov45_0222C4B4(WiFiPlazaUnk20C *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->ids[group][seat];
    return *slot;
}

int ov45_0222C4E4(WiFiPlazaUnk20C *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->states[group];
}

int ov45_0222C4FC(WiFiPlazaUnk20C *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->counters[group];
}

#pragma force_active on

void ov45_0222C514(WiFiPlazaUnk20C *groups, u32 id) {
    u32 place = ov45_0222C5B4(groups, id);
    if (place != (u32)-1) {
        groups->unk_128[place / 3][place % 3] = 1;
    }
}

#pragma force_active reset

int ov45_0222C54C(WiFiPlazaUnk20C *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->unk_128[group][seat];
    return *slot;
}

void ov45_0222C580(WiFiPlazaUnk20C *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->unk_128[group][seat];
    *slot = 0;
}

#pragma force_active on

// The place (group * 3 + seat) of id, or -1.
int ov45_0222C5B4(WiFiPlazaUnk20C *groups, int id) {
    WiFiPlazaUnk20C *row = groups;
    int i;
    int j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            if (id == row->ids[0][j]) {
                return i * 3 + j;
            }
        }
        row = (WiFiPlazaUnk20C *)((u8 *)row + sizeof(row->ids[0]));
    }
    return -1;
}

#pragma force_active reset

void ov45_0222C5E8(WiFiPlazaUnk20C *groups, u32 place, u32 value) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->unk_bc[group][seat];
    *slot = value + 1;
}

BOOL ov45_0222C61C(WiFiPlazaUnk20C *groups, u32 place, u32 value) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->unk_bc[group][seat];
    if (*slot <= value) {
        return FALSE;
    }
    return TRUE;
}

u32 ov45_0222C658(WiFiPlazaUnk20C *groups, u32 place) {
    u32 group;
    u32 seat;
    u32 *slot;
    if (place >= 27) {
        GF_AssertFail();
    }
    group = place / 3;
    seat = place % 3;
    slot = &groups->unk_bc[group][seat];
    return *slot;
}
