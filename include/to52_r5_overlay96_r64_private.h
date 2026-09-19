#ifndef TO52_SOL_R5_W06_PRIVATE_H
#define TO52_SOL_R5_W06_PRIVATE_H
#include "global.h"
typedef struct Ov96R64Item {
    s32 active;
    void *handle;
    s32 delay;
    s32 played;
} Ov96R64Item;
typedef struct Ov96R64Work {
    void *root;
    Ov96R64Item items[4];
    Ov96R64Item single;
    s32 timer;
    u8 state;
    u8 unk59;
    u8 count;
    u8 index;
    u32 unk5C;
    u32 fanfarePlayed;
} Ov96R64Work;
void GF_AssertFail(void);
void PlayFanfare(u32);
void ov96_021F8128(Ov96R64Item *, s32, s32);
void ov96_021F8160(Ov96R64Work *, s32, s32);
s32 ov96_021F8180(Ov96R64Item *, s32);
void ov96_021F81C0(Ov96R64Work *, s32);
void ov96_021F82E4(Ov96R64Work *, void *, s32);
void ov96_021F8334(Ov96R64Work *, void *, s32);
void ov96_021F8354(Ov96R64Item *);
s32 ov96_021F8360(Ov96R64Work *);
void *ov96_021F8378(Ov96R64Work *, void *, s32, s32);
void *ov96_021F83D0(Ov96R64Work *);
void ov96_021F83BC(Ov96R64Work *, void *);
s32 ov96_021F8978(void *);
s32 ov96_021F81CC(Ov96R64Work *, void *);
#endif
