#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x18];
    void *friends;
} Work;

extern struct {
    u32 a, b;
    Work *w;
} ov00_0221A918;

void *ov00_021EF3A4(void);
int DWC_GetGsProfileId(void *, void *);
int ov00_021F03BC(int);

int ov00_021F03BC(int index) {
    void *friends = ov00_0221A918.w->friends;
    int result;
    if (!friends) {
        return 0;
    }
    result = DWC_GetGsProfileId(ov00_021EF3A4(), (u8 *)friends + index * 12);
    if (result != 0 && result != -1) {
        return result;
    }
    return 0;
}
