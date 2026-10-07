#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 words[3];const u8 *source;u32 tail[4];u16 sector,pad;} FlashTask;
typedef struct {u32 a,size;u16 shift,count;u32 other,wait;} FlashInfo;
extern FlashInfo *AgbFlash;
extern u16 ctrdgi_flash_lock_id;
extern u32 ctrdgi_backup_irq;
extern volatile u16 ctrdg_flash_remainder;
extern u16 (*CTRDGi_PollingSR)(int,volatile u8 *,int);
extern u16 CTRDGi_EraseFlashSectorLE(u16);
extern u16 CTRDGi_ProgramFlashByteLE(const u8 *,u8 *);
#define WAIT (*(volatile u16 *)0x04000204)
#define IRQ (*(volatile u16 *)0x04000208)
u16 CTRDGi_WriteFlashSectorCoreMX5(const FlashTask *);
#pragma opt_propagation off
u16 CTRDGi_WriteFlashSectorCoreMX5(const FlashTask *input) {
 FlashTask task=*input;u16 sector=task.sector;u32 old;u16 result;const u8 *source=task.source;u8 *dest;
 if(sector>=AgbFlash->count)return 0x80ff;
 result=CTRDGi_EraseFlashSectorLE(sector);if(result)return result;
 OS_LockCartridge(ctrdgi_flash_lock_id);old=WAIT;{u32 wait=AgbFlash->wait;WAIT=(WAIT&~3)|wait;}
 old &=3;ctrdg_flash_remainder=AgbFlash->size;dest=(u8 *)(0xa000000+(sector<<AgbFlash->shift));
 while(ctrdg_flash_remainder) {result=CTRDGi_ProgramFlashByteLE(source,dest);if(result)break;ctrdg_flash_remainder--;source++;dest++;}
 WAIT=(WAIT&~3)|old;OS_UnlockCartridge(ctrdgi_flash_lock_id);return result;
}
