#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(int*)((p)+(o)))
#define P(p,o) (*(void**)((p)+(o)))
typedef struct Node {int state,flags,socket,key,deadline;u8 pad[0x24];void*array;struct Node*next;} Node;
typedef struct Item {int key;u32 v4;u8*owned8;u8*ownedC;void*owned10;u32 v14;void*owned18;} Item;
typedef int(*CB)(u8**,Item*,void*);
typedef struct Key {char*a,*b;Item**out;int matched;} Key;
typedef struct Iter {u8**ctx;CB fn;void*data;} Iter;
typedef struct Search {int key;Item*out;} Search;
extern int sprintf(char*,const char*,...);
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
extern void*ov00_021EC3A8(int);
extern void*ov00_021FE5B4(int,int,void(*)(void**));
extern void*ov00_021FED24(int,int,int(*)(int*,int),int(*)(int*,int*),void(*)(Item*));
extern Item*ov00_021FF0C0(void*,Item*);
extern char ov00_022191F4[],ov00_022190A8[],ov00_02218F84[],ov00_02219200[],ov00_02219118[],ov00_02218FA0[],ov00_02219214[],ov00_02219218[],ov00_02219328[];
extern int ov00_021EC3D8();
extern int ov00_021FF01C();
extern int ov00_021FF264();
extern int ov00_021FF44C();
extern int ov00_021FF694();
extern int ov00_021FF818();
extern int ov00_021FF900();
extern int ov00_021FFA70();
extern int ov00_02200ED8();
extern int ov00_02202244();
extern int ov00_02202324();
extern int ov00_02202424();
extern int ov00_02205E50();
extern int ov00_02206164();
extern int ov00_0220657C();
extern int ov00_02207158();
extern int ov00_02207254();
extern int ov00_02207370();
extern int ov00_02207FF0();
extern int ov00_0220A890();
int ov00_022073E8(u8**ctx);
Node* ov00_022074D8(u8**ctx,int x);
void ov00_02207510(void**p);
Node* ov00_0220752C(u8**ctx,int key,int flags);
int ov00_022075AC(u8**ctx,Node*n);
int ov00_0220792C(u8**ctx,Node*n,int z,u32*p);
int ov00_022079A8(u8**ctx,Node*n,char*data,int len);
int ov00_02207A94(int*p,int d);
int ov00_02207AA8(int*a,int*b);
void ov00_02207AB8(Item*p);
int ov00_02207B34(u8**ctx);
int ov00_02207DD0(u8**ctx,int x,Item**dest);
void ov00_02207E0C(u8**ctx,int x);
void ov00_02207E40(u8**ctx,Item*x);
int ov00_02207E54(u8**ctx,Item*n,Key*k);
int ov00_02207EB4(u8**ctx,char*a,char*b,Item**out);
int ov00_02207F00(Item*p,Iter*k);
int ov00_02207F1C(u8**ctx,CB fn,void*data);
int ov00_02207F5C(u8**ctx,Item*p,Search*k);
Item* ov00_02207F88(u8**ctx,int x);
int ov00_02207FB8(Item*p);
int ov00_022081F0(u8**ctx,u8**out,int x);
int ov00_022082A0(u8**ctx,int x,int y,int z,int a);
int ov00_02207DD0(u8**ctx,int x,Item**dest) {u8*root=*ctx;Item key,*out;key.key=x;out=ov00_021FF0C0(P(root,0x428),&key);if(dest)*dest=out;return out!=0;}

void ov00_02207E0C(u8**ctx,int x) {u8*root=*ctx;Item*out;if(!ov00_02207DD0(ctx,x,&out))return;ov00_021FF01C(P(root,0x428),out);}

void ov00_02207E40(u8**ctx,Item*x) {ov00_021FF01C(P(*ctx,0x428),x);}

int ov00_02207E54(u8**ctx,Item*n,Key*k) {u8*p=n->ownedC;if(p&&strcmp(k->a,*(char**)p)==0&&strcmp(k->b,*(char**)(p+8))==0){*k->out=n;k->matched=1;return 0;}return 1;}

int ov00_02207EB4(u8**ctx,char*a,char*b,Item**out) {Key k;k.a=a;k.b=b;k.out=out;k.matched=0;ov00_02207F1C(ctx,(CB)ov00_02207E54,&k);if(!k.matched)*out=0;return 0;}

int ov00_02207F00(Item*p,Iter*k) {return k->fn(k->ctx,p,k->data);}

int ov00_02207F1C(u8**ctx,CB fn,void*data) {u8*root=*ctx;Iter k;k.fn=fn;k.data=data;k.ctx=ctx;return ov00_021FF264(P(root,0x428),ov00_02207F00,&k)==0;}

int ov00_02207F5C(u8**ctx,Item*p,Search*k) {if(p->owned8&&k->key==*(int*)p->owned8){k->out=p;return 0;}return 1;}

Item* ov00_02207F88(u8**ctx,int x) {Search k;k.key=x;k.out=0;ov00_02207F1C(ctx,(CB)ov00_02207F5C,&k);return k.out;}

int ov00_02207FB8(Item*p) {if(p&&!p->ownedC&&!p->owned8&&!p->owned18&&!p->owned10)return 1;return 0;}
