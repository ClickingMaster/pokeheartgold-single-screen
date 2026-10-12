#include "pokeathlon/pokeathlon_ring_drop.h"

#include "global.h"

#include "pokeathlon/pokeathlon_ring_drop_cpu.h"

#include "assert.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

// Course helpers, mon objects and the computer participants (OVY_96).
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, u32 a1);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
Sprite **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int a1, void *resources, SpriteList *spriteList);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int event, int value);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u32 value);
void PokeathlonCourse_SetFinishCallback(PokeathlonCourseData *course, void (*callback)(PokeathlonCourseData *));
void PokeathlonCourseScoreList_Rank(void *gfx, int a1);
void PokeathlonCourseScoreList_Init(void *gfx);
void PokeathlonCourseScoreList_AddPointsToTotals(void *gfx);
void PokeathlonCourseScoreList_SaveEventPoints(void *gfx, int a1);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, void *gfx, int participant, u16 score, int a4, int a5, int a6);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *monObjects);
void PokeathlonCourseObject_CenterToGround(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObjects_PauseAnims(void *monObjects, int a1);
void PokeathlonEvent_CalcPoints(void *gfx, int a1);

extern const GraphicsBanks ov96_0221D774;

void PokeathlonScreenShake_Task(SysTask *task, PokeathlonScreenShake *shake);

BOOL PokeathlonScreenShake_IsDone(PokeathlonScreenShake *shake) {
    GF_ASSERT(shake != NULL);
    return shake->task == NULL;
}

void PokeathlonScreenShake_Init(PokeathlonScreenShake *shake, BgConfig *bgConfig, SpriteSystem *spriteSystem) {
    shake->bgConfig = bgConfig;
    shake->spriteSystem = spriteSystem;
}

#pragma force_active on

void PokeathlonScreenShake_Start(PokeathlonScreenShake *shake, int magnitude, int duration) {
    GF_ASSERT(shake->bgConfig != NULL);
    GF_ASSERT(shake->spriteSystem != NULL);
    if (shake->task != NULL) {
        SysTask_Destroy(shake->task);
    }
    shake->direction = 0;
    shake->frame = 0;
    shake->duration = duration;
    shake->offset = magnitude << 12;
    shake->magnitude = magnitude;
    shake->step = FX_Div(magnitude << 12, duration << 12);
    shake->task = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonScreenShake_Task, shake, 2);
}

#pragma force_active reset

BOOL PokeathlonRingDrop_IsOnSwitchButton(int x, int y) {
    return x >= 104 && x < 160 && y >= 0 && y < 32;
}

#pragma force_active on

BOOL PokeathlonRingDrop_IsWithinDistance(VecFx32 *a, VecFx32 *b, fx32 distance) {
    VecFx32 delta;

    VEC_Subtract(b, a, &delta);
    if (VEC_Mag(&delta) <= distance) {
        return TRUE;
    }
    return FALSE;
}

void PokeathlonRingDrop_Decay(fx32 *value, fx32 amount) {
    if (*value != 0) {
        if ((*value < 0 ? -*value : *value) < amount) {
            *value = 0;
            return;
        }
        *value -= amount * (*value > 0 ? 1 : -1);
    }
}

#pragma force_active reset

void PokeathlonRingDrop_DecayVelocity(VecFx32 *velocity, fx32 amountX, fx32 amountY) {
    PokeathlonRingDrop_Decay(&velocity->x, amountX);
    PokeathlonRingDrop_Decay(&velocity->y, amountY);
}

void PokeathlonRingDrop_ClampVelocity(VecFx32 *velocity, fx32 limit) {
    fx32 min = -limit;

    if (velocity->x > 0) {
        if (velocity->x > limit) {
            velocity->x = limit;
        }
    } else if (velocity->x < min) {
        velocity->x = min;
    }
    if (velocity->y > 0) {
        if (velocity->y > limit) {
            velocity->y = limit;
        }
    } else if (velocity->y < min) {
        velocity->y = min;
    }
}

BOOL PokeathlonRingDrop_State_FadeIn(PokeathlonCourseData *course, u8 *state) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, ringDrop->heapId);
        (*state)++;
    case 1:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 2:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonRingDrop_State_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (*state) {
    case 0:
        PokeathlonRingDrop_Update(course);
        if (PokeathlonRingDrop_ReadInput(course)) {
            (*state)++;
        }
        break;
    case 1:
        PokeathlonRingDrop_Update(course);
        if (PokeathlonCourse_RunFinish(course) && PokeathlonScreenShake_IsDone(&ringDrop->shake)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, ringDrop->heapId);
            PokeathlonCourse_SetStateField07(course, 2);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonRingDrop_State_Finish(PokeathlonCourseData *course, u8 *state) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            if (!ov96_021E5F24(course)) {
                int i;
                PokeathlonRingDropMonFlags *flags;
                // one cursor over the players' flags words
                for (i = 0, flags = &ringDrop->players[0].mon.flags; i < RING_DROP_PARTICIPANTS; i++, flags = (PokeathlonRingDropMonFlags *)((u8 *)flags + sizeof(PokeathlonRingDropPlayer))) {
                    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)i, flags->bits.score);
                }
            }
            return TRUE;
        }
    } else {
        GF_AssertFail();
    }
    return FALSE;
}

BOOL PokeathlonRingDrop_Init(PokeathlonCourseData *course) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_145, 0x60000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        PokeathlonRingDrop_SetGfxBanks();
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        ringDrop = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonRingDrop));
        MI_CpuFill8(ringDrop, 0, sizeof(PokeathlonRingDrop));
        ringDrop->heapId = HEAP_ID_145;
        ringDrop->timer = RING_DROP_FRAMES;
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        ringDrop->bgConfig = BgConfig_Alloc(ringDrop->heapId);
        PokeathlonCourse_SetHudPlanes(course, 8);
        PokeathlonRingDrop_InitBgs(ringDrop);
        PokeathlonRingDrop_LoadBgGraphics(ringDrop);
        PokeathlonRingDrop_AddScoreWindow(ringDrop);
        PokeathlonRingDrop_InitSprites(ringDrop);
        PokeathlonRingDrop_CreateMonObjects(ringDrop, course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        if (PokeathlonCourseMonObjects_IsLoaded(ringDrop->monObjects)) {
            PokeathlonRingDrop_LoadSpriteResources(ringDrop);
            Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSprites(course, 0, ringDrop->resources, SpriteManager_GetSpriteList(ringDrop->spriteManager)), 0);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 3: {
        u8 participant = ov96_021E5F24(course);
        ringDrop->cpu = PokeathlonRingDropCpu_New(course, ringDrop->heapId);
        ringDrop->ui = PokeathlonRingDropUi_New(ringDrop->spriteSystem, ringDrop->spriteManager, ringDrop->resources, ringDrop->bgConfig, ringDrop->msgData, ringDrop->msgFormat, participant, ringDrop->heapId, course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 4:
        PokeathlonRingDrop_InitMons(ringDrop, course);
        PokeathlonRingDropUi_Init(ringDrop->ui);
        PokeathlonRingDropDisplay_Init(&ringDrop->display, ringDrop->spriteSystem, ringDrop->spriteManager, ringDrop->ui, ringDrop->monObjects, ringDrop->heapId);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)ringDrop->bgConfig);
        PokeathlonScreenShake_Init(&ringDrop->shake, ringDrop->bgConfig, ringDrop->spriteSystem);
        PokeathlonCourse_SetField1F4(course, 1);
        if (ov96_021E5F24(course) == 0) {
            PokeathlonRingDropFramePacket *packet = (PokeathlonRingDropFramePacket *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + RING_DROP_DATA_FRAME_OUT);
            packet->timer = ringDrop->timer;
            PokeathlonRingDrop_BuildFramePacket(ringDrop->contacts, course);
        }
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonRingDrop_Draw(PokeathlonCourseData *course) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    GF_ASSERT(ringDrop != NULL);
    SpriteSystem_DrawSprites(ringDrop->spriteManager);
    return TRUE;
}

BOOL ov96_02215CE8(PokeathlonCourseData *course) {
    void *gfx = PokeathlonCourse_GetGraphicsSystem(course);
    int i;

    PokeathlonCourseScoreList_Init(gfx);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        PokeathlonCourse_AddScoreEntry(course, gfx, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 298);
    }
    PokeathlonCourseScoreList_Rank(gfx, 1);
    PokeathlonEvent_CalcPoints(gfx, 7);
    PokeathlonCourseScoreList_AddPointsToTotals(gfx);
    PokeathlonCourseScoreList_SaveEventPoints(gfx, 7);
    return TRUE;
}

BOOL PokeathlonRingDrop_Exit(PokeathlonCourseData *course) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);

    PokeathlonRingDropCpu_Free(ringDrop->cpu);
    PokeathlonRingDropUi_Free(ringDrop->ui);
    PokeathlonRingDropDisplay_Free(&ringDrop->display);
    PokeathlonRingDrop_FreeMonObjects(ringDrop);
    PokeathlonRingDrop_FreeSprites(ringDrop);
    PokeathlonRingDrop_FreeBgs(ringDrop);
    WirelessStatusIcon_Destroy();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_145);
    return TRUE;
}

s32 PokeathlonRingDrop_GetMonState(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    return ringDrop->players[participant].mon.state;
}

#pragma force_active on

VecFx32 *PokeathlonRingDrop_GetMonPosition(PokeathlonCourseData *course, int participant) {
    u32 mon = (u32) & ((PokeathlonRingDrop *)PokeathlonCourse_GetHeapAllocPtr4(course))->players[participant].mon;
    return (VecFx32 *)(mon + offsetof(PokeathlonRingDropMon, position));
}

void PokeathlonRingDrop_GetMonScreenPosition(PokeathlonCourseData *course, int participant, fx32 *out) {
    int x, y;
    PokeathlonRingDropMon *mon = &((PokeathlonRingDrop *)PokeathlonCourse_GetHeapAllocPtr4(course))->players[participant].mon;

    PokeathlonCourseObject_CenterToGround(mon->member->object, mon->position.x >> 12, mon->position.y >> 12, &x, &y);
    out[0] = x << 12;
    out[1] = y << 12;
}

u32 PokeathlonRingDrop_GetMonDirection(PokeathlonCourseData *course, int participant) {
    return ((PokeathlonRingDrop *)PokeathlonCourse_GetHeapAllocPtr4(course))->players[participant].mon.flags.bits.direction;
}

#pragma force_active reset

u8 PokeathlonRingDrop_GetMonStamina(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMember *member = ringDrop->players[participant].mon.member;
    return member->stamina >> 12;
}

u8 PokeathlonRingDrop_GetMonStaminaPercent(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMember *member = ringDrop->players[participant].mon.member;
    return (FX_Div(member->stamina, member->params->stamina) * 100) >> 12;
}

#pragma force_active on

u32 PokeathlonRingDrop_IsMonSwitching(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    return ringDrop->players[participant].mon.flags.bits.switching;
}

u32 PokeathlonRingDrop_IsMonPushed(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    return ringDrop->players[participant].mon.flags.bits.pushed;
}

#pragma force_active reset

BOOL PokeathlonRingDrop_CanMonAct(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    return PokeathlonRingDropMon_CanAct(&ringDrop->players[participant].mon);
}

#pragma force_active on

void PokeathlonRingDrop_SetMonTarget(PokeathlonCourseData *course, int participant, const s16 *target) {
    u8 *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMonFlags *flags = &((PokeathlonRingDrop *)ringDrop)->players[0].mon.flags;
    u32 offset = participant * sizeof(PokeathlonRingDropPlayer);

    GF_ASSERT(((PokeathlonRingDropMonFlags *)((u8 *)flags + offset))->bits.exhausted == 0);
    ((PokeathlonRingDropMonFlags *)((u8 *)flags + offset))->bits.hasTarget = TRUE;
    ((PokeathlonRingDrop *)(ringDrop + offset))->players[0].mon.targetX = target[0];
    ((PokeathlonRingDrop *)(ringDrop + offset))->players[0].mon.targetY = target[1];
}

void PokeathlonRingDrop_RequestJump(PokeathlonCourseData *course, int participant) {
    u8 *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMonFlags *flags = &((PokeathlonRingDrop *)ringDrop)->players[0].mon.flags;
    u32 offset = participant * sizeof(PokeathlonRingDropPlayer);

    GF_ASSERT(((PokeathlonRingDropMonFlags *)((u8 *)flags + offset))->bits.exhausted == 0);
    GF_ASSERT(((PokeathlonRingDropMonFlags *)((u8 *)flags + offset))->bits.switching == 0);
    ((PokeathlonRingDropMonFlags *)((u8 *)flags + offset))->bits.jumpRequested = TRUE;
}

#pragma force_active reset

void PokeathlonRingDrop_StartMonSwitch(PokeathlonCourseData *course, int participant) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMon_StartSwitch(&ringDrop->players[participant].mon);
}

void PokeathlonRingDrop_SetMonDestination(PokeathlonCourseData *course, int participant, const s16 *position) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropMon_SetDestination(&ringDrop->players[participant].mon, position);
}

int PokeathlonRingDrop_GetDirection(const VecFx32 *vec) {
    return PokeathlonRingDrop_VecToDirection(vec);
}

void PokeathlonRingDrop_SetGfxBanks(void) {
    GraphicsBanks banks = ov96_0221D774;
    GfGfx_SetBanks(&banks);
}

// Every participant: the host first runs the ring on all inputs and sends the frame, then the
// frame received is shown.
#pragma force_active on
#pragma opt_lifetimes off

void PokeathlonRingDrop_Update(PokeathlonCourseData *course) {
    int i;
    PokeathlonRingDrop *ringDrop;
    PokeathlonRingDropFramePacket *packet;
    u8 *data;
    PokeathlonRingDropPlayer *player;

    ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    data = PokeathlonCourse_GetDataCopyArea(course);
    packet = (PokeathlonRingDropFramePacket *)ov96_021E8A20(data + RING_DROP_DATA_FRAME_OUT);
    if (ov96_021E5F24(course) == 0) {
        if (packet->finished) {
            PokeathlonRingDrop_BuildFramePacket(ringDrop->contacts, course);
            return;
        }
        if (ringDrop->timer > 0) {
            ringDrop->timer--;
            packet->timer = ringDrop->timer;
        }
        *(PokeathlonRingDropInput *)ov96_021E8A20(data + RING_DROP_DATA_INPUTS) = *(PokeathlonRingDropInput *)ov96_021E8A20(data);
        i = 0;
        data += RING_DROP_DATA_INPUTS;
        player = ringDrop->players;
        for (; i < RING_DROP_PARTICIPANTS; player++, data += RING_DROP_DATA_SLOT_SIZE, i++) {
            PokeathlonRingDropInput *input = (PokeathlonRingDropInput *)ov96_021E8A20(data);
            PokeathlonRingDropTouch *touch = &player->touch;
            PokeathlonRingDropMon *mon = &player->mon;
            if (input->touching) {
                if (touch->started && touch->touching) {
                    touch->started = FALSE;
                } else if (!touch->started && !touch->touching) {
                    touch->started = TRUE;
                    touch->touching = TRUE;
                    player->touchStartX = input->x;
                    player->touchStartY = input->y;
                }
                player->touchX = input->x;
                player->touchY = input->y;
                touch->frames++;
            } else if (touch->touching) {
                touch->lastFrames = touch->frames;
                touch->frames = 0;
                touch->started = FALSE;
                touch->touching = FALSE;
            }
            PokeathlonRingDropMon_UpdateStatus(mon, touch, PokeathlonRingDrop_IsOnSwitchButton(player->touchX, player->touchY));
            if (mon->flags.bits.switching) {
                if (mon->flags.bits.switchState == 1 && input->switchState == 1) {
                    PokeathlonRingDropMon_NextMember(mon);
                    PokeathlonRingDropMon_ResetToStart(mon);
                    mon->flags.bits.switchState = 2;
                } else if (mon->flags.bits.switchState == 2 && input->switchState == 2) {
                    mon->flags.bits.switching = FALSE;
                    mon->flags.bits.switchState = 0;
                    PokeathlonRingDropMon_SetState(mon, 4);
                }
            }
        }
        PokeathlonRingDropCpu_Update(ringDrop->cpu);
        PokeathlonRingDrop_UpdateMons(ringDrop->players, course);
        if (packet->timer == 0) {
            packet->finished = TRUE;
        }
        PokeathlonRingDrop_BuildFramePacket(ringDrop->contacts, course);
    }
    PokeathlonRingDropDisplay_Update(&ringDrop->display, course);
}

#pragma opt_lifetimes reset
#pragma force_active reset

// Callback when the course ends: reports the own score.
void PokeathlonRingDrop_OnFinish(PokeathlonCourseData *course) {
    u8 score;

    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + RING_DROP_DATA_FRAME_IN);
    score = PokeathlonRingDropUi_GetScore(((PokeathlonRingDrop *)PokeathlonCourse_GetHeapAllocPtr4(course))->ui);
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonRingDrop_PrintScore(course, score);
}

// Every participant: reads the touch screen into the own input; TRUE once the host reports the
// time is up.
#pragma force_active on

BOOL PokeathlonRingDrop_ReadInput(PokeathlonCourseData *course) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *data = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonRingDropInput *input = (PokeathlonRingDropInput *)ov96_021E8A20(data);
    PokeathlonRingDropFramePacket *packet = (PokeathlonRingDropFramePacket *)ov96_021E8A20(data + RING_DROP_DATA_FRAME_IN);

    if (packet->finished == TRUE) {
        PokeathlonRingDropDisplay_StopAnimations(&ringDrop->display);
        PokeathlonCourseObjects_PauseAnims(ringDrop->monObjects, 1);
        PokeathlonRingDropUi_StopAnimations(ringDrop->ui);
        PokeathlonCourse_SetFinishCallback(course, PokeathlonRingDrop_OnFinish);
        return TRUE;
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        input->x = gSystem.touchX;
        input->y = gSystem.touchY;
        input->touching = TRUE;
    } else {
        input->touching = FALSE;
    }
    return FALSE;
}

#pragma force_active reset
