#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct State {
    void (*log)(int, char *, ...);
    u8 *work;
} State;
typedef struct {
    u8 prefix[0x58];
    u32 seed;
    u16 selectedChannel, busy, channelMask;
    u8 tail[2];
} ScanPrefix;
typedef struct {
    u16 words[3];
} MacAddress;
extern State ov13_0224F5C0;
extern void ov13_0223D6A8(u32), ov13_0223D724(u32);
extern u16 ov13_0223E014(u16);
int ov13_0223DF3C(void);
// @fn ov13_0223DF3C
int ov13_0223DF3C(void) {
    ScanPrefix *scan;
    u16 status;
    MacAddress mac;
    OS_GetMacAddress((u8 *)&mac);
    ((ScanPrefix *)ov13_0224F5C0.work)->seed = OS_GetVBlankCount() + mac.words[0] + mac.words[1] + mac.words[2];
    scan = (ScanPrefix *)ov13_0224F5C0.work;
    scan->seed = scan->seed * 69069u + 12345u;
    ((ScanPrefix *)ov13_0224F5C0.work)->selectedChannel = 0;
    ((ScanPrefix *)ov13_0224F5C0.work)->busy = 101;
    ov13_0223D6A8(3);
    status = ov13_0223E014(1);
    if (status == 24) {
        ov13_0223D724(24);
        ov13_0223D6A8(9);
        return 0;
    }
    if (status == 2) {
        return 1;
    }
    ov13_0223D724(status);
    ov13_0223D6A8(9);
    return 0;
}
