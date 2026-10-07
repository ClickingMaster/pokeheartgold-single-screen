#include <nitro/types.h>
#include <nitro/code32.h>
extern "C" unsigned strlen(const char *);
class ResizeDispatch {};
struct Vec;
class Decoder {
public:
 int Decode(const char *src,u32 len,Vec *dst) const;
};
struct Vec {char *ptr;u32 size,cap;};
extern "C" void ov45_0223795C(Vec *,u32,ResizeDispatch);
extern "C" char *ov45_02231C78(Vec *);
extern "C" int ov00_021EFF3C(const char *,u32,char *,u32);
extern "C" int ov45_02233FE4(const char *,Vec *);
inline void Shrink(Vec *v,u32 count,ResizeDispatch){v->size-=count;}
inline void Resize(Vec *v,u32 count){
 u32 size=v->size;
 if(count>size)ov45_0223795C(v,count-size,ResizeDispatch());
 else if(count<size)Shrink(v,size-count,ResizeDispatch());
}
inline int Decoder::Decode(const char *src,u32 len,Vec *dst) const {return ov00_021EFF3C(src,len,ov45_02231C78(dst),dst->size);}
#pragma force_active on
// @fn ov45_02233FE4
extern "C" int ov45_02233FE4(const char *text,Vec *out){
 Decoder codec=Decoder();
 u32 len=strlen(text);
 int n;
 Resize(out,len);
 if(len==0)return 1;
 n=codec.Decode(text,strlen(text),out);
 if(n==-1)return 0;
 Resize(out,n);
 return 1;
}
