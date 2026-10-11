#include <nitro/fx/fx_vec.h>

#include "global.h"

#include "geometry_math.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define VEC_AT(p, o) (*(VecFx32 *)((u8 *)(p) + (o)))

extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern BOOL PokeathlonCourse_AddMonCounter(void *, u8, u8, u32, u32);
extern BOOL ov96_021F218C(void *, void *);
extern BOOL ov96_021F22FC(void *, void *, u8, const VecFx32 *);
extern void ov96_021F234C(void *);
extern void ov96_021F2AA4(void *);
extern BOOL ov96_021F2F7C(void *, fx32, fx32);
extern const VecFx32 ov96_0221BC64;
typedef struct ShopCell {
    u8 raw[0x90];
} ShopCell;
typedef struct ShopWork {
    u8 pad00[0x20];
    ShopCell cells[4][3];
    u8 pad6E0[0x100];
    u8 flags[12];
} ShopWork;
void ov96_021F1CC0(void *course);

static inline void *GetCell(ShopWork *work, int row, int column) {
    return &work->cells[row][column];
}

void ov96_021F1CC0(void *course) {
    ShopWork *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;

    for (i = 0; i < 12; i++) {
        int column = i % 3;
        int row = i / 3;
        void *cell = GetCell(work, row, column);

        if (U8_AT(cell, 0x46) != 0) {
            S8_AT(cell, 0x47)
            --;
            if (S8_AT(cell, 0x47) <= 0) {
                U8_AT(cell, 0x46) = 0;
            }
        }
        if (U32_AT(cell, 0x18) != 0
            && ov96_021F2F7C(PTR_AT(cell, 0), VEC_AT(cell, 0x28).x, VEC_AT(cell, 0x28).y)) {
            U32_AT(cell, 0x18) = 0;
            U8_AT(cell, 0x45) = 0;
            U8_AT(cell, 0x44) = 0;
            PokeathlonCourse_AddMonCounter(course, row, column, 5, 1);
            PokeathlonCourse_AddMonCounter(course, row, column, 1, 1);
            if (work->flags[i] != 0) {
                PokeathlonCourse_AddMonCounter(course, row, column, 8, 1);
            }
            work->flags[i] = 0;
        }
    }

    for (i = 0; i < 12; i++) {
        int row = i / 3;
        int column = i % 3;
        void *cell = GetCell(work, row, column);

        if (U32_AT(cell, 0x18) == 2) {
            BOOL clear = TRUE;
            int j;
            for (j = 0; j < 12; j++) {
                int otherRow;
                int otherColumn;
                void *other;
                if (i == j) {
                    continue;
                }
                otherRow = j / 3;
                otherColumn = j % 3;
                other = GetCell(work, otherRow, otherColumn);
                if (U32_AT(other, 0x18) != 0 && ov96_021F218C(cell, other)) {
                    clear = FALSE;
                    break;
                }
            }
            if (clear) {
                U32_AT(cell, 0x18) = 1;
            }
        }
    }

    for (i = 0; i < 12; i++) {
        int row = i / 3;
        int column = i % 3;
        void *cell = GetCell(work, row, column);
        VecFx32 delta;
        VecFx32 normal;
        VecFx32 previous;
        fx32 distance;

        if (U32_AT(cell, 0x18) == 0) {
            continue;
        }
        U8_AT(cell, 0x41) = 0;
        VEC_AT(cell, 0x34).x = 0;
        VEC_AT(cell, 0x34).y = 0;
        VEC_AT(cell, 0x34).z = 0;
        previous = VEC_AT(cell, 0x28);
        VEC_Subtract(&VEC_AT(cell, 0x1C), &previous, &delta);
        distance = VEC_Mag(&delta);
        if (distance <= 0) {
            ov96_021F2AA4(cell);
            continue;
        }
        VEC_Normalize(&delta, &normal);
        {
            float speed;
            if (U8_AT(cell, 0x44) != 0) {
                speed = 0.5f;
            } else {
                speed = (float)U8_AT(cell, 0x89) / 10.0;
            }
            if ((float)distance <= 4096.0f * speed) {
                previous = VEC_AT(cell, 0x1C);
            } else {
                VecFx32 zero = { 0, 0, 0 };
                VEC_MultAdd((fx32)(4096.0f * speed), &normal, &zero, &delta);
                VEC_Add(&delta, &previous, &previous);
            }
        }
        if (distance != 0) {
            VecFx32 reference = ov96_0221BC64;
            VecFx32 direction;
            u16 angle;
            direction.x = normal.y;
            direction.z = normal.x;
            direction.y = 0;
            angle = CalcAngleBetweenVecs(&reference, &direction);
            if (angle <= 0x2000 || angle >= 0xE000) {
                U8_AT(cell, 0x40) = 4;
            } else if (angle > 0x2000 && angle < 0x6000) {
                U8_AT(cell, 0x40) = 2;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                U8_AT(cell, 0x40) = 3;
            } else {
                U8_AT(cell, 0x40) = 1;
            }
        }
        if (U32_AT(cell, 0x18) == 1) {
            if (!ov96_021F22FC(work, cell, (u8)i, &delta)) {
                VecFx32 zero = { 0, 0, 0 };
                float speed;
                if (U8_AT(cell, 0x44) != 0) {
                    continue;
                }
                U8_AT(cell, 0x41) = 1;
                speed = (float)((float)U16_AT(cell, 0x8A) / 10.0) + (float)U8_AT(cell, 0x46);
                VEC_MultAdd((fx32)(4096.0f * speed), &normal, &zero, &VEC_AT(cell, 0x34));
                if (U16_AT(cell, 0x8E) != 0) {
                    U16_AT(cell, 0x8E)
                    --;
                } else {
                    GF_AssertFail();
                }
            } else {
                ov96_021F2AA4(cell);
                VEC_AT(cell, 0x28) = previous;
            }
        } else {
            U16_AT(cell, 0x8E) = 0;
            VEC_AT(cell, 0x28) = previous;
        }
    }
    ov96_021F234C(work);
}
