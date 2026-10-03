#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 *words;int top,capacity,sign,flags;} Big;
extern Big *bn_expand2(Big*,int);
void bn_zexpand(Big*,int);
void bn_zexpand(Big*b,int n){int i;if(b->top>=n)return;if(n>b->capacity)(void)bn_expand2(b,n);if(!b->words)return;for(i=b->top;i<n;i++)b->words[i]=0;}
