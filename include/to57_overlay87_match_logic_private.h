#ifndef TO50_OVERLAY87_R12_PRIVATE_H
#define TO50_OVERLAY87_R12_PRIVATE_H
#include "global.h"
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
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern const s16 ov87_021E82C0[];
extern const s16 ov87_021E82C2[];
extern const Pair16 ov87_021E82E4[];
int PlaySE(int);
void ov87_021E7F28(State *, int);
void ov87_021E7FD4(SpriteWrap *, int);
void ov87_021E7FEC(SpriteWrap *, int, int);
void ov87_021E803C(SpriteWrap *, int);
void ov87_021E8078(SpriteWrap *, int);
void ov87_021E80A8(SpriteWrap *, int);
void ov87_021E7590(State *, int, int);
int ov87_021E75E0(State *, int);
void ov87_021E7698(State *);
int ov87_021E7734(State *);
u32 ov87_021E788C(State *);
int ov87_021E78D8(State *, int);
#endif
