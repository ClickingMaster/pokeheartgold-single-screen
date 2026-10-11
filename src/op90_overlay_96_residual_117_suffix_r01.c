#pragma force_active on
#include "global.h"
#include "filesystem.h"
#include "sprite_system.h"
typedef struct Info {
    u32 unk0, unk4, value;
} Info;
typedef struct Item {
    void *object;
    Info *info;
    u32 value;
    u32 index : 4;
    u32 rest : 28;
} Item;
typedef struct Entity {
    void *course;
    Item *items;
    void *region;
    u32 value;
    u8 pad10[0x10];
    fx32 x20, y20;
    u8 pad28[4];
    fx32 x2c, y2c;
    u8 pad34[0x54 - 0x34];
    u32 unk54;
    u8 pad58[0x5f - 0x58];
    u8 unk5F;
    u32 flags0 : 16;
    u32 flags16 : 4;
    u32 flags20 : 2;
    u32 flags22 : 10;
    u8 pad64[4];
    Item items_[3];
    u32 ordinal;
} Entity;
typedef struct Region {
    u32 hdr[3];
    Entity entity;
} Region;
typedef struct Work {
    u8 pad00[0xc];
    SpriteManager *spriteManager;
    u8 pad10[8];
    void *resource;
    void *objects;
    u8 pad20[0x180 - 0x20];
    u32 value;
    u8 pad184[0x198 - 0x184];
    Region regions[4];
} Work;
typedef struct Pos { s32 x, y; } Pos;
typedef struct Anchor { s16 x, y; } Anchor;
extern const s16 ov96_0221D6E4[4][2];
void ov96_0221918C(void *course);
void *PokeathlonCourseMonObjects_Get(void *objects, u8 idx);
void PokeathlonCourseMonObject_SetDrawFlag(void *obj, u8 flag);
void *PokeathlonCourse_GetPerformance(void *course, u8 row, u8 col);
int ov96_021E6138(void *);
void ov96_021EAF70(void *obj, s32 x, s32 y);
void PokeathlonCourseMonObject_GetGfx(void *obj);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(void *obj, int);
void ov96_02218510(void *obj, int);
u32 ov96_021E5F24(void *course);
void PokeathlonCourseMonObject_SetAnimState(void *obj, int);
void PokeathlonCourseMonObject_SetDrawPriority(void *obj, int);
void ov96_021EAF94(void *obj, s32 x, s32 y);
int ov96_021E6104(void);
void ov96_021EAF6C(void *obj, int);
void ov96_021EB0A4(void *obj, s32 x, s32 y, s32 *outX, s32 *outY);
Info *ov96_0221935C(void *course, u8 row, u8 col);
void PokeathlonCourse_CreateStartSprites(void *course, int, void *res, void *list, int, int, Anchor *anchor);
void ov96_02218578(Entity *e, int);
void ov96_02217B84(Work *w, void *course);
// @fn ov96_02217B84
void ov96_02217B84(Work *w, void *course) {
    Pos coords[3] = {0};
    struct {
        s32 py2, px2, py, px;
        Anchor anchor;
    } t;
    int i;
    u8 row, col;
    void *obj;
    Item *item;
    s16 x, y;
    Entity *e;

    ReadWholeNarcMemberByIdPair(coords, (NarcId)170, 19);
    GF_ASSERT(w != NULL);
    ov96_0221918C(course);
    for (i = 0; i < 12; i++) {
        int n;
        row = i / 3;
        col = i % 3;
        item = &w->regions[row].entity.items_[col];
        obj = PokeathlonCourseMonObjects_Get(w->objects, i);
        PokeathlonCourseMonObject_SetDrawFlag(obj, col == 0);
        n = ov96_021E6138(PokeathlonCourse_GetPerformance(course, row, col));
        ov96_021EAF70(obj, coords[n - 1].x, coords[n - 1].y);
        obj = PokeathlonCourseMonObjects_Get(w->objects, i);
        PokeathlonCourseMonObject_GetGfx(obj);
        PokeathlonCourseMonObject_SetAffineOverwriteMode(obj, 2);
        ov96_02218510(obj, 1);
        x = ov96_0221D6E4[row][0];
        y = ov96_0221D6E4[row][1];
        if (row == ov96_021E5F24(course)) {
            t.anchor.x = x;
            t.anchor.y = y;
        }
        PokeathlonCourseMonObject_SetAnimState(obj, 2);
        PokeathlonCourseMonObject_SetDrawPriority(obj, 7);
        ov96_021EAF94(obj, x, y);
        ov96_021EAF6C(obj, ov96_021E6104());
        ov96_021EB0A4(obj, x, y, &t.px, &t.py);
        item->index = i;
        item->object = obj;
        item->info = ov96_0221935C(course, row, col);
        item->value = item->info->value;
    }
    PokeathlonCourse_CreateStartSprites(course, 0, w->resource, SpriteManager_GetSpriteList(w->spriteManager), 1, 1, &t.anchor);
    for (i = 0; i < 4; i++) {
        e = &w->regions[i].entity;
        e->region = &w->regions[i];
        e->items = w->regions[i].entity.items_;
        e->value = w->value;
        e->course = course;
        ov96_021EB0A4(e->items->object, ov96_0221D6E4[i][0], ov96_0221D6E4[i][1], &t.px2, &t.py2);
        ov96_02218578(e, 1);
        e->flags16 = 2;
        e->x2c = t.px2 << FX32_SHIFT;
        e->y2c = t.py2 << FX32_SHIFT;
        e->x20 = t.px2 << FX32_SHIFT;
        e->y20 = t.py2 << FX32_SHIFT;
        e->flags20 = i;
        e->unk54 = 0;
        e->unk5F = 3;
        w->regions[i].entity.ordinal = i;
    }
}
