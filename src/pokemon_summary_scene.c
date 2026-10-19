#include "global.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "pokemon.h"
#include "pokemon_summary_internal.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "touchscreen.h"

// A condition chart vertex: at 0 (min), at 255 (max), and its step per condition point.
typedef struct ConditionChartVertexAnim {
    u16 max[3];
    u16 min[3];
    u16 step[3];
} ConditionChartVertexAnim;

typedef struct ConditionChartVertexAnimS {
    s16 max[3];
    s16 min[3];
    s16 step[3];
} ConditionChartVertexAnimS;

static void ConditionChart_GetVertex(const ConditionChartVertexAnim *src, ConditionChartVertex *dst, int t);
static void ConditionChart_GetStep(const ConditionChartVertex *from, const ConditionChartVertex *to, ConditionChartVertex *out);
static int PokemonSummary_GetTouchedButton(PokemonSummary *summary);

void PokemonSummary_Init3D(void);

void PokemonSummary_Init3D(void) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    G3_ViewPort(0, 0, 255, 191);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(2);
}

void PokemonSummary_DrawPokepic(PokemonSummary *summary) {
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(summary->pokepicManager);
    *(vu32 *)0x04000540 = 1;
}

void PokemonSummary_FreePokepicAndCamera(PokemonSummary *summary) {
    Camera_Delete(summary->camera);
    PokepicAnimManager_StopAnim(summary->pokepicAnimManager, 0);
    PokepicAnimManager_Delete(summary->pokepicAnimManager);
    PokepicManager_Delete(summary->pokepicManager);
}

static const VecFx32 sSummaryCameraPos = { 0, 0, FX32_CONST(16) };

void PokemonSummary_CreateCamera(PokemonSummary *summary) {
    VecFx32 pos = sSummaryCameraPos;
    CameraAngle angle = { 0, 0, 0, 0 };
    summary->camera = Camera_New(HEAP_ID_19);
    Camera_Init_FromPosDistanceAndAngle(&pos, 0x10000, &angle, 0x5C1, 1, summary->camera);
    Camera_SetPerspectiveClippingPlane(0, 0x64000, summary->camera);
    Camera_ClearFixedTarget(summary->camera);
    Camera_SetStaticPtr(summary->camera);
}

static void ConditionChart_GetVertex(const ConditionChartVertexAnim *src, ConditionChartVertex *dst, int t) {
    if (t == 0xFF) {
        dst->x = src->max[0];
        dst->y = src->max[1];
        dst->z = src->max[2];
        return;
    }
    if (t == 0) {
        dst->x = src->min[0];
        dst->y = src->min[1];
        dst->z = src->min[2];
        return;
    }
    dst->x = ((const ConditionChartVertexAnimS *)src)->min[0] + ((const ConditionChartVertexAnimS *)src)->step[0] * t;
    dst->y = ((const ConditionChartVertexAnimS *)src)->min[1] + ((const ConditionChartVertexAnimS *)src)->step[1] * t;
    dst->z = ((const ConditionChartVertexAnimS *)src)->min[2] + ((const ConditionChartVertexAnimS *)src)->step[2] * t;
}

static void ConditionChart_GetStep(const ConditionChartVertex *from, const ConditionChartVertex *to, ConditionChartVertex *out) {
    int diff;
    s16 rounded;

    diff = to->x - from->x;
    if ((float)diff / 4096.0f / 4.0f > 0.0f) {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f + 0.5f);
    } else {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f - 0.5f);
    }
    out->x = rounded;

    diff = to->y - from->y;
    if ((float)diff / 4096.0f / 4.0f > 0.0f) {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f + 0.5f);
    } else {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f - 0.5f);
    }
    out->y = rounded;

    diff = to->z - from->z;
    if ((float)diff / 4096.0f / 4.0f > 0.0f) {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f + 0.5f);
    } else {
        rounded = (s16)(((float)diff / 4096.0f / 4.0f) * 4096.0f - 0.5f);
    }
    out->z = rounded;
}

static const ConditionChartVertexAnim sConditionChartQuad3Vertex2 = {
    { 3106, -2955, 0 },
    { 4884, -106,  0 },
    { -7,   -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad3Vertex3 = {
    { 7079, -2955, 0 },
    { 5380, -106,  0 },
    { 7,    -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad0Vertex1 = {
    { 8344, 965, 0 },
    { 5538, 383, 0 },
    { 11,   2,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad3Vertex0 = {
    { 5138, 300, 0 },
    { 5138, 300, 0 },
    { 0,    0,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad2Vertex2 = {
    { 3106, -2955, 0 },
    { 4803, -106,  0 },
    { -7,   -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad2Vertex3 = {
    { 7079, -2955, 0 },
    { 5299, -106,  0 },
    { 7,    -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad2Vertex1 = {
    { 5045, 300, 0 },
    { 5045, 300, 0 },
    { 0,    0,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad2Vertex0 = {
    { 1843, 965, 0 },
    { 4645, 383, 0 },
    { -11,  2,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad1Vertex2 = {
    { 3106, -2955, 0 },
    { 4803, -106,  0 },
    { -7,   -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad1Vertex3 = {
    { 5045, 300, 0 },
    { 5045, 300, 0 },
    { 0,    0,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad1Vertex1 = {
    { 5045, 3784, 0 },
    { 5045, 735,  0 },
    { 0,    12,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad1Vertex0 = {
    { 1843, 965, 0 },
    { 4645, 383, 0 },
    { -11,  2,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad0Vertex2 = {
    { 5138, 300, 0 },
    { 5138, 300, 0 },
    { 0,    0,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad0Vertex3 = {
    { 7079, -2955, 0 },
    { 5380, -106,  0 },
    { 7,    -11,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad3Vertex1 = {
    { 8344, 965, 0 },
    { 5538, 383, 0 },
    { 11,   2,   0 },
};

static const ConditionChartVertexAnim sConditionChartQuad0Vertex0 = {
    { 5138, 3784, 0 },
    { 5138, 735,  0 },
    { 0,    12,   0 },
};

void PokemonSummary_InitConditionChart(PokemonSummary *summary) {
    u32 i;

    ConditionChart_GetVertex(&sConditionChartQuad0Vertex0, &summary->conditionChartTo[0].vertices[0], summary->mon.contestStats[0]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex1, &summary->conditionChartTo[0].vertices[1], summary->mon.contestStats[1]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex3, &summary->conditionChartTo[0].vertices[3], summary->mon.contestStats[2]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex2, &summary->conditionChartTo[0].vertices[2], 0);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex0, &summary->conditionChartTo[1].vertices[0], summary->mon.contestStats[4]);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex1, &summary->conditionChartTo[1].vertices[1], summary->mon.contestStats[0]);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex3, &summary->conditionChartTo[1].vertices[3], 0);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex2, &summary->conditionChartTo[1].vertices[2], summary->mon.contestStats[3]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex0, &summary->conditionChartTo[2].vertices[0], summary->mon.contestStats[4]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex1, &summary->conditionChartTo[2].vertices[1], 0);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex3, &summary->conditionChartTo[2].vertices[3], summary->mon.contestStats[2]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex2, &summary->conditionChartTo[2].vertices[2], summary->mon.contestStats[3]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex0, &summary->conditionChartTo[3].vertices[0], 0);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex1, &summary->conditionChartTo[3].vertices[1], summary->mon.contestStats[1]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex3, &summary->conditionChartTo[3].vertices[3], summary->mon.contestStats[2]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex2, &summary->conditionChartTo[3].vertices[2], summary->mon.contestStats[3]);

    for (i = 0; i < 4; i++) {
        ConditionChart_GetStep(&summary->conditionChartFrom[i].vertices[0], &summary->conditionChartTo[i].vertices[0], &summary->conditionChartStep[i].vertices[0]);
        ConditionChart_GetStep(&summary->conditionChartFrom[i].vertices[1], &summary->conditionChartTo[i].vertices[1], &summary->conditionChartStep[i].vertices[1]);
        ConditionChart_GetStep(&summary->conditionChartFrom[i].vertices[2], &summary->conditionChartTo[i].vertices[2], &summary->conditionChartStep[i].vertices[2]);
        ConditionChart_GetStep(&summary->conditionChartFrom[i].vertices[3], &summary->conditionChartTo[i].vertices[3], &summary->conditionChartStep[i].vertices[3]);
    }
    summary->conditionChartDone = 0;
}

void PokemonSummary_CreatePokepic(PokemonSummary *summary) {
    PokepicTemplate pokepicTemplate;
    void *mon;

    summary->pokepicManager = PokepicManager_Create(HEAP_ID_19);
    mon = PokemonSummary_GetShownMon(summary);
    if (summary->args->monDataType == 2) {
        GetBoxmonSpriteCharAndPlttNarcIds(&pokepicTemplate, mon, 2, FALSE);
    } else {
        GetPokemonSpriteCharAndPlttNarcIds(&pokepicTemplate, mon, 2);
    }
    NARC_ReadPokepicAnimScript(summary->pokepicAnimNarc, summary->pokepicAnimScript, summary->mon.species, 1);
    summary->pokepicAnimReverseX = 0;
    summary->pokepic = PokepicManager_CreatePokepic(summary->pokepicManager, &pokepicTemplate, 0xd0, 0x68, 0, 0, summary->pokepicAnimScript, NULL);
    Pokepic_SetAttr(summary->pokepic, 6, 1);
    Pokepic_SetAttr(summary->pokepic, 0x23, 0);
}

void PokemonSummary_StartPokepicAnim(PokemonSummary *summary) {
    if (summary->mon.isEgg) {
        NARC_StartPokepicAnim(summary->pokepicAnimNarc, summary->pokepicAnimManager, summary->pokepic, 0, 2, summary->pokepicAnimReverseX, 0);
    } else {
        Pokepic_StartAnim(summary->pokepic, 1);
        NARC_StartPokepicAnim(summary->pokepicAnimNarc, summary->pokepicAnimManager, summary->pokepic, summary->mon.species, 2, summary->pokepicAnimReverseX, 0);
    }
}

void PokemonSummary_ReloadPokepic(PokemonSummary *summary) {
    PokepicAnimManager_StopAnim(summary->pokepicAnimManager, 0);
    PokepicManager_Delete(summary->pokepicManager);
    PokemonSummary_CreatePokepic(summary);
    PokemonSummary_StartPokepicAnim(summary);
}

static const u8 sTouchButtons[][4] = {
    { 0,    1,    1,    4    },
    { 1,    2,    5,    15   },
    { 2,    4,    25,   10   },
    { 3,    4,    26,   4    },
    { 4,    0,    0,    0    },
    { 5,    0,    0,    0    },
    { 6,    0,    0,    0    },
    { 7,    0,    0,    0    },
    { 8,    0,    0,    0    },
    { 9,    0,    0,    0    },
    { 0xFF, 0xFF, 0xFF, 0xFF },
};

static const TouchscreenHitbox sTouchButtonHitboxes[] = {
    { .rect = { 165, 191, 2, 45 } },
    { .rect = { 165, 191, 48, 96 } },
    { .rect = { 165, 191, 99, 140 } },
    { .rect = { 165, 191, 189, 250 } },
    { .rect = { 38, 66, 165, 203 } },
    { .rect = { 46, 74, 205, 243 } },
    { .rect = { 70, 98, 165, 203 } },
    { .rect = { 78, 106, 205, 243 } },
    { .rect = { 102, 130, 165, 203 } },
    { .rect = { 110, 138, 205, 243 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static int PokemonSummary_GetTouchedButton(PokemonSummary *summary) {
    int button;

    if (summary->unk7BF_0 == 0xF) {
        return -1;
    }
    if (summary->args->monDataType != 2) {
        button = TouchscreenHitbox_FindRectAtTouchNew(sTouchButtonHitboxes);
        if (button != -1) {
            button = sTouchButtons[button][0];
        }
        return button;
    } else {
        button = TouchscreenHitbox_FindRectAtTouchNew(sTouchButtonHitboxes);
        if (button >= 4 && button <= 9) {
            return -1;
        }
        if (button != -1) {
            button = sTouchButtons[button][0];
        }
        return button;
    }
}

u8 PokemonSummary_CheckTouchedButton(PokemonSummary *summary) {
    int button = PokemonSummary_GetTouchedButton(summary);
    if (button != -1) {
        return (u8)button;
    }
    return 0xFF;
}
