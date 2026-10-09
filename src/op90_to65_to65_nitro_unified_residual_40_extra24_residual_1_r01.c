// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { vu16 subpInitialized; u16 lockID; } CTRDGWorkView;
typedef struct { u32 a, b; } CTRDGLockInfo;
typedef struct { u32 a, b; } CTRDGCycleInfo;
typedef struct {
    u8 pad[0xac];
    u32 gameCode;
    u16 makerCode;
    u8 pad2[3];
    u8 unk[3];
    u8 pad3[6];
    u16 isNitro;
} CTRDGHeaderView;
typedef struct { u16 isNitro; u8 unk[3]; u8 pad; u16 makerCode; u32 gameCode; } CTRDGModuleInfoView;

extern CTRDGWorkView CTRDGi_Work;
extern u8 _021E4D80[];
extern u8 _021E4D84[];
void CTRDGi_LockByProcessor(u16 lockID, CTRDGLockInfo *info);
void CTRDGi_UnlockByProcessor(u16 lockID, CTRDGLockInfo *info);
void CTRDGi_ChangeLatestAccessCycle(CTRDGCycleInfo *info);
void CTRDGi_RestoreAccessCycle(CTRDGCycleInfo *info);
void CTRDGi_SendtoPxi(u32 data);
BOOL CTRDG_IsExisting(void);
void CTRDGi_InitModuleInfo(void);
#pragma push
#pragma optimization_level 2
#pragma opt_propagation off
void CTRDGi_InitModuleInfo(void)
{
    CTRDGLockInfo lock;
    CTRDGCycleInfo cycle;
    OSIrqMask lastMask;
    BOOL lastIme;
    int priority;
    int i;
    static BOOL _021E4D60;

    if (_021E4D60) {
        return;
    }
    {
        BOOL t = TRUE;
        u16 post = *(vu16 *)0x04000300;
        _021E4D60 = t;
        if (!(post & 1)) {
            return;
        }
    }
    lastMask = OS_SetIrqMask(0x40000);
    lastIme = OS_EnableIrq();
    CTRDGi_LockByProcessor(CTRDGi_Work.lockID, &lock);
    priority = (reg_MI_EXMEMCNT & 0x8000) >> 15;
    CTRDGi_ChangeLatestAccessCycle(&cycle);
    reg_MI_EXMEMCNT = (u16)(reg_MI_EXMEMCNT & ~0x8000);
    DC_InvalidateRange((u8 *)_021E4D80 + 0x80, 0x40);
    MI_DmaCopy16(1, (void *)0x08000080, (void *)((u32)_021E4D80 + 0x80), 0x40);
    reg_MI_EXMEMCNT = (u16)((reg_MI_EXMEMCNT & ~0x8000) | (priority << 15));
    CTRDGi_RestoreAccessCycle(&cycle);
    CTRDGi_UnlockByProcessor(CTRDGi_Work.lockID, &lock);
    if (*(u8 *)0x027FFF9B != 0 || *(u8 *)0x027FFF9A == 0) {
        CTRDGHeaderView *hdr = (CTRDGHeaderView *)_021E4D80;
        CTRDGModuleInfoView *info = (CTRDGModuleInfoView *)0x027FFC30;
        info->isNitro = hdr->isNitro;
        for (i = 0; i < 3; i++) {
            info->unk[i] = hdr->unk[i];
        }
        info->makerCode = hdr->makerCode;
        info->gameCode = hdr->gameCode;
        *(u8 *)0x027FFF9B = CTRDG_IsExisting() ? 1 : 0;
        *(u8 *)0x027FFF9A = 1;
    }
    MIi_CpuCopy32((u32 *)0xFFFF0020, (u32 *)_021E4D84, 0x9c);
    DC_FlushAll();
    CTRDGi_SendtoPxi((((u32)((u8 *)_021E4D80 - 0x02000000)) >> 5) << 6 | 1);
    while (CTRDGi_Work.subpInitialized != 1) {
        SVC_WaitByLoop(1);
    }
    (void)OS_RestoreIrq(lastIme);
    OS_SetIrqMask(lastMask);
}
#pragma pop
