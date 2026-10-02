#pragma force_active on
#include <nitro/types.h>
#include <stdarg.h>
#pragma force_active on
struct Dec {u8 sign,pad;s16 exponent;u8 length;s8 digits[32];};
void round_decimal__wide(struct Dec *,int);
struct String {u16 *buffer;u32 size,count;};
extern u16 *wmemcpy(u16 *,const u16 *,u32);
u16 *__wStringWrite(void *,const u16 *,u32);
extern int __wpformatter(u16 *(*)(void *,const u16 *,u32),void *,const u16 *,va_list);
int swprintf(u16 *,u32,const u16 *,...);
int vswprintf(u16 *,u32,const u16 *,va_list);
u16 *__wStringWrite(void *arg,const u16 *data,u32 count) {struct String *out=arg;u16 *result;if(out->count+count>out->size)count=out->size-out->count;result=wmemcpy(out->buffer+out->count,data,count);out->count+=count;return result;}
int swprintf(u16 *buffer,u32 size,const u16 *format,...) {va_list args;va_start(args,format);return vswprintf(buffer,size,format,args);}
int vswprintf(u16 *buffer,u32 size,const u16 *format,va_list args) {struct String out;out.buffer=buffer;out.size=size;out.count=0;int result=__wpformatter(__wStringWrite,&out,format,args);if(result<0)return result;if((u32)result<size)buffer[result]=0;else {buffer[size-1]=0;return -1;}return result;}
