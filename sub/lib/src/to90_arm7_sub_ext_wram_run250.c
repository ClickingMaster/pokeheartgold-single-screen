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
} M35581;

#define MLME ((M35581 *)(BASE + 4 + 0x400))
extern void WStart(void);
extern void WStop(void);
extern void IssueMlmeConfirm(void);
extern void MLME_JoinTimeOut(void);
extern void SetupTimeOut(u32, void (*)(void));
void MLME_JoinTask(void);

void MLME_JoinTask(void) {
    M35581 *p = MLME;
    switch (p->state) {
    case 32:
        WStart();
        p->result = 0;
        p->reason = 0;
        p->state = 33;
        SetupTimeOut(*(u16 *)(p->req + 16), MLME_JoinTimeOut);
        break;
    case 37:
        *(u16 *)(p->confirm + 4) = p->result;
        *(u16 *)(p->confirm + 6) = p->reason;
        if (p->result) {
            WStop();
        }
        p->state = 0;
        IssueMlmeConfirm();
        break;
    }
}
