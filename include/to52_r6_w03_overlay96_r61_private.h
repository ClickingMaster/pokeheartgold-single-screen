#ifndef TO52_R6_W03_PRIVATE_H
#define TO52_R6_W03_PRIVATE_H
#include "global.h"
typedef struct R61Node {
    u32 active;
    u8 pad04[0xc];
    void *sprite10;
    void *sprite14;
    void *sprite18;
} R61Node;
typedef struct R61Out {
    u32 unk00;
    u32 value;
    u8 sumsA[12];
    u8 values[4];
    u8 count;
    u8 unk1D;
    u8 unk1E;
    u8 three;
    u8 start;
    u8 end;
    u8 pad22[2];
} R61Out;
void ov96_021EB564(void *, int);
void ov96_021EB52C(void *, int, int);
u16 LCRandom(void);
int _s32_div_f(int, int);
void *Heap_Alloc(u32, u32);
u8 *ov96_021E60D8(void *, u32, u32);
void ov96_021F75E8(R61Node *, s32, s32);
void ov96_021F7620(R61Node *);
R61Out *ov96_021F7684(u32, u8, u32, u32, void *);
#endif
