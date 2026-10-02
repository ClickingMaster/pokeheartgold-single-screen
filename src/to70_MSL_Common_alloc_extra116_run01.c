#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
void*__sys_alloc(u32);void __sys_free(void*);void*malloc(u32);void free(void*);
void*malloc(u32 n){if(!n)return 0;return __sys_alloc(n);}
void free(void*p){if(p)__sys_free(p);}
