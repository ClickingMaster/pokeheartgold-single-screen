#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "pokeathlon/pokeathlon_ring_drop.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "palette.h"
#include "sound_playback.h"
#include "sprite_system.h"

typedef struct PokeathlonRingDropBgTemplates {
    BgTemplate templates[7];
} PokeathlonRingDropBgTemplates;

// A Pokemon's screen y, to sort the Pokemon by depth.
typedef struct PokeathlonRingDropDepth {
    u8 y;
    void *monObject;
} PokeathlonRingDropDepth;

extern const GraphicsModes ov96_0221D6C4;
extern const OamCharTransferParam ov96_0221D6F4;
extern const SpriteResourceCountsListUnion ov96_0221D720;
extern const u32 ov96_0221D738[7];
extern const OamManagerParam ov96_0221D754;
extern const ManagedSpriteTemplate ov96_0221D79C;
extern const PokeathlonRingDropBgTemplates ov96_0221D834;

// Sound (declared by no header)
BOOL PlaySE_SetVolume(u16 seqNo, int volume);

// NitroSDK MATH_QSort (no header declares it)
void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *), void *context);

// The code calls these with an int member.
#define PokeathlonRingDropUi_SetMemberSwitching(ui, participant, member, switching) (((void (*)(PokeathlonRingDropUi *, u8, int, int))PokeathlonRingDropUi_SetSwitching)((ui), (participant), (member), (switching)))
#define PokeathlonRingDrop_GetTeamMemberParams(course, participant, member)         (((PokeathlonRingDropMemberParams * (*)(PokeathlonCourseData *, u8, int)) PokeathlonRingDrop_GetMemberParams)((course), (participant), (member)))

// Course helpers, mon objects and the computer participants (OVY_96).
void *PokeathlonCourseSprites_CreateResources(int heapId, int flags, int charVram);
void PokeathlonCourseSprites_FreeResources(void *resources);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, u32 timer);
void *PokeathlonCourseMonGfx_GetInfo(void *mon);
void PokeathlonCourseMonGfx_LoadPalette(void *info, enum HeapID heapId, u16 *palette);
void *PokeathlonCourseMonObjects_Get(void *monObjects, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *object);
void PokeathlonCourseMonObject_SetDrawFlag(void *object, int draw);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, int priority);
void PokeathlonCourseMonObject_SetAnimState(void *object, int anim);
void PokeathlonCourseMonObject_SetAnim(void *object, int anim);
void PokeathlonCourseMonObjects_SetPalette(void *monObjects, u8 index, int vram, u16 *palette);
void PokeathlonCourseObject_SetCenterPosition(void *object, int x, int y, int moveShadow);
void PokeathlonCourseObject_CenterToGround(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_SetScale(void *object, f32 scaleX, f32 scaleY);

#pragma force_active on

void PokeathlonRingDrop_InitSprites(PokeathlonRingDrop *ringDrop) {
    SpriteResourceCountsListUnion counts = ov96_0221D720;
    OamManagerParam oamParam = ov96_0221D754;
    OamCharTransferParam transferParam = ov96_0221D6F4;

    transferParam.maxTasks = 128;
    ringDrop->spriteSystem = SpriteSystem_Alloc(ringDrop->heapId);
    ringDrop->spriteManager = SpriteManager_New(ringDrop->spriteSystem);
    SpriteSystem_Init(ringDrop->spriteSystem, &oamParam, &transferParam, 32);
    SpriteSystem_InitSprites(ringDrop->spriteSystem, ringDrop->spriteManager, 128);
    SpriteSystem_InitManagerWithCapacities(ringDrop->spriteSystem, ringDrop->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(ringDrop->spriteSystem), 0, FX32_CONST(480));
    ringDrop->resources = PokeathlonCourseSprites_CreateResources(ringDrop->heapId, 743, 1);
}

#pragma force_active reset

void PokeathlonRingDrop_FreeSprites(PokeathlonRingDrop *ringDrop) {
    PokeathlonCourseSprites_FreeResources(ringDrop->resources);
    SpriteSystem_FreeResourcesAndManager(ringDrop->spriteSystem, ringDrop->spriteManager);
    SpriteSystem_Free(ringDrop->spriteSystem);
}

#pragma force_active on

void PokeathlonRingDrop_LoadSpriteResources(PokeathlonRingDrop *ringDrop) {
    SpriteSystem *spriteSystem;
    int i;
    SpriteManager *spriteManager;

    spriteSystem = ringDrop->spriteSystem;
    spriteManager = ringDrop->spriteManager;

    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 16, TRUE, vram, 10000);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 13, FALSE, 2, vram, 10000);
        SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 15, TRUE, 10000);
        SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 14, TRUE, 10000);
    }
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DSUB;
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 20, TRUE, vram, 10001);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 17, FALSE, 2, vram, 10001);
        SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 19, TRUE, 10001);
        SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 18, TRUE, 10001);
    }
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DSUB;
        for (i = 0; i < 2; i++) {
            SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 24, TRUE, vram, 10002 + i);
            SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 21, FALSE, 1, vram, 10002 + i);
        }
        SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 23, TRUE, 10002);
        SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 22, TRUE, 10002);
    }
}

#pragma force_active reset

ManagedSprite *PokeathlonRingDrop_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 drawPriority) {
    ManagedSpriteTemplate template = ov96_0221D79C;
    ManagedSprite *sprite;

    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = drawPriority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(480));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

#pragma force_active on

void PokeathlonRingDrop_InitBgs(PokeathlonRingDrop *ringDrop) {
    PokeathlonRingDropBgTemplates templates;
    GraphicsModes modes;
    BgConfig *bgConfig = ringDrop->bgConfig;
    enum HeapID heapId = ringDrop->heapId;
    int i;

    modes = ov96_0221D6C4;
    SetBothScreensModesAndDisable(&modes);
    templates = ov96_0221D834;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, (u8)ov96_0221D738[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)ov96_0221D738[i]);
        BG_ClearCharDataRange((u8)ov96_0221D738[i], 0x20, 0, heapId);
    }
    ringDrop->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, ringDrop->heapId);
    ringDrop->msgFormat = MessageFormat_New(ringDrop->heapId);
    FontID_Alloc((FontID)4, ringDrop->heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active reset

void PokeathlonRingDrop_FreeBgs(PokeathlonRingDrop *ringDrop) {
    int i;

    RemoveWindow(&ringDrop->window);
    MessageFormat_Delete(ringDrop->msgFormat);
    DestroyMsgData(ringDrop->msgData);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(ringDrop->bgConfig, (u8)ov96_0221D738[i]);
    }
    FontID_Release((FontID)4);
    Heap_Free(ringDrop->bgConfig);
}

void PokeathlonRingDrop_LoadBgGraphics(PokeathlonRingDrop *ringDrop) {
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 1, ringDrop->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 3, ringDrop->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 5, ringDrop->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 2, ringDrop->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 4, ringDrop->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 6, ringDrop->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_0, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, ringDrop->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, ringDrop->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 9, ringDrop->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_4_0, 11, ringDrop->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 12, ringDrop->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_0, 10, ringDrop->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, ringDrop->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_0, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, ringDrop->heapId);
}

// Switch mode 1: the ball flies up off the Pokemon; then the next member is requested.
#pragma force_active on

void PokeathlonRingDropDisplay_SwitchOut(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL cpu, BOOL own) {
    s16 x, y;

    switch (mon->switchStep) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->dustSprite, position[0], position[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->dustSprite, TRUE);
        ManagedSprite_ResetSpriteAnimCtrlState(mon->dustSprite);
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->switchSprite, position[0], position[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->switchSprite, TRUE);
        PokeathlonCourseMonObject_SetDrawFlag(mon->monObject, 0);
        mon->switchFrames = 0;
        PokeathlonRingDrop_PlaySE(SEQ_SE_GS_W100, own);
        mon->switchStep++;
        break;
    case 1:
        ManagedSprite_GetPositionXY(mon->switchSprite, &x, &y);
        y -= 40;
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->switchSprite, x, y, FX32_CONST(480));
        mon->switchFrames++;
        if (mon->switchFrames >= 10) {
            ManagedSprite_SetDrawFlag(mon->switchSprite, FALSE);
            mon->switchStep++;
        }
        break;
    case 2:
        if (own) {
            if (PokeathlonRingDropUi_RunSwitch(ui, participant, mon->member)) {
                input->switchState = 1;
                mon->switchStep++;
            }
        } else if (cpu) {
            if (PokeathlonRingDropUi_WaitSwitch(ui, participant, mon->member)) {
                input->switchState = 1;
                mon->switchStep++;
            }
        } else {
            mon->switchStep++;
        }
        break;
    case 3:
        break;
    default:
        GF_AssertFail();
        break;
    }
}

// Switch mode 2: the ball comes down to the Pokemon's position.
void PokeathlonRingDropDisplay_SwitchIn(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL report, BOOL own) {
    s16 x, y;

    switch (mon->switchStep) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->switchSprite, position[0], position[1] - 400, FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->switchSprite, TRUE);
        mon->switchFrames = 0;
        PokeathlonRingDrop_PlaySE(SEQ_SE_GS_NAGERU, own);
        mon->switchStep++;
        break;
    case 1:
        ManagedSprite_GetPositionXY(mon->switchSprite, &x, &y);
        y += 40;
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->switchSprite, x, y, FX32_CONST(480));
        mon->switchFrames++;
        if (mon->switchFrames >= 10) {
            mon->switchStep++;
        }
        break;
    case 2:
        if (report) {
            input->switchState = 2;
        }
        mon->switchStep++;
        break;
    case 3:
        break;
    default:
        GF_AssertFail();
        break;
    }
}

#pragma force_active reset

// Switch mode 0: the new Pokemon appears.
void PokeathlonRingDropDisplay_EndSwitch(PokeathlonRingDropMonSprites *mon, const s16 *position, PokeathlonRingDropInput *input, BOOL report, BOOL own) {
    PokeathlonCourseMonObject_SetDrawFlag(mon->monObject, 1);
    ManagedSprite_SetPositionXYWithSubscreenOffset(mon->dustSprite, position[0], position[1], FX32_CONST(480));
    ManagedSprite_ResetSpriteAnimCtrlState(mon->dustSprite);
    ManagedSprite_SetDrawFlag(mon->switchSprite, FALSE);
    PokeathlonRingDrop_PlaySE(SEQ_SE_GS_BOWA2, own);
    if (report) {
        input->switchState = 0;
    }
}

#pragma force_active on

void PokeathlonRingDropDisplay_UpdateSwitch(PokeathlonRingDropMonPacket *packet, PokeathlonRingDropMonSprites *mon, void *ui, PokeathlonCourseData *course, u8 participant) {
    PokeathlonRingDropInput *input;
    BOOL cpu;
    BOOL changed;
    BOOL hostCpu;
    s16 position[2];
    u8 *data;
    BOOL host;
    BOOL own;
    BOOL report;

    changed = FALSE;
    data = PokeathlonCourse_GetDataCopyArea(course);
    position[0] = packet->x;
    position[1] = packet->y;
    if (PokeathlonCourse_GetParticipantCount(course) <= participant) {
        cpu = TRUE;
        input = (PokeathlonRingDropInput *)PokeathlonCourse_ToBytes(data + RING_DROP_DATA_INPUTS + participant * RING_DROP_DATA_SLOT_SIZE);
    } else {
        cpu = FALSE;
        input = (PokeathlonRingDropInput *)PokeathlonCourse_ToBytes(data);
    }
    host = PokeathlonCourse_GetNetId(course) == 0 ? TRUE : FALSE;
    if (host && cpu) {
        hostCpu = TRUE;
    } else {
        hostCpu = FALSE;
    }
    own = participant == PokeathlonCourse_GetNetId(course) ? TRUE : FALSE;
    report = hostCpu | own;
    if (packet->switchState != mon->switchState) {
        mon->switchStep = 0;
        mon->switchState = packet->switchState;
        changed = TRUE;
    }
    switch (mon->switchState) {
    case 1:
        if (changed) {
            PokeathlonRingDropUi_SetMemberSwitching(ui, participant, packet->member, 1);
        }
        PokeathlonRingDropDisplay_SwitchOut(mon, position, ui, participant, input, hostCpu, own);
        break;
    case 2:
        PokeathlonRingDropDisplay_SwitchIn(mon, position, ui, participant, input, report, own);
        break;
    case 0:
        if (changed) {
            PokeathlonRingDropDisplay_EndSwitch(mon, position, input, report, own);
            PokeathlonRingDropUi_SetMemberSwitching(ui, participant, packet->member, 0);
        }
        break;
    }
}

#pragma force_active reset

int PokeathlonRingDropDisplay_CompareDepth(const void *a, const void *b) {
    if (*(const u8 *)a == *(const u8 *)b) {
        return 0;
    }
    if (*(const u8 *)a < *(const u8 *)b) {
        return 1;
    }
    return -1;
}

int PokeathlonRingDropDisplay_CompareScores(const void *a, const void *b) {
    if (*(const u8 *)a == *(const u8 *)b) {
        return 0;
    }
    if (*(const u8 *)a < *(const u8 *)b) {
        return 1;
    }
    return -1;
}

// Every participant: shows each Pokemon of the frame packet (sounds, hit and charge effects, the
// marks above the Pokemon, switching, jumps and squashing), then sorts the Pokemon by depth.
#pragma force_active on

void PokeathlonRingDropDisplay_ApplyFramePacket(PokeathlonRingDropDisplay *display, PokeathlonRingDropFramePacket *framePacket, PokeathlonCourseData *course) {
    int participant;
    PokeathlonRingDropMonPacket *packet;
    PokeathlonRingDropMonSprites *mon;
    PokeathlonRingDropDepth depths[RING_DROP_PARTICIPANTS] = { 0 };
    PokeathlonRingDrop *ringDrop;
    void *monObject;
    BOOL showHit;
    f32 scaleX;
    BOOL hostCpu;
    s16 halfDx;
    s16 halfDy;
    int index;
    int member;
    int team;
    u8 objectIndex;
    f32 scaleY;

    ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (framePacket->marks.bits.bumped && !display->bumped) {
        PlaySE(SEQ_SE_GS_IDOUHIT);
    }
    participant = 0;
    index = 0;
    display->bumped = framePacket->marks.bits.bumped;
    packet = framePacket->mons;
    mon = display->mons;
    for (; participant < RING_DROP_PARTICIPANTS; participant++, packet++, mon++, index += RING_DROP_TEAM_SIZE) {
        BOOL cpu = PokeathlonCourse_GetParticipantCount(course) <= participant;
        BOOL host = PokeathlonCourse_GetNetId(course) == 0;
        hostCpu = FALSE;
        if (host && cpu) {
            hostCpu = TRUE;
        }
        GF_ASSERT(packet != NULL);
        GF_ASSERT(mon != NULL);
        team = packet->member;
        mon->member = team;
        objectIndex = (u8)(packet->member + index);
        monObject = PokeathlonCourseMonObjects_Get(ringDrop->monObjects, objectIndex);
        mon->monObject = monObject;
        GF_ASSERT(monObject != NULL);
        if (participant == PokeathlonCourse_GetNetId(course)) {
            member = packet->member;
            {
                u32 marks = framePacket->marks.raw >> 8;
                int next = (u8)((member + 2) % RING_DROP_TEAM_SIZE + index);
                BOOL nextReady = ((marks >> (next * 2)) & 3) == 1;
                int afterNext = (u8)((member + 1) % RING_DROP_TEAM_SIZE + index);
                BOOL afterNextReady = ((marks >> (afterNext * 2)) & 3) == 1;
                PokeathlonRingDropUi_ShowNextReady(display->ui, nextReady, afterNextReady);
            }
        }
        if (mon->hit && packet->hit) {
            int other = packet->attacker;
            showHit = FALSE;
            if (other <= participant) {
                other--;
            }
            if (packet->hit == 1) {
                if (other == PokeathlonCourse_GetNetId(course)) {
                    PokeathlonScreenShake_Start(&ringDrop->shake, 4, 10);
                }
                showHit = TRUE;
                PlaySE(SEQ_SE_GS_TACKLEHIT);
            } else if (packet->hit == 2) {
                if (other == PokeathlonCourse_GetNetId(course)) {
                    PokeathlonScreenShake_Start(&ringDrop->shake, 10, 15);
                }
                showHit = TRUE;
                PlaySE(SEQ_SE_GS_PRESSHIT);
            }
            if (showHit) {
                int x, y, otherX, otherY;
                int dx, dy;
                s16 hitX, hitY;
                y = packet->y;
                otherY = framePacket->mons[other].y;
                otherX = framePacket->mons[other].x;
                x = packet->x;
                dx = x - otherX;
                if (dx < 0) {
                    dx = -dx;
                }
                halfDx = dx / 2;
                dy = y - otherY;
                if (dy < 0) {
                    dy = -dy;
                }
                halfDy = dy / 2;
                if (x > otherX) {
                    x = otherX;
                }
                hitX = halfDx + x;
                if (y > otherY) {
                    y = otherY;
                }
                hitY = halfDy + y;
                ManagedSprite_SetPositionXYWithSubscreenOffset(mon->hitSprite, hitX, hitY, FX32_CONST(480));
                ManagedSprite_ResetSpriteAnimCtrlState(mon->hitSprite);
                ManagedSprite_SetDrawFlag(mon->hitSprite, TRUE);
            }
        }
        mon->hit = packet->hit;
        mon->attacker = packet->attacker;
        PokeathlonCourseMonObject_SetAnimState(monObject, packet->direction + 1);
        if (packet->state == 4 && mon->state != 4) {
            PokeathlonCourseMonObjects_SetPalette(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][1]);
        } else if (mon->state == 4 && packet->state != 4) {
            PokeathlonCourseMonObjects_SetPalette(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][0]);
        }
        if (packet->state == 5 && mon->state != 5) {
            if (participant == PokeathlonCourse_GetNetId(course)) {
                PlaySE_SetPitch(SEQ_SE_GS_TACKLEHATUDOU, 256);
            } else {
                PlaySE_SetVolume(SEQ_SE_GS_TACKLEHATUDOU2, 64);
            }
        } else if (packet->state == 12 && mon->state != 12) {
            if (PokeathlonRingDrop_GetTeamMemberParams(course, participant, team)->jumpType) {
                PlaySE(SEQ_SE_GS_PRESSCHAKUCHI);
            }
        }
        if (packet->state == 6 && mon->state != 6) {
            ManagedSprite_SetAnimNoRestart(mon->chargeSprite, (u16)(packet->direction + 11));
            ManagedSprite_SetDrawFlag(mon->chargeSprite, TRUE);
        } else if ((packet->state != 6 && mon->state == 6) || (packet->state == 7 && mon->state == 5)) {
            ManagedSprite_SetAnim(mon->chargeSprite, (u16)(packet->direction + 15));
            ManagedSprite_SetDrawFlag(mon->chargeSprite, TRUE);
        }
        if (ManagedSprite_IsAnimated(mon->chargeSprite) == TRUE) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->chargeSprite, packet->x, packet->y, FX32_CONST(480));
        }
        if (packet->state == 6) {
            PokeathlonCourseMonObject_SetAnim(monObject, 12);
        }
        if (!packet->x || packet->x == 255 || !packet->y || packet->y == 255) {
            PokeathlonCourseMonObject_SetDrawFlag(monObject, 0);
            ManagedSprite_SetDrawFlag(mon->chargeSprite, FALSE);
            ManagedSprite_SetDrawFlag(mon->markSprite, FALSE);
        } else {
            BOOL switching = packet->switchState != 0;
            BOOL airborne = packet->state == 3;
            u8 mark = ((framePacket->marks.raw >> 8) >> (objectIndex * 2)) & 3;
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->markSprite, packet->x, (s16)(packet->y - packet->height - 24), FX32_CONST(480));
            if (airborne || switching) {
                mark = 0;
            } else if (mark == 2) {
                ManagedSprite_SetAnimNoRestart(mon->markSprite, 1);
                if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                    PlaySE(SEQ_SE_GS_W146);
                }
            } else if (mark == 1) {
                ManagedSprite_SetAnimNoRestart(mon->markSprite, 9);
            }
            ManagedSprite_SetDrawFlag(mon->markSprite, mark != 0);
        }
        PokeathlonRingDropDisplay_UpdateSwitch(packet, mon, display->ui, course, (u8)participant);
        {
            u8 airborne = packet->state == 3;
            if (airborne == TRUE && !mon->airborne) {
                mon->jumping = TRUE;
                mon->jumpFrames = 6;
                mon->jumpScale = FX32_ONE;
                PlaySE(SEQ_SE_GS_PUSH02);
                PokeathlonRingDropUi_SetMemberSwitching(display->ui, (u8)participant, team, 1);
                if (participant == PokeathlonCourse_GetNetId(course) || hostCpu) {
                    mon->recordJump = TRUE;
                }
            } else if (!airborne && mon->airborne == TRUE) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(mon->dustSprite, packet->x, packet->y, FX32_CONST(480));
                ManagedSprite_SetDrawFlag(mon->dustSprite, TRUE);
                ManagedSprite_ResetSpriteAnimCtrlState(mon->dustSprite);
                PokeathlonRingDrop_PlaySE(SEQ_SE_GS_BOWA2, participant == PokeathlonCourse_GetNetId(course));
                PokeathlonRingDropUi_SetMemberSwitching(display->ui, (u8)participant, team, 0);
                PokeathlonCourseMonObject_SetDrawFlag(monObject, 1);
                mon->jumping = FALSE;
            }
            mon->airborne = airborne;
        }
        PokeathlonCourseObject_SetCenterPosition(monObject, packet->x, packet->y, 1);
        PokeathlonCourseObject_SetCenterPosition(monObject, packet->x, packet->y - packet->height, 0);
        if (mon->jumping) {
            if (mon->recordJump) {
                if (hostCpu) {
                    if (PokeathlonRingDropUi_WaitSwitch(ringDrop->ui, (u8)participant, (u8)mon->member)) {
                        mon->recordJump = FALSE;
                    }
                } else {
                    if (PokeathlonRingDropUi_RunSwitch(ringDrop->ui, (u8)participant, (u8)mon->member)) {
                        mon->recordJump = FALSE;
                    }
                }
            }
            if (--mon->jumpFrames <= 0) {
                PokeathlonCourseMonObject_SetDrawFlag(monObject, 0);
            } else {
                int x = packet->x, y = packet->y;
                f32 scale;
                mon->jumpScale = (s32)((u32)mon->jumpScale - 410);
                scale = (f32)mon->jumpScale / 4096.0f;
                PokeathlonCourseObject_SetScale(monObject, scale, scale);
                PokeathlonCourseObject_CenterToGround(monObject, x, y, &x, &y);
                if (y >= 160) {
                    int fall = 6 - mon->jumpFrames;
                    fall = packet->y + fall * 4;
                    PokeathlonCourseObject_SetCenterPosition(monObject, packet->x, fall, 1);
                }
            }
        } else {
            scaleX = 1.2f - (f32)((160 - packet->y) * 12) / 4096.0f;
            scaleY = scaleX;
            if (packet->state == 5) {
                u32 frames = mon->squashFrames;
                scaleX = (f32)((double)scaleX * (0.8 - (double)frames * 0.1));
                if (frames < 4) {
                    mon->squashFrames = frames + 1;
                }
            }
            if (packet->state == 8) {
                PokeathlonRingDropMemberParams *params = PokeathlonRingDrop_GetTeamMemberParams(course, participant, team);
                u32 frames;
                if (!mon->squashFrames) {
                    if (!params->jumpType) {
                        if (participant == PokeathlonCourse_GetNetId(course)) {
                            PlaySE(SEQ_SE_DP_BOX03);
                        }
                    } else {
                        PlaySE(SEQ_SE_DP_DANSA);
                    }
                }
                frames = mon->squashFrames;
                scaleY = (f32)((double)scaleY * (0.8 - (double)frames * 0.1));
                if (frames < 5) {
                    mon->squashFrames = frames + 1;
                }
            }
            if (packet->state != 8 && packet->state != 5) {
                if (mon->squashFrames) {
                    PokeathlonRingDrop_SetMonAffineCenter(monObject, 1);
                    mon->squashFrames = 0;
                }
            } else {
                PokeathlonRingDrop_SetMonAffineCenter(monObject, 0);
            }
            PokeathlonCourseObject_SetScale(monObject, scaleX, scaleY);
        }
        mon->state = packet->state;
    }
    {
        int i, index;
        PokeathlonRingDropMonPacket *packet;
        PokeathlonRingDropDepth *depth;
        for (i = 0, index = 0, packet = framePacket->mons, depth = depths; i < RING_DROP_PARTICIPANTS; i++, packet++, index += RING_DROP_TEAM_SIZE, depth++) {
            void *monObject = PokeathlonCourseMonObjects_Get(ringDrop->monObjects, (u8)(packet->member + index));
            int x = packet->x, y = packet->y;
            PokeathlonCourseObject_CenterToGround(monObject, x, y, &x, &y);
            depth->y = y;
            depth->monObject = monObject;
        }
        MATH_QSort(depths, RING_DROP_PARTICIPANTS, sizeof(PokeathlonRingDropDepth), PokeathlonRingDropDisplay_CompareDepth, NULL);
        for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
            PokeathlonCourseMonObject_SetDrawPriority(depths[i].monObject, i + 7);
        }
    }
}

#pragma force_active reset

// PokeathlonCourseMonObjects_Get as the first call in PokeathlonRingDropDisplay_Init was built: with an int index.
typedef void *(*PokeathlonRingDropGetMonObjectFunc)(void *monObjects, int index);

void PokeathlonRingDropDisplay_Init(PokeathlonRingDropDisplay *display, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *ui, void *monObjects, enum HeapID heapId) {
    volatile s32 control[3];
    s32 next;
    int member;
    int index;
    PokeathlonRingDropMonSprites *mon;
    u16 *palette;
    u16 *grey;

    display->ui = ui;
    display->spriteSystem = spriteSystem;
    display->spriteManager = spriteManager;
    display->statusSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 128, 16, 3, 1);
    control[2] = 0;
    mon = display->mons;
    control[1] = control[2];
    control[0] = heapId;
    do {
        index = (u8)control[1];
        mon->monObject = ((PokeathlonRingDropGetMonObjectFunc)PokeathlonCourseMonObjects_Get)(monObjects, index);
        palette = mon->palettes[0][0];
        grey = mon->palettes[0][1];
        for (member = 0; member < RING_DROP_TEAM_SIZE; index++, member++, palette += 32, grey += 32) {
            PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObjects_Get(monObjects, (u8)index))), (enum HeapID)control[0], palette);
            MI_CpuCopy8(palette, grey, 32);
            TintPalette_GrayScale(grey, 16);
        }
        mon->hitSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 7, 3);
        ManagedSprite_SetDrawFlag(mon->hitSprite, FALSE);
        mon->markSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 1, 5);
        ManagedSprite_SetDrawFlag(mon->markSprite, FALSE);
        mon->chargeSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 11, 4);
        ManagedSprite_SetDrawFlag(mon->chargeSprite, FALSE);
        mon->switchSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 21, 6);
        ManagedSprite_SetDrawFlag(mon->switchSprite, FALSE);
        mon->dustSprite = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 8, 2);
        ManagedSprite_SetDrawFlag(mon->dustSprite, FALSE);
        mon++;
        control[1] += RING_DROP_TEAM_SIZE;
        next = control[2] + 1;
        control[2] = next;
    } while (next < RING_DROP_PARTICIPANTS);
}

// Every participant: shows the frame packet the host sent.
void PokeathlonRingDropDisplay_Update(PokeathlonRingDropDisplay *display, PokeathlonCourseData *course) {
    u8 sorted[RING_DROP_PARTICIPANTS][2];
    PokeathlonRingDropFramePacket *packet;
    PokeathlonRingDrop *ringDrop;
    u8 status;
    u32 anim;
    int i;

    packet = (PokeathlonRingDropFramePacket *)(void *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + RING_DROP_DATA_FRAME_IN);
    ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropDisplay_ApplyFramePacket(display, packet, course);
    PokeathlonCourse_UpdateTimeUpCountdown(course, packet->timer);
    if (packet->timer == 450) {
        PlaySE(SEQ_SE_DP_TRAIN04);
    }
    status = packet->mons[PokeathlonCourse_GetNetId(course)].status;
    switch (status) {
    case 0:
        anim = 2;
        break;
    case 3:
        anim = 3;
        break;
    case 2:
        anim = 4;
        break;
    case 4:
        anim = 5;
        break;
    case 1:
        anim = 6;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ManagedSprite_SetAnimNoRestart(display->statusSprite, anim);
    if (status != display->status) {
        if (status == 2) {
            PlaySE(SEQ_SE_GS_CHANGE1);
        } else if (status == 4) {
            PlaySE(SEQ_SE_GS_CHANGE2);
        }
    }
    display->status = status;
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        sorted[i][1] = (u8)i;
        sorted[i][0] = packet->mons[i].score;
    }
    MATH_QSort(sorted, RING_DROP_PARTICIPANTS, 2, PokeathlonRingDropDisplay_CompareScores, NULL);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        if (packet->mons[i].score != display->scores[i]) {
            PokeathlonRingDropUi_SetRanks(display->ui, (u8)i, packet->mons[i].score, sorted[0]);
        }
        display->scores[i] = packet->mons[i].score;
    }
    PokeathlonRingDropUi_SetTimer(ringDrop->ui, packet->timer / 30);
}

void PokeathlonRingDropDisplay_Free(PokeathlonRingDropDisplay *display) {
    int i;
    int j;
    PokeathlonRingDropMonSprites *mon;
    PokeathlonRingDropMonSprites *cursor;

    GF_ASSERT(display->spriteSystem != NULL);
    GF_ASSERT(display->spriteManager != NULL);
    GF_ASSERT(display->statusSprite != NULL);
    Sprite_DeleteAndFreeResources(display->statusSprite);
    for (i = 0, mon = display->mons; i < RING_DROP_PARTICIPANTS; i++, mon++) {
        // one cursor over the five sprite pointers
        for (j = 0, cursor = mon; j < 5; j++, cursor = (PokeathlonRingDropMonSprites *)((u8 *)cursor + sizeof(ManagedSprite *))) {
            Sprite_DeleteAndFreeResources(cursor->chargeSprite);
        }
    }
}

#pragma force_active on

void PokeathlonRingDropDisplay_StopAnimations(PokeathlonRingDropDisplay *display) {
    int i = 0;
    int j;
    PokeathlonRingDropDisplay *group = display;
    BOOL animate = FALSE;

    // one cursor per participant, one over the five sprite pointers
    do {
        PokeathlonRingDropDisplay *cursor;
        j = 0;
        cursor = group;
        do {
            ManagedSprite_SetAnimateFlag(cursor->mons[0].chargeSprite, animate);
            j++;
            cursor = (PokeathlonRingDropDisplay *)((u8 *)cursor + sizeof(ManagedSprite *));
        } while (j < 5);
        group = (PokeathlonRingDropDisplay *)((u8 *)group + sizeof(PokeathlonRingDropMonSprites));
        i++;
    } while (i < RING_DROP_PARTICIPANTS);
    ManagedSprite_SetAnimateFlag(display->statusSprite, FALSE);
}

// A Pokemon falling on another (a press) cannot be pushed itself.
BOOL PokeathlonRingDropMon_CanBePushed(int state, int attackerState) {
    switch (state) {
    case RING_DROP_MON_JUMP_FALL:
        return FALSE;
    default:
        return TRUE;
    }
}

#pragma force_active reset

void PokeathlonRingDropMon_FaceDirection(PokeathlonRingDropMon *mon, const VecFx32 *direction) {
    if (mon->state != RING_DROP_MON_DASH && mon->state != RING_DROP_MON_JUMP_FALL) {
        mon->flags.bits.direction = PokeathlonRingDrop_VecToDirection(direction);
    }
}
