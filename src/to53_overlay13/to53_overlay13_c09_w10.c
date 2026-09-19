#include "private_w10.h"

void *ov13_022228A4(s32 unused, s32 size) {
    void *(*alloc)(s32);
    if (size > 0) {
        alloc = (void *(*)(s32))ov13_0224DD80[0];
        return alloc(size);
    }
    return NULL;
}
void *ov13_022228BC(s32 unused, s32 size) {
    void *(*alloc)(s32) = (void *(*)(s32))ov13_0224DD8C[0];
    return alloc(size);
}
