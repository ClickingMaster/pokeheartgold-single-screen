#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_uninitializedvar off
typedef struct Task Task;
typedef int (*TaskFn)(Task*);
struct Task {TaskFn fn;TaskFn callback;int result;void *data;u32 words[4];u16 sector;u8 busy,pad;};
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define H(p,o) (*(u16 *)((char *)(p)+(o)))
#define V16(a) (*(volatile u16 *)(a))
#define V8(a) (*(volatile u8 *)(a))
extern void *AgbFlash;
extern u16 ctrdgi_flash_lock_id;
extern u32 ctrdgi_backup_irq;
extern int (*CTRDGi_PollingSR)(int,void*,int);
extern char _021E4F68[],_021E4F6C[],_021E4F90[];
extern int CTRDGi_EraseFlashChipCoreLE(Task*);
extern int CTRDGi_EraseFlashSectorCoreLE(Task*);
extern int CTRDGi_WriteFlashSectorCoreLE(Task*);
extern int CTRDGi_ProgramFlashByteLE(u8*,u8*);
extern int CTRDGi_EraseFlashChipCoreMX(Task*);
extern int CTRDGi_EraseFlashSectorCoreMX(Task*);
extern int CTRDGi_WriteFlashSectorCoreMX(Task*);
extern int CTRDGi_ProgramFlashByteMX(u8*,u8*);
extern int CTRDGi_WriteFlashSectorCoreMX5(Task*);
extern int CTRDGi_ProgramFlashByteLE(u8 *from,u8 *to);
extern int CTRDGi_VerifyFlashCoreFF(u8 *p);
extern int CTRDGi_VerifyFlashErase(void *addr,int(*fn)(void*));
extern u16 CTRDGi_EraseFlashChipLE(void);
extern u16 CTRDGi_EraseFlashSectorLE(u16 sector);
extern u16 CTRDGi_WriteFlashSectorLE(u16 sector,void *data);
extern void CTRDGi_EraseFlashChipAsyncLE(TaskFn cb);
extern void CTRDGi_EraseFlashSectorAsyncLE(u16 sector,TaskFn cb);
extern void CTRDGi_WriteFlashSectorAsyncLE(u16 sector,void *data,TaskFn cb);
extern u16 CTRDGi_EraseFlashChipMX(void);
extern u16 CTRDGi_EraseFlashSectorMX(u16 sector);
extern u16 CTRDGi_WriteFlashSectorMX(u16 sector,void *data);
extern void CTRDGi_EraseFlashChipAsyncMX(TaskFn cb);
extern void CTRDGi_EraseFlashSectorAsyncMX(u16 sector,TaskFn cb);
extern void CTRDGi_WriteFlashSectorAsyncMX(u16 sector,void *data,TaskFn cb);
extern int CTRDGi_ProgramFlashByteMX(u8 *from,u8 *to);
extern u16 CTRDGi_WriteFlashSectorMX5(u16 sector,void *data);
extern void CTRDGi_WriteFlashSectorAsyncMX5(u16 sector,void *data,TaskFn cb);
extern int CTRDGi_IsTaskAvailable(void);
extern void CTRDGi_InitTaskInfo(Task *t);
extern void CTRDGi_SetTask(Task *t,TaskFn fn,TaskFn cb);

extern void CTRDGi_TaskThread(void*);
extern int CTRDGi_EraseFlashChipCoreLE(Task *unused);
extern int CTRDGi_EraseFlashChipCoreMX(Task *unused);
extern void CTRDGi_InitTaskThread(void *p);
extern void CTRDGi_TaskThread(void *p);
int CTRDGi_VerifyFlashCoreFF(u8 *p){int n=W(AgbFlash,4);while(n){if(*p++!=255)break;n--;}return n;}
