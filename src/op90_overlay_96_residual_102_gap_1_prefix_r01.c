#pragma force_active on
#include "global.h"
#include "pokeathlon/pokeathlon.h"

typedef struct ObjData {
    u32 unk0;
    fx32 mass;
} ObjData;

typedef struct Obj {
    void *course;
    void *sprite;
    ObjData *data;
    u32 state;
    VecFx32 unk10;
    VecFx32 pos;
    VecFx32 prev;
    VecFx32 vel;
    u32 unk40_0 : 20;
    u32 dir : 4;
    u32 kind : 2;
    u32 hit : 1;
    u32 unk40_27 : 5;
    u8 unk44[4];
} Obj;

typedef struct Group {
    s16 x;
    s16 y;
    u32 flags;
    Obj objs[3];
    u32 padE0;
} Group;

typedef struct Work {
    u8 unk0[0xCC];
    u8 contact[12][12];
    Group groups[4];
} Work;

BOOL ov96_0220D694(Obj *obj);
BOOL ov96_0220D5D0(Obj *a, Obj *b);
int ov96_0220D6CC(VecFx32 *v);
void ov96_0220E974(VecFx32 *v, fx32 scale);
void ov96_0220F710(PokeathlonCourseData *data);

// @fn ov96_0220F710
void ov96_0220F710(PokeathlonCourseData *data) {
    int i, j;
    Obj *a, *b;
    fx32 mass;
    Work *work = PokeathlonCourse_GetHeapAllocPtr4(data);
    u8 (*contact)[12] = work->contact;

    for (i = 0; i < 12; i++) {
        a = &work->groups[i / 3].objs[i % 3];
        for (j = i + 1; j < 12; j++) {
            b = &work->groups[j / 3].objs[j % 3];
            if (ov96_0220D694(a) && ov96_0220D694(b) && ov96_0220D5D0(a, b) && !contact[i][j]) {
                VecFx32 dir = { 0, 0, 0 };
                contact[i][j] = contact[j][i] = TRUE;
                VEC_Subtract(&a->pos, &b->pos, &dir);
                VEC_Normalize(&dir, &dir);
                mass = b->data->mass;
                if (a->kind == 0) {
                    mass = mass * 6 / 5;
                }
                VEC_MultAdd(mass, &dir, &a->prev, &a->vel);
                mass = a->data->mass;
                if (b->kind == 0) {
                    mass = mass * 6 / 5;
                }
                VEC_MultAdd(mass, &dir, &b->prev, &b->vel);
                b->vel.x *= -1;
                b->vel.y *= -1;
                ov96_0220E974(&a->vel, FX32_CONST(8));
                ov96_0220E974(&b->vel, FX32_CONST(8));
                b->dir = ov96_0220D6CC(&dir);
                b->hit = TRUE;
                a->hit = TRUE;
            } else {
                contact[i][j] = contact[j][i] = FALSE;
            }
        }
    }
}
