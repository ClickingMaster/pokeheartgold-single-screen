#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Lays out the TX slots in the MAC memory for the mode (0: test, 1: parent, 2: child, 3: other), sets their
// TX end callbacks and the fixed tails, and the data frame control template (WlBss.unk_8a; 0x4000: WEP).
void InitTxCtrl(void) {
    WlWork *w = wlWorkPlain;
    WlTxCtrl *txCtrl = &w->txCtrl;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    __VENEER_MIi_CpuClear32(0, txCtrl, 0xB0);
    w->txCtrl.txq[0].active = 0;
    txCtrl->txq[0].heapFrame = NULL;
    txCtrl->txq[1].active = 0;
    txCtrl->txq[1].heapFrame = NULL;
    txCtrl->txq[2].active = 0;
    txCtrl->txq[2].heapFrame = NULL;
    txCtrl->unk_a2 = 0xFFFF;
    txCtrl->unk_a4 = 0xFFFF;
    switch (bss->mode) {
    case 0:
        txCtrl->txq[0].frame = (u8 *)0x04804170;
        txCtrl->txq[1].frame = (u8 *)0x04804028;
        txCtrl->txq[2].frame = (u8 *)0x04804000;
        txCtrl->txq[0].callback = TxqEndData;
        txCtrl->txq[1].callback = TxqEndManCtrl;
        txCtrl->txq[2].callback = TxqEndPsPoll;
        *(vu16 *)0x04804024 = 0xB6B8;
        *(vu16 *)0x04804026 = 0x1D46;
        *(vu16 *)0x0480416C = 0xB6B8;
        *(vu16 *)0x0480416E = 0x1D46;
        *(vu16 *)0x04804790 = 0xB6B8;
        *(vu16 *)0x04804792 = 0x1D46;
        bss->unk_8a = 8;
        WL_REG(0x0AE) = 1;
        break;
    case 1:
        txCtrl->txq[0].frame = (u8 *)0x04804AA0;
        txCtrl->txq[1].frame = (u8 *)0x04804958;
        txCtrl->txq[2].frame = (u8 *)0x04804334;
        txCtrl->txq[0].callback = TxqEndData;
        txCtrl->txq[1].callback = TxqEndManCtrl;
        txCtrl->txq[2].callback = TxqEndBroadCast;
        txCtrl->beacon.frame = (u8 *)0x04804238;
        txCtrl->txq[3].frame = (u8 *)0x04804000;
        *(vu16 *)0x04804234 = 0xB6B8;
        *(vu16 *)0x04804236 = 0x1D46;
        *(vu16 *)0x04804330 = 0xB6B8;
        *(vu16 *)0x04804332 = 0x1D46;
        *(vu16 *)0x04804954 = 0xB6B8;
        *(vu16 *)0x04804956 = 0x1D46;
        *(vu16 *)0x04804A9C = 0xB6B8;
        *(vu16 *)0x04804A9E = 0x1D46;
        *(vu16 *)0x048050C0 = 0xB6B8;
        *(vu16 *)0x048050C2 = 0x1D46;
        bss->unk_8a = 0x208;
        txCtrl->beacon.frame = (u8 *)0x04804238;
        MakeBeaconFrame();
        break;
    case 2:
        txCtrl->txq[0].frame = (u8 *)0x048045D8;
        txCtrl->txq[1].frame = (u8 *)0x04804490;
        txCtrl->txq[2].frame = (u8 *)0x04804468;
        txCtrl->txq[0].callback = TxqEndData;
        txCtrl->txq[1].callback = TxqEndManCtrl;
        txCtrl->txq[2].callback = TxqEndPsPoll;
        txCtrl->txq[4].frame = (u8 *)0x04804000;
        txCtrl->txq[5].frame = (u8 *)0x04804234;
        *(vu16 *)0x04804230 = 0xB6B8;
        *(vu16 *)0x04804232 = 0x1D46;
        *(vu16 *)0x04804464 = 0xB6B8;
        *(vu16 *)0x04804466 = 0x1D46;
        *(vu16 *)0x0480448C = 0xB6B8;
        *(vu16 *)0x0480448E = 0x1D46;
        *(vu16 *)0x048045D4 = 0xB6B8;
        *(vu16 *)0x048045D6 = 0x1D46;
        *(vu16 *)0x04804BF8 = 0xB6B8;
        *(vu16 *)0x04804BFA = 0x1D46;
        bss->unk_8a = 0x108;
        WL_REG(0x0AE) = 13;
        break;
    case 3:
        txCtrl->txq[0].frame = (u8 *)0x04804170;
        txCtrl->txq[1].frame = (u8 *)0x04804028;
        txCtrl->txq[2].frame = (u8 *)0x04804000;
        txCtrl->txq[0].callback = TxqEndData;
        txCtrl->txq[1].callback = TxqEndManCtrl;
        txCtrl->txq[2].callback = TxqEndPsPoll;
        *(vu16 *)0x04804024 = 0xB6B8;
        *(vu16 *)0x04804026 = 0x1D46;
        *(vu16 *)0x0480416C = 0xB6B8;
        *(vu16 *)0x0480416E = 0x1D46;
        *(vu16 *)0x04804790 = 0xB6B8;
        *(vu16 *)0x04804792 = 0x1D46;
        bss->unk_8a = 0x108;
        WL_REG(0x0AE) = 13;
        break;
    }
    if (param->wepMode) {
        bss->unk_8a |= 0x4000;
    }
}
