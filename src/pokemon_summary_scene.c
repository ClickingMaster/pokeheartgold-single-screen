#include "global.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "touchscreen.h"

// The parts of the summary screen state this module uses.
typedef struct PokemonSummaryScene {
    u8 pad_000[0x22C];
    void *args; // 0x22C
    u8 pad_230[0x23C - 0x230];
    u16 species; // 0x23C
    u8 pad_23E[0x280 - 0x23E];
    u32 status : 28; // 0x280
    u32 isEgg : 1;
    u32 unk_280_29 : 3;
    u8 pad_284[0x29C - 0x284];
    void *camera;             // 0x29C
    void *pokepicManager;     // 0x2A0
    u32 pokepicAnimScript[2]; // 0x2A4
    u8 pad_2AC[0x2CC - 0x2AC];
    void *pokepicAnimManager; // 0x2CC
    void *pokepic;            // 0x2D0
    u32 pokepicAnimReverseX;  // 0x2D4
    u8 pad_2D8[0x7B8 - 0x2D8];
    void *pokepicAnimNarc; // 0x7B8
    u8 pad_7BC[3];
    u8 unk_7BF_0 : 4; // 0x7BF
    u8 unk_7BF_4 : 4;
} PokemonSummaryScene;

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

typedef struct ConditionChartVertex {
    s16 x, y, z;
} ConditionChartVertex;

struct PokemonSummaryArgsDataTypeView {
    u8 pad_00[0x11];
    u8 monDataType;
};

void NNS_G2dSetupSoftwareSpriteCamera(void);
void PokepicManager_DrawAll(void *mgr);
void PokepicManager_Delete(void *mgr);
void PokepicAnimManager_StopAnim(void *a, int b);
void PokepicAnimManager_Delete(void *a);
void Pokepic_StartAnim(void *p, int a);
void NARC_StartPokepicAnim(void *a, void *b, void *c, int d, int e, u32 f, int g);
void *PokepicManager_Create(int heapId);
int PokemonSummary_GetShownMon(PokemonSummaryScene *summary);
void GetPokemonSpriteCharAndPlttNarcIds(u32 *dest, int species, int c);
void GetBoxmonSpriteCharAndPlttNarcIds(u32 *dest, int species, int c, int d);
void NARC_ReadPokepicAnimScript(void *narc, void *dest, u16 species, int d);
void *PokepicManager_CreatePokepic(void *mgr, u32 *a, int x, int y, int e, int f, void *g, int h);
void Pokepic_SetAttr(void *pic, int attr, int value);

void PokemonSummary_DrawPokepic(PokemonSummaryScene *summary);
void PokemonSummary_FreePokepicAndCamera(PokemonSummaryScene *summary);
void PokemonSummary_InitConditionChart(PokemonSummaryScene *summary);
void PokemonSummary_CreatePokepic(PokemonSummaryScene *summary);
void PokemonSummary_StartPokepicAnim(PokemonSummaryScene *summary);
void PokemonSummary_ReloadPokepic(PokemonSummaryScene *summary);
u8 PokemonSummary_CheckTouchedButton(PokemonSummaryScene *summary);

static void ConditionChart_GetVertex(const ConditionChartVertexAnim *src, ConditionChartVertex *dst, int t);
static void ConditionChart_GetStep(const ConditionChartVertex *from, const ConditionChartVertex *to, ConditionChartVertex *out);
static int PokemonSummary_GetTouchedButton(PokemonSummaryScene *summary);

void PokemonSummary_Init3D(void);
void PokemonSummary_CreateCamera(PokemonSummaryScene *summary);

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

void PokemonSummary_DrawPokepic(PokemonSummaryScene *summary) {
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(summary->pokepicManager);
    *(vu32 *)0x04000540 = 1;
}

void PokemonSummary_FreePokepicAndCamera(PokemonSummaryScene *summary) {
    Camera_Delete(summary->camera);
    PokepicAnimManager_StopAnim(summary->pokepicAnimManager, 0);
    PokepicAnimManager_Delete(summary->pokepicAnimManager);
    PokepicManager_Delete(summary->pokepicManager);
}

static const VecFx32 sSummaryCameraPos = { 0, 0, FX32_CONST(16) };

void PokemonSummary_CreateCamera(PokemonSummaryScene *summary) {
    VecFx32 pos = sSummaryCameraPos;
    CameraAngle angle = { 0, 0, 0, 0 };
    summary->camera = Camera_New((enum HeapID)0x13);
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

typedef struct PictureVectorQuad {
    ConditionChartVertex vectors[4];
} PictureVectorQuad;

typedef struct PictureVectorWork {
    u8 pad_000[0x2D8];
    PictureVectorQuad from[4];
    PictureVectorQuad out[4];
    PictureVectorQuad to[4];
    u32 done;
} PictureVectorWork;

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

void PokemonSummary_InitConditionChart(PokemonSummaryScene *summary) {
    u32 i;
    u8 *raw = (u8 *)summary;
    PictureVectorWork *vectors = (PictureVectorWork *)summary;

    ConditionChart_GetVertex(&sConditionChartQuad0Vertex0, (ConditionChartVertex *)&raw[0x398], raw[0x275]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex1, (ConditionChartVertex *)&raw[0x39E], raw[0x276]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex3, (ConditionChartVertex *)&raw[0x3AA], raw[0x277]);
    ConditionChart_GetVertex(&sConditionChartQuad0Vertex2, (ConditionChartVertex *)&raw[0x3A4], 0);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex0, (ConditionChartVertex *)&raw[0x3B0], raw[0x279]);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex1, (ConditionChartVertex *)&raw[0x3B6], raw[0x275]);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex3, (ConditionChartVertex *)&raw[0x3C2], 0);
    ConditionChart_GetVertex(&sConditionChartQuad1Vertex2, (ConditionChartVertex *)&raw[0x3BC], raw[0x278]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex0, (ConditionChartVertex *)&raw[0x3C8], raw[0x279]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex1, (ConditionChartVertex *)&raw[0x3CE], 0);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex3, (ConditionChartVertex *)&raw[0x3DA], raw[0x277]);
    ConditionChart_GetVertex(&sConditionChartQuad2Vertex2, (ConditionChartVertex *)&raw[0x3D4], raw[0x278]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex0, (ConditionChartVertex *)&raw[0x3E0], 0);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex1, (ConditionChartVertex *)&raw[0x3E6], raw[0x276]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex3, (ConditionChartVertex *)&raw[0x3F2], raw[0x277]);
    ConditionChart_GetVertex(&sConditionChartQuad3Vertex2, (ConditionChartVertex *)&raw[0x3EC], raw[0x278]);

    for (i = 0; i < 4; i++) {
        ConditionChart_GetStep(&vectors->from[i].vectors[0], &vectors->to[i].vectors[0], &vectors->out[i].vectors[0]);
        ConditionChart_GetStep(&vectors->from[i].vectors[1], &vectors->to[i].vectors[1], &vectors->out[i].vectors[1]);
        ConditionChart_GetStep(&vectors->from[i].vectors[2], &vectors->to[i].vectors[2], &vectors->out[i].vectors[2]);
        ConditionChart_GetStep(&vectors->from[i].vectors[3], &vectors->to[i].vectors[3], &vectors->out[i].vectors[3]);
    }
    vectors->done = 0;
}

void PokemonSummary_CreatePokepic(PokemonSummaryScene *summary) {
    u32 pokepicTemplate[4];
    int species;

    summary->pokepicManager = PokepicManager_Create(0x13);
    species = PokemonSummary_GetShownMon(summary);
    if (((struct PokemonSummaryArgsDataTypeView *)summary->args)->monDataType == 2) {
        GetBoxmonSpriteCharAndPlttNarcIds(pokepicTemplate, species, 2, 0);
    } else {
        GetPokemonSpriteCharAndPlttNarcIds(pokepicTemplate, species, 2);
    }
    NARC_ReadPokepicAnimScript(summary->pokepicAnimNarc, summary->pokepicAnimScript, summary->species, 1);
    summary->pokepicAnimReverseX = 0;
    summary->pokepic = PokepicManager_CreatePokepic(summary->pokepicManager, pokepicTemplate, 0xd0, 0x68, 0, 0, summary->pokepicAnimScript, 0);
    Pokepic_SetAttr(summary->pokepic, 6, 1);
    Pokepic_SetAttr(summary->pokepic, 0x23, 0);
}

void PokemonSummary_StartPokepicAnim(PokemonSummaryScene *summary) {
    if (summary->isEgg) {
        NARC_StartPokepicAnim(summary->pokepicAnimNarc, summary->pokepicAnimManager, summary->pokepic, 0, 2, summary->pokepicAnimReverseX, 0);
    } else {
        Pokepic_StartAnim(summary->pokepic, 1);
        NARC_StartPokepicAnim(summary->pokepicAnimNarc, summary->pokepicAnimManager, summary->pokepic, summary->species, 2, summary->pokepicAnimReverseX, 0);
    }
}

void PokemonSummary_ReloadPokepic(PokemonSummaryScene *summary) {
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

static int PokemonSummary_GetTouchedButton(PokemonSummaryScene *summary) {
    int button;

    if (summary->unk_7BF_0 == 0xF) {
        return -1;
    }
    if (((struct PokemonSummaryArgsDataTypeView *)summary->args)->monDataType != 2) {
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

u8 PokemonSummary_CheckTouchedButton(PokemonSummaryScene *summary) {
    int button = PokemonSummary_GetTouchedButton(summary);
    if (button != -1) {
        return (u8)button;
    }
    return 0xFF;
}
