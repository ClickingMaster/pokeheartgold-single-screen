#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" int strcmp(const char *,const char *);
extern "C" int memcmp(const void *,const void *,unsigned long);
struct TA {};struct TB {};struct TD {};struct TE {};
struct TU {};
struct Iter {const u8 *p;u32 extra;Iter(const u8 *q):p(q){}};
struct Vec;
extern "C" void ov45_022327B8(Vec *,Iter,Iter,TD);
extern "C" void ov45_022383E0(Vec *,const u8 *,const u8 *,TU);
extern "C" void ov45_02232760(Vec *,const u8 *,const u8 *,TE);
extern "C" const u8 *ov45_02231C78(const Vec *);
extern "C" void ov45_022379D4(const Vec *);
struct Vec {
 u8 *ptr;u32 size,cap;
 void Init(){ptr=0;size=0;cap=0;}
 void Assign(const u8 *first,const u8 *last){Assign(Iter(first),Iter(last));}
 void Assign(Iter first,Iter last){Assign(first,last,TA(),TB());}
 void Assign(Iter first,Iter last,const TA&,const TB&){ov45_022327B8(this,first,last,TD());}
 void InitCopy(const Vec &o,TU al){ptr=0;size=0;cap=0;ov45_022383E0(this,o.ptr,o.ptr+o.size,al);}
 void AssignVec(const Vec &o){if(this!=&o)ov45_02232760(this,o.ptr,o.ptr+o.size,TE());}
};
inline bool operator==(const Vec &a,const Vec &b){
 return a.size==b.size&&(a.size==0||memcmp(ov45_02231C78(&a),ov45_02231C78(&b),a.size)==0);
}
struct Quad {u32 a;u64 b;u32 c;};
struct Triple {s64 b;u32 a;};
struct VecBox {Vec v;int changed;
 void Init(const Vec &x,TU al){v.InitCopy(x,al);changed=1;}
 void InitM(const Vec &x,TU al){v.InitCopy(x,al);Mark();}
 void Mark(){if(this)changed=1;}
 void Update(const VecBox &o){if(!(v==o.v)){v.AssignVec(o.v);changed=1;}}};
inline bool operator==(const Quad &a,const Quad &b){return memcmp(&a,&b,16)==0;}
struct QuadBox {Quad q;int changed;
 void Init(const Quad &x){q=x;changed=1;}
 void Update(const QuadBox &o){if(!(q==o.q)){q=o.q;changed=1;}}};
struct Rec {
 u32 x0;QuadBox q;VecBox vec;u32 x28,x2c;int qRecv;int vecRecv;int valid;
 void SetVec(const u8 *d,u32 n,TU al);
 void SetQuad(const u8 *d);
 Triple GetTriple(){Triple t;t.a=q.q.a;t.b=q.q.b;return t;}
};
struct Val {u64 v;Val(const u8 *d){((u32 *)&v)[0]=((const u32 *)d)[0];((u32 *)&v)[1]=((const u32 *)d)[1];}Val(const Val &o):v(o.v){}};
struct Work {
 u8 pad0[0x20];
 void (*onQuad)(int,const Triple *);
 void (*onData)(int,const u8 *,u32);
 u8 pad28[0x4c-0x28];
 int id;
 u8 pad50[0xe8-0x50];
 u8 table[0x144-0xe8];
 int status;
 int ready;
 Val pair;
 void SetPair(Val p){ready=1;pair=p;}
 void OnPair(const u8 *d){SetPair(Val(d));}
};
extern "C" Work *_022577C4;
extern "C" const char ov45_022550C4[],ov45_022550D4[],ov45_022550E4[],ov45_022550F4[];
extern "C" Rec *ov45_0223DEC4(void *,int);
extern "C" int ov45_02238C74(int *,const u8 *,u32);
extern "C" int ov45_0223415C(int,int,int,int,const char *,const u8 *,u32);
inline void Rec::SetVec(const u8 *d,u32 n,TU al){
 Vec v;
 VecBox box;
 v.Init();
 v.Assign(Iter(d),Iter(d+n));
 VecBox *pb=&box;
 pb->InitM(v,al);
 VecBox *pc=&box;
 vec.Update(*pc);
 ov45_022379D4(&box.v);
 vecRecv=1;
 ov45_022379D4(&v);
}
inline void Rec::SetQuad(const u8 *d){
 QuadBox qbox;
 QuadBox *pq=&qbox;
 pq->Init(*(const Quad *)d);
 q.Update(*pq);
 qRecv=1;
}
static inline void OnPairFn(const u8 *d){_022577C4->SetPair(Val(d));}
#pragma force_active on
// @fn ov45_0223415C
extern "C" int ov45_0223415C(int a0,int a1,int sel,int id,const char *cmd,const u8 *data,u32 len){
 if(sel)return 0;
 if(strcmp(cmd,ov45_022550C4)==0){
  if(id==_022577C4->id)return 1;
  Rec *r=ov45_0223DEC4(_022577C4->table,id);
  if(!r)return 1;
  TU al;
  r->SetVec(data,len,al);
  if(r->valid)_022577C4->onData(id,data,len);
  return 1;
 }
 if(strcmp(cmd,ov45_022550D4)==0){
  if(id==_022577C4->id)return 1;
  if(len!=16)return 1;
  Rec *r=ov45_0223DEC4(_022577C4->table,id);
  if(!r)return 1;
  r->SetQuad(data);
  if(r->valid&&r->q.changed){
   r->q.changed=0;
   Triple t=r->GetTriple();
   _022577C4->onQuad(id,&t);
  }
  return 1;
 }
 if(strcmp(cmd,ov45_022550E4)==0){
  if(len!=8)return 1;
  OnPairFn(data);
  return 1;
 }
 if(strcmp(cmd,ov45_022550F4)==0){
  if(!ov45_02238C74(&_022577C4->status,data,len))return 1;
  _022577C4->status=4;
  return 1;
 }
 return 0;
}
