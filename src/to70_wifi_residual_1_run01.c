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

int SOCL_Startup(u32 *p) {
    OSi_ReferSymbol(_version_NINTENDO_WiFi);
    if (_02110628) {
        return 0;
    }
    _02110628 = p;
    SOCLi_StartupCPS();
    return SOCLi_StartupSOCL();
}

int SOCLi_StartupSOCL(void) {
    int result = SOCLi_StartupCommandPacketQueue(_02110628[8]);
    if (result >= 0) {
        _021D4420[3] = SOCL_CreateSocket(_02110660);
    }
    return result;
}
