// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 mac[6];u16 port;u8 pad[22];} Entry;
typedef struct {u16 unused,mask;u8 pad[32];Entry entries[1];} State;
extern State *ov13_0224F5BC;
u16 ov13_0223D5C4(const u8 *);
static inline int macEqual(const u8 *mac,const u8 *other){return mac[0]==other[0]&&mac[1]==other[1]&&mac[2]==other[2]&&mac[3]==other[3]&&mac[4]==other[4]&&mac[5]==other[5];}
#pragma force_active on
// @fn ov13_0223D5C4
u16 ov13_0223D5C4(const u8 *mac) {
    u16 i;
    for (i = 1; i < 2; i++) {
        if (ov13_0224F5BC->mask & (1 << i)) {
            if (macEqual(mac, ov13_0224F5BC->entries[i - 1].mac)) {
                return ov13_0224F5BC->entries[i - 1].port;
            }
        }
    }
    return 0;
}
