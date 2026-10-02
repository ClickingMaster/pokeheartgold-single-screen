#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
int strcmp(const char *,const char *);
int strcmp(const char *first,const char *second) {const u8 *a=(const u8 *)first,*b=(const u8 *)second;u32 x=*a,y=*b,align;int delta=(int)x-(int)y;if(delta)return delta;align=(u32)a&3;if(((u32)b&3)==align) {if(align) {if(!x)return 0;align=3-align;if(align) {do {x=*++a;y=*++b;delta=(int)x-(int)y;if(delta)return delta;if(!x)return 0;}while(--align);}a++;b++;}x=*(const u32 *)a;y=*(const u32 *)b;if((((x+0xfefefeff)&~x)&0x80808080))goto tail;if(x!=y)goto difference;do {a+=4;b+=4;x=*(const u32 *)a;y=*(const u32 *)b;if((x+0xfefefeff)&0x80808080)goto tail;}while(x==y);difference:a--;b--;goto bytes;tail:x=*a;y=*b;delta=(int)x-(int)y;if(delta)return delta;}bytes:if(!x)return 0;do {x=*++a;y=*++b;delta=(int)x-(int)y;if(delta)return delta;}while(x);return 0;}
