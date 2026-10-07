#pragma force_active on
#include "global.h"
typedef struct {
    void *x0;
    u8 pad4[0x58];
    u8 x5c;
} Obj;
void *PokeathlonCourse_GetGraphicsSystem(void *course);
int ov96_021E9524(void *gfx);
void *ov96_021E94EC(void *gfx, u8 idx);
void ov96_021F8830(void *a, int b, int c, void *d);
void ov96_021F8378(Obj *obj, void *course, int a2, int a3);
// @fn ov96_021F8378
void ov96_021F8378(Obj *obj, void *course, int a2, int a3) {
    void *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int n = ov96_021E9524(gfx) - 1;
    void *res = ov96_021E94EC(gfx, n - obj->x5c);
    obj->x5c++;
    ov96_021F8830(obj->x0, a2, a3, res);
}
