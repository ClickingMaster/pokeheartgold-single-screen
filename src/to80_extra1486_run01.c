#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 *words;int top,capacity,sign,flags;} Big;
extern Big *bn_expand2(Big*,int);
Big *BN_copy(Big*,const Big*);
Big *BN_copy(Big*dst,const Big*src){if(dst==src)return dst;if(!(src->top>dst->capacity?bn_expand2(dst,src->top):dst))return 0;MI_CpuCopy8(src->words,dst->words,src->top*4);dst->top=src->top;if(dst->top==0&&dst->words)dst->words[0]=0;dst->sign=src->sign;return dst;}
