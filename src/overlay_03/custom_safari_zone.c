#include "overlay_01.h"
#include "overlay_03.h"

#include "bag.h"
#include "bag_types_def.h"
#include "bag_view.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "global.h"
#include "launch_application.h"
#include "link_ruleset_data.h"
#include "mart.h"
#include "render_text.h"
#include "render_window.h"
#include "safari_zone.h"
#include "save_link_ruleset.h"
#include "scrcmd.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_0200ACF0.h"
#include "unk_0200FA24.h"
#include "unk_02033AE0.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_0205FD20.h"
#include "unk_02091054.h"
#include "unk_02097268.h"
#include "unk_02097720.h"

#include "field/overlay_01_021E8744.h"
#include "field/ov01_021E7FDC.h"
#include "field/overlay_01_02204004.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0046.h"
#include "msgdata/msg/msg_0182.h"
#include "msgdata/msg/msg_0277.h"

typedef struct UnkFuncs {
    void (*unk0)(BOOL, u32, SafariZoneAreaSet*, UnkCommStruct*);
    u32 (*unk4)();
    u32 (*unk8)(int, int);
} UnkFuncs;

static u32 ov03_02258828();
static u32 ov03_02258830(int mult, int arg1);
static void ov03_0225884C(BOOL unkBool, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, UnkCommStruct* unkCommStruct);

static void ov03_02258810(void *ptr, UnkCommStruct *unkCommStruct) {
    unkCommStruct->func = ptr;
}

static const UnkFuncs ov03_022597FC = {
    ov03_0225884C,
    ov03_02258828,
    ov03_02258830
};

static void ov03_02258814(UnkCommStruct *unkCommStruct) {
    sub_0203410C(&ov03_022597FC, 1, unkCommStruct);
}

static u32 ov03_02258828() {
    return 744;
}

static u32 ov03_02258830(int mult, int arg1) {
    if (mult >= 2) {
        GF_AssertFail();
    }
    return arg1 + 8 + (744 * mult);
}

static void ov03_0225884C(BOOL unkBool, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, UnkCommStruct* unkCommStruct) {
    if (unkBool != sub_0203769C()) {
        SafariZone_SetAreaSet(unkCommStruct->safariZone, 1, areaSetSrc);
        SafariZone_SetLinkLeaderFromProfile(unkCommStruct->safariZone, sub_02034818(unkBool), HEAP_ID_FIELD2);
    }
}

static UnkCommStruct *ov03_02258878() {
    UnkCommStruct *unkCommStruct = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkCommStruct));
    MI_CpuFill8(unkCommStruct, 0, sizeof(UnkCommStruct));
    return unkCommStruct;
}

static BOOL ov03_02258894(TaskManager *taskManager) {
    UnkCommStruct *unkCommStruct = TaskManager_GetEnvironment(taskManager);
    if (unkCommStruct->func == NULL) {
        Heap_Free(unkCommStruct);
        return TRUE;
    }
    unkCommStruct->func();
    return FALSE;
}

static void ov03_022588B0(UnkCommStruct *unkCommStruct) {
    if (sub_02037B38(13)) {
        sub_02034818(1 - sub_0203769C());
        ov03_02258810(NULL, unkCommStruct);
    }
}

static void ov03_022588D4(UnkCommStruct *unkCommStruct) {
    if (sub_02037B38(12) && sub_02034818(1)) {
        sub_020376D4(22, SafariZone_GetAreaSet(unkCommStruct->safariZone, 0));
        sub_02037AC0(13);
        ov03_02258810(&ov03_022588B0, unkCommStruct);
    }
}

void ov03_02258910(FieldSystem *fieldSystem) {
    UnkCommStruct *unkCommStruct = ov03_02258878();
    unkCommStruct->safariZone = Save_SafariZone_Get(fieldSystem->saveData);
    ov03_02258814(unkCommStruct);
    sub_02037AC0(12);
    ov03_02258810(&ov03_022588D4, unkCommStruct);
    TaskManager_Call(fieldSystem->taskman, &ov03_02258894, unkCommStruct);
}
