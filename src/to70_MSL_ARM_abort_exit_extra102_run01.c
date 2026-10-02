#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
struct ExitState {void (*console)(void);void (*stdio)(void);int count,aborting;};
extern struct ExitState __console_exit;
extern int raise(int);
extern void __destroy_global_chain(void);
void abort(void);
void exit(int);
void __exit(int);
extern int OS_TryLockMutex(void *);
extern void OS_LockMutex(void *);
extern void OS_UnlockMutex(void *);
extern u8 __cs[];
extern u32 __cs_id[],__cs_ref[];
extern void *OSi_ThreadInfo[];
extern void (*__atexit_funcs[])(void);
extern int fflush(void *);
extern void _ExitProcess(void);
static inline u32 ThreadID(void) {return *(u32 *)((u8 *)OSi_ThreadInfo[1]+0x6c);}
void abort(void) {raise(1);__console_exit.aborting=1;exit(1);}
void exit(int status) {if(!__console_exit.aborting) {__destroy_global_chain();if(__console_exit.stdio){__console_exit.stdio();__console_exit.stdio=0;}}__exit(status);}
