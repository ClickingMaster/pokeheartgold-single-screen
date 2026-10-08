#pragma force_active on
#include "global.h"
#include "math_util.h"
typedef struct CourseObj {
    u8 pad00[0x2c];
    VecFx32 pos;
    u8 pad38[0x5d - 0x38];
    s8 unk5D;
    u8 pad5E[2];
    u32 unk60_0 : 27;
    u32 unk60_27 : 1;
    u32 unk60_28 : 4;
    u8 pad64[0xa8 - 0x64];
} CourseObj;
typedef struct CourseHeap {
    u8 pad00[0x188];
    u8 collide[4][4];
    u8 pad198[0x1a4 - 0x198];
    CourseObj objs[4];
} CourseHeap;
CourseHeap *PokeathlonCourse_GetHeapAllocPtr4(void *course);
BOOL ov96_02218744(CourseObj *);
BOOL ov96_02218598(CourseObj *, CourseObj *);
BOOL ov96_022178A0(CourseObj *, CourseObj *, VecFx32 *);
int ov96_02217E08(CourseObj *);
void ov96_02217DC8(CourseObj *, int);
u32 MTRandom(void);
void ov96_02217E7C(void *course);
// @fn ov96_02217E7C
void ov96_02217E7C(void *course) {
    int i;
    CourseHeap *heap;
    u8 (*collide)[4];
    int j;
    heap = PokeathlonCourse_GetHeapAllocPtr4(course);
    collide = heap->collide;
    for (i = 0; i < 4; i++) {
        CourseObj *a = &heap->objs[i];
        if (!ov96_02218744(a) || a->unk5D > 0) {
            continue;
        }
        for (j = i + 1; j < 4; j++) {
            CourseObj *b = &heap->objs[j];
            if (ov96_02218744(b) && ov96_02218598(a, b)) {
                VecFx32 d = {0, 0, 0};
                BOOL r1, r2;
                int s1, s2;
                if (collide[i][j] && !a->unk60_27) {
                    collide[i][j] = collide[j][i] = 0;
                }
                if (collide[i][j] == 0) {
                    VEC_Subtract(&a->pos, &b->pos, &d);
                    if (d.x == 0 && d.y == 0) {
                        d.x = (MTRandom() & 0x3f) - 32;
                        d.y = (MTRandom() & 0x3f) - 32;
                    }
                    VEC_Normalize(&d, &d);
                    r1 = ov96_022178A0(a, b, &d);
                    d.x *= -1;
                    d.y *= -1;
                    r2 = ov96_022178A0(b, a, &d);
                    s1 = ov96_02217E08(a);
                    s2 = ov96_02217E08(b);
                    ov96_02217DC8(a, s1);
                    ov96_02217DC8(b, s2);
                    if (r1 || r2) {
                        collide[i][j] = collide[j][i] = 1;
                    }
                }
            }
        }
    }
}
