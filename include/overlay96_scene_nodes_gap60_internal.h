#ifndef OVERLAY96_SCENE_NODES_GAP60_INTERNAL_H
#define OVERLAY96_SCENE_NODES_GAP60_INTERNAL_H
#include "global.h"
typedef struct Ov96R60Node {
    s32 zero;
    VecFx32 pos;
    void *a;
    void *b;
    void *c;
} Ov96R60Node;
extern const u16 ov96_0221C21C[];
void *ov96_021EB3E4(void *, void *, int, int, int);
void ov96_021EB52C(void *, int, int);
void ov96_021EB564(void *, int);
void ov96_021EB630(void *, int);
void ov96_021EB588(void *, VecFx32 *);
void ov96_021F7620(void *);
void ov96_021F74D0(Ov96R60Node *, void *, void *);
void ov96_021F7544(Ov96R60Node *);
void ov96_021F7598(u8 *);
void ov96_021F75BC(Ov96R60Node *, int);
#endif
