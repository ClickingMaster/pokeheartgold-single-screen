#pragma force_active on
#include "global.h"
typedef struct{u32 pad[2];void*next;}Node;
typedef struct{u8 pad[0x10];volatile int count;int other[2];}List;
extern int ov41_0224895C(void*,int);extern Node*ov41_02248ABC(void*,int,int);extern int ov41_02248750(void*,int,int);
int ov41_02248790(void*,int,int);
int ov41_02248790(void*p,int group,int reverse){int start=ov41_0224895C(p,group);List*l=(List*)((u8*)p+group*12);int i;for(i=1;i<l->count;i++){int n;if(reverse==0)n=(i+start)%l->count;else{n=start-i;if(n<0)n+=l->count;} {Node*node=ov41_02248ABC(p,group,n);if(node->next!=node)return ov41_02248750(p,group,n);}}return 0;}
