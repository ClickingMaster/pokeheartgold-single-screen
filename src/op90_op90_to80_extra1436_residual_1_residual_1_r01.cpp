#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
inline void *operator new(std::size_t,void *p){return p;}
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *ov45_02237B58(Str *,const char *);
struct Str {
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 Str(const char *c){ov45_02237B58(this,c);}
 const char *CStr() const {return !raw.l.flag?raw.s.data:raw.l.data;}
};
enum ReqMethod {REQ_METHOD_GET,REQ_METHOD_POST};
struct Al {};
struct TA {};struct TB {};struct TC {};struct TD {};
struct Iter {const u8 *p;u32 extra;Iter(const u8 *q):p(q){}};
struct Vec;
extern "C" void ov45_022327B8(Vec *,Iter,Iter,TD);
extern "C" const char *ov45_02231C78(const Vec *);
struct Vec {
 char *ptr;u32 size,cap;
 Vec():ptr(0),size(0),cap(0){}
 void Assign(Iter first,Iter last){Assign(first,last,TA(),TB());}
 void Assign(Iter first,Iter last,const TA&,const TB&){ov45_022327B8(this,first,last,TD());}
 void Assign(const u8 *first,const u8 *last){Assign(Iter(first),Iter(last));}
};
struct Info {u32 x;u16 one;u8 a;u8 b;u8 mac[6];u8 rest[6];};
struct Obj;
struct ObjVtbl {void (*f0)(Obj *);void (*del)(Obj *);};
extern "C" const ObjVtbl ov45_02254FC0;
extern "C" void ov45_02234D68(void);
struct Obj {
 const ObjVtbl *vtbl;int kind;int flag;Str name;Str host;u32 x;Vec data;void (*cb)(void);int x38;int x3c;
 Obj(int f,const char *n,const char *h,u32 x_,const Info &info):vtbl(&ov45_02254FC0),kind(3),flag(f),name(n),host(h),x(x_),data(),cb(ov45_02234D68),x38(0),x3c(1){
  data.Assign(Iter((const u8 *)&info),Iter((const u8 *)&info+sizeof(Info)));
 }
};
struct Work {int state;int sub;u8 pad8[0x34-8];void (*cb)(int,void *,u32);void *cb38;void (*cb3c)(int,void *,u32);u8 pad40[0x4c-0x40];u32 x4c;u32 x50;u8 pad54[0x110-0x54];Vec field;u8 pad11c[0x144-0x11c];u8 x144[4];};
extern "C" Work *_022577C4;
extern "C" Obj *ov45_022577D4;
extern "C" const char *const _02254F58[];
extern "C" int ov45_02234A24(int,int,int,void **,u32 *);
extern "C" void *ov45_02242394(u32);
extern "C" void ov45_0225443C(int,const char *);
extern "C" int ov45_02254A04(const char *,u32,const char *,u32,int);
extern "C" int ov45_022549E0(const char *,u32,const char *,u32,int);
extern "C" void ov45_022546F8(int);
extern "C" int ov45_022325B0(const void *,u32,int);
extern "C" void ov45_02234A88(int a0,int a1,int a2);
#pragma force_active on
// @fn ov45_02234A88
extern "C" void ov45_02234A88(int a0,int a1,int a2){
 Info info;
 const char *name;
 const char *host;
 Obj *o;
 u32 x;
 ReqMethod flag;
 void *buf;
 u32 len;
 Work *w=_022577C4;
 if(w==0||w->state==9)return;
 buf=0;len=0;
 int r=ov45_02234A24(a0,a1,a2,&buf,&len);
 if(r==0&&len!=0x94)r=2;
 if(r==0)ov45_022325B0(buf,len,0);
 _022577C4->cb(r,buf,len);
 if(r!=0){Work *g=_022577C4;g->sub=6;g->state=10;return;}
 const u8 *f=(const u8 *)ov45_02231C78(&_022577C4->field);
 info.x=_022577C4->x4c;
 info.one=1;
 info.a=f[0x42];
 info.b=f[0x39];
 OS_GetMacAddress(info.mac);
 x=_022577C4->x4c;
 host=_02254F58[4];
 name=_02254F58[3];
 flag=_022577C4->x50==0?REQ_METHOD_GET:REQ_METHOD_POST;
 if(ov45_022577D4!=0)return;
 o=(Obj *)ov45_02242394(sizeof(Obj));
 new(o) Obj(flag,name,host,x,info);
 ov45_022577D4=o;
 ov45_0225443C(o->flag,o->name.CStr());
 int ret;
 if(o->x3c){
  u32 n=o->data.size;
  ret=ov45_02254A04(o->host.CStr(),o->x,ov45_02231C78(&o->data),n,0);
 }else{
  u32 n=o->data.size;
  ret=ov45_022549E0(o->host.CStr(),o->x,ov45_02231C78(&o->data),n,0);
 }
 int ok;
 if(ret==0)ok=1;
 else{ov45_022546F8(ret);ok=0;}
 if(ok)return;
 if(ov45_022577D4!=0){
  Obj *p=ov45_022577D4;
  if(p!=0)p->vtbl->del(p);
  ov45_022577D4=0;
 }
}
