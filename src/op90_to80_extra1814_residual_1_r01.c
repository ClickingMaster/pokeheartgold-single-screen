#pragma force_active on
#include "global.h"
typedef struct {
    u32 active;
    void *obj;
    u16 x;
    u8 y;
    u8 kind;
} Ent;
typedef struct {
    u8 pad[0x14];
    int heapId;
    u8 pad18[0x45c - 0x18];
    s32 packed[30];
    Ent ents[30];
} Work;
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *obj, int anim);
void PokeathlonCourseSpritePoolSprite_Show(void *obj, int a, int b);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *obj, VecFx32 *pos);
void ov96_021FFB44(Ent *ent);
void ov96_021FEAEC(Work *work, int a);
void PokeathlonCourse_SetStateTransitionType(void *course, int type);
void PokeathlonCourse_SetStateField07(void *course, int value);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
BOOL IsPaletteFadeFinished(void);
BOOL ov96_021FD2E0(void *course, u8 *state);
// @fn ov96_021FD2E0
BOOL ov96_021FD2E0(void *course, u8 *state) {
    int i;
    Work *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    VecFx32 pos;
    switch (*state) {
    case 0:
        for (i = 0; i < 30; i++) {
            u8 kind = work->packed[i];
            u8 y = work->packed[i] >> 8;
            u16 x = work->packed[i] >> 16;
            if (kind != 0) {
                work->ents[i].active = TRUE;
                work->ents[i].kind = kind;
                work->ents[i].x = x;
                work->ents[i].y = y;
                PokeathlonCourseSpritePoolSprite_SetAnim(work->ents[i].obj, kind - 1);
                PokeathlonCourseSpritePoolSprite_Show(work->ents[i].obj, 1, 1);
                pos.z = 0;
                pos.x = (x + 0x50) << FX32_SHIFT;
                pos.y = (y + 0x20) << FX32_SHIFT;
                PokeathlonCourseSpritePoolSprite_SetPosition(work->ents[i].obj, &pos);
                ov96_021FFB44(&work->ents[i]);
            }
        }
        ov96_021FEAEC(work, 0);
        (*state)++;
        PokeathlonCourse_SetStateTransitionType(course, 0x12);
        break;
    case 1:
        BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, work->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}
