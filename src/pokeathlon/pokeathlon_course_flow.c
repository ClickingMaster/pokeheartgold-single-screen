#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_save.h"

#include "bg_window.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "unk_02031B0C.h"
#include "communication_command.h"
#include "unk_02035900.h"
#include "vram_transfer_manager.h"

#pragma force_active on

// Reads the performance data (NARC poketool/personal_performance) of every participant's Pokemon.
void PokeathlonCourse_LoadPerformanceData(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    NARC *narc = NARC_New(NARC_poketool_personal_performance, layout->heapId);
    int i;
    int j;
    PokeathlonCoursePerformance *row;
    // layout / monBase are the course pointer moved on by whole participants / Pokemon, so that their
    // participants[0].mons[0] is the current Pokemon (needed to match)
    i = 0;
    row = layout->performance.mons[0];
    for (; i < 4; layout = (PokeathlonCourseLayout *)((u8 *)layout + sizeof(PokeathlonCourseParticipant)), row += 3, i++) {
        PokeathlonCourseLayout *monBase = layout;
        PokeathlonCoursePerformance *dest = row;
        for (j = 0; j < 3; j++, monBase = (PokeathlonCourseLayout *)((u8 *)monBase + sizeof(PokeathlonCourseMon)), dest++) {
            NARC_ReadWholeMember(narc, PokeathlonCourse_GetPerformanceFileId(monBase->participants[0].mons[0].species, monBase->participants[0].mons[0].form), dest);
        }
    }
    NARC_Delete(narc);
}

#pragma force_active reset

PokeathlonCoursePerformance *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int mon) {
    return &((PokeathlonCourseLayout *)course)->performance.mons[participant][mon];
}

#pragma force_active on

u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int mon) {
    if (participant >= 4) {
        GF_AssertFail();
    }
    if (mon >= 3) {
        GF_AssertFail();
    }
    // participant and Pokemon offsets added to the first Pokemon's stats (needed to match)
    return ((PokeathlonCourseLayout *)course)->participants[0].mons[0].stats + participant * sizeof(PokeathlonCourseParticipant) + mon * sizeof(PokeathlonCourseMon);
}

#pragma force_active reset

int ov96_021E6104(void) {
    return 0x50;
}

int ov96_021E6108(const PokeathlonCourseUnkTypes *types) {
    if (types == NULL) {
        return 0;
    }
    switch (types->unk08) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

int ov96_021E6138(const PokeathlonCourseUnkTypes *types) {
    if (types == NULL) {
        return 0;
    }
    switch (types->unk06) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

// Fills the sprite loader info of a participant's Pokemon; isLarge from its follower parameters.
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int which, PokeathlonCourseMonInfo *info) {
    u8 followParam[4];
    PokeathlonCourseMon *mon;
    if (which >= 3) {
        GF_AssertFail();
    }
    mon = (PokeathlonCourseMon *)(PokeathlonCourse_GetParticipantUnk04(course, participant) + which * sizeof(PokeathlonCourseMon));
    info->isShiny = mon->isShiny;
    info->species = mon->species;
    info->form = mon->form;
    info->gender = mon->gender;
    info->participant = participant;
    info->personality = mon->personality;
    if (info->species == SPECIES_NONE) {
        GF_AssertFail();
    }
    if (info->species == SPECIES_NONE) {
        info->species = SPECIES_BULBASAUR;
    }
    ReadWholeNarcMemberByIdPair(followParam, NARC_fielddata_tsurepoke_tp_param, SpeciesToOverworldModelIndexOffset(info->species));
    if (followParam[1] != 0) {
        info->isLarge = TRUE;
    } else {
        info->isLarge = FALSE;
    }
}

#pragma force_active on

// Creates the countdown / finish, new record and time-up sprites of the event HUD.
Sprite **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    VecFx32 position;
    VecFx32 timeUpPosition;
    layout->countSprite = PokeathlonCourseSprites_CreateCountdown(resources, spriteList, 0, NNS_G2D_VRAM_TYPE_2DMAIN, course->heapId);
    layout->newRecordSprite = PokeathlonCourseSprites_CreateNewRecord(resources, spriteList, 0, course->heapId);
    layout->timeUpSprite = PokeathlonCourseSprites_CreateTimeUpCountdown(resources, spriteList, 0, course->heapId);
    position.z = 0;
    position.x = 128 * FX32_ONE;
    position.y = (baseY + 96) << FX32_SHIFT;
    Sprite_SetMatrix(layout->countSprite, &position);
    layout->hudBaseY = baseY;
    timeUpPosition.z = 0;
    timeUpPosition.x = 128 * FX32_ONE;
    timeUpPosition.y = (layout->hudBaseY + 72) << FX32_SHIFT;
    Sprite_SetMatrix(layout->timeUpSprite, &timeUpPosition);
    layout->finishState = 0;
    position.y = (baseY + 130) << FX32_SHIFT;
    Sprite_SetMatrix(layout->newRecordSprite, &position);
    return &layout->countSprite;
}

#pragma force_active reset

Sprite **ov96_021E6290(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, void *eventGfx) {
    return PokeathlonCourse_CreateHudSprites(course, baseY, resources, PokeathlonCourseSpritePool_GetSpriteList(eventGfx));
}

#pragma force_active on

// Creates count "Start" sprites at the given positions (offset by baseY).
void PokeathlonCourse_CreateStartSprites(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, NNS_G2D_VRAM_TYPE whichScreen, u8 count, PokeathlonCoursePoint *positions) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    VecFx32 position;
    layout->startSpriteCount = count;
    for (i = 0; i < count; i++) {
        layout->startSprites[i] = PokeathlonCourseSprites_CreateStart(resources, spriteList, whichScreen, 0, course->heapId);
        position.z = 0;
        position.x = positions[i].x << FX32_SHIFT;
        position.y = (baseY + positions[i].y) << FX32_SHIFT;
        Sprite_SetMatrix(layout->startSprites[i], &position);
    }
    for (; i < 3; i++) {
        layout->startSprites[i] = NULL;
    }
    layout->startState = 0;
}

#pragma force_active reset

void ov96_021E634C(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, void *eventGfx, NNS_G2D_VRAM_TYPE whichScreen, u8 count, PokeathlonCoursePoint *positions) {
    PokeathlonCourse_CreateStartSprites(course, baseY, resources, PokeathlonCourseSpritePool_GetSpriteList(eventGfx), whichScreen, count, positions);
}

#pragma force_active on

// "Start" animation, then "3, 2, 1" (one step every 21 frames). Returns TRUE when the event may begin.
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    BOOL done = FALSE;
    switch (layout->startState) {
    case 0:
        PlaySE(SEQ_SE_GS_TOUCHPEN);
        PokeathlonCourse_ShowStartSprites(course);
        layout->startState++;
        break;
    case 1:
        if (PokeathlonCourse_IsStartAnimDone(course)) {
            layout->startState++;
            Sound_SetPlayerPause(7, 0);
        }
        break;
    case 2:
        if (layout->hudTimer == 0) {
            PlaySE(SEQ_SE_GS_STARTCOUNT);
            Sprite_SetDrawFlag(layout->countSprite, TRUE);
            Sprite_SetAnimCtrlSeq(layout->countSprite, 0);
        } else if (layout->hudTimer == 21) {
            PlaySE(SEQ_SE_GS_STARTCOUNT);
            Sprite_SetAnimCtrlSeq(layout->countSprite, 1);
        } else if (layout->hudTimer == 42) {
            PlaySE(SEQ_SE_GS_STARTCOUNT);
            Sprite_SetAnimCtrlSeq(layout->countSprite, 2);
        }
        layout->hudTimer++;
        if (layout->hudTimer > 63) {
            PlaySE(SEQ_SE_GS_CON_016);
            Sprite_SetDrawFlag(layout->countSprite, FALSE);
            layout->hudTimer = 0;
            done = TRUE;
        }
        break;
    }
    return done;
}

#pragma force_active reset

// Shows the "3, 2, 1" of the last seconds of an event (framesLeft at 30 fps).
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int framesLeft) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    int seconds = (framesLeft + 29) / 30;
    if (seconds <= 0) {
        Sprite_SetDrawFlag(layout->timeUpSprite, FALSE);
        return;
    }
    if (seconds <= 3) {
        if (layout->timeUpSeconds != seconds) {
            PlaySE(SEQ_SE_GS_ENDCOUNT);
            layout->timeUpSeconds = seconds;
        }
        Sprite_SetDrawFlag(layout->timeUpSprite, TRUE);
        Sprite_SetAnimCtrlSeq(layout->timeUpSprite, seconds + 1);
        return;
    }
    Sprite_SetDrawFlag(layout->timeUpSprite, FALSE);
}

void PokeathlonCourse_InitMarkers(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    layout->markers.count = 0;
    for (i = 0; i < 3; i++) {
        if (layout->markers.markers[i].active != FALSE) {
            GF_AssertFail();
        }
        layout->markers.markers[i].object = NULL;
        layout->markers.markers[i].sprite = NULL;
        layout->markers.markers[i].task = NULL;
    }
}

#pragma force_active on

// Adds a marker sprite that follows object (up to 3).
Sprite *PokeathlonCourse_AddMarker(PokeathlonCourseData *course, PokeathlonCourseMonObject *object, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourseMarker *marker;
    if (layout->markers.count >= 3) {
        GF_AssertFail();
        return NULL;
    }
    marker = &layout->markers.markers[(u8)layout->markers.count];
    marker->active = TRUE;
    marker->object = object;
    marker->sprite = PokeathlonCourseSprites_CreateMarker(resources, spriteList, priority, course->heapId);
    marker->task = SysTask_CreateOnMainQueue(PokeathlonCourse_MarkerTask, marker, 1);
    layout->markers.count++;
    return marker->sprite;
}

#pragma force_active reset

void PokeathlonCourse_FreeMarkers(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    for (i = 0; i < 3; i++) {
        layout->markers.markers[i].active = FALSE;
        layout->markers.markers[i].object = NULL;
        layout->markers.markers[i].sprite = NULL;
        SysTask_Destroy(layout->markers.markers[i].task);
        layout->markers.markers[i].task = NULL;
    }
}

void PokeathlonCourse_SetMarkerAnim(PokeathlonCourseData *course, int index, int seq) {
    Sprite_TryChangeAnimSeq(((PokeathlonCourseLayout *)course)->markers.markers[index].sprite, seq);
}

void PokeathlonCourse_StopMarkerAnims(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    for (i = 0; i < 3; i++) {
        if (layout->markers.markers[i].active == FALSE) {
            GF_AssertFail();
        }
        Sprite_SetAnimActiveFlag(layout->markers.markers[i].sprite, FALSE);
    }
}

void PokeathlonCourse_ShowStartSprites(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    if (layout->startSpriteCount != 0) {
        for (i = 0; i < layout->startSpriteCount; i++) {
            Sprite_SetDrawFlag(layout->startSprites[i], TRUE);
            Sprite_SetAnimCtrlSeq(layout->startSprites[i], 0);
        }
    }
}

BOOL PokeathlonCourse_IsStartAnimDone(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    if (layout->startSpriteCount == 0) {
        return TRUE;
    }
    if (!Sprite_IsAnimated(layout->startSprites[0])) {
        for (i = 0; i < layout->startSpriteCount; i++) {
            Sprite_SetDrawFlag(layout->startSprites[i], FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, u32 planes) {
    ((PokeathlonCourseLayout *)course)->hudPlanes = planes;
}

#pragma force_active on

// "Finish": shows the finish sprite, runs the event's result callback, shows "New record" if the result beats
// the saved record, then waits. Returns TRUE when done.
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    switch (layout->finishState) {
    case 0: {
        VecFx32 position;
        Sprite_SetDrawFlag(layout->timeUpSprite, FALSE);
        PlaySE(SEQ_SE_GS_FINISH);
        position.z = 0;
        position.x = 128 * FX32_ONE;
        position.y = (layout->hudBaseY + 72) << FX32_SHIFT;
        Sprite_SetMatrix(layout->countSprite, &position);
        Sprite_SetAnimCtrlSeq(layout->countSprite, 3);
        Sprite_SetDrawFlag(layout->countSprite, TRUE);
        layout->finishState = 1;
        break;
    }
    case 1:
        layout->hudTimer++;
        if (layout->hudTimer > 60) {
            layout->hudTimer = 0;
            layout->finishState = 2;
        }
        break;
    case 2:
        if (layout->finishCallback) {
            layout->finishCallback(course);
        }
        if (PokeathlonCourse_IsNewRecord(course)) {
            PlaySE(SEQ_SE_GS_NEWRECORD);
            Sprite_SetAnimCtrlSeq(layout->newRecordSprite, 0);
            Sprite_SetDrawFlag(layout->newRecordSprite, TRUE);
        }
        GfGfx_EngineATogglePlanes((u8)layout->hudPlanes, TRUE);
        layout->finishCallback = NULL;
        layout->finishState = 3;
        break;
    case 3:
        layout->hudTimer++;
        if (layout->hudTimer > 120) {
            layout->hudTimer = 0;
            layout->finishState = 4;
            return TRUE;
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// File of a Pokemon's form in NARC poketool/personal_performance.
int PokeathlonCourse_GetPerformanceFileId(int species, int form) {
    return form + ov96_0221AAE8[species];
}

// The course states (PokeathlonCourse_SetStateField07) that run between the events, and the event results.

void ov96_021E67AC(PokeathlonCourseData *data) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)data;
    if (layout->field_1F4 != 0) {
        layout->event->unk04(data, 0);
    }
}

// Host: chooses the events of the course and sends them to every player.
u32 PokeathlonCourse_State_SendEvents(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    BOOL sent = TRUE;
    if (!ov96_021E5F24(course)) {
        PokeathlonCourse_ChooseEvents(layout->args, layout->rounds, layout->eventIds);
        sent = PokeathlonCourse_TrySendCommand(POKEATHLON_COURSE_CMD_EVENTS, layout->eventIds, (void *)PokeathlonCourseCmd_EventsSize(), (int)layout->link);
    }
    if (sent) {
        PokeathlonCourse_SetStateField07_IfDifferent(course, 38, 1);
    }
    return 0;
}

// Waits for the event that was running in the sub overlay to end.
u32 PokeathlonCourse_State_RunEventOverlay(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (OverlayManager_Run(layout->subOverlay)) {
        OverlayManager *manager = layout->subOverlay;
        PokeathlonCourseArgsLayout *args;
        OverlayManager_Delete(manager);
        args = layout->args;
        if (args->mode == 0) {
            if (layout->unk1FC != 0) {
                args->shouldFreeHeap = TRUE;
                PokeathlonCourse_SetStateField07(course, 37);
            } else {
                args->shouldFreeHeap = FALSE;
                PokeathlonCourse_SetStateField07(course, 3);
            }
        } else {
            args->shouldFreeHeap = FALSE;
            PokeathlonCourse_SetStateField07(course, 2);
        }
    }
    return 0;
}

#pragma force_active on

u32 ov96_021E6870(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    switch (*state) {
    case 0:
        layout->event = &ov96_0221A8BC;
        layout->field_1ED = 0;
        PokeathlonCourse_InitStateInfo(ov96_0221DA68, &layout->eventStateInfo);
        (*state)++;
        break;
    case 1:
        if (PokeathlonCourse_RunSubStateLoop(course)) {
            (*state)++;
        }
        break;
    case 2:
        if (layout->eventStateInfo.ptr == NULL) {
            (*state)++;
            break;
        }
        if (layout->courseState.argsPtr == &layout->stateInfo) {
            layout->courseState.argsPtr = &layout->eventStateInfo;
            layout->courseState.exitFlag = 0;
        }
        if (((PokeathlonCourseStepFunc *)layout->eventStateInfo.ptr)[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
            PokeathlonCourse_SetField1F4(course, 0);
            layout->courseState.argsPtr = &layout->stateInfo;
            layout->courseState.exitFlag = 0;
            (*state)++;
        }
        break;
    case 3:
        layout->event->exit(course, 0);
        if (layout->heapAllocPtr4) {
            GF_AssertFail();
        }
        PokeathlonCourse_SetStateField07(course, 3);
        break;
    }
    return 0;
}

#pragma force_active reset

u32 PokeathlonCourse_State_SendParticipant(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->localParticipant.npcTeam = 0;
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_PARTICIPANT, &layout->localParticipant, PokeathlonCourseCmd_ParticipantSize(), layout->link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

u32 PokeathlonCourse_State_SendParticipants(PokeathlonCourseData *course) {
    void *participants = PokeathlonCourse_GetParticipantData(course, 0);
    int size = PokeathlonCourseCmd_AllParticipantsSize();
    void *link = PokeathlonCourse_GetSystem(course);
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_ALL_PARTICIPANTS, participants, size, link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

u32 PokeathlonCourse_State_SendProfiles(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    BOOL sent = TRUE;
    if (!ov96_021E5F24(course)) {
        sent = PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_PROFILES, layout->playerProfiles, PokeathlonCourseCmd_ProfilesSize(), layout->link);
    }
    if (sent) {
        PokeathlonCourse_SetStateField07_IfDifferent(course, 38, 6);
    }
    return 0;
}

#pragma force_active on

u32 PokeathlonCourse_State_SendRecord(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->results.record = *PokeathlonSave_GetAgainUnkB00(Save_Pokeathlon_Get(layout->args->saveData));
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_RECORD, &layout->results.record, PokeathlonCourseCmd_RecordSize(), layout->link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6A54(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->counterEnabled = TRUE;
    PokeathlonCourse_ResetDataCopyArea(course);
    layout->event = &ov96_0221A808;
    layout->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA50, &layout->eventStateInfo);
    PokeathlonCourse_SetStateField07(course, 8);
    return 0;
}

u32 ov96_021E6A94(PokeathlonCourseData *course) {
    if (PokeathlonCourse_RunSubStateLoop(course)) {
        PokeathlonCoursePacket *packets = (PokeathlonCoursePacket *)PokeathlonCourse_GetDataCopyArea(course);
        packets[0].unk24 = 0;
        packets[1].unk24 = 1;
        PokeathlonCourseLink_SetCheckSize(PokeathlonCourse_GetSystem(course), TRUE);
        PokeathlonCourse_SetStateTransitionType(course, 6);
        PokeathlonCourse_SetStateField07(course, 9);
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(25, SEQ_GS_PT_OPEN, 0);
    }
    return 0;
}

#pragma force_active on

u32 ov96_021E6AE8(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (layout->eventStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(course, 10);
        return 0;
    }
    if (layout->courseState.argsPtr == &layout->stateInfo) {
        layout->courseState.argsPtr = &layout->eventStateInfo;
        layout->courseState.exitFlag = 0;
    }
    if (((PokeathlonCourseStepFunc *)layout->eventStateInfo.ptr)[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
        PokeathlonCourse_SetField1F4(course, 0);
        layout->courseState.argsPtr = &layout->stateInfo;
        layout->courseState.exitFlag = 0;
        PokeathlonCourse_SetStateField07(course, 10);
    } else if (!ov96_021E5F24(course)) {
        u8 *dest;
        u8 *src;
        u32 n;
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[1], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
        dest = ov96_021E8A20(&layout->packets[2]);
        src = ov96_021E8A20(&layout->packets[0]);
        n = sizeof(PokeathlonCoursePacket);
        do {
            *dest++ = *src++;
        } while (--n);
    } else {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[0], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6BC0(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->event->exit(course, 0);
    if (layout->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(course, 11);
    return 0;
}

u32 ov96_021E6BEC(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->event = &ov96_0221A8E4;
    layout->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA6C, &layout->eventStateInfo);
    PokeathlonCourse_SetStateField07(course, 12);
    return 0;
}

u32 ov96_021E6C20(PokeathlonCourseData *course) {
    if (PokeathlonCourse_RunSubStateLoop(course)) {
        PokeathlonCourse_SetStateTransitionType(course, 7);
        PokeathlonCourse_SetStateField07(course, 13);
    }
    return 0;
}

#pragma force_active on

u32 ov96_021E6C40(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (layout->eventStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(course, 14);
        return 0;
    }
    if (layout->courseState.argsPtr == &layout->stateInfo) {
        layout->courseState.argsPtr = &layout->eventStateInfo;
        layout->courseState.exitFlag = 0;
    }
    if (((PokeathlonCourseStepFunc *)layout->eventStateInfo.ptr)[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
        PokeathlonCourse_SetField1F4(course, 0);
        layout->courseState.argsPtr = &layout->stateInfo;
        layout->courseState.exitFlag = 0;
        PokeathlonCourse_SetStateField07(course, 14);
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6CB8(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->event->exit(course, 0);
    if (layout->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(course, 15);
    return 0;
}

#pragma force_active on

// Sets up the event of the current round.
u32 PokeathlonCourse_State_StartRound(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 event = layout->roundEvents[layout->round];
    layout->startSpriteCount = 0;
    layout->recordScore = 0xFFFF;
    layout->timeUpSeconds = 0;
    layout->finishCallback = NULL;
    PokeathlonCourse_ResetDataCopyArea(course);
    layout->event = ov96_0221DA28[event];
    layout->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(_0221DA00[event], &layout->eventStateInfo);
    PokeathlonCourse_SetStateField07(course, 16);
    return 0;
}

u32 PokeathlonCourse_State_WaitRoundStart(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (PokeathlonCourse_RunSubStateLoop(course)) {
        PokeathlonCoursePacket *packets = (PokeathlonCoursePacket *)PokeathlonCourse_GetDataCopyArea(course);
        packets[0].unk24 = 0;
        packets[1].unk24 = 1;
        PokeathlonCourseLink_SetCheckSize(PokeathlonCourse_GetSystem(course), TRUE);
        PokeathlonCourse_SetStateTransitionType(course, 8);
        if (layout->round == layout->rounds - 1) {
            Sound_SetScene(0);
            Sound_SetSceneAndPlayBGM(24, SEQ_GS_PT_GAMEF, 0);
        } else {
            Sound_SetScene(0);
            Sound_SetSceneAndPlayBGM(24, SEQ_GS_PT_GAME, 0);
        }
        Sound_SetPlayerPause(7, 1);
        PokeathlonCourse_SetStateField07(course, 17);
        if (!GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON)) {
            GF_AssertFail();
        }
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6DE8(PokeathlonCourseData *course) {
    if (course->field_3A4[3] != 0) {
        if (!ov96_021E5F24(course)) {
            if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_EVENT_DATA, (void *)course->field_3A4[0], course->field_3A4[2], course->system)) {
                PokeathlonCourse_SetStateField07(course, 38);
            }
        } else {
            PokeathlonCourse_SetStateField07(course, 38);
        }
    } else {
        PokeathlonCourse_SetStateField07(course, 18);
    }
    return 0;
}

#pragma force_active on

// Runs the event's state functions; the host sends its packet every frame, the others theirs.
u32 PokeathlonCourse_State_RunEvent(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (!layout->eventStateInfo.ptr) {
        PokeathlonCourse_SetStateField07(course, 24);
        return 0;
    }
    if (layout->courseState.argsPtr == &layout->stateInfo) {
        layout->courseState.argsPtr = &layout->eventStateInfo;
        layout->courseState.exitFlag = 0;
    }
    if (((PokeathlonCourseStepFunc *)layout->eventStateInfo.ptr)[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
        PokeathlonCourse_SetField1F4(course, 0);
        layout->courseState.argsPtr = &layout->stateInfo;
        layout->courseState.exitFlag = 0;
        PokeathlonCourse_SetStateTransitionType(course, 16);
        PokeathlonCourse_SetStateField07(course, 19);
    } else if (!ov96_021E5F24(course)) {
        u8 *dest;
        u8 *src;
        u32 n;
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[1], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
        dest = ov96_021E8A20(&layout->packets[2]);
        src = ov96_021E8A20(&layout->packets[0]);
        n = sizeof(PokeathlonCoursePacket);
        do {
            *dest++ = *src++;
        } while (--n);
    } else {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[0], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6F18(PokeathlonCourseData *course) {
    PokeathlonCourse_ResetDataCopyArea(course);
    PokeathlonCourse_SetStateField07(course, 20);
    return 0;
}

#pragma force_active on

u32 ov96_021E6F2C(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    BOOL done;
    if (layout->event->unk10 != 0) {
        PokeathlonCoursePoint *point = (PokeathlonCoursePoint *)ov96_021E8A20(&layout->packets[0]);
        int netId = ov96_021E5F24(course);
        point->x = layout->eventResults[netId].unk0;
        point->y = layout->eventResults[netId].unk2;
        done = PokeathlonCourse_TrySendCommand(POKEATHLON_COURSE_CMD_EVENT_RESULT, &layout->packets[0], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
        if (done && !ov96_021E5F24(course)) {
            int size = PokeathlonCourse_GetUnkConstant4(course);
            int i = PokeathlonCourse_GetParticipantCount(course);
            for (; i < 4; i++) {
                memcpy((u8 *)ov96_021E8A20(&layout->packets[1]) + i * size, &layout->eventResults[i], size);
            }
        }
    } else if (!ov96_021E5F24(course)) {
        done = PokeathlonCourse_SendEventResults(course);
    } else {
        done = TRUE;
    }
    if (done) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

#pragma force_active reset

u32 ov96_021E6FFC(PokeathlonCourseData *course) {
    u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
    void *size = PokeathlonCourseCmd_PacketSize();
    void *link = PokeathlonCourse_GetSystem(course);
    if (PokeathlonCourse_TrySendCommand(POKEATHLON_COURSE_CMD_EVENT_RESULTS, dataCopy + sizeof(PokeathlonCoursePacket), size, (int)link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

// Takes over the event results the host sent and stores the round's points.
u32 PokeathlonCourse_State_EndRound(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u16 *src = (void *)ov96_021E8A20(&layout->packets[6]);
    u16 *dest = layout->receivedResults;
    u32 n = 18;
    do {
        *dest++ = *src++;
    } while (--n != 0);
    if (layout->event->onResults != NULL) {
        layout->event->onResults(course, 0);
    }
    PokeathlonCourse_StoreRoundResults(course, (u8)layout->round);
    PokeathlonCourse_SetStateField07(course, 23);
    return 0;
}

u32 ov96_021E7080(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (!GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON)) {
        GF_AssertFail();
    }
    layout->event->exit(course, 0);
    if (layout->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    if ((s32)layout->round < layout->rounds) {
        if (layout->event->onResults != NULL) {
            PokeathlonCourse_SetStateField07(course, 25);
        } else {
            PokeathlonCourse_SetStateField07(course, 24);
        }
    } else {
        layout->round++;
        PokeathlonCourse_SetStateField07(course, 29);
    }
    return 0;
}

u32 PokeathlonCourse_State_NextRound(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->round++;
    if ((s32)layout->round >= layout->rounds) {
        PokeathlonCourse_SetStateField07(course, 29);
    } else {
        PokeathlonCourse_SetStateField07(course, 11);
    }
    return 0;
}

u32 ov96_021E711C(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->event = &ov96_0221A86C;
    layout->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DC24, &layout->eventStateInfo);
    PokeathlonCourse_SetStateField07(course, 26);
    return 0;
}

u32 ov96_021E7150(PokeathlonCourseData *course) {
    if (PokeathlonCourse_RunSubStateLoop(course)) {
        u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
        dataCopy[0x24] = 0;
        dataCopy[0x4C] = 1;
        PokeathlonCourseLink_SetCheckSize(PokeathlonCourse_GetSystem(course), TRUE);
        PokeathlonCourse_SetStateTransitionType(course, 11);
        PokeathlonCourse_SetStateField07(course, 27);
    }
    return 0;
}

u32 ov96_021E7190(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourseStepFunc *steps;
    if (layout->eventStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(course, 28);
        return 0;
    }
    if (layout->courseState.argsPtr == &layout->stateInfo) {
        layout->courseState.argsPtr = &layout->eventStateInfo;
        layout->courseState.exitFlag = 0;
    }
    steps = layout->eventStateInfo.ptr;
    if (steps[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
        PokeathlonCourse_SetField1F4(course, 0);
        layout->courseState.argsPtr = &layout->stateInfo;
        layout->courseState.exitFlag = 0;
        PokeathlonCourse_SetStateField07(course, 28);
    } else if (!ov96_021E5F24(course)) {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[1], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
        u8 *dest = (void *)ov96_021E8A20(&layout->packets[2]);
        u8 *src = (void *)ov96_021E8A20(&layout->packets[0]);
        u32 n = sizeof(PokeathlonCoursePacket);
        do {
            *dest++ = *src++;
        } while (--n != 0);
    } else {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[0], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
    }
    return 0;
}

u32 ov96_021E7268(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    layout->event->exit(course, 0);
    if (layout->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(course, 24);
    return 0;
}

u32 ov96_021E7294(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourse_ResetDataCopyArea(course);
    layout->event = &ov96_0221A844;
    layout->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA5C, &layout->eventStateInfo);
    PokeathlonCourse_SetStateField07(course, 32);
    return 0;
}

u32 ov96_021E72CC(PokeathlonCourseData *course) {
    if (PokeathlonCourse_RunSubStateLoop(course)) {
        u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
        dataCopy[0x24] = 0;
        dataCopy[0x4C] = 1;
        PokeathlonCourseLink_SetCheckSize(PokeathlonCourse_GetSystem(course), TRUE);
        PokeathlonCourse_SetStateTransitionType(course, 12);
        PokeathlonCourse_SetStateField07(course, 33);
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(25, SEQ_GS_PT_END, 0);
    }
    return 0;
}

u32 ov96_021E7320(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourseStepFunc *steps;
    if (layout->eventStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(course, 34);
        return 0;
    }
    if (layout->courseState.argsPtr == &layout->stateInfo) {
        layout->courseState.argsPtr = &layout->eventStateInfo;
        layout->courseState.exitFlag = 0;
    }
    steps = layout->eventStateInfo.ptr;
    if (steps[layout->eventStateInfo.stateIndex](course, &layout->eventStateInfo.stateArgsBase)) {
        PokeathlonCourse_SetField1F4(course, 0);
        layout->courseState.argsPtr = &layout->stateInfo;
        layout->courseState.exitFlag = 0;
        PokeathlonCourse_SetStateField07(course, 34);
    } else if (!ov96_021E5F24(course)) {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[1], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
        u8 *dest = (void *)ov96_021E8A20(&layout->packets[2]);
        u8 *src = (void *)ov96_021E8A20(&layout->packets[0]);
        u32 n = sizeof(PokeathlonCoursePacket);
        do {
            *dest++ = *src++;
        } while (--n != 0);
    } else {
        PokeathlonCourseLink_SendCommand(POKEATHLON_COURSE_CMD_PACKET, &layout->packets[0], PokeathlonCourseCmd_PacketSize(), (int)layout->link);
    }
    return 0;
}

// Course over: creates the Apricorn juice record of the course (heapAllocPtr2) and the buffer for the records of
// every player (heapAllocPtr3).
u32 PokeathlonCourse_State_CreateJuiceRecords(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    SaveApricornBox *apricornBox;
    PlayerProfile *profile;
    Pokeathlon_UnkSubStruct_B00 *record;
    u32 size;
    layout->event->exit(course, 0);
    if (layout->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    apricornBox = Save_ApricornBox_Get(layout->args->saveData);
    profile = Save_PlayerData_GetProfile(layout->args->saveData);
    record = PokeathlonSave_GetUnkB00(Save_Pokeathlon_Get(layout->args->saveData));
    size = ApricornJuiceRecord_sizeof() * 4;
    layout->heapAllocPtr3 = Heap_AllocAtEnd(layout->heapId, size);
    memset(layout->heapAllocPtr3, 0, size);
    layout->heapAllocPtr2 = ApricornRecord_New(apricornBox, profile, record->unk70, layout->heapId);
    if (layout->args->mode == 1) {
        PokeathlonCourse_SetStateTransitionType(course, 14);
        PokeathlonCourse_SetStateField07(course, 35);
    } else {
        PokeathlonCourse_SetStateField07(course, 37);
    }
    return 0;
}

u32 PokeathlonCourse_State_SendMonCounters(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    ov96_021E5F24(course);
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_MON_COUNTERS, layout->localCounters, PokeathlonCourseCmd_MonCountersSize(), layout->link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

u32 PokeathlonCourse_State_SendResults(PokeathlonCourseData *course) {
    void *results = PokeathlonCourse_GetFieldData_AtIndex(course, 0);
    int size = PokeathlonCourseCmd_ResultsSize();
    void *link = PokeathlonCourse_GetSystem(course);
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_RESULTS, results, size, link)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

u32 PokeathlonCourse_State_SendJuiceRecord(PokeathlonCourseData *course) {
    u32 size = ApricornJuiceRecord_sizeof();
    if (PokeathlonCourseLink_SendLargeCommand(POKEATHLON_COURSE_CMD_JUICE_RECORD, course->heapAllocPtr2, size, course->system)) {
        PokeathlonCourse_SetStateField07(course, 38);
    }
    return 0;
}

u32 PokeathlonCourse_State_StoreJuiceRecords(PokeathlonCourseData *course) {
    SaveApricornBox *apricornBox = Save_ApricornBox_Get(course->args->saveData);
    u32 count = PokeathlonCourse_GetParticipantCount(course);
    u32 netId = Comm_GetLocalNetId();
    ApricornBox_StoreJuiceRecords(apricornBox, course->heapAllocPtr3, count, netId);
    if (!GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON)) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(course, 37);
    return 0;
}

u32 PokeathlonCourse_State_Exit(PokeathlonCourseData *course) {
    if (!GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON)) {
        GF_AssertFail();
    }
    if (course->args->shouldFreeHeap == FALSE) {
        PokeathlonCourse_SaveResults(course);
    }
    return 1;
}

int ov96_021E75B8(void) {
    return 0;
}

void PokeathlonCourse_VBlankCallback(void *bgConfig) {
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(bgConfig);
    GF_RunVramTransferTasks();
    ((vu32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

// Host: sends every player's event result to the others.
BOOL PokeathlonCourse_SendEventResults(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 *dataCopy = PokeathlonCourse_GetDataCopyArea(course);
    int i;
    u8 *src;
    u32 size;
    void *packetSize;
    void *link;

    if (ov96_021E5F24(course) != 0) {
        GF_AssertFail();
        return FALSE;
    }
    src = (u8 *)layout->eventResults;
    for (i = 0; i < 4; i++, src += sizeof(PokeathlonCourseEventResult)) {
        size = PokeathlonCourse_GetUnkConstant4(course);
        memcpy((u8 *)(void *)ov96_021E8A20(dataCopy + sizeof(PokeathlonCoursePacket)) + i * size, src, size);
    }
    packetSize = PokeathlonCourseCmd_PacketSize();
    link = PokeathlonCourse_GetSystem(course);
    return PokeathlonCourse_TrySendCommand(POKEATHLON_COURSE_CMD_EVENT_RESULTS, dataCopy + sizeof(PokeathlonCoursePacket), packetSize, (int)link);
}

#pragma force_active on

// Stores the points and scores of the round from the score list, and whether the player won it or came last.
void PokeathlonCourse_StoreRoundResults(PokeathlonCourseData *course, int round) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 netId = ov96_021E5F24(course);
    u8 i;
    PokeathlonCourseScore *entry = NULL;
    for (i = 0; i < 4; i++) {
        PokeathlonCourseScore *score = PokeathlonCourseScoreList_GetEntry(layout->scores, i);
        layout->results.eventPoints[i][round] = score->points;
        layout->results.eventScores[i][round] = score->score;
    }
    for (i = 0; i < 4; i++) {
        entry = PokeathlonCourseScoreList_GetRanked(layout->scores, i);
        if (entry->participant == netId) {
            break;
        }
    }
    if (i == 4) {
        GF_AssertFail();
        return;
    }
    if (entry->rank == 0) {
        layout->results.wonRound[round] = TRUE;
        layout->results.lastInRound[round] = FALSE;
        return;
    }
    layout->results.wonRound[round] = FALSE;
    {
        PokeathlonCourseScore *last = PokeathlonCourseScoreList_GetRanked(layout->scores, 3);
        u8 rank = last->rank;
        if (rank == entry->rank) {
            layout->results.lastInRound[round] = TRUE;
        } else {
            layout->results.lastInRound[round] = FALSE;
        }
    }
}

#pragma force_active reset

// Saves the results of the course: athlete points, event and course records, medals, statistics.
void PokeathlonCourse_SaveResults(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonSave *pokeathlon = Save_Pokeathlon_Get(layout->args->saveData);
    PokeathlonCourseResults *results = (PokeathlonCourseResults *)PokeathlonCourse_GetFieldData(course);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(layout->args->saveData);
    u16 points;
    void *records;

    if (results->wonCourse) {
        PokeathlonCourse_SetCourseMedals(course);
        if (!Save_VarsFlags_CheckFlagInArray(varsFlags, FLAG_UNK_0F0) && PokeathlonCourse_HasAllMedalsOfAMon(course)) {
            Save_VarsFlags_SetFlagInArray(varsFlags, FLAG_UNK_0F0);
        }
    }
    points = results->totalPoints;
    if (layout->args->course == 10) { // the link course with random events
        points *= 2;
    }
    PokeathlonSave_AddAthletePoints(pokeathlon, points);
    if (layout->args->mode == 1) {
        records = PokeathlonSave_GetRecordsLink2(pokeathlon);
        PokeathlonCourse_SaveLinkEventRecords(course, records);
    } else {
        records = PokeathlonSave_GetRecordsSolo(pokeathlon);
        PokeathlonCourse_SaveSoloEventBests(course, records, PokeathlonSave_GetUnkAEC(pokeathlon));
        if (results->wonCourse) {
            PokeathlonCourse_SaveCourseRecord(course, PokeathlonSave_dummy2(pokeathlon));
            if (!Save_VarsFlags_CheckFlagInArray(varsFlags, FLAG_UNK_0EF) && PokeathlonCourse_HasAllCourseRecords(PokeathlonSave_dummy2(pokeathlon))) {
                Save_VarsFlags_SetFlagInArray(varsFlags, FLAG_UNK_0EF);
            }
        }
        PokeathlonCourse_SaveSoloEventRecords(course, (PokeathlonCourseEventRecord *)PokeathlonSave_GetRecordsSolo2(pokeathlon));
    }
    PokeathlonCourse_SaveStatistics(course, PokeathlonSave_GetAgainUnkB00(pokeathlon));
    if (layout->args->mode == 0 && !Save_VarsFlags_CheckFlagInArray(varsFlags, FLAG_UNK_0F1) && PokeathlonCourse_BeatsAllEventTargets(course)) {
        Save_VarsFlags_SetFlagInArray(varsFlags, FLAG_UNK_0F1);
    }
    layout->args->totalPoints = results->totalPoints;
    layout->args->points = points;
    layout->args->rank = results->rank;
}

// Saves the course's best total (after a win) with the round points and the three Pokemon.
void PokeathlonCourse_SaveCourseRecord(PokeathlonCourseData *course, PokeathlonSave *pokeathlon) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 courseId = layout->args->course;
    PokeathlonCourseBestRecord *record = (PokeathlonCourseBestRecord *)pokeathlon + courseId;
    u16 total = layout->results.totalPoints;
    int i;
    PokeathlonCourseMon *mon;
    PokeathlonCourseSavedMon *savedMon;

    if (total <= record->totalPoints) {
        return;
    }
    record->totalPoints = total;
    if (record->totalPoints > 999) {
        record->totalPoints = 999;
    }
    for (i = 0; i < 3; i++) {
        record->roundPoints[i] = layout->results.eventPoints[0][i];
        if (record->roundPoints[i] > 999) {
            record->roundPoints[i] = 999;
        }
    }
    mon = (PokeathlonCourseMon *)PokeathlonCourse_GetParticipantUnk04(course, ov96_021E5F24(course));
    i = 0;
    savedMon = record->mons;
    // record: moved on by one Pokemon per step, mons[0] is the current one (needed to match)
    while (i < 3) {
        savedMon->species = mon->species;
        savedMon->form = mon->form;
        savedMon->gender = mon->gender;
        savedMon->isShiny = mon->isShiny;
        record->mons[0].personality = mon->personality;
        mon++;
        savedMon++;
        record = (PokeathlonCourseBestRecord *)((u8 *)record + sizeof(PokeathlonCourseSavedMon));
        i++;
    }
}

#pragma force_active on

// Solo: counts the plays of each event and adds the player's result to the event's top 5.
void PokeathlonCourse_SaveSoloEventRecords(PokeathlonCourseData *course, PokeathlonCourseEventRecord *records) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourseRankEntry entry;
    PokeathlonCourseRankEntry top[5];
    for (u8 i = 0; i < 3; i++) {
        u8 event = layout->roundEvents[i];
        PokeathlonCourseEventRecord *record = &records[event];
        if (record->timesPlayed < 9999999) {
            record->timesPlayed++;
        }
        int order = _0221A7D8[event];
        for (u8 j = 0; j < 5; j++) {
            top[j].score = record->top[j].score;
            for (u8 k = 0; k < 3; k++) {
                top[j].mons[k] = record->top[j].mons[k];
            }
        }
        entry.score = layout->results.eventScores[(u8)ov96_021E5F24(course)][i];
        u8 netId = ov96_021E5F24(course);
        PokeathlonCourseMon *mons = layout->participants[netId].mons;
        for (u8 j = 0; j < 3; j++) {
            entry.mons[j] = (mons[j].form << 10) + mons[j].species;
        }
        PokeathlonCourse_InsertRankEntry(order, &entry, top);
        PokeathlonCourse_CopySoloRanking(course, record, top);
    }
}

static inline void CopyRankEntryTrainer(PokeathlonCourseRankEntry *dest, PokeathlonCourseLinkEventRecord *record, u8 j) {
    u8 k;
    dest->trainerId = record->trainers[j].trainerId;
    for (k = 0; k < 8; k++) {
        dest->name[k] = record->trainers[j].name[k];
    }
    dest->language = record->trainers[j].language;
}

// Link: counts the plays of each event and adds every player's result (with their name) to the event's top 5.
void PokeathlonCourse_SaveLinkEventRecords(PokeathlonCourseData *course, PokeathlonCourseLinkEventRecord *records) {
    // (a struct: its stack layout is needed to match)
    struct {
        PokeathlonCourseLayout *roundBase; // the course pointer moved on by the round, eventScores[x][0] is the round's
        s32 firstNetId;
    } loop;

    loop.firstNetId = (u8)ov96_021E5F24(course);
    u8 i, j, k, n;
    PokeathlonCourseRankEntry top[5], entry;
    for (i = 0; i < 4; i++) {
        // (byte offset: needed to match the stack layout)
        u8 event = (u8) * (u32 *)((u8 *)((PokeathlonCourseLayout *)course)->roundEvents + 4 * i);
        u32 order;
        PokeathlonCourseLinkEventRecord *record = &records[event];
        if (record->timesPlayed < 9999999) {
            record->timesPlayed++;
        }
        order = _0221A7D8[event];
        for (j = 0; j < 5; j++) {
            top[j].score = record->top[j].score;
            for (k = 0; k < 3; k++) {
                top[j].mons[k] = record->top[j].mons[k];
            }
            CopyRankEntryTrainer(&top[j], record, j);
        }
        n = 0;
        {
            loop.roundBase = (PokeathlonCourseLayout *)((u8 *)course + i * sizeof(u16));
            for (; n < 4; n++) {
                u8 netId = (loop.firstNetId + n) % 4;
                PokeathlonCourseMon *mons;
                PlayerProfile *profile;
                entry.score = loop.roundBase->results.eventScores[netId][0];
                mons = ((PokeathlonCourseLayout *)course)->participants[netId].mons;
                for (k = 0; k < 3; k++) {
                    entry.mons[k] = (mons[k].form << 10) + mons[k].species;
                }
                profile = PokeathlonCourse_GetPlayerProfileFromData(course, netId);
                entry.trainerId = PlayerProfile_GetTrainerID(profile);
                {
                    const u16 *name = PlayerProfile_GetNamePtr(profile);
                    for (k = 0; k < 8; k++) {
                        entry.name[k] = name[k];
                    }
                }
                entry.language = PlayerProfile_GetLanguage(profile);
                PokeathlonCourse_InsertRankEntry(order, &entry, top);
            }
        }
        PokeathlonCourse_CopyLinkRanking(course, record, top);
    }
}

#pragma force_active reset

// Solo: keeps the player's points of each event whose result beats the event's record (bests[event]).
void PokeathlonCourse_SaveSoloEventBests(PokeathlonCourseData *course, Pokeathlon_RecordsSolo *records, u16 *bests) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    u8 event;

    for (i = 0; i < 3; i++) {
        event = (u8)layout->roundEvents[i];
        if (PokeathlonCourse_BeatsRecord(records[event].entries[0].unk_0, layout->results.eventScores[0][i], _0221A7D8[event])) {
            bests[event] = layout->results.eventPoints[0][i];
        }
    }
}

#pragma force_active on

// Inserts entry into the sorted top 5 if it beats the last one.
void PokeathlonCourse_InsertRankEntry(u32 order, PokeathlonCourseRankEntry *entry, PokeathlonCourseRankEntry *top) {
    int i;
    PokeathlonCourseRankEntry *p;
    PokeathlonCourseRankEntry temp;
    if (PokeathlonCourse_BeatsRecord(top[4].score, entry->score, order)) {
        top[4] = *entry;
        i = 4;
        p = &top[4];
        goto loop;
loop: {
    if (!PokeathlonCourse_BeatsRecord(p[-1].score, p->score, order)) {
        goto finished;
    }
    temp = p[-1];
    p[-1] = *p;
    *p = temp;
    i--;
    p--;
    if (i > 0) {
        goto loop;
    }
}
    }
finished:;
}

#pragma force_active reset

void PokeathlonCourse_CopySoloRanking(PokeathlonCourseData *course, PokeathlonCourseEventRecord *record, PokeathlonCourseRankEntry *top) {
    int i;
    int j;
    ov96_021E5F24(course);
    for (i = 0; i < 5; i++) {
        record->top[i].score = top[i].score;
        for (j = 0; j < 3; j++) {
            record->top[i].mons[j] = top[i].mons[j];
        }
    }
}

#pragma force_active on

void PokeathlonCourse_CopyLinkRanking(PokeathlonCourseData *course, PokeathlonCourseLinkEventRecord *record, PokeathlonCourseRankEntry *top) {
    int i = 0;
    int j;
    for (; i < 5; i++) {
        record->top[i].score = top[i].score;
        for (j = 0; j < 3; j++) {
            record->top[i].mons[j] = top[i].mons[j];
        }
        record->trainers[i].trainerId = top[i].trainerId;
        {
            int k = 0;
            for (; k < 8; k++) {
                record->trainers[i].name[k] = top[i].name[k];
            }
        }
        record->trainers[i].language = top[i].language;
    }
}

#pragma force_active reset

// Whether score beats record (0xFFFF: no record yet) in the event's order.
BOOL PokeathlonCourse_BeatsRecord(int record, int score, u32 order) {
    if (record == 0xFFFF) {
        return TRUE;
    }
    return PokeathlonCourse_BeatsRecordInOrder(record, score, order);
}

// Order 0: higher scores are better, order 1: lower ones (times); 0xFFFF is no record.
BOOL PokeathlonCourse_BeatsRecordInOrder(int record, int score, u32 order) {
    BOOL better = FALSE;
    if (order == 0) {
        if (record == 0xFFFF) {
            better = TRUE;
        } else if (record < score) {
            better = TRUE;
        }
    } else if (order == 1) {
        if (record == 0xFFFF) {
            better = TRUE;
        } else if (record > score) {
            better = TRUE;
        }
    } else {
        GF_AssertFail();
    }
    return better;
}

#pragma force_active on

// pokeathlon.h declares the counter as a Pokeathlon_UnkSubStruct_B00, whose first counter it is.
static inline void AddToStatistic(u32 amount, u32 *counter) {
    PokeathlonCourse_AddCapped(amount, 9999999, (Pokeathlon_UnkSubStruct_B00 *)counter);
}

static inline void AddCountersToStatistics(const u32 *counters, Pokeathlon_UnkSubStruct_B00 *statistics) {
    AddToStatistic(counters[0], &statistics->unk18);
    AddToStatistic(counters[1], &statistics->unk1C);
    AddToStatistic(counters[2], &statistics->unk20);
    AddToStatistic(counters[3], &statistics->unk24);
    AddToStatistic(counters[4], &statistics->unk28);
    AddToStatistic(counters[5], &statistics->unk2C);
    AddToStatistic(counters[6], &statistics->unk30);
    AddToStatistic(counters[7], &statistics->unk34);
}

// Adds the course to the saved statistics (courses played / won, rounds won, the Pokemon's counters).
void PokeathlonCourse_SaveStatistics(PokeathlonCourseData *course, Pokeathlon_UnkSubStruct_B00 *statistics) {
    u32 netId;
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonCourseResults *results = &layout->results;
    u8 i;
    u8 j;
    u32 *counters;
    netId = (u8)ov96_021E5F24(course);
    if (layout->args->mode == 1) {
        AddToStatistic(1, &statistics->unk38);
        if (results->wonCourse) {
            AddToStatistic(1, &statistics->unk3C);
            PokeathlonCourse_SaveFriendshipStatues(course);
        } else if (results->unkBit1) {
            AddToStatistic(1, &statistics->unk40);
        }
    } else {
        AddToStatistic(1, &statistics->unk4);
        if (results->wonCourse) {
            AddToStatistic(1, &statistics->unk8);
            PokeathlonCourse_SaveFriendshipStatues(course);
        } else if (results->unkBit1) {
            AddToStatistic(1, &statistics->unkC);
        }
    }
    for (i = 0; i < layout->rounds; i++) {
        if (results->wonRound[i]) {
            AddToStatistic(1, &statistics->unk44[(u8)layout->roundEvents[i]]);
        } else if (results->lastInRound[i]) {
            AddToStatistic(1, &statistics->unk6C);
        }
    }
    for (j = 0; j < 3; j++) {
        counters = results->counters[netId][j];
        AddCountersToStatistics(counters, statistics);
    }
    AddToStatistic(results->unk180, &statistics->unk10);
    AddToStatistic(results->unk184, &statistics->unk14);
}

#pragma force_active reset

// The player's three Pokemon become the statues of the Friendship Room.
void PokeathlonCourse_SaveFriendshipStatues(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    u8 i;
    PokeathlonSave_FriendshipRecords *records = Save_Pokeathlon_FriendshipRecords_Get(layout->args->saveData);
    u8 netId = (u8)ov96_021E5F24(course);
    PokeathlonCourseMon *mons = layout->participants[netId].mons;
    i = 0;
    do {
        records->friendshipRoomStatues[i].species = mons[i].species;
        records->friendshipRoomStatues[i].form = (u8)mons[i].form;
        records->friendshipRoomStatues[i].gender = mons[i].gender;
        i++;
    } while (i < 3);
}

void PokeathlonCourse_AddCapped(s32 frameCount, u32 maxValue, Pokeathlon_UnkSubStruct_B00 *result) {
    if (result->unk0 + frameCount > maxValue) {
        result->unk0 = maxValue;
    } else {
        result->unk0 += frameCount;
    }
}

// Gives the player's Pokemon the medal of the course.
void PokeathlonCourse_SetCourseMedals(PokeathlonCourseData *course) {
    switch (((PokeathlonCourseLayout *)course)->args->course) {
    case 0:
        PokeathlonCourse_SetMonMedals(course, 1);
        break;
    case 1:
        PokeathlonCourse_SetMonMedals(course, 2);
        break;
    case 2:
        PokeathlonCourse_SetMonMedals(course, 4);
        break;
    case 3:
        PokeathlonCourse_SetMonMedals(course, 8);
        break;
    case 4:
        PokeathlonCourse_SetMonMedals(course, 0x10);
        break;
    case 5:
        PokeathlonCourse_SetMonMedals(course, 0x11);
        break;
    case 6:
        PokeathlonCourse_SetMonMedals(course, 0x14);
        break;
    case 7:
        PokeathlonCourse_SetMonMedals(course, 6);
        break;
    case 8:
        PokeathlonCourse_SetMonMedals(course, 0xA);
        break;
    case 9:
        PokeathlonCourse_SetMonMedals(course, 9);
        break;
    }
}

void PokeathlonCourse_SetMonMedals(PokeathlonCourseData *course, u32 medals) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonSave *pokeathlon = Save_Pokeathlon_Get(layout->args->saveData);
    u8 i = 0;
    do {
        PokeathlonSave_SetUnkDC_AtIndex(pokeathlon, medals, layout->localParticipant.mons[i].species);
        i++;
    } while (i < 3);
}

// Whether every one of the first 5 courses has a saved record.
BOOL PokeathlonCourse_HasAllCourseRecords(PokeathlonSave *pokeathlon) {
    PokeathlonCourseBestRecord *records = (PokeathlonCourseBestRecord *)pokeathlon;
    u8 i = 0;
    do {
        if (records[i].totalPoints == 0) {
            return FALSE;
        }
        i++;
    } while (i < 5);
    return TRUE;
}

// Whether one of the player's Pokemon has all 5 medals.
BOOL PokeathlonCourse_HasAllMedalsOfAMon(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonSave *pokeathlon = Save_Pokeathlon_Get(layout->args->saveData);
    u8 i = 0;
    do {
        u32 index = layout->localParticipant.mons[i].species - 1;
        u8 *medals = PokeathlonSave_GetUnkDC(pokeathlon);
        if (medals[index] == 0x1F) {
            return TRUE;
        }
        i++;
    } while (i < 3);
    return FALSE;
}

// Whether the solo record of every event beats the event's target.
BOOL PokeathlonCourse_BeatsAllEventTargets(PokeathlonCourseData *course) {
    u8 i;
    Pokeathlon_RecordsSolo *records = PokeathlonSave_GetRecordsSolo2(Save_Pokeathlon_Get(((PokeathlonCourseLayout *)course)->args->saveData));
    i = 0;
    do {
        u16 record = records[i].entries[0].unk_0;
        if (record == 0xFFFF) {
            return FALSE;
        }
        if (!PokeathlonCourse_BeatsRecordInOrder(ov96_0221A894[i], record, _0221A7D8[i])) {
            return FALSE;
        }
        i++;
    } while (i < 10);
    return TRUE;
}

// The events of the course's rounds (course 10 picks one of 10 sets at random).
void PokeathlonCourse_ChooseEvents(PokeathlonCourseArgsLayout *args, u32 rounds, u8 *eventIds) {
    u8 courseId;
    BOOL valid = FALSE;
    eventIds[0] = 10;
    courseId = args->course;
    if (args->mode == 1) {
        switch (courseId) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            valid = TRUE;
            break;
        default:
            GF_AssertFail();
            break;
        }
    } else {
        switch (courseId) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            valid = TRUE;
            break;
        default:
            GF_AssertFail();
            break;
        }
    }
    if (valid) {
        u8 i;
        if (courseId == 10) {
            u8 set = (u8)((s32)LCRandom() % 10);
            for (i = 0; i < rounds; i++) {
                eventIds[i] = ov96_0221A95C[set][i];
            }
        } else {
            for (i = 0; i < rounds; i++) {
                eventIds[i] = ov96_0221A934[courseId][i];
            }
        }
    }
}

// Keeps a marker over its Pokemon, shown while the Pokemon is.
void PokeathlonCourse_MarkerTask(SysTask *task, void *data) {
    PokeathlonCourseMarker *marker = data;
    s32 x;
    s32 y;
    VecFx32 position;
    PokeathlonCourseMonObject_GetPosition(marker->object, &x, &y);
    Sprite_SetDrawFlag(marker->sprite, TRUE);
    position.z = 0;
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(marker->sprite, &position);
    if (PokeathlonCourseObject_IsDrawn(marker->object)) {
        Sprite_SetDrawFlag(marker->sprite, TRUE);
    } else {
        Sprite_SetDrawFlag(marker->sprite, FALSE);
    }
}

#pragma force_active on

// Adds to a counter of a participant's Pokemon (counter 0: the course's own count). On the host the counters
// of every participant are kept; the others keep only their own, which they send at the end.
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u32 participant, u32 mon, int counter, int amount) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (counter == 0) {
        if (participant != ov96_021E5F24(course)) {
            GF_AssertFail();
        }
        if (amount != 1) {
            GF_AssertFail();
        }
        if (layout->results.unk184 < 9999) {
            layout->results.unk184++;
        }
    } else {
        if (mon >= 3) {
            GF_AssertFail();
        }
        if (layout->event->unk10 == 0) {
            if (ov96_021E5F24(course) != 0) {
                GF_AssertFail();
            }
            PokeathlonCourse_AddToCounter(counter, (u8)amount, layout->results.counters[participant][mon]);
        } else if (ov96_021E5F24(course) == 0) {
            if (participant >= PokeathlonCourse_GetParticipantCount(course)) {
                PokeathlonCourse_AddToCounter(counter, (u8)amount, layout->results.counters[participant][mon]);
            } else {
                if (participant != ov96_021E5F24(course)) {
                    GF_AssertFail();
                }
                PokeathlonCourse_AddToCounter(counter, (u8)amount, layout->localCounters[mon]);
            }
        } else {
            if (participant != ov96_021E5F24(course)) {
                GF_AssertFail();
            }
            PokeathlonCourse_AddToCounter(counter, (u8)amount, layout->localCounters[mon]);
        }
    }
}

#pragma force_active reset

void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score) {
    ((PokeathlonCourseLayout *)course)->recordScore = score;
}

void PokeathlonCourse_SetFinishCallback(PokeathlonCourseData *course, PokeathlonCourseFinishFunc callback) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    if (layout->finishCallback != NULL) {
        GF_AssertFail();
    }
    layout->finishCallback = callback;
}

#pragma force_active on

void PokeathlonCourse_AddToCounter(u32 counter, u32 amount, u32 *counters) {
    u32 *p;
    switch (counter) {
    case 1:
        p = counters;
        break;
    case 2:
        p = counters + 1;
        break;
    case 3:
        p = counters + 2;
        break;
    case 4:
        p = counters + 3;
        break;
    case 5:
        p = counters + 4;
        break;
    case 6:
        p = counters + 5;
        break;
    case 7:
        p = counters + 6;
        break;
    case 8:
        p = counters + 7;
        break;
    default:
        GF_AssertFail();
        return;
    }
    *p += amount;
    if (*p > 9999) {
        *p = 9999;
    }
}

#pragma force_active reset

// Whether the event result (recordScore) beats the saved record of the event.
BOOL PokeathlonCourse_IsNewRecord(PokeathlonCourseData *course) {
    PokeathlonCourseLayout *layout = (PokeathlonCourseLayout *)course;
    PokeathlonSave *pokeathlon = Save_Pokeathlon_Get(layout->args->saveData);
    u32 event;
    Pokeathlon_CourseEntry *record;
    u16 value;
    if (layout->recordScore == 0xFFFF) {
        GF_AssertFail();
        return FALSE;
    }
    event = PokeathlonCourse_GetField3D8_ForCurrentParticipant(course);
    if (layout->args->mode == 1) {
        record = PokeathlonSave_GetRecordsLink2(pokeathlon)[event].entries;
    } else {
        record = PokeathlonSave_GetRecordsSolo2(pokeathlon)[event].entries;
    }
    value = record->unk_0;
    if (value == 0xFFFF) {
        return TRUE;
    }
    if (_0221A7D8[event] == 0) {
        if (value < layout->recordScore) {
            return TRUE;
        }
    } else if (value > layout->recordScore) {
        return TRUE;
    }
    return FALSE;
}

u16 ov96_021E8424(u32 npcTeam) {
    if (npcTeam != 0 && npcTeam <= 100) {
        return ov96_0221AA20[(u8)(npcTeam - 1)];
    }
    GF_AssertFail();
    return 0;
}

u8 ov96_021E8448(PokeathlonCourseData *course, u32 npcTeam) {
    PokeathlonCourseNpcTeam team;
    NARC *narc;
    if (npcTeam != 0 && npcTeam <= 100) {
        narc = NARC_New(NARC_a_2_5_8, PokeathlonCourse_GetHeapID(course));
        NARC_ReadWholeMember(narc, npcTeam - 1, &team);
        NARC_Delete(narc);
        return team.unk07;
    }
    GF_AssertFail();
    return 0;
}

#pragma force_active on

// Fills the last count participant slots with NPC teams (NARC a/2/5/8, names from msg 310).
void PokeathlonCourse_SetupNpcParticipants(PokeathlonCourseData *course, int count) {
    u8 performance[21];
    u8 npcTeams[3];
    PokeathlonCourseNpcTeam team;
    String *speciesName;
    int j;
    int k;
    MsgData *msgData;
    NARC *performanceNarc;
    NARC *teamNarc;
    s32 i;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0310_bin, PokeathlonCourse_GetHeapID(course));
    i = 4 - count;
    PokeathlonCourse_ChooseNpcTeams(count, ((PokeathlonCourseLayout *)course)->args->course, ((PokeathlonCourseLayout *)course)->args->field_F, npcTeams);
    performanceNarc = NARC_New(NARC_poketool_personal_performance, PokeathlonCourse_GetHeapID(course));
    teamNarc = NARC_New(NARC_a_2_5_8, PokeathlonCourse_GetHeapID(course));
    for (j = 0; i < 4; i++, j++) {
        PokeathlonCourseParticipant *participant = (PokeathlonCourseParticipant *)PokeathlonCourse_GetParticipantData(course, i);
        String *name;
        participant->npcTeam = npcTeams[j];
        GF_ASSERT(participant->npcTeam != 0);
        NARC_ReadWholeMember(teamNarc, participant->npcTeam - 1, &team);
        name = NewString_ReadMsgData(msgData, team.nameId);
        Save_Profile_PlayerName_Set(PokeathlonCourse_GetPlayerProfileFromData(course, i), String_cstr(name));
        String_Delete(name);
        for (k = 0; k < 3; k++) {
            u8 gender;
            participant->mons[k].species = team.species[k];
            participant->mons[k].form = 0;
            participant->mons[k].isShiny = 0;
            participant->mons[k].personality = 0;
            switch ((u8)GetMonBaseStat(participant->mons[k].species, BASE_GENDER_RATIO)) {
            case MON_RATIO_MALE:
                gender = MON_MALE;
                break;
            case MON_RATIO_FEMALE:
                gender = MON_FEMALE;
                break;
            case MON_RATIO_UNKNOWN:
                gender = MON_GENDERLESS;
                break;
            default:
                gender = MON_MALE;
                break;
            }
            participant->mons[k].gender = gender;
            NARC_ReadWholeMember(performanceNarc, PokeathlonCourse_GetPerformanceFileId(participant->mons[k].species, 0), performance);
            participant->mons[k].stats[0] = performance[0];
            participant->mons[k].stats[1] = performance[1];
            participant->mons[k].stats[2] = performance[2];
            participant->mons[k].stats[3] = performance[3];
            participant->mons[k].stats[4] = performance[4];
            speciesName = GetSpeciesName(team.species[k], PokeathlonCourse_GetHeapID(course));
            CopyU16StringArrayN(participant->mons[k].nickname, String_cstr(speciesName), 11);
            String_Delete(speciesName);
        }
    }
    NARC_Delete(teamNarc);
    NARC_Delete(performanceNarc);
    DestroyMsgData(msgData);
}

#pragma force_active reset

// Picks count different NPC teams: 5 per course (courses 0..9) or any of 25 (course 10); hard ones are 50 later.
void PokeathlonCourse_ChooseNpcTeams(u32 count, u8 courseId, u32 hard, u8 *npcTeams) {
    u8 pool5[5];
    u8 pool25[25];
    u8 i = 0;
    u8 zero = i;
    for (; i < 3; i++) {
        npcTeams[i] = zero;
    }
    if (courseId == 10) {
        u8 remaining = 25;
        for (i = 0; i < 25; i++) {
            pool25[i] = i;
        }
        for (i = 0; i < count; i++) {
            u8 at = (u8)((s32)LCRandom() % remaining);
            npcTeams[i] = pool25[at];
            npcTeams[i] += 25;
            remaining--;
            pool25[at] = pool25[remaining];
        }
    } else {
        u8 remaining = 5;
        u32 base;
        for (i = 0; i < 5; i++) {
            pool5[i] = i;
        }
        i = 0;
        if (i < count) {
            base = courseId * 5;
            do {
                u8 at = (u8)((s32)LCRandom() % remaining);
                npcTeams[i] = pool5[at];
                npcTeams[i] += base;
                remaining--;
                pool5[at] = pool5[remaining];
                i++;
            } while (i < count);
        }
    }
    if (hard) {
        for (i = 0; i < count; i++) {
            npcTeams[i] += 50;
        }
    }
    for (i = 0; i < count; i++) {
        npcTeams[i]++;
    }
}

void *PokeathlonCourse_GetJuiceRecord(PokeathlonCourseData *course, u32 index) {
    u32 size;
    if (index >= 4) {
        GF_AssertFail();
    }
    if (course->heapAllocPtr3 == NULL) {
        GF_AssertFail();
    }
    size = ApricornJuiceRecord_sizeof();
    return (u8 *)course->heapAllocPtr3 + size * index;
}

#pragma force_active on

// Whether (x, y) lies within range of (targetX, targetY); the distance goes to *distance.
BOOL PokeathlonCourse_IsInRange(int x, int y, int targetX, int targetY, int range, int *distance) {
    VecFx32 a;
    VecFx32 b;
    a.x = x << FX32_SHIFT;
    a.y = y << FX32_SHIFT;
    a.z = 0;
    b.x = targetX << FX32_SHIFT;
    b.y = targetY << FX32_SHIFT;
    b.z = 0;
    VecFx32 delta;
    VEC_Subtract(&a, &b, &delta);
    {
        int magnitude = VEC_Mag(&delta);
        // the volatile accesses are needed to match
        **(int *volatile *)&distance = magnitude;
        if (magnitude < (*(volatile int *)&range << FX32_SHIFT)) {
            return TRUE;
        }
        return FALSE;
    }
}

#pragma force_active reset

// Creates the link of the course: the wireless command table (link mode) or a local command queue.
void *PokeathlonCourseLink_New(int a0, int a1, PokeathlonCourseData *data, int a3, enum HeapID heapId) {
    PokeathlonCourseLink *link = Heap_Alloc(heapId, sizeof(PokeathlonCourseLink));
    link->isLink = a3;
    link->checkSize = FALSE;
    link->queue = NULL;
    if (a3 == 1) {
        Comm_RegisterCommandTable((const void *)a0, a1, data);
    } else {
        link->queue = PokeathlonCourseCmdQueue_New((const PokeathlonCourseCmd *)a0, a1, data, heapId);
    }
    return link;
}

void PokeathlonCourseLink_SetCheckSize(PokeathlonCourseLink *link, u16 checkSize) {
    link->checkSize = checkSize;
}

void PokeathlonCourseLink_SendCommand(int a0, void *a1, void *a2, int a3) {
    PokeathlonCourseLink *link = (PokeathlonCourseLink *)a3;
    if (link->checkSize != 0 && (s32)a2 > POKEATHLON_COURSE_CMD_INLINE_SIZE) {
        GF_AssertFail();
    }
    if (link->isLink == 1) {
        Comm_SendCommand(a0, a1, (int)a2);
    } else {
        PokeathlonCourseCmdQueue_Add(link->queue, a0, a1, (s32)a2);
    }
}

BOOL PokeathlonCourseLink_SendLargeCommand(int cmd, void *data, int size, PokeathlonCourseLink *link) {
    if (link->isLink == 1) {
        return sub_02036FD8(cmd, data, size);
    } else {
        PokeathlonCourseCmdQueue_AddLarge(link->queue, cmd, data, size);
    }
}

void PokeathlonCourseLink_Free(void *ptr) {
    PokeathlonCourseLink *link = ptr;
    if (link->isLink == 0) {
        PokeathlonCourseCmdQueue_Free(link->queue);
    }
    Heap_Free(link);
}

BOOL PokeathlonCourseLink_GetNetId(void *ptr) {
    PokeathlonCourseLink *link = ptr;
    if (link->isLink == 1) {
        return Comm_GetLocalNetId();
    }
    return 0;
}

PokeathlonCourseCmdQueue *PokeathlonCourseCmdQueue_New(const PokeathlonCourseCmd *cmds, int cmdCount, void *context, enum HeapID heapId) {
    PokeathlonCourseCmdQueue *queue = Heap_Alloc(heapId, sizeof(PokeathlonCourseCmdQueue));
    memset(queue, 0, sizeof(PokeathlonCourseCmdQueue));
    queue->cmds = cmds;
    queue->cmdCount = cmdCount;
    queue->context = context;
    queue->task = SysTask_CreateOnVWaitQueue(PokeathlonCourseCmdQueue_Task, queue, 5000);
    return queue;
}

// Runs the queued commands (in VBlank, as received link commands would be).
void PokeathlonCourseCmdQueue_Task(SysTask *task, void *data) {
    PokeathlonCourseCmdQueue *queue = data;
    int i;
    PokeathlonCourseCmdSlot *slot;
    i = 0;
    slot = queue->slots;
    for (; i < 8; i++, slot++) {
        if (slot->active == 1) {
            if (slot->size <= POKEATHLON_COURSE_CMD_INLINE_SIZE) {
                queue->cmds[slot->cmd].func(0, slot->size, slot->data, queue->context);
            } else {
                queue->cmds[slot->cmd].func(0, slot->size, slot->buffer, queue->context);
            }
            memset(slot, 0, sizeof(PokeathlonCourseCmdSlot));
        }
    }
}

void PokeathlonCourseCmdQueue_Free(PokeathlonCourseCmdQueue *queue) {
    SysTask_Destroy(queue->task);
    Heap_Free(queue);
}

// Queues a command whose data (at most POKEATHLON_COURSE_CMD_INLINE_SIZE bytes) is copied into the slot.
void PokeathlonCourseCmdQueue_Add(PokeathlonCourseCmdQueue *queue, u32 cmd, void *data, s32 size) {
    int i;
    PokeathlonCourseCmdSlot *slot = NULL;
    for (i = 0; i < 8; i++) {
        if (queue->slots[i].active == 0) {
            slot = &queue->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        GF_AssertFail();
    }
    cmd -= POKEATHLON_COURSE_CMD_FIRST;
    if (queue->cmds[cmd].getSize != NULL) {
        u32 expected = queue->cmds[cmd].getSize();
        if (expected == size) {
        } else {
            GF_AssertFail();
        }
    }
    if (size > POKEATHLON_COURSE_CMD_INLINE_SIZE) {
        GF_AssertFail();
    }
    slot->cmd = cmd;
    if (size > 0) {
        memcpy(slot->data, data, size);
    }
    slot->size = size;
    slot->active = 1;
}

// Queues a command whose data goes to the command's buffer (or is used in place if it has none).
void PokeathlonCourseCmdQueue_AddLarge(PokeathlonCourseCmdQueue *queue, u32 cmd, void *data, s32 size) {
    int i;
    PokeathlonCourseCmdSlot *slot = NULL;
    u32 expected;
    for (i = 0; i < 8; i++) {
        if (queue->slots[i].active == 0) {
            slot = &queue->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        GF_AssertFail();
    }
    cmd -= POKEATHLON_COURSE_CMD_FIRST;
    expected = queue->cmds[cmd].getSize();
    if (expected != 0xFFFF && expected != size) {
        GF_AssertFail();
    }
    if (queue->cmds[cmd].getBuffer != NULL) {
        void *buffer = queue->cmds[cmd].getBuffer(0, queue->context, size);
        if (size > 0) {
            memcpy(buffer, data, size);
        }
        slot->buffer = buffer;
    } else {
        slot->buffer = data;
    }
    slot->cmd = cmd;
    slot->size = size;
    slot->active = 1;
}

u8 *ov96_021E8A20(void *ptr) {
    return ptr;
}

int PokeathlonCourseCmd_GetTable(void) {
    return (int)ov96_0221AEC4;
}

int PokeathlonCourseCmd_GetCount(void) {
    return 12;
}

// Allocates the Pokemon sprite manager (PokeathlonCourseMonGfx_InitManager sets it up).
PokeathlonCourseMonGfxManager *PokeathlonCourseMonGfx_AllocManager(enum HeapID heapId) {
    PokeathlonCourseMonGfxManager *manager = Heap_Alloc(heapId, sizeof(PokeathlonCourseMonGfxManager));
    manager->heapId = heapId;
    return manager;
}
