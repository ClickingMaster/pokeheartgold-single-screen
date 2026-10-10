#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int, int);

typedef struct {
    u16 state, x, result, reason, y, elapsed, step, z;
    u32 a, b;
    u8 *req, *confirm;
} M35580;

#define MLME ((M35580 *)(BASE + 4 + 0x400))
void MLME_ScanTimeOut(void);

void MLME_ScanTimeOut(void) {
    M35580 *p = MLME;
    p->elapsed += p->step;
    if (p->elapsed >= *(u16 *)(p->req + 0x4a)) {
        if (p->reason < 16) {
            p->state = 17;
        } else {
            p->state = 21;
        }
    }
    AddTask(2, 0);
}
