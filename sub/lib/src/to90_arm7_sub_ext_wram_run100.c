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

u16 DeleteTask(u32 priority);

u16 DeleteTask(u32 priority) {
    Work *w = WORK;
    u32 irq = __VENEER_OS_DisableIrqMask(0x1000010);
    u16 index = w->head[priority];
    if (index != 65535) {
        Task *t = w->tasks;
        t[index].queued = 0;
        {
            u16 next = t[index].next;
            if (next == 65535) {
                w->head[priority] = 65535;
                w->tail[priority] = 65535;
            } else {
                w->head[priority] = next;
                t[index].next = 65535;
            }
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    return index;
}
