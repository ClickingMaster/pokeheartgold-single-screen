#include <nitro/types.h>
#include <nitro/code32.h>
class String {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;} raw;
 u32 Size() const {return !raw.l.flag?raw.s.size:raw.l.size;}
};
extern "C" int ov45_02237F7C(const String *,const String *);
extern "C" int ov45_022340F8(const String &,const String &);
#pragma force_active on
// @fn ov45_022340F8
extern "C" int ov45_022340F8(const String &a,const String &b){
 return a.Size()==b.Size()&&ov45_02237F7C(&a,&b)==0;
}
