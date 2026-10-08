#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task_api.h"
typedef struct {
    u8 pad0[0x10];
    void *work;
    ManagedSprite *sprite;
} Ov85SpriteTask;
ManagedSprite *ov85_021E7044(void *work, const ManagedSpriteTemplate *tmpl);
void ov85_021E815C(SysTask *task, void *data);
void ManagedSprite_TickFrame(ManagedSprite *sprite);
SysTask *ov85_021E81E0(void *work);
// @fn ov85_021E81E0
SysTask *ov85_021E81E0(void *work) {
    Ov85SpriteTask *data;
    ManagedSpriteTemplate tmpl;
    SysTask *task;
    data = Heap_AllocAtEnd(HEAP_ID_102, sizeof(Ov85SpriteTask));
    memset(data, 0, sizeof(Ov85SpriteTask));
    tmpl.x = 128;
    tmpl.y = 100;
    tmpl.z = 0;
    tmpl.animation = 0;
    tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    tmpl.resIdList[0] = 4;
    tmpl.resIdList[1] = 5;
    tmpl.drawPriority = 0;
    tmpl.pal = 0;
    tmpl.bgPriority = 0;
    tmpl.vramTransfer = 0;
    tmpl.resIdList[2] = 6;
    tmpl.resIdList[3] = 7;
    tmpl.resIdList[4] = -1;
    tmpl.resIdList[5] = -1;
    data->sprite = ov85_021E7044(work, &tmpl);
    ManagedSprite_TickFrame(data->sprite);
    data->work = work;
    task = SysTask_CreateOnMainQueue(ov85_021E815C, data, 0);
    GF_ASSERT(task != NULL);
    return task;
}
