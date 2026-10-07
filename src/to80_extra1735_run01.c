#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Node{void *left,*right;u32 parent,value;}Node;typedef struct{Node*unused;void*root;}Work;typedef struct{u32 count;void*root;void*last;}Tree;
extern Node*ov45_02242394(u32);extern void ov45_02243CF8(Tree*,Node*,Node*,int),ov45_02244350(void*,Tree*),ov45_02244330(Tree*),ov00_021EC2EC(int,void*,int);
void ov45_02243BC0(u8*,Work*);
void ov45_02243BC0(u8*owner,Work*w){struct{u8 pad0;volatile u8 byte;u8 pad[2];Node*volatile a,*volatile b;void*volatile owner;Node*volatile cleanup;Tree tree;}f;Node*n;u32 target;u32 flag;if(!w->root)return;f.byte=0;n=ov45_02242394(16);f.owner=owner+4;f.a=n;f.b=n;f.cleanup=n;target=(u32)n+12;{Node*root=w->root;if(target)*(u32*)target=root->value;};f.tree.count=0;f.tree.root=0;f.tree.last=(Node*)&f.tree.root;n=f.cleanup;n->right=0;n->left=0;n->parent=(u32)&f.tree.root|(n->parent&1);flag=(((Node*)w->root)->parent&1)!=0;n=f.cleanup;if(flag)n->parent|=1;else n->parent&=~1;n=f.cleanup;f.tree.root=n;f.tree.count=1;f.tree.last=n;{Node*root=w->root;f.cleanup=0;if(root->right)ov45_02243CF8(&f.tree,n,root,0);}if(((Node*)w->root)->left)ov45_02243CF8(&f.tree,f.tree.root,w->root,1);ov45_02244350(owner,&f.tree);ov45_02244330(&f.tree);{Node*cleanup=f.cleanup;if(cleanup)ov00_021EC2EC(0,cleanup,0);};}
