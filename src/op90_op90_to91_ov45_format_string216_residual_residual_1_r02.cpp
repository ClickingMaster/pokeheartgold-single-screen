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
struct Vec;
extern "C" Vec *ov45_022379D4(Vec *);
struct Vec {
 char *ptr;u32 size,cap;
 void Init(){ptr=0;size=0;cap=0;}
 void Destroy(){ov45_022379D4(this);}
};
struct Entry {int id;};
struct Session {u8 pad[0x38];int active;};
typedef void (*Handler)(int,int,int,int,int,char *,u32,int);
struct Work {
 u8 pad0[0x1c];Handler handler;u8 pad20[0xb8-0x20];Entry a;u8 padbc[0xd0-0xbc];Entry b;u8 padd4[0xe8-0xd4];u8 sessions[4];
};
extern "C" Work *_022577C4;
extern "C" Str ov45_022320B4(Entry *);
extern "C" int ov45_022340F8(const Str &,const Str &);
extern "C" int ov45_02233FE4(const char *,Vec *);
extern "C" char *ov45_02231C78(Vec *);
extern "C" int ov45_0223415C(int,int,int,int,int,char *,u32,int);
extern "C" Session *ov45_0223DEC4(void *,int);
extern "C" void ov45_02233E10(int,int,char *,int,int,const char *,int);
inline int Find(Work *w,const Str &name){
 const Str &n=name;
 int eq;
 {Str t=ov45_022320B4(&w->a);eq=ov45_022340F8(t,n);t.Destroy();}
 if(eq)return w->a.id;
 {Str t=ov45_022320B4(&w->b);eq=ov45_022340F8(t,n);t.Destroy();}
 if(eq)return w->b.id;
 return 5;
}
inline int IsActive(Session *s){return s!=0&&s->active!=0;}
#pragma force_active on
// @fn ov45_02233E10
#pragma opt_common_subs off
extern "C" void ov45_02233E10(int a,int b,char *channel,int id,int arg,const char *text,int extra){
 int kind;
 Vec data;
 {Str name(channel);kind=Find(_022577C4,name);name.Destroy();}
 if(kind==5)return;
 data.Init();
 if(!ov45_02233FE4(text,&data)){data.Destroy();return;}
 u32 len=data.size;
 if(len==0){data.Destroy();return;}
 if(ov45_0223415C(a,b,kind,id,arg,ov45_02231C78(&data),len,extra)){data.Destroy();return;}
 if(id!=-1){
  if(!IsActive(ov45_0223DEC4(_022577C4->sessions,id))){data.Destroy();return;}
 }
 Work *w=_022577C4;
 w->handler(a,b,kind,id,arg,ov45_02231C78(&data),data.size,extra);
 data.Destroy();
}
#pragma opt_common_subs reset
