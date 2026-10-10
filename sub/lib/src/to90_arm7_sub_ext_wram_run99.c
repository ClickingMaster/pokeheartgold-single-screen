#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    volatile u16 next, queued;
    void (*fn)(void);
} Task;

typedef struct {
    volatile u16 head[4], tail[4], priority, selected, current;
    u8 pad[0xbc - 0x16];
    Task tasks[24];
    u32 mode, arena, heap;
    u8 extra[0x308 - 0x188];
    void *queue;
} Work;

#define WORK (*(Work *volatile *)0x380fff4)
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern void __VENEER_OS_EnableIrqMask(u32);
extern int __VENEER_OS_SendMessage(void *, void *, int);
void AddTask(int priority, u32 index);

void AddTask(int priority, u32 index) {
    Work *w = WORK;
    Task *t = w->tasks;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000010);
    if (t[index].queued == 0) {
        t[index].queued = 1;
        t[index].next = 65535;
        {
            u16 tail = w->tail[priority];
            if (tail == 65535) {
                w->head[priority] = index;
            } else {
                w->tasks[tail].next = index;
            }
        }
        w->tail[priority] = index;
        if (priority < w->priority) {
            w->priority = priority;
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    if (priority != 3 && w->selected == 3) {
        __VENEER_OS_SendMessage(WORK->queue, 0, 0);
    }
}
