#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef void (*TaskFunc)(void);
typedef struct {
    u16 next;
    u16 queued;
    TaskFunc func;
} WlTask;
typedef struct {
    u16 head[4];
    u16 tail[4];
    u16 current;
    u16 unk12;
    u8 unk14[0xbc - 0x14];
    WlTask task[24];
} WlTaskWork;
#define ROOT (*(WlTaskWork **)0x0380fff4)
extern const TaskFunc pTaskFunc[24];
extern void AddTask(u32 prio, u32 id);
void InitializeTask(void);
// @fn InitializeTask
void InitializeTask(void) {
    WlTaskWork *w = ROOT;
    u32 i;
    w->current = 0;
    w->unk12 = 0;
    for (i = 0; i < 4; i++) {
        w->tail[i] = 0xffff;
        w->head[i] = w->tail[i];
    }
    for (i = 0; i < 24; i++) {
        w->task[i].next = 0xffff;
        w->task[i].queued = 0;
        w->task[i].func = pTaskFunc[i];
    }
    AddTask(3, 12);
}
