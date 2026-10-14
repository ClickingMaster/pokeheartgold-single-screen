#include "pokeathlon/pokeathlon_circle_push.h"

#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "application/choose_starter/choose_starter_sub_res.naix"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

// NitroSDK math (not declared by a header yet).
void MATH_QSort(void *head, u32 num, u32 width, int (*compare)(const void *, const void *), void *stackBuffer);

// text.h declares the font id as u8; the code was compiled against this.
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

// Course code (OVY_96, not yet tidied).
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonCourseObjectInfo *info);
u8 *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
u32 PokeathlonCourse_GetSpriteSizeType(u8 *performance);
u32 PokeathlonCourse_GetCollisionSizeClass(u8 *performance);
void PokeathlonCourse_CreateStartSpritesInPool(PokeathlonCourseData *course, int baseY, void *resources, PokeathlonCourseSpritePool *pool, int whichScreen, int count, void *positions);
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
Sprite *PokeathlonCourse_AddMarker(PokeathlonCourseData *course, PokeathlonCourseObject *object, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority);
void PokeathlonCourse_InitMarkers(PokeathlonCourseData *course);
void PokeathlonCourse_FreeMarkers(PokeathlonCourseData *course);
void PokeathlonCourse_SetMarkerAnim(PokeathlonCourseData *course, u8 member, int anim);
void PokeathlonCourse_StopMarkerAnims(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, int planes);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
BOOL PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, u32 counter, u32 count);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u32 score);
BOOL PokeathlonCourse_IsInRange(int x, int y, int targetX, int targetY, int range, int *distance);
PokeathlonCourseMonInfo *PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonGfx *gfx);
void PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseObjectInfo *info, enum HeapID heapId, u16 *palette);
int PokeathlonCourseMonGfx_GetTopRow(PokeathlonCourseMonGfx *gfx);
void PokeathlonCourse_InitObjTransfer(ObjCharTransferTemplate *charTemplate, int plttCount, enum HeapID heapId, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);
void PokeathlonCourseScoreList_Rank(PokeathlonCourseScoreList *scoreList, int mode);
void PokeathlonCourseScoreList_Init(PokeathlonCourseScoreList *scoreList);
void PokeathlonCourseScoreList_AddPointsToTotals(PokeathlonCourseScoreList *scoreList);
void PokeathlonCourseScoreList_SaveEventPoints(PokeathlonCourseScoreList *scoreList, int mode);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, PokeathlonCourseScoreList *scoreList, u32 participant, u32 score, u8 idx, u8 digits, u32 msgId);
PokeathlonCourseSpriteResources *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, int flags, int charVram);
void PokeathlonCourseSprites_FreeResources(PokeathlonCourseSpriteResources *resources);
PokeathlonCourseMonObjects *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int baseDrawPriority, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList);
void PokeathlonCourseMonObjects_Free(PokeathlonCourseMonObjects *objects);
void PokeathlonCourseMonObjects_Load(PokeathlonCourseMonObjects *objects, int count, PokeathlonCourseObjectInfo *infos, PokeathlonCourseObjectsTemplate *template, BOOL priority, BOOL baseIndex);
BOOL PokeathlonCourseMonObjects_IsLoaded(PokeathlonCourseMonObjects *objects);
void PokeathlonCourseMonObject_SetDrawFlag(void *object, int draw);
void PokeathlonCourseMonObject_SetShadowDrawFlag(void *object, int draw);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, int priority);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(void *object, int mode);
void PokeathlonCourseMonObject_SetAffineMatrix(void *object, VecFx32 *matrix);
void PokeathlonCourseMonObject_SetAnimState(void *object, int state);
void PokeathlonCourseMonObject_Move(PokeathlonCourseMonObject *object, int dx, int dy, int anim, int moveShadow);
void PokeathlonCourseMonObjects_SetPalette(PokeathlonCourseMonObjects *objects, u8 index, int vram, void *palette);
void PokeathlonEvent_CalcPoints(void *scoreList, int mode);
void PokeathlonCirclePushDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display);
int PokeathlonCourse_GetTouchRadius(); // called with and without an argument
Sprite **PokeathlonCourse_CreateHudSpritesInPool(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, PokeathlonCourseSpritePool *pool);

extern const PokeathlonCourseSpritePoolTemplate gPokeathlonCirclePushSpritePoolTemplate;

extern const struct {
    u16 x;
    u16 y;
} gPokeathlonCirclePushMonStartPositions[4][3];

extern const GraphicsBanks gPokeathlonCirclePushGraphicsBanks;
extern const GraphicsModes gPokeathlonCirclePushGraphicsModes;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Main0;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Main1;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Main2;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Main3;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Sub0;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Sub1;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Sub3;
extern const BgTemplate gPokeathlonCirclePushBgTemplate_Sub2;
extern const VecFx32 gPokeathlonCirclePushDirectionReference;

// Copied as a whole (block copies).
typedef struct CirclePushTimerTiles {
    u16 tiles[8];
} CirclePushTimerTiles;

typedef struct CirclePushFallScales {
    float scales[6];
} CirclePushFallScales;

typedef struct CirclePushFallOffsets {
    int offsets[6];
} CirclePushFallOffsets;

extern const CirclePushTimerTiles gPokeathlonCirclePushTimerTiles;
extern const CirclePushFallScales gPokeathlonCirclePushFallScales;
extern const CirclePushFallOffsets gPokeathlonCirclePushFallOffsets;
extern const s32 gPokeathlonCirclePushShakeOffsets[18];
extern const s32 gPokeathlonCirclePushBounceOffsets[5];
extern const WindowTemplate gPokeathlonCirclePushScoreWindowTemplate;
extern const PokeathlonCirclePushZone *gPokeathlonCirclePushLayoutZones[];
extern const u16 *gPokeathlonCirclePushZoneKinds[];
extern const u32 gPokeathlonCirclePushNameplateCoords[];

// Contacts between the Pokemon of this frame.
typedef struct PokeathlonCirclePushContact {
    BOOL touching;
    int unk04;
    PokeathlonCirclePushMon *other;
    int unk0C;
} PokeathlonCirclePushContact;

extern PokeathlonCirclePushContact gPokeathlonCirclePushContacts[12][12];

// Collision circle of a Pokemon size (NARC a/1/7/0 #11).
typedef struct CirclePushCollision {
    s32 offsetY;
    s32 radius;
} CirclePushCollision;

static inline u32 GetMonSizeType(PokeathlonCourseData *course, int mon) {
    return PokeathlonCourse_GetCollisionSizeClass(PokeathlonCourse_GetPerformance(course, mon / 3, mon % 3));
}

static inline u32 LoadMonInfo(PokeathlonCourseData *course, int mon, PokeathlonCourseObjectInfo *infos) {
    int member = mon % 3;
    int team = mon / 3;
    PokeathlonCourse_GetMonInfo(course, team, member, &infos[mon]);
    return PokeathlonCourse_GetSpriteSizeType(PokeathlonCourse_GetPerformance(course, team, member));
}

#pragma force_active on

BOOL PokeathlonCirclePush_Init(PokeathlonCourseData *course) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        ObjCharTransferTemplate charTemplate;
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_140, 0x50000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= 0xFFFFE0FF;
        reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
        PokeathlonCirclePush_SetBanks();
        circlePush = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonCirclePush));
        MI_CpuFill8(circlePush, 0, sizeof(PokeathlonCirclePush));
        circlePush->sortBuffer = Heap_Alloc(HEAP_ID_140, 40);
        MI_CpuFill8(circlePush->sortBuffer, 0, 40);
        circlePush->bgConfig = BgConfig_Alloc(HEAP_ID_140);
        PokeathlonCourse_SetHudPlanes(course, 8);
        charTemplate.maxTasks = 104;
        charTemplate.sizeMain = 0x40000;
        charTemplate.sizeSub = 0x4000;
        charTemplate.heapID = HEAP_ID_140;
        PokeathlonCourse_InitObjTransfer(&charTemplate, 22, HEAP_ID_140, GX_OBJVRAMMODE_CHAR_1D_256K, GX_OBJVRAMMODE_CHAR_1D_32K);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_140);
        circlePush->heapId = HEAP_ID_140;
        FontID_Alloc(4, HEAP_ID_140);
        PokeathlonCirclePush_InitBgs(circlePush->bgConfig);
        PokeathlonCirclePush_AddScoreWindow(circlePush);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1: {
        int participants;
        circlePush->display = PokeathlonCirclePushDisplay_New(circlePush->heapId, circlePush->bgConfig, course);
        participants = PokeathlonCourse_GetParticipantCount(course);
        circlePush->ai = PokeathlonCirclePushAi_New(circlePush->heapId, 4 - participants, PokeathlonCourse_GetMode(course));
        circlePush->spriteResources = PokeathlonCourseSprites_CreateResources(circlePush->heapId, 2727, 1);
        circlePush->zoneMarkers = PokeathlonCirclePushZoneMarkers_New(circlePush->heapId);
        circlePush->effects.markers = circlePush->zoneMarkers;
        PokeathlonCirclePushDisplay_LoadPics(circlePush->display);
        PokeathlonCourse_InitMarkers(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 2: {
        int i;
        PokeathlonCourseSpritePoolTemplate template = gPokeathlonCirclePushSpritePoolTemplate;
        circlePush->spritePool = PokeathlonCourseSpritePool_New(circlePush->heapId, &template);
        PokeathlonCourseSpritePool_SetSurfaceCoords(circlePush->spritePool, 0, 0, 0, 0x200000);
        circlePush->objects = PokeathlonCourseMonObjects_Create(circlePush->heapId, 12, 5, circlePush->spriteResources, PokeathlonCourseSpritePool_GetSpriteList(circlePush->spritePool));
        PokeathlonCourseSpritePool_SetResourceId(circlePush->spritePool, 0, 101);
        PokeathlonCourseSpritePool_SetResourceId(circlePush->spritePool, 1, 104);
        PokeathlonCourseSpritePool_SetResourceId(circlePush->spritePool, 2, 102);
        PokeathlonCourseSpritePool_SetResourceId(circlePush->spritePool, 3, 103);
        PokeathlonCourseSpritePool_SetResourceId(circlePush->spritePool, 4, 105);
        PokeathlonCourseSpritePool_LoadChar(circlePush->spritePool, NARC_a_1_6_7, 3, 101, 1);
        PokeathlonCourseSpritePool_LoadPltt(circlePush->spritePool, NARC_a_1_6_7, 0, 101, 1, 1);
        PokeathlonCourseSpritePool_LoadCell(circlePush->spritePool, NARC_a_1_6_7, 2, 101);
        PokeathlonCourseSpritePool_LoadAnim(circlePush->spritePool, NARC_a_1_6_7, 1, 101);
        PokeathlonCourseSpritePool_LoadChar(circlePush->spritePool, NARC_a_1_6_7, 28, 104, 1);
        PokeathlonCourseSpritePool_LoadPltt(circlePush->spritePool, NARC_a_1_6_7, 25, 104, 1, 1);
        PokeathlonCourseSpritePool_LoadCell(circlePush->spritePool, NARC_a_1_6_7, 27, 104);
        PokeathlonCourseSpritePool_LoadAnim(circlePush->spritePool, NARC_a_1_6_7, 26, 104);
        PokeathlonCourseSpritePool_LoadChar(circlePush->spritePool, NARC_a_1_6_7, 32, 105, 2);
        PokeathlonCourseSpritePool_LoadPltt(circlePush->spritePool, NARC_a_1_6_7, 29, 105, 2, 5);
        PokeathlonCourseSpritePool_LoadCell(circlePush->spritePool, NARC_a_1_6_7, 31, 105);
        PokeathlonCourseSpritePool_LoadAnim(circlePush->spritePool, NARC_a_1_6_7, 30, 105);
        PokeathlonCirclePushDisplay_LoadSpriteResources(circlePush->spritePool, circlePush->display);
        PokeathlonCourseSpritePool_TransferResources(circlePush->spritePool);
        for (i = 0; i < 8; i++) {
            PokeathlonCourseSpritePool_CreateSprite(circlePush->spritePool, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 101, 2);
        }
        for (i = 0; i < 12; i++) {
            PokeathlonCourseSpritePool_CreateSprite(circlePush->spritePool, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 104, 4);
            PokeathlonCourseSpritePool_CreateSprite(circlePush->spritePool, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 104, 7);
        }
        PokeathlonCirclePushZoneMarkers_CreateSprites(circlePush->zoneMarkers, circlePush->spritePool);
        for (i = 0; i < 12; i++) {
            // A view of the event moved so that its first Pokemon is the i-th one.
            PokeathlonCirclePush *view = (PokeathlonCirclePush *)&((PokeathlonCirclePushMon(*)[3])circlePush)[i / 3][i % 3];
            view->mons[0][0].effectSprite = PokeathlonCourseSpritePool_Take(circlePush->spritePool, 104, 4);
            Sprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite_GetSprite(view->mons[0][0].effectSprite), 2);
            view->mons[0][0].tiredSprite = PokeathlonCourseSpritePool_Take(circlePush->spritePool, 104, 7);
            Sprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite_GetSprite(view->mons[0][0].tiredSprite), 4);
        }
        circlePush->markerSprite = *PokeathlonCourse_CreateHudSpritesInPool(course, 0, circlePush->spriteResources, circlePush->spritePool);
        Sprite_SetDrawPriority(circlePush->markerSprite, 1);
        PokeathlonCirclePushDisplay_CreatePicSprites(circlePush->spritePool, circlePush->display);
        PokeathlonCirclePushDisplay_CreateSprites(circlePush->spritePool, circlePush->display);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3: {
        PokeathlonCourseObjectInfo infos[12];
        PokeathlonCourseObjectsTemplate template;
        s32 i;
        for (i = 0; i < 12; i++) {
            PokeathlonCirclePushMon *mon;
            template.sizeTypes[i] = LoadMonInfo(course, i, infos);
            mon = &circlePush->mons[i / 3][i % 3];
            PokeathlonCourseMonGfx_LoadPalette(&infos[i], circlePush->heapId, mon->palette);
            MI_CpuCopy8(mon->palette, mon->grayPalette, 32);
            TintPalette_GrayScale(mon->grayPalette, 16);
        }
        template.loadAtOnce = FALSE;
        template.priority = 1;
        template.vramTransfer = FALSE;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(circlePush->objects, 12, infos, &template, FALSE, FALSE);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 4: {
        s32 i;
        int own;

        struct {
            u16 x;
            u16 y;
        } ownPositions[3];

        CirclePushCollision collision[3];
        VecFx32 scale;
        VecFx32 *scalePtr = &scale;
        u8 ringData[60];
        int x;
        int y;
        if (!PokeathlonCourseMonObjects_IsLoaded(circlePush->objects)) {
            break;
        }
        own = PokeathlonCourse_GetNetId(course);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)circlePush->bgConfig);
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(collision, NARC_a_1_7_0, 11);
        for (i = 0; i < 12; i++) {
            PokeathlonCourseObject *object = PokeathlonCourseMonObjects_Get(circlePush->objects, i);
            PokeathlonCourseMonGfx *gfx;
            int team;
            int member;
            PokeathlonCirclePushMon *mon;
            int topRow;
            int startX;
            int startY;
            PokeathlonCourseMonObject_SetDrawFlag(object, 1);
            {
                // collision circle by size type 1..3
                CirclePushCollision *entry = &collision[GetMonSizeType(course, i)];
                PokeathlonCourseObject_SetCollision(object, entry[-1].offsetY, entry[-1].radius);
            }
            object = PokeathlonCourseMonObjects_Get(circlePush->objects, i);
            gfx = PokeathlonCourseMonObject_GetGfx(object);
            scalePtr->x = 0;
            scalePtr->y = 0;
            scalePtr->z = 0;
            team = i / 3;
            member = i % 3;
            mon = &circlePush->mons[team][member];
            mon->object = object;
            mon->state = 1;
            topRow = PokeathlonCourseMonGfx_GetTopRow(gfx);
            if (((u16 *)PokeathlonCourseMonGfx_GetInfo(gfx))[2]) {
                scale.x = 0x20000;
                scale.y = (topRow + (64 - topRow) / 2) << FX32_SHIFT;
            } else {
                scale.x = 0x10000;
                scale.y = (topRow + (32 - topRow) / 2) << FX32_SHIFT;
            }
            startX = gPokeathlonCirclePushMonStartPositions[team][member].x;
            startY = gPokeathlonCirclePushMonStartPositions[team][member].y;
            mon->direction = 2;
            PokeathlonCourseMonObject_SetAnimState(object, 2);
            PokeathlonCourseObject_SetGroundPosition(object, startX, startY);
            PokeathlonCourseObject_SetTouchRadius(object, PokeathlonCourse_GetTouchRadius());
            PokeathlonCourseObject_GroundToCenter(object, startX, startY, &x, &y);
            mon->x = x << FX32_SHIFT;
            mon->y = y << FX32_SHIFT;
            mon->position.x = x << FX32_SHIFT;
            mon->position.y = y << FX32_SHIFT;
            mon->target.x = x << FX32_SHIFT;
            mon->target.y = y << FX32_SHIFT;
            if (team == PokeathlonCourse_GetNetId(course)) {
                PokeathlonCourseMonObject_SetDrawPriority(object, 5);
            } else {
                PokeathlonCourseMonObject_SetDrawPriority(object, 6);
            }
            PokeathlonCourseMonObject_SetAffineMatrix(object, &scale);
            if (PokeathlonCourse_GetNetId(course) == 0) {
                PokeathlonCirclePush_MakePacket(circlePush, (PokeathlonCirclePushPacket *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0x28));
            }
            if (team == own) {
                ownPositions[member].x = x;
                ownPositions[member].y = y;
                Sprite_SetDrawPriority(PokeathlonCourse_AddMarker(course, object, circlePush->spriteResources, PokeathlonCourseSpritePool_GetSpriteList(circlePush->spritePool), 1), 3);
            }
        }
        PokeathlonCourse_CreateStartSpritesInPool(course, 0, circlePush->spriteResources, circlePush->spritePool, 1, 3, ownPositions);
        ReadWholeNarcMemberByIdPair(ringData, NARC_a_1_7_0, 1);
        {
            int team;
            int member;
            for (team = 0; team < 4; team++) {
                circlePush->heldMons[team] = 12;
                for (member = 0; member < 3; member++) {
                    PokeathlonCirclePushMon *mon = &circlePush->mons[team][member];
                    PokeathlonCirclePush_SetMonStats(course, (PokeathlonCirclePushRingData *)ringData, team, member, mon);
                    PokeathlonCirclePushAi_SetMon(circlePush->ai, member + team * 3, mon->pushSpeed, &mon->stamina, &mon->position, &mon->state, &mon->target, mon->object);
                }
            }
        }
        circlePush->layout = 0;
        circlePush->nextLayout = 0;
        circlePush->effects.bounceMon = 12;
        PokeathlonCirclePushZoneMarkers_Show(circlePush->zoneMarkers, circlePush->layout);
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0, 12, 4);
        circlePush->scoringMon = 12;
        PokeathlonCirclePush_LoadBgGfx(circlePush);
        PokeathlonCirclePushDisplay_PrintPlayerNames(circlePush->display);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 5:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonCirclePush_Main(PokeathlonCourseData *course) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourseSpritePool_Draw(circlePush->spritePool);
    return TRUE;
}

// The event's points: the points the teams collected.
BOOL PokeathlonCirclePush_CalcResults(PokeathlonCourseData *course) {
    PokeathlonCourseScoreList *scoreList;
    int i;
    PokeathlonCourse_GetHeapAllocPtr4(course);
    scoreList = PokeathlonCourse_GetGraphicsSystem(course);
    PokeathlonCourseScoreList_Init(scoreList);
    for (i = 0; i < 4; i++) {
        PokeathlonCourse_AddScoreEntry(course, scoreList, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, msg_0309_00293);
    }
    PokeathlonCourseScoreList_Rank(scoreList, 1);
    PokeathlonEvent_CalcPoints(scoreList, 2);
    PokeathlonCourseScoreList_AddPointsToTotals(scoreList);
    PokeathlonCourseScoreList_SaveEventPoints(scoreList, 2);
    return TRUE;
}

#pragma force_active on

BOOL PokeathlonCirclePush_Exit(PokeathlonCourseData *course) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    WirelessStatusIcon_Destroy();
    PokeathlonCourse_FreeMarkers(course);
    PokeathlonCirclePushZoneMarkers_Free(circlePush->zoneMarkers);
    Heap_Free(circlePush->ringScreenFile);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(circlePush->bgConfig, GF_BG_LYR_SUB_3);
    RemoveWindow(&circlePush->scoreWindow);
    Heap_Free(circlePush->bgConfig);
    PokeathlonCourseSpritePool_Free(circlePush->spritePool);
    PokeathlonCourseMonObjects_Free(circlePush->objects);
    PokeathlonCourseSprites_FreeResources(circlePush->spriteResources);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    PokeathlonCirclePushAi_Free(circlePush->ai);
    PokeathlonCirclePushDisplay_Free(circlePush->display);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    Heap_Free(circlePush->sortBuffer);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_140);
    return TRUE;
}

#pragma force_active reset

void PokeathlonCirclePush_SetBanks(void) {
    GraphicsBanks banks = gPokeathlonCirclePushGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

BOOL PokeathlonCirclePush_FadeIn(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        PokeathlonCourse_SetStateTransitionType(course, 0x15);
        (*state)++;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0, 6, 1, circlePush->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}

BOOL PokeathlonCirclePush_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonCirclePush_RunHost(course);
        if (PokeathlonCirclePush_Update(course)) {
            (*state)++;
        }
        break;
    case 2:
        if (PokeathlonCourse_RunFinish(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, circlePush->heapId);
            PokeathlonCourse_SetStateField07(course, 2);
        }
        break;
    }
    return FALSE;
}

// The host keeps the teams' points of the event (the points of their Pokemon).
BOOL PokeathlonCirclePush_End(PokeathlonCourseData *course, u8 *state) {
    int team;
    PokeathlonCirclePush *circlePush;
    circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (PokeathlonCourse_GetNetId(course)) {
        return TRUE;
    }
    for (team = 0; team < 4; team++) {
        int points = 0;
        int member = points;
        // Walks through the team by moving a view of the event one Pokemon on.
        PokeathlonCirclePush *view = circlePush;
        for (; member < 3; member++) {
            points += view->mons[0][0].points;
            PokeathlonCourse_SetField5E0_AtIndex(course, (u8)team, points);
            view = (PokeathlonCirclePush *)((u8 *)view + sizeof(PokeathlonCirclePushMon));
        }
        circlePush = (PokeathlonCirclePush *)((u8 *)circlePush + sizeof(circlePush->mons[0]));
    }
    return TRUE;
}

void PokeathlonCirclePush_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0;
    BgTemplate main1;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub3;
    BgTemplate sub2;

    modes = gPokeathlonCirclePushGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    main0 = gPokeathlonCirclePushBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = gPokeathlonCirclePushBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main2 = gPokeathlonCirclePushBgTemplate_Main2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = gPokeathlonCirclePushBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = gPokeathlonCirclePushBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = gPokeathlonCirclePushBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub3 = gPokeathlonCirclePushBgTemplate_Sub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_AFFINE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    sub2 = gPokeathlonCirclePushBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonCirclePush_LoadBgGfx(PokeathlonCirclePush *circlePush) {
    void *file;
    NNSG2dScreenData *screen;

    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 5, circlePush->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, circlePush->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 9, circlePush->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, circlePush->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 12, circlePush->bgConfig, GF_BG_LYR_SUB_1, 0, 0x1000, FALSE, circlePush->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 22, circlePush->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, circlePush->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_6_7, 15, circlePush->bgConfig, GF_BG_LYR_SUB_2, 0x80, 0, FALSE, circlePush->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_6_7, 6, circlePush->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, circlePush->heapId);
    file = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 10, FALSE, &screen, circlePush->heapId);
    BG_LoadScreenTilemapData(circlePush->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, screen->szByte);
    BgTilemapRectChangePalette(circlePush->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, 1);
    ScheduleBgTilemapBufferTransfer(circlePush->bgConfig, GF_BG_LYR_MAIN_2);
    Heap_Free(file);
    circlePush->ringScreenFile = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 7, FALSE, &circlePush->ringScreen, circlePush->heapId);
    BG_LoadScreenTilemapData(circlePush->bgConfig, GF_BG_LYR_MAIN_1, circlePush->ringScreen->rawData, circlePush->ringScreen->szByte);
    BgCopyOrUncompressTilemapBufferRangeToVram(circlePush->bgConfig, GF_BG_LYR_MAIN_1, circlePush->ringScreen->rawData, circlePush->ringScreen->szByte, 0);
    PokeathlonCirclePushDisplay_LoadScreens(circlePush->display);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, circlePush->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 0x20, circlePush->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 21, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, circlePush->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 11, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, 0x20, circlePush->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_1_6_7, 14, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_3_OFFSET, 0x20, circlePush->heapId);
    reg_G2_BG2OFS = 0x00800000;
}

// The host's state of the twelve Pokemon for the players (the packet).
#pragma force_active on

void PokeathlonCirclePush_MakePacket(PokeathlonCirclePush *circlePush, PokeathlonCirclePushPacket *packet) {
    u8 i;
    int x;
    int y;
    u32 points;
    int team;
    PokeathlonCirclePushMon(*mons)[3];
    u32 shapeBits;
    u32 stateBits;
    u32 heldBits;

    shapeBits = 0;
    stateBits = 0;
    heldBits = 0;
    i = 0;
    mons = circlePush->mons;
    for (; i < 12; i++) {
        PokeathlonCirclePushMon *mon = &mons[(int)i / 3][(int)i % 3];

        x = mon->position.x / FX32_ONE;
        y = mon->position.y / FX32_ONE;
        if (x < 0) {
            x = 0;
        } else if (x > 255) {
            x = 255;
        }
        if (y < 0) {
            y = 0;
        } else if (y > 255) {
            y = 255;
        }
        {
            u8 state = mon->state;
            u8 direction = mon->direction;

            packet->x[i] = x;
            packet->y[i] = y;
            shapeBits += ((u32)direction - 1) << (2 * i);
            stateBits += (u32)state << (2 * i);
        }
        heldBits += mon->exhausted << i;
    }

    packet->shapeBits = shapeBits;
    packet->shapeBits += (u32)circlePush->timer << 24;
    packet->stateBits = 0;
    packet->stateBits += stateBits;
    packet->stateBits += (u32)circlePush->nextLayout << 24;
    packet->stateBits += (u32)circlePush->scoringMon << 28;

    team = circlePush->packetTeam;
    points = circlePush->scores[team];
    circlePush->packetTeam = (team + 1) % 4;

    packet->flagBits = 0;
    packet->flagBits += heldBits;
    packet->flagBits += (u32)circlePush->over << 12;
    packet->flagBits += (u32)circlePush->roundStart << 13;
    packet->flagBits += (u32)circlePush->scoring << 14;
    packet->flagBits += (u32)circlePush->scoringEffect << 15;
    packet->flagBits += (u8)points << 18;
    packet->flagBits += (u32)team << 26;
    packet->flagBits += (u32)circlePush->round << 28;

    circlePush->roundStart = 0;
    circlePush->scoringMon = 12;
    circlePush->scoringEffect = 0;
}

#pragma force_active reset

// Every player: sends the touch, follows the host's packet. Returns TRUE when the event is over.
BOOL PokeathlonCirclePush_Update(PokeathlonCourseData *course) {
    PokeathlonCirclePush *circlePush;
    u8 *copyArea;
    PokeathlonCirclePushInput *input;

    circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    copyArea = PokeathlonCourse_GetDataCopyArea(course);
    input = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(copyArea);
    input->touching = FALSE;
    copyArea = PokeathlonCourse_ToBytes(copyArea + 0xF0);
    {
        s32 flagBits = ((PokeathlonCirclePushPacket *)copyArea)->flagBits;
        u8 over = (flagBits >> 12) & 1;
        if (over) {
            PokeathlonCourseSpritePool_PauseAnims(circlePush->spritePool, TRUE);
            PokeathlonCourseObjects_PauseAnims(circlePush->objects, TRUE);
            PokeathlonCourse_StopMarkerAnims(course);
            u32 score = PokeathlonCirclePushDisplay_GetScore(circlePush->display);
            PokeathlonCourse_SetRecordScore(course, score);
            PokeathlonCirclePush_PrintScore(circlePush, score);
            return TRUE;
        }
    }
    if (circlePush->changeLayout != 0) {
        int kind;
        u8 round = circlePush->round;
        if (round < 1) {
            kind = 0;
        } else if (round < 3) {
            kind = 1;
        } else if (round < 4) {
            kind = 2;
        } else {
            kind = 3;
        }
        circlePush->nextLayout = PokeathlonCirclePush_ChooseLayout((u8)circlePush->nextLayout, kind);
        circlePush->changeLayout = 0;
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        input->x = gSystem.touchX;
        input->y = gSystem.touchY;
        input->touching = TRUE;
    }
    PokeathlonCirclePush_UpdateScreen(course);
    return FALSE;
}

// Locals of RunHost that live on the stack.
typedef struct CirclePushHostFrame {
    int count;
    u8 *copyArea;

    union {
        PokeathlonCirclePushInput *input;
        PokeathlonCirclePushMon *mon;
    } ptr;
} CirclePushHostFrame;

// Host: the round timer, the scoring at its end, the players' touches and the CPU teams; then
// moves the Pokemon and makes the packet.
#pragma force_active on

void PokeathlonCirclePush_RunHost(PokeathlonCourseData *course) {
    CirclePushHostFrame frame;
    frame.copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (PokeathlonCourse_GetNetId(course)) {
        return;
    }
    PokeathlonCirclePushPacket *packet = (PokeathlonCirclePushPacket *)PokeathlonCourse_ToBytes(frame.copyArea + 40);
    if (circlePush->waitingForReady == TRUE) {
        u8 ready = 0;
        int i;
        frame.count = PokeathlonCourse_GetParticipantCount(course);
        i = ready;
        if (frame.count > 0) {
            u8 *entry = frame.copyArea + 80;
            do {
                frame.ptr.input = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(entry);
                if (frame.ptr.input->ready) {
                    ready++;
                }
                entry += 40;
                i++;
            } while (i < frame.count);
        }
        if (ready == frame.count) {
            circlePush->timer = 0;
            circlePush->scoring = 0;
            circlePush->waitingForReady = FALSE;
        }
    } else if (circlePush->roundDelay) {
        circlePush->roundDelay--;
        if (circlePush->roundDelay == 0) {
            circlePush->waitingForReady = TRUE;
        } else if (circlePush->roundDelay == 60) {
            circlePush->changeLayout = 1;
            circlePush->round++;
        }
    } else if (circlePush->scoring) {
        circlePush->timer = 44;
        if (circlePush->scoringDelay == 0) {
            if (circlePush->scoringTimer == 0) {
                if (circlePush->scoringIndex >= circlePush->scoringCount) {
                    PokeathlonCirclePush_RespawnMons(circlePush);
                    circlePush->roundDelay = 90;
                } else {
                    circlePush->scoringMon = circlePush->scoringMons[circlePush->scoringIndex];
                    circlePush->scoringEffect = PokeathlonCirclePush_GetPointsEffect(circlePush->scoringPoints[circlePush->scoringIndex]);
                    u8 team = circlePush->scoringMon / 3;
                    int *scores = circlePush->scores;
                    scores[team] += circlePush->scoringPoints[circlePush->scoringIndex];
                    circlePush->scoringIndex++;
                    circlePush->scoringTimer = 6;
                    PokeathlonCourse_AddMonCounter(course, team, (u8)(circlePush->scoringMon % 3), 3, 1);
                }
            } else {
                circlePush->scoringTimer--;
            }
        } else {
            circlePush->scoringDelay--;
        }
    } else {
        int interval;
        if (circlePush->round < 1) {
            interval = 8;
        } else if (circlePush->round < 3) {
            interval = 6;
        } else {
            interval = 4;
        }
        circlePush->timerTick++;
        if (circlePush->timerTick >= interval) {
            circlePush->timerTick = 0;
            circlePush->timer++;
            if (circlePush->timer >= 43) {
                circlePush->scoreZones = 1;
                circlePush->roundStart = 1;
                circlePush->scoring = 1;
                circlePush->scoringDelay = 20;
                int i;
                for (i = 0; i < 4; i++) {
                    circlePush->heldMons[i] = 12;
                }
                for (i = 0; i < 12; i++) {
                    PokeathlonCirclePushMon *mon = &circlePush->mons[i / 3][i % 3];
                    mon->target = mon->position;
                    mon->velocity.x = 0;
                    mon->velocity.y = 0;
                    mon->stamina = mon->maxStamina;
                    mon->exhausted = 0;
                    mon->exhaustTimer = 0;
                    mon->boost = 0;
                    mon->boostTimer = 0;
                }
            }
        }
    }
    if (circlePush->round >= 6) {
        circlePush->over = 1;
    } else {
        circlePush->over = 0;
    }
    PokeathlonCirclePushInput *hostInput = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(frame.copyArea + 80);
    PokeathlonCirclePushInput *ownInput = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(frame.copyArea);
    *hostInput = *ownInput;
    if (!circlePush->scoring) {
        int player = 0;
        u8 *inputs = frame.copyArea + 80;
        // The touches are reached through a view of the event moved one touch on per player.
        PokeathlonCirclePush *view = circlePush;
        PokeathlonCirclePushMon *teamMons = circlePush->mons[0];
        for (; player < 4; player++, inputs += 40, view = (PokeathlonCirclePush *)((u8 *)view + sizeof(PokeathlonCirclePushTouch)), teamMons += 3) {
            PokeathlonCirclePushInput *input = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(inputs);
            if (input->touching) {
                if (view->touches[0].pressed && view->touches[0].held) {
                    view->touches[0].pressed = FALSE;
                } else if (!view->touches[0].pressed && !view->touches[0].held) {
                    view->touches[0].pressed = TRUE;
                    view->touches[0].held = TRUE;
                }
            } else {
                view->touches[0].pressed = FALSE;
                view->touches[0].held = FALSE;
            }
            if (view->touches[0].pressed) {
                if (!PokeathlonCirclePush_IsTeamOut(circlePush, player)) {
                    int mon = PokeathlonCirclePush_FindTouchedMon(circlePush, (u8)player, input->x, input->y);
                    if (mon != 12) {
                        PokeathlonCirclePushMon *touched = &teamMons[mon % 3];
                        circlePush->heldMons[player] = mon;
                        touched->boost = 2;
                        touched->boostTimer = 3;
                    }
                }
            } else if (view->touches[0].held) {
                if (circlePush->heldMons[player] != 12) {
                    u8 member = (u8)(circlePush->heldMons[player] % 3);
                    frame.ptr.mon = &teamMons[member];
                    PokeathlonCirclePush_SetTarget(input->x, input->y, &frame.ptr.mon->position, 0, &frame.ptr.mon->target);
                    if (PokeathlonCirclePush_IsOutOfRing(frame.ptr.mon->object, frame.ptr.mon->target.x, frame.ptr.mon->target.y)) {
                        circlePush->targetOutOfRing[circlePush->heldMons[player]] = 1;
                    } else {
                        circlePush->targetOutOfRing[circlePush->heldMons[player]] = 0;
                    }
                }
            } else if (circlePush->heldMons[player] != 12) {
                circlePush->heldMons[player] = 12;
            }
        }
        int mon = PokeathlonCirclePushAi_Update(circlePush->ai, (u8)circlePush->nextLayout);
        if (mon != 12) {
            u8 team = (u8)(mon / 3);
            if (team >= PokeathlonCourse_GetParticipantCount(course)) {
                PokeathlonCirclePushMon *cpuMon = &circlePush->mons[team][mon % 3];
                cpuMon->boost = 2;
                cpuMon->boostTimer = 3;
            } else {
                GF_AssertFail();
            }
        }
    }
    PokeathlonCirclePush_MoveMons(course);
    PokeathlonCirclePush_UpdateContacts(circlePush);
    PokeathlonCirclePush_ScoreZones(circlePush);
    PokeathlonCirclePush_MakePacket(circlePush, packet);
}

static inline void SetMonAnim(PokeathlonCourseData *course, PokeathlonCirclePush *circlePush, PokeathlonCirclePushMon *mon, int index, int team, u8 own, int anim, int x, int y, VecFx32 *pos, u8 scoring, Sprite *sprite) {
    void *object;

    switch (PokeathlonCourse_GetPerformance(course, index / 3, index % 3)[7]) {
    case 1:
        pos->y -= 0x8000;
        break;
    case 2:
        pos->y -= 0x8000;
        break;
    case 3:
        pos->y -= 0x10000;
        break;
    default:
        GF_AssertFail();
        break;
    }
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(mon->tiredSprite), pos);
    object = PokeathlonCourseMonObjects_Get(circlePush->objects, index);
    PokeathlonCourseMonObject_SetAnimState(object, anim);
    PokeathlonCourseObject_SetCenterPosition(object, x, y, 1);
    if (team == own && PokeathlonCourse_GetMonStats(course, own, index % 3)[2]) {
        PokeathlonCourseMonObject_Move(object, 0, circlePush->effects.shakeOffset, anim, 0);
    }
    if (scoring) {
        PokeathlonCourseObject *bounced = PokeathlonCourseMonObjects_Get(circlePush->objects, index);
        Sprite_SetAnimActiveFlag(sprite, FALSE);
        circlePush->effects.bounceActive = TRUE;
        PokeathlonCirclePush_ApplyBounce(course, &circlePush->effects, bounced, index, anim);
    } else {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
    }
}

#pragma force_active reset

// Every player: draws the Pokemon, the timer and the effects from the host's packet.
#pragma force_active on

void PokeathlonCirclePush_UpdateScreen(PokeathlonCourseData *course) {
    s32 scoringMon;
    u8 roundStart;
    u8 scoring;
    u8 packetTeam;
    u8 teamPoints;
    u8 round;
    PokeathlonCirclePushPacket *packet;
    u8 direction;
    u8 state;
    u8 exhausted;
    Sprite *sprite;
    int offset;
    u8 own;
    u8 effect;
    s32 y;
    s32 x;
    int member;
    s32 i;
    int layout;
    u8 timer;
    PokeathlonCirclePush *circlePush;
    u8 outOfRing[12];

    circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    packet = (PokeathlonCirclePushPacket *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    own = PokeathlonCourse_GetNetId(course);
    scoringMon = (packet->stateBits >> 28) & 0xF;
    layout = (packet->stateBits >> 24) & 0xF;
    timer = packet->shapeBits >> 24;
    roundStart = (packet->flagBits >> 13) & 1;
    scoring = (packet->flagBits >> 14) & 1;
    effect = (packet->flagBits >> 15) & 7;
    packetTeam = (packet->flagBits >> 26) & 3;
    teamPoints = packet->flagBits >> 18;
    round = (u8)((packet->flagBits >> 28) & 0xF) + 1;

    if (!scoring) {
        ((PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course)))->ready = 0;
    }
    {
        u8 oldLayout = circlePush->layout;
        u8 newLayout = layout;
        if (oldLayout != newLayout) {
            circlePush->layout = newLayout;
            for (i = 0; i < 12; i++) {
                circlePush->grayed[i] = 0;
            }
            PokeathlonCirclePush_StartLayoutChange(&circlePush->effects, oldLayout, newLayout, course);
        }
    }
    if (timer == 0) {
        BG_LoadScreenTilemapData(circlePush->bgConfig, GF_BG_LYR_MAIN_1, circlePush->ringScreen->rawData, circlePush->ringScreen->szByte);
        ScheduleBgTilemapBufferTransfer(circlePush->bgConfig, GF_BG_LYR_MAIN_1);
        circlePush->alarm = FALSE;
    } else if (timer < 43) {
        if (circlePush->alarm == FALSE) {
            if (round <= 1 && timer >= 32) {
                PlaySE(SEQ_SE_DP_TRAIN04);
                circlePush->alarm = TRUE;
            } else if (round > 1 && timer >= 28) {
                PlaySE(SEQ_SE_DP_TRAIN04);
                circlePush->alarm = TRUE;
            } else if (round > 3 && timer >= 21) {
                PlaySE(SEQ_SE_DP_TRAIN04);
                circlePush->alarm = TRUE;
            }
        }
        PokeathlonCirclePush_DrawTimer(timer, circlePush->bgConfig);
    } else {
        CirclePushTimerTiles tiles = gPokeathlonCirclePushTimerTiles;
        LoadRectToBgTilemapRect(circlePush->bgConfig, GF_BG_LYR_MAIN_1, &tiles, 14, 2, 4, 2);
        PokeathlonCirclePush_DrawTimer(42, circlePush->bgConfig);
    }
    if (roundStart) {
        PlaySE(SEQ_SE_GS_TIMERZERO);
    }
    PokeathlonCirclePushDisplay_SetRound(circlePush->display, round);

    for (i = 0; i < 12; i++) {
        PokeathlonCirclePushMon *mon;
        member = i % 3;
        mon = &circlePush->mons[(i / 3)][member];
        sprite = PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonObject_GetGfx(mon->object));
        state = (packet->stateBits >> (i * 2)) & 3;
        exhausted = (packet->flagBits >> i) & 1;
        if (state == 0) {
            offset = 0;
            if (circlePush->falls[i].state == 0) {
                circlePush->falls[i].state = 1;
            } else if (circlePush->falls[i].state == 1) {
                VecFx32 scale;
                CirclePushFallScales scales = gPokeathlonCirclePushFallScales;
                CirclePushFallOffsets offsets = gPokeathlonCirclePushFallOffsets;
                int groundX;
                int groundY;
                scale.x = 4096.0f * scales.scales[circlePush->falls[i].timer / 2];
                scale.y = 4096.0f * scales.scales[circlePush->falls[i].timer / 2];
                scale.z = 4096.0f * scales.scales[circlePush->falls[i].timer / 2];
                PokeathlonCourseObject_CenterToGround(mon->object, packet->x[i], packet->y[i], &groundX, &groundY);
                if (groundY >= 176) {
                    offset = offsets.offsets[circlePush->falls[i].timer / 2];
                }
                circlePush->falls[i].timer++;
                PokeathlonCourseMonObject_SetAffineOverwriteMode(mon->object, 1);
                Sprite_SetAffineScale(sprite, &scale);
                if (circlePush->falls[i].timer >= 12) {
                    circlePush->falls[i].state = 2;
                }
            } else {
                PokeathlonCourseMonObject_SetDrawFlag(mon->object, 0);
            }
            PokeathlonCourseSpritePoolSprite_Show(mon->tiredSprite, TRUE, FALSE);
            PokeathlonCourseMonObject_SetShadowDrawFlag(mon->object, 0);
            PokeathlonCourseObject_SetCenterPosition(mon->object, packet->x[i], packet->y[i] + offset, 1);
            outOfRing[i] = 1;
        } else {
            VecFx32 scale;
            circlePush->falls[i].state = 0;
            circlePush->falls[i].timer = 0;
            scale.x = FX32_ONE;
            scale.y = FX32_ONE;
            scale.z = FX32_ONE;
            PokeathlonCourseMonObject_SetAffineOverwriteMode(mon->object, 0);
            if (exhausted) {
                PokeathlonCourseSpritePoolSprite_Show(mon->tiredSprite, TRUE, TRUE);
                if ((i / 3) == own && !IsSEPlaying(SEQ_SE_GS_W146)) {
                    PlaySE(SEQ_SE_GS_W146);
                }
            } else {
                PokeathlonCourseSpritePoolSprite_Show(mon->tiredSprite, TRUE, FALSE);
            }
            Sprite_SetAffineScale(sprite, &scale);
            PokeathlonCourseMonObject_SetDrawFlag(mon->object, 1);
            if (state == 2 && circlePush->grayed[i] == 0) {
                circlePush->grayed[i] = 1;
                PokeathlonCourseMonObjects_SetPalette(circlePush->objects, i, 1, mon->grayPalette);
            } else if (state == 1 && circlePush->grayed[i] == 1) {
                circlePush->grayed[i] = 0;
                PokeathlonCourseMonObjects_SetPalette(circlePush->objects, i, 1, mon->palette);
            }
            outOfRing[i] = 0;
            x = packet->x[i];
            y = packet->y[i];
            direction = (packet->shapeBits >> (i * 2)) & 3;
            mon->x = x << FX32_SHIFT;
            mon->y = y << FX32_SHIFT;
            VecFx32 effectPos = { 0, 0, 0 };
            effectPos.x = mon->x;
            effectPos.y = mon->y;
            Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(mon->effectSprite), &effectPos);
            VecFx32 tiredPos = { 0, 0, 0 };
            tiredPos.x = mon->x;
            tiredPos.y = mon->y;
            SetMonAnim(course, circlePush, mon, i, (i / 3), own, (u8)(direction + 1), x, y, &tiredPos, scoring, sprite);
            if (scoringMon == i) {
                if (effect >= 1 && effect <= 4) {
                    Sprite *effectSprite = PokeathlonCourseSpritePoolSprite_GetSprite(mon->effectSprite);
                    PokeathlonCourseSpritePoolSprite_Show(mon->effectSprite, TRUE, TRUE);
                    Sprite_SetAnimCtrlSeq(effectSprite, effect + 2);
                    if (effect <= 2) {
                        PlaySE(SEQ_SE_GS_POINT1);
                    } else if (effect == 3) {
                        PlaySE(SEQ_SE_GS_POINT2);
                    } else {
                        PlaySE(SEQ_SE_GS_POINT3);
                    }
                    PokeathlonCirclePushDisplay_ShowPlayerEvent(circlePush->display, (u8)(i / 3), effect);
                    PokeathlonCirclePush_StartBounce(&circlePush->effects, i);
                } else {
                    GF_AssertFail();
                }
            }
            if ((i / 3) == own) {
                int groundX;
                int groundY;
                u8 zone;
                PokeathlonCourseObject_CenterToGround(mon->object, x, y, &groundX, &groundY);
                if (state == 1 && PokeathlonCirclePush_GetZonePoints(circlePush->layout, groundX, groundY, &zone)) {
                    PokeathlonCourse_SetMarkerAnim(course, member, 1);
                } else {
                    PokeathlonCourse_SetMarkerAnim(course, member, 0);
                }
            }
        }
    }
    PokeathlonCirclePush_SortByDepth(circlePush, packet);
    if (circlePush->effects.bounceActive && !scoring) {
        PokeathlonCirclePush_StopEffects(&circlePush->effects);
    }
    PokeathlonCirclePushDisplay_SetScore(circlePush->display, teamPoints, packetTeam);
    PokeathlonCirclePushDisplay_Update(circlePush->display, scoring, outOfRing);
}

#pragma force_active reset

// Host: moves every Pokemon in the ring towards its target; a Pokemon that touches another one
// pushes it (PokeathlonCirclePush_PushMons).
void PokeathlonCirclePush_MoveMons(PokeathlonCourseData *course) {
    PokeathlonCirclePush *circlePush = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;

    for (i = 0; i < 12; i++) {
        int member = i % 3;
        int team = i / 3;
        PokeathlonCirclePushMon *mon = &circlePush->mons[team][member];

        if (mon->boost != 0) {
            mon->boostTimer--;
            if (mon->boostTimer <= 0) {
                mon->boost = 0;
            }
        }
        if (mon->state != 0 && PokeathlonCirclePush_IsOutOfRing(mon->object, mon->position.x, mon->position.y)) {
            mon->state = 0;
            mon->exhaustTimer = 0;
            mon->exhausted = 0;
            PokeathlonCourse_AddMonCounter(course, team, member, 5, 1);
            PokeathlonCourse_AddMonCounter(course, team, member, 1, 1);
            if (circlePush->targetOutOfRing[i] != 0) {
                PokeathlonCourse_AddMonCounter(course, team, member, 8, 1);
            }
            circlePush->targetOutOfRing[i] = 0;
        }
    }

    for (i = 0; i < 12; i++) {
        int team = i / 3;
        int member = i % 3;
        PokeathlonCirclePushMon *mon = &circlePush->mons[team][member];

        if (mon->state == 2) {
            BOOL clear = TRUE;
            int j;
            for (j = 0; j < 12; j++) {
                int otherTeam;
                int otherMember;
                PokeathlonCirclePushMon *other;
                if (i == j) {
                    continue;
                }
                otherTeam = j / 3;
                otherMember = j % 3;
                other = &circlePush->mons[otherTeam][otherMember];
                if (other->state != 0 && PokeathlonCirclePush_MonsCollide(mon, other)) {
                    clear = FALSE;
                    break;
                }
            }
            if (clear) {
                mon->state = 1;
            }
        }
    }

    for (i = 0; i < 12; i++) {
        int team = i / 3;
        int member = i % 3;
        PokeathlonCirclePushMon *mon = &circlePush->mons[team][member];
        VecFx32 delta;
        VecFx32 direction;
        VecFx32 position;
        fx32 distance;

        if (mon->state == 0) {
            continue;
        }
        mon->pushing = 0;
        mon->velocity.x = 0;
        mon->velocity.y = 0;
        mon->velocity.z = 0;
        position = mon->position;
        VEC_Subtract(&mon->target, &position, &delta);
        distance = VEC_Mag(&delta);
        if (distance <= 0) {
            PokeathlonCirclePush_RecoverStamina(mon);
            continue;
        }
        VEC_Normalize(&delta, &direction);
        {
            float speed;
            if (mon->exhausted != 0) {
                speed = 0.5f;
            } else {
                speed = (float)mon->runSpeed / 10.0;
            }
            if ((float)distance <= 4096.0f * speed) {
                position = mon->target;
            } else {
                VecFx32 zero = { 0, 0, 0 };
                VEC_MultAdd((fx32)(4096.0f * speed), &direction, &zero, &delta);
                VEC_Add(&delta, &position, &position);
            }
        }
        if (distance != 0) {
            VecFx32 reference = gPokeathlonCirclePushDirectionReference;
            VecFx32 facing;
            u16 angle;
            facing.x = direction.y;
            facing.z = direction.x;
            facing.y = 0;
            angle = CalcAngleBetweenVecs(&reference, &facing);
            if (angle <= 0x2000 || angle >= 0xE000) {
                mon->direction = 4;
            } else if (angle > 0x2000 && angle < 0x6000) {
                mon->direction = 2;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                mon->direction = 3;
            } else {
                mon->direction = 1;
            }
        }
        if (mon->state == 1) {
            if (!PokeathlonCirclePush_CanMove(circlePush, mon, (u8)i, &delta)) {
                VecFx32 zero = { 0, 0, 0 };
                float speed;
                if (mon->exhausted != 0) {
                    continue;
                }
                mon->pushing = 1;
                speed = (float)((float)mon->pushSpeed / 10.0) + (float)mon->boost;
                VEC_MultAdd((fx32)(4096.0f * speed), &direction, &zero, &mon->velocity);
                if (mon->stamina != 0) {
                    mon->stamina--;
                } else {
                    GF_AssertFail();
                }
            } else {
                PokeathlonCirclePush_RecoverStamina(mon);
                mon->position = position;
            }
        } else {
            mon->stamina = 0;
            mon->position = position;
        }
    }
    PokeathlonCirclePush_PushMons(circlePush);
}

// The target follows the stylus once it is more than range away.
void PokeathlonCirclePush_SetTarget(int x, int y, const VecFx32 *position, fx32 range, VecFx32 *target) {
    VecFx32 delta;
    delta.x = x << FX32_SHIFT;
    delta.y = y << FX32_SHIFT;
    delta.z = 0;
    VEC_Subtract(&delta, position, &delta);
    if (VEC_Mag(&delta) > range) {
        target->x = x << FX32_SHIFT;
        target->y = y << FX32_SHIFT;
        target->z = 0;
    }
}

// The team's Pokemon nearest to the touch (12: none in reach).
#pragma force_active on

u8 PokeathlonCirclePush_FindTouchedMon(PokeathlonCirclePush *circlePush, u8 team, u16 x, u16 y) {
    int range = PokeathlonCourse_GetTouchRadius(circlePush);
    fx32 nearest = range << FX32_SHIFT;
    u8 i;
    u8 count = 0;
    u8 found;
    s32 distances[3];
    u8 inReach[3];
    int distance;
    for (i = 0; i < 3; i++) {
        PokeathlonCirclePushMon *mon = &circlePush->mons[team][i];
        if (PokeathlonCourse_IsInRange(mon->position.x / FX32_ONE, mon->position.y / FX32_ONE, x, y, range, &distance)) {
            distances[i] = distance;
            inReach[i] = TRUE;
            count++;
        } else {
            distances[i] = 0;
            inReach[i] = FALSE;
        }
    }
    if (count == 0) {
        return 12;
    }
    found = 3;
    for (i = 0; i < 3; i++) {
        if (inReach[i] && distances[i] < nearest) {
            found = i;
            nearest = distances[i];
        }
    }
    if (found == 3) {
        GF_ASSERT(FALSE);
        return 12;
    }
    return team * 3 + found;
}

#pragma force_active reset

BOOL PokeathlonCirclePush_MonsCollide(PokeathlonCirclePushMon *a, PokeathlonCirclePushMon *b) {
    VecFx32 delta;
    VecFx32 centerA;
    VecFx32 centerB;
    s32 radiusA;
    s32 radiusB;
    fx32 reach;
    PokeathlonCourseObject_GetCollisionCircle(a->object, a->position.x, a->position.y, &centerA.x, &centerA.y, &radiusA);
    PokeathlonCourseObject_GetCollisionCircle(b->object, b->position.x, b->position.y, &centerB.x, &centerB.y, &radiusB);
    reach = (radiusA + radiusB) << FX32_SHIFT;
    centerA.z = 0;
    centerB.z = 0;
    VEC_Subtract(&centerA, &centerB, &delta);
    if (VEC_Mag(&delta) > reach) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

void PokeathlonCirclePush_UpdateContacts(PokeathlonCirclePush *circlePush) {
    u8 i;
    u8 j;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++) {
            if (i == j) {
                gPokeathlonCirclePushContacts[i][j].touching = FALSE;
                gPokeathlonCirclePushContacts[i][j].other = NULL;
            } else if (j >= i) {
                PokeathlonCirclePushMon *a = &circlePush->mons[i / 3][i % 3];
                PokeathlonCirclePushMon *b = &circlePush->mons[j / 3][j % 3];
                if (a->state != 1 || b->state != 1) {
                    gPokeathlonCirclePushContacts[i][j].touching = FALSE;
                    gPokeathlonCirclePushContacts[j][i].touching = FALSE;
                    gPokeathlonCirclePushContacts[i][j].other = NULL;
                    gPokeathlonCirclePushContacts[j][i].other = NULL;
                } else if (PokeathlonCirclePush_MonsCollide(a, b)) {
                    gPokeathlonCirclePushContacts[i][j].touching = TRUE;
                    gPokeathlonCirclePushContacts[j][i].touching = TRUE;
                    gPokeathlonCirclePushContacts[i][j].other = b;
                    gPokeathlonCirclePushContacts[j][i].other = a;
                } else {
                    gPokeathlonCirclePushContacts[i][j].touching = FALSE;
                    gPokeathlonCirclePushContacts[j][i].touching = FALSE;
                    gPokeathlonCirclePushContacts[i][j].other = NULL;
                    gPokeathlonCirclePushContacts[j][i].other = NULL;
                }
            }
        }
    }
}

#pragma force_active reset

// Whether no Pokemon touching mon is in the way of a move by delta.
BOOL PokeathlonCirclePush_CanMove(PokeathlonCirclePush *circlePush, PokeathlonCirclePushMon *mon, u32 index, const VecFx32 *delta) {
    VecFx32 toOther;
    u8 i = 0;
    PokeathlonCirclePushContact *contacts = gPokeathlonCirclePushContacts[index];
#pragma unused(circlePush)
    for (; i < 12; i++) {
        if (contacts[i].touching != FALSE) {
            VEC_Subtract(&contacts[i].other->position, &mon->position, &toOther);
            if (VEC_DotProduct(delta, &toOther) > 0) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

typedef struct CirclePushPushNode {
    VecFx32 *push;

    struct {
        u8 mons[12];
        u16 count;
        u16 index;
    } next;

    VecFx32 delta[12];
} CirclePushPushNode;

// Pushing Pokemon move the Pokemon they touch, and those the ones they touch, along the push
// (a depth-first walk over the contacts); a pushing Pokemon that ran out of stamina is
// exhausted for 60 frames.
#pragma force_active on

void PokeathlonCirclePush_PushMons(PokeathlonCirclePush *circlePush) {
    CirclePushPushNode nodes[12];
    VecFx32 pushes[12][12];
    VecFx32 moved[12];
    u8 pushers[12];
    u8 visited[12];
    PokeathlonCirclePushStack stack;
    VecFx32 push;
    VecFx32 delta;
    VecFx32 last;
    PokeathlonCirclePushMon *mon;
    u8 count = 0;
    u8 pusher;
    u8 i;
    u8 j;
    u8 k;
    u8 current;
    BOOL entered;
    CirclePushPushNode *node;
    for (i = 0; i < 12; i++) {
        mon = &circlePush->mons[i / 3][i % 3];
        if (mon->pushing && mon->state == 1) {
            pushers[count++] = i;
        }
        VEC_Subtract(&mon->target, &mon->position, &moved[i]);
    }
    if (count == 0) {
        return;
    }
    MI_CpuClear32(pushes, sizeof(pushes));
    for (pusher = 0; pusher < count; pusher++) {
        current = pushers[pusher];
        stack.count = 0;
        MI_CpuFill8(nodes, 0, sizeof(nodes));
        for (i = 0; i < 12; i++) {
            nodes[i].push = &pushes[pusher][i];
        }
        MI_CpuFill8(visited, 0, sizeof(visited));
        push = circlePush->mons[current / 3][current % 3].velocity;
        entered = TRUE;
        do {
            int team = current / 3;
            int member = current % 3;
            VecFx32 *position = &circlePush->mons[team][member].position;
            visited[current] = 1;
            if (entered) {
                node = &nodes[current];
                node->next.count = 0;
                node->next.index = 0;
                for (i = 0; i < 12; i++) {
                    if (gPokeathlonCirclePushContacts[current][i].touching) {
                        VEC_Subtract(&gPokeathlonCirclePushContacts[current][i].other->position, position, &delta);
                        if (VEC_DotProduct(&push, &delta) > 0) {
                            node->next.mons[node->next.count] = i;
                            node->delta[node->next.count] = delta;
                            node->next.count++;
                        }
                    }
                }
                VEC_Add(nodes[current].push, &push, nodes[current].push);
            }
            node = &nodes[current];
            entered = FALSE;
            if (node->next.index >= node->next.count) {
                visited[current] = 0;
                current = PokeathlonCirclePushStack_Pop(&stack);
            } else if (visited[node->next.mons[node->next.index]]) {
                node->next.index++;
            } else {
                if (PokeathlonCirclePushStack_Push(&stack, current) == 255) {
                    break;
                }
                // The push goes on in its own direction when it hits nearly head-on (cos >= 1/sqrt(2)), else
                // only its share towards the next Pokemon.
                if (PokeathlonCirclePush_Cosine(node->push, &node->delta[node->next.index]) >= FX_SinCosTable_[0x401]) {
                    push = *node->push;
                } else {
                    PokeathlonCirclePush_Project(node->push, &node->delta[node->next.index], &push);
                }
                current = node->next.mons[node->next.index++];
                entered = TRUE;
            }
        } while (current != 255);
    }
    for (pusher = 0; pusher < count; pusher++) {
        for (j = 0; j < 12; j++) {
            int offset = j * sizeof(VecFx32);
            mon = &circlePush->mons[j / 3][j % 3];
            VEC_Add(&mon->position, (VecFx32 *)((u8 *)pushes[pusher] + offset), &mon->position);
            if (VEC_Mag((VecFx32 *)((u8 *)pushes[pusher] + offset)) != 0 && !mon->pushing) {
                mon->target = mon->position;
            }
        }
        current = pushers[pusher];
        mon = &circlePush->mons[current / 3][current % 3];
        if (mon->stamina == 0) {
            int team;
            for (team = 0; team < 4; team++) {
                if (circlePush->heldMons[team] == current) {
                    circlePush->heldMons[team] = 12;
                    break;
                }
            }
            if (mon->state != 1) {
                GF_AssertFail();
            }
            mon->exhausted = 1;
            mon->exhaustTimer = 60;
            mon->target = mon->position;
            mon->velocity.x = 0;
            mon->velocity.y = 0;
            mon->pushing = 0;
        }
    }
    for (k = 0; k < 12; k++) {
        mon = &circlePush->mons[k / 3][k % 3];
        VEC_Subtract(&mon->target, &mon->position, &last);
        if (VEC_DotProduct(&moved[k], &last) < 0) {
            mon->target = mon->position;
        }
    }
}

#pragma force_active reset

u8 PokeathlonCirclePushStack_Push(PokeathlonCirclePushStack *stack, u8 mon) {
    s32 count;
    if (stack->count >= 12) {
        GF_AssertFail();
    }
    count = stack->count;
    if (count >= 12) {
        return 0xFF;
    }
    stack->count = count + 1;
    stack->mons[(u8)count] = mon;
    return (u8)count;
}

u8 PokeathlonCirclePushStack_Pop(PokeathlonCirclePushStack *stack) {
    int i = stack->count - 1;
    if (i < 0) {
        return 0xFF;
    }
    stack->count = i;
    return stack->mons[i];
}

// The part of a in the direction of b.
#pragma force_active on

void PokeathlonCirclePush_Project(VecFx32 *a, VecFx32 *b, VecFx32 *out) {
    VecFx32 normA;
    VecFx32 normB;
    VecFx32 zero;
    fx32 dot;
    fx32 length;
    VEC_Normalize(a, &normA);
    VEC_Normalize(b, &normB);
    dot = VEC_DotProduct(&normA, &normB);
    length = VEC_Mag(a);
    memset(&zero, 0, sizeof(zero));
    VEC_MultAdd((fx32)(((s64)dot * length + 2048) >> 12), &normB, &zero, out);
}

#pragma force_active reset

fx32 PokeathlonCirclePush_Cosine(const VecFx32 *a, const VecFx32 *b) {
    VecFx32 normA;
    VecFx32 normB;
    VEC_Normalize(a, &normA);
    VEC_Normalize(b, &normB);
    return VEC_DotProduct(&normA, &normB);
}

// Host, end of a round: the Pokemon standing in bonus zones score their points.
#pragma force_active on

void PokeathlonCirclePush_ScoreZones(PokeathlonCirclePush *circlePush) {
    u8 zoneMons[4][12];
    u8 zonePoints[4];
    u8 counts[4];
    u8 zone;
    int groundX;
    int groundY;
    int i;
    int j;
    int points;
    if (!circlePush->scoreZones) {
        return;
    }
    for (i = 0; i < 4; i++) {
        counts[i] = 0;
    }
    circlePush->scoreZones = 0;
    circlePush->scoringCount = 0;
    circlePush->scoringIndex = 0;
    for (i = 0; i < 12; i++) {
        PokeathlonCirclePushMon *mon = &circlePush->mons[i / 3][i % 3];
        if (mon->state == 1) {
            PokeathlonCourseObject_CenterToGround(mon->object, mon->position.x / 4096, mon->position.y / 4096, &groundX, &groundY);
            points = PokeathlonCirclePush_GetZonePoints((u8)circlePush->layout, (u16)groundX, (u16)groundY, &zone);
            if (points) {
                mon->points += points;
                zoneMons[zone][counts[zone]++] = i;
                zonePoints[zone] = points;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < counts[i]; j++) {
            circlePush->scoringMons[circlePush->scoringCount] = zoneMons[i][j];
            circlePush->scoringPoints[circlePush->scoringCount] = zonePoints[i];
            circlePush->scoringCount++;
        }
    }
}

#pragma force_active reset

void PokeathlonCirclePush_DrawTimer(int time, BgConfig *bgConfig) {
    int i;

    for (i = 1; i <= time; i++) {
        PokeathlonCirclePush_DrawTimerTile((u8)i, bgConfig);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_1);
}

// The timer runs around the ring: 42 tiles of 2x2.
void PokeathlonCirclePush_DrawTimerTile(u32 index, BgConfig *bgConfig) {
    int y;
    int x;

    if (index >= 1 && index <= 6) {
        x = (index - 1) * 2 + 18;
        y = 2;
    } else if (index >= 16 && index <= 28) {
        x = 26 - (index - 16) * 2;
        y = 20;
    } else if (index >= 38 && index < 43) {
        x = (index - 38) * 2 + 4;
        y = 2;
    } else if (index >= 7 && index <= 15) {
        x = 28;
        y = (index - 7) * 2 + 4;
    } else {
        x = 2;
        y = 18 - (index - 29) * 2;
    }
    FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_1, 0, x, y, 2, 2, 16);
}

// Host: the Pokemon that fell out of the ring come back at their start.
#pragma force_active on

void PokeathlonCirclePush_RespawnMons(PokeathlonCirclePush *circlePush) {
    int team;
    int member;
    s32 x;
    s32 y;
    for (team = 0; team < 4; team++) {
        for (member = 0; member < 3; member++) {
            PokeathlonCirclePushMon *mon = &circlePush->mons[team][member];
            if (mon->state == 0) {
                mon->state = 2;
                PokeathlonCourseObject_GroundToCenter(mon->object, gPokeathlonCirclePushMonStartPositions[team][member].x, gPokeathlonCirclePushMonStartPositions[team][member].y, &x, &y);
                mon->position.x = x << FX32_SHIFT;
                mon->position.y = y << FX32_SHIFT;
                mon->target = mon->position;
                mon->direction = 2;
            }
        }
    }
}

#pragma force_active reset

// Whether none of the team's Pokemon is in the game (all fell out of the ring).
BOOL PokeathlonCirclePush_IsTeamOut(PokeathlonCirclePush *circlePush, int team) {
    int i;

    for (i = 0; i < 3; i++) {
        if (circlePush->mons[team][i].state != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void PokeathlonCirclePush_RecoverStamina(PokeathlonCirclePushMon *mon) {
    if (mon->exhausted) {
        mon->exhaustTimer--;
        if (mon->exhaustTimer == 0) {
            mon->exhaustTimer = 0;
            mon->exhausted = 0;
            mon->stamina = mon->maxStamina;
        }
    } else {
        mon->stamina += 2;
        if (mon->stamina > mon->maxStamina) {
            mon->stamina = mon->maxStamina;
        }
        if (mon->stamina == mon->maxStamina) {
            mon->exhaustTimer = 0;
            mon->exhausted = 0;
        }
    }
}

// Speeds and stamina from the Pokemon's performance (ring data NARC a/1/7/0 #1).
void PokeathlonCirclePush_SetMonStats(PokeathlonCourseData *course, PokeathlonCirclePushRingData *ringData, u8 team, u8 member, PokeathlonCirclePushMon *mon) {
    u8 *stats = PokeathlonCourse_GetMonStats(course, team, member);

    mon->pushSpeed = ringData->pushSpeeds[stats[0]];
    mon->maxStamina = ringData->staminas[stats[3]];
    mon->stamina = ringData->staminas[stats[3]];
    mon->runSpeed = ringData->runSpeeds[stats[1]];
}

void PokeathlonCirclePush_StartLayoutChange(PokeathlonCirclePushEffects *effects, u8 oldLayout, u8 newLayout, PokeathlonCourseData *course) {
    effects->oldLayout = oldLayout;
    effects->newLayout = newLayout;
    effects->state = 0;
    effects->changeTask = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonCirclePush_LayoutChangeTask, effects, 0);
    effects->fadeTask = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonCirclePush_LayoutFadeTask, effects, 1);
    effects->course = course;
    reg_G2_BG2OFS = 2 << 22;
    effects->slideTask = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonCirclePush_RingSlideTask, effects, 2);
}

void PokeathlonCirclePush_LayoutChangeTask(SysTask *task, PokeathlonCirclePushEffects *effects) {
    switch (effects->state) {
    case 0:
        if (effects->slideTask == NULL) {
            effects->shakeTask = SysTask_CreateOnMainQueue(PokeathlonCirclePush_ShakeTask, effects, 3);
            effects->state++;
        }
        break;
    case 1:
        if (effects->shakeTask == NULL) {
            PokeathlonCirclePushInput *input;
            effects->changeTask = NULL;
            effects->state = 0;
            input = (PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(effects->course));
            input->ready = 1;
            SysTask_Destroy(task);
        }
        break;
    }
}

// The zone markers of the old layout fade out, those of the new one in.
void PokeathlonCirclePush_LayoutFadeTask(SysTask *task, PokeathlonCirclePushEffects *effects) {
    u16 value;

    switch (effects->fadeState) {
    case 0:
        effects->fadeTimer++;
        if (effects->fadeTimer <= 5) {
            value = (u8)effects->fadeTimer * 12 / 5;
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0, 12 - value, value + 4);
        } else {
            effects->fadeState++;
        }
        break;
    case 1:
        PokeathlonCirclePushZoneMarkers_Show(effects->markers, (u8)effects->newLayout);
        PokeathlonCirclePushZoneMarkers_Hide(effects->markers, (u8)effects->oldLayout);
        effects->fadeTimer = 0;
        effects->fadeState++;
        PlaySE(SEQ_SE_GS_PUSH03);
        break;
    case 2:
        effects->fadeTimer++;
        if (effects->fadeTimer <= 5) {
            value = (u8)effects->fadeTimer * 12 / 5;
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0, value, 16 - value);
        } else {
            effects->fadeTask = NULL;
            effects->fadeTimer = 0;
            effects->fadeState = 0;
            SysTask_Destroy(task);
        }
        break;
    }
}

// The ring's floor (main BG 2) slides out to the left and back in.
void PokeathlonCirclePush_RingSlideTask(SysTask *task, PokeathlonCirclePushEffects *effects) {
    s32 value;

    switch (effects->slideState) {
    case 0:
        effects->slideTimer++;
        value = (effects->slideTimer << 7) / 5;
        reg_G2_BG2OFS = ((0x80 - value) << 16) & 0x01FF0000;
        if (effects->slideTimer >= 5) {
            effects->slideTimer = 0;
            effects->slideState++;
        }
        break;
    case 1:
        effects->slideTimer++;
        if (effects->slideTimer >= 20) {
            effects->slideTimer = 0;
            effects->slideState++;
        }
        break;
    case 2:
        effects->slideTimer++;
        value = -(effects->slideTimer << 7) / 5;
        reg_G2_BG2OFS = (value << 16) & 0x01FF0000;
        if (effects->slideTimer >= 5) {
            effects->slideTimer = 0;
            effects->slideTask = NULL;
            effects->slideState = 0;
            SysTask_Destroy(task);
        }
        break;
    }
}

void PokeathlonCirclePush_ShakeTask(SysTask *task, void *data) {
    PokeathlonCirclePushEffects *effects = data;
    s16 offset = effects->shakeOffset;
    u16 step = effects->shakeStep;
    effects->shakeOffset = offset + gPokeathlonCirclePushShakeOffsets[step];
    effects->shakeStep++;
    if (effects->shakeStep >= 18) {
        effects->shakeOffset = 0;
        effects->shakeTask = NULL;
        effects->shakeStep = 0;
        SysTask_Destroy(task);
    }
}

#pragma force_active on

void PokeathlonCirclePush_StopEffects(PokeathlonCirclePushEffects *effects) {
    effects->bounceActive = 0;
    if (effects->changeTask) {
        ((PokeathlonCirclePushInput *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(effects->course)))->ready = 1;
        SysTask_Destroy(effects->changeTask);
        effects->changeTask = NULL;
    }
    if (effects->slideTask) {
        SysTask_Destroy(effects->slideTask);
        effects->slideTask = NULL;
    }
    if (effects->fadeTask) {
        SysTask_Destroy(effects->fadeTask);
        effects->fadeTask = NULL;
    }
    if (effects->shakeTask) {
        SysTask_Destroy(effects->shakeTask);
        effects->shakeTask = NULL;
    }
    if (effects->bounceTask) {
        SysTask_Destroy(effects->bounceTask);
        effects->bounceTask = NULL;
    }
    reg_G2_BG2OFS = 0x800000;
    effects->state = 0;
    effects->fadeState = 0;
    effects->slideState = 0;
    effects->fadeTimer = 0;
    effects->slideTimer = 0;
    effects->shakeStep = 0;
    effects->bounceStep = 0;
    effects->shakeOffset = 0;
    effects->bounceOffset = 0;
    effects->bounceMon = 12;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0, 12, 4);
}

#pragma force_active reset

void PokeathlonCirclePush_StartBounce(PokeathlonCirclePushEffects *effects, u8 mon) {
    if (effects->bounceTask == NULL) {
        effects->bounceMon = mon;
        effects->bounceTask = SysTask_CreateOnMainQueue(PokeathlonCirclePush_BounceTask, effects, 0);
    }
}

void PokeathlonCirclePush_BounceTask(SysTask *task, void *data) {
    PokeathlonCirclePushEffects *effects = data;
    s16 offset = effects->bounceOffset;
    u16 step = effects->bounceStep;
    effects->bounceOffset = offset + gPokeathlonCirclePushBounceOffsets[step];
    effects->bounceStep++;
    if (effects->bounceStep >= 5) {
        effects->bounceOffset = 0;
        effects->bounceTask = NULL;
        effects->bounceStep = 0;
        effects->bounceMon = 12;
        SysTask_Destroy(task);
    }
}

void PokeathlonCirclePush_ApplyBounce(PokeathlonCourseData *course, PokeathlonCirclePushEffects *effects, void *object, u8 mon, int anim) {
    if (effects->bounceMon == mon) {
        s32 team = mon / 3;
        s32 member = mon % 3;

        if (PokeathlonCourse_GetMonStats(course, team, member)[2] != 0) {
            PokeathlonCourseMonObject_Move(object, 0, effects->bounceOffset, anim, 0);
        }
    }
}

void PokeathlonCirclePush_AddScoreWindow(PokeathlonCirclePush *circlePush) {
    AddWindow(circlePush->bgConfig, &circlePush->scoreWindow, &gPokeathlonCirclePushScoreWindowTemplate);
    BG_FillCharDataRange(circlePush->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, circlePush->heapId);
}

void PokeathlonCirclePush_PrintScore(PokeathlonCirclePush *circlePush, s32 score) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;

    FillWindowPixelBuffer(&circlePush->scoreWindow, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, circlePush->heapId);
    msgFormat = MessageFormat_New(circlePush->heapId);
    BufferIntegerAsString(msgFormat, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0309_00158, circlePush->heapId);
    AddTextPrinterParameterizedWithColor(&circlePush->scoreWindow, 4, string, 0, 0, 0xFF, 0xF0E00, NULL); // no transfer; colours 15, 14, 0
    String_Delete(string);
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&circlePush->scoreWindow);
}

BOOL PokeathlonCirclePush_IsOutOfRing(PokeathlonCourseObject *object, fx32 x, fx32 y) {
    int groundX;
    int groundY;

    PokeathlonCourseObject_CenterToGround(object, x / FX32_ONE, y / FX32_ONE, &groundX, &groundY);
    if (groundX < 16 || groundX >= 240 || groundY < 16 || groundY >= 176) {
        return TRUE;
    }
    return FALSE;
}

typedef struct CirclePushDepth {
    u16 mon;
    u16 y;
} CirclePushDepth;

// Lower on the screen first; then by Pokemon.
int PokeathlonCirclePush_CompareDepth(const void *a, const void *b) {
    const CirclePushDepth *depthA = a;
    const CirclePushDepth *depthB = b;
    if (depthA->y > depthB->y) {
        return -1;
    }
    if (depthA->y < depthB->y) {
        return 1;
    }
    if (depthA->mon < depthB->mon) {
        return -1;
    }
    if (depthA->mon > depthB->mon) {
        return 1;
    }
    return 0;
}

// Pokemon lower on the screen are drawn in front.
#pragma force_active on

void PokeathlonCirclePush_SortByDepth(PokeathlonCirclePush *circlePush, PokeathlonCirclePushPacket *packet) {
    CirclePushDepth depths[12];
    int groundX;
    int groundY;
    u8 i;
    for (i = 0; i < 12; i++) {
        depths[i].mon = i;
        PokeathlonCourseObject_CenterToGround(circlePush->mons[i / 3][i % 3].object, packet->x[i], packet->y[i], &groundX, &groundY);
        depths[i].y = groundY;
    }
    MATH_QSort(depths, 12, sizeof(CirclePushDepth), PokeathlonCirclePush_CompareDepth, circlePush->sortBuffer);
    for (i = 0; i < 12; i++) {
        u8 mon = depths[i].mon;
        int team = mon / 3;
        int member = mon % 3;
        PokeathlonCourseMonObject_SetDrawPriority(circlePush->mons[team][member].object, i + 6);
    }
}

#pragma force_active reset

PokeathlonCirclePushZoneMarkers *PokeathlonCirclePushZoneMarkers_New(enum HeapID heapId) {
    PokeathlonCirclePushZoneMarkers *markers = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushZoneMarkers));
    MI_CpuFill8(markers, 0, sizeof(PokeathlonCirclePushZoneMarkers));
    return markers;
}

void PokeathlonCirclePushZoneMarkers_Free(PokeathlonCirclePushZoneMarkers *markers) {
    Heap_Free(markers);
}

void PokeathlonCirclePushZoneMarkers_CreateSprites(PokeathlonCirclePushZoneMarkers *markers, PokeathlonCourseSpritePool *pool) {
    int i;
    PokeathlonCirclePushZoneMarker *marker = markers->markers;
    for (i = 0; i < 8; i++, marker++) {
        marker->sprite = PokeathlonCourseSpritePool_Take(pool, 101, 2);
        Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(marker->sprite);
        Sprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
        Sprite_SetDrawPriority(sprite, 100);
    }
}

// The next zone layout: one of the layouts of the round's kind (kind 0: layout 0; 1: 1..5,
// 2: 6..9, 3: 10..14), not the current one.
u8 PokeathlonCirclePush_ChooseLayout(u8 layout, int kind) {
    u8 first;
    u8 next;
    int count;
    int currentKind;
    if (layout == 15) {
        return 0;
    }
    if (layout < 1) {
        currentKind = 0;
    } else if (layout < 6) {
        currentKind = 1;
    } else if (layout < 10) {
        currentKind = 2;
    } else {
        currentKind = 3;
    }
    if (kind == 1) {
        first = 1;
        count = 5;
    } else if (kind == 2) {
        first = 6;
        count = 4;
    } else {
        first = 10;
        count = 5;
    }
    if (kind == currentKind) {
        u32 step;
        GF_ASSERT(layout >= first);
        step = LCRandom() % (count - 1);
        next = (u8)(layout - first + 1);
        next = (u8)(next + step);
        next = (u8)(next % (u32)count);
        return (u8)(next + first);
    }
    return (u8)(first + LCRandom() % count);
}

// Points of the bonus zone of layout that (x, y) lies in (0: none); *zone: its index.
int PokeathlonCirclePush_GetZonePoints(u8 layout, u16 x, u16 y, u8 *zone) {
    u8 i;
    u32 points;
    if (layout == 15) {
        return 0;
    }
    i = 0;
    points = 0;
    for (; i < 4; i++) {
        const PokeathlonCirclePushZone *zones = gPokeathlonCirclePushLayoutZones[layout];
        const PokeathlonCirclePushZone *entry = &zones[i];
        u32 kind = entry->kind;
        if (!kind) {
            break;
        }
        {
            const u16 *kindData = gPokeathlonCirclePushZoneKinds[kind - 1];
            if (PokeathlonCirclePush_IsInCircle(x, y, entry->x, entry->y, kindData[0])) {
                points = (u8)kindData[1];
                *zone = i;
                break;
            }
        }
    }
    return points;
}

// Shows the markers of the bonus zones of layout.
void PokeathlonCirclePushZoneMarkers_Show(PokeathlonCirclePushZoneMarkers *markers, u8 layout) {
    // The volatile layout copy and the volatile read of next keep the original's loads.
    vs32 savedLayout = layout;
    int i = 0;
    const PokeathlonCirclePushZone *zone = gPokeathlonCirclePushLayoutZones[savedLayout];
    for (; i < 4; i++, zone++) {
        if (zone->kind) {
            Sprite *sprite;
            VecFx32 position;
            {
                PokeathlonCirclePushZoneMarker *marker = &markers->markers[markers->next];
                GF_ASSERT(marker->shown != TRUE);
                marker->shown = TRUE;
                marker->layout = savedLayout;
                marker->zone = zone;
            }
            sprite = PokeathlonCourseSpritePoolSprite_GetSprite(markers->markers[markers->next].sprite);
            PokeathlonCourseSpritePoolSprite_Show(markers->markers[markers->next].sprite, TRUE, TRUE);
            Sprite_SetAnimCtrlSeq(sprite, zone->kind - 1);
            position.z = 0;
            position.x = zone->x << FX32_SHIFT;
            position.y = zone->y << FX32_SHIFT;
            Sprite_SetMatrix(sprite, &position);
            {
                s32 next = *(vs32 *)&markers->next + 1;
                markers->next = next % 8;
            }
        }
    }
}

void PokeathlonCirclePushZoneMarkers_Hide(PokeathlonCirclePushZoneMarkers *markers, u8 layout) {
    int i;
    PokeathlonCirclePushZoneMarker *marker = markers->markers;
    for (i = 0; i < 8; i++, marker++) {
        if (layout == marker->layout) {
            marker->layout = 15;
            marker->zone = NULL;
            marker->shown = FALSE;
            PokeathlonCourseSpritePoolSprite_Show(marker->sprite, FALSE, FALSE);
        }
    }
}

// Effect (sprite animation) of the points 1, 2, 3, 5.
int PokeathlonCirclePush_GetPointsEffect(int points) {
    int effect = 0;
    switch (points) {
    case 1:
        effect = 1;
        break;
    case 2:
        effect = 2;
        break;
    case 3:
        effect = 3;
        break;
    case 5:
        effect = 4;
        break;
    default:
        GF_ASSERT(FALSE);
    }
    return effect;
}

BOOL PokeathlonCirclePush_IsInCircle(int x, int y, int centerX, int centerY, u16 radius) {
    VecFx32 a;
    VecFx32 b;
    a.x = x << FX32_SHIFT;
    a.y = y << FX32_SHIFT;
    a.z = 0;
    b.x = centerX << FX32_SHIFT;
    b.y = centerY << FX32_SHIFT;
    b.z = 0;
    VEC_Subtract(&a, &b, &a);
    if ((radius << FX32_SHIFT) > VEC_Mag(&a)) {
        return TRUE;
    }
    return FALSE;
}

// The bonus zones of layout as circles for the CPU players; returns how many there are.
u8 PokeathlonCirclePush_GetZoneCircles(int layout, PokeathlonCirclePushAiZone *circles) {
    // The tables are read through volatile pointers at byte offsets, which keeps the original's loads.
    int i;
    u8 count;
    int offset;
    int tableOffset;
    count = 0;
    i = count;
    offset = count;
    tableOffset = layout << 2;
    for (; i < 4; i++, offset += 8, circles++) {
        const u8 *zones = *(const u8 *volatile *)((u8 *)gPokeathlonCirclePushLayoutZones + tableOffset);
        const PokeathlonCirclePushZone *zone = (const PokeathlonCirclePushZone *)(zones + offset);
        u32 kind = zone->kind;
        if (!kind) {
            circles->radius = 0;
            circles->x = 0;
            circles->y = 0;
        } else {
            const u16 *kindData = *(const u16 *volatile *)((u8 *)gPokeathlonCirclePushZoneKinds + kind * 4 - 4);
            circles->radius = kindData[0];
            circles->x = zone->x;
            circles->y = zone->y;
            count = (u8)(count + 1);
        }
    }
    return count;
}

// teamCount CPU teams, the last ones; they act more often in a link course.
PokeathlonCirclePushAi *PokeathlonCirclePushAi_New(enum HeapID heapId, int teamCount, int linkMode) {
    PokeathlonCirclePushAi *ai = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushAi));
    MI_CpuFill8(ai, 0, sizeof(PokeathlonCirclePushAi));
    ai->teamCount = teamCount;
    ai->firstTeam2 = 4 - teamCount;
    ai->firstTeam = ai->firstTeam2;
    if (linkMode == 0) {
        ai->interval = 20;
    } else {
        ai->interval = 10;
    }
    ai->timer = ai->interval;
    return ai;
}

void PokeathlonCirclePushAi_SetMon(PokeathlonCirclePushAi *ai, u8 slot, u16 speed, u16 *stamina, VecFx32 *position, u32 *state, VecFx32 *target, PokeathlonCourseObject *object) {
    PokeathlonCirclePushAiMon *mons = ai->mons;
    mons[slot].speed = speed;
    mons[slot].stamina = stamina;
    mons[slot].position = position;
    mons[slot].state = state;
    mons[slot].cell = -1;
    mons[slot].slot = slot;
    mons[slot].object = object;
    if (slot / 3 >= ai->firstTeam) {
        mons[slot].target = target;
    } else {
        mons[slot].target = NULL;
    }
}

void PokeathlonCirclePushAi_Free(PokeathlonCirclePushAi *ai) {
    Heap_Free(ai);
}

// Every interval frames one Pokemon of each CPU team (in turn) gets a new target. Returns it
// (12: none).
int PokeathlonCirclePushAi_Update(PokeathlonCirclePushAi *ai, u8 layout) {
    int result = 12;
    int mon;

    if (ai->firstTeam2 == 4) {
        return result;
    }
    ai->timer++;
    if (ai->timer >= ai->interval) {
        u8 i;
        ai->moving = TRUE;
        ai->timer = 0;
        for (i = 0; i < ai->teamCount; i++) {
            ai->skipped[i] = 0;
        }
    }
    if (ai->moving != FALSE) {
        u8 firstTeam = ai->firstTeam;
        mon = 2 - ai->member + (firstTeam + ai->team) * 3;
        if (mon / 3 < firstTeam) {
            GF_AssertFail();
        }
        PokeathlonCirclePushAi_ChooseTarget(ai, (u8)mon, layout);
        result = (u8)mon;
        ai->team++;
        if (ai->team >= ai->teamCount) {
            ai->team = 0;
            ai->member = (ai->member + 1) % 3;
            ai->moving = FALSE;
        }
    }
    return result;
}

// A CPU Pokemon picks a target cell around it (or anywhere, if no cell nearby is good):
// bonus zones, cells nobody else wants, staying away from stronger Pokemon.
#pragma force_active on

void PokeathlonCirclePushAi_ChooseTarget(PokeathlonCirclePushAi *ai, u8 index, u8 layout) {
    PokeathlonCirclePushAiCell *all[25];
    PokeathlonCirclePushAiCell *choices[25];
    PokeathlonCirclePushAiZone zones[4];
    PokeathlonCirclePushAiCell *near[9];
    PokeathlonCirclePushAiCell *far[16];
    int ownX;
    int ownY;
    int x;
    int y;
    s32 radius;
    int nearX;
    int nearY;
    int farX;
    int farY;
    s16 column;
    s16 row;
    PokeathlonCirclePushAi *work = ai;
    PokeathlonCirclePushAiMon *mon = &work->mons[index];
    int nearCount;
    int farCount;
    int i;
    int count;
    PokeathlonCirclePushAiCell *choice;

    if (!*mon->state) {
        return;
    }
    if (!mon->target) {
        GF_ASSERT(0);
        return;
    }
    if (work->skipped[work->member] < 3 && (int)LCRandom() % 100 < 30) {
        work->skipped[work->member]++;
        return;
    }
    for (i = 0; i < 12; i++) {
        PokeathlonCirclePushAiMon *other = &work->mons[i];

        if (*other->state) {
            PokeathlonCourseObject_GetCollisionCircle(other->object, other->position->x, other->position->y, &x, &y, &radius);
            other->cell = PokeathlonCirclePushAi_GetCell(x / FX32_ONE, y / FX32_ONE);
        } else {
            other->cell = -1;
        }
    }
    PokeathlonCourseObject_CenterToGround(mon->object, mon->position->x / FX32_ONE, mon->position->y / FX32_ONE, &ownX, &ownY);
    mon->cell = PokeathlonCirclePushAi_GetCell(ownX, ownY);
    PokeathlonCirclePush_GetZoneCircles(layout, zones);
    for (i = 0; i < 4; i++) {
        if (zones[i].radius > 0) {
            work->zoneCells[i] = PokeathlonCirclePushAi_GetCell(zones[i].x, zones[i].y);
        } else {
            work->zoneCells[i] = -1;
        }
    }
    if (!PokeathlonCirclePushAi_GetColumnRow((u16)ownX, (u16)ownY, &column, &row)) {
        GF_ASSERT(0);
    }
    nearCount = 0;
    farCount = 0;
    for (i = 0; i < 25; i++) {
        int dx;
        int dy;

        work->cells[i].index = i;
        work->cells[i].x = i % 5 - 2 + column;
        work->cells[i].y = i / 5 - 2 + row;
        if (work->cells[i].x < 0 || work->cells[i].y < 0 || work->cells[i].x >= 7 || work->cells[i].y >= 5) {
            work->cells[i].inRing = NULL;
        } else {
            work->cells[i].inRing = (void *)1;
        }
        dx = work->cells[i].x - column;
        dy = work->cells[i].y - row;
        all[i] = &work->cells[i];
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
            near[nearCount++] = &work->cells[i];
        } else {
            far[farCount++] = &work->cells[i];
        }
    }
    GF_ASSERT(nearCount == 9);
    GF_ASSERT(farCount == 16);
    PokeathlonCirclePushAi_PlaceInCells(9, near);
    near[4]->pixelX = ownX;
    near[4]->pixelY = ownY;
    PokeathlonCirclePushAi_ScoreCells(work, 9, zones, mon, near);
    count = PokeathlonCirclePushAi_SortCells(9, near, choices);
    if (choices[0]->score >= 3) {
        choice = choices[(u8)((int)LCRandom() % count)];
        PokeathlonCourseObject_GroundToCenter(mon->object, choice->pixelX, choice->pixelY, &nearX, &nearY);
        mon->target->x = (u32)nearX << FX32_SHIFT;
        mon->target->y = (u32)nearY << FX32_SHIFT;
        mon->target->z = 0;
        return;
    }
    PokeathlonCirclePushAi_PlaceInCells(16, far);
    PokeathlonCirclePushAi_ScoreCells(work, 25, zones, mon, all);
    count = PokeathlonCirclePushAi_SortCells(25, all, choices);
    choice = choices[(u8)((int)LCRandom() % count)];
    PokeathlonCourseObject_GroundToCenter(mon->object, choice->pixelX, choice->pixelY, &farX, &farY);
    mon->target->x = (u32)farX << FX32_SHIFT;
    mon->target->y = (u32)farY << FX32_SHIFT;
    mon->target->z = 0;
}

#pragma force_active reset

// A random point of each cell (cells are 32 pixels, from (16, 16)).
void PokeathlonCirclePushAi_PlaceInCells(u8 count, PokeathlonCirclePushAiCell **cells) {
    u8 i;

    for (i = 0; i < count; i++) {
        PokeathlonCirclePushAiCell *cell = cells[i];
        if (cell->inRing != NULL) {
            cell->pixelX = cell->x * 32 + 16 + LCRandom() % 32;
            cell = cells[i];
            cell->pixelY = cell->y * 32 + 16 + LCRandom() % 32;
            cell = cells[i];
            cell->cellIndex = cell->x + cell->y * 7;
        } else {
            cell->pixelX = -1;
            cells[i]->pixelY = -1;
            cells[i]->cellIndex = -1;
        }
    }
}

void PokeathlonCirclePushAi_ScoreCells(PokeathlonCirclePushAi *ai, u8 count, PokeathlonCirclePushAiZone *zones, PokeathlonCirclePushAiMon *mon, PokeathlonCirclePushAiCell **cells) {
    u8 i;

    for (i = 0; i < count; i++) {
        if (cells[i]->inRing == NULL) {
            cells[i]->score = -100;
        } else {
            BOOL inZone;
            cells[i]->score = PokeathlonCirclePushAi_ScoreCell(ai, zones, cells[i]->cellIndex, mon, cells[i]->pixelX, cells[i]->pixelY, &inZone);
            if (inZone != FALSE) {
                PokeathlonCirclePushAi_SnapToZone(ai, zones, (u8)cells[i]->cellIndex, &cells[i]->pixelX, &cells[i]->pixelY);
            }
        }
    }
}

int PokeathlonCirclePushAi_GetCell(int x, int y) {
    int column = (x - 16) / 32;
    int row = (y - 16) / 32;
    int cell;

    if (column < 0 || row < 0) {
        return -1;
    }
    cell = row * 7 + column;
    if (cell > 34) {
        cell = -1;
    }
    return cell;
}

#pragma force_active on

int PokeathlonCirclePushAi_ScoreCell(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, int cell, PokeathlonCirclePushAiMon *mon, int x, int y, BOOL *inZone) {
    int score = 0;
    if (cell < 0) {
        return score;
    }
    *inZone = FALSE;
    if (*mon->stamina != 0 && *mon->state == 1) {
        if (PokeathlonCirclePushAi_IsInZone(zones, x << FX32_SHIFT, y << FX32_SHIFT)) {
            score += 5;
        }
        if (PokeathlonCirclePushAi_IsZoneCell(ai, (u8)cell)) {
            score += 4;
            *inZone = TRUE;
        }
        score += PokeathlonCirclePushAi_CountRivals(ai->mons, mon, cell);
    } else {
        int rivals;
        if (PokeathlonCirclePushAi_IsInZone(zones, x << FX32_SHIFT, y << FX32_SHIFT)) {
            score += 5;
        }
        if (PokeathlonCirclePushAi_IsZoneCell(ai, (u8)cell)) {
            score += 4;
            *inZone = TRUE;
        }
        rivals = PokeathlonCirclePushAi_CountRivals(ai->mons, mon, cell);
        if (rivals == 0) {
            score += 2;
        }
        if (mon->cell == cell) {
            score += 2;
        }
        if (rivals != 0) {
            score = 0;
        }
        if (PokeathlonCirclePushAi_IsInZone(zones, mon->position->x, mon->position->y)) {
            score += 4;
        }
    }
    return score;
}

#pragma force_active reset

#pragma opt_loop_invariants off

// Minus one for every other Pokemon in the cell, two if it is a faster rival.
#pragma force_active on

int PokeathlonCirclePushAi_CountRivals(const PokeathlonCirclePushAiMon *mons, const PokeathlonCirclePushAiMon *mon, int cell) {
    int result;
    int i;
    int slot;
    if (mon->cell == -1) {
        return 0;
    }
    slot = mon->slot;
    result = 0;
    for (i = 0; i < 12; i++, mons++) {
        if (i != slot && cell == mons->cell) {
            if (mon->speed < mons->speed) {
                if (slot / 3 == mons->slot / 3) {
                    result--;
                } else {
                    result -= 2;
                }
            } else {
                result--;
            }
        }
    }
    return result;
}

#pragma force_active reset

#pragma opt_loop_invariants reset

BOOL PokeathlonCirclePushAi_IsInZone(PokeathlonCirclePushAiZone *zones, fx32 x, fx32 y) {
    VecFx32 point;
    VecFx32 delta;
    s32 i;

    point.x = x;
    point.y = y;
    point.z = 0;
    for (i = 0; i < 4; i++, zones++) {
        if (zones->radius <= 0) {
            break;
        }
        delta.x = zones->x << FX32_SHIFT;
        delta.y = zones->y << FX32_SHIFT;
        delta.z = 0;
        VEC_Subtract(&point, &delta, &delta);
        if (VEC_Mag(&delta) < zones->radius << FX32_SHIFT) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PokeathlonCirclePushAi_IsZoneCell(PokeathlonCirclePushAi *ai, u8 cell) {
    s32 i;
    for (i = 0; i < 4; i++) {
        s32 zoneCell = ai->zoneCells[i];
        if (zoneCell == -1) {
            break;
        }
        if (cell == zoneCell) {
            return TRUE;
        }
    }
    return FALSE;
}

void PokeathlonCirclePushAi_SnapToZone(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, s32 cell, s32 *x, s32 *y) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (cell == ai->zoneCells[i]) {
            *x = zones[i].x;
            *y = zones[i].y;
            return;
        }
    }
}

BOOL PokeathlonCirclePushAi_GetColumnRow(s32 x, s32 y, s16 *column, s16 *row) {
    *column = (x - 16) / 32;
    *row = (y - 16) / 32;
    if (*column < 0 || *row < 0) {
        return FALSE;
    }
    return TRUE;
}

typedef struct CirclePushSortNode {
    struct CirclePushSortNode *prev;
    struct CirclePushSortNode *next;
    PokeathlonCirclePushAiCell *cell;
} CirclePushSortNode;

// Sorts the cells by score (insertion into a list); returns how many share the best score.
#pragma force_active on

u8 PokeathlonCirclePushAi_SortCells(u32 count, PokeathlonCirclePushAiCell **cells, PokeathlonCirclePushAiCell **sorted) {
    u8 best;
    CirclePushSortNode nodes[26];
    u8 i;
    u8 j;
    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    best = 0;
    nodes[0].cell = NULL;
    for (i = 0; i < count; i++) {
        CirclePushSortNode *node = &nodes[i + 1];
        CirclePushSortNode *pos;
        node->cell = cells[i];
        node->next = NULL;
        node->prev = NULL;
        pos = &nodes[0];
        for (j = 0; j < i; j++) {
            pos = pos->next;
            if (pos->cell->score < node->cell->score) {
                pos->prev->next = node;
                node->prev = pos->prev;
                node->next = pos;
                pos->prev = node;
                break;
            }
        }
        if (j == i) {
            pos->next = node;
            node->prev = pos;
        }
    }
    {
        CirclePushSortNode *pos = nodes[0].next;
        int bestScore = pos->cell->score;
        pos = &nodes[0];
        for (i = 0; i < count; i++) {
            pos = pos->next;
            sorted[i] = pos->cell;
            if (bestScore == sorted[i]->score) {
                best++;
            }
        }
    }
    if (best == 0) {
        GF_AssertFail();
    }
    return best;
}

#pragma force_active reset

PokeathlonCirclePushDisplay *PokeathlonCirclePushDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonCirclePushDisplay *display = Heap_Alloc(heapId, sizeof(PokeathlonCirclePushDisplay));
    MI_CpuFill8(display, 0, sizeof(PokeathlonCirclePushDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonCirclePushDisplay_CreateWindows(display);
    display->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    display->msgFormat = MessageFormat_New(heapId);
    return display;
}

// Walks through the arrays by moving a view of the display one element on, as the original did.
void PokeathlonCirclePushDisplay_Free(PokeathlonCirclePushDisplay *display) {
    s32 i = 0;
    PokeathlonCirclePushDisplay *view = display;
    for (; i < 12; i++, view = (PokeathlonCirclePushDisplay *)((u8 *)view + sizeof(PokeathlonCirclePushMonPic))) {
        Heap_Free(view->anim.monPics[0].charData);
        Heap_Free(view->anim.monPics[0].paletteData);
    }
    {
        PokeathlonCirclePushDisplay *fileView;
        s32 j;
        j = 0;
        fileView = display;
        for (; j < 4; j++, fileView = (PokeathlonCirclePushDisplay *)((u8 *)fileView + sizeof(void *))) {
            Heap_Free(fileView->anim.screenFiles[0]);
        }
        Heap_Free(display->anim.screenFiles[4]);
        {
            PokeathlonCirclePushDisplay *lastView;
            s32 k;
            k = 0;
            lastView = display;
            for (; k < 2; k++, lastView = (PokeathlonCirclePushDisplay *)((u8 *)lastView + sizeof(void *))) {
                Heap_Free(lastView->anim.screenFiles[5]);
            }
            MessageFormat_Delete(display->msgFormat);
            DestroyMsgData(display->msgData);
            {
                Window *window;
                s32 n = 0;
                window = display->windows;
                for (; n < 5; n++, window++) {
                    RemoveWindow(window);
                }
            }
        }
    }
    Heap_Free(display);
}

// Bottom screen frame (sub BG 1) and the screens of the top screen's animations.
#pragma force_active on

void PokeathlonCirclePushDisplay_LoadScreens(PokeathlonCirclePushDisplay *display) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 13, FALSE, &screen, display->heapId);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_1, screen->rawData, screen->szByte);
    BgTilemapRectChangePalette(display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 32, 24, 2);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(file);
    display->anim.screenFiles[5] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 23, FALSE, &display->anim.zoomBg3Screen, display->heapId);
    display->anim.screenFiles[6] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 24, FALSE, &display->anim.matchupBg3Screen, display->heapId);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_3, display->anim.zoomBg3Screen->rawData, display->anim.zoomBg3Screen->szByte);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    display->anim.screenFiles[0] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 18, FALSE, &display->anim.scoreboardScreen, display->heapId);
    display->anim.screenFiles[1] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 19, FALSE, &display->anim.matchupBg2Screen, display->heapId);
    display->anim.screenFiles[2] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 17, FALSE, &display->anim.zoomBg2Screen, display->heapId);
    display->anim.screenFiles[3] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 20, FALSE, &display->anim.monPicScreen, display->heapId);
    display->anim.screenFiles[4] = GfGfxLoader_GetScrnData(NARC_a_1_6_7, 16, FALSE, &display->anim.digitsScreen, display->heapId);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.scoreboardScreen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.matchupBg2Screen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.monPicScreen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.zoomBg2Screen);
    PokeathlonCirclePushDisplay_OffsetScreenTiles(128, display->anim.digitsScreen);
    BG_LoadScreenTilemapData(display->bgConfig, GF_BG_LYR_SUB_2, display->anim.scoreboardScreen->rawData, display->anim.scoreboardScreen->szByte);
    PokeathlonCirclePushDisplayAnim_PrintScore(&display->anim, display->bgConfig);
    BgTilemapRectChangePalette(display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, 3);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_2);
}

#pragma force_active reset

void PokeathlonCirclePushDisplay_LoadPics(PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushDisplay_LoadMonPics(display);
}

// Resources of the two picture sprites (NARC 93).
void PokeathlonCirclePushDisplay_LoadSpriteResources(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display) {
    u32 resIds[2];
    u32 *resId = resIds;
    int i;
    resIds[0] = gPokeathlonCirclePushNameplateCoords[2];
    resIds[1] = gPokeathlonCirclePushNameplateCoords[3];
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_LoadChar(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000009_NCGR, *resId, 2);
        PokeathlonCourseSpritePool_LoadPltt(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000006_NCLR, *resId, 2, 1);
        PokeathlonCourseSpritePool_LoadCell(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, *resId);
        PokeathlonCourseSpritePool_LoadAnim(pool, NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, *resId);
        resId++;
    }
}
