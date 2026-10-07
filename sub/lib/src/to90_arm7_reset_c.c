#include <nitro.h>

typedef struct { u16 initialized; vu16 occurred; } ResetFlags;
extern ResetFlags OSi_ResetFlags;
void OSi_SendToPxi(u16 data);
void OSi_DoBoot(void);
void OSi_DoResetSystem(void);

#pragma force_active on
void OS_InitReset(void) {
    if (OSi_ResetFlags.initialized) {
        return;
    }
    OSi_ResetFlags.initialized = TRUE;
#ifdef SDK_ARM9
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_OS, PXI_PROC_ARM7)) {}
#endif //SDK_ARM9
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_OS, OSi_CommonCallback);
}

BOOL OS_IsResetOccurred(void) {
    return OSi_ResetFlags.occurred;
}

void OSi_CommonCallback(PXIFifoTag tag, u32 data, BOOL err) {
#pragma unused(tag)
#pragma unused(err)
    u16 command;
    u16 commandArg;

    command = (u16)((data & OS_PXI_COMMAND_MASK) >> OS_PXI_COMMAND_SHIFT);
    commandArg = (u16)((data & OS_PXI_DATA_MASK) >> OS_PXI_DATA_SHIFT);
    if (command == OS_PXI_COMMAND_RESET) {
        OSi_ResetFlags.occurred = TRUE;
    } else {
        OS_Panic("");
    }
}

#pragma force_active off
void OSi_SendToPxi(u16 data) {
    u32 pxi_send_data;

    pxi_send_data = (u32)(data) << OS_PXI_COMMAND_SHIFT;
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_OS, pxi_send_data, FALSE) != PXI_FIFO_SUCCESS) {}
}

#pragma force_active on
void OS_ResetSystem(void) {
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    OS_SetIrqMask(OS_IE_FIFO_RECV);
    OS_ResetRequestIrqMask(0xFFFFFFFF);
    SND_Shutdown();
    OSi_SendToPxi(OS_PXI_COMMAND_RESET);
    OSi_DoResetSystem();
}

#pragma force_active off
void OSi_DoResetSystem(void) {
    reg_OS_IME = 0;
    OSi_DoBoot();
}
