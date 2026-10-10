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
} M35600;

#define MLME ((M35600 *)(BASE + 4 + 0x400))
extern void MIi_CpuClear16(int, void *, u32);
void InitializeMLME(void);

void InitializeMLME(void) {
    MIi_CpuClear16(0, MLME, 32);
}
