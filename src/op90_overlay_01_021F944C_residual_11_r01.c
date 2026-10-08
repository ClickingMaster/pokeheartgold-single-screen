#pragma force_active on
#include "global.h"
#include "sys_task.h"
#include "billboard_internal.h"
void *FldObjSys_ReadMModelFromNarc(void *, u32, u32);
void ov01_021FA61C(void *, u32, void *, u32);
typedef struct LoadRequest { u32 model, id, extra, active; } LoadRequest;
typedef struct RenderQueue { s16 count, limit, processed; u16 reserved6; void *first; LoadRequest *requests; } RenderQueue;
BOOL sub_020238F8(Billboard *);
void ov01_021FA564(SysTask *, void *);
// @fn ov01_021FA564
void ov01_021FA564(SysTask *unused, void *context) {
    u8 *owner = context;
    RenderQueue *queue = *(RenderQueue **)(owner + 0x100);
    int i = 0;
    int j;
    LoadRequest *entry = queue->requests;
    LoadRequest *entries;
    if (sub_020238F8(*(Billboard **)(owner + 0xe0)) == TRUE) {
        return;
    }
    while (queue->processed < queue->limit && i < queue->count) {
        if (entry->active) {
            void *model = FldObjSys_ReadMModelFromNarc(owner, entry->model, 0);
            ov01_021FA61C(owner, entry->id, model, entry->extra);
            entry->active = 0;
            queue->processed++;
        }
        entry++;
        i++;
    }
    entries = queue->requests;
    for (i = 0; i < queue->count - 1; i++) {
        if (entries[i].active == 0) {
            for (j = i + 1; j < queue->count; j++) {
                if (entries[j].active) {
                    entries[i] = entries[j];
                    entries[j].active = 0;
                    break;
                }
            }
        }
    }
    queue->processed = 0;
}
