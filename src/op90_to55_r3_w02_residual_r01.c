#pragma force_active on
#include "global.h"
typedef struct Ov112Setup {
    int state;
    void *resource;
    void *extra;
    u8 callback[4];
} Ov112Setup;
typedef struct Ov112Work {
    u8 pad0[0x18];
    void *bgConfig;
    u8 pad1C[4];
    void *saveData;
    u8 pad24[0x1D764 - 0x24];
    Ov112Setup setup;
} Ov112Work;
typedef void (*Ov112Cb)(void *);
extern void PokewalkerApp_Teardown(void *);
extern void *BgConfig_Alloc(int);
extern void ov112_021EF15C(void);
extern void *sub_020932E0(int, int, int);
extern void *ov112_021F039C(int);
extern void ov112_021EF17C(void *);
extern void ov112_021EF19C(void);
extern void ov112_021EF1CC(void *);
extern void *SaveArray_PCStorage_Get(void *);
extern void *SaveArray_Party_Get(void *);
extern void ov112_021EF298(void *);
extern void ov112_021EF300(void *);
extern void ov112_021EF3F8(void *);
extern void ov112_021EF310(void *);
extern void sub_02093440(void *, void *, void *, void *, int, int, int, int, Ov112Cb, Ov112Cb, void *, Ov112Cb, void *);
extern void ov112_021F03BC(void *, void *);
extern void Main_SetVBlankIntrCB(Ov112Cb, void *);
int ov112_021EEF68(void *p);

#define work ((u8 *)p + 0x1D764)
#define SETUP ((Ov112Setup *)work)
// @fn ov112_021EEF68
int ov112_021EEF68(void *p) {
    void *party;
    void *boxes;

    switch (*(u32 *)work) {
    case 0:
        PokewalkerApp_Teardown(p);
        *(u32 *)work = 1;
        break;
    case 1:
        *(void **)((u8 *)p + 0x18) = BgConfig_Alloc(0x9A);
        ov112_021EF15C();
        *(void **)(work + 4) = sub_020932E0(0x9A, 1, 0x12);
        *(void **)(work + 8) = ov112_021F039C(0x9A);
        ov112_021EF17C(*(void **)((u8 *)p + 0x18));
        ov112_021EF19C();
        ov112_021EF1CC(&((Ov112Work *)p)->setup);
        ((Ov112Work *)p)->setup.state = 0;
        boxes = SaveArray_PCStorage_Get(*(void **)((u8 *)p + 0x20));
        party = SaveArray_Party_Get(*(void **)((u8 *)p + 0x20));
        sub_02093440(*(void **)(work + 4), *(void **)((u8 *)p + 0x18), boxes, party, 0, 0, 1, 0, ov112_021EF298, ov112_021EF300, work + 0xC, ov112_021EF3F8, p);
        ov112_021F03BC(*(void **)(work + 8), *(void **)((u8 *)p + 0x18));
        Main_SetVBlankIntrCB(ov112_021EF310, &((Ov112Work *)p)->setup);
        *(u32 *)work = 0;
        return 2;
    }
    return 1;
}
