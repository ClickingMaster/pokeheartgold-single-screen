#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern OSMutex __cs[];
extern u32 __cs_id[],__cs_ref[];
static inline void lock(int index) {
 if(!OS_TryLockMutex(&__cs[index])) {__cs_id[index]=OS_GetCurrentThread()->id;__cs_ref[index]=1;}
 else if(__cs_id[index]==OS_GetCurrentThread()->id) __cs_ref[index]++;
 else {OS_LockMutex(&__cs[index]);__cs_id[index]=OS_GetCurrentThread()->id;__cs_ref[index]=1;}
}
static inline void unlock(int index) {if(--__cs_ref[index]==0)OS_UnlockMutex(&__cs[index]);}
typedef struct {void(*fn)(void);void(*other)(void);volatile int count;} Console;
extern Console __console_exit;
extern void (* volatile __atexit_funcs[])(void);
extern int fflush(void*);
extern void _ExitProcess(void);
void __exit(void);
void __exit(void) {
 lock(0);
 while(__console_exit.count>0) {int index=__console_exit.count-1;void(*fn)(void)=__atexit_funcs[index];__console_exit.count=index;fn();}
 unlock(0);
 if(__console_exit.fn) {__console_exit.fn();__console_exit.fn=0;}
 fflush(0);_ExitProcess();
}
