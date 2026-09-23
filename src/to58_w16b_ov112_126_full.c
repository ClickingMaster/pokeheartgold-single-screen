#include "global.h"
extern void *SaveArray_PCStorage_Get(void *);
extern void sub_020326A4(void *, int, int);
extern void sub_02032720(void *, void *);
extern void *PCStorage_GetMonByIndexPair(void *, int, int);
extern void Pokewalker_SetBoxMon(void *, void *);
extern void PCStorage_DeleteBoxMonByIndexPair(void *, int, int);
extern void ov112_021EECF0(void *);
int ov112_021EE628(void *);
int ov112_021EE628(void *s) {
    void *pc = SaveArray_PCStorage_Get(*(void **)((u8 *)s + 0x20));
    int box = *(int *)((u8 *)s + 0x1d770);
    int slot = *(int *)((u8 *)s + 0x1d774);
    sub_020326A4(*(void **)((u8 *)s + 0x1e440), 2, box);
    sub_02032720(*(void **)((u8 *)s + 0x1e440), *(void **)((u8 *)s + 0x1d798));
    Pokewalker_SetBoxMon(*(void **)((u8 *)s + 0x1e440), PCStorage_GetMonByIndexPair(pc, box, slot));
    PCStorage_DeleteBoxMonByIndexPair(pc, box, slot);
    ov112_021EECF0(s);
    return 2;
}

extern int (*ov112_021FF8D8[])(void *);
int ov112_021EE688(void *);
int ov112_021EE688(void *s) {
    int result = ov112_021FF8D8[*(int *)((u8 *)s + 8)](s);
    *(int *)((u8 *)s + 8) = result;
    if (result == 8) {
        *(int *)((u8 *)s + 8) = 0;
        return 3;
    }
    return 2;
}
