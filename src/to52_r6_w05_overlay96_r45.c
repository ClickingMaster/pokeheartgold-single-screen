#include "to52_r6_w05_overlay96_r45_private.h"

typedef struct Ov96SceneWork {
    int heapId;
    int unk04;
    void *object;
} Ov96SceneWork;

typedef struct Ov96LookupField {
    int value;
    int padding;
} Ov96LookupField;

extern const Ov96LookupField ov96_0221B1D4[];
extern const Ov96LookupField ov96_0221B1D8[];

void ov96_021EED70(void **sprite, int narc, int member, int useMain, int tint, int heap);
int ov96_021EEDCC(int key);
int ov96_021EEE0C(void *course);

void ov96_021EED70(void **sprite, int narc, int member, int useMain, int tint, int heap) {
    int type;
    int location;
    u32 offset;

    if (useMain) {
        type = 1;
        location = 1;
    } else {
        type = 2;
        location = 5;
    }
    offset = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(*sprite), (NNS_G2D_VRAM_TYPE)type);
    GfGfxLoader_GXLoadPal(narc, member, location, offset, 0x20, heap);
    if (tint) {
        void *palette;
        if (useMain) {
            palette = (void *)0x05000200;
        } else {
            palette = (void *)0x05000600;
        }
        TintPalette_GrayScale((void *)(offset + (u32)palette), 0x20);
    }
}

int ov96_021EEDCC(int key) {
    int i;

    for (i = 0; i < 9999; i++) {
        if (i == 0xffff) {
            return 0;
        }
        if (key == ov96_0221B1D4[i].value) {
            return ov96_0221B1D8[i].value;
        }
    }
    GF_AssertFail();
    return 0;
}

int ov96_021EEE0C(void *course) {
    Ov96SceneWork *work = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        ov96_021EEFAC(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        GfGfx_EngineATogglePlanes(0x10, 1);
        GfGfx_EngineBTogglePlanes(0x10, 0);
        BeginNormalPaletteFade(0, 1, 1, 0, 4, 1, work->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            ov96_021EE8CC(work->object, 0x136);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 3:
        if (!ov96_021EE830(work->object)) {
            ov96_021EE994(work->object);
            PokeathlonCourse_SetStateTransitionType(course, 0x11);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
