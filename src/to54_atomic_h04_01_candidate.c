#include "to54_atomic_h04_private.h"

void ov96_0220DEAC(void *a, void *b, void *course) {
    SortRec recs[12] = { 0 };
    s32 x;
    s32 y;
    s32 u;
    s32 v;
    f32 f;
    void *w;
    void *e;
    u8 flag;
    SortRec *rp;
    void *spr;
    u8 key;
    int i;
    int sh;

    w = PokeathlonCourse_GetHeapAllocPtr4(course);
    e = (u8 *)a + 0x3c;
    i = 0;
    rp = recs;
    sh = 0;
    for (; i < 12; i++) {
        spr = PokeathlonCourseMonObjects_Get(PTR(w, 0x20), (u8)i);
        if (e == NULL) {
            GF_AssertFail();
        }
        if (spr == NULL) {
            GF_AssertFail();
        }
        key = U8((u8 *)b + (u32)i, 0x10);
        PokeathlonCourseObject_GroundToCenter(spr, U8((u8 *)b + (u32)i, 4), key, &x, &y);
        rp->key = key;
        rp->spr = spr;
        flag = (u8)((BITS(b, 0x20).m.m12 >> i) & 1);
        if (flag == 1 && BITS(e, 0x18).e.f19 == 0) {
            ov96_0220D554(spr, 1);
            BITS(e, 0x18).e.f18 = 1;
            BITS(e, 0x18).e.b = 6;
            S32(e, 0x14) = 0x1000;
            PlaySE(0x89E);
        } else if (flag == 0 && BITS(e, 0x18).e.f19 == 1) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(PTR(e, 0), (s16)x, (s16)y, 0x1E0000);
            ManagedSprite_SetDrawFlag(PTR(e, 0), 1);
            ManagedSprite_ResetSpriteAnimCtrlState(PTR(e, 0));
            ov96_0220D554(spr, 0);
            PokeathlonCourseObject_SetScale(spr, 1.0f, 1.0f);
            PokeathlonCourseMonObject_SetDrawFlag(spr, 1);
            BITS(e, 0x18).e.f18 = 0;
            PlaySE(0x8B6);
        }
        BITS(e, 0x18).e.f19 = flag;
        if (BITS(e, 0x18).e.f18) {
            BITS(e, 0x18).e.b--;
            if (BITS(e, 0x18).e.b == 0) {
                PokeathlonCourseMonObject_SetDrawFlag(spr, 0);
            } else {
                f32 t;
                S32(e, 0x14) -= 0x19A;
                t = (f32)S32(e, 0x14) / 4096.0f;
                PokeathlonCourseObject_SetScale(spr, t, t);
                PokeathlonCourseObject_CenterToGround(spr, U8((u8 *)b + (u32)i, 4), U8((u8 *)b + (u32)i, 0x10), &u, &v);
                if (v >= 180) {
                    u32 bb = (6 - BITS(e, 0x18).e.b) * 4;
                    u32 sum = key;
                    sum += bb;
                    PokeathlonCourseObject_SetCenterPosition(spr, x, sum, 1);
                }
            }
        } else {
            u8 g = (u8)((BITS(b, 0x1c).n.w24 >> sh) & 3);
            PokeathlonCourseObject_SetCenterPosition(spr, x, y, 1);
            PokeathlonCourseMonObject_SetAnimState(spr, g + 1);
            if ((BITS(b, 0x20).m.n12 >> i) & 1) {
                void *rec;
                int q;
                q = i / 3;
                rec = ov96_0220F378(course, (u8)q, (u8)(i % 3));
                if (q == (int)ov96_021E5F24(course) && BITS(e, 0x18).e.f16 == 0 && BITS(e, 0x18).e.f17 == 0) {
                    if (U8(rec, 2) == 0) {
                        PlaySE(0x5F3);
                    } else {
                        PlaySE(0x8C5);
                    }
                }
                if (U8(rec, 2) == 0) {
                    ov96_0220D52C((u8 *)e + 4, spr);
                    BITS(e, 0x18).e.f17 = 1;
                } else {
                    BITS(e, 0x18).e.f16 = 1;
                }
            } else {
                BITS(e, 0x18).e.f17 = 0;
            }
            if (BITS(e, 0x18).e.f16) {
                void *rec2;
                u32 n;
                rec2 = ov96_0220F378(course, (u8)(i / 3), (u8)(i % 3));
                PokeathlonCourseObject_SetCenterPosition(spr, x, y + ov96_0220E8C0(rec2, BITS(e, 0x18).e.a, &f), 0);
                PokeathlonCourseObject_SetScale(spr, f, f);
                n = ov96_0220F3B4(rec2);
                BITS(e, 0x18).e.a++;
                if (BITS(e, 0x18).e.a > n) {
                    BITS(e, 0x18).e.f16 = 0;
                    BITS(e, 0x18).e.a = 0;
                    PokeathlonCourseObject_SetCenterPosition(spr, x, y, 0);
                    PokeathlonCourseObject_SetScale(spr, 1.0f, 1.0f);
                }
            }
        }
        rp++;
        sh += 2;
        e = (u8 *)e + 0x1c;
    }
    MATH_QSort(recs, 12, sizeof(SortRec), ov96_0220DE90, NULL);
    sh = 0;
    rp = recs;
    for (; sh < 12; sh++) {
        if (rp->spr == NULL) {
            GF_AssertFail();
        }
        PokeathlonCourseMonObject_SetDrawPriority(rp->spr, sh + 7);
        rp++;
    }
}
