#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u32 *_02110628;
extern volatile u32 _021D4420[];
extern volatile u32 _021D4670[];
extern volatile u32 _021D463C[];
extern volatile u32 _021D464C[];
extern volatile u32 _021D4680[];
extern u8 _02110660[];
extern u8 _version_NINTENDO_WiFi[];
extern void OSi_ReferSymbol(const void *);
extern void SOCLi_StartupCPS(void);
extern int SOCLi_StartupSOCL(void);
extern int SOCLi_StartupCommandPacketQueue(u32);
extern int SOCL_CreateSocket(void *);
extern const void *WCM_GetApMacAddress(void);
extern int SOCL_Startup(u32 *);
extern void SOCLi_SetMyIP(void);
extern void SOCLi_DhcpTimeout(void);
extern int SOCL_LinkIsOn(void);
extern u32 _021D4430[];
extern u16 _02110648[];
extern void CPS_SetThreadPriority(int);
extern void CPSi_RecvCallbackFunc(void);
extern void SOCLi_TrashSocket(void);
extern void WCM_SetRecvDCFCallback(void (*)(void));
extern void CPS_SetScavengerCallback(void (*)(void));
extern void CPS_Startup(void *);

void SOCLi_SetMyIP(void) {
    u32 *p = _02110628;
    _021D4670[0] = p[1];
    _021D463C[0] = p[2];
    _021D464C[0] = p[3];
    _021D4680[0] = p[4];
    _021D4680[1] = p[5];
    _021D4420[2] |= 2;
}

void SOCLi_DhcpTimeout(void) {
    _021D4420[2] |= 2;
}

int SOCL_LinkIsOn(void) {
    return WCM_GetApMacAddress() != NULL;
}
