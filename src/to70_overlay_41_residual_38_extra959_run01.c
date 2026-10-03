#pragma force_active on
#include "global.h"
typedef struct Node {u32 a,b;struct Node *prev,*next;} Node;typedef struct {Node *nodes;int count;u32 flag;} W;extern void *Heap_Alloc(int,u32);extern void ov41_022489E4(Node *,int);void ov41_02248A28(W *,int);
void ov41_02248A28(W *w,int count){int i;w->nodes=Heap_Alloc(14,count*16);w->count=count;w->flag=0;for(i=0;i<w->count;i++){w->nodes[i].prev=&w->nodes[i];w->nodes[i].next=&w->nodes[i];ov41_022489E4(&w->nodes[i],0);}}
