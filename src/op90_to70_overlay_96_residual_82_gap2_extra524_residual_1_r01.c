#pragma force_active on
#include "global.h"
#include "heap.h"
#include "system.h"
#include "bg_window.h"

typedef struct Mon {
    u16 species;
    u16 form;
    u16 unk04;
    u8 unk06;
    u8 gender;
    u32 unk08;
    u32 personality;
} Mon;

typedef struct RenderOptions {
    u32 v[5];
    u32 types[12];
} RenderOptions;

typedef struct Setup {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    int heapID;
} Setup;

typedef struct Triple {
    u32 v[3];
} Triple;

typedef struct Slot {
    void *sprite;
    u8 unk04[0x1C];
} Slot;

typedef struct Competitor {
    u8 unk00[0x48];
} Competitor;

typedef struct State {
    BgConfig *bg;
    u8 unk04[0x40];
    u32 heap;
    void *renderer;
    u8 unk4C[0x24];
    void *aux[9];
    void *sprites[9];
    Competitor competitors[12];
    Slot slots[12];
    void *auxList;
    u8 unk59C[0x30];
    u8 flags[4];
    u32 unk5D0;
    void *resource;
    void *icons;
    void *secondary;
    void *screen;
    u32 unk5E4;
    u16 duration;
} State;

typedef struct SpriteInfo {
    u8 unk00[4];
    u16 flipped;
} SpriteInfo;

extern const Triple ov96_0221C7B8;
extern const Triple ov96_0221C7D0;
extern const s32 ov96_0221C8C8[9];
extern const s32 ov96_0221C8EC[9];
extern const u8 ov96_0221CA1C[9];

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
u8 PokeathlonCourse_GetField1ED(void *course);
void *PokeathlonCourse_AllocPtr4FromHeap(void *course, u32 size);
int PokeathlonCourse_GetHeapID(void *course);
void PokeathlonCourse_IncrementField1ED(void *course);
u8 PokeathlonCourse_GetParticipantCount(void *course);
u32 PokeathlonCourse_GetMode(void *course);
void PokeathlonCourse_SetVBlankIntrCB(BgConfig *bg);
void PokeathlonCourse_SetField1F4(void *course, int value);
u8 *PokeathlonCourse_GetDataCopyArea(void *course);
int ov96_021E5F24(void *course);
u8 *ov96_021E8A20(void *ptr);
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void GfGfx_SwapDisplay(void);
void ov96_02201C90(void);
void PokeathlonCourse_SetHudPlanes(void *course, int value);
void PokeathlonCourse_InitObjTransfer(Setup *setup, int a, int heapID, int b, int c);
void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(int a, int b, int c, int d, int e, int f, int g, int h, int heapID);
void FontID_Alloc(int id, int heapID);
void ov96_02201CB0(BgConfig *bg);
void ov96_02203310(State *s);
void ov96_0220382C(State *s, void *course);
void *ov96_02203A00(u32 heap, BgConfig *bg, void *course);
void *ov96_02204364(u32 heap, int count, u32 mode);
void *PokeathlonCourseSpritePool_New(u32 heap, Triple *config);
void PokeathlonCourseSpritePool_SetSurfaceCoords(void *renderer, int a, int b, int c, int d);
void PokeathlonCourseSpritePool_SetResourceId(void *renderer, int index, int value);
void ov96_02201E70(void *renderer);
void ov96_02203B44(void *screen, void *renderer);
void PokeathlonCourseSpritePool_TransferResources(void *renderer);
void *PokeathlonCourseSprites_CreateResources(u32 heap, int id, int count);
void *PokeathlonCourseSpritePool_GetSpriteList(void *renderer);
void *PokeathlonCourseMonObjects_Create(u32 heap, int count, int max, void *resource, void *list);
void **ov96_021E6290(void *course, int index, void *resource, void *renderer);
void Sprite_SetDrawPriority(void *sprite, int priority);
void ov96_02201EF0(State *s, void *renderer, u8 selected);
void PokeathlonCourse_GetMonInfo(void *course, int participant, int member, Mon *mon);
void *PokeathlonCourse_GetPerformance(void *course, int participant, int member);
u32 ov96_021E6108(void *mon);
void ov96_02203B8C(void *screen, void *renderer, Mon *mons);
u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 isEgg, u8 form, u32 personality);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *sprite, VecFx32 *pos);
void PokeathlonCourseMonObjects_Load(void *icons, int count, Mon *mons, RenderOptions *options, int a, int b);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *icons);
void *PokeathlonCourseMonObjects_Get(void *icons, u8 index);
void PokeathlonCourseMonObject_SetDrawFlag(void *sprite, int value);
void PokeathlonCourseMonObject_SetAnimState(void *sprite, u32 anim);
void PokeathlonCourseObject_SetGroundPosition(void *sprite, int x, int y);
void *PokeathlonCourseMonObject_GetGfx(void *sprite);
SpriteInfo *PokeathlonCourseMonGfx_GetInfo(void *sprite);
void PokeathlonCourseMonObject_SetDrawPriority(void *sprite, int value);
void ov96_0220329C(void *course, u8 *raw, u8 team, u8 member, Slot *slot);
void ov96_022043AC(void *secondary, u8 index, Slot *slot, Competitor *competitor);
void ov96_02202738(State *s, u8 *data);
void ov96_02203D74(void *screen, void *renderer, void *resource);
void ov96_02201E10(State *s, u8 selected);
void WirelessStatusIcon_CreateOnScreenIfConnected(int value);
void BeginNormalPaletteFade(int a, int b, int c, int d, int e, int f, u32 heap);
BOOL IsPaletteFadeFinished(void);
void ReadWholeNarcMemberByIdPair(void *dest, int narc, int member);
int ov96_02201558(void *course);

// @fn ov96_02201558
int ov96_02201558(void *course) {
    Mon mons[12];
    RenderOptions options;
    u8 raw[80];
    State *s;
    State *newState;
    int participant;
    int monIndex;
    int spriteIndex;
    void *sprite;
    u8 slot;
    int x;
    int team;
    Slot *slotPtr;
    int base;
    int n;
    int selected;
    int member;
    int y, j;

    s = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        Setup setup;

        Heap_Create((enum HeapID)92, (enum HeapID)146, 0x40000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        ov96_02201C90();
        newState = PokeathlonCourse_AllocPtr4FromHeap(course, 0x5F4);
        MI_CpuFill8(newState, 0, 0x5F4);
        newState->auxList = Heap_Alloc((enum HeapID)146, 40);
        MI_CpuFill8(newState->auxList, 0, 40);
        newState->bg = BgConfig_Alloc((enum HeapID)146);
        PokeathlonCourse_SetHudPlanes(course, 8);
        setup.maxTasks = 115;
        setup.sizeMain = 0x40000;
        setup.sizeSub = 0x4000;
        setup.heapID = PokeathlonCourse_GetHeapID(course);
        PokeathlonCourse_InitObjTransfer(&setup, 22, 146, 0x300010, 0x300010);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, 146);
        newState->heap = 146;
        FontID_Alloc(4, 146);
        ov96_02201CB0(newState->bg);
        ov96_02203310(newState);
        ov96_0220382C(newState, course);
        gSystem.screensFlipped = FALSE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1:
        s->screen = ov96_02203A00(s->heap, s->bg, course);
        n = PokeathlonCourse_GetParticipantCount(course);
        s->secondary = ov96_02204364(s->heap, 4 - n, PokeathlonCourse_GetMode(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2: {
        Triple config = ov96_0221C7B8;

        s->renderer = PokeathlonCourseSpritePool_New(s->heap, &config);
        PokeathlonCourseSpritePool_SetSurfaceCoords(s->renderer, 0, 0, 0, FX32_CONST(288));
        PokeathlonCourseSpritePool_SetResourceId(s->renderer, 0, 106);
        PokeathlonCourseSpritePool_SetResourceId(s->renderer, 1, 101);
        PokeathlonCourseSpritePool_SetResourceId(s->renderer, 2, 105);
        ov96_02201E70(s->renderer);
        ov96_02203B44(s->screen, s->renderer);
        PokeathlonCourseSpritePool_TransferResources(s->renderer);
        s->resource = PokeathlonCourseSprites_CreateResources(s->heap, 1255, 1);
        s->icons = PokeathlonCourseMonObjects_Create(s->heap, 9, 32, s->resource, PokeathlonCourseSpritePool_GetSpriteList(s->renderer));
        Sprite_SetDrawPriority(*ov96_021E6290(course, 0, s->resource, s->renderer), 1);
        selected = ov96_021E5F24(course);
        ov96_02201EF0(s, s->renderer, selected);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3:
        selected = (u8)ov96_021E5F24(course);
        for (monIndex = 0; monIndex < 12; monIndex++) {
            n = (u8)(((u8)(((selected + 1) * 3) % 12) + monIndex) % 12);
            member = n % 3;
            participant = n / 3;
            PokeathlonCourse_GetMonInfo(course, participant, member, &mons[monIndex]);
            options.types[monIndex] = ov96_021E6108(PokeathlonCourse_GetPerformance(course, participant, member));
        }
        ov96_02203B8C(s->screen, s->renderer, &mons[9]);
        for (monIndex = 0; monIndex < 3; monIndex++) {
            VecFx32 monPos = { 0, 0, 0 };
            Mon *mon = &mons[9 + monIndex];
            u8 height = GetMonPicHeightBySpeciesGenderForm(mon->species, mon->gender, 0, mon->form, mon->personality);
            monPos.x = (16 + monIndex * 50) << 12;
            monPos.y = (height + 376) << 12;
            monPos.y -= FX32_ONE * 16;
            PokeathlonCourseSpritePoolSprite_SetPosition(s->slots[selected * 3 + monIndex].sprite, &monPos);
        }
        options.v[0] = 0;
        options.v[1] = 3;
        options.v[2] = 0;
        options.v[3] = 1;
        options.v[4] = 1;
        PokeathlonCourseMonObjects_Load(s->icons, 9, mons, &options, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        if (!PokeathlonCourseMonObjects_IsLoaded(s->icons)) {
            break;
        } else {
        Triple anims = ov96_0221C7D0;

        PokeathlonCourse_SetVBlankIntrCB(s->bg);
        PokeathlonCourse_SetField1F4(course, 1);
        selected = ov96_021E5F24(course);
        for (spriteIndex = 0; spriteIndex < 9; spriteIndex++) {
            s->sprites[spriteIndex] = PokeathlonCourseMonObjects_Get(s->icons, spriteIndex);
            sprite = s->sprites[spriteIndex];
            PokeathlonCourseMonObject_SetDrawFlag(sprite, 1);
            slot = ((u8)(((selected + 1) * 3) % 12) + spriteIndex) % 12;
            x = ov96_0221C8C8[spriteIndex];
            y = ov96_0221C8EC[spriteIndex];
            PokeathlonCourseMonObject_SetAnimState(sprite, anims.v[spriteIndex / 3]);
            PokeathlonCourseObject_SetGroundPosition(sprite, x, y);
            {
                VecFx32 iconPos = { 0, 0, 0 };
                if (PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(sprite))->flipped) {
                    iconPos.x = x << 12;
                    iconPos.y = (y - 40) << 12;
                } else {
                    iconPos.x = x << 12;
                    iconPos.y = (y - 25) << 12;
                }
                PokeathlonCourseSpritePoolSprite_SetPosition(s->slots[slot].sprite, &iconPos);
                PokeathlonCourseSpritePoolSprite_SetPosition(s->aux[spriteIndex], &iconPos);
            }
            PokeathlonCourseMonObject_SetDrawPriority(sprite, ov96_0221CA1C[spriteIndex] + 32);
        }
        ReadWholeNarcMemberByIdPair(raw, 170, 3);
        s->duration = 1800;
        for (team = 0, base = 0; team < 4; base += 3, team++) {
            s->flags[team] = 12;
            for (j = 0; j < 3; j++) {
                slotPtr = &s->slots[j + base];
                ov96_0220329C(course, raw, team, j, slotPtr);
                if (ov96_021E5F24(course) == 0) {
                    ov96_022043AC(s->secondary, j + base, slotPtr, &s->competitors[j + base]);
                }
            }
        }
        if (ov96_021E5F24(course) == 0) {
            ov96_02202738(s, ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 40));
        }
        ov96_02203D74(s->screen, s->renderer, s->resource);
        ov96_02201E10(s, ov96_021E5F24(course));
        GfGfx_EngineATogglePlanes(16, 1);
        GfGfx_EngineBTogglePlanes(16, 1);
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, s->heap);
        PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            if (!GF_heap_c_dummy_return_true((enum HeapID)92)) {
                GF_AssertFail();
            }
            return 1;
        }
        break;
    }
    return 0;
}
