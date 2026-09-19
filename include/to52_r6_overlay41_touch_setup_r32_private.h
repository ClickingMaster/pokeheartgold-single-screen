#ifndef W17_R6_PRIVATE_H
#define W17_R6_PRIVATE_H
#include "global.h"

#include "touchscreen.h"
typedef struct Node {
    u32 a, b;
    struct Node *next;
} Node;
typedef struct Config {
    void *resource;
    u32 f04, f08, f0c, f10, f14, f18, f1c, f20, f24, f28, f2c;
} Config;
BOOL ov41_02249AA8(Node *, int, int, int);
void ov41_02249C7C(void *, const Config *);
BOOL ov41_022481D8(void *, u32, u32);
Node *ov41_022481F4(void *, int, int, int);
void ov41_0224825C(u8 *, int, void *);
#endif
