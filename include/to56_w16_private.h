#ifndef TO56_W16_PRIVATE_H
#define TO56_W16_PRIVATE_H

#include "global.h"

typedef struct Ov13W16Runtime {
    u8 pad00[0x20];
    int started;
    int socStarted;
    u8 pad28[0x2c];
    void *allocation;
} Ov13W16Runtime;

extern Ov13W16Runtime ov13_0224DF30;
extern void OS_Sleep(u32);
extern int ov13_02223C10(void);
extern int ov13_02223C48(void);
extern int ov13_02223E6C(void);
extern void SOC_Cleanup(void);
int ov13_02223F84(void);
void ov13_02224100(void);

#endif
