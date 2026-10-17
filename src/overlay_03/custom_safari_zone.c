#include "global.h"

#include "overlay_01.h"
#include "overlay_03.h"
#include "safari_zone.h"
#include "task.h"
#include "communication_command.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"

typedef struct CustomSafariLinkWork {
    void (*func)();
    SafariZone *safariZone;
    u8 recvBuffers[0x5D0];
} CustomSafariLinkWork; // Size: 0x5D8

typedef struct CustomSafariLinkCommand {
    void (*recv)(BOOL, u32, SafariZoneAreaSet *, CustomSafariLinkWork *);
    u32 (*getSize)();
    u32 (*getBuffer)(int, int);
} CustomSafariLinkCommand;

static u32 CustomSafariLink_GetAreaSetSize();
static u32 CustomSafariLink_GetRecvBuffer(int mult, int arg1);
static void CustomSafariLink_RecvAreaSet(BOOL netId, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, CustomSafariLinkWork *work);

static void CustomSafariLink_SetStep(void *ptr, CustomSafariLinkWork *work) {
    work->func = ptr;
}

static const CustomSafariLinkCommand sCustomSafariLinkCommands = {
    CustomSafariLink_RecvAreaSet,
    CustomSafariLink_GetAreaSetSize,
    CustomSafariLink_GetRecvBuffer
};

static void CustomSafariLink_RegisterCommands(CustomSafariLinkWork *work) {
    Comm_RegisterCommandTable(&sCustomSafariLinkCommands, 1, work);
}

static u32 CustomSafariLink_GetAreaSetSize() {
    return 744;
}

static u32 CustomSafariLink_GetRecvBuffer(int mult, int arg1) {
    if (mult >= 2) {
        GF_AssertFail();
    }
    return arg1 + 8 + (744 * mult);
}

static void CustomSafariLink_RecvAreaSet(BOOL netId, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, CustomSafariLinkWork *work) {
    if (netId != Comm_GetLocalNetId()) {
        SafariZone_SetAreaSet(work->safariZone, 1, areaSetSrc);
        SafariZone_SetLinkLeaderFromProfile(work->safariZone, CommPlayerInfo_GetProfile(netId), HEAP_ID_FIELD2);
    }
}

static CustomSafariLinkWork *CustomSafariLink_AllocWork() {
    CustomSafariLinkWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(CustomSafariLinkWork));
    MI_CpuFill8(work, 0, sizeof(CustomSafariLinkWork));
    return work;
}

static BOOL Task_CustomSafariLink(TaskManager *taskManager) {
    CustomSafariLinkWork *work = TaskManager_GetEnvironment(taskManager);
    if (work->func == NULL) {
        Heap_Free(work);
        return TRUE;
    }
    work->func();
    return FALSE;
}

static void CustomSafariLink_WaitDone(CustomSafariLinkWork *work) {
    if (LinkSync_IsSynced(13)) {
        CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId());
        CustomSafariLink_SetStep(NULL, work);
    }
}

static void CustomSafariLink_SendAreaSet(CustomSafariLinkWork *work) {
    if (LinkSync_IsSynced(12) && CommPlayerInfo_GetProfile(1)) {
        Comm_SendFixedCommandNoCopy(22, SafariZone_GetAreaSet(work->safariZone, 0));
        LinkSync_StartSync(13);
        CustomSafariLink_SetStep(&CustomSafariLink_WaitDone, work);
    }
}

void CustomSafariLink_ExchangeAreaSets(FieldSystem *fieldSystem) {
    CustomSafariLinkWork *work = CustomSafariLink_AllocWork();
    work->safariZone = Save_SafariZone_Get(fieldSystem->saveData);
    CustomSafariLink_RegisterCommands(work);
    LinkSync_StartSync(12);
    CustomSafariLink_SetStep(&CustomSafariLink_SendAreaSet, work);
    TaskManager_Call(fieldSystem->taskman, &Task_CustomSafariLink, work);
}
