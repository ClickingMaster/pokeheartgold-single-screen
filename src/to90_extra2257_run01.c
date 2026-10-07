#pragma force_active on
#include "global.h"
typedef struct Node {u32 a,b;struct Node *next;} Node;
typedef struct {u32 pad;Node one;u32 extra;Node two;} Context;
typedef struct {Context *owner;Node *end;} Iterator;
extern BOOL ov41_02249AA8(Node *,u32,u32,u32);Node *ov41_022481F4(Context *,u32,u32,u32);
#pragma force_active on
Node *ov41_022481F4(Context *p,u32 a,u32 b,u32 c){Node *node=p->two.next;Iterator it={p,(Node*)p};Node *end;it.end=&p->two;if(node!=it.end){do{if(ov41_02249AA8(node,a,b,c)==1)return node;node=node->next;}while(node!=it.end);}node=p->one.next;end=&p->one;while(node!=end){if(ov41_02249AA8(node,a,b,c)==1)return node;node=node->next;}return NULL;}
