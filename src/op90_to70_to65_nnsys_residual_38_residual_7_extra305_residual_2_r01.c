#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g3d/kernel.h>
#include <stddef.h>

/* Public forward tags become the same complete types in every producer/callee. */
typedef struct {
    fx32 words[3];
} MdlScaleTriple;

typedef union {
    fx32 words[384];
    MdlScaleTriple triples[128];
} MdlScaleCache;

typedef struct NNSG3dJntAnmResult_ {
    u32 flags;

    union {
        fx32 scale[3];
        VecFx32 scaleVector;
    };

    union {
        fx32 inverse[6];
        MdlScaleTriple inverseTriples[2];

        struct {
            VecFx32 scaleEx1, scaleEx2;
        };
    };

    union {
        fx32 rot[9];
        VecFx32 rotVectors[3];
    };

    union {
        fx32 trans[3];
        VecFx32 transVector;
    };
} MdlJointResult;

typedef struct NNSG3dMatAnmResult_ {
    u32 flags, diffuse, specular, polygon, texture, palette;
    fx32 scaleX, scaleY;
    fx16 sin, cos;
    fx32 transX, transY;
    u16 width, height;
    fx32 ratioW, ratioH;
} MdlMaterialResult;

typedef void (*MdlTextureFunction)(MdlMaterialResult *);
typedef void (*MdlTextureCalculator)(MtxFx44 *, const MdlMaterialResult *);
typedef char MdlMaterialSize[(sizeof(MdlMaterialResult) == 56) ? 1 : -1];
typedef void (*MdlScaleFunction)(MdlJointResult *, const fx32 *, const u8 *, u32);

typedef struct NNSG3dRS_ {
    const u8 *command;
    NNSG3dRenderObj *input;
    u32 flags;
    NNSG3dSbcCallBackFunc slots[32];
    u8 timing[32];
    u8 reservedAC, materialId, reservedAE[2];
    MdlMaterialResult *materialResult;
    MdlJointResult *jointResult;
    void *reservedB8;
    u32 materialFlags[2];
    u32 scaleFlags[NNS_G3D_SIZE_JNT_MAX / 32];
    u8 reservedCC[8];
    NNSG3dResNodeInfo *nodes;
    void *materials, *shapes;
    u32 extra1, extra2;
    MdlScaleFunction scale;
    void *matrixHandler;
    MdlTextureFunction textureHandler;
    MdlMaterialResult material;
    MdlJointResult joint;
    u8 trailing184[4];
} MdlRenderState;

typedef char MdlMaterialOffset[(offsetof(MdlRenderState, material) == 0xf4) ? 1 : -1];
typedef char MdlMaterialResultOffset[(offsetof(MdlRenderState, materialResult) == 0xb0) ? 1 : -1];
typedef char MdlJointOffset[(offsetof(MdlRenderState, joint) == 0x12c) ? 1 : -1];
typedef char MdlCacheSize[(sizeof(MdlScaleCache) == 1536) ? 1 : -1];
typedef char MdlJointSize[(sizeof(MdlJointResult) == 88) ? 1 : -1];
typedef char MdlRendererSize[(sizeof(MdlRenderState) == 0x188) ? 1 : -1];
extern MdlRenderState *_021DA700;
extern MdlScaleFunction _02110A80[3];
typedef void (*MdlJointEvaluator)(MdlJointResult *, const NNSG3dAnmObj *, u32);
void NNSi_G3dAnmCalcNsBca(MdlJointResult *, const NNSG3dAnmObj *, u32);
int NNSi_G3dAnmBlendJnt(MdlJointResult *, const NNSG3dAnmObj *, u32);
void getMdlScale_(MdlJointResult *);
void getMdlTrans_(MdlJointResult *);
void getMdlRot_(MdlJointResult *);
void getJntSRTAnmResult_(u8 *, u32, s32, MdlJointResult *);
void G3dDrawInternal_(MdlRenderState *, NNSG3dRenderObj *);
void G3dDrawInternal_Loop_(MdlRenderState *);
void NNSi_G3dGetJointScaleBasic(MdlJointResult *, const fx32 *, const u8 *, u32);
void NNSi_G3dGetJointScaleMaya(MdlJointResult *, const fx32 *, const u8 *, u32);
void NNSi_G3dGetJointScaleSi3d(MdlJointResult *, const fx32 *, const u8 *, u32);
/* These ASM-owned aliases are interior positions in ONE 1536-byte backing. */
extern MdlScaleCache _021DB504;
extern fx32 _021DB508[383];
extern fx32 _021DB50C[382];
extern MdlScaleTriple _021DB510[127];
extern fx32 _021DB514[380];
extern fx32 _021DB518[379];
/* Preserve the old Si3d access qualifiers without qualifying Maya's accesses. */
#define MdlCache504Vol ((volatile fx32 *)&_021DB504)
#define MdlCache508Vol ((volatile fx32 *)_021DB508)
#define MdlCache50CVol ((volatile fx32 *)_021DB50C)
#define MdlCache510Vol ((volatile fx32 *)_021DB510)
#define MdlCache514Vol ((volatile fx32 *)_021DB514)
#define MdlCache518Vol ((volatile fx32 *)_021DB518)
extern void *_02110A74[3];
extern MdlTextureFunction _02110A8C[4];
#include <nnsys/g3d/binres/res_struct_accessor_inline.h>
#include <string.h>

typedef struct {
    u32 head, diffuse, specular, polygon, polyMask, texture, reserved18;
    u16 palette, flags, width, height;
    fx32 ratioW, ratioH;
    u8 transformations[];
} MdlResourceMaterial;

typedef struct {
    fx32 x, y;
} MdlMaterialVector;

typedef struct {
    fx16 sin, cos;
} MdlMaterialRotation;

extern MdlMaterialResult _021DA704[64];
extern const u32 _02109474[8];

typedef struct {
    u16 ofsTex, ofsPltt;
    NNSG3dResDict dict;
} MatSet;

static inline MdlResourceMaterial *FindMaterial(void *resource, u32 index) {
    MatSet *mat = (MatSet *)resource;
    if (mat) {
        const NNSG3dResDictMdlSetData *entry = (const NNSG3dResDictMdlSetData *)NNS_G3dGetResDataByIdx(&mat->dict, index);
        if (entry) {
            return (MdlResourceMaterial *)((u8 *)mat + entry->offset);
        }
    }
    return NULL;
}

static inline MdlMaterialResult *SelectGlobalMaterial(u32 *valid, u32 index) {
    u32 block = index >> 5;
    valid[block] |= 1u << (index & 31);
    return &_021DA704[index];
}

void NNSi_G3dFuncSbc_MAT_InternalDefault(MdlRenderState *, u32, MdlResourceMaterial *, u32);

static inline void InitializeMaterialPhase(MdlRenderState *context, MdlMaterialResult *result, MdlResourceMaterial *material, u32 index) {
    u8 *data;
    result->flags = 0;
    if (FindMaterial(context->materials, index)->flags & 0x20) {
        result->flags |= 0x20;
    }
    {
        u32 mask = _02109474[(material->flags >> 6) & 7];
        result->diffuse = (NNS_G3dGlb.prmMatColor0 & ~mask) | (material->diffuse & mask);
        mask = _02109474[(material->flags >> 9) & 7];
        result->specular = (NNS_G3dGlb.prmMatColor1 & ~mask) | (material->specular & mask);
        result->polygon = (NNS_G3dGlb.prmPolygonAttr & ~material->polyMask) | (material->polygon & material->polyMask);
    }
    result->texture = material->texture;
    result->palette = material->palette;
    if (material->flags & 1) {
        data = material->transformations;
        if (!(material->flags & 2)) {
            result->scaleX = ((const MdlMaterialVector *)data)->x;
            result->scaleY = ((const MdlMaterialVector *)data)->y;
            data += 8;
        } else {
            result->flags |= 1;
        }
        if (!(material->flags & 4)) {
            result->sin = ((const MdlMaterialRotation *)data)->sin;
            result->cos = ((const MdlMaterialRotation *)data)->cos;
            data += 4;
        } else {
            result->flags |= 2;
        }
        if (!(material->flags & 8)) {
            result->transX = ((const MdlMaterialVector *)data)->x;
            result->transY = ((const MdlMaterialVector *)data)->y;
        } else {
            result->flags |= 4;
        }
        result->flags |= 8;
    }
    {
        NNSG3dRenderObj *input = context->input;
        if (input->anmMat && (input->hintMatAnmExist[index >> 5] & (1u << (index & 31)))) {
            input->funcBlendMat(result, input->anmMat, index);
        }
    }
    if (result->flags & 0x18) {
        result->width = material->width;
        result->height = material->height;
        result->ratioW = material->ratioW;
        result->ratioH = material->ratioH;
    }
}

void NNSi_G3dFuncSbc_MAT_InternalDefault(MdlRenderState *context, u32 option, MdlResourceMaterial *material, u32 index) {
    u32 command[7];
    MdlMaterialResult *cacheFirst;
    MdlMaterialResult *result;
    MdlMaterialResult *cacheSecond;
    u32 skip;
    context->materialId = (u8)index;
    context->flags |= 8;
    context->materialResult = &context->material;
    u32 stage;
    stage = context->slots[4] ? context->timing[4] : 0;
    if (stage == 1) {
        context->flags &= ~0x40u;
        context->slots[4](context);
        stage = context->slots[4] ? context->timing[4] : 0;
        skip = context->flags & 0x40;
    } else {
        skip = 0;
    }
    if (skip == 0) {
        cacheFirst = context->input->recMatAnm;
        if (cacheFirst && !(context->flags & 0x80)) {
            result = &cacheFirst[index];
        } else {
            if ((option == 0x20 || option == 0x40) && (context->materialFlags[index >> 5] & (1u << (index & 31)))) {
                if (cacheFirst) {
                    result = &cacheFirst[index];
                } else {
                    result = &_021DA704[index];
                }
            } else {
                if (cacheFirst) {
                    u32 *flags = context->materialFlags;
                    flags[index >> 5] |= 1u << (index & 31);
                    result = &context->input->recMatAnm[index];
                } else {
                    if (option == 0x40) {
                        context->materialFlags[index >> 5] |= 1u << (index & 31);
                        result = &_021DA704[index];
                    } else {
                        result = &context->material;
                    }
                }
                InitializeMaterialPhase(context, result, material, index);
            }
        }
        context->materialResult = result;
    }
    if (stage == 2) {
        context->flags &= ~0x40u;
        context->slots[4](context);
        stage = context->slots[4] ? context->timing[4] : 0;
        skip = context->flags & 0x40;
    } else {
        skip = 0;
    }
    if (!skip) {
        cacheSecond = context->materialResult;
        if (cacheSecond->polygon & 0x1f0000) {
            if (cacheSecond->flags & 0x20) {
                cacheSecond->polygon &= ~0x1f0000u;
            }
            context->flags &= ~2u;
            if (!(context->flags & 0x100)) {
                command[0] = 0x293130;
                command[1] = cacheSecond->diffuse;
                command[2] = cacheSecond->specular;
                command[3] = cacheSecond->polygon;
                command[4] = 0x2b2a;
                command[5] = cacheSecond->texture;
                command[6] = cacheSecond->palette;
                NNS_G3dGeBufferOP_N(command[0], command + 1, 6);
                if (cacheSecond->flags & 0x18) {
                    context->textureHandler(cacheSecond);
                }
            }
        } else {
            context->flags |= 2;
        }
    }
    if (stage == 3) {
        context->flags &= ~0x40u;
        context->slots[4](context);
    }
}
