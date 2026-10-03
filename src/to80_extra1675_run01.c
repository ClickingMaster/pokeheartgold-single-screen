#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct{u16 family,port;u32 ip;}Address;extern int SOCL_WriteTo(int,const void*,int,u16,u32,int);
int SOC_SendTo(int socket,const void*data,int len,int flags,Address*addr);
int SOC_SendTo(int socket,const void*data,int len,int flags,Address*addr){u16 port;u32 ip;if(addr){port=(u16)(((addr->port>>8)&255)|((addr->port<<8)&0xff00));ip=((addr->ip>>24)&255)|((addr->ip>>8)&0xff00)|((addr->ip<<8)&0xff0000)|((addr->ip<<24)&0xff000000);}else{port=0;ip=0;}return SOCL_WriteTo(socket,data,len,port,ip,flags);}
