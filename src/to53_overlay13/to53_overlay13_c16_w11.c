#include "global.h"

extern u8 ov13_0224DF30[];
extern u8 ov13_0224DFB4[];
extern u8 ov13_0224DFC4[];

extern int WCM_DisconnectAsync(void);
extern int WCM_CleanupAsync(void);
extern int ov13_02223A7C(void);

int ov13_02223C10(void);
int ov13_02223C48(void);
int ov13_02223DE0(void);
void *ov13_02223DEC(int unused, int size);
void ov13_02223E1C(int unused, void *ptr, int size);
void ov13_02223E44(void);
u32 ov13_02223E6C(void);
void ov13_02223EA4(u32 value, int unused);
int ov13_02224A24(u8 *dst);

typedef struct WcmState {
    u8 pad00[4];
    void *(*alloc)(u32);
    u8 pad08[4];
    void (*free)(void *);
    u8 pad10[0x1c];
    int state;
    u8 pad30[0x38];
    int write;
    int read;
} WcmState;

#define STATE (*(WcmState *)ov13_0224DF30)

int ov13_02223C10(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (STATE.state == 7 && WCM_DisconnectAsync() == 3) {
        STATE.state = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}
int ov13_02223C48(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (STATE.state == 3) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        STATE.state = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (ov13_02223A7C() == 1) {
        STATE.state = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}
