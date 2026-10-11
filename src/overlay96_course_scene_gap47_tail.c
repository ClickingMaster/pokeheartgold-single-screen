#include "overlay96_course_scene_gap47_internal.h"
void ov96_021EF05C(void *course) {
    Ov96R47Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    WirelessStatusIcon_Destroy();
    ov96_021EE808(w->sub);
    ov96_021EEF98(w);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    Heap_Destroy(0x9f);
}
int ov96_021EF094(void *course, u8 *state) {
    Ov96R47Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        if (++w->timer > 15) {
            ov96_021EE9BC(w->sub);
            BeginNormalPaletteFade(0, 0, 0, 0, 4, 1, w->heap);
            (*state)++;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            ov96_021EE944(w->sub);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
int ov96_021EF0FC(void *course) {
    Ov96R47Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        ov96_021EF2C0(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        ov96_021EF818(w);
        ov96_021EF4D0(w->bg, w->timer);
        w->obj34 = ov96_021EE5B4(w->kind, w->timer);
        ov96_021EE60C(w->obj34, w->bg);
        ov96_021EF54C(w);
        ov96_021EE644(w->obj34);
        ov96_021EFA28(w);
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        GfGfx_EngineATogglePlanes(0x10, 1);
        GfGfx_EngineBTogglePlanes(0x10, 1);
        PokeathlonCourse_SetField1F4(course, 1);
        PokeathlonCourse_SetField1ED(course, 0);
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
