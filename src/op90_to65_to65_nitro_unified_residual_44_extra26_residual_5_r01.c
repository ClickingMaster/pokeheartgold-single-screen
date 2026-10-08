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
u32 CTRDGi_WriteFlashSectorCoreMX(CTRDGTaskInfo*);
extern void CTRDGi_SetFlashBankMx(u16);
extern u16 CTRDGi_EraseFlashSectorMX(u16);
extern u16 CTRDGi_ProgramFlashByteMX(u8*,u8*);
extern u16 ctrdg_flash_remainder;
typedef char CheckFlash[(sizeof(CTRDGFlashType)==28)?1:-1];
u32 CTRDGi_EraseFlashSectorCoreLE(CTRDGTaskInfo*);
#define WAIT V16(0x04000204)
// @fn CTRDGi_WriteFlashSectorCoreMX
u32 CTRDGi_WriteFlashSectorCoreMX(CTRDGTaskInfo *input){
 CTRDGTaskInfo task;
 u16 sector;
 MICartridgeRamCycle old;
 u16 result;
 u8 *src;
 u8 *dst;
 task=*input;
 sector=task.sec_num;
 src=task.data;
 if(sector>=AgbFlash->sector.count)return 0x80ff;
 result=CTRDGi_EraseFlashSectorMX(sector);
 if(result)return result;
 OS_LockCartridge(ctrdgi_flash_lock_id);
 old=MI_GetCartridgeRamCycle();
 MI_SetCartridgeRamCycle(AgbFlash->agbWait[0]);
 CTRDGi_SetFlashBankMx((u16)(sector>>4));
 sector&=0xf;
 ctrdg_flash_remainder=(u16)AgbFlash->sector.size;
 dst=(u8*)(0x0a000000+(sector<<AgbFlash->sector.shift));
 ctrdgi_backup_irq=OS_DisableIrq();
 while(ctrdg_flash_remainder){
  result=CTRDGi_ProgramFlashByteMX(src,dst);
  if(result)break;
  src++;
  dst++;
  ctrdg_flash_remainder--;
 }
 (void)OS_RestoreIrq((BOOL)ctrdgi_backup_irq);
 MI_SetCartridgeRamCycle(old);
 OS_UnlockCartridge(ctrdgi_flash_lock_id);
 return result;
}
