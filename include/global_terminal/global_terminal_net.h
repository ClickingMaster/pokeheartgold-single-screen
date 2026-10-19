#ifndef POKEHEARTGOLD_GLOBAL_TERMINAL_NET_H
#define POKEHEARTGOLD_GLOBAL_TERMINAL_NET_H

#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "save.h"

// Global Terminal server requests (the command id is also the first u16 of the response)
#define GTS_REQUEST_DRESS_UP_UPLOAD   20000
#define GTS_REQUEST_DRESS_UP_DOWNLOAD 20001
#define GTS_REQUEST_BOX_UPLOAD        21000
#define GTS_REQUEST_BOX_DOWNLOAD      21001
#define GTS_REQUEST_22000             22000
#define GTS_REQUEST_RANKINGS          22001
#define GTS_REQUEST_VIDEO_UPLOAD      23000
#define GTS_REQUEST_VIDEO_SEARCH      23001
#define GTS_REQUEST_VIDEO_DOWNLOAD    23002
#define GTS_REQUEST_23003             23003
#define GTS_REQUEST_NONE              23004

enum GlobalTerminalSocketState {
    GTS_SOCKET_STATE_NONE,
    GTS_SOCKET_STATE_READY,
    GTS_SOCKET_STATE_FETCH_TOKEN,
    GTS_SOCKET_STATE_OPEN,
    GTS_SOCKET_STATE_CONNECT,
    GTS_SOCKET_STATE_SEND,
    GTS_SOCKET_STATE_RECEIVE,
    GTS_SOCKET_STATE_CANCELLED,
    GTS_SOCKET_STATE_DONE,
    GTS_SOCKET_STATE_ERROR,
};

typedef struct GlobalTerminalNet GlobalTerminalNet;

typedef struct GlobalTerminalNetError {
    u32 isError;
    u32 type; // 0 = server status, 1 = cancelled, 2 = request result
    u32 command;
    u32 code;
} GlobalTerminalNetError;

typedef void (*GlobalTerminalNetResponseFunc)(void *arg, GlobalTerminalNetError *error);
typedef BOOL (*GlobalTerminalNetCompleteFunc)(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
typedef int (*GlobalTerminalNetStepFunc)(GlobalTerminalNet *net, void *stepWork);
typedef void (*GlobalTerminalNetMessageFunc)(void *arg, String *message);

typedef struct GlobalTerminalNetCallbacks {
    void *arg;
    GlobalTerminalNetResponseFunc onResponse[10]; // one per request, in command order
} GlobalTerminalNetCallbacks;

typedef struct GlobalTerminalNetArgs {
    u32 dwcProfileId;
    enum HeapID heapID;
    SaveData *saveData;
    GlobalTerminalNetCallbacks callbacks;
    GlobalTerminalNetMessageFunc messageFunc;
    void *messageArg;
} GlobalTerminalNetArgs;

// login block handed to the server client (GlobalTerminalServer_Init)
typedef struct GlobalTerminalNetLogin {
    u32 dwcProfileId;
    u8 gameVersion;
    u8 unk_05; // always 2
    u8 token[0x132];
} GlobalTerminalNetLogin;

// dress-up photo as sent to the server (SaveFashionDataSub without its unsent parts)
typedef struct GlobalTerminalNetDressUpForm {
    u32 magic;
    u32 personality;
    u32 otId;
    u16 species;
    u16 secondText[8];
    s8 z;
    u8 x;
    u8 y;
    u8 form;
    u8 unk_22[2];
    u32 itemMask;
    u16 mailMsgBank;
    u16 mailMsgNo;
    u16 mailField0;
    u16 mailField1;
    u8 items[40];
    u8 backdrop;
    u8 language;
    u16 unk_5A;
    u16 crc;
    u16 unk_5E;
} GlobalTerminalNetDressUpForm;

// the layout of SaveFashionDataSub
typedef struct GlobalTerminalNetDressUpRecord {
    u32 magic;
    u32 personality;
    u32 otId;
    u16 species;
    u8 nickname[0x24 - 0x0E];
    u16 secondText[8];
    s8 z;
    u8 x;
    u8 y;
    u8 form;
    u8 unk_38[4];
    u32 itemMask;
    u16 mailMsgBank;
    u16 mailMsgNo;
    u16 mailField0;
    u16 mailField1;
    u8 items[40];
    u8 backdrop;
    u8 language;
} GlobalTerminalNetDressUpRecord;

// box as sent to the server
typedef struct GlobalTerminalNetBoxForm {
    u16 name[20];
    u16 species[30];
    u32 personality[30];
    u32 otIds[30];
    u32 eggFlags; // bit per slot
    u8 forms[30];
    u8 wallpaper;
    u8 category;
    u8 unk_178[0x198 - 0x178];
    u16 crc;
    u8 unk_19A[2];
} GlobalTerminalNetBoxForm;

typedef struct GlobalTerminalNetPair {
    u32 recordId;
    u32 value;
} GlobalTerminalNetPair;

// header of the rankings request, built by GlobalTerminalNet_SetRankingsHeader
typedef struct GlobalTerminalNetRankingsHeader {
    u8 version;
    u8 unk_01;
    u8 birthdayMonth;
    u8 avatar;
    u16 species;
    u8 isEgg : 1;
    u8 unk_06_1 : 7;
    u8 unk_07;
} GlobalTerminalNetRankingsHeader;

typedef struct GlobalTerminalNetRankingsSource {
    u8 unk_00[0x15];
    u8 birthdayMonth;
    u8 avatar;
    u8 unk_17[2];
    u8 version;
    u8 unk_1A;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
} GlobalTerminalNetRankingsSource;

typedef struct GlobalTerminalNetVideoSearch {
    u16 species;
    u8 facility;
    u8 country;
    u8 region;
    u8 unk_05[3];
    u32 unk_08;
    u32 unk_0C;
} GlobalTerminalNetVideoSearch;

typedef union GlobalTerminalNetRequest {
    struct {
        u8 header[0x80];

        union {
            GlobalTerminalNetDressUpForm dressUp;
            GlobalTerminalNetBoxForm box;
        } form;
    } upload; // 20000, 21000

    struct {
        GlobalTerminalNetRankingsHeader header;
        u32 playTime;
        GlobalTerminalNetPair pairs[3];
    } rankings; // 22001

    void *video;                         // 23000
    GlobalTerminalNetVideoSearch search; // 23001 kind 0
    u32 unk_00;                          // 23001 kinds 1 and 2
} GlobalTerminalNetRequest;

typedef struct GlobalTerminalNetStepWork {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} GlobalTerminalNetStepWork;

struct GlobalTerminalNet {
    SaveData *saveData;
    GlobalTerminalNetMessageFunc messageFunc;
    void *messageArg;
    GlobalTerminalNetLogin login;
    enum HeapID heapID;
    int status;
    int prevStatus;
    GlobalTerminalNetCallbacks callbacks;
    GlobalTerminalNetError error;
    void *osHeapMemory;
    GlobalTerminalNetRequest request;

    union {
        u32 word;
        u16 half;
        u8 bytes[4];
    } requestArg;

    u32 requestArgHigh;
    void *responseBuffer;
    int initialized;
    u32 sequence;
    u32 sequenceStep;
    int isRunning;
    u32 unk_3C8;
    GlobalTerminalNetStepWork stepWork;
    u8 unk_3D8[8];
    int errorCode;
    int errorType;
    int pendingCommand;
    int activeCommand;
    u32 searchKind;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    GlobalTerminalNetResponseFunc responseFunc;
    u8 unk_404[8];
    GlobalTerminalNetCompleteFunc completeFunc;
    u8 unk_410;
    u8 sendDelay;
    u8 unk_412[2];
};

// battle video header in the 23001 response
typedef struct GlobalTerminalNetVideoHeader {
    u8 unk_00[0x80];
    u8 data[0x58];
    u64 number;
    u16 crc;
    u8 unk_E2[2];
} GlobalTerminalNetVideoHeader;

// first bytes of every response
typedef struct GlobalTerminalNetResponse {
    u16 command;
    u16 result;
} GlobalTerminalNetResponse;

void GlobalTerminalNet_UnpackDressUpPhoto(const GlobalTerminalNetDressUpForm *form, GlobalTerminalNetDressUpRecord *record);
void GlobalTerminalNet_PackBoxData(SaveData *saveData, PCStorage *storage, int box, GlobalTerminalNetBoxForm *form, enum HeapID heapID);
void GlobalTerminalNet_CopyRequestHeader(const void *src, void *dst);
void GlobalTerminalNet_SetRankingsHeader(SaveData *saveData, const GlobalTerminalNetRankingsSource *src, GlobalTerminalNetRankingsHeader *dst);
BOOL GlobalTerminalNet_Init(GlobalTerminalNet *net, GlobalTerminalNetArgs *args);
void GlobalTerminalNet_Free(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestDressUpUpload(GlobalTerminalNet *net, const void *header, const GlobalTerminalNetDressUpRecord *record);
BOOL GlobalTerminalNet_RequestDressUpDownload(GlobalTerminalNet *net, int arg);
BOOL GlobalTerminalNet_RequestBoxUpload(GlobalTerminalNet *net, int value, const void *header, PCStorage *storage, int box);
BOOL GlobalTerminalNet_RequestBoxDownload(GlobalTerminalNet *net, int arg);
BOOL GlobalTerminalNet_RequestRankingsHeader(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestRankings(GlobalTerminalNet *net, const GlobalTerminalNetRankingsSource *src, const GlobalTerminalNetPair *pairs);
BOOL GlobalTerminalNet_RequestVideoUpload(GlobalTerminalNet *net, const void *header);
BOOL GlobalTerminalNet_RequestVideoSearch(GlobalTerminalNet *net, u16 species, u8 facility, u8 country, u8 region);
BOOL GlobalTerminalNet_RequestDefaultVideoSearchNoFrontier(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestDefaultVideoSearch(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestLatestVideos(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestPopularVideos(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestVideoDownload(GlobalTerminalNet *net, u32 numberLow, u32 numberHigh);
BOOL GlobalTerminalNet_ReportVideoSaved(GlobalTerminalNet *net, u32 argLow, u32 argHigh);
BOOL GlobalTerminalNet_Update(GlobalTerminalNet *net);
u32 GlobalTerminalNet_GetError(GlobalTerminalNet *net, GlobalTerminalNetError **error);
u64 GlobalTerminalNet_GetUploadedVideoNumber(GlobalTerminalNet *net);
int GlobalTerminalNet_GetDressUpList(GlobalTerminalNet *net, void **out, int max);
int GlobalTerminalNet_GetBoxList(GlobalTerminalNet *net, void **out, int max);
void GlobalTerminalNet_GetRankingsHeader(GlobalTerminalNet *net, void **out);
void GlobalTerminalNet_GetRankings(GlobalTerminalNet *net, void **out1, void **out2);
u32 GlobalTerminalNet_GetDownloadedVideo(GlobalTerminalNet *net, u8 **out);
int GlobalTerminalNet_GetVideoList(GlobalTerminalNet *net, GlobalTerminalNetVideoHeader **out, int max);

// dress-up form converters
void GlobalTerminalNet_DressUpToUploadForm(const GlobalTerminalNetDressUpRecord *src, GlobalTerminalNetDressUpForm *dst);
void GlobalTerminalNet_UploadFormToDressUp(const GlobalTerminalNetDressUpForm *src, GlobalTerminalNetDressUpRecord *dst);

#endif // POKEHEARTGOLD_GLOBAL_TERMINAL_NET_H
