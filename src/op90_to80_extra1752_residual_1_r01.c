#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g3d/kernel.h>

struct NNSG3dJntAnmResult_ {
    u32 flags;
    VecFx32 scale, scaleEx0, scaleEx1;
    MtxFx33 matrix;
    VecFx32 translate;
};
typedef struct NNSG3dJntAnmResult_ Node;
typedef NNSG3dRenderObj Model;
typedef struct NNSG3dRS_ Work;

struct NNSG3dRS_ {
    u8 *stream;
    Model *model;
    u32 flags;
    NNSG3dSbcCallBackFunc callbacks[32];
    u8 timings[32];
    u8 beforeNodeId[2];
    u8 nodeId;
    u8 beforeResult[5];
    Node *result;
    u8 pad3[0x1c];
    u8 *dict;
    u8 pad4[0x10];
    void (*parse)(Node *, const void *, const u8 *, u32);
    void (*emit)(const Node *);
    u8 pad5[0x3c];
    Node local;
    u32 finalWord;
};

typedef char WorkExtent[(sizeof(Work) == 0x188) ? 1 : -1];
typedef char NodeExtent[(sizeof(Node) == 0x58) ? 1 : -1];

typedef struct {
    u16 flags;
    s16 rotation00;
    u8 body[];
} NodeResource;

extern const u8 _02109494[9][4];
extern void NNS_G3dGeBufferOP_N(u32, const u32 *, u32);
extern void MI_Zero36B(void *);

typedef struct {
    u8 tag, count;
    u16 size, unused, entryOffset;
} DictionaryHeader;

typedef struct {
    u16 stride, reserved;
    u8 entries[];
} DictionaryEntries;

static inline u32 *GetEntry(u8 *dict, u8 id) {
    DictionaryHeader *h = (DictionaryHeader *)dict;
    if (h && id < (u32)h->count) {
        DictionaryEntries *e = (DictionaryEntries *)(dict + h->entryOffset);
        return (u32 *)(e->entries + e->stride * id);
    }
    return NULL;
}

static inline u8 *GetNode(u8 *dict, u8 id) {
    if (dict) {
        u32 *offset = GetEntry(dict, id);
        if (offset) {
            return dict + *offset;
        }
    }
    return NULL;
}

void NNSi_G3dFuncSbc_NODEDESC(Work *, u32);

void NNSi_G3dFuncSbc_NODEDESC(Work *w, u32 option) {
    struct {
        u32 pop, restore, early;
    } t;

    int pb, pa;
    u8 id = w->stream[1];
    u32 length = 4;
    int timing, handled;
    Node *node;
    NodeResource *header;
    u8 *cursor;
    int axis;
    w->nodeId = id;
    {
        u32 stableFlags = w->flags | 0x10;
        w->flags = stableFlags;
        if (stableFlags & 0x400) {
            if (option == 0x40 || option == 0x60) {
                length++;
            }
            if (option == 0x20 || option == 0x60) {
                length++;
                if (!(stableFlags & 0x100)) {
                    t.early = w->stream[4];
                    NNS_G3dGeBufferOP_N(0x14, &t.early, 1);
                }
            }
            w->stream += length;
            return;
        }
    }
    if (option == 0x40 || option == 0x60) {
        if (option == 0x40) {
            t.restore = w->stream[4];
        } else {
            t.restore = w->stream[5];
        }
        length++;
        if (!(w->flags & 0x100)) {
            NNS_G3dGeBufferOP_N(0x14, &t.restore, 1);
        }
    }
    w->result = &w->local;
    timing = w->callbacks[6] ? w->timings[6] : 0;
    if (timing == 1) {
        w->flags &= ~0x40;
        w->callbacks[6](w);
        timing = w->callbacks[6] ? w->timings[6] : 0;
        handled = w->flags & 0x40;
    } else {
        handled = 0;
    }
    if (!handled) {
        if (w->model->recJntAnm) {
            node = &w->model->recJntAnm[id];
            handled = (w->flags & 0x80) ? 0 : 1;
        } else {
            node = &w->local;
            handled = 0;
        }
        if (!handled) {
            node->flags = 0;
            if (w->model->anmJnt) {
                handled = w->model->funcBlendJnt(node, w->model->anmJnt, id);
                if (handled) {
                    goto parsed;
                }
            }
            header = (NodeResource *)GetNode(w->dict, id);
            cursor = header->body;
            if (header->flags & 1) {
                node->flags |= 4;
            } else {
                node->translate.x = *(s32 *)cursor;
                node->translate.y = *(s32 *)(cursor + 4);
                node->translate.z = *(s32 *)(cursor + 8);
                cursor += 12;
            }
            if (header->flags & 2) {
                node->flags |= 2;
            } else if (header->flags & 8) {
                axis = (header->flags & 0xf0) >> 4;
                pa = *(s16 *)cursor;
                pb = *(s16 *)(cursor + 2);
                MI_Zero36B(&node->matrix);
                node->matrix.a[axis] = (header->flags & 0x100) ? -4096 : 4096;
                node->matrix.a[_02109494[axis][0]] = pa;
                node->matrix.a[_02109494[axis][1]] = pb;
                pb = (header->flags & 0x200) ? -pb : pb;
                node->matrix.a[_02109494[axis][2]] = pb;
                pa = (header->flags & 0x400) ? -pa : pa;
                node->matrix.a[_02109494[axis][3]] = pa;
                cursor += 4;
            } else {
                node->matrix.a[0] = header->rotation00;
                node->matrix.a[1] = *(s16 *)cursor;
                node->matrix.a[2] = *(s16 *)(cursor + 2);
                node->matrix.a[3] = *(s16 *)(cursor + 4);
                node->matrix.a[4] = *(s16 *)(cursor + 6);
                node->matrix.a[5] = *(s16 *)(cursor + 8);
                node->matrix.a[6] = *(s16 *)(cursor + 10);
                node->matrix.a[7] = *(s16 *)(cursor + 12);
                node->matrix.a[8] = *(s16 *)(cursor + 14);
                cursor += 16;
            }
            w->parse(node, cursor, w->stream, header->flags);
        }
parsed:
        w->result = node;
    }
    if (timing == 2) {
        w->flags &= ~0x40;
        w->callbacks[6](w);
        timing = w->callbacks[6] ? w->timings[6] : 0;
        handled = w->flags & 0x40;
    } else {
        handled = 0;
    }
    if (!handled && !(w->flags & 0x100)) {
        w->emit(w->result);
    }
    w->result = NULL;
    if (timing == 3) {
        w->flags &= ~0x40;
        w->callbacks[6](w);
        handled = w->flags & 0x40;
    } else {
        handled = 0;
    }
    if (option == 0x20 || option == 0x60) {
        length++;
        if (!handled && !(w->flags & 0x100)) {
            t.pop = w->stream[4];
            NNS_G3dGeBufferOP_N(0x13, &t.pop, 1);
        }
    }
    w->stream += length;
}
