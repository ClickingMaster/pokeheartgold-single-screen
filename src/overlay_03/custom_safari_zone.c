#include "global.h"

#include "overlay_01.h"
#include "overlay_03.h"
#include "safari_zone.h"
#include "task.h"
#include "communication_command.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
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
static void CustomSafariLink_RecvAreaSet(BOOL unkBool, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, CustomSafariLinkWork *unkCommStruct);

static void CustomSafariLink_SetStep(void *ptr, CustomSafariLinkWork *unkCommStruct) {
    unkCommStruct->func = ptr;
}

static const CustomSafariLinkCommand sCustomSafariLinkCommands = {
    CustomSafariLink_RecvAreaSet,
    CustomSafariLink_GetAreaSetSize,
    CustomSafariLink_GetRecvBuffer
};

static void CustomSafariLink_RegisterCommands(CustomSafariLinkWork *unkCommStruct) {
    Comm_RegisterCommandTable(&sCustomSafariLinkCommands, 1, unkCommStruct);
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

static void CustomSafariLink_RecvAreaSet(BOOL unkBool, u32 arg1_unused, SafariZoneAreaSet *areaSetSrc, CustomSafariLinkWork *unkCommStruct) {
    if (unkBool != Comm_GetLocalNetId()) {
        SafariZone_SetAreaSet(unkCommStruct->safariZone, 1, areaSetSrc);
        SafariZone_SetLinkLeaderFromProfile(unkCommStruct->safariZone, CommPlayerInfo_GetProfile(unkBool), HEAP_ID_FIELD2);
    }
}

static CustomSafariLinkWork *CustomSafariLink_AllocWork() {
    CustomSafariLinkWork *unkCommStruct = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(CustomSafariLinkWork));
    MI_CpuFill8(unkCommStruct, 0, sizeof(CustomSafariLinkWork));
    return unkCommStruct;
}

static BOOL Task_CustomSafariLink(TaskManager *taskManager) {
    CustomSafariLinkWork *unkCommStruct = TaskManager_GetEnvironment(taskManager);
    if (unkCommStruct->func == NULL) {
        Heap_Free(unkCommStruct);
        return TRUE;
    }
    unkCommStruct->func();
    return FALSE;
}

static void CustomSafariLink_WaitDone(CustomSafariLinkWork *unkCommStruct) {
    if (LinkSync_IsSynced(13)) {
        CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId());
        CustomSafariLink_SetStep(NULL, unkCommStruct);
    }
}

static void CustomSafariLink_SendAreaSet(CustomSafariLinkWork *unkCommStruct) {
    if (LinkSync_IsSynced(12) && CommPlayerInfo_GetProfile(1)) {
        sub_020376D4(22, SafariZone_GetAreaSet(unkCommStruct->safariZone, 0));
        LinkSync_StartSync(13);
        CustomSafariLink_SetStep(&CustomSafariLink_WaitDone, unkCommStruct);
    }
}

void CustomSafariLink_ExchangeAreaSets(FieldSystem *fieldSystem) {
    CustomSafariLinkWork *unkCommStruct = CustomSafariLink_AllocWork();
    unkCommStruct->safariZone = Save_SafariZone_Get(fieldSystem->saveData);
    CustomSafariLink_RegisterCommands(unkCommStruct);
    LinkSync_StartSync(12);
    CustomSafariLink_SetStep(&CustomSafariLink_SendAreaSet, unkCommStruct);
    TaskManager_Call(fieldSystem->taskman, &Task_CustomSafariLink, unkCommStruct);
}
