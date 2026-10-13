#include "global_terminal_net.h"

#include <nitro/os/alloc.h>
#include <nitro/os/arena.h>

#include "global.h"

#include "msgdata/msg.naix"

#include "assert.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_00_thumb.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "string_util.h"
#include "system.h"

// request API of the socket client (src/global_terminal_socket.c, ARM)
BOOL GlobalTerminalServer_Init(GlobalTerminalNetLogin *login);
void GlobalTerminalServer_Stop(void);
int GlobalTerminalServer_GetState(void);
int GlobalTerminalServer_GetError(void);
BOOL GlobalTerminalServer_UploadDressUp(void *request, void *response);
BOOL GlobalTerminalServer_DownloadDressUp(int arg, void *response);
BOOL GlobalTerminalServer_UploadBox(int arg, void *request, void *response);
BOOL GlobalTerminalServer_DownloadBox(int arg, void *response);
void GlobalTerminalServer_GetResponseSize(GlobalTerminalNetResponse *response);
void *GlobalTerminalServer_GetResponse(GlobalTerminalNet *net);
int GlobalTerminalServer_GetResponseBufferSize(int command);
BOOL ov39_0222A2CC(void *response);
BOOL GlobalTerminalServer_GetRankings(void *request, void *response);
BOOL GlobalTerminalServer_UploadVideo(void *request, void *response);
BOOL GlobalTerminalServer_SearchVideos(void *request, void *response);
BOOL ov39_0222A3DC(void *request, void *response);
BOOL ov39_0222A434(void *request, void *response);
BOOL GlobalTerminalServer_DownloadVideo(u32 argLow, u32 argHigh, int arg2, void *response);
BOOL ov39_0222A4C0(u32 argLow, u32 argHigh, void *response);

// no public header
void ov00_021EC210(void);
void ov00_021EC8D8(void);
void CommState_FreeWfcSession(void);
u32 BattleRecord_sizeof(void);
void *sub_0202FC74(void);
void *BattleRecord_GetProfile(void);
void VsRecorder_EncryptRecord(SaveData *saveData);
int VsRecorder_SaveRecordNumber(SaveData *saveData, int arg1, int arg2, void *out1, void *out2);

extern const GlobalTerminalNetStepFunc *const sGlobalTerminalNetSequences[];
extern OSHeapHandle sGlobalTerminalNetOsHeap;

BOOL GlobalTerminalNet_SendPendingRequest(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_PollStatus(GlobalTerminalNet *net);
BOOL ov39_02227B1C(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL ov39_02227B20(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL GlobalTerminalNet_SaveUploadedVideoNumber(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL ov39_02227B50(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL ov39_02227B54(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL ov39_02227B58(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
BOOL GlobalTerminalNet_HandleResponse(GlobalTerminalNet *net);
void GlobalTerminalNet_PrintMessage(GlobalTerminalNet *net, String *message);
void GlobalTerminalNet_ShowErrorMessage(GlobalTerminalNet *net, int msgId, int errorNumber);
BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net);
u32 GlobalTerminalNet_GetSequence(GlobalTerminalNet *net);
void *GlobalTerminalNet_CreateOsHeap(enum HeapID heapID);
void GlobalTerminalNet_ClearOsHeap(void);
BOOL GlobalTerminalNet_CheckDressUpUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckDressUpDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckBoxUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckBoxDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_Check22000Result(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckRankingsResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckVideoUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckVideoSearchResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_CheckVideoDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);
BOOL GlobalTerminalNet_Check23003Result(GlobalTerminalNet *net, GlobalTerminalNetResponse *response);

// 20001 response: list of dress-up photos
typedef struct GlobalTerminalNetDressUpEntry {
    u8 unk_00[12];
    u8 photo[0xEC - 12];
} GlobalTerminalNetDressUpEntry;

typedef struct GlobalTerminalNetDressUpList {
    int count;
    GlobalTerminalNetDressUpEntry entries[1];
} GlobalTerminalNetDressUpList;

typedef struct GlobalTerminalNetDressUpListResponse {
    int unk_00;
    GlobalTerminalNetDressUpList list;
} GlobalTerminalNetDressUpListResponse;

// 21001 response: list of boxes
typedef struct GlobalTerminalNetBoxEntry {
    u8 unk_00[16];
    u8 box[0x21C];
} GlobalTerminalNetBoxEntry;

typedef struct GlobalTerminalNetBoxListResponse {
    int unk_00;
    int count;
    GlobalTerminalNetBoxEntry entries[1];
} GlobalTerminalNetBoxListResponse;

// 22000 / 22001 responses
typedef struct GlobalTerminalNetBlockResponse {
    u32 unk_00;
    u32 data;
    u32 unk_08;
} GlobalTerminalNetBlockResponse;

// 23000 response
typedef struct GlobalTerminalNetVideoNumberResponse {
    u32 unk_00;
    u32 numberLow;
    u32 numberHigh;
} GlobalTerminalNetVideoNumberResponse;

// 23001 response: list of battle video headers
typedef struct GlobalTerminalNetVideoEntry {
    u32 unk_00;
    u64 number;
    GlobalTerminalNetVideoHeader header;
} GlobalTerminalNetVideoEntry;

typedef struct GlobalTerminalNetVideoList {
    int count;
    GlobalTerminalNetVideoEntry entries[1];
} GlobalTerminalNetVideoList;

typedef struct GlobalTerminalNetVideoListResponse {
    u8 unk_00[4];
    GlobalTerminalNetVideoList list;
} GlobalTerminalNetVideoListResponse;

// 23002 response: one battle video
typedef struct GlobalTerminalNetVideoResponse {
    u64 unk_00;
    u64 number;
    u8 unk_10[0xD8];
    u64 savedNumber;
} GlobalTerminalNetVideoResponse;

typedef struct GlobalTerminalNetVideoView {
    u8 unk_00[0xE4];
    u8 savedNumber[8];
} GlobalTerminalNetVideoView;

void GlobalTerminalNet_PackDressUpPhoto(SaveData *saveData, const GlobalTerminalNetDressUpRecord *record, GlobalTerminalNetDressUpForm *form) {
    GlobalTerminalNet_DressUpToUploadForm(record, form);
    form->crc = SaveArray_CalcCRC16(saveData, form, 0x5C);
}

void GlobalTerminalNet_UnpackDressUpPhoto(const GlobalTerminalNetDressUpForm *form, GlobalTerminalNetDressUpRecord *record) {
    GlobalTerminalNet_UploadFormToDressUp(form, record);
}

#pragma force_active on

void GlobalTerminalNet_PackBoxData(SaveData *saveData, PCStorage *storage, int box, GlobalTerminalNetBoxForm *form, enum HeapID heapID) {
    String *name;
    int i;
    u8 *halves;
    u8 *words;

    MI_CpuFill8(form, 0, sizeof(GlobalTerminalNetBoxForm));
    name = String_New(60, heapID);
    PCStorage_GetBoxName(storage, box, name);
    CopyStringToU16Array(name, form->name, 20);
    String_Delete(name);
    // walks two byte pointers (2- and 4-byte strides) instead of indexing the arrays
    for (i = 0, halves = (u8 *)form, words = (u8 *)form; i < 30; i++, halves += 2, words += 4) {
        *(u16 *)(halves + 40) = PCStorage_GetMonDataByIndexPair(storage, box, i, 5, NULL);
        *(u32 *)(words + 100) = PCStorage_GetMonDataByIndexPair(storage, box, i, 0, NULL);
        *(u32 *)(words + 220) = PCStorage_GetMonDataByIndexPair(storage, box, i, 7, NULL);
        if (PCStorage_GetMonDataByIndexPair(storage, box, i, 174, NULL) == 494) {
            form->eggFlags |= 1 << i;
        }
        form->unk_158[i] = PCStorage_GetMonDataByIndexPair(storage, box, i, 112, NULL);
    }
    form->wallpaper = PCStorage_GetBoxWallpaper(storage, box);
    form->unk_177 = 0;
    form->crc = SaveArray_CalcCRC16(saveData, form, 408);
}

#pragma force_active reset

void GlobalTerminalNet_SealBoxData(SaveData *saveData, GlobalTerminalNetBoxForm *form, int value) {
    form->unk_177 = value;
    form->crc = SaveArray_CalcCRC16(saveData, form, 0x198);
}

void GlobalTerminalNet_SetRequestHeader(SaveData *saveData, const void *src, void *dst) {
    MI_CpuFill8(dst, 0, 0x80);
    MI_CpuCopy8(src, dst, 0x80);
}

void GlobalTerminalNet_CopyRequestHeader(const void *src, void *dst) {
    MI_CpuCopy8(src, dst, 0x80);
}

#pragma force_active on

void GlobalTerminalNet_SetRankingsHeader(SaveData *saveData, const GlobalTerminalNetRankingsSource *src, GlobalTerminalNetRankingsHeader *dst) {
    MI_CpuFill8(dst, 0, sizeof(GlobalTerminalNetRankingsHeader));
    dst->unk_00 = src->unk_19;
    dst->unk_01 = src->unk_1A;
    dst->unk_02 = src->unk_15;
    dst->unk_03 = src->unk_16;
    dst->unk_04 = src->unk_1C;
    dst->unk_06_0 = src->unk_1B_0;
}

BOOL GlobalTerminalNet_Init(GlobalTerminalNet *net, GlobalTerminalNetArgs *args) {
    MI_CpuFill8(net, 0, sizeof(GlobalTerminalNet));
    net->heapID = args->heapID;
    net->saveData = args->saveData;
    net->callbacks = args->callbacks;
    net->messageArg = args->messageArg;
    net->messageFunc = args->messageFunc;
    net->prevStatus = -1;
    net->pendingCommand = GTS_REQUEST_NONE;
    net->activeCommand = GTS_REQUEST_NONE;
    net->responseBuffer = Heap_Alloc(args->heapID, GlobalTerminalServer_GetResponseBufferSize(-1));
    MI_CpuFill8(net->responseBuffer, 0, GlobalTerminalServer_GetResponseBufferSize(-1));
    net->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, args->heapID);
    net->msgFormat = MessageFormat_New(args->heapID);
    net->string = String_New(256, args->heapID);
    net->osHeapMemory = GlobalTerminalNet_CreateOsHeap(args->heapID);
    net->login.dwcProfileId = args->dwcProfileId;
    net->login.gameVersion = GAME_VERSION;
    net->login.unk_05 = 2;
    GF_ASSERT(GlobalTerminalServer_Init(&net->login) == TRUE);
    net->isRunning = 1;
    net->unk_3B8 = 1;
    return TRUE;
}

void GlobalTerminalNet_Free(GlobalTerminalNet *net) {
    GlobalTerminalServer_Stop();
    net->isRunning = 0;
    String_Delete(net->string);
    MessageFormat_Delete(net->msgFormat);
    DestroyMsgData(net->msgData);
    Heap_Free(net->responseBuffer);
    GlobalTerminalNet_ClearOsHeap();
    Heap_Free(net->osHeapMemory);
}

#pragma force_active reset

BOOL GlobalTerminalNet_SeqStopClient(GlobalTerminalNet *net) {
    GlobalTerminalServer_Stop();
    net->isRunning = 0;
    return TRUE;
}

BOOL GlobalTerminalNet_SeqDisconnect(GlobalTerminalNet *net) {
    ov00_021EC210();
    ov00_021EC8D8();
    CommState_FreeWfcSession();
    net->unk_3C8 = 0;
    return TRUE;
}

BOOL GlobalTerminalNet_SeqShowError(GlobalTerminalNet *net) {
    int msgId = WifiConnection_ClassifyErrorCode(-net->errorCode, net->errorType);
    GlobalTerminalNet_ShowErrorMessage(net, msgId, -net->errorCode);
    return TRUE;
}

BOOL GlobalTerminalNet_SeqWaitButton(GlobalTerminalNet *net) {
    u32 keys = gSystem.newKeys;
    if ((keys & PAD_BUTTON_A) || (keys & PAD_BUTTON_B)) {
        GlobalTerminalNet_PrintMessage(net, NULL);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL GlobalTerminalNet_RequestDressUpUpload(GlobalTerminalNet *net, const void *header, const GlobalTerminalNetDressUpRecord *record) {
    if (GlobalTerminalNet_IsIdle(net) == FALSE) {
        return FALSE;
    }
    GlobalTerminalNet_SetRequestHeader(net->saveData, header, net->request.upload.header);
    GlobalTerminalNet_PackDressUpPhoto(net->saveData, record, &net->request.upload.form.dressUp);
    net->sendDelay = 60;
    net->pendingCommand = GTS_REQUEST_DRESS_UP_UPLOAD;
    return TRUE;
}

#pragma force_active reset

BOOL GlobalTerminalNet_RequestDressUpDownload(GlobalTerminalNet *net, u16 arg) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    net->requestArg.half = arg;
    net->pendingCommand = GTS_REQUEST_DRESS_UP_DOWNLOAD;
    return TRUE;
}

#pragma force_active on

BOOL GlobalTerminalNet_RequestBoxUpload(GlobalTerminalNet *net, int value, const void *header, PCStorage *storage, int box) {
    if (GlobalTerminalNet_IsIdle(net) == FALSE) {
        return FALSE;
    }
    net->requestArg.bytes[0] = value;
    net->requestArg.bytes[1] = box;
    GlobalTerminalNet_SetRequestHeader(net->saveData, header, net->request.upload.header);
    GlobalTerminalNet_PackBoxData(net->saveData, storage, box, &net->request.upload.form.box, net->heapID);
    GlobalTerminalNet_SealBoxData(net->saveData, &net->request.upload.form.box, value);
    net->sendDelay = 60;
    net->pendingCommand = GTS_REQUEST_BOX_UPLOAD;
    return TRUE;
}

#pragma force_active reset

BOOL GlobalTerminalNet_RequestBoxDownload(GlobalTerminalNet *net, u8 arg) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    net->requestArg.bytes[0] = arg;
    net->pendingCommand = GTS_REQUEST_BOX_DOWNLOAD;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestRankingsHeader(GlobalTerminalNet *net) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    net->pendingCommand = GTS_REQUEST_22000;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestRankings(GlobalTerminalNet *net, const GlobalTerminalNetRankingsSource *src, const GlobalTerminalNetPair *pairs) {
    s32 i;

    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuCopy8(Save_PlayerData_GetIGTAddr(net->saveData), &net->request.rankings.playTime, 4);
    GlobalTerminalNet_SetRankingsHeader(net->saveData, src, &net->request.rankings.header);
    for (i = 0; i < 3; i++) {
        net->request.rankings.pairs[i] = pairs[i];
    }
    net->pendingCommand = GTS_REQUEST_RANKINGS;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestVideoUpload(GlobalTerminalNet *net, const void *header) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    if (BattleRecord_sizeof() - 4 != 0x1D4C) {
        GF_AssertFail();
    }
    net->request.video = sub_0202FC74();
    VsRecorder_EncryptRecord(net->saveData);
    MI_CpuCopy8(header, BattleRecord_GetProfile(), 0x80);
    net->sendDelay = 60;
    net->pendingCommand = GTS_REQUEST_VIDEO_UPLOAD;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestVideoSearch(GlobalTerminalNet *net, u16 arg0, u8 arg1, u8 arg2, u8 arg3) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuFill8(&net->request, 0, sizeof(GlobalTerminalNetVideoSearch));
    net->request.search.unk_00 = arg0;
    net->request.search.unk_02 = arg1;
    net->request.search.unk_03 = arg2;
    net->request.search.unk_04 = arg3;
    net->request.search.unk_08 = 0x140;
    net->searchKind = 0;
    net->pendingCommand = GTS_REQUEST_VIDEO_SEARCH;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestDefaultVideoSearchNoFrontier(GlobalTerminalNet *net) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuFill8(&net->request, 0, sizeof(GlobalTerminalNetVideoSearch));
    net->request.search.unk_00 = 0xFFFF;
    net->request.search.unk_02 = 0xFE;
    net->request.search.unk_03 = 0xFF;
    net->request.search.unk_04 = 0xFF;
    net->request.search.unk_08 = 0x140;
    net->searchKind = 0;
    net->pendingCommand = GTS_REQUEST_VIDEO_SEARCH;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestDefaultVideoSearch(GlobalTerminalNet *net) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuFill8(&net->request, 0, sizeof(GlobalTerminalNetVideoSearch));
    net->request.search.unk_00 = 0xFFFF;
    net->request.search.unk_02 = 0xFF;
    net->request.search.unk_03 = 0xFF;
    net->request.search.unk_04 = 0xFF;
    net->request.search.unk_08 = 0x140;
    net->searchKind = 0;
    net->pendingCommand = GTS_REQUEST_VIDEO_SEARCH;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestLatestVideos(GlobalTerminalNet *net) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuFill8(&net->request.unk_00, 0, 4);
    net->request.unk_00 = 0x140;
    net->searchKind = 1;
    net->pendingCommand = GTS_REQUEST_VIDEO_SEARCH;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestPopularVideos(GlobalTerminalNet *net) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    MI_CpuFill8(&net->request.unk_00, 0, 4);
    net->request.unk_00 = 0x140;
    net->searchKind = 2;
    net->pendingCommand = GTS_REQUEST_VIDEO_SEARCH;
    return TRUE;
}

BOOL GlobalTerminalNet_RequestVideoDownload(GlobalTerminalNet *net, u32 numberLow, u32 numberHigh) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    net->requestArg.word = numberLow;
    net->requestArgHigh = numberHigh;
    net->pendingCommand = GTS_REQUEST_VIDEO_DOWNLOAD;
    return TRUE;
}

BOOL GlobalTerminalNet_ReportVideoSaved(GlobalTerminalNet *net, u32 argLow, u32 argHigh) {
    if (!GlobalTerminalNet_IsIdle(net)) {
        return FALSE;
    }
    net->requestArg.word = argLow;
    net->requestArgHigh = argHigh;
    net->pendingCommand = GTS_REQUEST_23003;
    return TRUE;
}

BOOL GlobalTerminalNet_Update(GlobalTerminalNet *net) {
    const GlobalTerminalNetStepFunc *sequence;

    if (net->isRunning == 1) {
        if (net->pendingCommand != GTS_REQUEST_NONE) {
            if (net->sendDelay != 0) {
                net->sendDelay--;
            } else {
                switch (GlobalTerminalServer_GetState()) {
                case 1:
                case 7:
                case 8:
                case 9:
                    if (GlobalTerminalNet_SendPendingRequest(net) == TRUE) {
                        net->pendingCommand = GTS_REQUEST_NONE;
                    }
                    break;
                }
            }
        }
        GlobalTerminalNet_PollStatus(net);
        if (net->completeFunc != NULL) {
            if (net->completeFunc(net, &net->responseFunc) == TRUE) {
                net->completeFunc = NULL;
                if (net->responseFunc != NULL) {
                    net->responseFunc(net->callbacks.arg, &net->error);
                    net->responseFunc = NULL;
                }
                MI_CpuFill8(&net->responseFunc, 0, 16);
                net->activeCommand = GTS_REQUEST_NONE;
            }
        }
    }

    sequence = sGlobalTerminalNetSequences[net->sequence];
    if (sequence != NULL) {
        switch (sequence[net->sequenceStep](net, &net->stepWork)) {
        case 0:
            break;
        case 1:
            MI_CpuFill8(&net->stepWork, 0, sizeof(GlobalTerminalNetStepWork));
            net->sequenceStep++;
            MI_CpuFill8(&net->stepWork, 0, sizeof(GlobalTerminalNetStepWork));
            if (sGlobalTerminalNetSequences[net->sequence][net->sequenceStep] == NULL) {
                net->sequenceStep = 0;
                net->sequence = 0;
            }
            break;
        case 2:
            MI_CpuFill8(&net->stepWork, 0, sizeof(GlobalTerminalNetStepWork));
            net->sequenceStep = 0;
            break;
        }
    }
    return TRUE;
}

BOOL GlobalTerminalNet_SendPendingRequest(GlobalTerminalNet *net) {
    BOOL result = FALSE;

    switch (net->pendingCommand) {
    case GTS_REQUEST_DRESS_UP_UPLOAD:
        result = GlobalTerminalServer_UploadDressUp(&net->request, net->responseBuffer);
        break;
    case GTS_REQUEST_DRESS_UP_DOWNLOAD:
        result = GlobalTerminalServer_DownloadDressUp(net->requestArg.half, net->responseBuffer);
        break;
    case GTS_REQUEST_BOX_UPLOAD:
        result = GlobalTerminalServer_UploadBox(net->requestArg.bytes[0], &net->request, net->responseBuffer);
        break;
    case GTS_REQUEST_BOX_DOWNLOAD:
        result = GlobalTerminalServer_DownloadBox(net->requestArg.bytes[0], net->responseBuffer);
        break;
    case GTS_REQUEST_22000:
        result = ov39_0222A2CC(net->responseBuffer);
        break;
    case GTS_REQUEST_RANKINGS:
        result = GlobalTerminalServer_GetRankings(&net->request, net->responseBuffer);
        break;
    case GTS_REQUEST_VIDEO_UPLOAD:
        result = GlobalTerminalServer_UploadVideo(net->request.video, net->responseBuffer);
        break;
    case GTS_REQUEST_VIDEO_SEARCH:
        switch (net->searchKind) {
        case 0:
            result = GlobalTerminalServer_SearchVideos(&net->request, net->responseBuffer);
            break;
        case 1:
            result = ov39_0222A3DC(&net->request, net->responseBuffer);
            break;
        case 2:
            result = ov39_0222A434(&net->request, net->responseBuffer);
            break;
        }
        break;
    case GTS_REQUEST_VIDEO_DOWNLOAD:
        result = GlobalTerminalServer_DownloadVideo(net->requestArg.word, net->requestArgHigh, 0x140, net->responseBuffer);
        break;
    case GTS_REQUEST_23003:
        result = ov39_0222A4C0(net->requestArg.word, net->requestArgHigh, net->responseBuffer);
        break;
    default:
        GF_AssertFail();
        break;
    }

    if (result == TRUE) {
        net->activeCommand = net->pendingCommand;
    }
    return result;
}

BOOL GlobalTerminalNet_PollStatus(GlobalTerminalNet *net) {
    if (net->activeCommand == GTS_REQUEST_NONE) {
        return TRUE;
    }
    net->status = GlobalTerminalServer_GetState();
    if (net->status != net->prevStatus) {
        switch (net->status) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            break;
        case 7:
            net->error.type = 1;
            net->error.command = 0;
            net->error.code = 7;
            net->error.isError = 1;
            net->completeFunc = ov39_02227B58;
            break;
        case 8:
            net->completeFunc = ov39_02227B1C;
            GlobalTerminalNet_HandleResponse(net);
            break;
        case 9:
            net->error.type = 0;
            net->error.command = 0;
            net->error.code = GlobalTerminalServer_GetError();
            net->error.isError = 1;
            net->completeFunc = ov39_02227B58;
            break;
        }
        net->prevStatus = net->status;
    }
    return TRUE;
}

BOOL ov39_02227B1C(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    return TRUE;
}

BOOL ov39_02227B20(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    return TRUE;
}

#pragma force_active on

// stores the server's number for the uploaded video in the recording and saves it; done when the save finished
BOOL GlobalTerminalNet_SaveUploadedVideoNumber(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    GlobalTerminalNetVideoNumberResponse *response = GlobalTerminalServer_GetResponse(net);
    int result = VsRecorder_SaveRecordNumber(net->saveData, response->numberLow, response->numberHigh, (u8 *)responseFunc + 4, (u8 *)responseFunc + 6);
    return (u32)(result - 2) <= 1;
}

#pragma force_active reset

BOOL ov39_02227B50(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    return TRUE;
}

BOOL ov39_02227B54(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    return TRUE;
}

BOOL ov39_02227B58(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc) {
    return TRUE;
}

#pragma force_active on

BOOL GlobalTerminalNet_HandleResponse(GlobalTerminalNet *net) {
    BOOL result = TRUE;
    GlobalTerminalNetResponse *response = GlobalTerminalServer_GetResponse(net);

    GlobalTerminalServer_GetResponseSize(response);
    GF_ASSERT(net->activeCommand == response->command);
    net->responseFunc = NULL;
    switch (response->command) {
    case 0:
        break;
    case GTS_REQUEST_DRESS_UP_UPLOAD:
        result = GlobalTerminalNet_CheckDressUpUploadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[0];
        if (!result) {
            net->completeFunc = ov39_02227B50;
        }
        break;
    case GTS_REQUEST_DRESS_UP_DOWNLOAD:
        result = GlobalTerminalNet_CheckDressUpDownloadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[1];
        break;
    case GTS_REQUEST_BOX_UPLOAD:
        result = GlobalTerminalNet_CheckBoxUploadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[2];
        if (!result) {
            net->completeFunc = ov39_02227B54;
        }
        break;
    case GTS_REQUEST_BOX_DOWNLOAD:
        result = GlobalTerminalNet_CheckBoxDownloadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[3];
        break;
    case GTS_REQUEST_22000:
        result = GlobalTerminalNet_Check22000Result(net, response);
        net->responseFunc = net->callbacks.onResponse[4];
        break;
    case GTS_REQUEST_RANKINGS:
        result = GlobalTerminalNet_CheckRankingsResult(net, response);
        net->responseFunc = net->callbacks.onResponse[5];
        break;
    case GTS_REQUEST_VIDEO_UPLOAD:
        result = GlobalTerminalNet_CheckVideoUploadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[6];
        if (!result) {
            net->completeFunc = ov39_02227B20;
        } else {
            net->completeFunc = GlobalTerminalNet_SaveUploadedVideoNumber;
        }
        break;
    case GTS_REQUEST_VIDEO_SEARCH:
        result = GlobalTerminalNet_CheckVideoSearchResult(net, response);
        net->responseFunc = net->callbacks.onResponse[7];
        break;
    case GTS_REQUEST_VIDEO_DOWNLOAD:
        result = GlobalTerminalNet_CheckVideoDownloadResult(net, response);
        net->responseFunc = net->callbacks.onResponse[8];
        break;
    case GTS_REQUEST_23003:
        result = GlobalTerminalNet_Check23003Result(net, response);
        net->responseFunc = net->callbacks.onResponse[9];
        break;
    }
    if (!result) {
        net->error.type = 2;
        net->error.command = response->command;
        net->error.code = response->result;
        net->error.isError = 1;
    } else {
        net->error.isError = 0;
    }
    return result;
}

#pragma force_active reset

u32 GlobalTerminalNet_GetError(GlobalTerminalNet *net, GlobalTerminalNetError **error) {
    *error = &net->error;
    return net->error.isError;
}

void GlobalTerminalNet_PrintMessage(GlobalTerminalNet *net, String *message) {
    net->messageFunc(net->messageArg, message);
}

#pragma force_active on

void GlobalTerminalNet_ShowErrorMessage(GlobalTerminalNet *net, int msgId, int errorNumber) {
    String *string;

    if (msgId == -1) {
        msgId = 11;
    }
    BufferIntegerAsString(net->msgFormat, 0, errorNumber, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    string = NewString_ReadMsgData(net->msgData, msgId);
    StringExpandPlaceholders(net->msgFormat, net->string, string);
    String_Delete(string);
    GlobalTerminalNet_PrintMessage(net, net->string);
}

#pragma force_active reset

BOOL GlobalTerminalNet_IsIdle(GlobalTerminalNet *net) {
    if (GlobalTerminalNet_GetSequence(net) == 0 && net->pendingCommand == GTS_REQUEST_NONE && net->activeCommand == GTS_REQUEST_NONE) {
        return TRUE;
    }
    return FALSE;
}

u32 GlobalTerminalNet_GetSequence(GlobalTerminalNet *net) {
    return net->sequence;
}

void *GlobalTerminalNet_CreateOsHeap(enum HeapID heapID) {
    void *memory = Heap_Alloc(heapID, 0x2000);
    void *end = (u8 *)memory + 0x2000;
    void *arenaLo = OS_InitAlloc(OS_ARENA_MAIN, memory, end, 1);

    OS_SetArenaLo(OS_ARENA_MAIN, arenaLo);
    arenaLo = (void *)(((u32)arenaLo + 31) & ~31);
    end = (void *)(((u32)end + 31) & ~31);
    sGlobalTerminalNetOsHeap = OS_CreateHeap(OS_ARENA_MAIN, arenaLo, end);
    OS_SetCurrentHeap(OS_ARENA_MAIN, sGlobalTerminalNetOsHeap);
    return memory;
}

void GlobalTerminalNet_ClearOsHeap(void) {
    OS_ClearAlloc(OS_ARENA_MAIN);
}

// every result check has a case list with the server's error results, all of which fall through to FALSE
BOOL GlobalTerminalNet_CheckDressUpUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    }
    return ret;
}

BOOL GlobalTerminalNet_CheckDressUpDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
        break;
    }
    return ret;
}

#pragma force_active on

int GlobalTerminalNet_GetDressUpList(GlobalTerminalNet *net, void **out, int max) {
    GlobalTerminalNetDressUpList *list = &((GlobalTerminalNetDressUpListResponse *)GlobalTerminalServer_GetResponse(net))->list;
    int count = list->count;
    int i;
    GlobalTerminalNetDressUpEntry *entry;

    if (count > max) {
        count = max;
    }
    entry = list->entries;
    i = 0;
    if (count > 0) {
        do {
            out[i] = entry->photo;
            i++;
            entry++;
        } while (i < count);
    }
    for (; i < max; i++) {
        out[i] = NULL;
    }
    return count;
}

#pragma force_active reset

BOOL GlobalTerminalNet_CheckBoxUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    }
    return ret;
}

BOOL GlobalTerminalNet_CheckBoxDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
        break;
    }
    return ret;
}

#pragma force_active on

int GlobalTerminalNet_GetBoxList(GlobalTerminalNet *net, void **out, int max) {
    int count;
    int *base = &((GlobalTerminalNetBoxListResponse *)GlobalTerminalServer_GetResponse(net))->count;
    int i;
    GlobalTerminalNetBoxEntry *entry;

    count = *base;
    if (count > max) {
        count = max;
        GF_AssertFail();
    }
    entry = (GlobalTerminalNetBoxEntry *)(base + 1);
    i = 0;
    if (i < count) {
        void **dst = out;
        do {
            *dst++ = entry->box;
            i++;
            entry++;
        } while (i < count);
    }
    if (i < max) {
        void **dst = out + i;
        do {
            *dst++ = NULL;
            i++;
        } while (i < max);
    }
    return count;
}

#pragma force_active reset

BOOL GlobalTerminalNet_Check22000Result(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
        break;
    }
    return ret;
}

void GlobalTerminalNet_GetRankingsHeader(GlobalTerminalNet *net, void **out) {
    GlobalTerminalNetBlockResponse *response = GlobalTerminalServer_GetResponse(net);
    *out = &response->data;
}

BOOL GlobalTerminalNet_CheckRankingsResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    }
    return ret;
}

void GlobalTerminalNet_GetRankings(GlobalTerminalNet *net, void **out1, void **out2) {
    u8 *data = (u8 *)&((GlobalTerminalNetBlockResponse *)GlobalTerminalServer_GetResponse(net))->data;
    *out1 = data;
    *out2 = data + 0x558;
}

BOOL GlobalTerminalNet_CheckVideoUploadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return ret;
}

#pragma force_active on

u64 GlobalTerminalNet_GetUploadedVideoNumber(GlobalTerminalNet *net) {
    GlobalTerminalNetVideoNumberResponse *response = GlobalTerminalServer_GetResponse(net);
    return *(u64 *)&response->numberLow;
}

#pragma force_active reset

BOOL GlobalTerminalNet_CheckVideoSearchResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
        break;
    }
    return ret;
}

#pragma force_active on

int GlobalTerminalNet_GetVideoList(GlobalTerminalNet *net, GlobalTerminalNetVideoHeader **out, int max) {
    GlobalTerminalNetVideoList *list = &((GlobalTerminalNetVideoListResponse *)GlobalTerminalServer_GetResponse(net))->list;
    int count = list->count;
    GlobalTerminalNetVideoEntry *entry;
    int i;

    if (count > max) {
        count = max;
        GF_AssertFail();
    }
    entry = list->entries;
    for (i = 0; i < count; i++) {
        out[i] = &entry->header;
        if (out[i]->number != entry->number) {
            out[i]->number = entry->number;
            out[i]->crc = SaveArray_CalcCRC16(net->saveData, out[i]->data, sizeof(out[i]->data));
        }
        entry++;
    }
    for (; i < max; i++) {
        out[i] = NULL;
    }
    return count;
}

#pragma force_active reset

BOOL GlobalTerminalNet_CheckVideoDownloadResult(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
        break;
    }
    return ret;
}

#pragma force_active on

u32 GlobalTerminalNet_GetDownloadedVideo(GlobalTerminalNet *net, u8 **out) {
    GlobalTerminalNetVideoResponse *response = GlobalTerminalServer_GetResponse(net);
    GlobalTerminalNetVideoView *view = (GlobalTerminalNetVideoView *)((u8 *)response + 4);

    *out = (u8 *)((u32)view + 12);
    if (*(u64 *)view->savedNumber != response->number) {
        *(u64 *)view->savedNumber = response->number;
        *(u16 *)(*out + 0xE0) = SaveArray_CalcCRC16(net->saveData, *out + 0x80, 0x58);
    }
    return response->number; // low word only
}

#pragma force_active reset

BOOL GlobalTerminalNet_Check23003Result(GlobalTerminalNet *net, GlobalTerminalNetResponse *response) {
    BOOL ret = FALSE;
    switch (response->result) {
    case 0:
        ret = TRUE;
        break;
    case 1:
    case 2:
    case 3:
        break;
    }
    return ret;
}
