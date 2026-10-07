#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Node {void *left,*right;u32 parent;u32 value;} Node;
typedef struct {int count;void *head,*tail;} Container;
typedef struct {u8 active;Node *allocated,*value;void **owner;Node *pending;} Guard;
extern int printf(const char*,...);extern void abort(void);
extern const char ov45_0225533C[];
extern void *ov45_02242394(u32);
extern void ov45_02236EC4(Node*,void *),ov00_021EC2EC(int,void*,int);
Node *ov45_0224210C(Container*,void*,int,int,const u32*);
Node *ov45_0224210C(Container*c,void*owner,int first,int last,const u32*value){Node *node,*pending;u32 *dest;u32 *parent;volatile Guard guard;
 if(c->count==-1){printf(ov45_0225533C);abort();}
 guard.active=0;
 node=ov45_02242394(16);
 guard.owner=&c->head;
 dest=(u32*)((u8*)node+12);
 guard.pending=node;guard.allocated=node;
 if(dest)*dest=*value;
 node->right=NULL;guard.pending=NULL;node->left=NULL;
 parent=(u32*)((u8*)node+8);if(parent)*parent=(u32)owner;
 if(first)*(void**)owner=node;else*(void**)((u8*)owner+4)=node;
 c->count++;
 guard.value=node;
 ov45_02236EC4(node,c->head);
 if(last)c->tail=node;
 pending=guard.pending;if(pending)ov00_021EC2EC(0,pending,0);
 return node;
}
