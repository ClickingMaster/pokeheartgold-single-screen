#include "global.h"

void ov41_02248E84(void *context, u32 *out);

u32 sub_0202BA70(void *, int);
int sub_0202BAB0(void *, int);

void ov41_02248E84(void *context, u32 *out)
{
    int i;
    for (i = 0; i < 100; i++) {
        out[i] = sub_0202BA70(context, i);
    }
    for (i = 0; i < 18; i++) {
        out[100 + i] = 18;
    }
    for (i = 0; i < 18; i++) {
        int id = sub_0202BAB0(context, i);
        if (id != 18) {
            out[100 + id] = i;
        }
    }
}
