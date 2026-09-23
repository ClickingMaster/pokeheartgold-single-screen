#include "global.h"
void *ov06_0221BA40(void *);
int sub_02015264(int, int, void *);
void sub_0201526C(void *, int, int, int);
void *ov07_0221FE88(void *, int, int);
void *ov07_0221FEB0(void *, int, int, int);

void *ov07_0221FE88(void *state, int a, int b) {
    void *result = ov06_0221BA40(state);
    int value = sub_02015264(0x1D, a, state);
    sub_0201526C(result, value, 10, b);
    return result;
}

void *ov07_0221FEB0(void *state, int a, int b, int c) {
    void *result = ov06_0221BA40(state);
    int value = sub_02015264(a, b, state);
    sub_0201526C(result, value, 10, c);
    return result;
}
