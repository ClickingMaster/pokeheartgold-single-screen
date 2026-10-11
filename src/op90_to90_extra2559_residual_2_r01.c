#pragma force_active on
#include "global.h"
#include "system.h"

#define PTR8(p, o) (*(void **)((u8 *)(p) + (o)))

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void *PokeathlonCourse_GetDataCopyArea(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_021EB63C(void *obj, int value);
void ov96_021EB144(void *obj, int value);
BOOL ov96_022038D4(void *course);
void PokeathlonCourse_SetFinishCallback(void *course, BOOL (*func)(void *));
int ov96_021E5F24(void *course);
void PokeathlonCourse_AddMonCounter(void *course, u8 participant, u8 slot, u32 event, u32 value);
BOOL System_GetTouchNew(void);
BOOL System_GetTouchHeld(void);
void ov96_02202958(void *course);
BOOL ov96_022028BC(void *course);

// @fn ov96_022028BC
BOOL ov96_022028BC(void *course) {
    u8 *work;
    u8 *copy;
    u8 *touch;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    copy = PokeathlonCourse_GetDataCopyArea(course);
    touch = (void *)ov96_021E8A20(copy);
    *(u32 *)touch = 0;
    copy = (void *)ov96_021E8A20(copy + 0xf0);
    {
        u8 flag = ((s32) * (u32 *)(copy + 0x20) >> 24) & 1;
        if (flag) {
            ov96_021EB63C(PTR8(work, 0x48), 1);
            ov96_021EB144(PTR8(work, 0x5d8), 1);
            PokeathlonCourse_SetFinishCallback(course, ov96_022038D4);
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        touch[4] = gSystem.touchX;
        touch[5] = gSystem.touchY;
        *(u32 *)touch = 1;
    }
    ov96_02202958(course);
    return FALSE;
}
