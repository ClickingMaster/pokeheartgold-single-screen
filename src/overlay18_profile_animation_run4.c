#include "overlay18_profile_animation_internal.h"

void PokedexApp_RunWeightCheckAnim(R21State *s) {
    sWeightCheckAnimFuncs[s->state](s);
}

void ov18_021F5180(R21State *s, s32 x, s32 y) {
    if (s->direction == 0) {
        s->x = -x;
        s->y = y;
    } else {
        s->x = x;
        s->y = -y;
    }
}

void ov18_021F5198(R21State *s, s32 x, s32 y, s32 z) {
    if (s->direction == 0) {
        s->x = -x;
        s->y = -y;
        s->z = -z;
    } else {
        s->x = x;
        s->y = y;
        s->z = z;
    }
}
