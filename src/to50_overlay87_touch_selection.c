#include "global.h"

#include "touchscreen.h"

typedef struct {
    u8 raw[0x4000];
} State;
typedef struct {
    u8 raw[12];
    void *sprite;
} SpriteWrap;
typedef struct {
    s16 x, y;
} Pair16;
typedef struct {
    u8 keep : 1;
    u8 clear : 7;
} Ov87Flags;
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern const Pair16 ov87_021E81A0[];
extern const s16 ov87_021E81A2[];
extern const Pair16 ov87_021E82E4[];
extern const TouchscreenHitbox ov87_021E81C0[];
int Options_GetFrame(void *);
void ov87_021E7FEC(SpriteWrap *, int, int);
void ov87_021E7FE0(SpriteWrap *, int);
void ov87_021E7FD4(SpriteWrap *, int);
void ov87_021E8058(SpriteWrap *, int);
void ov87_021E806C(SpriteWrap *, const void *);
void ov87_021E8014(SpriteWrap *, int *, int *);
void ov87_021E8134(void *, int);
int ov87_021E7048(State *);
void ov87_021E73E0(State *, u8);
void ov87_021E7A2C(State *);
void ov87_021E7334(State *);
void ov87_021E7324(State *, int);
void ov87_021E734C(void *, int);
void ov87_021E74B8(State *);
void ov87_021E74D4(State *);
void ov87_021E7460(State *, int, int, int, u8);
void BgSetPosTextAndCommit(void *, int, int, int);
void BG_LoadCharTilesData(void *, int, int, void *, int);
void BgCommitTilemapBufferToVram(void *, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
int PlaySE(int);
void ClearFrameAndWindow2(void *, int);
BOOL System_GetTouchHeldCoords(u32 *, u32 *);

int ov87_021E5B48(State *p);
int ov87_021E5C38(State *p);
int ov87_021E5CEC(State *p);
int ov87_021E64F8(State *p);

int ov87_021E5C38(State *p) {
    int hit;
    u32 x, y;
    switch (U8(p, 8)) {
    case 0:
        GfGfx_EngineATogglePlanes(4, 1);
        U8(p, 0xf) = 30;
        U8(p, 8) = 1;
        break;
    case 1:
        if (U8(p, 0xf) == 0) {
            GfGfx_EngineATogglePlanes(4, 0);
            ov87_021E7FD4(PTR(p, 0x340), 0);
        } else {
            U8(p, 0xf)
            --;
        }
        hit = TouchscreenHitbox_FindRectAtTouchHeld(ov87_021E81C0);
        if (hit != -1) {
            GfGfx_EngineATogglePlanes(4, 0);
            ov87_021E7FD4(PTR(p, 0x340), 0);
            PlaySE(0x5e4);
            u8 *selected = (u8 *)p + U8(p, 0xe);
            selected[0x15] = hit;
            U8(p, 0xe)
            ++;
            System_GetTouchHeldCoords(&x, &y);
            ClearFrameAndWindow2((u8 *)p + 0x14c, 0);
            GfGfx_EngineBTogglePlanes(1, 0);
            U8(p, 8) = 2;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

int ov87_021E5CEC(State *p) {
    int index = U8(p, 0xe);
    int selected;
    index--;
    selected = U8((u8 *)p + index, 0x15);
    switch (U8(p, 8)) {
    case 0:
        *(fx32 *)((u8 *)p + 0x1c) = FX32_ONE;
        *(fx32 *)((u8 *)p + 0x20) = FX32_ONE;
        *(fx32 *)((u8 *)p + 0x24) = FX32_ONE;
        S16(p, 0x10) = 0;
        ((Ov87Flags *)((u8 *)p + 0x3a0))->clear = 0;
        ov87_021E7FE0(*(SpriteWrap **)((u8 *)p + 0x32c + selected * 4), 0);
        U8(p, 8) = 1;
        break;
    case 1:
        ov87_021E73E0(p, (u8)selected);
        *(fx32 *)((u8 *)p + 0x1c) += 0x100;
        *(fx32 *)((u8 *)p + 0x20) += 0x100;
        *(fx32 *)((u8 *)p + 0x24) += 0x100;
        S16(p, 0x10)
        ++;
        ov87_021E806C(*(SpriteWrap **)((u8 *)p + 0x32c + selected * 4), (u8 *)p + 0x1c);
        if (*(fx32 *)((u8 *)p + 0x1c) == 0x1800) {
            S16(p, 0x10) = 0;
            *(fx32 *)((u8 *)p + 0x1c) = FX32_ONE;
            *(fx32 *)((u8 *)p + 0x20) = FX32_ONE;
            *(fx32 *)((u8 *)p + 0x24) = FX32_ONE;
            PlaySE(0x560);
            U8(p, 8) = 2;
        }
        break;
    case 2: {
        int i;
        u8 *q;
        S16(p, 0x10) += 16;
        i = 0;
        q = (u8 *)p;
        for (; i < 4; i++, q += 4) {
            int x, y;
            ov87_021E8014(*(SpriteWrap **)(q + 0x32c), &x, &y);
            ov87_021E7FEC(*(SpriteWrap **)(q + 0x32c), x + 16, y);
        }
        if (S16(p, 0x10) >= 256) {
            ov87_021E8058(*(SpriteWrap **)((u8 *)p + 0x32c + selected * 4), 0);
            U8(p, 8) = 3;
        }
        break;
    }
    case 3:
        S16(p, 0x10) = 0;
        return TRUE;
    }
    return FALSE;
}
