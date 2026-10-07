#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x10];
    int x10;
} Sub;
typedef struct {
    u16 a;
    u16 b;
} Pos;
typedef struct {
    u8 pad0[0x1e0];
    Sub *x1e0;
    u8 pad1e4[0x288 - 0x1e4];
    u32 x288;
    u8 x28c[0x2b4 - 0x28c];
    u8 x2b4[0x5e0 - 0x2b4];
    Pos x5e0[4];
} Course;
void *ov96_021E8A20(void *block);
BOOL ov96_021E5F24(Course *course);
int ov96_021E9A14(void);
BOOL ov96_021E87B4(int a, void *b, int c, u32 d);
int PokeathlonCourse_GetUnkConstant4(Course *course);
int PokeathlonCourse_GetParticipantCount(Course *course);
BOOL ov96_021E75E4(Course *course);
void PokeathlonCourse_SetStateField07(Course *course, int value);
void *memcpy(void *dst, const void *src, u32 size);
BOOL ov96_021E6F2C(Course *course);
// @fn ov96_021E6F2C
BOOL ov96_021E6F2C(Course *course) {
    BOOL done;
    if (course->x1e0->x10 != 0) {
        Pos *pos = ov96_021E8A20(course->x28c);
        int idx = ov96_021E5F24(course);
        pos->a = course->x5e0[idx].a;
        pos->b = course->x5e0[idx].b;
        done = ov96_021E87B4(0x1d, course->x28c, ov96_021E9A14(), course->x288);
        if (done && !ov96_021E5F24(course)) {
            int size = PokeathlonCourse_GetUnkConstant4(course);
            int i = PokeathlonCourse_GetParticipantCount(course);
            for (; i < 4; i++) {
                memcpy((u8 *)ov96_021E8A20(course->x2b4) + i * size, &course->x5e0[i], size);
            }
        }
    } else if (!ov96_021E5F24(course)) {
        done = ov96_021E75E4(course);
    } else {
        done = TRUE;
    }
    if (done) {
        PokeathlonCourse_SetStateField07(course, 0x26);
    }
    return FALSE;
}
