#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef void *(*Allocate)(u32);
typedef void (*Release)(void *);
typedef struct{u32 dhcp;u8 unused4[20];Allocate allocate;Release release;u32 unused20,size;void *memory;int priority,mtu,receive;}UserConfig;
typedef struct{u32 staticAddress;Allocate allocate;Release release;void(*addressCallback)(void);int(*link)(void);u32 reserved14,reserved18;void *memory;u32 size;int payload;u32 dhcpAddress,localAddress;}CPSConfig;
typedef struct{u32 ip,netmask,dhcp,unusedC;CPSConfig config;}Runtime;
typedef struct{u16 unused,receive,half;}ReceiveConfig;
extern UserConfig *_02110628;
extern Runtime _021D4420;
extern u32 _021D4670;
extern ReceiveConfig _02110648;
extern void SOCLi_DhcpTimeout(void),SOCLi_SetMyIP(void),CPS_SetThreadPriority(int),SOCLi_TrashSocket(void);
typedef void(*Recv)(void*,void*,void*,u32);
extern void CPSi_RecvCallbackFunc(void*,void*,void*,u32);
extern int SOCL_LinkIsOn(void);
extern void WCM_SetRecvDCFCallback(Recv),CPS_SetScavengerCallback(void*),CPS_Startup(void*);
typedef char ConfigSize[(sizeof(CPSConfig)==0x30)?1:-1];
static inline void *AllocWork(CPSConfig *c){return _02110628->allocate(c->size);}
void SOCLi_StartupCPS(void);

/* native keeps the shared constant 1 in one register across the dhcp branch */
#pragma opt_propagation off
// @fn SOCLi_StartupCPS
void SOCLi_StartupCPS(void){
 CPSConfig *config=&_021D4420.config;UserConfig *user=_02110628;void *memory; int mtu; int priority; int receive; int payload; int half; ReceiveConfig *rc; u32 *zp;
 MI_CpuFill8((void*)config,0,0x30);
 _021D4420.config.allocate=user->allocate;_021D4420.config.release=user->release;
 _021D4420.config.link=SOCL_LinkIsOn;_021D4420.config.reserved14=0;_021D4420.config.reserved18=0;_021D4420.config.localAddress=_021D4420.ip;
 if(user->size)config->size=user->size;else config->size=0x4000;
 memory=user->memory;if(!memory)memory=AllocWork(config);
config->memory=memory;
mtu=user->mtu;
receive=user->receive;
if(!mtu)mtu=0x240;
if(!receive)receive=0x10c0;
payload=mtu-0x28;
rc=&_02110648;
zp=&_021D4670;
half=receive/2;
_02110648.receive=receive;
rc->half=half;
config->payload=payload;
*zp=0;
 {u32 on=1;if(user->dhcp){config->staticAddress=0;config->addressCallback=SOCLi_DhcpTimeout;_021D4420.dhcp=on;config->dhcpAddress=_021D4420.netmask;}
 else{_021D4420.dhcp=0;config->staticAddress=on;config->addressCallback=SOCLi_SetMyIP;}}
 priority=user->priority;if(!priority)priority=11;CPS_SetThreadPriority(priority);
 WCM_SetRecvDCFCallback(CPSi_RecvCallbackFunc);CPS_SetScavengerCallback((void*)SOCLi_TrashSocket);CPS_Startup((void*)config);
}
#pragma opt_propagation reset
