#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g3d/binres/res_struct.h>
/* REVIEW PROPOSAL ONLY: local assembly/public declaration reconstruction. */
#ifndef POKEHEARTGOLD_RENDERER_JOINT_FAMILY_PRIVATE_H
#define POKEHEARTGOLD_RENDERER_JOINT_FAMILY_PRIVATE_H
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

typedef void (*MdlScaleFunction)(MdlJointResult *, const fx32 *, const u8 *, u32);

typedef struct NNSG3dRS_ {
    const u8 *volatile command;
    NNSG3dRenderObj *volatile input;
    u32 flags;
    NNSG3dSbcCallBackFunc slots[32];
    u8 timing[32];
    u8 reservedAC[0x18];
    u32 scaleFlags[NNS_G3D_SIZE_JNT_MAX / 32];
    u8 reservedCC[8];
    NNSG3dResNodeInfo *nodes;
    void *materials, *shapes;
    u32 extra1, extra2;
    MdlScaleFunction scale;
    void *matrixHandler, *textureHandler;
    u8 reservedF4[0x38];
    MdlJointResult joint;
    u8 trailing184[4];
} MdlRenderState;

typedef char MdlJointOffset[(offsetof(MdlRenderState, joint) == 0x12c) ? 1 : -1];
typedef char MdlCacheSize[(sizeof(MdlScaleCache) == 1536) ? 1 : -1];
typedef char MdlJointSize[(sizeof(MdlJointResult) == 88) ? 1 : -1];
typedef char MdlRendererSize[(sizeof(MdlRenderState) == 0x188) ? 1 : -1];
extern MdlRenderState *gCurrentRenderState;
extern MdlScaleFunction gGetJointScaleFuncs[3];
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
extern void *gSendJointSRTFuncs[3];
extern void *gSendTexSRTFuncs[4];
#endif

typedef struct {
    u16 flags, rotation00;
    u8 data[1];
} Node;

static inline void *GetData(NNSG3dResDict *dict, u32 index) {
    if (dict && index < dict->numEntry) {
        NNSG3dResDictEntryHeader *header = (NNSG3dResDictEntryHeader *)((u8 *)dict + dict->ofsEntry);
        return (u8 *)header + 4 + header->sizeUnit * index;
    }
    return 0;
}

static inline Node *GetNode(NNSG3dResNodeInfo *nodes, u32 index) {
    if (nodes) {
        u32 *entry = GetData(&nodes->dict, index);
        if (entry) {
            return (Node *)((u8 *)nodes + *entry);
        }
    }
    return 0;
}

#pragma force_active on

void getMdlScale_(MdlJointResult *out) {
    MdlRenderState *state = gCurrentRenderState;
    const u8 *command = state->command;
    Node *node = GetNode(state->nodes, command[1]);
    u16 flags = node->flags;
    u8 *data = (u8 *)node + 4;
    if (!(flags & 1)) {
        data += 12;
    }
    if (!(flags & 2)) {
        if (flags & 8) {
            data += 4;
        } else {
            data += 16;
        }
    }
    state->scale(out, (const fx32 *)data, command, flags);
}
