#pragma force_active on
#include "global.h"
typedef struct {u8 pad[16];s16 score;} Item;
typedef struct Node {struct Node *prev,*next;Item *item;} Node;
u8 ov96_022014A4(Item **,Item **);
u8 ov96_022014A4(Item **input,Item **output){Node list[17];u8 count,i,j;list[0].prev=&list[1];list[0].next=&list[1];count=0;list[0].item=NULL;for(i=0;i<16;i++){Node *node=&list[i+1];Node *cursor;node->item=input[i];node->next=NULL;node->prev=NULL;cursor=&list[0];for(j=0;j<i;j++){cursor=cursor->next;if(cursor->item->score<node->item->score){cursor->prev->next=node;node->prev=cursor->prev;node->next=cursor;cursor->prev=node;break;}}if(j==i){cursor->next=node;node->prev=cursor;}}{Node *first=list[0].next;Node *cursor;s16 highest;i=0;highest=first->item->score;cursor=&list[0];for(;i<16;i++){cursor=cursor->next;output[i]=cursor->item;if(highest==output[i]->score)count++;}}GF_ASSERT(count);return count;}
