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
} M35579;

#define MLME    ((M35579 *)(BASE + 4 + 0x400))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void WSetStaState(int);
extern void WStart(void);
extern void WStop(void);
extern void IssueMlmeConfirm(void);
extern void MLME_ScanTimeOut(void);
extern u32 FLASH_VerifyCheckSum(u32 *);
extern u32 WL_ReadByte(const void *);
extern void WSetChannel(int, int), TxManCtrlFrame(void *), SetupTimeOut(u32, void (*)(void));
extern u8 *MakeProbeReqFrame(const void *);
void MLME_ScanTask(void);

void MLME_ScanTask(void) {
    M35579 *p;
    u8 *w;
    int schedule = 0;
    u8 *base = BASE;
    p = (M35579 *)(base + 4 + 0x400);
    w = base + 0x344;
    switch (p->state) {
    case 16:
        WSetStaState(32);
        H(w, 12) = 2;
        H(p->confirm, 8) = 0;
        H(p->confirm, 6) = 0;
        p->reason = 0;
        p->y = 0;
        if (H(p->req, 0x38) == 0) {
            p->step = ((int)H(p->req, 0x4a) + 3) / 4;
            if (p->step < 10) {
                p->step = 10;
            }
        } else {
            p->step = H(p->req, 0x4a);
        }
        H(p->confirm, 4) = 0;
    case 17: {
        int channel = WL_ReadByte(p->req + 0x3a + p->reason);
        if (!channel) {
            p->state = 21;
            schedule = 1;
            break;
        }
        p->reason++;
        p->elapsed = 0;
        if (FLASH_VerifyCheckSum(0)) {
            H(p->confirm, 4) = 14;
            p->state = 21;
            schedule = 1;
            break;
        }
        if (p->state == 16) {
            WSetChannel(channel, 0);
            WStart();
        } else {
            WSetChannel(channel, 0);
        }
        p->state = 18;
    }
    case 18:
    case 19: {
        u8 *frame;
        p->state = 19;
        if (!H(p->req, 0x38)) {
            frame = MakeProbeReqFrame(p->req + 16);
            if (!frame) {
                H(p->confirm, 4) = 8;
                p->state = 21;
                schedule = 1;
                break;
            }
            TxManCtrlFrame(frame);
        }
        SetupTimeOut(p->step, MLME_ScanTimeOut);
        break;
    }
    case 20:
        break;
    case 21:
        p->state = 0;
        WStop();
        H(w, 12) = H(BASE, 0x32e);
        IssueMlmeConfirm();
        break;
    }
    if (schedule) {
        AddTask(2, 0);
    }
}
