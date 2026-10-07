#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct ParticleState {
    void *context;
    void *particles;
    u32 unk08;
    u32 args[6];
    u32 attacker;
    u32 defender;
    u32 unk2C[3];
} ParticleState;
void *sub_02015504(void);
enum HeapID ov07_0221BFD0(void *context);
void *ov07_0221C478(void *context);
u32 ov07_0221C468(void *context);
u32 ov07_0221C470(void *context);
void ov07_0221F9A8(void *context, u32 *args, int count);
void ov07_022208B4(void *task, ParticleState *state);
void ov07_022215B4(void *task);

// @fn ov07_022215B4
void ov07_022215B4(void *task) {
    void *context = sub_02015504();
    ParticleState *state = Heap_Alloc(ov07_0221BFD0(context), sizeof(ParticleState));
    state->context = context;
    state->particles = ov07_0221C478(state->context);
    ov07_0221F9A8(state->context, state->args, 6);
    switch (state->args[1]) {
    default:
    case 0:
        state->attacker = ov07_0221C468(state->context);
        state->defender = ov07_0221C470(state->context);
        break;
    case 1:
        state->attacker = ov07_0221C470(state->context);
        state->defender = ov07_0221C468(state->context);
        break;
    case 2:
        state->attacker = ov07_0221C468(state->context);
        state->defender = ov07_0221C470(state->context);
        break;
    case 3:
        state->attacker = ov07_0221C470(state->context);
        state->defender = ov07_0221C468(state->context);
        break;
    case 4:
        state->attacker = ov07_0221C468(state->context);
        state->defender = ov07_0221C470(state->context);
        break;
    }
    ov07_022208B4(task, state);
    Heap_Free(state);
}
