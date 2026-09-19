#ifndef W07_OV87_PRIVATE_H
#define W07_OV87_PRIVATE_H
#include "global.h"
typedef struct {
    u8 raw[0x400];
} W07State;
typedef struct {
    u8 raw[12];
    void *sprite;
} W07SpriteWrap;
typedef struct {
    u16 a;
    u16 b;
} W07Pair16;
extern const W07Pair16 ov87_021E82E4[];
extern const W07Pair16 ov87_021E8194[];
W07SpriteWrap *ov87_021E7F6C(void *, int, int, int, int, int, int, int);
void ov87_021E7FD4(W07SpriteWrap *, int);
void ov87_021E6668(W07State *);
void ov87_021E66B8(W07State *);
void ov87_021E6704(W07State *);
#endif
