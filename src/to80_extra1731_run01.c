#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Node {struct Node *left,*right;u32 tagged;}Node;
extern void ov45_02236E58(Node**),ov45_0223753C(Node*,Node**),ov45_022375B8(Node*,Node**);
Node*ov45_022370D0(Node*,Node*,...);
static inline Node*parent(Node*n){return(Node*)(n->tagged&~1);}
static inline int red(Node*n){if(*(volatile u32*)&n->tagged&1)return 1;return 0;}
static inline void setParent(Node*n,Node*p){n->tagged=(u32)p|(n->tagged&1);}
static inline void setRed(Node*n,int r){u32 t=n->tagged;if(r)t|=1;else t&=~1;n->tagged=t;}
static inline int redLoaded(Node*n,u32*tag){u32 t=*(volatile u32*)&n->tagged;*tag=t;if(t&1)return 1;return 0;}
Node*ov45_022370D0(Node*node,Node*root,...){Node*iter=node,*removed,*child,*up;int left,fix;ov45_02236E58(&iter);removed=node;if(node->left&&node->right)removed=iter;child=removed->left;if(!child)child=removed->right;up=parent(removed);if(child)setParent(child,up);{Node*p=parent(removed);if(removed==p->left){p->left=child;left=1;}else{p->right=child;left=0;}}fix=!red(removed);
 if(removed!=node){Node*p=parent(node);setParent(removed,p);p=parent(removed);if(node==p->left)p->left=removed;else p->right=removed;{Node*c=node->left;removed->left=c;if(!c)up=removed;else setParent(c,removed);}{Node*c=node->right;removed->right=c;if(!c)up=removed;else setParent(c,removed);}setRed(removed,red(node));if(root==node)root=removed;}
 else if(!child&&node==root)fix=0;
 if(fix){while(child!=root&&(!child||!red(child))){Node*sibling;u32 colorword;if(left){sibling=up->right;if(redLoaded(sibling,&colorword)==1){sibling->tagged=colorword&~1;setRed(up,1);ov45_0223753C(up,&root);sibling=up->right;}
 if((!sibling->left||!red(sibling->left))&&(!sibling->right||!red(sibling->right))){setRed(sibling,1);child=up;up=parent(up);left=(child==up->left);}
 else{if(!sibling->right||!red(sibling->right)){setRed(sibling->left,0);setRed(sibling,1);ov45_022375B8(sibling,&root);sibling=up->right;}setRed(sibling,red(up));setRed(up,0);setRed(sibling->right,0);ov45_0223753C(up,&root);child=root;}
 }else{sibling=up->left;if(redLoaded(sibling,&colorword)==1){sibling->tagged=colorword&~1;setRed(up,1);ov45_022375B8(up,&root);sibling=up->left;}
 if((!sibling->left||!red(sibling->left))&&(!sibling->right||!red(sibling->right))){setRed(sibling,1);child=up;up=parent(up);left=(child==up->left);}
 else{if(!sibling->left||!red(sibling->left)){setRed(sibling->right,0);setRed(sibling,1);ov45_0223753C(sibling,&root);sibling=up->left;}setRed(sibling,red(up));setRed(up,0);setRed(sibling->left,0);ov45_022375B8(up,&root);child=root;}
 }}if(child)setRed(child,0);}
 return iter;
}
