#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" unsigned strlen(const char *);
class String {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;} raw;
 u32 Size() const {return !raw.l.flag?raw.s.size:raw.l.size;}
 const char *Data() const {return !raw.l.flag?raw.s.data:raw.l.data;}
 int Compare(const String &other) const;
 int Compare(const char *text) const;
};
extern "C" int ov45_02238024(const String *,u32,u32,const char *,u32);
extern "C" int ov45_02237F7C(const String *,const String *);
extern "C" int ov45_02237FD8(const String *,const char *);
inline int String::Compare(const String &other) const {const char *d;u32 n;if(!other.raw.l.flag){d=other.raw.s.data;n=other.raw.s.size;}else{d=other.raw.l.data;n=other.raw.l.size;}return ov45_02238024(this,0,Size(),d,n);}
inline int String::Compare(const char *text) const {u32 n=strlen(text);return ov45_02238024(this,0,Size(),text,n);}
// @fn ov45_02237F7C
extern "C" int ov45_02237F7C(const String *a,const String *b){return a->Compare(*b);}
// @fn ov45_02237FD8
extern "C" int ov45_02237FD8(const String *a,const char *b){return a->Compare(b);}
