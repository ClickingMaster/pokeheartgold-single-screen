#pragma force_active on
#include "global.h"
typedef struct {
    void *host;
    u8 pad4[0x14];
    int active;
    VecFx32 prevPos;
    VecFx32 pos;
    u8 pad34[0xc];
    u8 x40;
    u8 pad41[0x90 - 0x41];
} Ent;
typedef struct {
    Ent ents[3];
} Team;
typedef struct {
    u8 pad[0x20];
    Team teams[4];
} Course;
typedef struct {
    u16 x;
    u16 y;
} StartPos;
extern StartPos ov96_0221BDD4[4][3];
void ov96_021EB0A4(void *host, s32 x, s32 y, s32 *outX, s32 *outY);
void ov96_021F2A00(Course *course);
// @fn ov96_021F2A00
void ov96_021F2A00(Course *course) {
    int t;
    int i;
    s32 x;
    s32 y;
    for (t = 0; t < 4; t++) {
        for (i = 0; i < 3; i++) {
            Ent *ent = &course->teams[t].ents[i];
            if (ent->active == 0) {
                ent->active = 2;
                ov96_021EB0A4(ent->host, ov96_0221BDD4[t][i].x, ov96_0221BDD4[t][i].y, &x, &y);
                ent->pos.x = x << FX32_SHIFT;
                ent->pos.y = y << FX32_SHIFT;
                ent->prevPos = ent->pos;
                ent->x40 = 2;
            }
        }
    }
}
