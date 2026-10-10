#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Task {
    struct Task *next;

    union {
        u32 flags;

        struct {
            u32 busy : 1, priority : 31;
        } bits;
    };

    void (*function)(void);
    void (*callback)(void);
} Task;

extern OSThread *volatile ov13_0224CF80;
int MBi_IsTaskAvailable(void);
int MBi_IsTaskBusy(Task *);
void MBi_SetTask(Task *, void (*)(void), void (*)(void), u32);

int MBi_IsTaskBusy(Task *t) {
    return t->bits.busy ? 1 : 0;
}

void MBi_SetTask(Task *t, void (*fn)(void), void (*cb)(void), u32 priority) {
    OSThread *thread = ov13_0224CF80;
    Task *volatile *head = (Task *volatile *)((u8 *)thread + 0xc0);
    Task *cur;
    if (!MBi_IsTaskAvailable()) {
        return;
    }
    if (t->bits.busy) {
        return;
    }
    if (priority > 31) {
        u32 old = OS_GetThreadPriority(thread);
        if (priority == 32) {
            priority = old ? old - 1 : 0;
        } else if (priority == 33) {
            priority = old < 31 ? old + 1 : 31;
        } else if (priority == 34) {
            priority = old;
        } else {
            priority = 31;
        }
    }
    {
        OSIntrMode mode = OS_DisableInterrupts();
        t->flags = (t->flags & ~1) | 1;
        t->flags = (t->flags & 1) | (priority << 1);
        t->function = fn;
        t->callback = cb;
        if (!*head) {
            if (t == (Task *)((u8 *)thread + 0xc4)) {
                ov13_0224CF80 = 0;
            }
            *head = t;
            OS_WakeupThreadDirect(thread);
        } else {
            cur = *head;
            if (t == (Task *)((u8 *)thread + 0xc4)) {
                while (cur->next) {
                    cur = cur->next;
                }
                cur->next = t;
                ov13_0224CF80 = 0;
            } else {
                if (priority < (cur->flags >> 1)) {
                    *head = t;
                    t->next = cur;
                } else {
                    Task *next;
                    goto test;
advance:
                    cur = next;
test:
                    next = cur->next;
                    if (next && priority >= (next->flags >> 1)) {
                        goto advance;
                    }
                    t->next = next;
                    cur->next = t;
                }
            }
        }
        OS_RestoreInterrupts(mode);
    }
}
