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
} M35582;

#define MLME ((M35582 *)(BASE + 4 + 0x400))
void MLME_JoinTimeOut(void);

void MLME_JoinTimeOut(void) {
    M35582 *p = MLME;
    p->result = 7;
    p->state = 37;
    AddTask(2, 1);
}
