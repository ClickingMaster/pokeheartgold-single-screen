#pragma force_active on
#include "global.h"
typedef struct{u16 x,y;} Pair;typedef struct{int p;Pair items[8];u16 read,write;} Queue;int ov91_0225EE18(Queue *,Pair *);void ov91_0225EDC8(Queue *,Pair,int,int);
void ov91_0225EDC8(Queue *q,Pair value,int a,int b){Pair old;if((q->write+1)%8==q->read)ov91_0225EE18(q,&old);q->items[q->write]=value;q->write=(q->write+1)%8;}
int ov91_0225EE18(Queue *q,Pair *out){if(q->write==q->read)return 0;*out=q->items[q->read];q->read=(q->read+1)%8;return 1;}
