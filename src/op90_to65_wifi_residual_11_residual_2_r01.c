#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    char *h_name;
    char **h_aliases;
    s16 h_addrtype;
    s16 h_length;
    char **h_addr_list;
} HostEnt;

typedef struct {
    char *addrList[2];
    HostEnt hostEnt;
    u8 unk18[0x10];
} SocHostEntWork;

u32 _021D4494[6];
SocHostEntWork _021D44AC;

extern char _021D451C[];
extern u32 SOCL_Resolve(const char *name);
extern void MI_CpuFill8(void *dst, u8 val, u32 size);
extern int STD_CopyLString(char *dst, const char *src, int len);
HostEnt *SOC_GetHostByName(const char *name);

// @fn SOC_GetHostByName
HostEnt *SOC_GetHostByName(const char *name) {
    u32 addr = SOCL_Resolve(name);
    if (addr == 0) {
        return NULL;
    }
    MI_CpuFill8(_021D451C, 0, 0x101);
    STD_CopyLString(_021D451C, name, 0x101);
    _021D44AC.hostEnt.h_name = _021D451C;
    _021D44AC.hostEnt.h_aliases = NULL;
    _021D44AC.hostEnt.h_addrtype = 2;
    _021D44AC.hostEnt.h_length = 4;
    _021D44AC.hostEnt.h_addr_list = _021D44AC.addrList;
    _021D44AC.addrList[0] = (char *)_021D4494;
    _021D44AC.addrList[1] = NULL;
    _021D4494[0] = ((addr >> 24) & 0xff) | ((addr >> 8) & 0xff00) | ((addr << 8) & 0xff0000) | ((addr << 24) & 0xff000000);
    return &_021D44AC.hostEnt;
}
