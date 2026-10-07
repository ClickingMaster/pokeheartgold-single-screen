#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    char *h_name;
    char **h_aliases;
    s16 h_addrtype;
    s16 h_length;
    u8 **h_addr_list;
} HostEnt;
HostEnt *SOC_GetHostByName(const char *name);
u32 ov00_021FFA44(const char *name);
u32 ov00_0220FFE4(const char *name);
// @fn ov00_0220FFE4
u32 ov00_0220FFE4(const char *name) {
    HostEnt *h;
    u32 addr = ov00_021FFA44(name);
    if (addr != (u32)-1) {
        return addr;
    }
    h = SOC_GetHostByName(name);
    if (h == NULL) {
        return 0;
    }
    return *(u32 *)h->h_addr_list[0];
}
