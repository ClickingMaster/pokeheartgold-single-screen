#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380FFF4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

typedef struct {
    u16 state, pad;
    u32 samples, total;
    u16 power, bbp13, bbp35, pad12, index, result;
    u8 *req, *confirm;
} M37025;

typedef struct {
    u8 pad[8];
    u16 value;
} Slot37025;

extern u32 __VENEER__u32_div_f(u32, u32);
extern u16 BBP_Read(u16);
extern void BBP_Write(u16, u16), WSetCCA_ED(u16, u16), WSetChannel(u16, int), WStart(void), WStop(void), WSetForcePowerState(u16), MLME_MeasChanTimeOut(void), SetupTimeOut(u32, void (*)(void)), IssueMlmeConfirm(void), AddTask(int, int);
extern u32 WL_ReadByte(const void *);
extern u32 FLASH_VerifyCheckSum(u32 *);
void MLME_MeasChannelTask(void);

void MLME_MeasChannelTask(void) {
    u8 *base = BASE;
    u8 *w = base + 0x344;
    M37025 *p = (M37025 *)(base + 4 + 0x400);
    switch (p->state) {
    case 0x80:
        p->index = 0;
        p->bbp13 = BBP_Read(0x13);
        p->bbp35 = BBP_Read(0x35);
        WSetCCA_ED(H(p->req, 0x12), H(p->req, 0x14));
        H(w, 12) = 4;
        p->result = 0;
    case 0x81: {
        u32 ch;
        p->samples = 0;
        p->total = 0;
        ch = WL_ReadByte(p->req + 0x18 + p->index);
        if (!ch || p->index >= 16) {
            p->state = 0x84;
            break;
        }
        if (FLASH_VerifyCheckSum(0)) {
            p->result = 14;
            p->state = 0x84;
            break;
        }
        if (p->state == 0x80) {
            WSetChannel(ch, 0);
            WStart();
            p->power = *(volatile u16 *)0x04808040;
            WSetForcePowerState(0x8000);
        } else {
            WSetChannel(ch, 0);
        }
        p->state = 0x82;
        SetupTimeOut(H(p->req, 0x16), MLME_MeasChanTimeOut);
    }
    case 0x82:
        p->samples++;
        if (*(volatile u16 *)0x0480819c & 1) {
            p->total += 100;
        }
        break;
    case 0x83: {
        u32 ch = WL_ReadByte(p->req + 0x18 + p->index);
        u32 level = 0;
        if (p->samples && p->total) {
            level = __VENEER__u32_div_f(p->total, p->samples) + 1;
            if (level > 100) {
                level = 100;
            }
        }
        ((Slot37025 *)(p->confirm + p->index * 2))->value = ch | (level << 8);
        p->index++;
        p->state = 0x81;
        break;
    }
    case 0x84: {
        u32 i;
        WStop();
        H(w, 12) = H(BASE, 0x32e);
        BBP_Write(0x13, p->bbp13);
        BBP_Write(0x35, p->bbp35);
        WSetForcePowerState(p->power);
        H(p->confirm, 4) = p->result;
        p->state = 0;
        i = p->index;
        while (i < 16) {
            H(p->confirm, 8 + i * 2) = 0;
            i++;
        }
        IssueMlmeConfirm();
        break;
    }
    }
    if (p->state) {
        AddTask(2, 5);
    }
}
