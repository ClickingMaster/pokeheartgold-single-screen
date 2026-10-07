#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x28];
    fx32 x;
    fx32 y;
    u8 pad30[0x90 - 0x30];
} Ent;
typedef struct {
    Ent ents[3];
} Team;
typedef struct {
    u8 pad[0x20];
    Team teams[4];
} Course;
int ov96_021E6104(Course *course);
BOOL ov96_021E872C(int x, int y, int a2, int a3, int r, int *out);
u8 ov96_021F20C4(Course *course, u8 team, u16 a2, u16 a3);
// @fn ov96_021F20C4
u8 ov96_021F20C4(Course *course, u8 team, u16 a2, u16 a3) {
    int r = ov96_021E6104(course);
    fx32 min = r << FX32_SHIFT;
    u8 i;
    u8 count = 0;
    u8 best;
    s32 dist[3];
    u8 valid[3];
    int out;
    for (i = 0; i < 3; i++) {
        Ent *ent = &course->teams[team].ents[i];
        if (ov96_021E872C(ent->x / FX32_ONE, ent->y / FX32_ONE, a2, a3, r, &out)) {
            dist[i] = out;
            valid[i] = TRUE;
            count++;
        } else {
            dist[i] = 0;
            valid[i] = FALSE;
        }
    }
    if (count == 0) {
        return 12;
    }
    best = 3;
    for (i = 0; i < 3; i++) {
        if (valid[i] && dist[i] < min) {
            best = i;
            min = dist[i];
        }
    }
    if (best == 3) {
        GF_ASSERT(FALSE);
        return 12;
    }
    return team * 3 + best;
}
