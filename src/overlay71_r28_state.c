#include "global.h"
#define S32(p,n) (*(s32 *)((p)+(n)))
extern void ov71_02249A98(void *);
extern void ov71_02249A60(void *, s32, s32, s32, s32);
extern void ov71_022476C4(void *, void *);
extern int ov71_02247738(void *);
extern void ov71_02249C90(void *);
extern void ov71_02247704(void *, void *);
extern void SysTask_Destroy(void *);
void ov71_02249B64(void *, u8 *);
void ov71_02249B64(void *task, u8 *work) {
    if (S32(work, 0x68) < 60) S32(work, 0x68)++;
    switch (S32(work, 0x60)) {
    case 0:
        S32(work, 0x14) += S32(work, 0x3C);
        S32(work, 0x18) += S32(work, 0x40);
        ov71_02249A98(work);
        ov71_022476C4(*(void **)(work + 0xC), work + 0x28);
        if (ov71_02247738(*(void **)(work + 0xC))) S32(work, 0x60)++;
        break;
    case 1:
        if (!ov71_02247738(*(void **)(work + 0xC))) {
            ov71_02249C90(*(void **)work);
            S32(work, 0x60)++;
            return;
        }
        S32(work, 0x14) += S32(work, 0x3C);
        S32(work, 0x18) += S32(work, 0x40);
        ov71_02249A98(work);
        ov71_022476C4(*(void **)(work + 0xC), work + 0x28);
        return;
    case 2:
        S32(work, 0x14) += S32(work, 0x3C);
        S32(work, 0x18) += S32(work, 0x40);
        ov71_02249A98(work);
        ov71_022476C4(*(void **)(work + 0xC), work + 0x28);
        if (S32(work, 0x68) >= 60) {
            ov71_02249A60(work, 0x800, 0x800, 0x200, 12);
            S32(work, 0x64) = 0;
            S32(work, 0x60)++;
        }
        break;
    case 3:
        ov71_02249A98(work);
        ov71_022476C4(*(void **)(work + 0xC), work + 0x28);
        S32(work, 0x64)++;
        if (S32(work, 0x64) >= 12) {
            ov71_02247704(*(void **)(work + 0xC), NULL);
            S32(work, 0x64) = 0;
            S32(work, 0x60)++;
        }
        break;
    case 4:
        S32(work, 0x64)++;
        if (S32(work, 0x64) > 8) {
            S32(work, 4) = 0;
            SysTask_Destroy(task);
        }
        break;
    }
}
