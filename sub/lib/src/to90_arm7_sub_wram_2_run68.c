#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern int SPIi_CheckException(u32);
extern void SPIi_GetException(u32);
extern void SPIi_ReleaseException(u32);
extern void SPIi_ReturnResult(u16, u32);
extern int SPIi_SetEntry(u32, u32, u16, ...);

typedef struct {
    u32 unused, cmd, a, b;
    void *data;
} W37002Request;

extern void NVRAM_WriteEnable(void);
extern void NVRAM_WriteDisable(void);
extern void NVRAM_ReadStatusRegister(void *);
extern int NvramCheckReadyToRead(void);
extern int NvramCheckReadyToWrite(void);
extern void NVRAM_ReadDataBytes(u32, u32, void *);
extern void NVRAM_ReadDataBytesAtHigherSpeed(u32, u32, void *);
extern void NVRAM_PageWrite(u32, u16, const void *);
extern void NVRAM_PageProgram(u32, u16, const void *);
extern void NVRAM_PageErase(u32);
extern void NVRAM_SectorErase(u32);
extern void NVRAM_DeepPowerDown(void);
extern void NVRAM_ReleaseFromDeepPowerDown(void);
extern void NVRAM_ChipErase(void);
extern void NVRAM_ReadSiliconId(void *);
extern void NVRAM_SoftwareReset(void);
void NVRAM_ExecuteProcess(W37002Request *);

void NVRAM_ExecuteProcess(W37002Request *q) {
    u32 irq = OS_DisableInterrupts();
    if (!SPIi_CheckException(1)) {
        OS_RestoreInterrupts(irq);
        SPIi_ReturnResult((u16)q->cmd, 4);
        return;
    }
    SPIi_GetException(1);
    OS_RestoreInterrupts(irq);
    switch (q->cmd) {
    case 0x20:
        NVRAM_WriteEnable();
        break;
    case 0x21:
        NVRAM_WriteDisable();
        break;
    case 0x22:
        NVRAM_ReadStatusRegister(q->data);
        break;
    case 35:
        if (!NvramCheckReadyToRead()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ReadDataBytes(q->a, q->b, q->data);
        break;
    case 36:
        if (!NvramCheckReadyToRead()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ReadDataBytesAtHigherSpeed(q->a, q->b, q->data);
        break;
    case 37:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageWrite(q->a, (u16)q->b, q->data);
        break;
    case 38:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageProgram(q->a, (u16)q->b, q->data);
        break;
    case 39:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageErase(q->a);
        break;
    case 40:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_SectorErase(q->a);
        break;
    case 0x29:
        NVRAM_DeepPowerDown();
        break;
    case 0x2a:
        NVRAM_ReleaseFromDeepPowerDown();
        break;
    case 0x2b:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)q->cmd, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ChipErase();
        break;
    case 0x2c:
        NVRAM_ReadSiliconId(q->data);
        break;
    case 0x2d:
        NVRAM_SoftwareReset();
        break;
    default:
        SPIi_ReleaseException(1);
        SPIi_ReturnResult((u16)q->cmd, 1);
        return;
    }
    SPIi_ReturnResult((u16)q->cmd, 0);
    SPIi_ReleaseException(1);
}
