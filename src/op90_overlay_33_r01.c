#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task.h"
#include "systask_environment.h"

typedef struct Ov33Param {
    u8 unk0[4];
    u8 unk4;
    u8 unk5;
    u16 unk6;
} Ov33Param;

typedef struct Ov33Work {
    void *unk0;
    u32 unk4;
    u32 unk8;
    Ov33Param *unkC;
    Ov33Param *param;
    SysTask *task;
    u8 unk18[0x1A2];
    u16 unk1BA;
    u8 unk1BC[4];
} Ov33Work;

void ov33_0225D5D0(SysTask *task, void *data);
void ov33_0225D6F8(void);
void ov33_0225D720(void *a0);
void ov33_0225D7D4(Ov33Work *work);
void ov33_0225D84C(Ov33Work *work);
void ov33_0225D8D4(Ov33Work *work);
SysTask *ov33_0225D520(void *a0, u32 a1, u32 a2, Ov33Param *param);

// @fn ov33_0225D520
SysTask *ov33_0225D520(void *a0, u32 a1, u32 a2, Ov33Param *param) {
    SysTask *task;
    Ov33Work *work;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    reg_G2S_DB_BLDCNT = 0;
    task = CreateSysTaskAndEnvironment(ov33_0225D5D0, sizeof(Ov33Work), 10, HEAP_ID_8);
    work = SysTask_GetData(task);
    work->unk0 = a0;
    work->unk4 = a1;
    work->unk8 = a2;
    work->unkC = param;
    work->task = task;
    work->param = param;
    work->unk1BA = work->param->unk4;
    work->param->unk6 = 0xFFFF;
    work->param->unk5 = 0;
    ov33_0225D6F8();
    ov33_0225D720(a0);
    ov33_0225D7D4(work);
    ov33_0225D84C(work);
    ov33_0225D8D4(work);
    return task;
}
