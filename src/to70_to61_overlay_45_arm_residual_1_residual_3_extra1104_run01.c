#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Node {struct Node *left,*right;u32 parent;} Node;
static inline Node *Parent(Node *n){return (Node*)(n->parent&~1);}
static inline void Set(Node **p,Node *n){*p=n;}
void ov45_02236E58(Node **);
extern void ov45_0223753C(Node *,Node **),ov45_022375B8(Node *,Node **);
static inline int Red(Node *n){return (n->parent&1)?1:0;}
static inline void Black(Node*n){n->parent &=~1;}
static inline void Paint(Node*n){n->parent |=1;}
void ov45_02236EC4(Node *,Node *,...);
void ov45_02236EC4(Node *n,Node *volatile root,...){Paint(n);while(n!=root&&Red(Parent(n))==1){Node*p=Parent(n),*g=Parent(p);if(p==g->left){Node*u=g->right;if(u&&Red(u)==1){Black(p);Black(u);n=Parent(Parent(n));Paint(n);}else{if(n==p->right){n=p;ov45_0223753C(n,(Node **)&root);}Black(Parent(n));Paint(Parent(Parent(n)));ov45_022375B8(Parent(Parent(n)),(Node **)&root);}}else{Node*u=g->left;if(u&&Red(u)==1){Black(p);Black(u);n=Parent(Parent(n));Paint(n);}else{if(n==p->left){n=p;ov45_022375B8(n,(Node **)&root);}Black(Parent(n));Paint(Parent(Parent(n)));ov45_0223753C(Parent(Parent(n)),(Node **)&root);}}}Black(root);}
