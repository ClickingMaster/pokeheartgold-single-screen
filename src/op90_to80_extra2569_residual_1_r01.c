#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g3d/binres/res_struct_accessor_inline.h>

#include "renderer_joint_family_private.h"

typedef struct {
    u16 flags;
    s16 firstRotation;
    s16 tail[];
} Node;

typedef struct {
    u32 nodeOffset;
} NodeEntry;

typedef char NodeDictInitialMember[offsetof(NNSG3dResNodeInfo, dict) == 0 ? 1 : -1];
extern const u8 _021094B8[36];
extern void MI_Zero36B(void *);

static inline u32 pivotIndex(const u8 *table, int axis) {
    return table[axis * 4];
}

static inline void DecodePivotRotation(fx32 *matrix, int axis, const s16 *rotation, const Node *node) {
    s32 a = rotation[0], b = rotation[1];
    MI_Zero36B(matrix);
    matrix[axis] = (node->flags & 0x100) ? -4096 : 4096;
    matrix[pivotIndex(_021094B8, axis)] = a;
    matrix[pivotIndex(_021094B8 + 1, axis)] = b;
    if (node->flags & 0x200) {
        b = -b;
    }
    matrix[pivotIndex(_021094B8 + 2, axis)] = b;
    if (node->flags & 0x400) {
        a = -a;
    }
    matrix[pivotIndex(_021094B8 + 3, axis)] = a;
}

#pragma push
#pragma opt_common_subs off

void getMdlRot_(MdlJointResult *out) {
    MdlRenderState *ctx = _021DA700;
    u32 index = ctx->command[1];
    NNSG3dResNodeInfo *info = ctx->nodes;
    const NNSG3dResDict *dict = (const NNSG3dResDict *)info;
    int axis;
    const u8 *node;
    const NodeEntry *p;
    if (dict && (p = NNS_G3dGetResDataByIdx(dict, index))) {
        node = (const u8 *)dict + p->nodeOffset;
    } else {
        node = 0;
    }
    {
        const Node *model = (const Node *)node;
        int flags = model->flags;
        const s16 *rot = model->tail;
        if (!(flags & 1)) {
            rot += 6;
        }
        if (!(flags & 2)) {
            if (flags & 8) {
                axis = (flags & 0xf0) >> 4;
                DecodePivotRotation(out->rot, axis, rot, model);
                return;
            }
            out->rot[0] = model->firstRotation;
            out->rot[1] = rot[0];
            out->rot[2] = rot[1];
            out->rot[3] = rot[2];
            out->rot[4] = rot[3];
            out->rot[5] = rot[4];
            out->rot[6] = rot[5];
            out->rot[7] = rot[6];
            out->rot[8] = rot[7];
            return;
        }
        out->flags |= 2;
    }
}

#pragma pop
