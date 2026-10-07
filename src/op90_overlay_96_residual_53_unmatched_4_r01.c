#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x18];
    int active;
    u8 pad1c[0x90 - 0x1c];
} Ent;
typedef struct {
    Ent ents[3];
} Team;
typedef struct {
    u8 pad[0x20];
    Team teams[4];
} Course;
typedef struct {
    BOOL hit;
    int x4;
    Ent *other;
    int xc;
} Contact;
extern Contact _0221DCA0[12][12];
BOOL ov96_021F218C(Ent *a, Ent *b);
void ov96_021F21EC(Course *course);
// @fn ov96_021F21EC
void ov96_021F21EC(Course *course) {
    u8 i;
    u8 j;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++) {
            if (i == j) {
                _0221DCA0[i][j].hit = FALSE;
                _0221DCA0[i][j].other = NULL;
            } else if (j >= i) {
                Ent *a = &course->teams[i / 3].ents[i % 3];
                Ent *b = &course->teams[j / 3].ents[j % 3];
                if (a->active != TRUE || b->active != TRUE) {
                    _0221DCA0[i][j].hit = FALSE;
                    _0221DCA0[j][i].hit = FALSE;
                    _0221DCA0[i][j].other = NULL;
                    _0221DCA0[j][i].other = NULL;
                } else if (ov96_021F218C(a, b)) {
                    _0221DCA0[i][j].hit = TRUE;
                    _0221DCA0[j][i].hit = TRUE;
                    _0221DCA0[i][j].other = b;
                    _0221DCA0[j][i].other = a;
                } else {
                    _0221DCA0[i][j].hit = FALSE;
                    _0221DCA0[j][i].hit = FALSE;
                    _0221DCA0[i][j].other = NULL;
                    _0221DCA0[j][i].other = NULL;
                }
            }
        }
    }
}
