#include "global.h"
void *ov06_0221BA40(void *);
int ParticleSystem_LoadResourceFile(int, int, void *);
void ParticleSystem_SetResource(void *, int, int, int);
void *ov07_0221FE88(void *, int, int);
void *ov07_0221FEB0(void *, int, int, int);

void *ov07_0221FE88(void *state, int a, int b) {
    void *result = ov06_0221BA40(state);
    int value = ParticleSystem_LoadResourceFile(0x1D, a, state);
    ParticleSystem_SetResource(result, value, 10, b);
    return result;
}

void *ov07_0221FEB0(void *state, int a, int b, int c) {
    void *result = ov06_0221BA40(state);
    int value = ParticleSystem_LoadResourceFile(a, b, state);
    ParticleSystem_SetResource(result, value, 10, c);
    return result;
}
