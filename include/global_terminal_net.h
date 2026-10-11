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

typedef struct GlobalTerminalNet GlobalTerminalNet;

typedef struct GlobalTerminalNetError {
    u32 isError;          // 0x00
    u32 type;             // 0x04, 0 = server status, 1 = cancelled, 2 = request result
    u32 command;          // 0x08
    u32 code;             // 0x0C
} GlobalTerminalNetError; // size 0x10

typedef void (*GlobalTerminalNetResponseFunc)(void *arg, GlobalTerminalNetError *error);
typedef BOOL (*GlobalTerminalNetCompleteFunc)(GlobalTerminalNet *net, GlobalTerminalNetResponseFunc *responseFunc);
typedef int (*GlobalTerminalNetStepFunc)(GlobalTerminalNet *net, void *stepWork);
typedef void (*GlobalTerminalNetMessageFunc)(void *arg, String *message);

typedef struct GlobalTerminalNetCallbacks {
    void *arg;                                    // 0x00
    GlobalTerminalNetResponseFunc onResponse[10]; // 0x04, one per request, in command order
} GlobalTerminalNetCallbacks;                     // size 0x2C

typedef struct GlobalTerminalNetArgs {
    u32 dwcProfileId;                         // 0x00
    enum HeapID heapID;                       // 0x04
    SaveData *saveData;                       // 0x08
    GlobalTerminalNetCallbacks callbacks;     // 0x0C
    GlobalTerminalNetMessageFunc messageFunc; // 0x38
    void *messageArg;                         // 0x3C
} GlobalTerminalNetArgs;                      // size 0x40

// login block handed to the server client (GlobalTerminalServer_Init)
typedef struct GlobalTerminalNetLogin {
    u32 dwcProfileId;     // 0x00
    u8 gameVersion;       // 0x04
    u8 unk_05;            // 0x05, always 2
    u8 unk_06[0x132];     // 0x06
} GlobalTerminalNetLogin; // size 0x138

// dress-up photo as sent to the server (SaveFashionDataSub without its unsent parts)
typedef struct GlobalTerminalNetDressUpForm {
    u32 unk_00;                 // 0x00
    u32 unk_04;                 // 0x04
    u32 unk_08;                 // 0x08
    u16 unk_0C;                 // 0x0C
    u16 unk_0E[8];              // 0x0E
    s8 unk_1E;                  // 0x1E
    u8 unk_1F;                  // 0x1F
    u8 unk_20;                  // 0x20
    u8 unk_21;                  // 0x21
    u8 unk_22[2];               // 0x22
    u32 unk_24;                 // 0x24
    u16 unk_28;                 // 0x28
    u16 unk_2A;                 // 0x2A
    u16 unk_2C;                 // 0x2C
    u16 unk_2E;                 // 0x2E
    u8 unk_30[40];              // 0x30
    u8 unk_58;                  // 0x58
    u8 unk_59;                  // 0x59
    u16 unk_5A;                 // 0x5A
    u16 crc;                    // 0x5C
    u16 unk_5E;                 // 0x5E
} GlobalTerminalNetDressUpForm; // size 0x60

// view of SaveFashionDataSub (include/fashion_case.h only declares fillers there)
typedef struct GlobalTerminalNetDressUpRecord {
    u32 unk_00;             // 0x00
    u32 unk_04;             // 0x04
    u32 unk_08;             // 0x08
    u16 unk_0C;             // 0x0C
    u8 unk_0E[0x24 - 0x0E]; // 0x0E, 11 u16
    u16 unk_24[8];          // 0x24
    s8 unk_34;              // 0x34
    u8 unk_35;              // 0x35
    u8 unk_36;              // 0x36
    u8 unk_37;              // 0x37
    u8 unk_38[4];           // 0x38
    u32 unk_3C;             // 0x3C
    u16 unk_40;             // 0x40
    u16 unk_42;             // 0x42
    u16 unk_44;             // 0x44
    u16 unk_46;             // 0x46
    u8 unk_48[40];          // 0x48
    u8 unk_70;              // 0x70
    u8 unk_71;              // 0x71
} GlobalTerminalNetDressUpRecord;

// box as sent to the server
typedef struct GlobalTerminalNetBoxForm {
    u16 name[20];              // 0x000
    u16 species[30];           // 0x028
    u32 personality[30];       // 0x064
    u32 unk_DC[30];            // 0x0DC, mon data 7
    u32 eggFlags;              // 0x154, bit per slot (mon data 174 == 494)
    u8 unk_158[30];            // 0x158, mon data 112
    u8 wallpaper;              // 0x176
    u8 unk_177;                // 0x177
    u8 unk_178[0x198 - 0x178]; // 0x178
    u16 crc;                   // 0x198
    u8 unk_19A[2];             // 0x19A
} GlobalTerminalNetBoxForm;    // size 0x19C

typedef struct GlobalTerminalNetPair {
    u32 unk_00;
    u32 unk_04;
} GlobalTerminalNetPair;

// 8-byte header of the rankings request, built from a 0x24-byte source by ov39_022271CC
typedef struct GlobalTerminalNetRankingsHeader {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04;
    u8 unk_06_0 : 1;
    u8 unk_06_1 : 7;
    u8 unk_07;
} GlobalTerminalNetRankingsHeader;

typedef struct GlobalTerminalNetRankingsSource {
    u8 unk_00[0x15];
    u8 unk_15;
    u8 unk_16;
    u8 unk_17[2];
    u8 unk_19;
    u8 unk_1A;
    u8 unk_1B_0 : 1;
    u8 unk_1B_1 : 7;
    u16 unk_1C;
} GlobalTerminalNetRankingsSource;

typedef struct GlobalTerminalNetVideoSearch {
    u16 unk_00;   // 0x00
    u8 unk_02;    // 0x02
    u8 unk_03;    // 0x03
    u8 unk_04;    // 0x04
    u8 unk_05[3]; // 0x05
    u32 unk_08;   // 0x08
    u32 unk_0C;   // 0x0C
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
} GlobalTerminalNetRequest;              // size 0x21C

typedef struct GlobalTerminalNetStepWork {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} GlobalTerminalNetStepWork;

struct GlobalTerminalNet {
    SaveData *saveData;                       // 0x000
    GlobalTerminalNetMessageFunc messageFunc; // 0x004
    void *messageArg;                         // 0x008
    GlobalTerminalNetLogin login;             // 0x00C
    enum HeapID heapID;                       // 0x144
    int status;                               // 0x148
    int prevStatus;                           // 0x14C
    GlobalTerminalNetCallbacks callbacks;     // 0x150
    GlobalTerminalNetError error;             // 0x17C
    void *osHeapMemory;                       // 0x18C
    GlobalTerminalNetRequest request;         // 0x190

    union {
        u32 word;
        u16 half;
        u8 bytes[4];
    } requestArg; // 0x3AC

    u32 requestArgHigh;                         // 0x3B0
    void *responseBuffer;                       // 0x3B4
    int unk_3B8;                                // 0x3B8
    u32 sequence;                               // 0x3BC
    u32 sequenceStep;                           // 0x3C0
    int isRunning;                              // 0x3C4
    u32 unk_3C8;                                // 0x3C8
    GlobalTerminalNetStepWork stepWork;         // 0x3CC
    u8 unk_3D8[8];                              // 0x3D8
    int errorCode;                              // 0x3E0
    int errorType;                              // 0x3E4
    int pendingCommand;                         // 0x3E8
    int activeCommand;                          // 0x3EC
    u32 searchKind;                             // 0x3F0
    MsgData *msgData;                           // 0x3F4
    MessageFormat *msgFormat;                   // 0x3F8
    String *string;                             // 0x3FC
    GlobalTerminalNetResponseFunc responseFunc; // 0x400
    u8 unk_404[8];                              // 0x404
    GlobalTerminalNetCompleteFunc completeFunc; // 0x40C
    u8 unk_410;                                 // 0x410
    u8 sendDelay;                               // 0x411
    u8 unk_412[2];                              // 0x412
}; // size 0x414

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

void GlobalTerminalNet_PackDressUpPhoto(SaveData *saveData, const GlobalTerminalNetDressUpRecord *record, GlobalTerminalNetDressUpForm *form);
void GlobalTerminalNet_UnpackDressUpPhoto(const GlobalTerminalNetDressUpForm *form, GlobalTerminalNetDressUpRecord *record);
void GlobalTerminalNet_PackBoxData(SaveData *saveData, PCStorage *storage, int box, GlobalTerminalNetBoxForm *form, enum HeapID heapID);
void GlobalTerminalNet_SealBoxData(SaveData *saveData, GlobalTerminalNetBoxForm *form, int value);
void GlobalTerminalNet_SetRequestHeader(SaveData *saveData, const void *src, void *dst);
void GlobalTerminalNet_CopyRequestHeader(const void *src, void *dst);
void ov39_022271CC(SaveData *saveData, const GlobalTerminalNetRankingsSource *src, GlobalTerminalNetRankingsHeader *dst);
BOOL GlobalTerminalNet_Init(GlobalTerminalNet *net, GlobalTerminalNetArgs *args);
void GlobalTerminalNet_Free(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_SeqStopClient(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_SeqDisconnect(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_SeqShowError(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_SeqWaitButton(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestDressUpUpload(GlobalTerminalNet *net, const void *header, const GlobalTerminalNetDressUpRecord *record);
BOOL GlobalTerminalNet_RequestDressUpDownload(GlobalTerminalNet *net, u16 arg);
BOOL GlobalTerminalNet_RequestBoxUpload(GlobalTerminalNet *net, int value, const void *header, PCStorage *storage, int box);
BOOL GlobalTerminalNet_RequestBoxDownload(GlobalTerminalNet *net, u8 arg);
BOOL ov39_022274B4(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestRankings(GlobalTerminalNet *net, const GlobalTerminalNetRankingsSource *src, const GlobalTerminalNetPair *pairs);
BOOL GlobalTerminalNet_RequestVideoUpload(GlobalTerminalNet *net, const void *header);
BOOL GlobalTerminalNet_RequestVideoSearch(GlobalTerminalNet *net, u16 arg0, u8 arg1, u8 arg2, u8 arg3);
BOOL ov39_022275E8(GlobalTerminalNet *net);
BOOL ov39_02227648(GlobalTerminalNet *net);
BOOL ov39_022276A4(GlobalTerminalNet *net);
BOOL ov39_022276E0(GlobalTerminalNet *net);
BOOL GlobalTerminalNet_RequestVideoDownload(GlobalTerminalNet *net, u32 numberLow, u32 numberHigh);
BOOL ov39_0222774C(GlobalTerminalNet *net, u32 argLow, u32 argHigh);
BOOL GlobalTerminalNet_Update(GlobalTerminalNet *net);
u32 GlobalTerminalNet_GetError(GlobalTerminalNet *net, GlobalTerminalNetError **error);
u64 GlobalTerminalNet_GetUploadedVideoNumber(GlobalTerminalNet *net);
int GlobalTerminalNet_GetDressUpList(GlobalTerminalNet *net, void **out, int max);
int GlobalTerminalNet_GetBoxList(GlobalTerminalNet *net, void **out, int max);
void ov39_02227F74(GlobalTerminalNet *net, void **out);
void GlobalTerminalNet_GetRankings(GlobalTerminalNet *net, void **out1, void **out2);
u32 GlobalTerminalNet_GetDownloadedVideo(GlobalTerminalNet *net, u8 **out);
int GlobalTerminalNet_GetVideoList(GlobalTerminalNet *net, GlobalTerminalNetVideoHeader **out, int max);

// dress-up form converters (end of src/global_terminal_connect.c)
void GlobalTerminalNet_DressUpToUploadForm(const GlobalTerminalNetDressUpRecord *src, GlobalTerminalNetDressUpForm *dst);
void GlobalTerminalNet_UploadFormToDressUp(const GlobalTerminalNetDressUpForm *src, GlobalTerminalNetDressUpRecord *dst);

#endif // POKEHEARTGOLD_GLOBAL_TERMINAL_NET_H
