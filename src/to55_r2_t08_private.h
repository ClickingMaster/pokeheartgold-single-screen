#ifndef TO55_R2_T08_PRIVATE_H
#define TO55_R2_T08_PRIVATE_H

#include "global.h"

typedef struct Ov13StateT08 {
    int unk00;
    void *unk04;
    int unk08;
    void *unk0C;
    int unk10;
    void *unk14;
    u8 pad18[0x0C];
    void *unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
} Ov13StateT08;

typedef struct Ov13CipherStateT08 {
    u32 unk00;
    u32 unk04;
    u8 *table;
    u32 tableSize;
} Ov13CipherStateT08;

extern Ov13StateT08 _0224CF98;
extern u8 ov13_0224CFB8[];

void ov13_02222978(void *dst, int value, int size);
int ov13_02222A1C(int value);
int ov13_02222A44(int value);
int ov13_022229EC(void *a0, void *a1, int a2, int a3, const void *a4, int a5);
int ov13_022226C8(Ov13CipherStateT08 *state);
u32 _u32_div_f(u32, u32);
void ov13_022224CC(void *, void *, int, void *);
void ov13_02222638(Ov13CipherStateT08 *, const u8 *, u32, u32);
void ov13_0222269C(Ov13CipherStateT08 *, u8 *, const u8 *, u32);

#endif
