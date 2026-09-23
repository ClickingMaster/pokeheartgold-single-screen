#include "global.h"

extern int ov73_021E83D4(void *);
extern int ov00_021EC5B4(void);
extern void ov73_021E844C(void *, u32, u32);
int ov73_021E8CBC(void *);

int ov73_021E8CBC(void *work) {
    int state = ov73_021E83D4(*(void **)((u8 *)work + 0x12C8));
    if (state != 0) {
        if (state == 2) {
            if (ov00_021EC5B4() == 0) {
                *(u32 *)((u8 *)work + 0x1c) = 0;
            } else {
                *(u32 *)((u8 *)work + 0x1c) = 41;
            }
        } else {
            if (ov00_021EC5B4() == 0) {
                ov73_021E844C(work, 0, 0);
                *(u32 *)((u8 *)work + 0x1c) = 46;
            } else {
                *(u32 *)((u8 *)work + 0x1c) = 41;
            }
            *(u32 *)(*(u8 **)work + 0x20) = 1;
        }
    }
    return 3;
}
