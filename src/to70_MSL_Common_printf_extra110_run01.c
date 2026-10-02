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
u32 __FileWrite(void *file,const char *data,u32 count) {if(__fwrite(data,1,count,file)!=count)file=0;return (u32)file;}
u32 __StringWrite(void *arg,const char *data,u32 count) {struct String *out=arg;if(out->count+count>out->size)count=out->size-out->count;memcpy(out->buffer+out->count,data,count);out->count+=count;return 1;}
