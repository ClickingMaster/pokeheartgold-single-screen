#include "global.h"

extern int sub_0203769C(void);
extern int sub_02037454(void);
extern int sub_02039EB4(void *, u32, void *);
extern void StopSE(u32, u32);
extern void ov73_021E756C(void *, u32, u32);
extern void ov73_021E670C(void *, u32);
extern void Sprite_SetAnimCtrlSeq(void *, u32);
extern u8 gSystem[];
void *ov73_021E6C50(void *, void *);

void *ov73_021E6C50(void *work, void *result) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (sub_02039EB4(**(void ***)((u8 *)work + 8), 2, (u8 *)work + 0x374)) {
        StopSE(0x657, 8);
        ov73_021E756C(work, 13, 0);
        ov73_021E670C(work, 29);
        Sprite_SetAnimCtrlSeq(*(void **)((u8 *)work + 0x228), 0);
        *(u32 *)((u8 *)work + 0xc) = 0;
        *(u32 *)((u8 *)work + 0x324) = 0;
        gSystem[0x6c] = 0;
        *(u8 *)((u8 *)work + 0x4A14) = 0;
    }
    return result;
}
