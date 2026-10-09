#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
extern "C" int printf(const char *,...);
extern "C" void abort(void);
extern "C" char ov45_02255104[];
extern "C" void *ov45_02242394(u32);
extern "C" void ov00_021EC2EC(int,void *,int);
inline void *operator new(std::size_t,void *p){return p;}
class Alloc {};
struct Pair8 {u64 v;};
struct Inner {u64 x;u64 y;u32 p;u32 q;};

template<class A,class B> struct Pair {A first;B second;Pair(const A &a,const B &b):first(a){B *dst=&second;*dst=b;}};

typedef Pair<u32,Inner> Value;
struct Node {Node *left,*right,*parent;Value val;};
struct Tree {u32 count;Node *root;u32 unk8;Node *leftmost;};
class Holder {
public:
 Node *a;Node *b;Node **alloc;Node *volatile ptr;
 Holder(Node *n,Node **al):a(n),b(n),alloc(al),ptr(n){}
 void Destroy(){Node *p=ptr;if(p)ov00_021EC2EC(0,p,0);}
 void Release(){ptr=0;}
};
inline Node *Allocate(Alloc){return (Node *)ov45_02242394(sizeof(Node));}
extern "C" void ov45_02236EC4(Node *,Node *);
extern "C" Node *ov45_02236CE0(Tree *,Node *,int,int,Value *);
#pragma force_active on
// @fn ov45_02236CE0
extern "C" Node *ov45_02236CE0(Tree *t,Node *parent,int left,int leftmost,Value *v){
 if(t->count==(u32)-1){printf(ov45_02255104);abort();}
 Node *n=Allocate(Alloc());
 Holder h(n,&t->root);
 new(&n->val) Value(v->first,v->second);
 n->right=0;
 h.Release();
 n->left=0;
 new(&n->parent) Node*(parent);
 if(left)parent->left=n;else parent->right=n;
 t->count++;
 ov45_02236EC4(n,t->root);
 if(leftmost)t->leftmost=n;
 h.Destroy();
 return n;
}
