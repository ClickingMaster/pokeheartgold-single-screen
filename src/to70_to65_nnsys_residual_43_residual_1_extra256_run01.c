#include "global.h"
#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "renderer_joint_family_private.h"

typedef struct {
    u16 flags, pad;
    s32 translation[3];
} Node;

static inline void *GetData(NNSG3dResDict *dict, u32 index) {
    if (dict && index < dict->numEntry) {
        NNSG3dResDictEntryHeader *h = (NNSG3dResDictEntryHeader *)((u8 *)dict + dict->ofsEntry);
        return (u8 *)h + 4 + h->sizeUnit * index;
    }
    return 0;
}

static inline Node *GetNode(NNSG3dResNodeInfo *nodes, u8 index) {
    if (nodes) {
        u32 *entry = GetData(&nodes->dict, index);
        if (entry) {
            return (Node *)((u8 *)nodes + *entry);
        }
    }
    return 0;
}

void getMdlTrans_(MdlJointResult *out) {
    MdlRenderState *state = gCurrentRenderState;
    Node *node = GetNode(state->nodes, state->command[1]);
    if (node->flags & 1) {
        out->flags |= 4;
        return;
    }
    out->trans[0] = node->translation[0];
    out->trans[1] = node->translation[1];
    out->trans[2] = node->translation[2];
}
