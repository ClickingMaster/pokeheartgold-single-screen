#include "global.h"
void *ov06_0221BA88(void *);
int ov07_0221FEDC(void *, int, void *);
void sub_0201526C(void *, int, int, int);
int sub_02015494(void *, void *, void *);
extern void *ov07_022353E4[];
void *ov07_0221FEE4(void *, void *, int, int);
int ov07_0221FF18(void *, void *, int);

void *ov07_0221FEE4(void *state, void *obj, int b, int c) {
    void *result = ov06_0221BA88(obj);
    if (result == NULL) return NULL;
    int value = ov07_0221FEDC(state, b, obj);
    sub_0201526C(result, value, 10, c);
    return result;
}
