#include "overlay92_sol_partial_internal.h"

extern void Comm_RegisterCommandTable(const void *, int, void *);

void ov92_0225C5A8(void *arg0);

void ov92_0225C5A8(void *arg0) {
    const void *arg1 = ov92_0225C59C();
    int arg2 = ov92_0225C5A4();
    Comm_RegisterCommandTable(arg1, arg2, arg0);
}
