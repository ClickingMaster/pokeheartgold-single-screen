// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <CTRDG_task.h>
typedef struct CTRDGiFlashSectorTag {u32 size;u16 shift,count,top;u8 reserved[2];} CTRDGiFlashSector;
typedef struct CTRDGFlashTypeTag {u32 romSize;CTRDGiFlashSector sector;MICartridgeRamCycle agbWait[2];u16 makerID,deviceID;} CTRDGFlashType;
extern const CTRDGFlashType *AgbFlash;
extern u16 (*CTRDGi_PollingSR)(u16,u8*,u16);
#define V16(a) (*(volatile u16 *)(a))
#define V8(a) (*(volatile u8 *)(a))
extern u16 ctrdgi_flash_lock_id;
extern u32 ctrdgi_backup_irq;
u32 CTRDGi_EraseFlashChipCoreLE(CTRDGTaskInfo*);
typedef char CheckFlash[(sizeof(CTRDGFlashType)==28)?1:-1];
u32 CTRDGi_EraseFlashSectorCoreLE(CTRDGTaskInfo*);
#define WAIT V16(0x04000204)
// @fn CTRDGi_EraseFlashChipCoreLE
u32 CTRDGi_EraseFlashChipCoreLE(CTRDGTaskInfo *input){
 MICartridgeRamCycle old;
 u16 result;
 OS_LockCartridge(ctrdgi_flash_lock_id);
 old=MI_GetCartridgeRamCycle();
 MI_SetCartridgeRamCycle(AgbFlash->agbWait[0]);
 ctrdgi_backup_irq=OS_DisableIrq();
 V8(0x0a005555)=0xaa;
 V8(0x0a002aaa)=0x55;
 V8(0x0a005555)=0x80;
 V8(0x0a005555)=0xaa;
 V8(0x0a002aaa)=0x55;
 V8(0x0a005555)=0x10;
 (void)OS_RestoreIrq((BOOL)ctrdgi_backup_irq);
 result=CTRDGi_PollingSR(3,(u8*)0x0a000000,0xff);
 MI_SetCartridgeRamCycle(old);
 OS_UnlockCartridge(ctrdgi_flash_lock_id);
 return result;
}
