#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int PXI_SendWordByFifo(u32, u32, int);
extern void VENEER_SVC_WaitByLoop(u32);
void CTRDGi_SendtoPxi(u32 data);

void CTRDGi_SendtoPxi(u32 data) {
    while (PXI_SendWordByFifo(13, data, 0)) {
        VENEER_SVC_WaitByLoop(1);
    }
}
