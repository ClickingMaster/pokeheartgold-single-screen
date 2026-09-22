#include "to57_overlay112_052_private.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define PARTICLES(p) ((Ov112Particle *)((u8 *)(p) + 0x1EC80))

void ov112_021EA6D8(Ov112State *p, int index, int anim, int x, int y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(p->sprites[index + 0x75], (s16)x, (s16)(y + 208), 1 << 20);
        ManagedSprite_SetAnim(p->sprites[index + 0x75], anim + 4);
        ManagedSprite_SetDrawFlag(p->sprites[index + 0x75], 1);
        ManagedSprite_SetDrawFlag(p->sprites[index + 0x11], 0);
    } else {
        ManagedSprite_SetPositionXY(p->sprites[index + 0x11], (s16)x, (s16)y);
        ManagedSprite_SetAnim(p->sprites[index + 0x11], anim + 0x13);
        ManagedSprite_SetDrawFlag(p->sprites[index + 0x75], 0);
        ManagedSprite_SetDrawFlag(p->sprites[index + 0x11], 1);
    }
}

void ov112_021EA76C(Ov112State *p) {
    int i = ov112_021EA9A0(p->particles);
    if (i >= 0) {
        Ov112Particle *q;
        p->particles[i].active = 1;
        q = &p->particles[i];
        q->velocity = MTRandom() % 6 + 2;
        q->angle = MTRandom() % 300;
        q->x = 128;
        q->y = 116;
        q->speedFactor = MTRandom() % 40 + 40;
        q->life = MTRandom() % 60 + 120;
    }
}

void ov112_021EA7D0(Ov112State *p) {
    int i = ov112_021EA9A0(p->particles);
    if (i >= 0) {
        Ov112Particle *q;
        p->particles[i].active = 1;
        q = &p->particles[i];
        q->velocity = ~0 * (MTRandom() % 5 + 5);
        q->angle = MTRandom() % 300;
        q->x = 128;
        q->y = -109;
        q->speedFactor = MTRandom() % 40 + 40;
        q->life = MTRandom() % 60 + 120;
    }
}

int ov112_021EA838(Ov112State *p) {
    int i;
    int count = 0;
    Ov112Particle *q = p->particles;
    i = count;
    while (i < 100) {
        if (q->active) {
            count++;
            q->life -= 20;
        }
        i++;
        q++;
    }
    return count;
}

void ov112_021EA864(Ov112State *p) {
    int i;
    Ov112Particle *q;
    i = 0;
    q = p->particles;
    while (i < 100) {
        int amplitude = q->speedFactor * (q->y + 180) / 320;
        if (q->active) {
            int phase;
            q->angleSin = GF_SinDeg(q->angle) / 360;
            q->dx = GF_CosDeg(q->angle) * amplitude / 4096;
            q->angle += 10;
            phase = (q->angle % 360) / 73;
            ov112_021EA6D8(p, i, phase, q->x + q->dx, q->y + q->angleSin);
            q->y -= q->velocity;
            q->life--;
            if (q->y < -110 || q->life < 0 || q->y > 160) {
                q->active = 0;
                ManagedSprite_SetDrawFlag(p->sprites[i + 0x75], 0);
                ManagedSprite_SetDrawFlag(p->sprites[i + 0x11], 0);
            }
            if ((q->angle % 360) >= 0 && (q->angle % 360) <= 180) {
                ManagedSprite_SetDrawPriority(p->sprites[i + 0x75], 1);
                ManagedSprite_SetDrawPriority(p->sprites[i + 0x11], 1);
            } else {
                ManagedSprite_SetDrawPriority(p->sprites[i + 0x75], 3);
                ManagedSprite_SetDrawPriority(p->sprites[i + 0x11], 3);
            }
        }
        q++;
        i++;
    }
}

void ov112_021EA984(void *p) {
    Ov112Particle *q = PARTICLES(p);
    int i;
    for (i = 0; i < 100; i++, q++) {
        q->active = 0;
        q->life = 0;
    }
}
