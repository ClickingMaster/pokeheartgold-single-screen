#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int words[6];int score;} Item;
typedef struct Node{struct Node*prev,*next;Item*item;}Node;
u8 ov96_021F3B38(u32,Item**,Item**);
u8 ov96_021F3B38(u32 count,Item**input,Item**output){u8 tied;Node nodes[26];u8 i,j;nodes[0].prev=&nodes[1];nodes[0].next=&nodes[1];tied=0;nodes[0].item=0;for(i=0;i<count;i++){Node*node=&nodes[i+1];Node*pos;node->item=input[i];node->next=0;node->prev=0;pos=&nodes[0];for(j=0;j<i;j++){pos=pos->next;if(pos->item->score<node->item->score){pos->prev->next=node;node->prev=pos->prev;node->next=pos;pos->prev=node;break;}}if(j==i){pos->next=node;node->prev=pos;}}{Node*pos=nodes[0].next;int best=pos->item->score;pos=&nodes[0];for(i=0;i<count;i++){pos=pos->next;output[i]=pos->item;if(best==output[i]->score)tied++;}}if(tied==0)GF_AssertFail();return tied;}
