#pragma force_active on
#include "global.h"

typedef struct Node {
    u8 unk00[0x18];
    int kind;
    VecFx32 position;
    VecFx32 target;
    VecFx32 motion;
    u8 unk40;
    u8 flag;
    u8 unk42[2];
    u8 active;
    u8 timer;
    u8 unk46[0x8E - 0x46];
    u16 state;
} Node;

typedef struct Work {
    u8 unk00[0x20];
    struct {
        Node nodes[3];
    } rows[4];
    u8 unk6E0[0x720 - 0x6E0];
    u8 ids[4];
} Work;

typedef struct Graph {
    VecFx32 *vec;
    struct {
        u8 ids[12];
        u16 count;
        u16 index;
    } pending;
    VecFx32 delta[12];
} Graph;

typedef struct Stack {
    u8 ids[12];
    u32 count;
} Stack;

typedef struct Edge {
    int valid;
    u32 unk04;
    Node *node;
    u32 unk0C;
} Edge;

extern Edge _0221DCA0[12][12];

u8 ov96_021F2780(Stack *stack, u8 id);
u8 ov96_021F27A8(Stack *stack);
int ov96_021F2814(VecFx32 *a, VecFx32 *b);
void ov96_021F27B8(VecFx32 *a, VecFx32 *b, VecFx32 *out);
void ov96_021F234C(Work *w);

// @fn ov96_021F234C
void ov96_021F234C(Work *w) {
    Graph graphs[12];
    VecFx32 accum[12][12];
    VecFx32 original[12];
    u8 ids[12];
    u8 visited[12];
    Stack stack;
    VecFx32 motion, delta, last;
    Node *node;
    u8 n = 0;
    u8 group;
    u8 i;
    u8 j;
    u8 k;
    u8 current;
    BOOL fresh;
    Graph *g;
    for (i = 0; i < 12; i++) {
        node = &w->rows[i/3].nodes[i%3];
        if (node->flag && node->kind == 1) ids[n++] = i;
        VEC_Subtract(&node->position, &node->target, &original[i]);
    }
    if (n == 0) return;
    MI_CpuClear32(accum, sizeof(accum));
    for (group = 0; group < n; group++) {
        current = ids[group];
        stack.count = 0;
        MI_CpuFill8(graphs, 0, sizeof(graphs));
        for (i = 0; i < 12; i++) graphs[i].vec = &accum[group][i];
        MI_CpuFill8(visited, 0, sizeof(visited));
        motion = w->rows[current/3].nodes[current%3].motion;
        fresh = 1;
        do {
            int row = current/3;
            int column = current%3;
            VecFx32 *currentTarget = &w->rows[row].nodes[column].target;
            visited[current] = 1;
            if (fresh) {
                g = &graphs[current];
                g->pending.count = 0;
                g->pending.index = 0;
                for (i = 0; i < 12; i++) {
                    if (_0221DCA0[current][i].valid) {
                        VEC_Subtract(&_0221DCA0[current][i].node->target, currentTarget, &delta);
                        if (VEC_DotProduct(&motion, &delta) > 0) {
                            g->pending.ids[g->pending.count] = i;
                            g->delta[g->pending.count] = delta;
                            g->pending.count++;
                        }
                    }
                }
                VEC_Add(graphs[current].vec, &motion, graphs[current].vec);
            }
            g = &graphs[current];
            fresh = 0;
            if (g->pending.index >= g->pending.count) {
                visited[current] = 0;
                current = ov96_021F27A8(&stack);
            } else if (visited[g->pending.ids[g->pending.index]]) g->pending.index++;
            else {
                if (ov96_021F2780(&stack, current) == 255) break;
                if (ov96_021F2814(g->vec, &g->delta[g->pending.index]) >= FX_SinCosTable_[0x401]) motion = *g->vec;
                else ov96_021F27B8(g->vec, &g->delta[g->pending.index], &motion);
                current = g->pending.ids[g->pending.index++];
                fresh = 1;
            }
        } while (current != 255);
    }
    for (group = 0; group < n; group++) {
        for (j = 0; j < 12; j++) {
            int off = j*sizeof(VecFx32);
            node = &w->rows[j/3].nodes[j%3];
            VEC_Add(&node->target, (VecFx32 *)((u8 *) accum[group] + off), &node->target);
            if (VEC_Mag((VecFx32 *)((u8 *) accum[group] + off)) != 0 && !node->flag) node->position = node->target;
        }
        current = ids[group];
        node = &w->rows[current/3].nodes[current%3];
        if (node->state == 0) {
            int l;
            for (l = 0; l < 4; l++) {
                if (w->ids[l] == current) {
                    w->ids[l] = 12;
                    break;
                }
            }
            if (node->kind != 1) GF_AssertFail();
            node->active = 1;
            node->timer = 60;
            node->position = node->target;
            node->motion.x = 0;
            node->motion.y = 0;
            node->flag = 0;
        }
    }
    for (k = 0; k < 12; k++) {
        node = &w->rows[k/3].nodes[k%3];
        VEC_Subtract(&node->position, &node->target, &last);
        if (VEC_DotProduct(&original[k], &last) < 0) node->position = node->target;
    }
}
