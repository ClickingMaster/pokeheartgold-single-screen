#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 *words;int top,capacity,sign,flags;} Big;
extern Big *bn_expand2(Big*,int);
extern void *CRYPTOi_MyAlloc(u32);
extern void CRYPTOi_MyFree(void*);
Big *bn_expand2(Big*b,int n){u32*p;if(n>b->capacity){if(b->flags&2)return 0;p=CRYPTOi_MyAlloc((n+1)*4);if(!p)return 0;if(b->words){MI_CpuCopy8(b->words,p,b->top*4);CRYPTOi_MyFree(b->words);}b->words=p;b->capacity=n;}return b;}
