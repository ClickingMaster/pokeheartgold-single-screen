#pragma force_active on
#include <nitro/types.h>
#include <stdarg.h>
#pragma force_active on
struct Dec {u8 sign,pad;s16 exponent;u8 length;s8 digits[32];};
void round_decimal(struct Dec *,int);
struct String {char *buffer;u32 size,count;};
extern void *memcpy(void *,const void *,u32);
extern u32 __fwrite(const void *,u32,u32,void *);
u32 __FileWrite(void *,const char *,u32);
u32 __StringWrite(void *,const char *,u32);
extern int __pformatter(u32 (*)(void *,const char *,u32),void *,const char *,va_list);
int vsnprintf(char *,u32,const char *,va_list);
int vsprintf(char *,const char *,va_list);
int snprintf(char *,u32,const char *,...);
int sprintf(char *,const char *,...);
int vsnprintf(char *buffer,u32 size,const char *format,va_list args) {struct String out;out.buffer=buffer;out.size=size;out.count=0;int result=__pformatter(__StringWrite,&out,format,args);if(buffer) {if((u32)result<size)buffer[result]=0;else if(size)buffer[size-1]=0;}return result;}
int vsprintf(char *buffer,const char *format,va_list args) {return vsnprintf(buffer,-1,format,args);}
int snprintf(char *buffer,u32 size,const char *format,...) {va_list args;va_start(args,format);return vsnprintf(buffer,size,format,args);}
int sprintf(char *buffer,const char *format,...) {va_list args;va_start(args,format);return vsnprintf(buffer,-1,format,args);}
