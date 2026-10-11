#include "pokeathlon/pokeathlon_goal_roll.h"

#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_0203A3B0.h"

// Course helpers, sprite scene and object helpers (OVY_96, not yet tidied).
void *ov96_021E60C0(PokeathlonCourseData *course, int participant, int member);
int ov96_021E6104(void);
int ov96_021E6108(const void *mon);
int ov96_021E6138(const void *mon);
void ov96_021E6168(PokeathlonCourseData *course, int participant, int member, void *view);
Sprite **ov96_021E6290(PokeathlonCourseData *course, int a1, void *resources, void *pool);
void ov96_021E634C(PokeathlonCourseData *course, int a1, void *resources, void *pool, u32 a4, u8 a5, const u16 (*coords)[2]);
BOOL ov96_021E637C(PokeathlonCourseData *course);
void ov96_021E64B8(PokeathlonCourseData *course);
Sprite *ov96_021E64F8(PokeathlonCourseData *course, void *object, void *resources, SpriteList *spriteList, int a4);
void ov96_021E6550(PokeathlonCourseData *course);
void ov96_021E65A4(PokeathlonCourseData *course);
void ov96_021E6670(PokeathlonCourseData *course, u32 a1);
BOOL ov96_021E667C(PokeathlonCourseData *course);
void ov96_021E8228(PokeathlonCourseData *course, u8 participant, u8 member, int event, int value);
void ov96_021E8324(PokeathlonCourseData *course, void (*callback)(PokeathlonCourseData *));
u8 *ov96_021E8A20(void *ptr);
void ov96_021E8BAC(void *object);
u8 ov96_021E90FC(u8 *image);
void ov96_021E92B0(ObjCharTransferTemplate *config, int a1, enum HeapID heapId, GXOBJVRamModeChar a3, GXOBJVRamModeChar a4);
void ov96_021E93B4(void *gfx, int a1);
void ov96_021E9510(void *gfx);
void ov96_021E952C(void *gfx);
void ov96_021E9570(void *gfx, int a1);
void ov96_021E95F8(PokeathlonCourseData *course, void *gfx, int participant, u16 score, int a4, int a5, int a6);
PokeathlonGoalRollResources *ov96_021E9A78(int heapId, int a1, int a2);
void ov96_021E9C0C(PokeathlonGoalRollResources *resources);
PokeathlonGoalRollMonObjects *ov96_021EA854(enum HeapID heapId, int count, int a2, PokeathlonGoalRollResources *resources, SpriteList *spriteList);
void ov96_021EA894(PokeathlonGoalRollMonObjects *monObjects);
void ov96_021EA8A8(PokeathlonGoalRollMonObjects *monObjects, int count, void *views, void *info, int a4, int a5);
BOOL ov96_021EAA00(PokeathlonGoalRollMonObjects *monObjects);
PokeathlonGoalRollMonObject *ov96_021EAA04(void *monObjects, u8 index);
u8 *ov96_021EAA20(void *object);
void ov96_021EAB38(u8 *object, int a1);
void ov96_021EABA8(void *object, int priority);
void ov96_021EABDC(u8 *object, u32 a1);
void ov96_021EAC0C(void *object, u8 anim);
void ov96_021EAC5C(void *object, int anim);
void ov96_021EAF6C(u8 *object, u32 a1);
void ov96_021EAF70(u8 *object, u32 width, u32 height);
s32 ov96_021EAF8C(void *object);
void ov96_021EAF94(void *object, s32 x, s32 y);
void ov96_021EB01C(void *object, int x, int y, int a3);
void ov96_021EB06C(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void ov96_021EB0A4(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void ov96_021EB144(PokeathlonGoalRollMonObjects *monObjects, int a1);
PokeathlonGoalRollSpritePool *ov96_021EB180(enum HeapID heapId, const PokeathlonGoalRollSpritePoolConfig *config);
void ov96_021EB21C(PokeathlonGoalRollSpritePool *pool);
void ov96_021EB29C(PokeathlonGoalRollSpritePool *pool, int a1, int a2);
void ov96_021EB2BC(PokeathlonGoalRollSpritePool *pool, NarcId narcId, int member, int a3, int a4);
void ov96_021EB2F4(PokeathlonGoalRollSpritePool *pool, NarcId narcId, int member, int a3, int a4, u8 a5);
void ov96_021EB334(PokeathlonGoalRollSpritePool *pool, NarcId narcId, int member, int a3);
void ov96_021EB36C(PokeathlonGoalRollSpritePool *pool, NarcId narcId, int member, int a3);
void ov96_021EB3A4(PokeathlonGoalRollSpritePool *pool);
PokeathlonGoalRollSprite *ov96_021EB3E4(PokeathlonGoalRollSpritePool *pool, int a1, int a2, int a3, u8 a4);
void ov96_021EB52C(PokeathlonGoalRollSprite *sprite, BOOL a1, BOOL show);
void ov96_021EB564(PokeathlonGoalRollSprite *sprite, int anim);
void ov96_021EB570(PokeathlonGoalRollSprite *sprite, int anim);
BOOL ov96_021EB57C(PokeathlonGoalRollSprite *sprite);
void ov96_021EB588(PokeathlonGoalRollSprite *sprite, const VecFx32 *position);
void ov96_021EB5AC(PokeathlonGoalRollSprite *sprite, u16 palette, int a2);
void ov96_021EB5BC(PokeathlonGoalRollSpritePool *pool);
void ov96_021EB5C8(PokeathlonGoalRollSpritePool *pool, int a1, int a2, int a3, int a4);
SpriteList *ov96_021EB5E8(void *pool);
void ov96_021EB630(PokeathlonGoalRollSprite *sprite, int priority);
void ov96_021EB63C(PokeathlonGoalRollSpritePool *pool);
void ov96_0221A56C(void *gfx, int a1);

extern const VecFx32 ov96_0221D208;
extern const VecFx32 ov96_0221D220;
extern const PokeathlonGoalRollSpritePoolConfig ov96_0221D238;
extern const VecFx32 ov96_0221D250;
extern const GraphicsModes ov96_0221D28C;
extern const BgTemplate ov96_0221D29C;
extern const BgTemplate ov96_0221D2B8;
extern const BgTemplate ov96_0221D2D4;
extern const BgTemplate ov96_0221D2F0;
extern const BgTemplate ov96_0221D30C;
extern const BgTemplate ov96_0221D328;
extern const BgTemplate ov96_0221D344;
extern const GraphicsBanks ov96_0221D380;
extern const VecFx32 ov96_0221D3A8[4];
extern const VecFx32 ov96_0221D3D8[4];
extern const u16 ov96_0221D408[GOAL_ROLL_PARTICIPANTS][GOAL_ROLL_TEAM_SIZE][2];
extern const VecFx32 ov96_0221D4B4[4][2];

extern const PokeathlonGoalRollPaletteData ov96_0221D1F4;

// A Pokemon as ov96_021E6168 describes it to the 3D object list.
typedef struct PokeathlonGoalRollMonView {
    u16 species;
    u16 unk02;
    u16 unk04;
    u8 kind;
    u8 unk07;
    u32 participant;
    u32 unk0C;
} PokeathlonGoalRollMonView;

typedef struct PokeathlonGoalRollMonObjectsInfo {
    u32 unk00[5];
    int sizes[GOAL_ROLL_MONS];
} PokeathlonGoalRollMonObjectsInfo;

typedef struct PokeathlonGoalRollModelSize {
    u32 width;
    u32 height;
} PokeathlonGoalRollModelSize;

typedef struct PokeathlonGoalRollVec4 {
    VecFx32 v[4];
} PokeathlonGoalRollVec4;

// Frame locals of PokeathlonGoalRoll_Init.
typedef struct PokeathlonGoalRollInitWork {
    u16 ownCoords[GOAL_ROLL_TEAM_SIZE][2];
    VecFx32 goalPositions[GOAL_ROLL_PARTICIPANTS];
    VecFx32 ballPosition;
    PokeathlonGoalRollSpritePoolConfig poolConfig;
    ObjCharTransferTemplate charTransfer;
    PokeathlonGoalRollMonParams monParams; // NARC a/1/7/0 member 8
    PokeathlonGoalRollMonObjectsInfo objectsInfo;
    PokeathlonGoalRollMonView views[GOAL_ROLL_MONS];
} PokeathlonGoalRollInitWork;

// The goal sprites are set up through one cursor that moves by one pointer per goal, so that
// goal->goalSprites[0], goal->goalFlashSprites[0] and goal->goalLabelSprites[0] address goal i.
#define GOAL_CURSOR_NEXT(cursor) ((PokeathlonGoalRoll *)((u32 *)(cursor) + 1))

static inline void PokeathlonGoalRoll_SetModelSize(PokeathlonCourseData *course, PokeathlonGoalRollMonObjects *monObjects, s32 index, const PokeathlonGoalRollModelSize *sizes) {
    PokeathlonGoalRollMonObject *object;
    int participant;
    int member;
    int size;
    object = ov96_021EAA04((u8 *)monObjects, (u8)index);
    ov96_021EAB38((u8 *)object, 1);
    participant = index / 3;
    member = index % 3;
    size = ov96_021E6138(ov96_021E60C0(course, participant, member));
    ov96_021EAF70((u8 *)object, sizes[size - 1].width, sizes[size - 1].height);
}

// PokeathlonGoalRollCpu_New as this call was built: with an int count and the course mode as an
// extra (unused) argument.
typedef PokeathlonGoalRollCpu *(*PokeathlonGoalRollCpuNewFunc)(u32 heapId, int cpuCount, int mode);

#pragma force_active on

BOOL PokeathlonGoalRoll_Init(PokeathlonCourseData *course) {
    PokeathlonGoalRollModelSize modelSizes[3];
    PokeathlonGoalRollSprite *sprite;
    int j;
    u8 *image;
    int member;
    int i;
    u16 goalPalettes[4];

    struct {
        int y;
        int x;
    } screen;

    struct {
        VecFx32 post;
        VecFx32 corner;
        VecFx32 normal;
    } wall;

    int own;
    int x;
    u32 y;
    int participant;
    PokeathlonGoalRollMon(*team)[GOAL_ROLL_TEAM_SIZE];
    PokeathlonGoalRollInitWork work;
    PokeathlonGoalRoll *goal;
    const VecFx32 *goalPosition;
    s32 flip;
    u16 *goalPalette;
    int mode;
    u32 markAnim;
    const VecFx32 *markPosition;
    const VecFx32 *labelPosition;
    int row;
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        u8 participant;
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_147, 0x50000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        PokeathlonGoalRoll_SetGfxBanks();
        goalRoll = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonGoalRoll));
        MI_CpuFill8(goalRoll, 0, sizeof(PokeathlonGoalRoll));
        goalRoll->sortWork = Heap_Alloc(HEAP_ID_147, 40);
        MI_CpuFill8(goalRoll->sortWork, 0, 40);
        goalRoll->bgConfig = BgConfig_Alloc(HEAP_ID_147);
        ov96_021E6670(course, 8);
        work.charTransfer.maxTasks = 133;
        work.charTransfer.sizeMain = 0x40000;
        work.charTransfer.sizeSub = 0x4000;
        work.charTransfer.heapID = HEAP_ID_147;
        ov96_021E92B0(&work.charTransfer, 16, HEAP_ID_147, (GXOBJVRamModeChar)0x300010, (GXOBJVRamModeChar)16);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 1, 126, 0, 32, HEAP_ID_147);
        goalRoll->heapId = HEAP_ID_147;
        FontID_Alloc((FontID)4, HEAP_ID_147);
        PokeathlonGoalRoll_InitBgs(goalRoll->bgConfig);
        PokeathlonGoalRoll_InitWindows(goalRoll);
        PokeathlonGoalRoll_LoadStrings(course);
        for (participant = 0; participant < 4; participant++) {
            PokeathlonGoalRoll_PrintPlate(goalRoll, participant, 0);
        }
        PokeathlonGoalRoll_CreatePlateTasks(goalRoll);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1: {
        int count;
        goalRoll->display = PokeathlonGoalRollDisplay_New(goalRoll->heapId, goalRoll->bgConfig, course);
        count = PokeathlonCourse_GetParticipantCount(course);
        mode = PokeathlonCourse_GetMode(course);
        goalRoll->cpu = ((PokeathlonGoalRollCpuNewFunc)PokeathlonGoalRollCpu_New)((u32)goalRoll->heapId, 4 - count, mode);
        goalRoll->resources = ov96_021E9A78(goalRoll->heapId, 2735, 1);
        ov96_021E64B8(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 2: {
        work.poolConfig = ov96_0221D238;
        goalRoll->spritePool = ov96_021EB180(goalRoll->heapId, &work.poolConfig);
        ov96_021EB5C8(goalRoll->spritePool, 0, 0, 0, 0x200000);
        goalRoll->monObjects = ov96_021EA854(goalRoll->heapId, 12, 8, goalRoll->resources, ov96_021EB5E8((u8 *)goalRoll->spritePool));
        ov96_021EB29C(goalRoll->spritePool, 0, 101);
        ov96_021EB29C(goalRoll->spritePool, 1, 102);
        ov96_021EB2BC(goalRoll->spritePool, NARC_a_2_3_4, 18, 101, 1);
        ov96_021EB2F4(goalRoll->spritePool, NARC_a_2_3_4, 15, 101, 1, 2);
        ov96_021EB334(goalRoll->spritePool, NARC_a_2_3_4, 17, 101);
        ov96_021EB36C(goalRoll->spritePool, NARC_a_2_3_4, 16, 101);
        ov96_021EB2BC(goalRoll->spritePool, NARC_a_2_3_4, 14, 102, 2);
        ov96_021EB2F4(goalRoll->spritePool, NARC_a_2_3_4, 11, 102, 2, 1);
        ov96_021EB334(goalRoll->spritePool, NARC_a_2_3_4, 13, 102);
        ov96_021EB36C(goalRoll->spritePool, NARC_a_2_3_4, 12, 102);
        ov96_021EB3A4(goalRoll->spritePool);
        for (i = 0; i < 12; i++) {
            goalRoll->mons[i / 3][i % 3].dashSprite = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 2);
            ov96_021EB630(goalRoll->mons[i / 3][i % 3].dashSprite, 5);
            for (j = 0; j < 3; j++) {
                goalRoll->mons[i / 3][i % 3].trailSprites[j] = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 7);
                ov96_021EB630(goalRoll->mons[i / 3][i % 3].trailSprites[j], 47);
            }
        }
        for (i = 0; i < 2; i++) {
            PokeathlonGoalRollBall *ball = &goalRoll->balls[i];
            memset(&work.ballPosition, 0, sizeof(VecFx32));
            ball->sprite = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 3);
            ball->effectSprite = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 8);
            ov96_021EB564(ball->sprite, 10);
            ov96_021EB564(ball->effectSprite, 13);
            ov96_021EB630(ball->sprite, 46);
            ov96_021EB630(ball->effectSprite, 45);
            PokeathlonGoalRoll_ResetBall(ball);
            ball->state = GOAL_ROLL_BALL_ROLLING;
            work.ballPosition.x = 0x80000;
            work.ballPosition.y = 0x68000;
            ov96_021EB588(ball->sprite, &work.ballPosition);
            ov96_021EB588(ball->effectSprite, &work.ballPosition);
            ball->position = work.ballPosition;
            ball->spawnPoint = 7;
            if (i == 0) {
                ov96_021EB52C(ball->sprite, TRUE, TRUE);
                ball->kind = GOAL_ROLL_BALL_SMALL;
            }
        }
        goalPalettes[0] = ov96_0221D1F4.data.goalPalettes[0];
        goalPalettes[1] = ov96_0221D1F4.data.goalPalettes[1];
        goalPalettes[2] = ov96_0221D1F4.data.goalPalettes[2];
        goalPalettes[3] = ov96_0221D1F4.data.goalPalettes[3];
        *(PokeathlonGoalRollVec4 *)work.goalPositions = *(const PokeathlonGoalRollVec4 *)ov96_0221D3D8;
        {
            int n;
            for (n = 0, goal = goalRoll, goalPosition = work.goalPositions, goalPalette = goalPalettes, markAnim = 2, markPosition = ov96_0221D4B4[0], labelPosition = ov96_0221D3A8; n < 4; goal = GOAL_CURSOR_NEXT(goal), goalPosition++, goalPalette++, markAnim += 2, markPosition += 2, labelPosition++, n++) {
                goal->goalSprites[0] = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 5);
                ov96_021EB52C(goal->goalSprites[0], TRUE, TRUE);
                ov96_021EB564(goal->goalSprites[0], 1);
                ov96_021EB588(goal->goalSprites[0], goalPosition);
                ov96_021EB5AC(goal->goalSprites[0], *goalPalette, 1);
                ov96_021EB630(goal->goalSprites[0], 3);
                for (j = 0; j < 2; j++) {
                    sprite = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 6);
                    ov96_021EB52C(sprite, TRUE, TRUE);
                    ov96_021EB564(sprite, j + markAnim);
                    ov96_021EB588(sprite, markPosition + j);
                    ov96_021EB630(sprite, 103);
                }
                goal->goalFlashSprites[0] = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 11);
                ov96_021EB588(goal->goalFlashSprites[0], labelPosition);
                ov96_021EB630(goal->goalFlashSprites[0], 2);
                goal->goalLabelSprites[0] = ov96_021EB3E4(goalRoll->spritePool, 1, 1, 101, 12);
                ov96_021EB588(goal->goalLabelSprites[0], goalPosition);
                ov96_021EB630(goal->goalLabelSprites[0], 102);
            }
        }
        Sprite_SetDrawPriority(*ov96_021E6290(course, 0, goalRoll->resources, goalRoll->spritePool), 1);
        PokeathlonGoalRollDisplay_CreateSprites(goalRoll->spritePool, goalRoll->display, goalRoll->resources);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3: {
        int col;
        u32 participantIndex;
        PokeathlonGoalRollMonObjectsInfo *info;
        info = &work.objectsInfo;
        for (i = 0; i < 12; i++) {
            col = i % 3;
            participantIndex = i / 3;
            ov96_021E6168(course, participantIndex, col, &work.views[i]);
            info->sizes[i] = ov96_021E6108(ov96_021E60C0(course, participantIndex, col));
        }
        work.objectsInfo.unk00[0] = 0;
        work.objectsInfo.unk00[1] = 1;
        work.objectsInfo.unk00[2] = 0;
        work.objectsInfo.unk00[3] = 1;
        work.objectsInfo.unk00[4] = 1;
        ov96_021EA8A8(goalRoll->monObjects, 12, work.views, &work.objectsInfo, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 4: {
        PokeathlonGoalRollMonObject *object;
        PokeathlonGoalRollMon *mon;
        if (!ov96_021EAA00(goalRoll->monObjects)) {
            break;
        }
        own = ov96_021E5F24(course);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)goalRoll->bgConfig);
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(modelSizes, NARC_a_1_7_0, 11);
        for (i = 0; i < 12; i++) {
            PokeathlonGoalRoll_SetModelSize(course, goalRoll->monObjects, i, modelSizes);
            object = ov96_021EAA04((u8 *)goalRoll->monObjects, (u8)i);
            image = ov96_021EAA20(&object->object);
            member = i % 3;
            row = i / 3;
            mon = &goalRoll->mons[row][member];
            mon->object = object;
            mon->radius = ov96_021EAF8C(object);
            mon->mode = GOAL_ROLL_MODE_WALK;
            ov96_021E90FC(image);
            x = ov96_0221D408[row][member][0];
            y = ov96_0221D408[row][member][1];
            mon->facing = 2;
            ov96_021EAC0C(object, 2);
            ov96_021EAF94(object, x, y);
            ov96_021EAF6C((u8 *)object, ov96_021E6104());
            ov96_021EB0A4(object, x, y, &screen.x, &screen.y);
            mon->startX = (s32)((u32)screen.x << 12);
            mon->startY = (s32)((u32)screen.y << 12);
            mon->position.x = (s32)((u32)screen.x << 12);
            mon->position.y = (s32)((u32)screen.y << 12);
            mon->target.x = (s32)((u32)screen.x << 12);
            mon->target.y = (s32)((u32)screen.y << 12);
            if (row == ov96_021E5F24(course)) {
                ov96_021EABA8(object, 8);
            } else {
                ov96_021EABA8(object, 16);
            }
            ov96_021EABDC((u8 *)object, 20);
            if (row == own) {
                work.ownCoords[member][0] = screen.x;
                work.ownCoords[member][1] = screen.y;
                ov96_021E64F8(course, object, goalRoll->resources, ov96_021EB5E8((u8 *)goalRoll->spritePool), 1);
            }
        }
        ov96_021E634C(course, 0, goalRoll->resources, goalRoll->spritePool, 1, 3, work.ownCoords);
        if (ov96_021E5F24(course) == 0) {
            ReadWholeNarcMemberByIdPair(&work.monParams, NARC_a_1_7_0, 8);
            for (participant = 0, team = goalRoll->mons; participant < 4; team++, participant++) {
                goalRoll->heldMon[participant] = GOAL_ROLL_NO_MON;
                for (j = 0; j < 3; j++) {
                    PokeathlonGoalRollMon *mon = &(*team)[j];
                    PokeathlonGoalRoll_InitMonStats(course, &work.monParams, (u8)participant, (u8)j, mon);
                    PokeathlonGoalRollCpu_SetMon(goalRoll->cpu->teams, (u8)(j + participant * 3), mon);
                }
            }
            PokeathlonGoalRollCpu_SetBalls(goalRoll->cpu, goalRoll->balls);
            PokeathlonGoalRollCpu_ChooseAllRoles(goalRoll->cpu->teams);
            flip = -1;
            wall.normal = ov96_0221D220;
            VEC_Normalize(&wall.normal, &wall.normal);
            wall.corner = ov96_0221D250;
            goalRoll->walls[0] = wall.corner;
            goalRoll->walls[1] = wall.corner;
            goalRoll->walls[1].x *= flip;
            wall.post = ov96_0221D208;
            goalRoll->walls[2] = wall.post;
            goalRoll->walls[3] = wall.post;
            goalRoll->walls[3].y *= flip;
            goalRoll->walls[4] = wall.normal;
            goalRoll->walls[5] = wall.normal;
            goalRoll->walls[5].x *= flip;
            goalRoll->walls[6] = wall.normal;
            goalRoll->walls[6].y *= flip;
            goalRoll->walls[7] = wall.normal;
            goalRoll->walls[7].x *= flip;
            goalRoll->walls[7].y *= flip;
        }
        if (ov96_021E5F24(course) == 0) {
            PokeathlonGoalRoll_PackFrame(goalRoll, (PokeathlonGoalRollFramePacket *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 40));
        }
        PokeathlonGoalRoll_LoadBgGfx(goalRoll);
        PokeathlonGoalRollDisplay_PrintName(goalRoll->display);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        goalRoll->timer = 2700;
        PokeathlonGoalRollDisplay_UpdateTimer(goalRoll->display, goalRoll->timer);
        sub_0203A994(2);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, goalRoll->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonGoalRoll_Draw(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    ov96_021EB5BC(goalRoll->spritePool);
    return TRUE;
}

BOOL ov96_02211660(PokeathlonCourseData *course) {
    void *gfx;
    int i;

    PokeathlonCourse_GetHeapAllocPtr4(course);
    gfx = PokeathlonCourse_GetGraphicsSystem(course);
    ov96_021E9510(gfx);
    for (i = 0; i < 4; i++) {
        ov96_021E95F8(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 300);
    }
    ov96_021E93B4(gfx, 1);
    ov96_0221A56C(gfx, 9);
    ov96_021E952C(gfx);
    ov96_021E9570(gfx, 9);
    return TRUE;
}

BOOL PokeathlonGoalRoll_Exit(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    Window *window;
    int i;

    WirelessStatusIcon_Destroy();
    ov96_021E6550(course);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(goalRoll->bgConfig, GF_BG_LYR_SUB_2);
    PokeathlonGoalRoll_DestroyPlateTasks(goalRoll);
    PokeathlonGoalRoll_FreeStrings(course);
    RemoveWindow(&goalRoll->scoreWindow);
    // the cursor starts at the work and is moved to the name windows after `i` is set
    window = (Window *)goalRoll;
    i = 0;
    window = (Window *)((u8 *)window + offsetof(PokeathlonGoalRoll, nameWindows));
    while (i < 4) {
        RemoveWindow(window);
        i++;
        window++;
    }
    Heap_Free(goalRoll->bgConfig);
    ov96_021EB21C(goalRoll->spritePool);
    ov96_021EA894(goalRoll->monObjects);
    ov96_021E9C0C(goalRoll->resources);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    PokeathlonGoalRollCpu_Free(goalRoll->cpu);
    PokeathlonGoalRollDisplay_Free(goalRoll->display);
    FontID_Release((FontID)4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    Heap_Free(goalRoll->sortWork);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    G2_BlendNone();
    Heap_Destroy(HEAP_ID_147);
    return TRUE;
}

void PokeathlonGoalRoll_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221D380;
    GfGfx_SetBanks(&banks);
}

BOOL PokeathlonGoalRoll_State_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        if (ov96_021E637C(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonGoalRoll_HostUpdate(course);
        if (PokeathlonGoalRoll_HandleInput(course)) {
            (*state)++;
        }
        break;
    case 2:
        PokeathlonGoalRoll_HostUpdate(course);
        PokeathlonGoalRoll_ApplyFrameScore(course);
        if (ov96_021E667C(course)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, goalRoll->heapId);
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    }
    return FALSE;
}

BOOL PokeathlonGoalRoll_State_Finish(PokeathlonCourseData *course, u8 *state) {
    int i;
    PokeathlonCourseData *savedCourse = course;
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);

    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        return FALSE;
    }
    if (ov96_021E5F24(savedCourse)) {
        return TRUE;
    }
    // the cursor walks the scores
    for (i = 0; i < 4; i++) {
        PokeathlonCourse_SetField5E0_AtIndex(savedCourse, (u8)i, (u16)goalRoll->scores[0]);
        goalRoll = GOAL_CURSOR_NEXT(goalRoll);
    }
    return TRUE;
}

void PokeathlonGoalRoll_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0;
    BgTemplate main1;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;

    modes = ov96_0221D28C;
    SetBothScreensModesAndDisable(&modes);
    main0 = ov96_0221D30C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = ov96_0221D328;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main2 = ov96_0221D344;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = ov96_0221D29C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = ov96_0221D2B8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = ov96_0221D2D4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = ov96_0221D2F0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonGoalRoll_LoadBgGfx(PokeathlonGoalRoll *goalRoll) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 1, goalRoll->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, goalRoll->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 3, goalRoll->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, goalRoll->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 6, goalRoll->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, goalRoll->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_4, 7, goalRoll->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, goalRoll->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 2, goalRoll->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, goalRoll->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 4, goalRoll->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, goalRoll->heapId);
    PokeathlonGoalRollDisplay_LoadBgGfx(goalRoll->display);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, goalRoll->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, goalRoll->heapId);
}

// Every participant: reads the received frame; TRUE once the host reports the time is up.
BOOL PokeathlonGoalRoll_HandleInput(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *area = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonGoalRollInput *input = (PokeathlonGoalRollInput *)ov96_021E8A20(area);
    PokeathlonGoalRollFramePacket *frame;

    input->touching = 0;
    frame = (PokeathlonGoalRollFramePacket *)ov96_021E8A20(area + 0xF0);
    u8 timeUp = 1;
    timeUp &= frame->flags >> 24;
    if (timeUp) {
        ov96_021EB63C(goalRoll->spritePool);
        ov96_021EB144(goalRoll->monObjects, 1);
        ov96_021E65A4(course);
        ov96_021E8324(course, PokeathlonGoalRoll_OnTimeUp);
        return TRUE;
    }
    if (System_GetTouchNew()) {
        ov96_021E8228(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        input->x = (u8)gSystem.touchX;
        input->y = (u8)gSystem.touchY;
        input->touching = 1;
    }
    PokeathlonGoalRoll_ApplyFrame(course);
    return FALSE;
}

#pragma force_active on

// The host: applies all participants' inputs, runs the field and sends the frame.
void PokeathlonGoalRoll_HostUpdate(PokeathlonCourseData *course) {
    struct {
        PokeathlonCourseData *course;
        PokeathlonGoalRollMon *team;
    } frame;

    PokeathlonGoalRollFramePacket *out;
    u8 *area;
    PokeathlonGoalRoll *goalRoll;
    PokeathlonGoalRoll *touchCursor;
    PokeathlonGoalRollInput *input;
    PokeathlonGoalRollFramePacket *ownInput;
    int i;
    int monIndex;
    u8 member;

    frame.course = course;
    area = PokeathlonCourse_GetDataCopyArea(frame.course);
    goalRoll = PokeathlonCourse_GetHeapAllocPtr4(frame.course);
    if (goalRoll->timer > 0) {
        goalRoll->timer--;
    }
    if (ov96_021E5F24(frame.course)) {
        return;
    }
    if (goalRoll->timeUp) {
        area += 40;
        PokeathlonGoalRoll_PackFrame(goalRoll, (PokeathlonGoalRollFramePacket *)ov96_021E8A20(area));
        return;
    }
    out = (PokeathlonGoalRollFramePacket *)ov96_021E8A20(area + 40);
    ownInput = (PokeathlonGoalRollFramePacket *)ov96_021E8A20(area + 80);
    *ownInput = *(PokeathlonGoalRollFramePacket *)ov96_021E8A20(area);
    area += 80;
    frame.team = goalRoll->mons[0];
    for (i = 0, touchCursor = goalRoll; i < 4; i++, touchCursor = (PokeathlonGoalRoll *)((u8 *)touchCursor + sizeof(PokeathlonGoalRollTouch)), area += 40, frame.team += GOAL_ROLL_TEAM_SIZE) {
        input = (PokeathlonGoalRollInput *)ov96_021E8A20(area);
        if (input->touching) {
            if (touchCursor->touch[0].newPress && touchCursor->touch[0].held) {
                touchCursor->touch[0].newPress = FALSE;
            } else if (touchCursor->touch[0].newPress == FALSE && touchCursor->touch[0].held == FALSE) {
                touchCursor->touch[0].newPress = TRUE;
                touchCursor->touch[0].held = TRUE;
            }
        } else {
            touchCursor->touch[0].newPress = FALSE;
            touchCursor->touch[0].held = FALSE;
        }
        if (touchCursor->touch[0].newPress) {
            monIndex = PokeathlonGoalRoll_GetTouchedMon(goalRoll, (u8)i, input->x, input->y);
            if (monIndex != GOAL_ROLL_NO_MON) {
                PokeathlonGoalRollMon *mon = &frame.team[monIndex % 3];
                if (mon->mode != GOAL_ROLL_MODE_STUNNED && mon->mode != GOAL_ROLL_MODE_DASH && mon->motion == GOAL_ROLL_MOTION_NONE) {
                    goalRoll->heldMon[i] = monIndex;
                    goalRoll->holdFrames = 0;
                }
            }
        } else if (touchCursor->touch[0].held) {
            if (goalRoll->heldMon[i] != GOAL_ROLL_NO_MON) {
                if (goalRoll->holdFrames < 20) {
                    goalRoll->holdFrames++;
                }
                member = (u8)(goalRoll->heldMon[i] % 3);
                PokeathlonGoalRoll_SetTouchTarget(input->x, input->y, &frame.team[member].target);
            }
        } else if (goalRoll->heldMon[i] != GOAL_ROLL_NO_MON) {
            member = (u8)(goalRoll->heldMon[i] % 3);
            {
                PokeathlonGoalRollMon *mon = &frame.team[member];
                if (mon->mode != GOAL_ROLL_MODE_TIRED && mon->mode != GOAL_ROLL_MODE_STUNNED) {
                    // a short touch makes the Pokemon dash to where it was released
                    if (goalRoll->holdFrames <= 8) {
                        if (mon->target.x != mon->position.x || mon->target.y != mon->position.y) {
                            mon->mode = GOAL_ROLL_MODE_DASH;
                            ov96_021E8228(frame.course, (u8)i, (u8)(goalRoll->heldMon[i] % 3), 6, 1);
                        }
                    } else {
                        mon->mode = GOAL_ROLL_MODE_WALK;
                    }
                }
            }
            goalRoll->holdFrames = 0;
            goalRoll->heldMon[i] = GOAL_ROLL_NO_MON;
        }
    }
    PokeathlonGoalRollCpu_Update(frame.course, goalRoll->cpu);
    PokeathlonGoalRoll_MoveMons(frame.course);
    PokeathlonGoalRoll_MoveBalls(frame.course);
    PokeathlonGoalRoll_CollideBalls(frame.course);
    PokeathlonGoalRoll_CheckGoals(frame.course);
    PokeathlonGoalRoll_UpdateTiredMons(frame.course);
    if (goalRoll->timer <= 0) {
        goalRoll->timeUp = TRUE;
    } else {
        goalRoll->timeUp = FALSE;
    }
    PokeathlonGoalRoll_PackFrame(goalRoll, out);
}

void PokeathlonGoalRoll_PackFrame(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollFramePacket *out) {
    PokeathlonGoalRoll *work = goalRoll;
    PokeathlonGoalRollBall *ball;
    u8 ballY;
    int ballX;
    u8 score;
    int i;
    PokeathlonGoalRollMon(*team)[GOAL_ROLL_TEAM_SIZE];
    int shift;
    u32 facings;
    u32 modes;
    u8 slot;
    u32 ballStates;

    facings = 0;
    modes = 0;
    ballStates = 0;
    for (i = 0, shift = 0, team = work->mons; i < 12; i++, shift += 2) {
        PokeathlonGoalRollMon *mon = &team[i / 3][i % 3];
        int y = mon->position.y / FX32_ONE;
        int x = mon->position.x / FX32_ONE;
        u8 mode = mon->mode;
        u8 facing = mon->facing - 1;
        out->monX[i] = x;
        out->monY[i] = y;
        facings += facing << shift;
        modes += mode << shift;
    }
    ball = work->balls;
    for (i = 0; i < 2; i++) {
        ballX = ball->position.x / FX32_ONE;
        ballY = PokeathlonGoalRoll_EncodeBallY(ball);
        out->ballX[i] = ballX;
        out->ballY[i] = ballY;
        ballStates += ball->state << (i * 2);
        ball++;
    }
    slot = work->packetScoreSlot;
    score = work->scores[slot];
    work->packetScoreSlot = (slot + 1) % 4;
    out->states = facings;
    out->states += ballStates << 24;
    out->states += slot << 28;
    {
        s32 *states = &out->states;
        if (work->balls[0].kind == GOAL_ROLL_BALL_BIG) {
            *states += 1 << 30;
        }
    }
    if (work->balls[1].kind != GOAL_ROLL_BALL_NONE) {
        out->states = out->states - (2 << 30);
    }
    out->flags = 0;
    out->flags += modes;
    out->flags += work->timeUp << 24;
    out->flags += score << 25;
}

// The course's record of a participant's Pokemon (ov96_021E60C0).
typedef struct PokeathlonGoalRollCourseMon {
    u8 unk00[7];
    u8 size; // 1..3
} PokeathlonGoalRollCourseMon;

static inline u8 PokeathlonGoalRoll_GetMonSize(PokeathlonCourseData *course, int index) {
    return ((PokeathlonGoalRollCourseMon *)ov96_021E60C0(course, index / 3, index % 3))->size;
}

static inline void PokeathlonGoalRoll_ClearVec(VecFx32 *vec) {
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
}

// PokeathlonGoalRollDisplay_DrawScore as this call was built: with a u8 score.
typedef void (*PokeathlonGoalRollDrawScoreFunc)(PokeathlonGoalRollDisplay *display, u8 score);

// PokeathlonGoalRoll_PlayGoalAreaSE as this call was built: with u8 arguments.
typedef void (*PokeathlonGoalRollPlayGoalAreaSEFunc)(PokeathlonGoalRoll *goalRoll, u8 ballIndex, u8 bigBall, u8 code);

// Every participant: shows the frame the host sent.
void PokeathlonGoalRoll_ApplyFrame(PokeathlonCourseData *course) {
    u8 secondBall;
    PokeathlonGoalRoll *goalRoll;
    int x, y;
    u8 state, mode;
    int trail;
    int shift;
    u8 bigBall;
    int participant;
    u8 own;
    PokeathlonGoalRollFramePacket *frame;
    s32 i;
    PokeathlonGoalRollMon *mon;
    PokeathlonGoalRollBall *ball;
    PokeathlonGoalRollMonObject *object;

    goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    own = ov96_021E5F24(course);
    frame = (PokeathlonGoalRollFramePacket *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    PokeathlonGoalRoll_UpdateLeaders(goalRoll);
    for (i = 0; i < 12; i++) {
        participant = i / 3;
        mon = &goalRoll->mons[participant][i % 3];
        ov96_021E8BAC(ov96_021EAA20(mon->object));
        mode = (frame->flags >> (i * 2)) & 3;
        if (mode == GOAL_ROLL_MODE_TIRED) {
            ov96_021EB52C(mon->dashSprite, TRUE, TRUE);
            ov96_021EB570(mon->dashSprite, 12);
        } else if (mode == GOAL_ROLL_MODE_STUNNED) {
            ov96_021EB52C(mon->dashSprite, TRUE, TRUE);
            ov96_021EB570(mon->dashSprite, 16);
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
        } else {
            ov96_021EB52C(mon->dashSprite, TRUE, FALSE);
        }
        x = frame->monX[i];
        y = frame->monY[i];
        state = (frame->states >> (i * 2)) & 3;
        {
            VecFx32 position = { 0, 0, 0 };
            position.x = x << 12;
            position.y = y << 12;
            switch (PokeathlonGoalRoll_GetMonSize(course, i)) {
            case 1:
                position.y -= FX32_CONST(8);
                break;
            case 2:
                position.y -= FX32_CONST(8);
                break;
            case 3:
                position.y -= FX32_CONST(16);
                break;
            default:
                GF_AssertFail();
                break;
            }
            ov96_021EB588(mon->dashSprite, &position);
        }
        if (mode == GOAL_ROLL_MODE_DASH) {
            if (mon->trailCooldown == 0) {
                VecFx32 trailPosition = { 0, 0, 0 };
                s32 screenX, screenY;

                object = ov96_021EAA04(goalRoll->monObjects, i);
                trail = mon->trailIndex;
                ov96_021EB06C(object, frame->monX[i], frame->monY[i], &screenX, &screenY);
                trailPosition.x = screenX << 12;
                trailPosition.y = screenY << 12;
                ov96_021EB588(mon->trailSprites[trail], &trailPosition);
                ov96_021EB52C(mon->trailSprites[trail], TRUE, TRUE);
                ov96_021EB564(mon->trailSprites[trail], 14);
                mon->trailIndex = (mon->trailIndex + 1) % 3;
                mon->trailCooldown = 5;
                if (participant == own) {
                    PlaySE(SEQ_SE_GS_DASH);
                }
            } else {
                mon->trailCooldown--;
            }
        }
        object = ov96_021EAA04(goalRoll->monObjects, i);
        if (mode == GOAL_ROLL_MODE_STUNNED) {
            ov96_021EAC5C(object, 20);
        } else {
            ov96_021EAC0C(object, state + 1);
        }
        ov96_021EB01C(object, x, y, 1);
    }
    PokeathlonGoalRoll_SortOwnTeamOrder(goalRoll->mons[own], &frame->monY[own * 3]);
    PokeathlonGoalRoll_SortOtherMonsOrder(goalRoll, frame, own);
    bigBall = (frame->states >> 30) & 1;
    secondBall = (frame->states >> 31) & 1;
    for (i = 0, shift = 0; i < 2; i++, shift += 2) {
        VecFx32 ballPosition = { 0, 0, 0 };
        ball = &goalRoll->balls[i];
        state = ((u8)(frame->states >> 24) >> shift) & 3;
        if (state == GOAL_ROLL_BALL_SCORED) {
            ov96_021EB52C(ball->sprite, TRUE, FALSE);
            ov96_021EB52C(ball->effectSprite, TRUE, FALSE);
            PokeathlonGoalRoll_ShowGoal(goalRoll, i, (i == 0 && bigBall) ? 1 : 0, frame->ballX[i], frame->ballY[i], own);
        } else {
            ball->scored = 0;
            if (i == 1) {
                if (secondBall) {
                    ov96_021EB52C(ball->sprite, TRUE, TRUE);
                } else {
                    ov96_021EB52C(ball->sprite, TRUE, FALSE);
                }
            } else {
                ov96_021EB52C(ball->sprite, TRUE, TRUE);
            }
            if (state == GOAL_ROLL_BALL_KICKED) {
                ov96_021EB52C(ball->effectSprite, TRUE, TRUE);
                if (ball->shownState != GOAL_ROLL_BALL_KICKED) {
                    PlaySE(SEQ_SE_GS_SHOT);
                }
            } else {
                ov96_021EB52C(ball->effectSprite, TRUE, FALSE);
            }
            if (i == 0 && bigBall) {
                ov96_021EB570(ball->sprite, 11);
            } else {
                ov96_021EB570(ball->sprite, 10);
            }
        }
        ball->shownState = state;
        ballPosition.x = frame->ballX[i] << 12;
        ballPosition.y = PokeathlonGoalRoll_DecodeBallY(frame->ballY[i]);
        ((PokeathlonGoalRollPlayGoalAreaSEFunc)PokeathlonGoalRoll_PlayGoalAreaSE)(goalRoll, i, bigBall, frame->ballY[i]);
        ov96_021EB588(ball->sprite, &ballPosition);
        ov96_021EB588(ball->effectSprite, &ballPosition);
        if (frame->ballY[i] < 192) {
            ov96_021EB630(ball->sprite, 46);
        } else {
            ov96_021EB630(ball->sprite, 4);
        }
    }

    for (i = 0; i < 4; i++) {
        if (!ov96_021EB57C(goalRoll->goalFlashSprites[i])) {
            ov96_021EB52C(goalRoll->goalLabelSprites[i], TRUE, FALSE);
        }
    }
    PokeathlonGoalRoll_ApplyFrameScore(course);
    ((PokeathlonGoalRollDrawScoreFunc)PokeathlonGoalRollDisplay_DrawScore)(goalRoll->display, PokeathlonGoalRollDisplay_GetScore(goalRoll->display));
    PokeathlonGoalRollDisplay_UpdateTimer(goalRoll->display, goalRoll->timer);
    PokeathlonGoalRoll_RefreshPlates(goalRoll);
}

// PokeathlonGoalRollDisplay_SetScore as this call was built: with u8 arguments.
typedef void (*PokeathlonGoalRollSetScoreFunc)(PokeathlonGoalRollDisplay *display, u8 score, u8 participant);

void PokeathlonGoalRoll_ApplyFrameScore(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonGoalRollFramePacket *frame = (PokeathlonGoalRollFramePacket *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    u8 slot = (frame->states >> 28) & 3;
    u8 score = (frame->flags >> 25) & 0x7F;

    goalRoll->shownScores[slot] = score;
    ((PokeathlonGoalRollSetScoreFunc)PokeathlonGoalRollDisplay_SetScore)(goalRoll->display, score, slot);
}

#pragma force_active reset
