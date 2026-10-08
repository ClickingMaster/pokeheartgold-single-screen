#pragma force_active on
#include "global.h"
typedef struct {
    s32 cur;
    s32 a, b, c, d, e;
} Motion;
typedef struct {
    s16 v[4];
} Timers;
typedef struct {
    u8 pad0[0x20];
    Timers timers;
} TimerTable;
typedef struct {
    s32 speed;
    s32 weight;
} Choice;
typedef struct {
    u8 pad0[0x114];
    u8 objA[0x320 - 0x114];
    u8 objB[0x4D0 - 0x320];
    MtxFx43 mtx;
    u8 pad500[0x1FC8 - 0x500];
    VecFx32 dir;
    u8 pad1FD4[0x2AE0 - 0x1FD4];
    s32 tick;
    s32 pad2AE4[2];
    s32 mode;
    u8 pad2AF0[0x2B1C - 0x2AF0];
    s16 velX;
    s16 velY;
    s16 time;
    s16 phase;
    Motion settledX;
    Motion settledY;
    Motion impulseX;
    Motion impulseY;
    s32 state;
    s32 settled;
    s32 due;
    s32 active;
    s32 pad2B94;
    s16 nextX;
    s16 nextY;
} World;
extern const TimerTable ov92_02263C34;
extern const VecFx32 ov92_02263C8C;
extern const VecFx32 ov92_02263C74;
extern const VecFx32 ov92_02263C80;
extern const Choice ov92_02263D84[][4];
extern const s32 ov92_02263D88[][8];
int ov92_022607F8(Motion *);
int ov92_02260428(void *, s32, s32, s32, s32, f32, s32);
void ov92_02260798(Motion *, s32, s32, s32, s32);
void MTX_MultVec43(const VecFx32 *, const MtxFx43 *, VecFx32 *);
u32 GetLCRNGSeed(void);
u16 LCRandom(void);
void SetLCRNGSeed(u32);
void ov92_0225FF1C(World *w);

// @fn ov92_0225FF1C
void ov92_0225FF1C(World *w) {
    Timers timers;
    VecFx32 v1;
    VecFx32 v2;
    VecFx32 v3;
    s16 x;
    s32 y;
    s32 oldX;
    s32 oldY;
    int r1;
    int r2;
    int i;
    s32 total;
    int rnd;
    u32 seed;
    s32 speed;
    s16 dx, dz;
    s16 sx, sy;
    s16 vx, vy;

    timers = ov92_02263C34.timers;
    w->tick++;
    if (w->time != 0) {
        if (w->phase < 9) {
            if (w->phase <= 8) {
                oldX = w->impulseX.cur;
                oldY = w->impulseY.cur;
                r1 = ov92_022607F8(&w->impulseX);
                r2 = ov92_022607F8(&w->impulseY);
                x = (s16)((w->impulseX.cur - oldX) >> 12);
                y = (s16)((w->impulseY.cur - oldY) >> 12);
                if (r1 == 1 || r2 == 1) {
                    w->impulseX.cur -= oldX;
                    w->impulseY.cur -= oldY;
                }
            } else {
                x = (s16)(w->impulseX.cur >> 12);
                y = (s16)(w->impulseY.cur >> 12);
            }
        } else {
            int s1, s2;
            oldX = w->settledX.cur;
            oldY = w->settledY.cur;
            s1 = ov92_022607F8(&w->settledX);
            s2 = ov92_022607F8(&w->settledY);
            x = (s16)((w->settledX.cur - oldX) >> 12);
            y = (s16)((w->settledY.cur - oldY) >> 12);
            if ((s1 == 1 || s2 == 1) && w->settled == 0) {
                w->settled = 1;
                w->settledX.cur -= oldX;
                w->settledY.cur -= oldY;
            }
        }
        ov92_02260428(w->objA, 0, 0, x, y, 1.88f, 0);
        if (ov92_02260428(w->objB, 0, 0, x, y, 0.8f, 0)) {
            v1 = ov92_02263C8C;
            MTX_MultVec43(&v1, &w->mtx, &w->dir);
        }
        w->phase++;
        w->time--;
        return;
    }
    if (w->tick % timers.v[w->mode] == 0) {
        w->tick = 0;
        w->due = 1;
    }
    if (w->state == 0 && w->settled == 1) {
        y = (s16)(w->settledY.cur >> 12);
        x = (s16)(w->settledX.cur >> 12);
        ov92_02260428(w->objA, 0, 0, x, y, 1.88f, 0);
        if (ov92_02260428(w->objB, 0, 0, x, y, 0.8f, 0)) {
            v2 = ov92_02263C74;
            MTX_MultVec43(&v2, &w->mtx, &w->dir);
        }
    } else if (w->state == 1 && w->settled == 1) {
        y = (s16)(w->settledY.cur >> 12);
        x = (s16)(w->settledX.cur >> 12);
        ov92_02260428(w->objA, 0, 0, x, y, 1.88f, 0);
        if (ov92_02260428(w->objB, 0, 0, x, y, 0.8f, 0)) {
            v3 = ov92_02263C80;
            MTX_MultVec43(&v3, &w->mtx, &w->dir);
        }
    }
    if (w->active != 0 && (w->due != 1 || w->settled != 1)) {
        return;
    }
    seed = GetLCRNGSeed();
    rnd = LCRandom();
    SetLCRNGSeed(seed);
    rnd = (u32)rnd % 100;
    total = ov92_02263D88[w->mode][0];
    for (i = 0; i < 3; i++) {
        if (rnd < total) {
            break;
        }
        total += ov92_02263D84[w->mode][i + 1].weight;
    }
    speed = ov92_02263D84[w->mode][i].speed;
    dx = w->dir.x >> 12;
    dz = w->dir.z >> 12;
    if (w->dir.z == 0) {
        if (LCRandom() % 2) {
            sy = 1;
        } else {
            sy = -1;
        }
    } else {
        sy = -(dz >= 0 ? 1 : -1);
    }
    if (w->dir.x == 0) {
        if (LCRandom() % 2) {
            sx = 1;
        } else {
            sx = -1;
        }
    } else {
        if (dx >= 0) {
            sx = 1;
        } else {
            sx = -1;
        }
        if (sy < 0) {
            sx *= -1;
        }
    }
    if (w->mode == 0) {
        sx = 0;
    }
    vx = speed * sx;
    vy = speed * sy;
    sx = 20 * sx;
    sy = 20 * sy;
    w->nextX = vx;
    w->nextY = vy;
    ov92_02260798(&w->impulseX, 0, FX32_CONST(sx), 0, 8);
    ov92_02260798(&w->impulseY, 0, FX32_CONST(sy), 0, 8);
    ov92_02260798(&w->settledX, 0, FX32_CONST(vx), 0, 12);
    ov92_02260798(&w->settledY, 0, FX32_CONST(vy), 0, 12);
    w->time = 22;
    w->phase = 0;
    w->velX = vx;
    w->velY = vy;
    w->state = 0;
    w->due = 0;
    w->settled = 0;
    w->active = 1;
}
