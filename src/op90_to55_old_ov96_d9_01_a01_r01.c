#pragma force_active on
#include "global.h"
#include <nitro/fx/fx_vec.h>

typedef struct Ov96Object {
    void *course;
    void *sprite;
    void *data;
    u32 state;
    VecFx32 previous;
    VecFx32 position;
    VecFx32 target;
    VecFx32 velocity;
    u32 timer : 8;
    u32 unk8 : 21;
    u32 flag29 : 1;
    u32 unk30 : 2;
    u8 wait;
    u8 row;
    u8 column;
    u8 field47;
} Ov96Object;

void ov96_0220D744(Ov96Object *, void *);
BOOL ov96_0220D8C4(Ov96Object *, fx32, fx32);
void ov96_021E8228(void *, u8, u8, u8, u32);
void ov96_0220D910(Ov96Object *);
void ov96_0220D9A4(Ov96Object *obj, void *arg);

// @fn ov96_0220D9A4
void ov96_0220D9A4(Ov96Object *obj, void *arg) {
    switch (obj->state) {
    case 4:
        obj->state = 2;
        break;
    case 3:
        obj->wait--;
        if (obj->wait == 0) {
            ov96_0220D910(obj);
            obj->state = 4;
        }
        break;
    case 2:
        ov96_0220D744(obj, arg);
        if (ov96_0220D8C4(obj, obj->position.x, obj->position.y)) {
            ov96_021E8228(obj->course, obj->row, obj->column, 1, 1);
            ov96_021E8228(obj->course, obj->row, obj->column, 5, 1);
            if (obj->flag29) {
                ov96_021E8228(obj->course, obj->row, obj->column, 8, 1);
            }
            obj->flag29 = 0;
            obj->wait = 90;
            obj->state = 3;
        }
        break;
    case 1:
        obj->timer--;
        if (obj->timer == 0) {
            obj->state = 2;
        }
        ov96_0220D744(obj, arg);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}
