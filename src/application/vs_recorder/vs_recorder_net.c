#include "global.h"

#include "application/vs_recorder/vs_recorder_screens.h"

#include "global_terminal_net.h"
#include "heap.h"
#include "sav_system_info.h"

// server response handlers of the screens, one per request in command order (still address-named)
void VsRecorder_OnDressUpUploaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnDressUpListReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnBoxUploaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnBoxListReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnRankingsHeaderReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnRankingsReceived(void *arg, GlobalTerminalNetError *error);
void ov40_02242DF0(void *arg, GlobalTerminalNetError *error);
void ov40_02242E14(void *arg, GlobalTerminalNetError *error);
void ov40_02242E48(void *arg, GlobalTerminalNetError *error);

// the Global Terminal client is embedded in the work (only started in the Global Terminal modes)
static inline GlobalTerminalNet *VsRecorder_NetClient(VsRecorderWork *work) {
    return &work->netClient;
}

GlobalTerminalNet *VsRecorder_GetNet(VsRecorderWork *work) {
    return &work->netClient;
}

#pragma force_active on

void VsRecorder_InitNet(VsRecorderWork *work);

void VsRecorder_InitNet(VsRecorderWork *work) {
    GlobalTerminalNetArgs args;
    args.dwcProfileId = Save_SysInfo_GetDwcProfileId(Save_SysInfo_Get(work->saveData));
    args.heapID = HEAP_ID_109;
    args.saveData = work->saveData;
    args.callbacks.arg = work;
    args.callbacks.onResponse[0] = VsRecorder_OnDressUpUploaded;
    args.callbacks.onResponse[1] = VsRecorder_OnDressUpListReceived;
    args.callbacks.onResponse[2] = VsRecorder_OnBoxUploaded;
    args.callbacks.onResponse[3] = VsRecorder_OnBoxListReceived;
    args.callbacks.onResponse[4] = VsRecorder_OnRankingsHeaderReceived;
    args.callbacks.onResponse[5] = VsRecorder_OnRankingsReceived;
    args.callbacks.onResponse[6] = ov40_02242DF0;
    args.callbacks.onResponse[7] = (GlobalTerminalNetResponseFunc)VsRecorder_OnVideoListReceived;
    args.callbacks.onResponse[8] = ov40_02242E14;
    args.callbacks.onResponse[9] = ov40_02242E48;
    args.messageFunc = NULL;
    args.messageArg = work;
    GlobalTerminalNet_Init(VsRecorder_NetClient(work), &args);
}

#pragma force_active reset

BOOL VsRecorder_IsNetIdle(VsRecorderWork *work) {
    if (VsRecorder_NetClient(work)->unk_3B8 == 0) {
        return FALSE;
    }
    return GlobalTerminalNet_IsIdle(VsRecorder_GetNet(work));
}

void VsRecorder_UpdateNet(VsRecorderWork *work) {
    if (VsRecorder_NetClient(work)->unk_3B8 != 0) {
        GlobalTerminalNet_Update(VsRecorder_GetNet(work));
    }
}

void VsRecorder_FreeNet(VsRecorderWork *work) {
    if (VsRecorder_NetClient(work)->unk_3B8 != 0) {
        GlobalTerminalNet_Free(VsRecorder_GetNet(work));
    }
}
