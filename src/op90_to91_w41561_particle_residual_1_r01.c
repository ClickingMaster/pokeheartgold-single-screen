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
void *ParticleSystem_GetEmitterParam(void);
enum HeapID BattleAnimSystem_GetHeapID(void *context);
void *BattleAnimSystem_GetCurrentParticleSystem(void *context);
u32 BattleAnimSystem_GetAttacker(void *context);
u32 BattleAnimSystem_GetDefender(void *context);
void BattleAnimSystem_ReadScriptArgs(void *context, u32 *args, int count);
void ov07_022208B4(void *task, ParticleState *state);
void ov07_022215B4(void *task);

// @fn ov07_022215B4
void ov07_022215B4(void *task) {
    void *context = ParticleSystem_GetEmitterParam();
    ParticleState *state = Heap_Alloc(BattleAnimSystem_GetHeapID(context), sizeof(ParticleState));
    state->context = context;
    state->particles = BattleAnimSystem_GetCurrentParticleSystem(state->context);
    BattleAnimSystem_ReadScriptArgs(state->context, state->args, 6);
    switch (state->args[1]) {
    default:
    case 0:
        state->attacker = BattleAnimSystem_GetAttacker(state->context);
        state->defender = BattleAnimSystem_GetDefender(state->context);
        break;
    case 1:
        state->attacker = BattleAnimSystem_GetDefender(state->context);
        state->defender = BattleAnimSystem_GetAttacker(state->context);
        break;
    case 2:
        state->attacker = BattleAnimSystem_GetAttacker(state->context);
        state->defender = BattleAnimSystem_GetDefender(state->context);
        break;
    case 3:
        state->attacker = BattleAnimSystem_GetDefender(state->context);
        state->defender = BattleAnimSystem_GetAttacker(state->context);
        break;
    case 4:
        state->attacker = BattleAnimSystem_GetAttacker(state->context);
        state->defender = BattleAnimSystem_GetDefender(state->context);
        break;
    }
    ov07_022208B4(task, state);
    Heap_Free(state);
}
