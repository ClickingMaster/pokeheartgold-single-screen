#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
class Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237A24(Str *,const Str *);
extern "C" Str *ov45_02237D48(Str *,const Str *);
class Str {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 Str(const Str &o){ov45_02237A24(this,&o);}
 void Destroy() const {ov45_02237BC8(const_cast<Str *>(this));}
 Str &operator=(const Str &o){ov45_02237D48(this,&o);return *this;}
 const char *CStr() const {return !raw.l.flag?raw.s.data:raw.l.data;}
};
struct Info {
 int id;Str name;int x;int y;
 Info(const Str &n):id(0),name(n),x(0),y(0){}
 void Destroy() const {name.Destroy();}
};
struct Work;
struct Work {u32 state;u32 sub;u8 pad8[0xb8-8];Info self;};
struct Callbacks {void (*a)();void (*b)();void (*c)();void (*d)();void (*e)();u32 f;
 Callbacks(void (*a_)(),void (*b_)(),void (*c_)(),void (*d_)(),void (*e_)()):a(a_),b(b_),c(c_),d(d_),e(e_),f(0){}};
extern "C" Work *_022577C4;
extern "C" void ov45_02235C40(),ov45_02235E4C(),ov45_02236090(),ov45_022362D0(),ov45_02236A44(),ov45_022354F4();
extern "C" Str ov45_02231EB0(Work *,int);
extern "C" Str ov45_02233994(int,const Str &);
extern "C" int ov45_02241048(const char *,int,int,Callbacks *,void (*)(),int);
extern "C" int ov45_02233CA0(void);
class ErrorMap {public: int Map(int r) const;};
inline int ErrorMap::Map(int r) const {
 int state=0;
 switch(r){case 1:state=2;break;case 2:state=3;break;}
 return state;
}
inline void SetSelf(const Str &n){Info i(n);Work *w=_022577C4;w->self.id=i.id;w->self.name=i.name;w->self.x=i.x;w->self.y=i.y;i.Destroy();}
inline Str Build(Str t){Str n=ov45_02233994(0,t);t.Destroy();return n;}
struct Info2 { int id; u32 nm[3]; int x; int y; Str &name() { return *(Str*)nm; } void Destroy() {name().Destroy();} };
struct Work2 {u32 state;u32 sub;u8 pad8[0xb8-8];Info2 self;};
inline void SetSelfK(const Str &n){Info2 i;Work2 *w;Info2 *p=&i;i.id=0;ov45_02237A24(&i.name(),&n);p->x=0;p->y=0;w=(Work2*)_022577C4;w->self.id=i.id;w->self.name()=i.name();w->self.x=i.x;w->self.y=i.y;i.Destroy();}
#pragma push
#pragma opt_propagation off
// @fn ov45_02233CA0
extern "C" int ov45_02233CA0(void){
 Callbacks cb(ov45_02235C40,ov45_02235E4C,ov45_02236090,ov45_022362D0,ov45_02236A44);
 Str name=Build(ov45_02231EB0(_022577C4,0));
 int result=ov45_02241048(name.CStr(),0,0x14,&cb,ov45_022354F4,0);
 {
  Work *w=_022577C4;
  int state=ErrorMap().Map(result);
  if(state!=0){w->sub=state;w->state=10;}
 }
 if(result!=0){name.Destroy();return result;}
 {
  SetSelfK(name);
 }
 name.Destroy();
 return result;
}

#pragma pop
