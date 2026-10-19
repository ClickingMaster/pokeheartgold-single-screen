#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
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
    PokeathlonCourseMonObject *monObject;
} PokeathlonRingDropDepth;

extern const GraphicsModes gPokeathlonRingDropGraphicsModes;
extern const OamCharTransferParam gPokeathlonRingDropOamCharTransferParam;
extern const SpriteResourceCountsListUnion gPokeathlonRingDropSpriteResourceCounts;
extern const u32 gPokeathlonRingDropBgLayers[7];
extern const OamManagerParam gPokeathlonRingDropOamManagerParam;
extern const ManagedSpriteTemplate gPokeathlonRingDropSpriteTemplate;
extern const PokeathlonRingDropBgTemplates gPokeathlonRingDropBgTemplates;

BOOL PlaySE_SetVolume(u16 seqNo, int volume);

void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *), void *context);

#define PokeathlonRingDropUi_SetMemberSwitching(ui, participant, member, switching) (((void (*)(PokeathlonRingDropUi *, u8, int, int))PokeathlonRingDropUi_SetSwitching)((ui), (participant), (member), (switching)))
#define PokeathlonRingDrop_GetTeamMemberParams(course, participant, member)         (((PokeathlonRingDropMemberParams * (*)(PokeathlonCourseData *, u8, int)) PokeathlonRingDrop_GetMemberParams)((course), (participant), (member)))

void PokeathlonCourseMonObjects_SetPalette(PokeathlonCourseMonObjects *monObjects, u8 index, int vram, u16 *palette);

void PokeathlonRingDrop_InitSprites(PokeathlonRingDrop *ringDrop) {
    SpriteResourceCountsListUnion counts = gPokeathlonRingDropSpriteResourceCounts;
    OamManagerParam oamParam = gPokeathlonRingDropOamManagerParam;
    OamCharTransferParam transferParam = gPokeathlonRingDropOamCharTransferParam;

    transferParam.maxTasks = 128;
    ringDrop->spriteSystem = SpriteSystem_Alloc(ringDrop->heapId);
    ringDrop->spriteManager = SpriteManager_New(ringDrop->spriteSystem);
    SpriteSystem_Init(ringDrop->spriteSystem, &oamParam, &transferParam, 32);
    SpriteSystem_InitSprites(ringDrop->spriteSystem, ringDrop->spriteManager, 128);
    SpriteSystem_InitManagerWithCapacities(ringDrop->spriteSystem, ringDrop->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(ringDrop->spriteSystem), 0, FX32_CONST(480));
    ringDrop->resources = PokeathlonCourseSprites_CreateResources(ringDrop->heapId, 743, 1);
}

void PokeathlonRingDrop_FreeSprites(PokeathlonRingDrop *ringDrop) {
    PokeathlonCourseSprites_FreeResources(ringDrop->resources);
    SpriteSystem_FreeResourcesAndManager(ringDrop->spriteSystem, ringDrop->spriteManager);
    SpriteSystem_Free(ringDrop->spriteSystem);
}

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

static ManagedSprite *PokeathlonRingDrop_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 drawPriority) {
    ManagedSpriteTemplate template = gPokeathlonRingDropSpriteTemplate;
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

void PokeathlonRingDrop_InitBgs(PokeathlonRingDrop *ringDrop) {
    PokeathlonRingDropBgTemplates templates;
    GraphicsModes modes;
    BgConfig *bgConfig = ringDrop->bgConfig;
    enum HeapID heapId = ringDrop->heapId;
    int i;

    modes = gPokeathlonRingDropGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = gPokeathlonRingDropBgTemplates;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, (u8)gPokeathlonRingDropBgLayers[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)gPokeathlonRingDropBgLayers[i]);
        BG_ClearCharDataRange((u8)gPokeathlonRingDropBgLayers[i], 0x20, 0, heapId);
    }
    ringDrop->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, ringDrop->heapId);
    ringDrop->msgFormat = MessageFormat_New(ringDrop->heapId);
    FontID_Alloc((FontID)4, ringDrop->heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonRingDrop_FreeBgs(PokeathlonRingDrop *ringDrop) {
    int i;

    RemoveWindow(&ringDrop->window);
    MessageFormat_Delete(ringDrop->msgFormat);
    DestroyMsgData(ringDrop->msgData);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(ringDrop->bgConfig, (u8)gPokeathlonRingDropBgLayers[i]);
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

static void PokeathlonRingDropDisplay_SwitchOut(PokeathlonRingDropMonSprites *mon, s16 *position, PokeathlonRingDropUi *ui, u8 participant, PokeathlonRingDropInput *input, BOOL cpu, BOOL own) {
    s16 x, y;

    switch (mon->switchStep) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_DUST], position[0], position[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_DUST], TRUE);
        ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[RING_DROP_MON_SPRITE_DUST]);
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], position[0], position[1], FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], TRUE);
        PokeathlonCourseMonObject_SetDrawFlag(mon->monObject, 0);
        mon->switchFrames = 0;
        PokeathlonRingDrop_PlaySE(SEQ_SE_GS_W100, own);
        mon->switchStep++;
        break;
    case 1:
        ManagedSprite_GetPositionXY(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], &x, &y);
        y -= 40;
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], x, y, FX32_CONST(480));
        mon->switchFrames++;
        if (mon->switchFrames >= 10) {
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], FALSE);
            mon->switchStep++;
        }
        break;
    case 2:
        if (own) {
            if (PokeathlonRingDropUi_RunSwitch(ui, participant, mon->member)) {
                input->switchState = RING_DROP_SWITCH_OUT;
                mon->switchStep++;
            }
        } else if (cpu) {
            if (PokeathlonRingDropUi_WaitSwitch(ui, participant, mon->member)) {
                input->switchState = RING_DROP_SWITCH_OUT;
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
static void PokeathlonRingDropDisplay_SwitchIn(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL report, BOOL own) {
    s16 x, y;

    switch (mon->switchStep) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], position[0], position[1] - 400, FX32_CONST(480));
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], TRUE);
        mon->switchFrames = 0;
        PokeathlonRingDrop_PlaySE(SEQ_SE_GS_NAGERU, own);
        mon->switchStep++;
        break;
    case 1:
        ManagedSprite_GetPositionXY(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], &x, &y);
        y += 40;
        ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], x, y, FX32_CONST(480));
        mon->switchFrames++;
        if (mon->switchFrames >= 10) {
            mon->switchStep++;
        }
        break;
    case 2:
        if (report) {
            input->switchState = RING_DROP_SWITCH_IN;
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

// Switch mode 0: the new Pokemon appears.
static void PokeathlonRingDropDisplay_EndSwitch(PokeathlonRingDropMonSprites *mon, const s16 *position, PokeathlonRingDropInput *input, BOOL report, BOOL own) {
    PokeathlonCourseMonObject_SetDrawFlag(mon->monObject, 1);
    ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_DUST], position[0], position[1], FX32_CONST(480));
    ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[RING_DROP_MON_SPRITE_DUST]);
    ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], FALSE);
    PokeathlonRingDrop_PlaySE(SEQ_SE_GS_BOWA2, own);
    if (report) {
        input->switchState = RING_DROP_SWITCH_NONE;
    }
}

static void PokeathlonRingDropDisplay_UpdateSwitch(PokeathlonRingDropMonPacket *packet, PokeathlonRingDropMonSprites *mon, void *ui, PokeathlonCourseData *course, u8 participant) {
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
        input = (PokeathlonRingDropInput *)PokeathlonCourse_ToBytes(data + POKEATHLON_COURSE_DATA_INPUTS + participant * POKEATHLON_COURSE_DATA_SLOT_SIZE);
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
    case RING_DROP_SWITCH_OUT:
        if (changed) {
            PokeathlonRingDropUi_SetMemberSwitching(ui, participant, packet->member, 1);
        }
        PokeathlonRingDropDisplay_SwitchOut(mon, position, ui, participant, input, hostCpu, own);
        break;
    case RING_DROP_SWITCH_IN:
        PokeathlonRingDropDisplay_SwitchIn(mon, position, ui, participant, input, report, own);
        break;
    case RING_DROP_SWITCH_NONE:
        if (changed) {
            PokeathlonRingDropDisplay_EndSwitch(mon, position, input, report, own);
            PokeathlonRingDropUi_SetMemberSwitching(ui, participant, packet->member, 0);
        }
        break;
    }
}

static int PokeathlonRingDropDisplay_CompareDepth(const void *a, const void *b) {
    if (*(const u8 *)a == *(const u8 *)b) {
        return 0;
    }
    if (*(const u8 *)a < *(const u8 *)b) {
        return 1;
    }
    return -1;
}

static int PokeathlonRingDropDisplay_CompareScores(const void *a, const void *b) {
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

static void PokeathlonRingDropDisplay_ApplyFramePacket(PokeathlonRingDropDisplay *display, PokeathlonRingDropFramePacket *framePacket, PokeathlonCourseData *course) {
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

    ringDrop = PokeathlonCourse_GetWork(course);
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
                ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_HIT], hitX, hitY, FX32_CONST(480));
                ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[RING_DROP_MON_SPRITE_HIT]);
                ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_HIT], TRUE);
            }
        }
        mon->hit = packet->hit;
        mon->attacker = packet->attacker;
        PokeathlonCourseMonObject_SetAnimState(monObject, packet->direction + 1);
        if (packet->state == RING_DROP_MON_RETURN && mon->state != RING_DROP_MON_RETURN) {
            PokeathlonCourseMonObjects_SetPalette(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][1]);
        } else if (mon->state == RING_DROP_MON_RETURN && packet->state != RING_DROP_MON_RETURN) {
            PokeathlonCourseMonObjects_SetPalette(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][0]);
        }
        if (packet->state == RING_DROP_MON_DASH_START && mon->state != RING_DROP_MON_DASH_START) {
            if (participant == PokeathlonCourse_GetNetId(course)) {
                PlaySE_SetPitch(SEQ_SE_GS_TACKLEHATUDOU, 256);
            } else {
                PlaySE_SetVolume(SEQ_SE_GS_TACKLEHATUDOU2, 64);
            }
        } else if (packet->state == RING_DROP_MON_LANDING && mon->state != RING_DROP_MON_LANDING) {
            if (PokeathlonRingDrop_GetTeamMemberParams(course, participant, team)->jumpType) {
                PlaySE(SEQ_SE_GS_PRESSCHAKUCHI);
            }
        }
        if (packet->state == RING_DROP_MON_DASH && mon->state != RING_DROP_MON_DASH) {
            ManagedSprite_SetAnimNoRestart(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], (u16)(packet->direction + 11));
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], TRUE);
        } else if ((packet->state != RING_DROP_MON_DASH && mon->state == RING_DROP_MON_DASH) || (packet->state == RING_DROP_MON_DASH_END && mon->state == RING_DROP_MON_DASH_START)) {
            ManagedSprite_SetAnim(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], (u16)(packet->direction + 15));
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], TRUE);
        }
        if (ManagedSprite_IsAnimated(mon->sprites[RING_DROP_MON_SPRITE_CHARGE]) == TRUE) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], packet->x, packet->y, FX32_CONST(480));
        }
        if (packet->state == RING_DROP_MON_DASH) {
            PokeathlonCourseMonObject_SetAnim(monObject, 12);
        }
        if (!packet->x || packet->x == 255 || !packet->y || packet->y == 255) {
            PokeathlonCourseMonObject_SetDrawFlag(monObject, 0);
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], FALSE);
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_MARK], FALSE);
        } else {
            BOOL switching = packet->switchState != RING_DROP_SWITCH_NONE;
            BOOL airborne = packet->state == RING_DROP_MON_FALL_OUT;
            u8 mark = ((framePacket->marks.raw >> 8) >> (objectIndex * 2)) & 3;
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_MARK], packet->x, (s16)(packet->y - packet->height - 24), FX32_CONST(480));
            if (airborne || switching) {
                mark = 0;
            } else if (mark == 2) {
                ManagedSprite_SetAnimNoRestart(mon->sprites[RING_DROP_MON_SPRITE_MARK], 1);
                if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                    PlaySE(SEQ_SE_GS_W146);
                }
            } else if (mark == 1) {
                ManagedSprite_SetAnimNoRestart(mon->sprites[RING_DROP_MON_SPRITE_MARK], 9);
            }
            ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_MARK], mark != 0);
        }
        PokeathlonRingDropDisplay_UpdateSwitch(packet, mon, display->ui, course, (u8)participant);
        {
            u8 airborne = packet->state == RING_DROP_MON_FALL_OUT;
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
                ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[RING_DROP_MON_SPRITE_DUST], packet->x, packet->y, FX32_CONST(480));
                ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_DUST], TRUE);
                ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[RING_DROP_MON_SPRITE_DUST]);
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
            if (packet->state == RING_DROP_MON_DASH_START) {
                u32 frames = mon->squashFrames;
                scaleX = (f32)((double)scaleX * (0.8 - (double)frames * 0.1));
                if (frames < 4) {
                    mon->squashFrames = frames + 1;
                }
            }
            if (packet->state == RING_DROP_MON_JUMP_START) {
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
            if (packet->state != RING_DROP_MON_JUMP_START && packet->state != RING_DROP_MON_DASH_START) {
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
            PokeathlonCourseMonObject *monObject = PokeathlonCourseMonObjects_Get(ringDrop->monObjects, (packet->member + index));
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

typedef void *(*PokeathlonRingDropGetMonObjectFunc)(void *monObjects, int index);

void PokeathlonRingDropDisplay_Init(PokeathlonRingDropDisplay *display, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonRingDropUi *ui, void *monObjects, enum HeapID heapId) {
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
            PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObjects_Get(monObjects, index))), (enum HeapID)control[0], palette);
            MI_CpuCopy8(palette, grey, 32);
            TintPalette_GrayScale(grey, 16);
        }
        mon->sprites[RING_DROP_MON_SPRITE_HIT] = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 7, 3);
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_HIT], FALSE);
        mon->sprites[RING_DROP_MON_SPRITE_MARK] = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 1, 5);
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_MARK], FALSE);
        mon->sprites[RING_DROP_MON_SPRITE_CHARGE] = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 11, 4);
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_CHARGE], FALSE);
        mon->sprites[RING_DROP_MON_SPRITE_SWITCH] = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 21, 6);
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_SWITCH], FALSE);
        mon->sprites[RING_DROP_MON_SPRITE_DUST] = PokeathlonRingDrop_NewSprite(spriteSystem, spriteManager, 0, 0, 8, 2);
        ManagedSprite_SetDrawFlag(mon->sprites[RING_DROP_MON_SPRITE_DUST], FALSE);
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

    packet = (PokeathlonRingDropFramePacket *)(void *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + POKEATHLON_COURSE_DATA_FRAME_IN);
    ringDrop = PokeathlonCourse_GetWork(course);
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

    GF_ASSERT(display->spriteSystem != NULL);
    GF_ASSERT(display->spriteManager != NULL);
    GF_ASSERT(display->statusSprite != NULL);
    Sprite_DeleteAndFreeResources(display->statusSprite);
    for (i = 0, mon = display->mons; i < RING_DROP_PARTICIPANTS; i++, mon++) {
        for (j = 0; j < RING_DROP_MON_SPRITE_COUNT; j++) {
            Sprite_DeleteAndFreeResources(mon->sprites[j]);
        }
    }
}

void PokeathlonRingDropDisplay_StopAnimations(PokeathlonRingDropDisplay *display) {
    int i;
    int j;
    BOOL animate = FALSE;

    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        for (j = 0; j < RING_DROP_MON_SPRITE_COUNT; j++) {
            ManagedSprite_SetAnimateFlag(display->mons[i].sprites[j], animate);
        }
    }
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

void PokeathlonRingDropMon_FaceDirection(PokeathlonRingDropMon *mon, const VecFx32 *direction) {
    if (mon->state != RING_DROP_MON_DASH && mon->state != RING_DROP_MON_JUMP_FALL) {
        mon->flags.bits.direction = PokeathlonRingDrop_VecToDirection(direction);
    }
}
