#include "overlay07_callbacks_w06_private.h"

void ov07_0222BADC(void *pokepic, int arg1, int arg2, int arg3, int arg4) {
    Pokepic_SetVisible(pokepic, arg1, 0x50 - arg3, arg2, arg4);
}
