#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
u32 strlen(const char*);int mbtowc(u16*,const char*,u32);u32 mbstowcs(u16*,const char*,u32);
u32 mbstowcs(u16*out,const char*s,u32 n){u32 i;u32 len=strlen(s);i=0;if(out){for(i=0;i<n;i++){if(*s){int used=mbtowc(out,s,len);out++;if(used>0){s+=used;len-=used;}else return -1;}else{*out=0;break;}}}return i;}
