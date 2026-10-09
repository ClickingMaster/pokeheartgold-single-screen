#include <nitro/types.h>
#include <nitro/code32.h>
struct Str;
extern "C" Str *ov45_02237B58(Str *,char *);
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237A24(Str *,const Str *);
struct Str {
 u32 words[3];
 Str(char *s){ov45_02237B58(this,s);}
 Str(const Str &o){ov45_02237A24(this,&o);}
 void Destroy(){ov45_02237BC8(this);}
};
struct Entry {int id;};
struct Work {
 u8 pad0[0x4c];int self;u8 pad50[0xb8-0x50];Entry a;u8 padbc[0xc8-0xbc];int flag;u8 padcc[0xd0-0xcc];Entry b;u8 padd4[0xe8-0xd4];u8 sessions[4];
};
extern "C" Work *_022577C4;
extern "C" char ov45_022550C4[],ov45_022550D4[];
extern "C" Str ov45_022320B4(Entry *);
extern "C" int ov45_022340F8(const Str &,const Str &);
extern "C" void ov45_0223DD3C(void *,int);
extern "C" void ov45_022417B4(char *,int,char *,void (*)(),int);
extern "C" void ov45_02236308();
extern "C" void ov45_02236690(int,char *,int,int *);
inline int Find(Work *w,const Str &name){
 const Str &n=name;
 int eq;
 {Str t=ov45_022320B4(&w->a);eq=ov45_022340F8(t,n);t.Destroy();}
 if(eq)return w->a.id;
 {Str t=ov45_022320B4(&w->b);eq=ov45_022340F8(t,n);t.Destroy();}
 if(eq)return w->b.id;
 return 5;
}
inline bool IsSelf(int id){return id==_022577C4->self;}
#pragma force_active on
// @fn ov45_02236690
#pragma opt_common_subs off
extern "C" void ov45_02236690(int a,char *channel,int count,int *ids){
 int result,i;
 if(a==0)return;
 {
  Str name(channel);
  Work *w=_022577C4;
  result=Find(w,name);
  name.Destroy();
 }
 if(result==5)return;
 if(result!=0)return;
 for(i=0;i<count;i++){
  if(!IsSelf(ids[i])){
   ov45_0223DD3C(_022577C4->sessions,ids[i]);
   ov45_022417B4(channel,ids[i],ov45_022550C4,ov45_02236308,0);
   ov45_022417B4(channel,ids[i],ov45_022550D4,ov45_02236308,0);
  }
 }
 _022577C4->flag=1;
}
#pragma opt_common_subs reset
