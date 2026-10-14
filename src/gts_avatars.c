#include "global.h"

#include "constants/sndseq.h"

#include "gf_gfx_loader.h"
#include "gts_internal.h"
#include "heap.h"
#include "overlay_manager.h"
#include "save_special_ribbons.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "touchscreen.h"
#include "union_room_avatar.h"
#include "pokemon_summary.h"

// The avatars on the sub screen: the player walking in and out of the GTS, the trainers a search found (in the
// Union Room avatar graphics), the marker over the selected one. Then screen 8: the Pokemon summary of the
// Pokemon chosen on screen 5.

// The player's walk in / walk out task.
typedef struct GtsPlayerAnim {
    int state;
    int y;
    int gender;
    GtsApp *app;
} GtsPlayerAnim;

// The OBJ char offset of each trainer avatar.
extern const u16 sGtsAvatarCharOffsets[];

// {x, y} of each trainer avatar; the asm labels the y column separately.
typedef struct GtsAvatarY {
    u16 y;
    u16 nextX;
} GtsAvatarY;

extern const u16 sGtsAvatarPositions[];
extern const GtsAvatarY sGtsAvatarY[];
extern const TouchscreenHitbox sGtsAvatarHitboxes[];
extern const u8 sGtsViewMonSummaryPages[];

void Gts_CreateAvatars(GtsApp *app, u32 gender);
void Gts_StartPlayerEnterAnim(GtsApp *app, int gender);
void Gts_SetPlayerAnim(GtsPlayerAnim *anim, int seq);
void Gts_PlayerEnterTask(SysTask *task, GtsPlayerAnim *anim);
void Gts_StartPlayerExitAnim(GtsApp *app, int gender);
void Gts_PlayerExitTask(SysTask *task, GtsPlayerAnim *anim);
void Gts_SetAvatarPosition(Sprite *sprite, int x, int y);
int Gts_GetTouchedAvatar(int count);
void Gts_ShowResultAvatars(GtsApp *app, int count, int animate);
int Gts_GetAvatarAnimSeq(int index);
void Gts_StopAvatarAnims(GtsApp *app);
void Gts_LoadAvatarGraphics(GtsApp *app);
void Gts_LoadAvatar(NNSG2dCharacterData *charData, NNSG2dPaletteData *plttData, u32 index, int trainerClass, int gender);
void Gts_FreeAvatarGraphics(GtsApp *app);
void Gts_SetAvatarMarker(GtsApp *app, u32 index, u32 scrollY);
void Gts_ResetAvatarPositions(GtsApp *app);
void Gts_LowerAvatars(GtsApp *app);
int GtsViewMon_Init(GtsApp *app, int state);
int GtsViewMon_Main(GtsApp *app, int state);
int GtsViewMon_Exit(GtsApp *app, int state);

// The player at the GTS counter, the seven trainer avatars, the marker and the sub screen panel's sprite.
void Gts_CreateAvatars(GtsApp *app, u32 gender) {
    SpriteTemplate template;
    int i;
    const u16 *position;
    GtsApp *slot;
    int seq;

    Gts_LoadAvatarGraphics(app);
    Gts_InitSpriteTemplate(&template, app, &app->headers[1], NNS_G2D_VRAM_TYPE_2DSUB);
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.position.x = 128 * FX32_ONE;
    template.position.y = 386 * FX32_ONE;
    app->avatarSprites[0] = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->avatarSprites[0], TRUE);
    Sprite_SetPriority(app->avatarSprites[0], 2);
    Sprite_SetAnimCtrlSeq(app->avatarSprites[0], gender * 7 + 3);
    Sprite_SetDrawFlag(app->avatarSprites[0], TRUE);
    i = 0;
    position = sGtsAvatarPositions;
    // The loops walk a base pointer shifted by one array element per avatar (the original strength reduction).
    slot = app;
    seq = 14;
    for (; i < 7; i++) {
        slot->avatarSprites[1] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(slot->avatarSprites[1], TRUE);
        Sprite_SetAnimCtrlSeq(slot->avatarSprites[1], seq);
        Sprite_SetDrawFlag(slot->avatarSprites[1], FALSE);
        Gts_SetAvatarPosition(slot->avatarSprites[1], position[0], position[1]);
        Sprite_SetPriority(slot->avatarSprites[1], 2);
        slot = (GtsApp *)((u8 *)slot + sizeof(Sprite *));
        seq += 4;
        position += 2;
    }
    app->avatarMarkerSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->avatarMarkerSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->avatarMarkerSprite, 43);
    Sprite_SetDrawFlag(app->avatarMarkerSprite, FALSE);
    Gts_SetAvatarPosition(app->avatarMarkerSprite, 128, 86);
    Sprite_SetPriority(app->avatarMarkerSprite, 1);
    app->panelSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->panelSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->panelSprite, 42);
    Gts_SetSpritePosition(app->panelSprite, 55, app->scrollY + 424);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    slot = app;
    for (i = 0; i < 8; i++) {
        VecFx32 *matrix = Sprite_GetMatrixPtr(slot->avatarSprites[0]);
        slot->avatarPositions[0][0] = matrix->x / FX32_ONE;
        slot->avatarPositions[0][1] = matrix->y / FX32_ONE;
        slot = (GtsApp *)((u8 *)slot + sizeof(Sprite *));
    }
}

// The player walks down to the counter; playerAnimDone is set at the end.
void Gts_StartPlayerEnterAnim(GtsApp *app, int gender) {
    SysTask *task;
    GtsPlayerAnim *anim;

    Gts_CreateAvatars(app, gender);
    task = CreateSysTaskAndEnvironment((SysTaskFunc)Gts_PlayerEnterTask, sizeof(GtsPlayerAnim), 5, HEAP_ID_61);
    app->playerAnimTask = task;
    anim = SysTask_GetData(app->playerAnimTask);
    anim->state = 0;
    anim->y = -40;
    anim->gender = gender;
    anim->app = app;
    Gts_SetPlayerAnim(anim, 0);
    PlaySE(SEQ_SE_DP_PYUU);
}

void Gts_SetPlayerAnim(GtsPlayerAnim *anim, int seq) {
    Sprite *sprite = anim->app->avatarSprites[0];
    Sprite_SetAnimCtrlSeq(sprite, seq + 7 * anim->gender);
}

void Gts_PlayerEnterTask(SysTask *task, GtsPlayerAnim *anim) {
    GtsApp *app = anim->app;
    switch (anim->state) {
    case 0:
        if (anim->y > 160) {
            anim->y = 160;
            anim->state = 1;
            Gts_SetPlayerAnim(anim, 1);
        }
        anim->y += 5;
        Gts_SetAvatarPosition(app->avatarSprites[0], 128, anim->y);
        break;
    case 1:
        if (!Sprite_IsAnimated(app->avatarSprites[0])) {
            Gts_SetPlayerAnim(anim, 2);
            anim->state = 2;
        }
        break;
    case 2:
        if (anim->y <= 138) {
            anim->y = 138;
            anim->state = 3;
            Gts_SetPlayerAnim(anim, 3);
        } else {
            anim->y -= 2;
        }
        Gts_SetAvatarPosition(app->avatarSprites[0], 128, anim->y);
        break;
    case 3:
        PlaySE(SEQ_SE_DP_PC_LOGIN);
        anim->app->playerAnimDone = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

// The player walks away; playerAnimDone is set at the end.
void Gts_StartPlayerExitAnim(GtsApp *app, int gender) {
    SysTask *task;
    GtsPlayerAnim *anim;

    task = CreateSysTaskAndEnvironment((SysTaskFunc)Gts_PlayerExitTask, sizeof(GtsPlayerAnim), 5, HEAP_ID_61);
    app->playerAnimTask = task;
    anim = SysTask_GetData(app->playerAnimTask);
    anim->state = 0;
    anim->y = 138;
    anim->gender = gender;
    anim->app = app;
    Gts_SetPlayerAnim(anim, 5);
    PlaySE(SEQ_SE_DP_PC_LOGOFF);
}

void Gts_PlayerExitTask(SysTask *task, GtsPlayerAnim *anim) {
    GtsApp *app = anim->app;
    switch (anim->state) {
    case 0:
        if (anim->y > 160) {
            anim->y = 160;
            anim->state = 1;
            Gts_SetPlayerAnim(anim, 6);
        }
        anim->y += 2;
        Gts_SetAvatarPosition(app->avatarSprites[0], 128, anim->y);
        break;
    case 1:
        if (!Sprite_IsAnimated(app->avatarSprites[0])) {
            Gts_SetPlayerAnim(anim, 0);
            anim->state = 2;
            PlaySE(SEQ_SE_DP_PYUU2);
        }
        break;
    case 2:
        if (anim->y < -20) {
            anim->state = 3;
            Gts_SetPlayerAnim(anim, 3);
        }
        anim->y -= 5;
        Gts_SetAvatarPosition(app->avatarSprites[0], 128, anim->y);
        break;
    case 3:
        app->playerAnimDone = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

// The avatars are on the sub screen, 256 px below the main screen; y is their feet.
void Gts_SetAvatarPosition(Sprite *sprite, int x, int y) {
    VecFx32 position;
    float f;

    if (x > 0) {
        f = (float)(x << FX32_SHIFT) + 0.5f;
    } else {
        f = (float)(x << FX32_SHIFT) - 0.5f;
    }
    position.x = (s32)f;
    y -= 8;
    if (y > 0) {
        f = (float)(y << FX32_SHIFT) + 0.5f;
    } else {
        f = (float)(y << FX32_SHIFT) - 0.5f;
    }
    position.y = (s32)f + 256 * FX32_ONE;
    position.z = 0;
    Sprite_SetMatrix(sprite, &position);
}

int Gts_GetTouchedAvatar(int count) {
    int avatar = TouchscreenHitbox_FindRectAtTouchNew(sGtsAvatarHitboxes);
    if (avatar == -1 || avatar >= count) {
        avatar = -1;
    }
    return avatar;
}

#pragma force_active on

// Shows the trainers of the first count search results (animate: they walk in).
void Gts_ShowResultAvatars(GtsApp *app, int count, int animate) {
    int i;
    int walkSeq;
    int standSeq;
    GtsApp *result;
    GtsApp *slot;

    if (count && animate == TRUE) {
        PlaySE(SEQ_SE_DP_TELE2);
    }
    // Base pointers shifted by one result / one sprite per avatar (the original strength reduction).
    result = app;
    i = 0;
    walkSeq = 14;
    slot = app;
    standSeq = 17;
    for (; i < 7; i++) {
        if (i < count) {
            Gts_LoadAvatar(app->avatarCharData, app->avatarPlttData, i, result->searchResults[0].trainerClass, result->searchResults[0].trainerGender);
            if (animate) {
                Sprite_SetAnimCtrlSeq(slot->avatarSprites[1], walkSeq);
            } else {
                Sprite_SetAnimCtrlSeq(slot->avatarSprites[1], standSeq);
            }
            Sprite_SetDrawFlag(slot->avatarSprites[1], TRUE);
        } else {
            Sprite_SetDrawFlag(slot->avatarSprites[1], FALSE);
        }
        result = (GtsApp *)((u8 *)result + sizeof(GtsRecord));
        walkSeq += 4;
        slot = (GtsApp *)((u8 *)slot + sizeof(Sprite *));
        standSeq += 4;
    }
}

#pragma force_active reset

int Gts_GetAvatarAnimSeq(int index) {
    return index * 4 + 14;
}

// The shown trainers stop walking.
void Gts_StopAvatarAnims(GtsApp *app) {
    s32 i;
    GtsApp *slot = app; // shifted by one sprite per avatar (the original strength reduction)

    for (i = 0; i < 7; i++, slot = (GtsApp *)((u8 *)slot + sizeof(Sprite *))) {
        Sprite *avatar = slot->avatarSprites[1];
        if (Sprite_GetDrawFlag(avatar)) {
            u32 seq = Sprite_GetAnimationNumber(slot->avatarSprites[1]);
            if (seq != Gts_GetAvatarAnimSeq(i) + 1) {
                Sprite_SetAnimCtrlSeq(slot->avatarSprites[1], Gts_GetAvatarAnimSeq(i) + 1);
            }
        }
    }
}

// The Union Room avatar graphics the trainers are drawn with.
void Gts_LoadAvatarGraphics(GtsApp *app) {
    app->avatarPlttAlloc = GfGfxLoader_GetPlttData(NARC_application_record_record, 11, &app->avatarPlttData, HEAP_ID_61);
    app->avatarCharAlloc = GfGfxLoader_GetCharData(NARC_application_record_record, 12, TRUE, &app->avatarCharData, HEAP_ID_61);
    DC_FlushRange(app->avatarCharData, 0x8000);
}

void Gts_LoadAvatar(NNSG2dCharacterData *charData, NNSG2dPaletteData *plttData, u32 index, int trainerClass, int gender) {
    u32 avatar = SpriteToUnionRoomAvatarIdx(gender, trainerClass);
    void *palettes = plttData->pRawData;
    GXS_LoadOBJ((u8 *)charData->pRawData + avatar * 0x600, sGtsAvatarCharOffsets[index], 0x200);
    GXS_LoadOBJPltt((u8 *)palettes + avatar * 0x20, (index + 2) * 0x20, 0x20);
}

// gts.c, at exit: the avatar graphics exist once the player walked in.
void Gts_FreeAvatarGraphics(GtsApp *app) {
    if (app->playerAnimDone) {
        Heap_Free(app->avatarPlttAlloc);
        Heap_Free(app->avatarCharAlloc);
    }
}

void Gts_SetAvatarMarker(GtsApp *app, u32 index, u32 scrollY) {
    Gts_SetAvatarPosition(app->avatarMarkerSprite, sGtsAvatarPositions[index * 2], scrollY + (sGtsAvatarY[index].y + 32));
}

void Gts_ResetAvatarPositions(GtsApp *app) {
    s32 i;
    for (i = 0; i < 8; i++) {
        Gts_SetSpritePosition(app->avatarSprites[i], app->avatarPositions[i][0], app->avatarPositions[i][1]);
    }
}

void Gts_LowerAvatars(GtsApp *app) {
    s32 i;
    for (i = 0; i < 8; i++) {
        Gts_SetSpritePosition(app->avatarSprites[i], app->avatarPositions[i][0], app->avatarPositions[i][1] + 32);
    }
}

// Screen 8: the summary of the Pokemon at selectedBox/selectedSlot, then back to screen 5.
int GtsViewMon_Init(GtsApp *app, int state) {
    app->summaryArgs.party = (Party *)Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
    app->summaryArgs.monDataType = 2;
    app->summaryArgs.partyCount = 1;
    app->summaryArgs.partySlot = 0;
    app->summaryArgs.mode = 1;
    app->summaryArgs.moveToLearn = 0;
    app->summaryArgs.unk2C = PokemonSummary_GetDefaultUnk2C(app->args->saveData);
    app->summaryArgs.natDexEnabled = app->args->natDexEnabled;
    app->summaryArgs.options = app->args->options;
    app->summaryArgs.ribbons = Save_SpecialRibbons_Get(app->args->saveData);
    app->summaryArgs.isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(app->args->saveData);
    PokemonSummary_SetPages(&app->summaryArgs, sGtsViewMonSummaryPages);
    PokemonSummaryArgs_SetPlayer(&app->summaryArgs, app->args->profile);
    app->tradeSequence = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &app->summaryArgs, HEAP_ID_61);
    app->reloadGraphics = TRUE;
    return 2;
}

int GtsViewMon_Main(GtsApp *app, int state) {
    int ret = 3;
    if (OverlayManager_Run(app->tradeSequence)) {
        OverlayManager_Delete(app->tradeSequence);
        Gts_SetNextScreen(app, 5, app->nextScreenArg);
        ret = 4;
    }
    return ret;
}

int GtsViewMon_Exit(GtsApp *app, int state) {
    Gts_SwitchScreen(app);
    return 1;
}
