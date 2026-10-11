#include "pokeathlon/pokeathlon_hurdle_dash.h"

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "wireless_status_icon.h"

// Course helpers, sprite scene and the rest of the event (OVY_96, not yet tidied).
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
int ov96_021E6108(void *mon);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonHurdleDashMonView *view);
Sprite **ov96_021E6290(PokeathlonCourseData *course, int a1, void *a2, void *scene);
void ov96_021E634C(PokeathlonCourseData *course, int a1, void *a2, void *scene, u32 a4, u8 a5, const u16 (*a6)[2]);
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, u32 a1);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
void PokeathlonCourseLink_SetCheckSize(void *system, u16 a1);
void PokeathlonCourse_InitObjTransfer(ObjCharTransferTemplate *template, int a1, enum HeapID heapId, GXOBJVRamModeChar a3, GXOBJVRamModeChar a4);
void PokeathlonCourse_FreeObjTransfer(void);
void PokeathlonCourseScoreList_Rank(void *gfx, int a1);
void PokeathlonCourseScoreList_Init(void *gfx);
void *PokeathlonCourseScoreList_AddPointsToTotals(void *gfx);
void PokeathlonCourseScoreList_SaveEventPoints(void *gfx, int a1);
void PokeathlonCourse_AddScoreEntry2(PokeathlonCourseData *course, void *gfx, u32 participant, u32 value, u32 seconds, u32 tenths, u8 a6, u8 a7, u8 a8, u8 a9, u32 msgId);
void *PokeathlonCourseSprites_CreateResources(int heapId, int a1, int a2);
void PokeathlonCourseSprites_FreeResources(void *a0);
Sprite *ov96_021EA374(void *a0, SpriteList *spriteList, int a2, enum HeapID heapId);
void *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int a1, int a2, void *a3, void *a4);
void PokeathlonCourseMonObjects_Free(void *manager);
void PokeathlonCourseMonObjects_Load(void *manager, int count, PokeathlonHurdleDashMonView *views, u32 *info, int a4, int a5);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *manager);
void *PokeathlonCourseMonObjects_Get(u8 *manager, int index);
void PokeathlonCourseMonObject_SetDrawFlag(u8 *sprite, int a1);
void *ov96_021EB180(enum HeapID heapId, const PokeathlonHurdleDashSceneConfig *config);
void ov96_021EB21C(void *scene);
void ov96_021EB29C(void *scene, int a1, int a2);
void ov96_021EB2BC(void *scene, NarcId narcId, int a2, int a3, int a4);
void ov96_021EB2F4(void *scene, NarcId narcId, int a2, int a3, int a4, u8 a5);
void ov96_021EB334(void *scene, NarcId narcId, int a2, int a3);
void ov96_021EB36C(void *scene, NarcId narcId, int a2, int a3);
void ov96_021EB3A4(void *scene);
void *ov96_021EB3E4(void *scene, int a1, int a2, int a3, u8 a4);
Sprite *ov96_021EB408(void *scene, int a1, int a2, int a3, u8 a4);
void ov96_021EB52C(void *sprite, BOOL a1, BOOL a2);
void ov96_021EB564(void *sprite, int anim);
void ov96_021EB588(void *sprite, const void *pos);
void ov96_021EB5BC(u8 *scene);
void ov96_021EB5C8(void *scene, int a1, int a2, int a3, int a4);
SpriteList *ov96_021EB5E8(u8 *scene);
void ov96_021EB630(void *sprite, int a1);
void ov96_021FA0E8(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table, const PokeathlonHurdleDashParams *params, void *a3, void *scene, void *monSpriteManager, u8 member, PokeathlonHurdleDashRunner *runner);
BOOL ov96_021FA6D0(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash);
void ov96_021FAF1C(PokeathlonCourseData *course);
u32 ov96_021FB60C(PokeathlonHurdleDash *hurdleDash);
void ov96_021FB630(PokeathlonCourseData *course);
void ov96_021FB7C8(PokeathlonHurdleDash *hurdleDash);
void ov96_021FB808(PokeathlonHurdleDash *hurdleDash, s32 total);
void ov96_021FBBB4(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table);
void ov96_021FBDBC(PokeathlonHurdleDash *hurdleDash);
s32 ov96_021FBDEC(s32 frames);
void *ov96_021FC028(enum HeapID heapId);
void ov96_021FC05C(void *a0);
void ov96_021FC07C(u8 **a0, void *scene);
void *ov96_021FC0E4(void *a0);
void *ov96_021FC0E8(void *a0);
int ov96_021FC0F4(void *a0);
void ov96_021FC144(void *a0);
void *ov96_021FC188(enum HeapID heapId);
void ov96_021FC1A8(void *a0);
void ov96_021FC214(void *a0, int species);
void ov96_021FC2B4(void *a0, BOOL a1);
void ov96_021FC314(void *a0);
void *ov96_021FC618(enum HeapID heapId);
void ov96_021FC630(void *a0, void *scene, int a2);
void ov96_021FC690(void *a0);
void ov96_0221A56C(void *gfx, int a1);

extern const PokeathlonHurdleDashSceneConfig ov96_0221C404;
extern const PokeathlonHurdleDashNumberX ov96_0221C410;
extern const u8 ov96_0221C3F4[8];
extern const u8 ov96_0221C3EC[8];
extern const GraphicsModes ov96_0221C434;
extern const BgTemplate ov96_0221C454;
extern const BgTemplate ov96_0221C470;
extern const BgTemplate ov96_0221C48C;
extern const BgTemplate ov96_0221C4A8;
extern const BgTemplate ov96_0221C4C4;
extern const BgTemplate ov96_0221C4E0;
extern const BgTemplate ov96_0221C4FC;
extern const BgTemplate ov96_0221C518;
extern const GraphicsBanks ov96_0221C534;

BOOL PokeathlonHurdleDash_Start(PokeathlonCourseData *course) {
    PokeathlonCourse_SetStateField07(course, 1);
    return FALSE;
}

BOOL PokeathlonHurdleDash_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            hurdleDash->started = TRUE;
            (*state)++;
        }
        break;
    case 1: {
        BOOL finished;
        u8 i;
        u32 total;
        ov96_021FBDBC(hurdleDash);
        finished = ov96_021FA6D0(course, hurdleDash);
        ov96_021FB630(course);
        if (finished) {
            total = 0;
            for (i = 0; i < 3; i++) {
                total += hurdleDash->runners[i].time;
            }
            PokeathlonCourse_SetRecordScore(course, (u16)total);
            ov96_021FB808(hurdleDash, total);
            (*state)++;
        }
        break;
    }
    case 2: {
        u8 ready = PokeathlonCourse_RunFinish(course);
        if (ov96_021FB60C(hurdleDash) && ready) {
            (*state)++;
        }
        ov96_021FBDBC(hurdleDash);
        ov96_021FA6D0(course, hurdleDash);
        ov96_021FB630(course);
        break;
    }
    case 3: {
        u8 *block;
        ov96_021FBDBC(hurdleDash);
        ov96_021FB630(course);
        block = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course));
        block[9] = TRUE;
        block = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
        if (*(u32 *)block != 0) {
            (*state)++;
            hurdleDash->endTimer = 0;
        }
        break;
    }
    case 4:
        if (++hurdleDash->endTimer > 90) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, hurdleDash->heapId);
            PokeathlonCourse_SetStateField07(course, 2);
        }
        break;
    }
    ov96_021FAF1C(course);
    return FALSE;
}

BOOL PokeathlonHurdleDash_CalcScores(PokeathlonCourseData *course, u8 *state) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    float frames;
    int participant;
    int member;
    int count;
    int sum;
    int self;
    int i;
    int total;

    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    self = ov96_021E5F24(course);
    total = 0;
    for (i = 0; i < 3; i++) {
        total += hurdleDash->runners[i].time;
    }
    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)self, (u16)ov96_021FBDEC(total));
    if (self == 0) {
        // the host simulates the races of the participants without a player
        count = PokeathlonCourse_GetParticipantCount(course);
        for (participant = count; participant < 4; participant++) {
            sum = 0;
            for (member = 0; member < 3; member++) {
                float speed;
                int framesLeft;
                int index = (participant - count) * 3 + member;
                if (hurdleDash->cpuRunners[index].finished == FALSE) {
                    speed = hurdleDash->cpuRunners[index].speed;
                    if (speed < hurdleDash->cpuRunners[index].minSpeed) {
                        speed = hurdleDash->cpuRunners[index].minSpeed;
                    }
                    frames = (float)(0x1000 - hurdleDash->cpuRunners[index].distance) / speed;
                    framesLeft = (int)frames;
                    if (frames - (float)framesLeft > 0.0f) {
                        framesLeft++;
                    }
                    hurdleDash->cpuRunners[index].time = hurdleDash->frame + framesLeft;
                }
                sum += hurdleDash->cpuRunners[index].time;
            }
            PokeathlonCourse_SetField5E0_AtIndex(course, (u8)participant, (u16)ov96_021FBDEC(sum));
        }
    }
    return TRUE;
}

#pragma force_active on

BOOL PokeathlonHurdleDash_Init(PokeathlonCourseData *course) {
    u32 spriteInfo[17];
    PokeathlonHurdleDashTable table;
    ObjCharTransferTemplate charTransfer;
    PokeathlonHurdleDashSceneConfig sceneConfig;
    VecFx32 mainPos;
    VecFx32 spritePos;
    u16 mons[12][2];
    PokeathlonHurdleDashMonView views[3];
    PokeathlonHurdleDashNumberX numberX;
    u16 numberPos[3][2];
    PokeathlonHurdleDashParams params;
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i, j;
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        void *a, *b;
        int c;
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_138, 0x48000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        PokeathlonHurdleDash_SetGfxBanks();
        hurdleDash = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonHurdleDash));
        MI_CpuFill8(hurdleDash, 0, sizeof(PokeathlonHurdleDash));
        hurdleDash->bgConfig = BgConfig_Alloc(HEAP_ID_138);
        hurdleDash->unkD8 = ov96_021FC028(hurdleDash->heapId);
        hurdleDash->unkDC = ov96_021FC618(hurdleDash->heapId);
        a = ov96_021FC0E4(hurdleDash->unkD8);
        b = ov96_021FC0E8(hurdleDash->unkD8);
        c = ov96_021FC0F4(hurdleDash->unkD8);
        PokeathlonCourse_SetField3A4(course, (u32)a, (u32)b, (u32)c);
        PokeathlonCourse_SetHudPlanes(course, 8);
        charTransfer.maxTasks = 107;
        charTransfer.sizeMain = 0x40000;
        charTransfer.sizeSub = 0x4000;
        charTransfer.heapID = HEAP_ID_138;
        PokeathlonCourse_InitObjTransfer(&charTransfer, 18, HEAP_ID_138, GX_OBJVRAMMODE_CHAR_1D_256K, GX_OBJVRAMMODE_CHAR_1D_256K);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_138);
        hurdleDash->heapId = HEAP_ID_138;
        FontID_Alloc(4, HEAP_ID_138);
        PokeathlonHurdleDash_InitBgs(hurdleDash->bgConfig);
        ov96_021FB7C8(hurdleDash);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1:
        sceneConfig = ov96_0221C404;
        hurdleDash->scene = ov96_021EB180(hurdleDash->heapId, &sceneConfig);
        ov96_021EB5C8(hurdleDash->scene, 0, FX32_CONST(272), 0, 0);
        ov96_021EB29C(hurdleDash->scene, 0, 101);
        ov96_021EB29C(hurdleDash->scene, 1, 102);
        ov96_021EB29C(hurdleDash->scene, 2, 103);
        ov96_021EB29C(hurdleDash->scene, 3, 104);
        ov96_021EB29C(hurdleDash->scene, 4, 105);
        ov96_021EB29C(hurdleDash->scene, 5, 106);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 9, 101, 3);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 6, 101, 3, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 8, 101);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 7, 101);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 16, 102, 1);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 13, 102, 1, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 15, 102);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 14, 102);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 12, 103, 1);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 6, 103, 1, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 11, 103);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 10, 103);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 20, 105, 1);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 17, 105, 1, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 19, 105);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 18, 105);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 28, 104, 1);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 25, 104, 1, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 27, 104);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 26, 104);
        ov96_021EB2BC(hurdleDash->scene, NARC_a_1_5_6, 32, 106, 1);
        ov96_021EB2F4(hurdleDash->scene, NARC_a_1_5_6, 29, 106, 1, 1);
        ov96_021EB334(hurdleDash->scene, NARC_a_1_5_6, 31, 106);
        ov96_021EB36C(hurdleDash->scene, NARC_a_1_5_6, 30, 106);
        ov96_021EB3A4(hurdleDash->scene);
        hurdleDash->unk3A0 = PokeathlonCourseSprites_CreateResources((int)hurdleDash->heapId, 743, 1);
        hurdleDash->monSpriteManager = PokeathlonCourseMonObjects_Create(hurdleDash->heapId, 3, 4, hurdleDash->unk3A0, ov96_021EB5E8(hurdleDash->scene));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        for (i = 0; i < 12; i++) {
            Sprite_SetDrawPriority(ov96_021EB408(hurdleDash->scene, 1, 3, 101, 2), 27);
            Sprite_SetDrawPriority(ov96_021EB408(hurdleDash->scene, 1, 1, 103, 6), 27);
        }
        for (i = 0; i < 3; i++) {
            ov96_021EB408(hurdleDash->scene, 1, 1, 102, 5);
            ov96_021EB408(hurdleDash->scene, 1, 1, 104, 7);
            ov96_021EB408(hurdleDash->scene, 1, 1, 105, 8);
        }
        hurdleDash->unk3A8 = ov96_021EB3E4(hurdleDash->scene, 1, 1, 106, 9);
        ov96_021EB564(hurdleDash->unk3A8, 0);
        mainPos.z = 0;
        mainPos.x = FX32_CONST(160);
        mainPos.y = FX32_CONST(456);
        ov96_021EB588(hurdleDash->unk3A8, &mainPos);
        ov96_021EB52C(hurdleDash->unk3A8, TRUE, TRUE);
        ov96_021EB630(hurdleDash->unk3A8, 1);
        for (i = 0; i < 6; i++) {
            hurdleDash->sprites[i] = ov96_021EA374(hurdleDash->unk3A0, ov96_021EB5E8(hurdleDash->scene), 0, hurdleDash->heapId);
            Sprite_SetDrawFlag(hurdleDash->sprites[i], TRUE);
            spritePos.x = ov96_0221C3F4[i] << FX32_SHIFT;
            spritePos.y = FX32_CONST(456);
            spritePos.z = 0;
            Sprite_SetMatrix(hurdleDash->sprites[i], &spritePos);
            Sprite_SetAnimCtrlSeq(hurdleDash->sprites[i], ov96_0221C3EC[i]);
        }
        ov96_021FC630(hurdleDash->unkDC, hurdleDash->scene, 1);
        hurdleDash->unk228 = ov96_021FC188(hurdleDash->heapId);
        {
            u16(*out)[2];
            i = 0;
            out = mons;
            for (; i < 4; i++) {
                PokeathlonHurdleDashTeamMon *mon = (PokeathlonHurdleDashTeamMon *)PokeathlonCourse_GetParticipantUnk04(course, i);
                for (j = 0; j < 3; j++) {
                    (*out)[0] = mon->species;
                    (*out)[1] = mon->form;
                    mon++;
                    out++;
                }
            }
        }
        ov96_021FC214(hurdleDash->unk228, (int)mons);
        ov96_021FC2B4(hurdleDash->unk228, FALSE);
        for (i = 0; i < 3; i++) {
            PokeathlonCourse_GetMonInfo(course, ov96_021E5F24(course), i, &views[i]);
            spriteInfo[i + 5] = (u32)ov96_021E6108(PokeathlonCourse_GetPerformance(course, ov96_021E5F24(course), i));
        }
        spriteInfo[0] = 0;
        spriteInfo[1] = 1;
        spriteInfo[2] = 0;
        spriteInfo[3] = 1;
        spriteInfo[4] = 1;
        PokeathlonCourseMonObjects_Load(hurdleDash->monSpriteManager, 3, views, spriteInfo, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        if (PokeathlonCourseMonObjects_IsLoaded(hurdleDash->monSpriteManager)) {
            PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)hurdleDash->bgConfig); // takes the BgConfig
            PokeathlonHurdleDash_LoadBgTiles(hurdleDash->bgConfig, hurdleDash->heapId);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 4:
        PokeathlonHurdleDash_LoadBgScreens(hurdleDash->bgConfig, hurdleDash->heapId);
        {
            reg_G2_BG0OFS = 0x1400000;
            reg_G2_BG1OFS = 0x1400000;
            reg_G2_BG2OFS = 0x1400000;
            reg_G2S_DB_BG0OFS = 0x300000;
            reg_G2S_DB_BG1OFS = 0x300000;
            reg_G2S_DB_BG2OFS = 0x300000;
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        PokeathlonCourse_SetField1F4(course, 1);
        for (i = 0; i < 3; i++) {
            PokeathlonCourseMonObject_SetDrawFlag(PokeathlonCourseMonObjects_Get(hurdleDash->monSpriteManager, (u8)i), 1);
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        if (ov96_021E5F24(course) == 0) {
            ov96_021FC144(hurdleDash->unkD8);
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 7: {
        u8 *dataCopy;
        ov96_021FC07C(hurdleDash->unkD8, hurdleDash->scene);
        Sprite_SetDrawPriority(*ov96_021E6290(course, 272, hurdleDash->unk3A0, hurdleDash->scene), 1);
        numberX = ov96_0221C410;
        ReadWholeNarcMemberByIdPair(&params, NARC_a_1_7_0, 10);
        ReadWholeNarcMemberByIdPair(&table, NARC_a_1_7_0, 0);
        {
            int k;
            for (k = 0; k < 3; k++) {
                ov96_021FA0E8(course, &table, &params, hurdleDash->unkD8, hurdleDash->scene, hurdleDash->monSpriteManager, (u8)k, &hurdleDash->runners[k]);
                numberPos[k][0] = numberX.x[k];
                numberPos[k][1] = 392;
            }
        }
        ov96_021E634C(course, 0, hurdleDash->unk3A0, hurdleDash->scene, 1, 3, numberPos);
        if (ov96_021E5F24(course) == 0) {
            ov96_021FBBB4(course, &table);
        }
        dataCopy = PokeathlonCourse_GetDataCopyArea(course);
        {
            u8 *block = ov96_021E8A20(dataCopy + 240);
            u8 k;
            for (k = 0; k < 6; k++) {
                block[28 + k] = 17;
            }
        }
        if (ov96_021E5F24(course) == 0) {
            u8 *block = ov96_021E8A20(dataCopy + 40);
            u8 k;
            for (k = 0; k < 6; k++) {
                block[28 + k] = 17;
            }
        }
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0, 6, 1, hurdleDash->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 8:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonHurdleDash_Draw(PokeathlonCourseData *course) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    ov96_021FC314(hurdleDash->unk228);
    ov96_021EB5BC(hurdleDash->scene);
    return TRUE;
}

// Shows every team's total time ("Total Time: X.Y Seconds").
BOOL PokeathlonHurdleDash_ShowResults(PokeathlonCourseData *course) {
    void *gfx;
    PokeathlonHurdleDash *hurdleDash;
    int i;
    int frames;
    int seconds;

    hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    gfx = PokeathlonCourse_GetGraphicsSystem(course);
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < 4; i++) {
        frames = PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i);
        seconds = frames / 30;
        PokeathlonCourse_AddScoreEntry2(course, gfx, i, (u16)frames, seconds, (frames % 30) * 10 / 30, 0, 1, 3, 1, 291);
    }
    PokeathlonCourseScoreList_Rank(gfx, 2);
    ov96_0221A56C(gfx, 0);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 0);
    return TRUE;
}

BOOL PokeathlonHurdleDash_Exit(PokeathlonCourseData *course) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);

    WirelessStatusIcon_Destroy();
    PokeathlonCourseLink_SetCheckSize(PokeathlonCourse_GetSystem(course), 0);
    ov96_021FC1A8(hurdleDash->unk228);
    PokeathlonCourse_ResetField3A4(course);
    ov96_021FC690(hurdleDash->unkDC);
    ov96_021FC05C(hurdleDash->unkD8);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(hurdleDash->bgConfig, GF_BG_LYR_SUB_3);
    RemoveWindow(&hurdleDash->window);
    Heap_Free(hurdleDash->bgConfig);
    PokeathlonCourseMonObjects_Free(hurdleDash->monSpriteManager);
    PokeathlonCourseSprites_FreeResources(hurdleDash->unk3A0);
    ov96_021EB21C(hurdleDash->scene);
    PokeathlonCourse_FreeObjTransfer();
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_138);
    return TRUE;
}

void PokeathlonHurdleDash_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221C534;
    GfGfx_SetBanks(&banks);
}

void PokeathlonHurdleDash_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = ov96_0221C434;
    SetBothScreensModesAndDisable(&modes);
    {
        BgTemplate bg = ov96_0221C48C;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate bg = ov96_0221C4A8;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate bg = ov96_0221C4C4;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    }
    {
        BgTemplate bg = ov96_0221C4E0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
    {
        BgTemplate bg = ov96_0221C454;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate bg = ov96_0221C470;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }
    {
        BgTemplate bg = ov96_0221C518;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    }
    {
        BgTemplate bg = ov96_0221C4FC;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonHurdleDash_LoadBgTiles(BgConfig *bgConfig, enum HeapID heapId) {
    GfGfxLoader_LoadCharData(NARC_a_1_5_6, 1, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_5_6, 1, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapId);
}

void PokeathlonHurdleDash_LoadBgScreens(BgConfig *bgConfig, enum HeapID heapId) {
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 2, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 4, bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 2, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 3, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 4, bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_6, 5, bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_6, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_5_6, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, heapId);
}
