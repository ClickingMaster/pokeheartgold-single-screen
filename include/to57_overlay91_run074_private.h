#ifndef TO57_OVERLAY91_RUN074_PRIVATE_H
#define TO57_OVERLAY91_RUN074_PRIVATE_H
#include "global.h"

#include "sprite.h"
typedef struct Particle {
    BOOL active;
    Sprite *sprite;
    VecFx32 position;
    s32 age;
    BOOL direction;
} Particle;
void ov91_02260D98(Particle *particles);
void ov91_02260E88(Particle *particles, s32 x, s32 y);
void ov91_02260EE0(s32 frame, s32 duration, fx32 radius, s32 startAngle, s32 endAngle, s32 *x, s32 *y);
#endif
