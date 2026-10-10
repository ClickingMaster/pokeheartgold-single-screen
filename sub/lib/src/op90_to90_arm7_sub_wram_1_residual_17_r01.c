#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define HW16(a)   (*(volatile u16 *)(a))
#define HW32(a)   (*(volatile u32 *)(a))
#define HW8(a)    (*(volatile u8 *)(a))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*((u8 *)(p) + (o)))

typedef struct W35245Param {
    u32 count;
    const u8 *source;
    u8 *destination;
    u32 verified;
} W35245Param;

typedef struct W35245Status {
    u32 checked;
    W35245Param param;
} W35245Status;

extern W35245Status status_checked$3825;
extern W35245Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arg$3983[];
extern u8 buf$3759[];
extern u8 arg$3806[];
void OS_Sleep(s32);
u64 OS_GetTick(void);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32);
void OS_WakeupThread(void *);
void OS_WakeupThreadDirect(void *);
u8 CARDi_CommandReadStatus(void);
void CARDi_CommandEnd(s32, s32);
int CARDi_WaitPrevCommand(void);
void CARDi_WriteEnable(void);
void CARDi_SendSpiAddressingCommand(u32, u32);
void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35245Param *);
void CARDi_CommWriteCore(W35245Param *);
void CARDi_CommVerifyCore(W35245Param *);
void CARDi_CommArray(const void *, void *, u32, void (*)(W35245Param *));
void CARDi_SetRomOp(u32, u32);
u32 CARDi_GetRomFlag(u32);

typedef struct W35245Wait {
    u8 delay, pad[3];
    u32 pending;
} W35245Wait;

extern W35245Wait _03806AF4;
void CARDi_CommArray(const void *src, void *dst, u32 size, void (*core)(W35245Param *));
#pragma opt_propagation off

void CARDi_CommArray(const void *src, void *dst, u32 size, void (*core)(W35245Param *)) {
    W35245Param *p = &cardi_param;
    p->source = src;
    p->destination = dst;
    HW16(0x040001a0) = 0xa040;
    while (size) {
        if (_03806AF4.pending && (((s32)U32((void *)U32(cardi_common, 0), 4) >> 16) & 255) == 255) {
            u64 start = OS_GetTick();
            while (((OS_GetTick() - start) * 64000) / 33514 < (s32)_03806AF4.delay) {}
            HW16(0x040001a0) = 0xa042;
            while (HW16(0x040001a0) & 128) {}
            HW16(0x040001a2) = 0;
            while (HW16(0x040001a0) & 128) {}
            { volatile u16 discard = HW16(0x040001a2); }
            _03806AF4.pending = 0;
            start = OS_GetTick();
            while (((OS_GetTick() - start) * 64000) / 33514 < (s32)_03806AF4.delay) {}
        }
        if (--p->count == 0) {
            HW16(0x040001a0) = 0xa000;
            _03806AF4.pending = 1;
        } else {
            HW16(0x040001a0) = 0xa040;
        }
        while (HW16(0x040001a0) & 128) {}
        core(p);
        if (!p->count) {
            break;
        }
        size--;
    }
    if (!p->count) {
        HW16(0x040001a0) = 0;
    }
}
