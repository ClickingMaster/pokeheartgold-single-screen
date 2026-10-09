#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
inline void*operator new(std::size_t,void*storage){return storage;}
namespace NativeSort {
struct Pair {s32 key;u32 payload;};
struct Buffer {s32 capacity,count;Pair*data;};
struct Cursor {Pair*position;explicit Cursor(Pair*p):position(p){};Cursor(){};
 Pair&operator*()const{return *position;} Pair*operator->()const{return position;}
 Cursor&operator++(){++position;return *this;} Cursor&operator--(){--position;return *this;}
 Cursor operator+(s32 n)const{Cursor t(*this);t.position+=n;return t;}
 s32 operator-(const Cursor&other)const{return (s32)((u32)position-(u32)other.position)/(s32)sizeof(Pair);}
 bool operator==(const Cursor&other)const{return position==other.position;}
};
inline bool operator!=(const Cursor&a,const Cursor&b){return !(a==b);}
struct ConstructingOut {Pair*position;Buffer*buffer;
 ConstructingOut(Pair*p,Buffer*b):position(p),buffer(b){}
 ConstructingOut(){}
 ConstructingOut operator*()const{return *this;}
 ConstructingOut&operator=(const Pair&value){
  if(position-buffer->data<buffer->count)*position=value;
  else{if(position)new(position)Pair(value);++buffer->count;}
  return *this;
 }
 s32 operator-(const ConstructingOut &o) const { return position - o.position; }
 ConstructingOut&operator++(){++position;return *this;}
 ConstructingOut&operator+=(s32 n){position+=n;return *this;}
 ConstructingOut operator+(s32 n)const{return ConstructingOut(*this)+=n;}
};
inline ConstructingOut CopyForward(Cursor first,Cursor last,ConstructingOut output){
 while(first!=last){*output=*first;++first;++output;}return output;
}
inline void IterSwap(const Cursor &a, const Cursor &b) { Pair t = *a; *a = *b; *b = t; }
inline bool Less(const Pair &a, const Pair &b) { return a.key < b.key; }
inline ConstructingOut CopyReverse(Cursor first,Cursor last,ConstructingOut output){
 while(first!=last){--last;*output=*last;++output;}return output;
}
}
using namespace NativeSort;
extern "C" void ov45_0223A9C0(Cursor, Cursor, void *);
extern "C" void ov45_0223A0CC(Cursor, Cursor, ConstructingOut, void *);
extern "C" Cursor ov45_02239F48(ConstructingOut, ConstructingOut, ConstructingOut, ConstructingOut, Cursor, void *);
extern "C" void ov45_02239518(Cursor first, Cursor middle, Cursor last, void *ctx, s32 len1, s32 len2, ConstructingOut buf, ConstructingOut bufEnd);
#pragma force_active on
extern "C" void ov45_02239294(Cursor first, Cursor last, ConstructingOut buf, ConstructingOut bufEnd, void *ctx);
// @fn ov45_02239294
extern "C" void ov45_02239294(Cursor first, Cursor last, ConstructingOut buf, ConstructingOut bufEnd, void *ctx) {
    s32 len = last - first;
    if (len <= 1) return;
    if (len == 2) {
        --last;
        if (Less(*last, *first)) IterSwap(first, last);
        return;
    }
    if (len <= 20) {
        ov45_0223A9C0(first, last, ctx);
        return;
    }
    s32 half = len - len / 2;
    Cursor middle = first + half;
    if (len <= bufEnd - buf) {
        ov45_0223A0CC(first, middle, buf, ctx);
        ov45_0223A0CC(middle, last, buf + half, ctx);
        ov45_02239F48(buf, buf + half, buf + half, buf + len, first, ctx);
    } else {
        ov45_02239294(first, middle, buf, bufEnd, ctx);
        ov45_02239294(middle, last, buf, bufEnd, ctx);
        ov45_02239518(first, middle, last, ctx, middle.position - first.position, last.position - middle.position, buf, bufEnd);
    }
}
