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

int ov13_02223DE0(void) {
    return STATE.state;
}
void *ov13_02223DEC(int unused, int size) {
    void *result;
    if (size > 0) {
        OS_LockMutex((OSMutex *)ov13_0224DFC4);
        result = STATE.alloc(size);
        OS_UnlockMutex((OSMutex *)ov13_0224DFC4);
        return result;
    }
    return NULL;
}
void ov13_02223E1C(int unused, void *ptr, int size) {
    if (ptr != NULL && size > 0) {
        OS_LockMutex((OSMutex *)ov13_0224DFC4);
        STATE.free(ptr);
        OS_UnlockMutex((OSMutex *)ov13_0224DFC4);
    }
}
void ov13_02223E44(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int i;
    STATE.write = 0;
    STATE.read = 0;
    for (i = 0; i < 4; i++) {
        ((u32 *)ov13_0224DFB4)[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}
u32 ov13_02223E6C(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int read = STATE.read;
    u32 result;
    if (STATE.write == read) {
        result = 0;
    } else {
        result = ((u32 *)ov13_0224DFB4)[read];
        STATE.read = read + 1;
        if (STATE.read >= 4) {
            STATE.read = 0;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}
void ov13_02223EA4(u32 value, int unused) {
    int write = STATE.write;
    int read = STATE.read;
    int next = write + 1;
    if (next != read && write != read + 3) {
        ((u32 *)ov13_0224DFB4)[write] = value;
        STATE.write = next;
        if (next >= 4) {
            STATE.write = 0;
        }
    }
}
