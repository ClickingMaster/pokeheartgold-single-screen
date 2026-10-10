#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int, int);

typedef struct {
    u16 state, x, result, reason, y, elapsed, step, z;
    u32 a, b;
    u8 *req, *confirm;
} M35602;

#define MLME ((M35602 *)(BASE + 4 + 0x400))
extern int WSetMacAdrs(const void *);
int PARAMSET_MacAdrsReqCmd(u8 *req, u16 *confirm);

int PARAMSET_MacAdrsReqCmd(u8 *req, u16 *confirm) {
    int r = 1;
    confirm[1] = 1;
    if (*(u16 *)(BASE + 0x34c) == 16) {
        r = WSetMacAdrs(req + 16);
    }
    return r;
}
