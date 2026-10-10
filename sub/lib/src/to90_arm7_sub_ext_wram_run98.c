#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    volatile u16 next, queued;
    void (*fn)(void);
} Task;

typedef struct {
    volatile u16 head[4], tail[4];
    u16 priority, selected, current;
    u8 pad[0xbc - 0x16];
    Task tasks[24];
    u32 mode, arena, heap;
    u8 extra[0x308 - 0x188];
    void *queue;
} Work;

#define WORK (*(Work *volatile *)0x380fff4)
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern void __VENEER_OS_EnableIrqMask(u32);
extern int __VENEER_OS_ReceiveMessage(void *, void **, int);
extern void ExecuteMessage(void **);
extern u16 DeleteTask(u32);
void MainTaskRoutine(void *arg);

void MainTaskRoutine(void *arg) {
    Work *w = WORK;
    void *message;
    u32 mask = 0x1000010;
    *(u16 *)((u8 *)w + 16) = 0;
    u32 empty = 65535;
    *(u16 *)((u8 *)w + 20) = 0;
    while (1) {
        if (__VENEER_OS_ReceiveMessage(WORK->queue, &message, 0)) {
            ExecuteMessage(&message);
        }
        {
            u32 irq = __VENEER_OS_DisableIrqMask(mask);
            u16 priority = (*(volatile u16 *)&w->priority);
            (*(volatile u16 *)&w->selected) = priority;
            if (w->head[priority] == empty) {
                (*(volatile u16 *)&w->priority)++;
                __VENEER_OS_EnableIrqMask(irq);
                continue;
            }
            __VENEER_OS_EnableIrqMask(irq);
        }
        (*(volatile u16 *)&w->current) = DeleteTask(*(volatile u16 *)&w->selected);
        w->tasks[(*(volatile u16 *)&w->current)].fn();
        (*(volatile u16 *)&w->current) = empty;
    }
}
