#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
typedef struct E788Row {
    s16 start;
    s16 end;
    s16 delta;
    s16 unk06;
    u32 packed;
} E788Row;
typedef struct E788Rows {
    E788Row rows[16];
    void *controller;
} E788Rows;
typedef struct E788Ctx {
    u8 unk00[0x18];
    int phase;
    u8 unk1C[4];
    E788Rows *rows;
    u8 unk24[4];
} E788Ctx;
typedef struct E788Root {
    enum HeapID heapId;
    u8 unk04[0x17C];
    u8 control[4];
} E788Root;
typedef struct E788State {
    u8 unk00[0x44];
    u16 flags;
    u8 unk46[2];
    E788Root *root;
} E788State;
extern s16 ov07_02234C20[];
void ov07_0221DD38(void *control, E788Ctx *ctx, int a2);
int ov07_0221FAF8(E788Root *root, int index);
void *ov07_02222D90(int a0);
u32 ov07_02222D88(int a0, int a1);
void *ov07_02222BE4(void *a0, u32 a1, enum HeapID heapId);
int ov07_0221DDB0(E788State *state, E788Root *root, int a2);
void ov07_0221E87C(SysTask *task, void *data);
BOOL ov07_0221E788(E788State *state);

// @fn ov07_0221E788
BOOL ov07_0221E788(E788State *state) {
    E788Root *root = state->root;
    E788Ctx *ctx;
    int i;
    ctx = Heap_Alloc(root->heapId, sizeof(E788Ctx));
    ctx->rows = Heap_Alloc(root->heapId, sizeof(E788Rows));
    ov07_0221DD38(root->control, ctx, 2);
    state->flags |= 2;
    ctx->phase = 0;
    ctx->rows->controller = ov07_02222BE4(ov07_02222D90(ov07_0221FAF8(root, 2)), ov07_02222D88(0, 0), root->heapId);
    for (i = 0; i < 16; i++) {
        ctx->rows->rows[i].start = i * 8;
        ctx->rows->rows[i].end = ctx->rows->rows[i].start + 8;
        ctx->rows->rows[i].delta = ov07_02234C20[i];
        ctx->rows->rows[i].unk06 = 0;
        ctx->rows->rows[i].packed = ov07_02222D88(0, 0);
        if (ov07_0221DDB0(state, state->root, 6) == 1) {
            ctx->rows->rows[i].delta *= -1;
        }
    }
    SysTask_CreateOnMainQueue(ov07_0221E87C, ctx, 0x1000);
    return FALSE;
}
