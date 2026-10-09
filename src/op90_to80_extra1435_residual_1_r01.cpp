#include <nitro/types.h>
#include <nitro/code32.h>
class Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237A24(Str *,const Str *);
extern "C" int ov45_02237FD8(const Str *,const char *);
class Str {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 Str(){u32 i;u32 *p=raw.words;for(i=0;i<3;i++)p[i]=0;}
 Str(const Str &o){ov45_02237A24(this,&o);}
 void Destroy(){ov45_02237BC8(this);}
 const char *CStr() const {return !raw.l.flag?raw.s.data:raw.l.data;}
 bool operator==(const char *t) const {return ov45_02237FD8(this,t)==0;}
};
struct Work {int state;int sub;u8 pad8[0xb8-8];u8 names[4];};
extern "C" Work *_022577C4;
extern "C" char ov45_022550B4[];
extern "C" Str ov45_02232434(u8 *,int);
extern "C" int ov45_0223D7D4(int,int,int,int,int,Str *);
extern "C" int ov45_022414C0(int,const char *);
extern "C" int ov45_022413D4(const char *,const char *);
extern "C" int ov45_0223325C(int,int,int,int,int,int,int);
class ErrorMap {public: int Map(int r) const;};
inline int ErrorMap::Map(int r) const {
 int state=0;
 switch(r){case 1:state=2;break;case 2:state=3;break;}
 return state;
}
inline void SetError(int r){
 Work *g=_022577C4;
 int state=ErrorMap().Map(r);
 if(state!=0){g->sub=state;g->state=10;}
}
#pragma force_active on
// @fn ov45_0223325C
extern "C" int ov45_0223325C(int id,int key,int a,int b,int c,int d,int e){
 int result;
 if(id==-1&&key==5)return 3;
 Str value;
 if(!ov45_0223D7D4(a,b,c,d,e,&value)){value.Destroy();return 3;}
 if(key==5){
  result=ov45_022414C0(id,value.CStr());
  SetError(result);
  value.Destroy();
  return result;
 }
 Str name=ov45_02232434(_022577C4->names,key);
 if(name==ov45_022550B4){name.Destroy();value.Destroy();return 5;}
 result=ov45_022413D4(name.CStr(),value.CStr());
 SetError(result);
 name.Destroy();
 value.Destroy();
 return result;
}
