#include "global.h"

typedef void (*Callback)(void *);

typedef struct Work {
    void *task0;
    void *task1;
    int active;
    int ready;
    Callback vwait;
    Callback vblank;
    void *arg;
    void *manager;
    u8 rest[0x600];
    void *owner;
} Work;

extern void sub_02014AA0(void);
extern void *sub_02014A60(void *);
extern void sub_02014A8C(void *);
extern void sub_02014AB0(void *, void *, int, int);
extern void sub_02014C08(void *);
extern void sub_02014C40(void *);
extern void sub_02014A38(void *);
extern void sub_02014A4C(void *);
extern void sub_02014BD8(void *);
extern void sub_02014BF8(void *);
extern void SysTask_Destroy(void *);
extern void Heap_Free(void *);
extern const void *ov07_02236520[];
extern const void *ov07_0223649C[];
extern u32 _u32_div_f(u32, u32);
extern void ov07_0222209C(void *, void *);
extern void ov07_022220DC(void *, void *);
extern void ov07_022220B8(void *, int, void *, int);

void *ov07_02222AF4(void *p);
void ov07_02222AF8(void *task, Work *work);
void ov07_02222B14(void *task, Work *work);
void ov07_02222B70(Work *work);
void ov07_02222B94(Work *work);
void ov07_02222BA4(Work *work);
void ov07_02222BC8(Work *work);
void ov07_02222BDC(Work *work);
void ov07_02222C60(Work *work);
void ov07_02222C84(Work *work);
void ov07_02222C98(Work *work);
void ov07_02222CAC(Work *work);
void ov07_02222CC0(Work *work);
void ov07_02222D3C(Work *work);
void ov07_02222D60(Work *work);
void ov07_02222D74(Work *work);
u32 ov07_02222D88(u32 lo, u32 hi);
void ov07_02222DC8(int x0, int y0, int x1, int y1, s16 *x, s16 *y);
BOOL ov07_02222E48(int *value, int target, int step);
u32 ov07_02222EE8(Work *work);
void ov07_02222EF8(Work *work);
const void *ov07_02223038(u32 n);
const void *ov07_0222304C(u32 n);
void ov07_02223060(void);

typedef struct Linear2 {
    s16 x;
    s16 y;
    u32 frames;
    s32 dx;
    s32 dy;
    s32 curX;
    s32 curY;
} Linear2;

typedef struct Linear1 {
    s32 value;
    u32 frames;
    u32 start;
    u32 unused;
    u32 delta;
} Linear1;

void ov07_02222494(Linear1 *, u32, u32, u32, u32);
BOOL ov07_02222558(Linear2 *);
BOOL ov07_0222260C(Linear2 *);
BOOL ov07_02222748(Linear2 *, void *);
BOOL ov07_02222768(Linear2 *, void *);
BOOL ov07_02222788(Linear2 *, void *);
BOOL ov07_0222283C(void *, void *, int, int);

BOOL ov07_0222260C(Linear2 *work) {
    if (work == NULL) {
        GF_AssertFail();
    }
    if (work->frames != 0) {
        work->frames--;
        work->curX += work->dx;
        work->curY += work->dy;
        work->x = work->curX >> 12;
        work->y = work->curY >> 12;
        return TRUE;
    }
    return FALSE;
}
