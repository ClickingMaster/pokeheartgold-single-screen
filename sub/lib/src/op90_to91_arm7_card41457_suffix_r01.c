#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u32 x00;
    u32 cmd;
    u32 count;
    u8 pad0c[0x48 - 0x0c];
    u8 thread[0xec - 0x48];
    void *cur;
    u8 padf0[0xfc - 0xf0];
    volatile u32 flag;
} CardCommon;
extern CardCommon cardi_common;
extern void OS_WakeupThreadDirect(void *);
void CARDi_OnFifoRecv(int tag, u32 data, BOOL err);
// @fn CARDi_OnFifoRecv
void CARDi_OnFifoRecv(int tag, u32 data, BOOL err)
{
    CardCommon *p;
    if (tag == 11 && err) {
        p = &cardi_common;
        if (p->count == 0)
            p->cmd = data;
        switch (p->cmd) {
        case 0:
            if (p->count == 0)
                break;
            if (p->count == 1) {
                p->x00 = data;
                p->flag |= 0x10;
            }
            break;
        case 1:
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            p->flag |= 0x10;
            break;
        }
        if (!(p->flag & 0x10)) {
            p->count++;
        } else {
            p->count = 0;
            OS_WakeupThreadDirect((p->flag & 4) ? p->cur : p->thread);
        }
    }
}
