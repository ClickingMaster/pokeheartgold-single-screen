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
} M35590;

#define MLME ((M35590 *)(BASE + 4 + 0x400))
extern int MLME_IssueBeaconLostIndication(const void *);
void MLME_BeaconLostTask(void);

void MLME_BeaconLostTask(void) {
    MLME_IssueBeaconLostIndication(BASE + 0x3c6);
}
