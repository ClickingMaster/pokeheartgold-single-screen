#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
void*OS_AllocFromHeap(int,int,u32);void OS_FreeToHeap(int,int,void*);void*__sys_alloc(u32);void __sys_free(void*);
void*__sys_alloc(u32 n){return OS_AllocFromHeap(0,-1,n);}
void __sys_free(void*p){OS_FreeToHeap(0,-1,p);}
