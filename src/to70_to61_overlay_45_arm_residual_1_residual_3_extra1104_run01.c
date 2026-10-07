#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "../lib/NitroDWC/src/tree-iterator4.h"
// clang-format on
#pragma force_active on
typedef struct Node {void *left,*right;u32 parent;} Node;
static inline Node *Parent(Node *n){return (Node*)(n->parent&~1);}
static inline void Set(Node **p,Node *n){*p=n;}
void ov45_02236E58(Iterator*);
extern void ov45_0223753C(Node *,void **),ov45_022375B8(Node *,void **);
static inline int Red(Node *n){return (n->parent&1)?1:0;}
static inline void Black(Node*n){n->parent &=~1;}
static inline void Paint(Node*n){n->parent |=1;}
void ov45_02236EC4(Node *,void *);
void ov45_02236EC4(Node *n,void *volatile root){Paint(n);while(n!=root&&Red(Parent(n))==1){Node*p=Parent(n),*g=Parent(p);if(p==g->left){Node*u=g->right;if(u&&Red(u)==1){Black(p);Black(u);n=Parent(Parent(n));Paint(n);}else{if(n==p->right){n=p;ov45_0223753C(n,(void **)&root);}Black(Parent(n));Paint(Parent(Parent(n)));ov45_022375B8(Parent(Parent(n)),(void **)&root);}}else{Node*u=g->left;if(u&&Red(u)==1){Black(p);Black(u);n=Parent(Parent(n));Paint(n);}else{if(n==p->left){n=p;ov45_022375B8(n,(void **)&root);}Black(Parent(n));Paint(Parent(Parent(n)));ov45_0223753C(Parent(Parent(n)),(void **)&root);}}}Black(root);}
