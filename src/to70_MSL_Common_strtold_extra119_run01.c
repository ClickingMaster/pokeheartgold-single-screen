#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
double strtold(const char*,char**);double atod(const char*);
extern int __StringRead(void*,int,int);extern double __strtold(int,int(*)(void*,int,int),void*,int*,int*);extern double fabs(double);extern int errno;
struct Input{const char*p;int flag;};
double strtold(const char*s,char**end){int count,range;struct Input input;double x,abs;input.p=s;input.flag=0;x=__strtold(0x7fffffff,__StringRead,&input,&count,&range);if(end)*end=(char*)s+count;abs=fabs(x);if(range || (0.0!=x && (abs<2.2250738585072014e-308 || abs>1.7976931348623157e308)))errno=34;return x;}
double atod(const char*s){return strtold(s,0);}
