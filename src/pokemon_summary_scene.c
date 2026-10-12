#include "global.h"

#include "camera.h"
#include "gf_gfx_planes.h"

// The parts of the summary screen state this module uses.
typedef struct PokemonSummaryScene {
    u8 pad_000[0x22C];
    void *unk_22C; // 0x22C
    u8 pad_230[0x23C - 0x230];
    u16 species; // 0x23C
    u8 pad_23E[0x280 - 0x23E];
    u32 unk_280_0 : 28; // 0x280
    u32 unk_280_28 : 1;
    u32 unk_280_29 : 3;
    u8 pad_284[0x29C - 0x284];
    void *camera;         // 0x29C
    void *pokepicManager; // 0x2A0
    u32 unk_2A4[2];       // 0x2A4
    u8 pad_2AC[0x2CC - 0x2AC];
    void *pokepicAnimManager; // 0x2CC
    void *pokepic;            // 0x2D0
    u32 unk_2D4;              // 0x2D4
    u8 pad_2D8[0x7B8 - 0x2D8];
    void *pokepicAnimNarc; // 0x7B8
    u8 pad_7BC[3];
    u8 unk_7BF_0 : 4; // 0x7BF
    u8 unk_7BF_4 : 4;
} PokemonSummaryScene;

// A condition chart vertex: at 0 (min), at 255 (max), and its step per condition point.
typedef struct ConditionChartVertexAnim {
    u16 max[3];  // 0x00
    u16 min[3];  // 0x06
    u16 step[3]; // 0x0C
} ConditionChartVertexAnim;

typedef struct ConditionChartVertexAnimS {
    s16 max[3];
    s16 min[3];
    s16 step[3];
} ConditionChartVertexAnimS;

typedef struct ConditionChartVertex {
    s16 x, y, z;
} ConditionChartVertex;

struct UnkStruct22C {
    u8 pad_00[0x11];
    u8 unk_11;
};

extern const VecFx32 _02104EA4;
extern const u8 _02104FD0[][4];

void NNS_G2dSetupSoftwareSpriteCamera(void);
void PokepicManager_DrawAll(void *mgr);
void PokepicManager_Delete(void *mgr);
void PokepicAnimManager_StopAnim(void *a, int b);
void PokepicAnimManager_Delete(void *a);
void Pokepic_StartAnim(void *p, int a);
void sub_0207294C(void *a, void *b, void *c, int d, int e, u32 f, int g);
void *PokepicManager_Create(int heapId);
int PokemonSummary_GetShownMon(PokemonSummaryScene *a);
void GetPokemonSpriteCharAndPlttNarcIds(u32 *dest, int species, int c);
void GetBoxmonSpriteCharAndPlttNarcIds(u32 *dest, int species, int c, int d);
void NARC_ReadPokepicAnimScript(void *narc, void *dest, u16 species, int d);
void *PokepicManager_CreatePokepic(void *mgr, u32 *a, int x, int y, int e, int f, void *g, int h);
void Pokepic_SetAttr(void *pic, int attr, int value);
int TouchscreenHitbox_FindRectAtTouchNew(const void *hitboxes);

extern const u8 _02104FFC[][4];

void PokemonSummary_DrawPokepic(PokemonSummaryScene *a);
void PokemonSummary_FreePokepicAndCamera(PokemonSummaryScene *a);
void ConditionChart_GetVertex(const ConditionChartVertexAnim *src, ConditionChartVertex *dst, int t);
void ConditionChart_GetStep(const ConditionChartVertex *from, const ConditionChartVertex *to, ConditionChartVertex *out);
void PokemonSummary_InitConditionChart(PokemonSummaryScene *summary);
void PokemonSummary_CreatePokepic(PokemonSummaryScene *a);
void PokemonSummary_StartPokepicAnim(PokemonSummaryScene *a);
void PokemonSummary_ReloadPokepic(PokemonSummaryScene *a);
int PokemonSummary_GetTouchedButton(PokemonSummaryScene *a);
u8 PokemonSummary_CheckTouchedButton(PokemonSummaryScene *a);

typedef char AssertAnimVecSize[(sizeof(ConditionChartVertexAnim) == 0x12) ? 1 : -1];
typedef char AssertAnimVecSignedSize[(sizeof(ConditionChartVertexAnimS) == 0x12) ? 1 : -1];
typedef char AssertPicture22C[(offsetof(PokemonSummaryScene, unk_22C) == 0x22C) ? 1 : -1];
typedef char AssertPicture23C[(offsetof(PokemonSummaryScene, species) == 0x23C) ? 1 : -1];
typedef char AssertPicture29C[(offsetof(PokemonSummaryScene, camera) == 0x29C) ? 1 : -1];
typedef char AssertPicture2A0[(offsetof(PokemonSummaryScene, pokepicManager) == 0x2A0) ? 1 : -1];
typedef char AssertPicture2A4[(offsetof(PokemonSummaryScene, unk_2A4) == 0x2A4) ? 1 : -1];
typedef char AssertPicture2CC[(offsetof(PokemonSummaryScene, pokepicAnimManager) == 0x2CC) ? 1 : -1];
typedef char AssertPicture2D0[(offsetof(PokemonSummaryScene, pokepic) == 0x2D0) ? 1 : -1];
typedef char AssertPicture2D4[(offsetof(PokemonSummaryScene, unk_2D4) == 0x2D4) ? 1 : -1];
typedef char AssertPicture7B8[(offsetof(PokemonSummaryScene, pokepicAnimNarc) == 0x7B8) ? 1 : -1];

void PokemonSummary_Init3D(void);
void PokemonSummary_CreateCamera(PokemonSummaryScene *p);

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

void PokemonSummary_DrawPokepic(PokemonSummaryScene *a) {
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(a->pokepicManager);
    *(vu32 *)0x04000540 = 1;
}

void PokemonSummary_FreePokepicAndCamera(PokemonSummaryScene *a) {
    Camera_Delete(a->camera);
    PokepicAnimManager_StopAnim(a->pokepicAnimManager, 0);
    PokepicAnimManager_Delete(a->pokepicAnimManager);
    PokepicManager_Delete(a->pokepicManager);
}

void PokemonSummary_CreateCamera(PokemonSummaryScene *p) {
    VecFx32 pos = _02104EA4;
    CameraAngle angle = { 0, 0, 0, 0 };
    p->camera = Camera_New((enum HeapID)0x13);
    Camera_Init_FromPosDistanceAndAngle(&pos, 0x10000, &angle, 0x5C1, 1, p->camera);
    Camera_SetPerspectiveClippingPlane(0, 0x64000, p->camera);
    Camera_ClearFixedTarget(p->camera);
    Camera_SetStaticPtr(p->camera);
}

void ConditionChart_GetVertex(const ConditionChartVertexAnim *src, ConditionChartVertex *dst, int t) {
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

extern const u8 _02104EB0[];
extern const u8 _02104EC2[];
extern const u8 _02104ED4[];
extern const u8 _02104EE6[];
extern const u8 _02104EF8[];
extern const u8 _02104F0A[];
extern const u8 _02104F1C[];
extern const u8 _02104F2E[];
extern const u8 _02104F40[];
extern const u8 _02104F52[];
extern const u8 _02104F64[];
extern const u8 _02104F76[];
extern const u8 _02104F88[];
extern const u8 _02104F9A[];
extern const u8 _02104FAC[];
extern const u8 _02104FBE[];

void ConditionChart_GetStep(const ConditionChartVertex *from, const ConditionChartVertex *to, ConditionChartVertex *out) {
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

void PokemonSummary_InitConditionChart(PokemonSummaryScene *summary) {
    u32 i;
    u8 *raw = (u8 *)summary;
    PictureVectorWork *vectors = (PictureVectorWork *)summary;

    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104EB0, (ConditionChartVertex *)&raw[0x398], raw[0x275]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104EC2, (ConditionChartVertex *)&raw[0x39E], raw[0x276]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104ED4, (ConditionChartVertex *)&raw[0x3AA], raw[0x277]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104EE6, (ConditionChartVertex *)&raw[0x3A4], 0);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104EF8, (ConditionChartVertex *)&raw[0x3B0], raw[0x279]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F0A, (ConditionChartVertex *)&raw[0x3B6], raw[0x275]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F1C, (ConditionChartVertex *)&raw[0x3C2], 0);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F2E, (ConditionChartVertex *)&raw[0x3BC], raw[0x278]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F40, (ConditionChartVertex *)&raw[0x3C8], raw[0x279]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F52, (ConditionChartVertex *)&raw[0x3CE], 0);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F64, (ConditionChartVertex *)&raw[0x3DA], raw[0x277]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F76, (ConditionChartVertex *)&raw[0x3D4], raw[0x278]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F88, (ConditionChartVertex *)&raw[0x3E0], 0);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104F9A, (ConditionChartVertex *)&raw[0x3E6], raw[0x276]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104FAC, (ConditionChartVertex *)&raw[0x3F2], raw[0x277]);
    ConditionChart_GetVertex((const ConditionChartVertexAnim *)_02104FBE, (ConditionChartVertex *)&raw[0x3EC], raw[0x278]);

    for (i = 0; i < 4; i++) {
        ConditionChart_GetStep(&vectors->from[i].vectors[0], &vectors->to[i].vectors[0], &vectors->out[i].vectors[0]);
        ConditionChart_GetStep(&vectors->from[i].vectors[1], &vectors->to[i].vectors[1], &vectors->out[i].vectors[1]);
        ConditionChart_GetStep(&vectors->from[i].vectors[2], &vectors->to[i].vectors[2], &vectors->out[i].vectors[2]);
        ConditionChart_GetStep(&vectors->from[i].vectors[3], &vectors->to[i].vectors[3], &vectors->out[i].vectors[3]);
    }
    vectors->done = 0;
}

void PokemonSummary_CreatePokepic(PokemonSummaryScene *a) {
    u32 sp10[4];
    int species;

    a->pokepicManager = PokepicManager_Create(0x13);
    species = PokemonSummary_GetShownMon(a);
    if (((struct UnkStruct22C *)a->unk_22C)->unk_11 == 2) {
        GetBoxmonSpriteCharAndPlttNarcIds(sp10, species, 2, 0);
    } else {
        GetPokemonSpriteCharAndPlttNarcIds(sp10, species, 2);
    }
    NARC_ReadPokepicAnimScript(a->pokepicAnimNarc, a->unk_2A4, a->species, 1);
    a->unk_2D4 = 0;
    a->pokepic = PokepicManager_CreatePokepic(a->pokepicManager, sp10, 0xd0, 0x68, 0, 0, a->unk_2A4, 0);
    Pokepic_SetAttr(a->pokepic, 6, 1);
    Pokepic_SetAttr(a->pokepic, 0x23, 0);
}

void PokemonSummary_StartPokepicAnim(PokemonSummaryScene *a) {
    if (a->unk_280_28) {
        sub_0207294C(a->pokepicAnimNarc, a->pokepicAnimManager, a->pokepic, 0, 2, a->unk_2D4, 0);
    } else {
        Pokepic_StartAnim(a->pokepic, 1);
        sub_0207294C(a->pokepicAnimNarc, a->pokepicAnimManager, a->pokepic, a->species, 2, a->unk_2D4, 0);
    }
}

void PokemonSummary_ReloadPokepic(PokemonSummaryScene *a) {
    PokepicAnimManager_StopAnim(a->pokepicAnimManager, 0);
    PokepicManager_Delete(a->pokepicManager);
    PokemonSummary_CreatePokepic(a);
    PokemonSummary_StartPokepicAnim(a);
}

int PokemonSummary_GetTouchedButton(PokemonSummaryScene *a) {
    int r;

    if (a->unk_7BF_0 == 0xF) {
        return -1;
    }
    if (((struct UnkStruct22C *)a->unk_22C)->unk_11 != 2) {
        r = TouchscreenHitbox_FindRectAtTouchNew(_02104FFC);
        if (r != -1) {
            r = _02104FD0[r][0];
        }
        return r;
    } else {
        r = TouchscreenHitbox_FindRectAtTouchNew(_02104FFC);
        if (r >= 4 && r <= 9) {
            return -1;
        }
        if (r != -1) {
            r = _02104FD0[r][0];
        }
        return r;
    }
}

u8 PokemonSummary_CheckTouchedButton(PokemonSummaryScene *a) {
    int r = PokemonSummary_GetTouchedButton(a);
    if (r != -1) {
        return (u8)r;
    }
    return 0xFF;
}
