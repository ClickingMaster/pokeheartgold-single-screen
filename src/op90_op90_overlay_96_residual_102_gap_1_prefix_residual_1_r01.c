#pragma force_active on
#include "global.h"
#include "filesystem.h"

typedef struct Racer {
    void *course;
    void *obj;
    void *data;
    int state;
    int x;
    int y;
    u8 pad18[4];
    int startX;
    int startY;
    u8 pad24[0x1c];
    u32 pad40 : 16;
    u32 index : 4;
    u32 mode : 4;
    u32 type : 2;
    u32 pad40b : 6;
    u8 pad44;
    u8 group;
    u8 slot;
    u8 pad47;
} Racer;

typedef struct Group {
    Racer racers[3];
    u32 index : 2;
    u32 pad : 30;
    u8 padDC[8];
} Group;

typedef struct Work {
    u8 pad0[4];
    void *course;
    u8 pad8[8];
    void *spriteManager;
    u8 pad14[8];
    void *res;
    void *objs;
    u8 pad24[0x140];
    Group groups[4];
} Work;

typedef struct Coord {
    s16 x;
    s16 y;
} Coord;

typedef struct Pair {
    int a;
    int b;
} Pair;

typedef struct Info {
    u8 pad0[4];
    u16 type;
} Info;

extern Coord ov96_0221CFBC[4][3];

void ov96_0220F280(void *course);
void *PokeathlonCourseMonObjects_Get(void *objs, u8 index);
void PokeathlonCourseMonObject_SetDrawFlag(void *obj, int v);
void *PokeathlonCourse_GetPerformance(void *course, int group, int slot);
int ov96_021E6138(void *p);
void ov96_021EAF70(void *obj, int a, int b);
void *PokeathlonCourseMonObject_GetGfx(void *obj);
Info *PokeathlonCourseMonGfx_GetInfo(void *p);
void ov96_0220D554(void *obj, int v);
void ov96_021EB10C(void *obj, f32 a, f32 b);
void PokeathlonCourseMonObject_SetAnimState(void *obj);
void ov96_021EAF94(void *obj, int x, int y);
int ov96_021E6104(void);
void ov96_021EAF6C(void *obj, int v);
void ov96_021EB0A4(void *obj, int x, int y, int *outX, int *outY);
void *ov96_0220F378(void *course, u8 group, u8 slot);
int ov96_021E5F24(void *course);
void *SpriteManager_GetSpriteList(void *manager);
void *PokeathlonCourse_AddMarker(void *course, void *obj, void *res, void *list, int v);
void Sprite_SetDrawPriority(void *sprite, u32 priority);
void PokeathlonCourseMonObject_SetDrawPriority(void *obj, int v);
u8 *PokeathlonCourse_GetDataCopyArea(void *course);
u8 *ov96_021E8A20(void *ptr);
void PokeathlonCourse_CreateStartSprites(void *course, int a, void *res, void *list, int b, int c, Coord *coords);
void ov96_0220F4A0(Work *w);

static inline int GetCourseType(void *course, int idx) {
    return ov96_021E6138(PokeathlonCourse_GetPerformance(course, idx / 3, idx % 3));
}

// @fn ov96_0220F4A0
void ov96_0220F4A0(Work *w) {
    int pairs[3][2] = { 0 };
    Coord coords[3];
    int outX, outY;
    void *course;
    s32 i;
    int group, slot;
    void *obj;
    void *obj2;
    Info *info;
    Racer *racer;
    s16 x, y;
    u8 *data;

    ReadWholeNarcMemberByIdPair(pairs, NARC_a_1_7_0, 17);
    GF_ASSERT(w != NULL);
    GF_ASSERT(w->course != NULL);
    course = w->course;
    ov96_0220F280(course);
    for (i = 0; i < 4; i++) {
        w->groups[i].index = i;
    }
    for (i = 0; i < 12; i++) {
        slot = i % 3;
        group = i / 3;
        racer = &w->groups[group].racers[slot];
        obj = PokeathlonCourseMonObjects_Get(w->objs, i);
        PokeathlonCourseMonObject_SetDrawFlag(obj, 1);
        {
            int k = GetCourseType(course, i);
            ov96_021EAF70(obj, pairs[k - 1][0], pairs[k - 1][1]);
        }
        obj2 = PokeathlonCourseMonObjects_Get(w->objs, i);
        info = PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(obj2));
        ov96_0220D554(obj2, 0);
        ov96_021EB10C(obj2, 1.0f, 1.0f);
        x = ov96_0221CFBC[group][slot].x;
        y = ov96_0221CFBC[group][slot].y;
        PokeathlonCourseMonObject_SetAnimState(obj2);
        ov96_021EAF94(obj2, x, y);
        ov96_021EAF6C(obj2, ov96_021E6104());
        ov96_021EB0A4(obj2, x, y, &outX, &outY);
        racer->course = course;
        racer->index = i;
        racer->group = group;
        racer->slot = slot;
        racer->obj = obj2;
        racer->type = info->type;
        racer->data = ov96_0220F378(course, group, slot);
        racer->state = 2;
        racer->mode = 2;
        racer->startX = outX * FX32_ONE;
        racer->startY = outY * FX32_ONE;
        racer->x = outX * FX32_ONE;
        racer->y = outY * FX32_ONE;
        if (group == ov96_021E5F24(course)) {
            coords[slot].x = x;
            coords[slot].y = y;
            Sprite_SetDrawPriority(PokeathlonCourse_AddMarker(course, obj2, w->res, SpriteManager_GetSpriteList(w->spriteManager), 1), 4);
            PokeathlonCourseMonObject_SetDrawPriority(obj2, 6);
        } else {
            PokeathlonCourseMonObject_SetDrawPriority(obj2, 7);
        }
        if (ov96_021E5F24(course) == 0) {
            data = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
            data[4 + i] = outX;
            data[0x10 + i] = outY;
        }
    }
    PokeathlonCourse_CreateStartSprites(course, 0, w->res, SpriteManager_GetSpriteList(w->spriteManager), 1, 3, coords);
}
