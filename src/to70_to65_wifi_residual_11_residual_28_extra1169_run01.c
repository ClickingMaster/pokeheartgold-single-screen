#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0x30];u8 session;u8 q;u16 method;u8 random[32];u8 p[32];u8 id[32];u8 r[0x455-0x94];u8 state;}Work;
extern int version_ok(int,int),select_method(u8*,int,int);extern void find_session_from_id(Work*);
void client_hello_v2(Work*,u8*),client_hello(Work*,u8*);
void client_hello_v2(Work*w,u8*data){int len,session,n,offset;u16 method;{int valid=version_ok(data[0],data[1]);if(valid==0)return;}len=(data[2]<<8)+data[3];method=select_method(data+8,len/3,3);if(method==0)return;w->method=method;n=data[4];session=(n<<8)+data[5];n=(data[6]<<8)+data[7];w->session=0;offset=len+8+session;if(n>=32)MI_CpuCopy8(data+offset,w->random,32);else{MI_CpuFill8(w->random,0,32-n);MI_CpuCopy8(data+offset,w->random+32-n,n);}w->state=1;return;}
void client_hello(Work*w,u8*data){int n,len;u16 method;{int valid=version_ok(data[0],data[1]);if(valid==0)return;}MI_CpuCopy8(data+2,w->random,32);n=data[34];data=(u8*)((u32)data+35);if(n!=32)w->session=0;else{MI_CpuCopy8(data,w->id,32);find_session_from_id(w);}{u8*methods=data+n;len=(methods[0]<<8)+methods[1];method=select_method(methods+2,len/2,2);}w->method=method;if(method){w->state=1;return;}return;}
