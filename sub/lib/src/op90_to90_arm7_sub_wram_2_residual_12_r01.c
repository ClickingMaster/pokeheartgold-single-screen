#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u16 makerCode;
    u8 x2;
    u8 x3;
    u8 x4;
    u8 isAgb : 1;
    u8 isPulledOut : 1;
    u8 rest : 6;
    u8 x6;
    u8 x7;
    u32 gameCode;
} ModuleInfo;
typedef struct {
    u32 owned;
    u32 irq;
} Lock;
typedef struct {
    u32 c1st;
    u32 c2nd;
} Cycle;
typedef struct {
    u8 x00[0xac];
    u32 gameCode;
    u8 xb0[2];
    u8 fixed;
    u8 xb3[0xbe - 0xb3];
    u16 makerCode;
} CartHeader;
#define CART ((CartHeader *)0x08000000)
extern u16 CTRDGi_EnableFlag[];
extern int CTRDGi_LockByProcessor(u32, Lock *);
extern void CTRDGi_UnlockByProcessor(u32, Lock *);
extern void OS_RestoreInterrupts(u32);
extern void CTRDGi_ChangeLatestAccessCycle(Cycle *);
extern void MI_SetCartridgeRomCycle1st(u32);
extern void MI_SetCartridgeRomCycle2nd(u32);
BOOL CTRDG_IsExisting(void);
// @fn CTRDG_IsExisting
BOOL CTRDG_IsExisting(void)
{
    BOOL ret = TRUE;
    Lock lock;
    Cycle cyc;
    ModuleInfo *mi = (ModuleInfo *)0x027ffc30;
    u8 fixed;
    CartHeader *h = CART;
    if (mi->makerCode == 0xffff)
        return FALSE;
    if (mi->isPulledOut == 1)
        return FALSE;
    if (CTRDGi_LockByProcessor(CTRDGi_EnableFlag[3], &lock) == 0) {
        OS_RestoreInterrupts(lock.irq);
        return ret;
    }
    CTRDGi_ChangeLatestAccessCycle(&cyc);
    fixed = h->fixed;
    if ((fixed == 0x96 && mi->makerCode != h->makerCode) || (fixed != 0x96 && mi->makerCode != *(u16 *)0x0801fffe)
        || (mi->gameCode != h->gameCode && mi->isAgb)) {
        ret = FALSE;
        mi->isPulledOut = 1;
    }
    MI_SetCartridgeRomCycle1st(cyc.c1st);
    MI_SetCartridgeRomCycle2nd(cyc.c2nd);
    CTRDGi_UnlockByProcessor(CTRDGi_EnableFlag[3], &lock);
    return ret;
}
