#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct { u8 b[4]; } IPAddr;
typedef struct {
    char *h_name;
    char **h_aliases;
    s16 h_addrtype;
    s16 h_length;
    IPAddr **h_addr_list;
} HostEnt;
HostEnt *ov00_021FF6E4(const char *name);
extern int ov00_0221B878;
extern IPAddr ov00_0221B87C[];
void ov00_02210E58(const char *name);
// @fn ov00_02210E58
void ov00_02210E58(const char *name) {
    HostEnt *h = ov00_021FF6E4(name);
    if (h == NULL) {
        return;
    }
    for (ov00_0221B878 = 0; ov00_0221B878 < 5; ov00_0221B878++) {
        if (h->h_addr_list[ov00_0221B878] == NULL) {
            return;
        }
        memcpy(&ov00_0221B87C[ov00_0221B878], h->h_addr_list[ov00_0221B878], 4);
    }
}
