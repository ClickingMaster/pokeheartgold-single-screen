#include "global.h"

#include "wifi_plaza_internal.h"

// Advances the plaza time; group i starts counting (state 1) once the time reaches ov45_02254A3C[i] and ends (state 2)
// after 1200 more updates.
void WiFiPlazaGroups_Update(WiFiPlazaGroups *groups) {
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
int WiFiPlazaGroups_Join(WiFiPlazaGroups *groups, u32 id, u32 value) {
    int seat;
    int base;
    int group;
    if (WiFiPlazaGroups_FindPlace(groups, (s32)id) == -1) {
        base = 0;
        group = 0;
        for (; group < 9; group++, base += 3) {
            if (WiFiPlazaGroups_GetState(groups, group) != 2) {
                for (seat = 0; seat < 3; seat++) {
                    if (WiFiPlazaGroups_HasPassedMarker(groups, seat + base, value) == 0 && WiFiPlazaGroups_GetPlaceId(groups, seat + base) == -1) {
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

void WiFiPlazaGroups_Leave(WiFiPlazaGroups *groups, u32 id) {
    int place = WiFiPlazaGroups_FindPlace(groups, id);
    if (place != -1) {
        u32 group = (u32)place / 3, seat = (u32)place % 3;
        groups->ids[group][seat] = (u32)-1;
    }
}

int WiFiPlazaGroups_GetPlaceId(WiFiPlazaGroups *groups, u32 place) {
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

int WiFiPlazaGroups_GetState(WiFiPlazaGroups *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->states[group];
}

int WiFiPlazaGroups_GetCounter(WiFiPlazaGroups *groups, u32 group) {
    if (group >= 9) {
        GF_AssertFail();
    }
    return groups->counters[group];
}

#pragma force_active on

void WiFiPlazaGroups_RequestAction(WiFiPlazaGroups *groups, u32 id) {
    u32 place = WiFiPlazaGroups_FindPlace(groups, id);
    if (place != (u32)-1) {
        groups->unk_128[place / 3][place % 3] = 1;
    }
}

#pragma force_active reset

int WiFiPlazaGroups_GetActionRequest(WiFiPlazaGroups *groups, u32 place) {
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

void WiFiPlazaGroups_ClearActionRequest(WiFiPlazaGroups *groups, u32 place) {
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
int WiFiPlazaGroups_FindPlace(WiFiPlazaGroups *groups, int id) {
    WiFiPlazaGroups *row = groups;
    int i;
    int j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            if (id == row->ids[0][j]) {
                return i * 3 + j;
            }
        }
        row = (WiFiPlazaGroups *)((u8 *)row + sizeof(row->ids[0]));
    }
    return -1;
}

#pragma force_active reset

void WiFiPlazaGroups_SetMarkerPassed(WiFiPlazaGroups *groups, u32 place, u32 value) {
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

BOOL WiFiPlazaGroups_HasPassedMarker(WiFiPlazaGroups *groups, u32 place, u32 value) {
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

u32 WiFiPlazaGroups_GetMarkerCount(WiFiPlazaGroups *groups, u32 place) {
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
