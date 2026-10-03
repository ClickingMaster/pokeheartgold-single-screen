#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Node {u32 pad,index,x;struct Node*next;} Node;typedef struct{u32 pad;Node*head;} List;typedef struct{u8 pad[0x270];List*list;u32 size;} Work;
Work*WCMi_GetSystemWork(void);Node*WcmSearchIndexedApList(u32);
Node*WcmSearchIndexedApList(u32 index){Work*w=(Work*)((u8*)WCMi_GetSystemWork()+0x2000);Node*n=0;List*list=w->list;if(list&&w->size>12){n=list->head;if(!n)return n;do{if(n->index==index)break;n=n->next;}while(n);}return n;}
