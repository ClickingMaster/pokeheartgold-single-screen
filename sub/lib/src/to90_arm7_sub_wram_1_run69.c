#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void CARD_Init(void);
void FS_Init(void);

void FS_Init(void) {
    CARD_Init();
}
