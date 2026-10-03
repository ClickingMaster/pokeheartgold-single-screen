#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Node {struct Node *left,*right;u32 parent;u32 data;} Node;
typedef struct {int count;Node *root,*last;} Tree;
extern void ov45_02236E58(Node **);extern Node *ov45_022370D0(Node *,Node *);extern void ov00_021EC2EC(int,void *,int);
extern Node *ov45_02242394(u32);
typedef struct {Node *first,*second;Node **root;Node *volatile cleanup;} Guard;
void ov45_02243CF8(Tree *,Node *,Node **,int);
void ov45_02243B50(Node **,Tree *,Node *);
void ov45_02243B50(Node **out,Tree *tree,Node *node){Node *old=node;if(node==tree->last){ov45_02236E58(&node);tree->last=node;}*out=ov45_022370D0(old,tree->root);if(old)ov00_021EC2EC(0,old,0);tree->count--;}
