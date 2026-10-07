#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
class String {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} raw;
 u32 Size() const {return !raw.l.flag?raw.s.size:raw.l.size;}
};
extern "C" void ov45_02237BFC(String *,u32);
extern "C" String *ov45_02237E04(String *,u32,u32,String *,u32,u32);
extern "C" String *ov45_02237AA0(String *,String *,u32,u32);
template<class T> inline const T &Min(const T &a,const T &b){return a<b?a:b;}
// @fn ov45_02237AA0
extern "C" String *ov45_02237AA0(String *self,String *str,u32 pos,u32 n){
 u32 i;
 for(i=0;i<3;i++)self->raw.words[i]=0;
 u32 size=str->Size();
 ov45_02237BFC(self,pos<=size?Min(size-pos,n):0);
 ov45_02237E04(self,0,0,str,pos,n);
 return self;
}
