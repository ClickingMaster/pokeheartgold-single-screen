#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#pragma force_active on
typedef struct Link {u8 pad[16];struct Link *next;u32 pad14;u8 rank;} Link;
void addLink_(Link **,Link *);
void addLink_(Link **head,Link *node){Link *p=*head,*q,*tail;if(!p){*head=node;return;}q=p->next;if(!q){if(p->rank>node->rank){tail=node;while(tail->next)tail=tail->next;tail->next=p;*head=node;}else p->next=node;return;}while(q){if(q->rank>=node->rank){tail=node;while(tail->next)tail=tail->next;p->next=node;tail->next=q;return;}p=q;q=q->next;}p->next=node;}
