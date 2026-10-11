#include "global.h"

extern int Comm_GetLocalNetId(void);
extern int Comm_CountConnected(void);
void *ov73_021E6BC8(void *, void *);

void *ov73_021E6BC8(void *work, void *result) {
    if (Comm_GetLocalNetId() == 0) {
        Comm_CountConnected();
    }
    if (*(u32 *)((u8 *)work + 0x4A08) == Comm_CountConnected()) {
        *(u32 *)((u8 *)work + 0x318) = 26;
    }
    return result;
}
