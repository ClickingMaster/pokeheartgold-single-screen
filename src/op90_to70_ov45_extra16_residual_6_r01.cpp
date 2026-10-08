#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" int memcmp(const void *,const void *,unsigned);
class Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237D48(Str *,const Str *);
class Str {
public:
 u32 words[3];
 Str(){u32 i;u32 *p=words;for(i=0;i<3;i++)p[i]=0;}
 void Destroy(){ov45_02237BC8(this);}
 Str &operator=(const Str &o){ov45_02237D48(this,&o);return *this;}
};
struct InfoHead { int id; };
struct Info : InfoHead {
 u64 mid;int z;
 Info(){}
 Info(const Info &o):InfoHead(o){*this=o;}
 bool operator==(const Info &o) const {return memcmp(this,&o,sizeof(Info))==0;}
};
struct Entry {
 Info info;int valid;
 Entry(const Info &i):info(i),valid(1){}
 void Assign(const Info &i){info=i;valid=1;}
};
struct Peer {int kind;Str name;int a;int b;
 void Reset(){Str empty;kind=5;name=empty;a=0;b=0;empty.Destroy();}
};
struct Work {
 int state;int sub;void (*callback)(int,int);u8 padc[0xd0-0xc];Peer peer;u8 pade8[0xfc-0xe8];Entry entry;u8 pad110[0x128-0x110];int dirty;
};
extern "C" Work *_022577C4;
extern "C" char ov45_02254D5C[],ov45_02254D1C[],ov45_02254D3C[];
extern "C" void ov45_02241D58(int, const char *);
extern "C" void ov45_02235A94(int,int,int,int);
#pragma force_active on
// @fn ov45_02235A94
#pragma opt_common_subs off
extern "C" void ov45_02235A94(int result,int unused,int arg,int kind){
 if(kind==5)return;
 if(result!=0){
  const char *name;
  switch(kind){
  case 1:case 2:name=ov45_02254D5C;break;
  case 3:name=ov45_02254D1C;break;
  case 4:name=ov45_02254D3C;break;
  default:name=0;break;
  }
  if(name==0){Work *w=_022577C4;w->sub=1;w->state=10;}
  ov45_02241D58(arg, name);
  {
   Work *w=_022577C4;
   w->entry.valid=0;
   Info info;
   info=w->entry.info;
   info.id=kind;
   Entry e(info);
   const Info &ni=e.info;
   if(!(w->entry.info==ni))w->entry.Assign(ni);
   w->dirty=1;
  }
 }else{
  _022577C4->peer.Reset();
 }
 _022577C4->callback(result,kind);
}
#pragma opt_common_subs reset
