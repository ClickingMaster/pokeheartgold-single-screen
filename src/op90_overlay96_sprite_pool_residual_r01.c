#pragma force_active on
#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "filesystem.h"
#include "heap.h"
#include "screen_fade.h"

typedef struct Coord {
    u16 x;
    u16 y;
} Coord;

typedef struct ObjB {
    BOOL active;
    void *sprite;
    u16 x;
    u16 y;
} ObjB;

typedef struct ObjA {
    BOOL active;
    void *sprite;
    void *sprite2;
    u16 x;
    u16 y;
} ObjA;

typedef struct Work {
    u8 unk0[0x14];
    enum HeapID heapId;
    u8 unk18[0x36C - 0x18];
    void *unk36C;
    u8 unk370[8];
    s32 fileId;
    ObjB objsB[20];
    ObjA objsA[10];
    u8 unk50C[0x564 - 0x50C];
    u8 unk564[0x5C8 - 0x564];
    Coord posC[20];
    u32 unk618;
    Coord posB[20];
    Coord posA[10];
} Work;

typedef struct Layout {
    u8 a[10][2];
    u8 b[20][2];
    u8 c[20][2];
} Layout;

u8 *ov96_021E8A20(void *area);
void ov96_021EB52C(void *sprite, int a, int b);
BOOL ov96_021E5F24(PokeathlonCourseData *data);
void PokeathlonPennantCapture_RefillPennants(Work *work);
void PokeathlonPennantCaptureCpu_SetObstacles(void *unk, u8 (*pairs)[2]);
void PokeathlonPennantCaptureCpu_SetPennants(void *unk, void *arg);
void PokeathlonPennantCapture_UpdateFieldDrawPriorities(Work *work);
void ov96_02205D30(PokeathlonCourseData *data);
BOOL ov96_022052B0(PokeathlonCourseData *data, u8 *state);

// @fn ov96_022052B0
BOOL ov96_022052B0(PokeathlonCourseData *data, u8 *state) {
    Layout layout;
    int i;
    u16 x, y;
    Work *work = PokeathlonCourse_GetHeapAllocPtr4(data);
    u8 *pa, *pb, *pc;

    switch (*state) {
    case 0:
        ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
        ReadWholeNarcMemberByIdPair(&layout, (NarcId)0xE3, work->fileId);
        pc = layout.c[0];
        pa = layout.a[0];
        for (i = 0, pb = layout.b[0]; i < 20; pb += 2, i++) {
            y = pb[1] * 8;
            x = pb[0] * 8;
            work->posB[i].x = x;
            work->posB[i].y = pb[1] * 8;
            if (x != 0 && y != 0) {
                work->objsB[i].active = TRUE;
                work->objsB[i].x = x;
                work->objsB[i].y = y;
                ov96_021EB52C(work->objsB[i].sprite, 1, 1);
            }
        }
        for (i = 0; i < 10; pa += 2, i++) {
            y = pa[1] * 8;
            x = pa[0] * 8;
            work->posA[i].x = x;
            work->posA[i].y = pa[1] * 8;
            if (x != 0 && y != 0) {
                work->objsA[i].active = TRUE;
                work->objsA[i].x = x;
                work->objsA[i].y = y;
                ov96_021EB52C(work->objsA[i].sprite, 1, 1);
                ov96_021EB52C(work->objsA[i].sprite2, 1, 1);
            }
        }
        for (i = 0; i < 20; i++, pc += 2) {
            work->posC[i].x = pc[0] * 8;
            work->posC[i].y = pc[1] * 8;
        }
        if (!ov96_021E5F24(data)) {
            PokeathlonPennantCapture_RefillPennants(work);
            PokeathlonPennantCaptureCpu_SetObstacles(work->unk36C, layout.b);
            PokeathlonPennantCaptureCpu_SetObstacles(work->unk36C, layout.a);
            PokeathlonPennantCaptureCpu_SetPennants(work->unk36C, work->unk564);
        }
        PokeathlonPennantCapture_UpdateFieldDrawPriorities(work);
        GF_ASSERT(GF_heap_c_dummy_return_true((enum HeapID)0x5C));
        (*state)++;
        PokeathlonCourse_SetStateTransitionType(data, 0x13);
        break;
    case 1:
        BeginNormalPaletteFade((enum FadeMode)2, (enum FadeType)3, (enum FadeType)3, 0, 6, 1, work->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(data, 1);
        }
        break;
    }
    ov96_02205D30(data);
    return FALSE;
}
