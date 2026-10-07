#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct NetState { u8 pad[0x20]; u32 dhcpServer; } NetState;
extern NetState _021D4620;
extern u8 _021D4B76[];
void CPS_SocUse(void);
void CPS_SocDatagramMode(void);
void CPS_SocRelease(void);
void CPS_SocBind(u16 a, u16 b, u32 ip);
u32 CPS_SocWrite(void *p, u32 a);
u8 *pad_mem(u8 c, u32 want, u8 *p, u32 n);
u8 *dhcp_setcommon(u8 *p, int a, u32 *out);
void dhcp_release_server(void);
// @fn dhcp_release_server
void dhcp_release_server(void) {
    u32 base;
    u8 *p;
    CPS_SocUse();
    CPS_SocDatagramMode();
    CPS_SocBind(0x44, 0x43, _021D4620.dhcpServer);
    base = (u32)_021D4B76;
    p = dhcp_setcommon((u8 *)base, 7, NULL);
    *p++ = 0xff;
    p = pad_mem(0, 300, p, (u32)p - base);
    CPS_SocWrite((u8 *)base, (u32)p - base);
    CPS_SocRelease();
}
