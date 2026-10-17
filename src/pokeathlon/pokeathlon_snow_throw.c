#include "pokeathlon/pokeathlon_snow_throw.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_event_points.h"

#include "assert.h"
#include "bg_window.h"
#include "char_tiles.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

enum {
    SNOW_THROW_STATE_PLAY,
    SNOW_THROW_STATE_SAVE_SCORES,
};

typedef char PokeathlonSnowThrowSizeCheck[(sizeof(PokeathlonSnowThrow) == 0x5F4 && sizeof(PokeathlonSnowThrowBall) == 0x48 && sizeof(PokeathlonSnowThrowMon) == 0x20 && sizeof(PokeathlonSnowThrowPacket) == 0x28 && sizeof(PokeathlonSnowThrowLink) == 0x118 && offsetof(PokeathlonSnowThrow, balls) == 0xB8 && offsetof(PokeathlonSnowThrow, mons) == 0x418 && offsetof(PokeathlonSnowThrow, cpu) == 0x5DC && offsetof(PokeathlonSnowThrow, timer) == 0x5E8) ? 1 : -1];

void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *result);

void PokeathlonCourseSpritePool_Draw(PokeathlonCourseSpritePool *pool);
void PokeathlonCourseSpritePool_Free(PokeathlonCourseSpritePool *pool);
PokeathlonCourseSpritePool *PokeathlonCourseSpritePool_New(enum HeapID heapId, VecFx32 *template);
void PokeathlonCourseSpritePool_SetSurfaceCoords(PokeathlonCourseSpritePool *pool, fx32 mainX, fx32 mainY, fx32 subX, fx32 subY);
void PokeathlonCourseSpritePool_SetResourceId(PokeathlonCourseSpritePool *pool, int index, u32 id);
void PokeathlonCourseSpritePool_TransferResources(PokeathlonCourseSpritePool *pool);
void PokeathlonCourseSpritePool_PauseAnims(PokeathlonCourseSpritePool *pool, BOOL paused);
void PokeathlonCourseSpritePool_LoadChar(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId, int vram);
void PokeathlonCourseSpritePool_LoadPltt(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId, int vram, int count);
void PokeathlonCourseSpritePool_LoadCell(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePool_LoadAnim(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_CreateAndTake(PokeathlonCourseSpritePool *pool, int priority, int whichScreen, int resId, int kind);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetScale(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *scale, int affineType);
void PokeathlonCourseObjects_PauseAnims(PokeathlonCourseObjects *objects, BOOL paused);
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseObject *object, s32 x, s32 y);

extern const VecFx32 sSnowThrowSpritePoolTemplate; // sprite pool template

// The animation state (facing) of each other team's Pokemon, by its place in the view.
typedef struct PokeathlonSnowThrowViewAnims {
    u32 animStates[SNOW_THROW_TEAMS - 1];
} PokeathlonSnowThrowViewAnims;

// Where the marks of the other teams' colours are on the field screen.
typedef struct PokeathlonSnowThrowTeamMarkPositions {
    VecFx32 positions[SNOW_THROW_TEAMS - 1];
} PokeathlonSnowThrowTeamMarkPositions;

extern const PokeathlonSnowThrowViewAnims sSnowThrowViewAnims;
extern const VecFx32 sSnowThrowPacketScale;
extern const s32 sSnowThrowOtherMonX[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const s32 sSnowThrowOtherMonY[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const u8 sSnowThrowOtherMonDrawPriorities[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
extern const GraphicsBanks sSnowThrowGraphicsBanks;
extern const GraphicsModes sSnowThrowGraphicsModes;
extern const BgTemplate sSnowThrowBgTemplate_Main0;
extern const BgTemplate sSnowThrowBgTemplate_Main1;
extern const BgTemplate sSnowThrowBgTemplate_Main3;
extern const BgTemplate sSnowThrowBgTemplate_Sub0;
extern const BgTemplate sSnowThrowBgTemplate_Sub1;
extern const BgTemplate sSnowThrowBgTemplate_Sub2;
extern const BgTemplate sSnowThrowBgTemplate_Sub3;
extern const PokeathlonSnowThrowTeamMarkPositions sSnowThrowTeamMarkPositions;
// Where each Pokemon stands on the field (fx32 field units, unturned).
extern const VecFx32 sSnowThrowMonPositions[SNOW_THROW_MONS];
extern const WindowTemplate sSnowThrowScoreWindowTemplate;
extern const WindowTemplate sSnowThrowNameWindowTemplates[SNOW_THROW_TEAMS - 1];

// NitroSDK MATH_QSort (no header declares it)
void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *), void *context);

void PokeathlonCourseSpritePool_CreateSprite(PokeathlonCourseSpritePool *pool, int priority, int whichScreen, int resId, int kind);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_Take(PokeathlonCourseSpritePool *pool, int resId, int kind);
void **PokeathlonCourseSpritePool_GetResourceSlot(PokeathlonCourseSpritePool *pool, u32 id, u32 kind);
void PokeathlonCourseSpritePoolSprite_TryChangeAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(PokeathlonCourseSpritePoolSprite *entry);

// Which of two Pokemon at the same depth is drawn in front, by view (own team) and Pokemon.
extern const u8 sSnowThrowDrawOrder[][SNOW_THROW_MONS];
extern const WindowTemplate sSnowThrowDisplayNameWindowTemplate;

// A snowball as sorted for drawing (8).
typedef struct PokeathlonSnowThrowDrawOrder {
    u16 own;
    u16 index;
    s32 y;
} PokeathlonSnowThrowDrawOrder;

// Where each Pokemon stands on the field (fx32 field units; the CPU's copy of the table).
extern const VecFx32 sSnowThrowCpuMonPositions[SNOW_THROW_MONS];

static void PokeathlonSnowThrow_SetGfxBanks(void);
static void PokeathlonSnowThrow_InitBgs(BgConfig *bgConfig);
static void PokeathlonSnowThrow_LoadBgGraphics(PokeathlonSnowThrow *work, u8 own);
static void PokeathlonSnowThrow_LoadSpriteGraphics(PokeathlonCourseSpritePool *pool);
static void PokeathlonSnowThrow_CreateSprites(PokeathlonSnowThrow *work, PokeathlonCourseSpritePool *pool, u8 own);
static void PokeathlonSnowThrow_UpdateHost(PokeathlonCourseData *course);
static void PokeathlonSnowThrow_PackPacket(PokeathlonSnowThrow *work, PokeathlonSnowThrowPacket *packet);
static BOOL PokeathlonSnowThrow_Update(PokeathlonCourseData *course);
static void PokeathlonSnowThrow_DrawPacket(PokeathlonCourseData *course);
static u8 PokeathlonSnowThrow_FindOwnMonAt(PokeathlonSnowThrow *work, u8 team, u32 x, u32 y, VecFx32 *out);
static void PokeathlonSnowThrow_InitMon(PokeathlonCourseData *course, s32 stats[][5], u8 team, u8 member, PokeathlonSnowThrowMon *mon);
static void PokeathlonSnowThrow_InitWindows(PokeathlonSnowThrow *work);
static void PokeathlonSnowThrow_UpdateMons(PokeathlonCourseData *course);
static void PokeathlonSnowThrow_MoveBalls(PokeathlonCourseData *course);
static void PokeathlonSnowThrow_HitMons(PokeathlonCourseData *course, PokeathlonSnowThrow *work);
static void PokeathlonSnowThrow_CollideBalls(PokeathlonSnowThrow *work);
static void PokeathlonSnowThrow_PrintNames(PokeathlonSnowThrow *work, PokeathlonCourseData *course);
static void PokeathlonSnowThrow_SetTimerDigits(PokeathlonSnowThrow *work, u32 seconds);
static void PokeathlonSnowThrow_ShowOwnScore(PokeathlonCourseData *course);
static void PokeathlonSnowThrow_SortSprites(PokeathlonSnowThrow *work, u16 own);
static PokeathlonSnowThrowDisplay *PokeathlonSnowThrowDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
static void PokeathlonSnowThrowDisplay_Delete(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_LoadBgGraphics(PokeathlonSnowThrowDisplay *display, u32 own);
static void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool);
static void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseMonInfo *mons);
static void PokeathlonSnowThrowDisplay_ShowTired(PokeathlonSnowThrowDisplay *display, u8 member, BOOL show);
static void PokeathlonSnowThrowDisplay_SetScore(PokeathlonSnowThrowDisplay *display, u16 score);
static void PokeathlonSnowThrowDisplay_SetPicAnim(PokeathlonSnowThrowDisplay *display, u8 member, int anim);
static BOOL PokeathlonSnowThrowDisplay_IsPicAnimated(PokeathlonSnowThrowDisplay *display, u8 member);
static void PokeathlonSnowThrowDisplay_Shake(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_UpdateShake(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseSpriteResources *monResources);
static void PokeathlonSnowThrowDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons);
static void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_GetPicVramOffsets(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ);
static void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY);
static void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction);
static PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode);
static void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball);
static void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu);
static int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed);
static void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed);

// The Pokemon the k-th of the other teams' nine is (they are listed from the team after the own one on).
static inline u8 PokeathlonSnowThrow_OtherMon(int own, int k) {
    return ((u8)(((own + 1) * SNOW_THROW_MEMBERS) % SNOW_THROW_MONS) + k) % SNOW_THROW_MONS;
}

// Set-up, a step per frame (PokeathlonCourse_GetEventInitState): heaps and screens, the top screen and the CPU,
// sprites, the Pokemon graphics, then the field once they are loaded.

BOOL PokeathlonSnowThrow_Init(PokeathlonCourseData *course) {
    PokeathlonCourseMonInfo infos[SNOW_THROW_MONS];
    PokeathlonCourseMonGfxTemplate template;
    s32 stats[4][5];
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrow *data;
    int participant;
    int i;
    int k;
    void *object;
    u8 mon;
    int x;
    int team;
    PokeathlonSnowThrowMon *teamMon;
    int first;
    int count;
    int own;
    int member;
    int y, j;

    work = PokeathlonCourse_GetWork(course);
    switch (PokeathlonCourse_GetEventInitState(course)) {
    case 0: {
        ObjCharTransferTemplate charTransfer;

        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_146, 0x40000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonSnowThrow_SetGfxBanks();
        data = PokeathlonCourse_AllocWork(course, sizeof(PokeathlonSnowThrow));
        MI_CpuFill8(data, 0, sizeof(PokeathlonSnowThrow));
        data->sortBuffer = Heap_Alloc(HEAP_ID_146, 40);
        MI_CpuFill8(data->sortBuffer, 0, 40);
        data->bgConfig = BgConfig_Alloc(HEAP_ID_146);
        PokeathlonCourse_SetHudPlanes(course, 8);
        charTransfer.maxTasks = 115;
        charTransfer.sizeMain = 0x40000;
        charTransfer.sizeSub = 0x4000;
        charTransfer.heapID = PokeathlonCourse_GetHeapID(course);
        PokeathlonCourse_InitObjTransfer(&charTransfer, 22, HEAP_ID_146, GX_OBJVRAMMODE_CHAR_1D_256K, GX_OBJVRAMMODE_CHAR_1D_256K);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_146);
        data->heapId = HEAP_ID_146;
        FontID_Alloc(4, HEAP_ID_146);
        PokeathlonSnowThrow_InitBgs(data->bgConfig);
        PokeathlonSnowThrow_InitWindows(data);
        PokeathlonSnowThrow_PrintNames(data, course);
        gSystem.screensFlipped = FALSE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    }
    case 1:
        work->display = PokeathlonSnowThrowDisplay_New(work->heapId, work->bgConfig, course);
        count = PokeathlonCourse_GetParticipantCount(course);
        work->cpu = PokeathlonSnowThrowCpu_New(work->heapId, SNOW_THROW_TEAMS - count, PokeathlonCourse_GetMode(course));
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case 2: {
        VecFx32 poolTemplate = sSnowThrowSpritePoolTemplate;

        work->spritePool = PokeathlonCourseSpritePool_New(work->heapId, &poolTemplate);
        PokeathlonCourseSpritePool_SetSurfaceCoords(work->spritePool, 0, 0, 0, FX32_CONST(288));
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 0, 106);
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 1, 101);
        PokeathlonCourseSpritePool_SetResourceId(work->spritePool, 2, 105);
        PokeathlonSnowThrow_LoadSpriteGraphics(work->spritePool);
        PokeathlonSnowThrowDisplay_LoadSpriteGraphics(work->display, work->spritePool);
        PokeathlonCourseSpritePool_TransferResources(work->spritePool);
        work->monResources = PokeathlonCourseSprites_CreateResources(work->heapId, 1255, 1);
        work->monObjectList = PokeathlonCourseMonObjects_Create(work->heapId, 9, 32, work->monResources, PokeathlonCourseSpritePool_GetSpriteList(work->spritePool));
        Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSpritesInPool(course, 0, work->monResources, work->spritePool), 1);
        own = PokeathlonCourse_GetNetId(course);
        PokeathlonSnowThrow_CreateSprites(work, work->spritePool, own);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    }
    case 3:
        // the other teams' Pokemon first, the own three last
        own = (u8)PokeathlonCourse_GetNetId(course);
        for (i = 0; i < SNOW_THROW_MONS; i++) {
            count = PokeathlonSnowThrow_OtherMon(own, i);
            member = count % SNOW_THROW_MEMBERS;
            participant = count / SNOW_THROW_MEMBERS;
            PokeathlonCourse_GetMonInfo(course, participant, member, &infos[i]);
            template.sizeTypes[i] = PokeathlonCourse_GetSpriteSizeType((PokeathlonPerformanceSizeInfo *)PokeathlonCourse_GetPerformance(course, participant, member));
        }
        PokeathlonSnowThrowDisplay_CreateSprites(work->display, work->spritePool, &infos[9]);
        for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
            VecFx32 position = { 0, 0, 0 };
            PokeathlonCourseMonInfo *info = &infos[9 + i];
            u8 height = GetMonPicHeightBySpeciesGenderForm(info->species, info->gender, 0, info->form, info->personality);
            position.x = (16 + i * 50) << FX32_SHIFT;
            position.y = (height + 376) << FX32_SHIFT;
            position.y -= FX32_ONE * 16;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[own * SNOW_THROW_MEMBERS + i].stunSprite, &position);
        }
        template.loadAtOnce = FALSE;
        template.priority = 3;
        template.vramTransfer = FALSE;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(work->monObjectList, 9, infos, &template, 0, 0);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case 4:
        if (!PokeathlonCourseMonObjects_IsLoaded(work->monObjectList)) {
            break;
        } else {
            PokeathlonSnowThrowViewAnims anims = sSnowThrowViewAnims;

            PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)work->bgConfig); // takes the BgConfig
            PokeathlonCourse_SetEventDrawEnabled(course, 1);
            own = PokeathlonCourse_GetNetId(course);
            for (k = 0; k < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; k++) {
                work->monObjects[k] = PokeathlonCourseMonObjects_Get(work->monObjectList, (u8)k);
                object = work->monObjects[k];
                PokeathlonCourseMonObject_SetDrawFlag(object, TRUE);
                mon = ((u8)(((own + 1) * SNOW_THROW_MEMBERS) % SNOW_THROW_MONS) + k) % SNOW_THROW_MONS; // PokeathlonSnowThrow_OtherMon
                x = sSnowThrowOtherMonX[k];
                y = sSnowThrowOtherMonY[k];
                PokeathlonCourseMonObject_SetAnimState(object, anims.animStates[k / SNOW_THROW_MEMBERS]);
                PokeathlonCourseObject_SetGroundPosition(object, x, y);
                {
                    VecFx32 markPosition = { 0, 0, 0 };
                    if (PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(object))->isLarge) {
                        markPosition.x = x << FX32_SHIFT;
                        markPosition.y = (y - 40) << FX32_SHIFT;
                    } else {
                        markPosition.x = x << FX32_SHIFT;
                        markPosition.y = (y - 25) << FX32_SHIFT;
                    }
                    PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[mon].stunSprite, &markPosition);
                    PokeathlonCourseSpritePoolSprite_SetPosition(work->tiredMarks[k], &markPosition);
                }
                PokeathlonCourseMonObject_SetDrawPriority(object, sSnowThrowOtherMonDrawPriorities[k] + 32);
            }
            ReadWholeNarcMemberByIdPair(stats, NARC_a_1_7_0, 3);
            work->timer = SNOW_THROW_DURATION;
            for (team = 0, first = 0; team < SNOW_THROW_TEAMS; first += SNOW_THROW_MEMBERS, team++) {
                work->heldBall[team] = SNOW_THROW_NONE;
                for (j = 0; j < SNOW_THROW_MEMBERS; j++) {
                    teamMon = &work->mons[j + first];
                    PokeathlonSnowThrow_InitMon(course, stats, team, j, teamMon);
                    if (PokeathlonCourse_GetNetId(course) == 0) {
                        PokeathlonSnowThrowCpu_SetMon(work->cpu, j + first, teamMon, &work->balls[j + first]);
                    }
                }
            }
            if (PokeathlonCourse_GetNetId(course) == 0) {
                PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packetToSend)));
            }
            PokeathlonSnowThrowDisplay_SetMonShadows(work->display, work->spritePool, work->monResources);
            PokeathlonSnowThrow_LoadBgGraphics(work, PokeathlonCourse_GetNetId(course));
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            WirelessStatusIcon_CreateOnScreenIfConnected(1);
            BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_AdvanceEventInitState(course);
        }
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            GF_ASSERT(GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL PokeathlonSnowThrow_Draw(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetWork(course);
    PokeathlonSnowThrowDisplay_UpdateShake(work->display);
    PokeathlonCourseSpritePool_Draw(work->spritePool);
    return TRUE;
}

// Results: the hits each team scored.
BOOL PokeathlonSnowThrow_ShowResults(PokeathlonCourseData *course) {
    PokeathlonCourseScoreList *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetReceivedScore(course, (u8)i), 0, 3, msg_0309_00299);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    PokeathlonEvent_CalcPoints(gfx, 8);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 8);
    return TRUE;
}

BOOL PokeathlonSnowThrow_Exit(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetWork(course);
    int i;
    Window *window;

    WirelessStatusIcon_Destroy();
    PokeathlonCourse_ClearEventDataTransfer(course);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_3);
    RemoveWindow(&work->scoreWindow);
    for (i = 0, window = work->nameWindows; i < SNOW_THROW_TEAMS - 1; i++, window++) {
        RemoveWindow(window);
    }
    Heap_Free(work->bgConfig);
    PokeathlonCourseSpritePool_Free(work->spritePool);
    PokeathlonCourseMonObjects_Free(work->monObjectList);
    PokeathlonCourseSprites_FreeResources(work->monResources);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    PokeathlonSnowThrowCpu_Delete(work->cpu);
    PokeathlonSnowThrowDisplay_Delete(work->display);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    Heap_Free(work->sortBuffer);
    PokeathlonCourse_FreeWork(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_146);
    return TRUE;
}

static void PokeathlonSnowThrow_SetGfxBanks(void) {
    GraphicsBanks banks = sSnowThrowGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void PokeathlonSnowThrow_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0, main1, main3, sub0, sub1, sub2, sub3;

    modes = sSnowThrowGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    main0 = sSnowThrowBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = sSnowThrowBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main3 = sSnowThrowBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = sSnowThrowBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = sSnowThrowBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = sSnowThrowBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    sub3 = sSnowThrowBgTemplate_Sub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

// The field (turned for the own team) and the top screen.
static void PokeathlonSnowThrow_LoadBgGraphics(PokeathlonSnowThrow *work, u8 own) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 1, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x4000, FALSE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 2, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, work->heapId);
    PokeathlonSnowThrowDisplay_LoadBgGraphics(work->display, own);
}

static void PokeathlonSnowThrow_LoadSpriteGraphics(PokeathlonCourseSpritePool *pool) {
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 21, 106, 3);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 18, 106, 3, 2);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 20, 106);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 19, 106);
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 17, 105, 1);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 14, 105, 1, 3);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 16, 105);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 15, 105);
}

#pragma push
#pragma opt_common_subs off

static void PokeathlonSnowThrow_CreateSprites(PokeathlonSnowThrow *work, PokeathlonCourseSpritePool *pool, u8 own) {
    int i;
    u8 anim;
    VecFx32 position;
    VecFx32 scale;
    VecFx32 framePosition;
    PokeathlonSnowThrowTeamMarkPositions teamMarkPositions;

    for (i = 0; i < 2; i++) {
        work->timerDigits[i] = PokeathlonCourse_CreateTimerDigitSprite(work->monResources, PokeathlonCourseSpritePool_GetSpriteList(pool), 3, work->heapId);
        Sprite_SetDrawFlag(work->timerDigits[i], TRUE);
        position.x = (i * 16) * FX32_ONE + 24 * FX32_ONE;
        position.y = 8 * FX32_ONE;
        position.z = 0;
        Sprite_SetMatrix(work->timerDigits[i], &position);
        Sprite_SetDrawPriority(work->timerDigits[i], 2);
    }
    PokeathlonSnowThrow_SetTimerDigits(work, SNOW_THROW_DURATION / 30);
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        work->balls[i].index = i;
        work->balls[i].sprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 3);
        work->balls[i].shadowSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 4);
        work->balls[i].splashSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 6);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->balls[i].shadowSprite, 1);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].sprite, 20);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].shadowSprite, 1000);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[i].splashSprite, 5);
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        work->mons[i].stunSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 3, 106, 9);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->mons[i].stunSprite, 2);
        scale.x = FX32_ONE;
        scale.y = FX32_ONE;
        scale.z = 0;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->mons[i].stunSprite, &scale);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->mons[i].stunSprite, 4);
    }
    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        work->hitMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 2, 1, 105, 11);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->hitMarks[i], 6);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->hitMarks[i], 2);
    }
    work->frameSprite = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 7);
    framePosition.x = 8 * FX32_ONE;
    framePosition.y = 8 * FX32_ONE;
    framePosition.z = 0;
    PokeathlonCourseSpritePoolSprite_SetPosition(work->frameSprite, &framePosition);
    PokeathlonCourseSpritePoolSprite_SetAnim(work->frameSprite, 5);
    PokeathlonCourseSpritePoolSprite_Show(work->frameSprite, TRUE, TRUE);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->frameSprite, 2);
    teamMarkPositions = sSnowThrowTeamMarkPositions;
    anim = (own + 1) % SNOW_THROW_TEAMS;
    for (i = 0; i < SNOW_THROW_TEAMS - 1; i++) {
        work->teamMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 8);
        PokeathlonCourseSpritePoolSprite_SetPosition(work->teamMarks[i], &teamMarkPositions.positions[i]);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->teamMarks[i], anim + 1);
        PokeathlonCourseSpritePoolSprite_Show(work->teamMarks[i], TRUE, TRUE);
        anim = (anim + 1) % SNOW_THROW_TEAMS;
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->teamMarks[i], 3);
    }
    for (i = 0; i < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; i++) {
        work->tiredMarks[i] = PokeathlonCourseSpritePool_CreateAndTake(pool, 3, 1, 105, 10);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->tiredMarks[i], 0);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->tiredMarks[i], 4);
    }
}

#pragma pop

BOOL PokeathlonSnowThrow_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetWork(course);
    switch (*state) {
    case 0:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonSnowThrow_UpdateHost(course);
        if (PokeathlonSnowThrow_Update(course)) {
            (*state)++;
        }
        break;
    case 2:
        PokeathlonSnowThrow_UpdateHost(course);
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_SetNextState(course, SNOW_THROW_STATE_SAVE_SCORES);
        }
        break;
    }
    return FALSE;
}

// The host stores everyone's score (the hits of the team) for the results.
BOOL PokeathlonSnowThrow_SaveScores(PokeathlonCourseData *course, u8 *state) {
    PokeathlonSnowThrowLink *link = (PokeathlonSnowThrowLink *)PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonSnowThrowPacket *packet;
    int i;
    PokeathlonCourse_GetWork(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (PokeathlonCourse_GetNetId(course)) {
        return TRUE;
    }
    packet = (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes(&link->packetToSend);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        PokeathlonCourse_SetEventScore(course, (u8)i, packet->teams[i] & 0x3FF);
    }
    return TRUE;
}

// Host: takes everyone's stylus (a touch on an own Pokemon starts a snowball, more taps on it make it bigger, a
// flick away from it throws it), steps the snowballs, the Pokemon and the CPU, and sends the packet.

static void PokeathlonSnowThrow_UpdateHost(PokeathlonCourseData *course) {
    VecFx32 touched;
    VecFx32 touchStart;
    VecFx32 released;
    int i;
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowTouchBlock *block;
    PokeathlonSnowThrowTouch *touch;
    int ball;
    f32 energyFactor;
    f32 speed;
    f32 hold;
    f32 strength;

    block = (PokeathlonSnowThrowTouchBlock *)PokeathlonCourse_GetDataCopyArea(course);
    work = PokeathlonCourse_GetWork(course);
    if (PokeathlonCourse_GetNetId(course) != 0) {
        return;
    }
    if (work->finished) {
        block++;
        PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes(block));
        return;
    }
    // the host's own stylus goes into its slot of everyone's
    *(PokeathlonSnowThrowTouch *)PokeathlonCourse_ToBytes(&block[2]) = *(PokeathlonSnowThrowTouch *)PokeathlonCourse_ToBytes(block);
    if (work->timer != 0) {
        work->timer--;
    }
    PokeathlonCourse_ToBytes(&block[1]);
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        touch = (PokeathlonSnowThrowTouch *)PokeathlonCourse_ToBytes(&(block + 2)[i]);
        if (touch->held) {
            if (work->presses[i].pressed && work->presses[i].held) {
                work->presses[i].pressed = FALSE;
            } else if (work->presses[i].pressed == FALSE && work->presses[i].held == FALSE) {
                work->presses[i].pressed = TRUE;
                work->presses[i].held = TRUE;
            }
        } else {
            work->presses[i].pressed = FALSE;
            work->presses[i].held = FALSE;
        }
        if (work->presses[i].pressed) {
            ball = PokeathlonSnowThrow_FindOwnMonAt(work, i, touch->x, touch->y, &touched);
            if (ball != SNOW_THROW_NONE) {
                if (work->mons[ball].stunTimer != 0) {
                    continue;
                }
                if (work->balls[ball].state == SNOW_THROW_BALL_NONE || work->balls[ball].state == SNOW_THROW_BALL_HELD) {
                    work->heldBall[i] = ball;
                    work->balls[ball].dragStart = touched;
                    touchStart.x = touch->x << FX32_SHIFT;
                    touchStart.z = touch->y << FX32_SHIFT;
                    touchStart.y = 0;
                    work->balls[ball].touchStart = touchStart;
                    if (work->balls[ball].state == SNOW_THROW_BALL_NONE) {
                        work->balls[ball].position = sSnowThrowMonPositions[ball];
                        work->balls[ball].state = SNOW_THROW_BALL_HELD;
                        work->balls[ball].taps = 0;
                        work->balls[ball].size = 0;
                    }
                }
            }
            work->holdFrames = 0;
        } else if (work->presses[i].held) {
            if (work->heldBall[i] != SNOW_THROW_NONE && work->holdFrames < 30.0) {
                work->holdFrames++;
            }
        } else {
            if (work->heldBall[i] != SNOW_THROW_NONE) {
                ball = PokeathlonSnowThrow_FindOwnMonAt(work, i, touch->x, touch->y, &released);
                if (ball != SNOW_THROW_NONE && ball == work->heldBall[i]) {
                    // released on the Pokemon: one more tap
                    if (work->balls[ball].taps < 4) {
                        work->balls[ball].taps++;
                        if (work->balls[ball].taps <= 1) {
                            work->balls[ball].size = 0;
                        } else if (work->balls[ball].taps <= 2) {
                            work->balls[ball].size = 1;
                        } else if (work->balls[ball].taps <= 3) {
                            work->balls[ball].size = 2;
                        } else {
                            work->balls[ball].size = 3;
                        }
                    }
                } else {
                    // released away from it: a throw, if the stylus went up the screen
                    VecFx32 zero = { 0, 0, 0 };
                    VecFx32 drag;
                    VecFx32 touchDrag;
                    VecFx32 touchEnd;
                    int held = work->heldBall[i];

                    VEC_Subtract(&released, &work->balls[held].dragStart, &drag);
                    touchEnd.x = touch->x << FX32_SHIFT;
                    touchEnd.z = touch->y << FX32_SHIFT;
                    touchEnd.y = 0;
                    VEC_Subtract(&touchEnd, &work->balls[held].touchStart, &touchDrag);
                    if (touchDrag.z <= 0) {
                        work->balls[held].velocity = drag;
                        if (work->mons[held].energy <= 60) {
                            energyFactor = 0.5f;
                        } else if (work->mons[held].energy <= 70) {
                            energyFactor = 0.6f;
                        } else if (work->mons[held].energy <= 80) {
                            energyFactor = 0.7f;
                        } else if (work->mons[held].energy <= 90) {
                            energyFactor = 0.8f;
                        } else {
                            energyFactor = 1.0f;
                        }
                        speed = VEC_Mag(&work->balls[held].velocity) / FX32_ONE;
                        if (speed > 2.0) {
                            speed = 2.0f;
                        } else if (speed < 1.0) {
                            speed = 1.0f;
                        }
                        hold = 1.0 + (f32)work->holdFrames / 30.0;
                        strength = energyFactor * (speed * (2.0 - work->mons[held].throwWeight)) / hold;
                        if (strength > 3.0) {
                            strength = 3.0f;
                        } else if (strength < 1.0) {
                            strength = 1.0f;
                        }
                        VEC_Normalize(&work->balls[held].velocity, &work->balls[held].velocity);
                        VEC_MultAdd(4096.0f * strength, &work->balls[held].velocity, &zero, &work->balls[held].velocity);
                        PokeathlonSnowThrow_AddAimSpread((u8)work->mons[held].aimSpread, &work->balls[held].velocity);
                        work->balls[held].state = SNOW_THROW_BALL_FLYING;
                    }
                }
                work->heldBall[i] = SNOW_THROW_NONE;
            }
            work->holdFrames = 0;
        }
    }
    PokeathlonSnowThrowCpu_Update(work->cpu, SNOW_THROW_DURATION - work->timer);
    PokeathlonSnowThrow_UpdateMons(course);
    PokeathlonSnowThrow_MoveBalls(course);
    PokeathlonSnowThrow_CollideBalls(work);
    PokeathlonSnowThrow_HitMons(course, work);
    if (work->timer == 0) {
        work->finished = TRUE;
    } else {
        work->finished = FALSE;
    }
    block++;
    PokeathlonSnowThrow_PackPacket(work, (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes(block));
}

// Host: packs what everyone draws (see PokeathlonSnowThrowPacket).

static void PokeathlonSnowThrow_PackPacket(PokeathlonSnowThrow *work, PokeathlonSnowThrowPacket *packet) {
    u8 sizes[SNOW_THROW_TEAMS] = { 0, 0, 0, 0 };
    u16 hits[SNOW_THROW_TEAMS] = { 0, 0, 0, 0 };
    int i;
    u32 states = 0;
    u8 hit, wasHit;
    int shift;
    int x, z, team;
    u8 *ballBytes, *monBytes, *packetBytes;

    // (steps byte pointers through the snowballs, the Pokemon and the packet, as built)
#define BALL ((PokeathlonSnowThrowBall *)(ballBytes + offsetof(PokeathlonSnowThrow, balls)))
#define MON  ((PokeathlonSnowThrowMon *)(monBytes + offsetof(PokeathlonSnowThrow, mons)))
    for (i = 0, shift = 0, ballBytes = (u8 *)work, monBytes = (u8 *)work, packetBytes = (u8 *)packet; i < SNOW_THROW_MONS; shift += 2, ballBytes += sizeof(PokeathlonSnowThrowBall), packetBytes += sizeof(u16), monBytes += sizeof(PokeathlonSnowThrowMon), i++) {
        states |= (u32)BALL->state << shift;
        wasHit = MON->wasHit;
        hit = BALL->hit != 0 ? 1 : 0;
        BALL->hit = FALSE;
        x = BALL->position.x / FX32_ONE;
        z = BALL->position.z / FX32_ONE;
        if (x < 1) {
            x = 1;
        } else if (x > 64) {
            x = 64;
        }
        if (z < 1) {
            z = 1;
        } else if (z > 64) {
            z = 64;
        }
        team = i / SNOW_THROW_MEMBERS;
        sizes[team] += BALL->size << ((i % SNOW_THROW_MEMBERS) * 2);
        {
            int value = (x - 1) + ((z - 1) << 6) + (wasHit << 12) + (hit << 13);
            value += MON->condition << 14;
            *(u16 *)(packetBytes + offsetof(PokeathlonSnowThrowPacket, mons)) = value;
        }
        hits[team] += MON->hits;
        if (hits[team] > 999) {
            hits[team] = 999;
        }
    }
#undef BALL
#undef MON
    for (i = 0; i < SNOW_THROW_TEAMS; i++) {
        packet->teams[i] = (sizes[i] << 10) + hits[i];
    }
    packet->flags = states;
    packet->flags = states + ((u32)work->finished << 24);
    packet->flags += ((u32)((work->timer + 30) / 30)) << 25;
}

// Sends the own stylus to the host. TRUE when the host says the event is over.

static BOOL PokeathlonSnowThrow_Update(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowLink *link;
    PokeathlonSnowThrowTouch *touch;

    work = PokeathlonCourse_GetWork(course);
    link = (PokeathlonSnowThrowLink *)PokeathlonCourse_GetDataCopyArea(course);
    touch = (PokeathlonSnowThrowTouch *)PokeathlonCourse_ToBytes(link);
    touch->held = FALSE;
    {
        PokeathlonSnowThrowPacket *packet = (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes(&link->packet);
        u8 finished = (packet->flags >> 24) & 1;
        if (finished) {
            PokeathlonCourseSpritePool_PauseAnims(work->spritePool, TRUE);
            PokeathlonCourseObjects_PauseAnims(work->monObjectList, TRUE);
            PokeathlonCourse_SetFinishCallback(course, PokeathlonSnowThrow_ShowOwnScore);
            return TRUE;
        }
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        touch->x = gSystem.touchX;
        touch->y = gSystem.touchY;
        touch->held = TRUE;
    }
    PokeathlonSnowThrow_DrawPacket(course);
    return FALSE;
}

// Every player: draws the received packet, the field turned so that the own team is at the bottom.

static void PokeathlonSnowThrow_DrawPacket(PokeathlonCourseData *course) {
    VecFx32 hitPosition;
    MtxFx43 hitRotation;
    VecFx32 hitFrom;
    VecFx32 hitTurned;
    VecFx32 ballPosition;
    MtxFx43 ballRotation;
    VecFx32 ballFrom;
    VecFx32 ballTurned;
    VecFx32 scale;
    int hitX;
    int hitY;
    int screenX;
    int screenY;
    int cellX;
    int cellZ;
    PokeathlonSnowThrow *work;
    PokeathlonSnowThrowPacket *packet;
    u8 seconds;
    u8 condition;
    u8 wasHit;
    u8 ballState;
    int ballX;
    int ballZ;
    u8 own;
    int team;
    int turns;
    const fx16 *sinCos;
    int y;
    int member;
    int k;
    u8 mon;
    u8 state;
    u8 size;
    int x;
    int z;
    int i;
    int done;
    f32 factor;

    work = PokeathlonCourse_GetWork(course);
    packet = (PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packet));
    own = PokeathlonCourse_GetNetId(course);
    seconds = (packet->flags >> 25) & 0x7F;
    PokeathlonSnowThrow_SetTimerDigits(work, seconds == 0 ? 0 : (u8)(seconds - 1));
    PokeathlonSnowThrowDisplay_SetScore(work->display, packet->teams[own] & 0x3FF);
    // where the own snowballs hit someone
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if ((u8)((packet->mons[i] >> 13) & 1) && own == i / SNOW_THROW_MEMBERS) {
            PokeathlonCourseSpritePoolSprite_Show(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], TRUE, TRUE);
            PokeathlonCourseSpritePoolSprite_SetAnim(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], 6);
            x = packet->mons[i] & 0x3F;
            z = (packet->mons[i] >> 6) & 0x3F;
            x++;
            z++;
            if (own != 0) {
                turns = SNOW_THROW_TEAMS - own;
                hitFrom.x = x << FX32_SHIFT;
                hitFrom.z = z << FX32_SHIFT;
                hitFrom.y = 0;
                hitFrom.x -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                hitFrom.z -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                sinCos = &FX_SinCosTable_[(((u8)turns * 0x4000) >> 4) << 1];
                MTX_RotY43_(&hitRotation, sinCos[0], sinCos[1]);
                MTX_MultVec43(&hitFrom, &hitRotation, &hitTurned);
                hitTurned.x += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                hitTurned.z += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                x = hitTurned.x / FX32_ONE;
                z = hitTurned.z / FX32_ONE;
            }
            PokeathlonSnowThrow_FieldToScreen(x, z, &hitX, &hitY);
            hitPosition.x = hitX << FX32_SHIFT;
            hitPosition.y = hitY << FX32_SHIFT;
            hitPosition.z = 0;
            hitPosition.y = (hitY - 8 - hitY / 20) << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->hitMarks[(u8)(i % SNOW_THROW_MEMBERS)], &hitPosition);
            PlaySE(SEQ_SE_GS_POINT3);
        }
    }
    // the own team, on the top screen
    for (k = 0; k < SNOW_THROW_MEMBERS; k++) {
        mon = own * SNOW_THROW_MEMBERS + k;
        condition = (packet->mons[mon] >> 14) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, TRUE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, FALSE);
        } else if (condition == SNOW_THROW_TIRED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, TRUE);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonSnowThrowDisplay_ShowTired(work->display, k, FALSE);
        }
        wasHit = (packet->mons[mon] >> 12) & 1;
        ballState = (packet->flags >> (mon * 2)) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 10);
            work->mons[mon].animTimer = 10;
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
            if (wasHit) {
                PlaySE(SEQ_SE_GS_TAMA);
            }
        } else if (wasHit) {
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 7);
            work->mons[mon].animTimer = 7;
            PlaySE(SEQ_SE_GS_TAMA);
            PokeathlonSnowThrowDisplay_Shake(work->display);
        } else if (ballState == SNOW_THROW_BALL_FLYING && work->mons[mon].throwShown == 0) {
            if (condition == SNOW_THROW_STUNNED) {
                GF_AssertFail();
            }
            PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 4);
            work->mons[mon].animTimer = 4;
            work->mons[mon].throwShown = 1;
            if (work->unk5EF[k] == 0) {
                PlaySE(SEQ_SE_GS_FW104);
            }
        } else {
            done = FALSE;
            if (work->mons[mon].animTimer == 10) {
                done = TRUE;
            } else if (!PokeathlonSnowThrowDisplay_IsPicAnimated(work->display, k)) {
                done = TRUE;
            }
            if (done) {
                PokeathlonSnowThrowDisplay_SetPicAnim(work->display, k, 1);
                work->mons[mon].animTimer = 1;
            }
        }
        if (ballState != SNOW_THROW_BALL_FLYING) {
            work->mons[mon].throwShown = 0;
        }
    }
    // the other teams' Pokemon on the field
    for (k = 0; k < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; k++) {
        mon = PokeathlonSnowThrow_OtherMon(own, k);
        condition = (packet->mons[mon] >> 14) & 3;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, TRUE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, FALSE);
        } else if (condition == SNOW_THROW_TIRED) {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, TRUE);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->mons[mon].stunSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->tiredMarks[k], TRUE, FALSE);
        }
        wasHit = (packet->mons[mon] >> 12) & 1;
        ballState = (packet->flags >> (mon * 2)) & 3;
        cellX = packet->mons[mon] & 0x3F;
        cellZ = (packet->mons[mon] >> 6) & 0x3F;
        if (condition == SNOW_THROW_STUNNED) {
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 20);
            work->mons[mon].animTimer = 20;
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
        } else if (wasHit) {
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 22);
            work->mons[mon].animTimer = 22;
        } else if (ballState == SNOW_THROW_BALL_FLYING && cellX + 1 == sSnowThrowMonPositions[mon].x / FX32_ONE && cellZ + 1 == sSnowThrowMonPositions[mon].z / FX32_ONE) {
            // just thrown
            if (condition == SNOW_THROW_STUNNED) {
                GF_AssertFail();
            }
            PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 16);
            work->mons[mon].animTimer = 16;
        } else {
            done = FALSE;
            if (work->mons[mon].animTimer == 20) {
                done = TRUE;
            } else if (!PokeathlonCourseMonObject_IsAnimated(work->monObjects[k])) {
                done = TRUE;
            }
            if (done) {
                PokeathlonCourseMonObject_SetAnim(work->monObjects[k], 0);
                work->mons[mon].animTimer = 0;
            }
        }
    }
    // the snowballs
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        state = (packet->flags >> (i * 2)) & 3;
        if (state == SNOW_THROW_BALL_FLYING || state == SNOW_THROW_BALL_HELD) {
            work->balls[i].splashShown = FALSE;
            if (state != SNOW_THROW_BALL_HELD) {
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, TRUE);
            } else {
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, FALSE);
            }
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].sprite, TRUE, TRUE);
            team = i / SNOW_THROW_MEMBERS;
            member = i % SNOW_THROW_MEMBERS;
            size = ((u8)((packet->teams[i / SNOW_THROW_MEMBERS] >> 10) & 0x3F) >> (member * 2)) & 3;
            ballX = (packet->mons[i] & 0x3F) + 1;
            ballZ = ((packet->mons[i] >> 6) & 0x3F) + 1;
            if (own != 0) {
                turns = SNOW_THROW_TEAMS - own;
                ballFrom.x = ballX << FX32_SHIFT;
                ballFrom.z = ballZ << FX32_SHIFT;
                ballFrom.y = 0;
                ballFrom.x -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballFrom.z -= FX32_CONST(SNOW_THROW_FIELD_CENTER);
                sinCos = &FX_SinCosTable_[(((u8)turns * 0x4000) >> 4) << 1];
                MTX_RotY43_(&ballRotation, sinCos[0], sinCos[1]);
                MTX_MultVec43(&ballFrom, &ballRotation, &ballTurned);
                ballTurned.x += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballTurned.z += FX32_CONST(SNOW_THROW_FIELD_CENTER);
                ballX = ballTurned.x / FX32_ONE;
                ballZ = ballTurned.z / FX32_ONE;
            }
            PokeathlonSnowThrow_FieldToScreen(ballX, ballZ, &screenX, &screenY);
            ballPosition.x = screenX << FX32_SHIFT;
            ballPosition.y = screenY << FX32_SHIFT;
            ballPosition.z = 0;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].shadowSprite, &ballPosition);
            y = screenY;
            ballPosition.y = (y - 8 - y / 20) << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].sprite, &ballPosition);
            // farther away is smaller; bigger snowballs are bigger
            scale = sSnowThrowPacketScale;
            factor = ((f32)screenY + 312.0) / 752.0 * (1.0 + (f32)size / 3.0);
            scale.x = 4096.0f * factor;
            scale.y = 4096.0f * factor;
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].shadowSprite, &scale, 2);
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].sprite, &scale, 2);
            PokeathlonCourseSpritePoolSprite_SetScale(work->balls[i].splashSprite, &scale, 2);
            if (team == own && size != work->shownSizes[member]) {
                PlaySE(SEQ_SE_GS_166);
                work->shownSizes[member] = size;
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].shadowSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(work->balls[i].sprite, TRUE, FALSE);
            if (state == SNOW_THROW_BALL_SPLASHED && !work->balls[i].splashShown) {
                work->balls[i].splashShown = TRUE;
                PokeathlonCourseSpritePoolSprite_SetPosition(work->balls[i].splashSprite, PokeathlonCourseSpritePoolSprite_GetPosition(work->balls[i].sprite));
                PokeathlonCourseSpritePoolSprite_SetAnim(work->balls[i].splashSprite, 3);
                PokeathlonCourseSpritePoolSprite_Show(work->balls[i].splashSprite, TRUE, TRUE);
                if (i / SNOW_THROW_MEMBERS == own) {
                    PlaySE(SEQ_SE_GS_TAMA3);
                    work->unk5EF[i % SNOW_THROW_MEMBERS] = 0;
                    work->shownSizes[i % SNOW_THROW_MEMBERS] = 0;
                }
            }
        }
    }
    PokeathlonSnowThrow_SortSprites(work, own);
    PokeathlonCourse_UpdateTimeUpCountdown(course, seconds * 30);
}

// The own Pokemon (of `team`) at touch screen x, y: returns its index (SNOW_THROW_NONE if none) and the field point.
static u8 PokeathlonSnowThrow_FindOwnMonAt(PokeathlonSnowThrow *work, u8 team, u32 x, u32 y, VecFx32 *out) {
    f32 fieldX;
    f32 fieldZ;
    VecFx32 position;
    MtxFx43 rotation;
    VecFx32 relative;
    VecFx32 turned;
    VecFx32 diff;
    int i;

#pragma unused(work)
    PokeathlonSnowThrow_ScreenToField(x, y + 288, &fieldX, &fieldZ);
    position.x = (fx32)(fieldX * FX32_ONE);
    position.z = (fx32)(fieldZ * FX32_ONE);
    position.y = 0;
    if (team != 0) {
        // turn the view back
        relative = position;
        relative.x -= SNOW_THROW_FIELD_CENTER * FX32_ONE;
        relative.z -= SNOW_THROW_FIELD_CENTER * FX32_ONE;
        MTX_RotY43(&rotation, FX_SinIdx(team << 14), FX_CosIdx(team << 14));
        MTX_MultVec43(&relative, &rotation, &turned);
        turned.x += SNOW_THROW_FIELD_CENTER * FX32_ONE;
        turned.z += SNOW_THROW_FIELD_CENTER * FX32_ONE;
        position = turned;
    }
    *out = position;
    for (i = team * SNOW_THROW_MEMBERS; i < (team + 1) * SNOW_THROW_MEMBERS; i++) {
        VEC_Subtract(&sSnowThrowMonPositions[i], &position, &diff);
        if (VEC_Mag(&diff) <= 4 * FX32_ONE) {
            return (u8)i;
        }
    }
    return SNOW_THROW_NONE;
}

// The throw parameters of a Pokemon from its Pokeathlon stats (tables in tenths, NARC a/1/7/0 member 3).
static void PokeathlonSnowThrow_InitMon(PokeathlonCourseData *course, s32 stats[][5], u8 team, u8 member, PokeathlonSnowThrowMon *mon) {
    u8 *levels = PokeathlonCourse_GetMonStats(course, team, member);
    mon->throwWeight = (double)(float)stats[0][levels[4]] / 10.0;
    mon->throwPower = (double)(float)stats[1][levels[0]] / 10.0;
    mon->aimSpread = stats[2][levels[4]];
    mon->hitDamage = (float)stats[3][levels[3]];
    mon->hits = 0;
    mon->stunTimer = 0;
    mon->condition = SNOW_THROW_FINE;
    mon->energy = SNOW_THROW_MAX_ENERGY;
}

static void PokeathlonSnowThrow_InitWindows(PokeathlonSnowThrow *work) {
    int i;

    AddWindow(work->bgConfig, &work->scoreWindow, &sSnowThrowScoreWindowTemplate);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    FillWindowPixelBuffer(&work->scoreWindow, 0);
    for (i = 0; i < SNOW_THROW_TEAMS - 1; i++) {
        AddWindow(work->bgConfig, &work->nameWindows[i], &sSnowThrowNameWindowTemplates[i]);
        FillWindowPixelBuffer(&work->nameWindows[i], 0);
    }
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
}

static void PokeathlonSnowThrow_PrintScore(PokeathlonSnowThrow *work, u16 score) {
    MsgData *msgData;
    MessageFormat *format;
    String *string;

    FillWindowPixelBuffer(&work->scoreWindow, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    format = MessageFormat_New(work->heapId);
    BufferIntegerAsString(format, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(format, msgData, msg_0309_00164, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->scoreWindow, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(format);
    DestroyMsgData(msgData);
    CopyWindowToVram(&work->scoreWindow);
}

// Host: stun and energy recovery of every Pokemon.
static void PokeathlonSnowThrow_UpdateMons(PokeathlonCourseData *course) {
    volatile PokeathlonSnowThrow *work = PokeathlonCourse_GetWork(course); // (re-read after each write, as built)
    int i;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if (work->mons[i].stunTimer != 0) {
            work->mons[i].stunTimer = work->mons[i].stunTimer - 1;
        }
        if (work->mons[i].energy < SNOW_THROW_MAX_ENERGY) {
            work->mons[i].energy = work->mons[i].energy + 1;
        }
        if (work->mons[i].stunTimer != 0) {
            work->mons[i].condition = SNOW_THROW_STUNNED;
        } else if (work->mons[i].energy <= 100) {
            work->mons[i].condition = SNOW_THROW_TIRED;
        } else {
            work->mons[i].condition = SNOW_THROW_FINE;
        }
    }
}

// Host: moves the flying snowballs; a snowball that leaves the field is gone, a splash lasts 8 frames.

static void PokeathlonSnowThrow_MoveBalls(PokeathlonCourseData *course) {
    PokeathlonSnowThrow *work = PokeathlonCourse_GetWork(course);
    int i;
    BOOL gone;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        gone = FALSE;
        if (work->balls[i].state == SNOW_THROW_BALL_FLYING) {
            VEC_MultAdd(FX32_ONE, &work->balls[i].velocity, &work->balls[i].position, &work->balls[i].position);
        } else if (work->balls[i].state == SNOW_THROW_BALL_SPLASHED) {
            work->balls[i].splashTimer++;
            if (work->balls[i].splashTimer >= 8) {
                work->balls[i].splashTimer = 0;
                work->balls[i].state = SNOW_THROW_BALL_NONE;
            }
        }
        if (work->balls[i].position.x < FX32_ONE || work->balls[i].position.x > 64 * FX32_ONE || work->balls[i].position.z < FX32_ONE || work->balls[i].position.z > 64 * FX32_ONE) {
            gone = TRUE;
        }
        if (gone) {
            work->balls[i].state = SNOW_THROW_BALL_NONE;
            work->balls[i].velocity.x = 0;
            work->balls[i].velocity.z = 0;
            work->balls[i].velocity.y = 0;
            work->balls[i].dragStart.x = 0;
            work->balls[i].dragStart.z = 0;
            work->balls[i].dragStart.y = 0;
        }
    }
}

// PokeathlonCourse_AddMonCounter as HitMons calls it: the participant and member are passed as bytes.
typedef void (*PokeathlonSnowThrowAddMonCounterFunc)(PokeathlonCourseData *course, u8 participant, u8 member, int counter, int amount);

// Host: a flying snowball within 3 units of another team's Pokemon hits it: a hit for the thrower, energy off the
// Pokemon hit (by speed, size, the thrower's power and the target's damage factor), and a stun if that was a lot.

static void PokeathlonSnowThrow_HitMons(PokeathlonCourseData *course, PokeathlonSnowThrow *work) {
    int j, i;
    int team;
    f32 sizeFactor;
    int damage;
    int speed;
    u8 lost;
    VecFx32 diff;

    for (j = 0; j < SNOW_THROW_MONS; j++) {
        work->mons[j].wasHit = FALSE;
        for (i = 0; i < SNOW_THROW_MONS; i++) {
            if (work->balls[i].state != SNOW_THROW_BALL_FLYING) {
                continue;
            }
            team = i / SNOW_THROW_MEMBERS;
            if (team == j / SNOW_THROW_MEMBERS) {
                continue;
            }
            VEC_Subtract(&sSnowThrowMonPositions[j], &work->balls[i].position, &diff);
            if (VEC_Mag(&diff) >= 3 * FX32_ONE) {
                continue;
            }
            work->balls[i].state = SNOW_THROW_BALL_SPLASHED;
            ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, team, i % SNOW_THROW_MEMBERS, 3, 1);
            work->mons[i].hits++;
            if (work->mons[i].hits > 999) {
                work->mons[i].hits = 999;
            }
            work->balls[i].hit = TRUE;
            work->mons[j].wasHit = TRUE;
            switch (work->balls[i].size) {
            case 0:
                sizeFactor = 1.0f;
                break;
            case 1:
                sizeFactor = 20.0f;
                break;
            case 2:
                sizeFactor = 50.0f;
                break;
            case 3:
                sizeFactor = 100.0f;
                break;
            default:
                GF_ASSERT(FALSE);
                sizeFactor = 1.0f;
                break;
            }
            work->balls[i].size = 0;
            work->balls[i].taps = 0;
            speed = VEC_Mag(&work->balls[i].velocity) / FX32_ONE;
            damage = (f32)speed * sizeFactor * work->mons[i].throwPower * work->mons[j].hitDamage;
            GF_ASSERT(damage >= 0);
            work->mons[j].energy = work->mons[j].energy - damage - 30;
            if (work->mons[j].energy < 0) {
                work->mons[j].energy = 0;
            }
            lost = SNOW_THROW_MAX_ENERGY - work->mons[j].energy;
            if (work->mons[j].stunTimer == 0) {
                if (lost > 40) {
                    work->mons[j].stunTimer = lost;
                    ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, j / SNOW_THROW_MEMBERS, j % SNOW_THROW_MEMBERS, 1, 1);
                }
            } else {
                work->mons[j].stunTimer += lost;
                ((PokeathlonSnowThrowAddMonCounterFunc)PokeathlonCourse_AddMonCounter)(course, j / SNOW_THROW_MEMBERS, j % SNOW_THROW_MEMBERS, 1, 1);
            }
        }
    }
}

// Host: two flying (or held) snowballs closer than 4 units both splash.
static void PokeathlonSnowThrow_CollideBalls(PokeathlonSnowThrow *work) {
    u8 splash[SNOW_THROW_MONS];
    VecFx32 diff;
    int i;
    int j;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        splash[i] = FALSE;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        switch (work->balls[i].state) {
        case SNOW_THROW_BALL_FLYING:
        case SNOW_THROW_BALL_HELD:
            for (j = 0; j < SNOW_THROW_MONS; j++) {
                if (i == j) {
                    continue;
                }
                switch (work->balls[j].state) {
                case SNOW_THROW_BALL_FLYING:
                case SNOW_THROW_BALL_HELD:
                    VEC_Subtract(&work->balls[i].position, &work->balls[j].position, &diff);
                    if (VEC_Mag(&diff) < 4 * FX32_ONE) {
                        splash[i] = TRUE;
                        splash[j] = TRUE;
                    }
                    break;
                }
            }
            break;
        }
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if (splash[i]) {
            work->balls[i].state = SNOW_THROW_BALL_SPLASHED;
            work->balls[i].size = 0;
            work->balls[i].taps = 0;
        }
    }
}

// The other players' names, in the order of the teams after the own one.
static void PokeathlonSnowThrow_PrintNames(PokeathlonSnowThrow *work, PokeathlonCourseData *course) {
    int i;
    int participant;
    Window *window;
    String *name;
    u8 own;

    own = (u8)PokeathlonCourse_GetNetId(course);
    i = 0;
    participant = own + 1;
    window = work->nameWindows;
    for (; i < SNOW_THROW_TEAMS - 1; i++) {
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, (u8)(participant % SNOW_THROW_TEAMS)), work->heapId);
        AddTextPrinterParameterizedWithColor(window, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
        String_Delete(name);
        CopyWindowToVram(window);
        participant++;
        window++;
    }
}

static void PokeathlonSnowThrow_SetTimerDigits(PokeathlonSnowThrow *work, u32 seconds) {
    u8 value = (u8)seconds;

    Sprite_SetAnimCtrlSeq(work->timerDigits[0], (u8)(value / 10) + 1);
    Sprite_SetAnimCtrlSeq(work->timerDigits[1], (u8)(value % 10) + 1);
}

// Finish callback: the own team's hits.
static void PokeathlonSnowThrow_ShowOwnScore(PokeathlonCourseData *course) {
    u16 *teams;
    PokeathlonSnowThrow *work;
    u16 score;

    teams = ((PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packet)))->teams;
    work = PokeathlonCourse_GetWork(course);
    score = (u16)(teams[(u8)PokeathlonCourse_GetNetId(course)] & 0x3FF);
    if (score > 999) {
        score = 999;
    }
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonSnowThrow_PrintScore(work, score);
}

// MATH_QSort: lower on the screen first, else by the view's order of the Pokemon.
static int PokeathlonSnowThrow_CompareDrawOrder(const void *a, const void *b) {
    const PokeathlonSnowThrowDrawOrder *orderA = a;
    const PokeathlonSnowThrowDrawOrder *orderB = b;
    u8 own = (u8)orderA->own;
    s32 result;

    if (orderA->y > orderB->y) {
        result = -1;
    } else if (orderA->y < orderB->y) {
        result = 1;
    } else if (sSnowThrowDrawOrder[own][orderA->index] < sSnowThrowDrawOrder[own][orderB->index]) {
        result = -1;
    } else if (sSnowThrowDrawOrder[own][orderA->index] > sSnowThrowDrawOrder[own][orderB->index]) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

// The snowballs and their shadows overlap in the order of their y on the screen.
static void PokeathlonSnowThrow_SortSprites(PokeathlonSnowThrow *work, u16 own) {
    PokeathlonSnowThrowDrawOrder orders[SNOW_THROW_MONS];
    u8 i;
    u8 ball;
    VecFx32 *position;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        position = PokeathlonCourseSpritePoolSprite_GetPosition(work->balls[i].shadowSprite);
        orders[i].index = work->balls[i].index;
        orders[i].y = position->y;
        orders[i].own = own;
    }
    MATH_QSort(orders, SNOW_THROW_MONS, sizeof(PokeathlonSnowThrowDrawOrder), PokeathlonSnowThrow_CompareDrawOrder, work->sortBuffer);
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        ball = (u8)orders[i].index;
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[ball].sprite, i + 20);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[ball].splashSprite, i + 5);
    }
}

static PokeathlonSnowThrowDisplay *PokeathlonSnowThrowDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonSnowThrowDisplay *display;

    display = Heap_Alloc(heapId, sizeof(PokeathlonSnowThrowDisplay));
    MI_CpuFill8(display, 0, sizeof(PokeathlonSnowThrowDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonSnowThrowDisplay_InitWindow(display);
    PokeathlonSnowThrowDisplay_PrintName(display);
    return display;
}

static void PokeathlonSnowThrowDisplay_Delete(PokeathlonSnowThrowDisplay *display) {
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        Heap_Free(display->monPics[i].character);
        Heap_Free(display->monPics[i].palette);
    }
    Heap_Free(display->scoreScreenFile);
    RemoveWindow(&display->nameWindow);
    Heap_Free(display);
}

static void PokeathlonSnowThrowDisplay_LoadBgGraphics(PokeathlonSnowThrowDisplay *display, u32 own) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 4, display->bgConfig, GF_BG_LYR_SUB_3, 0, 0x2000, FALSE, display->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 5, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 6, display->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 7, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 8, display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, display->heapId);
    display->scoreScreenFile = GfGfxLoader_GetScrnData(NARC_a_2_1_7, 9, FALSE, &display->scoreScreen, display->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x80, display->heapId);
    // the own team's colour
    CopyRectToBgTilemapRect(display->bgConfig, GF_BG_LYR_SUB_1, 0, 18, 4, 4, display->scoreScreen->rawData, own * 4, 16, 20, 20);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
}

static void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool) {
#pragma unused(display)
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 13, 101, 2);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 10, 101, 2, 4);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 12, 101);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 11, 101);
}

static void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseMonInfo *mons) {
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, 2, 101, 5);
    }
    PokeathlonSnowThrowDisplay_CreatePicSprites(pool, display, mons);
}

static void PokeathlonSnowThrowDisplay_ShowTired(PokeathlonSnowThrowDisplay *display, u8 member, BOOL show) {
    PokeathlonCourseSpritePoolSprite_Show(display->tiredSprites[member], TRUE, show);
}

// The own team's hits in big digits (from the score screen: 5 x 2 digits of 4 x 8 tiles).

static void PokeathlonSnowThrowDisplay_SetScore(PokeathlonSnowThrowDisplay *display, u16 score) {
    u8 srcY[3], srcX[3];
    u16 digit;
    u8 i;

    digit = score / 100;
    srcY[0] = (digit % 5) * 4;
    srcX[0] = (digit / 5) * 8;
    digit = (u16)(score - 100 * digit) / 10u;
    srcY[1] = (digit % 5) * 4;
    srcX[1] = (digit / 5) * 8;
    digit = score % 10;
    srcY[2] = (digit % 5) * 4;
    srcX[2] = (digit / 5) * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(display->bgConfig, GF_BG_LYR_SUB_1, 19 + i * 4, 16, 4, 8, display->scoreScreen->rawData, srcY[i], srcX[i], 20, 20);
    }
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
}

static void PokeathlonSnowThrowDisplay_SetPicAnim(PokeathlonSnowThrowDisplay *display, u8 member, int anim) {
    if (member >= SNOW_THROW_MEMBERS) {
        GF_AssertFail();
    }
    PokeathlonCourseSpritePoolSprite_TryChangeAnim(display->picSprites[member], anim + member);
}

static BOOL PokeathlonSnowThrowDisplay_IsPicAnimated(PokeathlonSnowThrowDisplay *display, u8 member) {
    return PokeathlonCourseSpritePoolSprite_IsAnimated(display->picSprites[member]);
}

// The top screen shakes when an own Pokemon is hit.
static void PokeathlonSnowThrowDisplay_Shake(PokeathlonSnowThrowDisplay *display) {
    display->shaking = TRUE;
    display->shakeTimer = 0;
}

static void PokeathlonSnowThrowDisplay_UpdateShake(PokeathlonSnowThrowDisplay *display) {
    int offset;

    if (display->shaking) {
        display->shakeTimer++;
        offset = -((display->shakeTimer % 2) << 3);
        if (display->shakeTimer >= 10) {
            display->shaking = FALSE;
            display->shakeTimer = 0;
            offset = 0;
        }
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, offset);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, offset);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, offset);
    } else {
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
    }
}

// The shadows under the own team's pictures.

static void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseSpriteResources *monResources) {
    PokeathlonCoursePoint positions[SNOW_THROW_MEMBERS];
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        VecFx32 *position = PokeathlonCourseSpritePoolSprite_GetPosition(display->picSprites[i]);
        positions[i].x = position->x / FX32_ONE;
        positions[i].y = position->y / FX32_ONE;
        PokeathlonCourse_CreateStartSpritesInPool(display->course, 0, monResources, pool, NNS_G2D_VRAM_TYPE_2DSUB, 3, positions);
    }
}

static void PokeathlonSnowThrowDisplay_LoadMonPics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    int i = 0;
    u8 *pic = (u8 *)display;
    PokepicTemplate template;

    // (steps a byte pointer through monPics, as built)
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&template, mons->species, mons->gender, 0, mons->isShiny, (u8)mons->form, mons->personality);
        ((PokeathlonSnowThrowMonPic *)(pic + offsetof(PokeathlonSnowThrowDisplay, monPics)))->character = CharTiles_AllocAndLoadMonPicAsObjFromNarc((NarcId) template.narcID, template.charDataID, display->heapId, mons->personality, FALSE, 0, mons->species);
        ((PokeathlonSnowThrowMonPic *)(pic + offsetof(PokeathlonSnowThrowDisplay, monPics)))->palette = AllocAndLoadPltt16FromNarc((NarcId) template.narcID, template.palDataID, display->heapId);
        mons++;
        pic += sizeof(PokeathlonSnowThrowMonPic);
        i++;
    } while (i < SNOW_THROW_MEMBERS);
}

// The own team's pictures (slid in from the left, 50 pixels apart) and the tired marks above them.

static void PokeathlonSnowThrowDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    VecFx32 position;
    int i;
    volatile int x; // (kept in memory, as built)
    u8 *slot;
    PokeathlonCourseMonInfo *mon;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, 2, 101, 2);
    }
    mon = mons;
    slot = (u8 *)display;
    i = 0;
    x = 16;
    for (; i < SNOW_THROW_MEMBERS; i++) {
        void **picSprite = (void **)(slot + offsetof(PokeathlonSnowThrowDisplay, picSprites));
        void **tiredSprite = (void **)(slot + offsetof(PokeathlonSnowThrowDisplay, tiredSprites));

        *picSprite = PokeathlonCourseSpritePool_Take(pool, 101, 2);
        position.z = 0;
        position.x = x << FX32_SHIFT;
        position.y = (GetMonPicHeightBySpeciesGenderForm(mon->species, mon->gender, 0, mon->form, mon->personality) + 376) << FX32_SHIFT;
        PokeathlonCourseSpritePoolSprite_SetPosition(*picSprite, &position);
        PokeathlonCourseSpritePoolSprite_SetAnim(*picSprite, i + 1);
        PokeathlonCourseSpritePoolSprite_Show(*picSprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(*picSprite, 19 - i);
        *tiredSprite = PokeathlonCourseSpritePool_Take(pool, 101, 5);
        PokeathlonCourseSpritePoolSprite_SetAnim(*tiredSprite, 0);
        position.y -= FX32_CONST(16);
        PokeathlonCourseSpritePoolSprite_SetPosition(*tiredSprite, &position);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(*tiredSprite, 4);
        {
            int next = x + 50;
            mon++;
            slot += sizeof(void *);
            x = next;
        }
    }
    PokeathlonSnowThrowDisplay_LoadMonPics(display, mons);
    PokeathlonSnowThrowDisplay_GetPicVramOffsets(pool, display);
    PokeathlonSnowThrowDisplay_TransferMonPics(display);
}

static void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display) {
    u8 i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        DC_FlushRange(display->monPics[i].character, 3200);
        GXS_LoadOBJ(display->monPics[i].character, display->picCharOffsets[i], 3200);
        DC_FlushRange(display->monPics[i].palette, 32);
        GXS_LoadOBJPltt(display->monPics[i].palette, display->picPlttOffsets[i], 32);
    }
}

// Each picture has its own 0xD00 bytes (after 0x300) of the resource's VRAM and its own palette (after the first).
static void PokeathlonSnowThrowDisplay_GetPicVramOffsets(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display) {
    SpriteResource *charResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 0);
    SpriteResource *plttResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 1);
    NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(charResource);
    NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(plttResource, image);
    u8 i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        u32 *offsets = (u32 *)((u8 *)display + i * 4);
        u32 offset = i;
        offset *= 0xD00;
        *(u32 *)((u8 *)offsets + offsetof(PokeathlonSnowThrowDisplay, picCharOffsets)) = offset + 0x300 + NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
        {
            u32 offset = i << 5;
            offset += 0x20;
            *(u32 *)((u8 *)offsets + offsetof(PokeathlonSnowThrowDisplay, picPlttOffsets)) = offset + NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
        }
    }
}

static void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display) {
    AddWindow(display->bgConfig, &display->nameWindow, &sSnowThrowDisplayNameWindowTemplate);
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
    FillWindowPixelBuffer(&display->nameWindow, 0);
}

static void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display) {
    u8 own = (u8)PokeathlonCourse_GetNetId(display->course);
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(display->course, own);
    String *name = PlayerProfile_GetPlayerName_NewString(profile, display->heapId);

    AddTextPrinterParameterizedWithColor(&display->nameWindow, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    CopyWindowToVram(&display->nameWindow);
}

// Touch screen pixels (y + 288: the touch screen is under the top screen) to field units, through the field's
// perspective.

static void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ) {
    float x = (float)screenX, y = (float)screenY;
    float z = (float)(22.53846153846154 + y * 5.0 / 52.0);
    float xOut = (float)((x * 0.19375 + y / 8.0 - 15.5 - 10.0) / (0.39402173913043476 + y / 512.0));
    *fieldZ = z;
    *fieldX = xOut;
}

// Field units to touch screen pixels (the inverse of PokeathlonSnowThrow_ScreenToField).

static void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY) {
    float y = (float)(4.0 / (1.0 + (fieldZ - 57.0) * (fieldZ - 57.0)) + (11.0 + (fieldZ * fieldZ * fieldZ) / 7600.0 - (24.0 - fieldZ) * (24.0 - fieldZ) * (24.0 - fieldZ) / 4400.0 - 1.0));
    y *= 8.0;
    *screenY = (int)y;
    float x = (float)((10.0 + 0.34782608695652173 * fieldX) - (0.6451612903225806 * (y / 8.0 - 10.0) * (64.0 - fieldX) / 64.0));
    x *= 8.0;
    *screenX = (int)x;
}

// Turns a direction by a random angle of at most `degrees` either way.
static void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction) {
    MtxFx43 rotation;
    u16 angle = (u16)(0xB6 * (degrees - (LCRandom() % (degrees * 2))));
    MTX_RotY43(&rotation, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec43(direction, &rotation, direction);
}

static PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode) {
    PokeathlonSnowThrowCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonSnowThrowCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonSnowThrowCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = SNOW_THROW_TEAMS - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 40;
    cpu->timer = cpu->interval;
    return cpu;
}

static void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball) {
    cpu->mons[index] = mon;
    cpu->balls[index] = ball;
}

static void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu) {
    Heap_Free(cpu);
}

// Called by the host every frame with the frames gone by. Every `interval` frames it looks at one member of each
// CPU team in turn (the next member the next time). Returns the Pokemon looked at, SNOW_THROW_NONE if none.
static int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed) {
    int result = SNOW_THROW_NONE;
    int team;
    int mon;

    if (cpu->humanCount == SNOW_THROW_TEAMS) {
        return 0;
    }

    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->thinking = TRUE;
        cpu->timer = 0;
    }

    if (cpu->thinking) {
        team = cpu->firstCpu + cpu->cursor;
        mon = cpu->member + team * SNOW_THROW_MEMBERS;
        if (mon < cpu->firstCpu) {
            GF_AssertFail();
        }
        if ((cpu->balls[mon]->state == SNOW_THROW_BALL_NONE || cpu->balls[mon]->state == SNOW_THROW_BALL_HELD) && cpu->mons[mon]->condition != SNOW_THROW_STUNNED) {
            PokeathlonSnowThrowCpu_Think(cpu, (u8)team, (u8)mon, elapsed);
        }
        result = (u8)mon;
        cpu->cursor++;
        if (cpu->cursor >= cpu->cpuCount) {
            cpu->cursor = 0;
            cpu->member = (cpu->member + 1) % SNOW_THROW_MEMBERS;
            cpu->thinking = FALSE;
        }
    }

    return result;
}

// A CPU Pokemon with enough energy (by chance) makes its snowball bigger, up to 4 taps; otherwise it throws at one
// of the other teams' Pokemon that are best to hit (many hits, a stun, low energy; by chance), with its aim spread
// and at a speed of 1 to 2.
static void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed) {
    int i;
    u8 count;
    u8 pick;
    f32 speed;
    PokeathlonSnowThrowCpuTarget *targets[SNOW_THROW_MONS];
    PokeathlonSnowThrowCpuTarget *sorted[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
    VecFx32 direction;
    const VecFx32 *home;

    if (cpu->mons[mon]->energy + LCRandom() % 51 <= SNOW_THROW_MAX_ENERGY && cpu->balls[mon]->taps < 4) {
        if (cpu->balls[mon]->state == SNOW_THROW_BALL_NONE) {
            cpu->balls[mon]->taps = 1;
            cpu->balls[mon]->state = SNOW_THROW_BALL_HELD;
            cpu->balls[mon]->position = sSnowThrowCpuMonPositions[mon];
            return;
        }
        GF_ASSERT(cpu->balls[mon]->state == SNOW_THROW_BALL_HELD);
        cpu->balls[mon]->taps++;
        if (cpu->balls[mon]->taps <= 1) {
            cpu->balls[mon]->size = 0;
        } else if (cpu->balls[mon]->taps <= 2) {
            cpu->balls[mon]->size = 1;
        } else if (cpu->balls[mon]->taps <= 3) {
            cpu->balls[mon]->size = 2;
        } else {
            cpu->balls[mon]->size = 3;
        }
        return;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        cpu->targets[i].mon = cpu->mons[i];
        cpu->targets[i].index = i;
        cpu->targets[i].score = cpu->mons[i]->hits / 20 + LCRandom() % 21;
        if (cpu->mons[i]->condition == SNOW_THROW_STUNNED) {
            cpu->targets[i].score++;
        }
        cpu->targets[i].score += (SNOW_THROW_MAX_ENERGY - cpu->mons[i]->energy) / 20;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        targets[i] = &cpu->targets[i];
    }
    count = PokeathlonSnowThrowCpu_SortTargets(team, targets, sorted);
    pick = LCRandom() % count;
    {
        VecFx32 zero = { 0, 0, 0 };
        home = &sSnowThrowCpuMonPositions[mon];
        VEC_Subtract(&sSnowThrowCpuMonPositions[(u8)sorted[pick]->index], home, &direction);
        VEC_Normalize(&direction, &direction);
        PokeathlonSnowThrow_AddAimSpread(3, &direction);
        PokeathlonSnowThrow_AddAimSpread((u8)cpu->mons[mon]->aimSpread, &direction);
        // (always at least 2: the random part is not scaled down)
        speed = (f32)elapsed / 40.0 + ((f32)cpu->mons[mon]->energy / 50.0 + (f32)(LCRandom() % 51));
        if (speed > 2.0) {
            speed = 2.0f;
        } else if (speed < 1.0) {
            speed = 1.0f;
        }
        VEC_MultAdd((fx32)(4096.0f * speed), &direction, &zero, &cpu->balls[mon]->velocity);
        cpu->balls[mon]->state = SNOW_THROW_BALL_FLYING;
        cpu->balls[mon]->position = *home;
    }
}

// Insertion sort of the other teams' nine Pokemon by score, best first (into a linked list). Returns how many share
// the best score.
u8 PokeathlonSnowThrowCpu_SortTargets(u32 ownTeam, PokeathlonSnowThrowCpuTarget **targets, PokeathlonSnowThrowCpuTarget **sorted) {
    u8 count;
    u8 i;
    u8 bestCount;
    PokeathlonSnowThrowCpuTargetNode nodes[SNOW_THROW_MONS - SNOW_THROW_MEMBERS + 1];

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].target = NULL;
    bestCount = 0;
    count = bestCount;

    for (i = count; i < SNOW_THROW_MONS; i++) {
        if (i / SNOW_THROW_MEMBERS != ownTeam) {
            u8 j;
            PokeathlonSnowThrowCpuTarget *target = targets[i];
            PokeathlonSnowThrowCpuTargetNode *node = &nodes[count + 1];
            PokeathlonSnowThrowCpuTargetNode *cur = &nodes[0];

            node->target = target;
            node->next = NULL;
            node->prev = NULL;
            j = 0;
            if (count > bestCount) {
                u16 score = node->target->score;
                do {
                    cur = cur->next;
                    if (cur->target->score < score) {
                        cur->prev->next = node;
                        node->prev = cur->prev;
                        node->next = cur;
                        cur->prev = node;
                        break;
                    }
                    j++;
                } while (j < count);
            }
            if (j == count) {
                cur->next = node;
                node->prev = cur;
            }
            count++;
        }
    }

    {
        PokeathlonSnowThrowCpuTargetNode *cur = nodes[0].next;
        u16 best = cur->target->score;
        cur = &nodes[0];
        for (i = 0; i < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; i++) {
            cur = cur->next;
            sorted[i] = cur->target;
            if (best == sorted[i]->score) {
                bestCount++;
            }
        }
    }
    if (bestCount == 0) {
        GF_AssertFail();
    }
    return bestCount;
}
