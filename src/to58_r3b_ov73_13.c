#include "global.h"

extern int Comm_GetLocalNetId(void);
extern int Comm_CountConnected(void);
extern void sub_02038C1C(int);
void ov73_021E781C(void *, int);

void ov73_021E781C(void *work, int index) {
    if (Comm_GetLocalNetId() == 0) {
        if (index == -1) {
            sub_02038C1C(1);
        } else {
            int count = Comm_CountConnected() + index;
            if (count > 5) {
                count = 5;
            }
            sub_02038C1C(count);
        }
        if (index == -1) {
            *(u8 *)((u8 *)work + 0x4A29) = 2;
        } else if (index == 0) {
            *(u8 *)((u8 *)work + 0x4A29) = 1;
        } else {
            *(u8 *)((u8 *)work + 0x4A29) = 0;
        }
    }
}
