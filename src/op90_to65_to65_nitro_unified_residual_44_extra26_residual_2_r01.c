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
u32 CTRDGi_WriteFlashSectorCoreLE(CTRDGTaskInfo*);
extern u16 CTRDGi_EraseFlashSectorLE(u16);
extern u16 CTRDGi_ProgramFlashByteLE(u8*,u8*);
extern u32 CTRDGi_VerifyFlashCoreFF(u8*);
extern u32 CTRDGi_VerifyFlashErase(u8*,u32(*)(u8*));
extern u16 ctrdg_flash_remainder;
typedef char CheckFlash[(sizeof(CTRDGFlashType)==28)?1:-1];
u32 CTRDGi_EraseFlashSectorCoreLE(CTRDGTaskInfo*);
#define WAIT V16(0x04000204)
// @fn CTRDGi_WriteFlashSectorCoreLE
u32 CTRDGi_WriteFlashSectorCoreLE(CTRDGTaskInfo *input){
 CTRDGTaskInfo task;
 u8 *dst;
 u16 sector;
 MICartridgeRamCycle old;
 u16 result;
 u8 *src;
 u16 retry;
 u16 n;
 task=*input;
 sector=task.sec_num;
 src=task.data;
 if(sector>=16)return 0x80ff;
 dst=(u8*)(0x0a000000+(sector<<AgbFlash->sector.shift));
 retry=0;
 for(;;){
  result=CTRDGi_EraseFlashSectorLE(sector);
  if(result==0){
   result=(u16)CTRDGi_VerifyFlashErase(dst,CTRDGi_VerifyFlashCoreFF);
   if(result==0)break;
  }
  if(retry++==80)return result;
 }
 n=1;
 if(retry!=0)n=6;
 for(retry=1;retry<=n;retry++)CTRDGi_EraseFlashSectorLE(sector);
 OS_LockCartridge(ctrdgi_flash_lock_id);
 old=MI_GetCartridgeRamCycle();
 MI_SetCartridgeRamCycle(AgbFlash->agbWait[0]);
 ctrdg_flash_remainder=(u16)AgbFlash->sector.size;
 ctrdgi_backup_irq=OS_DisableIrq();
 while(ctrdg_flash_remainder){
  result=CTRDGi_ProgramFlashByteLE(src,dst);
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
