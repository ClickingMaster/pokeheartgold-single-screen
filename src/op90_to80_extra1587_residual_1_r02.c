#pragma force_active on
#include "global.h"
typedef struct {
    u32 active;
    u32 x4;
    u16 x8;
    u8 pad[2];
} Ent;
typedef struct {
    u8 pad[0x45c];
    u32 types[30];
    Ent ents[30];
} Obj;
void ov96_021FF0BC(Obj *obj, fx32 x, u32 **listA, u32 **listB, u8 *countA, u8 *countB);
// @fn ov96_021FF0BC
void ov96_021FF0BC(Obj *obj, fx32 x, u32 **listA, u32 **listB, u8 *countA, u8 *countB) {
    int i;
    int xi;
    for (i = 0; i < 30; i++) {
        listA[i] = NULL;
        listB[i] = NULL;
    }
    *countA = 0;
    *countB = 0;
    xi = x / FX32_ONE;
    for (i = 0; i < 30; i++) {
        u8 type;
        int d;
        int v;
        if (obj->ents[i].active == 0) {
            return;
        }
        type = obj->types[i];
        d = xi - obj->ents[i].x8;
        if (d >= 768.0) {
            v = 80.0 + (1024.0 - d);
        } else if (d <= -768.0) {
            v = 80.0 - (1024.0 + d);
        } else {
            v = 80 - d;
        }
        if (v >= -32 && v <= 288) {
            if (type == 3) {
                listB[(*countB)++] = &obj->types[i];
            } else {
                listA[(*countA)++] = &obj->types[i];
            }
        }
    }
}
