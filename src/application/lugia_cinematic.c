#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "follow_mon.h"
#include "legendary_cinematic.h"
#include "legendary_cinematic_internal.h"
#include "screen_fade.h"
#include "sound_playback.h"

BOOL PlaySE_SetVolume(u16 seqNo, u16 volume);
void GF_SetVolumeBySeqNo(u16 seqNo, int volume);

static int LugiaCinematic_Init(LegendaryCinematic *cinematic);
static int LugiaCinematic_Exit(LegendaryCinematic *cinematic);
static int LugiaCinematic_WaitFade(LegendaryCinematic *cinematic);
static int LugiaCinematic_Play(LegendaryCinematic *cinematic);

static const GXRgb sLugiaCinematicEdgeColors[] = {
    GX_RGB(0, 0, 0),
    GX_RGB(9, 20, 30),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
};

static int (*const sScriptCinematicSubs_Lugia[4])(LegendaryCinematic *) = {
    LugiaCinematic_Init,
    LugiaCinematic_Exit,
    LugiaCinematic_WaitFade,
    LugiaCinematic_Play,
};

static const LegendaryCinematicModelSpec sLugiaCinematicModels[] = {
    { .model = 0, .anims = { 1, 2, 3, 4 },                                           .noMaterialColors = FALSE },
    { .model = 5, .anims = { 6, MODEL_ANIM_NONE, MODEL_ANIM_NONE, MODEL_ANIM_NONE }, .noMaterialColors = TRUE  },
};

static const LegendaryCinematicCamera sLugiaCinematicCamera = {
    .param.distance = FX32_CONST(339.707275390625),
    .param.angle.x = 0x982,
    .param.angle.y = 0x1E20,
    .param.angle.z = 0,
    .param.perspectiveType = CAMERA_PERSPECTIVE_TYPE_PERSPECTIVE,
    .param.perspective = 0xA01,
    .target.x = FX32_CONST(4.8701171875),
    .target.y = FX32_CONST(128.237548828125),
    .target.z = FX32_CONST(43.184814453125),
    .nearClip = FX32_CONST(123),
    .farClip = FX32_CONST(1700),
};

int ScriptCinematic_Lugia(void *data) {
    LegendaryCinematic *cinematic = data;
    cinematic->state = sScriptCinematicSubs_Lugia[cinematic->state](cinematic);
    if (cinematic->state == 4) {
        return FALSE;
    }
    LegendaryCinematic_DrawModels(cinematic);
    return TRUE;
}

static int LugiaCinematic_Init(LegendaryCinematic *cinematic) {
    LegendaryCinematic_Init3D(cinematic);
    LegendaryCinematic_SetCamera(cinematic, &sLugiaCinematicCamera);
    LegendaryCinematic_SetLights((ModelAttributes *)&cinematic->args->modelAttributes);
    LegendaryCinematic_SetEdgeMarking(sLugiaCinematicEdgeColors);
    LegendaryCinematic_LoadModels(cinematic, NARC_a_2_4_4, sLugiaCinematicModels, 2);
    LegendaryCinematic_FadeInFromBlack(cinematic, 6, 1, 3);
    return 2;
}

static int LugiaCinematic_Exit(LegendaryCinematic *cinematic) {
    LegendaryCinematic_FreeModels(cinematic);
    LegendaryCinematic_Free3D(cinematic);
    return 4;
}

static int LugiaCinematic_WaitFade(LegendaryCinematic *cinematic) {
    if (IsPaletteFadeFinished() == TRUE) {
        return cinematic->nextState;
    }
    return 2;
}

static int LugiaCinematic_Play(LegendaryCinematic *cinematic) {
    if (LegendaryCinematic_AreModelsAnimating(cinematic) == FALSE && IsPaletteFadeFinished() == TRUE) {
        cinematic->frame = 0;
        StopSE(SEQ_SE_GS_TAKI_RUGIADEMO, 0);
        LegendaryCinematic_StopMotionBlur(cinematic->capture);
        return 1;
    }
    if (cinematic->frame == 0) {
        PlaySE_SetVolume(SEQ_SE_GS_TAKI_RUGIADEMO, 0);
    } else if (cinematic->frame == 115) {
        PlaySE(SEQ_SE_GS_RUGIA_TAKISHUTUGEN);
    } else if (cinematic->frame == 145) {
        PlayCry(SPECIES_LUGIA, 0);
        cinematic->capture = LegendaryCinematic_StartMotionBlur(5, 13);
    } else if (cinematic->frame == 232) {
        LegendaryCinematic_FadeOutToBlack(cinematic, 6, 1, 1);
    }
    // the waterfall sound fades in over the first ten frames
    if (cinematic->frame != 0) {
        if (cinematic->frame == 10) {
            GF_SetVolumeBySeqNo(SEQ_SE_GS_TAKI_RUGIADEMO, 30);
        } else if (cinematic->frame < 10) {
            GF_SetVolumeBySeqNo(SEQ_SE_GS_TAKI_RUGIADEMO, cinematic->frame * 0x300 / 0x100);
        }
    }
    cinematic->frame++;
    return 3;
}
