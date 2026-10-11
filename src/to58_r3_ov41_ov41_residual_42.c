#include "global.h"

void ov41_02248E84(void *context, u32 *out);

u32 FashionCase_GetAccessoryCount(void *, int);
int FashionCase_GetWallpaperOrder(void *, int);

void ov41_02248E84(void *context, u32 *out)
{
    int i;
    for (i = 0; i < 100; i++) {
        out[i] = FashionCase_GetAccessoryCount(context, i);
    }
    for (i = 0; i < 18; i++) {
        out[100 + i] = 18;
    }
    for (i = 0; i < 18; i++) {
        int id = FashionCase_GetWallpaperOrder(context, i);
        if (id != 18) {
            out[100 + id] = i;
        }
    }
}
