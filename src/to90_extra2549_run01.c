#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int ov38_02220100(char *, const char *, int),ov38_02220148(unsigned);
int ov38_022201C8(char*out,char*in,int left);
int ov38_022202C0(char*out,char*in,int left);
int ov38_022202C0(char*out,char*in,int left){struct{s8 b[3];char a[4];}buf;int v,n,count;count=0;n=ov38_02220100(buf.a,in,4);v=count;while(n>0 && v>=0){buf.b[0]=0;buf.b[1]=0;buf.b[2]=0;v=ov38_02220148((u8)buf.a[0]);if(v>=0){buf.b[0]=v<<2;v=ov38_02220148((u8)buf.a[1]);if(v>=0){buf.b[0]|=v>>4;buf.b[1]=v<<4;v=ov38_02220148((u8)buf.a[2]);if(v>=0){buf.b[1]|=v>>2;buf.b[2]=v<<6;v=ov38_02220148((u8)buf.a[3]);if(v>=0)buf.b[2]|=v;}}} {int limit=n-1,i=0;if(limit>0){s8*p=buf.b;do{*out++=*p++;i++;}while(i<limit);}}in+=n;count+=n-1;left-=4;if(left<=0)break;n=ov38_02220100(buf.a,in,4);}return count;}
