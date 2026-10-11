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

// Course helpers, mon objects and the Ring Drop parts that are not yet tidied (OVY_96).
void *PokeathlonCourseSprites_CreateResources(int heapId, int a1, int a2);
void PokeathlonCourseSprites_FreeResources(void *resources);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, u32 timer);
void *PokeathlonCourseMonGfx_GetInfo(void *a0);
void PokeathlonCourseMonGfx_LoadPalette(void *a0, enum HeapID heapId, u16 *palette);
void *PokeathlonCourseMonObjects_Get(void *monObjects, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *object);
void PokeathlonCourseMonObject_SetDrawFlag(void *object, int a1);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, int priority);
void PokeathlonCourseMonObject_SetAnimState(void *object, int anim);
void PokeathlonCourseMonObject_SetAnim(void *object, int anim);
void ov96_021EAF60(void *monObjects, u8 index, int a2, u16 *palette);
void PokeathlonCourseObject_SetCenterPosition(void *object, int x, int y, int a3);
void PokeathlonCourseObject_CenterToGround(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_SetScale(void *object, f32 scaleX, f32 scaleY);
void ov96_02218510(void *object, int a1);
u8 *ov96_0221935C(PokeathlonCourseData *course, u8 participant, int member);
void ov96_022193CC(int seqNo, BOOL own);
void ov96_0221966C(void *ui, u8 participant, u8 rank, u8 (*sorted)[2]);
void ov96_022196E4(void *ui, u32 seconds);
void ov96_02219770(void *ui, BOOL a1, BOOL a2);
BOOL ov96_02219794(void *ui, u8 participant, u8 member);
BOOL ov96_02219940(void *ui, u8 participant, u8 member);
void ov96_0221996C(void *ui, u8 participant, int member, int a3);
int ov96_022186CC(int a0);

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
        ov96_022193CC(SEQ_SE_GS_W100, own);
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
            if (ov96_02219794(ui, participant, mon->member)) {
                input->switchState = 1;
                mon->switchStep++;
            }
        } else if (cpu) {
            if (ov96_02219940(ui, participant, mon->member)) {
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
        ov96_022193CC(SEQ_SE_GS_NAGERU, own);
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
    ov96_022193CC(SEQ_SE_GS_BOWA2, own);
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
        input = (PokeathlonRingDropInput *)ov96_021E8A20(data + RING_DROP_DATA_INPUTS + participant * RING_DROP_DATA_SLOT_SIZE);
    } else {
        cpu = FALSE;
        input = (PokeathlonRingDropInput *)ov96_021E8A20(data);
    }
    host = ov96_021E5F24(course) == 0 ? TRUE : FALSE;
    if (host && cpu) {
        hostCpu = TRUE;
    } else {
        hostCpu = FALSE;
    }
    own = participant == ov96_021E5F24(course) ? TRUE : FALSE;
    report = hostCpu | own;
    if (packet->switchState != mon->switchState) {
        mon->switchStep = 0;
        mon->switchState = packet->switchState;
        changed = TRUE;
    }
    switch (mon->switchState) {
    case 1:
        if (changed) {
            ov96_0221996C(ui, participant, packet->member, 1);
        }
        PokeathlonRingDropDisplay_SwitchOut(mon, position, ui, participant, input, hostCpu, own);
        break;
    case 2:
        PokeathlonRingDropDisplay_SwitchIn(mon, position, ui, participant, input, report, own);
        break;
    case 0:
        if (changed) {
            PokeathlonRingDropDisplay_EndSwitch(mon, position, input, report, own);
            ov96_0221996C(ui, participant, packet->member, 0);
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

int PokeathlonRingDropDisplay_CompareRanks(const void *a, const void *b) {
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
    if (framePacket->marks.bits.switchPressed && !display->switchPressed) {
        PlaySE(SEQ_SE_GS_IDOUHIT);
    }
    participant = 0;
    index = 0;
    display->switchPressed = framePacket->marks.bits.switchPressed;
    packet = framePacket->mons;
    mon = display->mons;
    for (; participant < RING_DROP_PARTICIPANTS; participant++, packet++, mon++, index += RING_DROP_TEAM_SIZE) {
        BOOL cpu = PokeathlonCourse_GetParticipantCount(course) <= participant;
        BOOL host = ov96_021E5F24(course) == 0;
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
        if (participant == ov96_021E5F24(course)) {
            member = packet->member;
            {
                u32 marks = framePacket->marks.raw >> 8;
                int next = (u8)((member + 2) % RING_DROP_TEAM_SIZE + index);
                BOOL nextReady = ((marks >> (next * 2)) & 3) == 1;
                int afterNext = (u8)((member + 1) % RING_DROP_TEAM_SIZE + index);
                BOOL afterNextReady = ((marks >> (afterNext * 2)) & 3) == 1;
                ov96_02219770(display->ui, nextReady, afterNextReady);
            }
        }
        if (mon->hit && packet->hit) {
            int other = packet->opponent;
            showHit = FALSE;
            if (other <= participant) {
                other--;
            }
            if (packet->hit == 1) {
                if (other == ov96_021E5F24(course)) {
                    PokeathlonScreenShake_Start(&ringDrop->shake, 4, 10);
                }
                showHit = TRUE;
                PlaySE(SEQ_SE_GS_TACKLEHIT);
            } else if (packet->hit == 2) {
                if (other == ov96_021E5F24(course)) {
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
        mon->opponent = packet->opponent;
        PokeathlonCourseMonObject_SetAnimState(monObject, packet->power + 1);
        if (packet->state == 4 && mon->state != 4) {
            ov96_021EAF60(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][1]);
        } else if (mon->state == 4 && packet->state != 4) {
            ov96_021EAF60(ringDrop->monObjects, objectIndex, 1, mon->palettes[team][0]);
        }
        if (packet->state == 5 && mon->state != 5) {
            if (participant == ov96_021E5F24(course)) {
                PlaySE_SetPitch(SEQ_SE_GS_TACKLEHATUDOU, 256);
            } else {
                PlaySE_SetVolume(SEQ_SE_GS_TACKLEHATUDOU2, 64);
            }
        } else if (packet->state == 12 && mon->state != 12) {
            if (ov96_0221935C(course, (u8)participant, team)[24]) {
                PlaySE(SEQ_SE_GS_PRESSCHAKUCHI);
            }
        }
        if (packet->state == 6 && mon->state != 6) {
            ManagedSprite_SetAnimNoRestart(mon->chargeSprite, (u16)(packet->power + 11));
            ManagedSprite_SetDrawFlag(mon->chargeSprite, TRUE);
        } else if ((packet->state != 6 && mon->state == 6) || (packet->state == 7 && mon->state == 5)) {
            ManagedSprite_SetAnim(mon->chargeSprite, (u16)(packet->power + 15));
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
                ov96_0221996C(display->ui, (u8)participant, team, 1);
                if (participant == ov96_021E5F24(course) || hostCpu) {
                    mon->recordJump = TRUE;
                }
            } else if (!airborne && mon->airborne == TRUE) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(mon->dustSprite, packet->x, packet->y, FX32_CONST(480));
                ManagedSprite_SetDrawFlag(mon->dustSprite, TRUE);
                ManagedSprite_ResetSpriteAnimCtrlState(mon->dustSprite);
                ov96_022193CC(SEQ_SE_GS_BOWA2, participant == ov96_021E5F24(course));
                ov96_0221996C(display->ui, (u8)participant, team, 0);
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
                    if (ov96_02219940(ringDrop->ui, (u8)participant, (u8)mon->member)) {
                        mon->recordJump = FALSE;
                    }
                } else {
                    if (ov96_02219794(ringDrop->ui, (u8)participant, (u8)mon->member)) {
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
                u8 *record = ov96_0221935C(course, (u8)participant, team);
                u32 frames;
                if (!mon->squashFrames) {
                    if (!record[24]) {
                        if (participant == ov96_021E5F24(course)) {
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
                    ov96_02218510(monObject, 1);
                    mon->squashFrames = 0;
                }
            } else {
                ov96_02218510(monObject, 0);
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

    packet = (PokeathlonRingDropFramePacket *)(void *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + RING_DROP_DATA_FRAME_IN);
    ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonRingDropDisplay_ApplyFramePacket(display, packet, course);
    PokeathlonCourse_UpdateTimeUpCountdown(course, packet->timer);
    if (packet->timer == 450) {
        PlaySE(SEQ_SE_DP_TRAIN04);
    }
    status = packet->mons[ov96_021E5F24(course)].status;
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
        sorted[i][0] = packet->mons[i].rank;
    }
    MATH_QSort(sorted, RING_DROP_PARTICIPANTS, 2, PokeathlonRingDropDisplay_CompareRanks, NULL);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        if (packet->mons[i].rank != display->ranks[i]) {
            ov96_0221966C(display->ui, (u8)i, packet->mons[i].rank, sorted);
        }
        display->ranks[i] = packet->mons[i].rank;
    }
    ov96_022196E4(ringDrop->ui, packet->timer / 30);
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

BOOL ov96_0221785C(int state) {
    switch (state) {
    case 11:
        return FALSE;
    default:
        return TRUE;
    }
}

#pragma force_active reset

void ov96_02217868(PokeathlonRingDropMon *mon, int a1) {
    if (mon->state != 6 && mon->state != 11) {
        mon->flags.bits.direction = ov96_022186CC(a1);
    }
}
