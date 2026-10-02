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
extern void (*signal_funcs[])(int);
int raise(int sig){void(*handler)(int);if(sig<1 || sig>7)return -1;if(!OS_TryLockMutex(__cs+168)){__cs_id[7]=ThreadID();__cs_ref[7]=1;}else{if(__cs_id[7]==ThreadID())__cs_ref[7]++;else{OS_LockMutex(__cs+168);__cs_id[7]=ThreadID();__cs_ref[7]=1;}}handler=signal_funcs[sig-1];if(handler!=(void(*)(int))1)signal_funcs[sig-1]=0;if(--__cs_ref[7]==0)OS_UnlockMutex(__cs+168);if(handler==(void(*)(int))1 || (!handler && sig==1))return 0;if(!handler)exit(0);handler(sig);return 0;}
