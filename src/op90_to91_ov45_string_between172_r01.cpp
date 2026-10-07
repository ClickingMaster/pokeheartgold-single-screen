#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" int printf(const char *,...);
extern "C" void abort(void);
extern "C" char ov45_02255144[];
class RangeDispatch {};
class String {
public:
 union {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;} raw;
};
extern "C" String *ov45_02238104(String *,u32,u32,char *,char *,RangeDispatch);
extern "C" String *ov45_02237E04(String *,u32,u32,String *,u32,u32);
template<class T> inline const T &Min(const T &a,const T &b){return a<b?a:b;}
// @fn ov45_02237E04
extern "C" String *ov45_02237E04(String *self,u32 pos,u32 n1,String *str,u32 pos2,u32 n2){
 char *data;u32 size;RangeDispatch dispatch;
 if(str->raw.l.flag){data=str->raw.l.data;size=str->raw.l.size;}else{data=str->raw.s.data;size=str->raw.s.size;}
 if(pos2>size){printf(ov45_02255144);abort();}
 data+=pos2;
 n2=Min(size-pos2,n2);
 return ov45_02238104(self,pos,n1,data,data+n2,dispatch);
}
