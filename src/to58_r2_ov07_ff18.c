#include "global.h"

extern void *ov07_022353E4[];
extern void sub_02015494(void *, int, void *, void *);
void ov07_0221FF18(void *, int, int, void *);

void ov07_0221FF18(void *first, int second, int index, void *arg) {
    sub_02015494(first, second, ov07_022353E4[index], arg);
}
