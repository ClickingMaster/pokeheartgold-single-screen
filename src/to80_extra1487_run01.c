#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 *words;int top,capacity,sign,flags;} Big;
extern Big *bn_expand2(Big*,int);
void bn_fix_top(Big*);
void bn_fix_top(Big*b){u32*p;if(b->top<=0)return;p=b->words+(b->top-1);while(b->top>0){if(*p--!=0)return;b->top--;}}
