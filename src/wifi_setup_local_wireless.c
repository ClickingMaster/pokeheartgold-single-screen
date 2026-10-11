#pragma force_active on
#include "wifi_setup_local_wireless.h"

#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_app.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Local wireless: sound player controls, title banner, dialog window, Wi-Fi USB connector scan and
// DS Download Play parent (0x0223ABB4-0x0223D4C8).

typedef struct WifiSetupDialogPlanes {
    u8 planes[6];
} WifiSetupDialogPlanes;

// Pointers to the Download Play parent's AID masks, followed by the parent's tables and state names.
typedef struct WifiSetupMbParentData {
    u16 *masks[6];
    u8 rest[0x848 - 0x18];
} WifiSetupMbParentData;

extern void *ov13_0224F5B0[]; // same address as gWifiSetupBanner.banner, then gWifiSetupUsbScan, gWifiSetupDialog

extern const u8 ov13_022457EC[][WIFI_SETUP_BANNER_NUM_LETTERS]; // banner letter templates per kind
extern const u8 ov13_022457FB[][WIFI_SETUP_BANNER_NUM_LETTERS]; // banner letter palettes per kind
extern WifiSetupPoint ov13_0224580A[WIFI_SETUP_BANNER_NUM_LETTERS];
extern u8 ov13_02245820[6]; // empty BSSID
extern u8 ov13_02245828[];  // "NWCUSBAP"
extern WifiSetupUsbScanParam ov13_02245834;
extern u8 ov13_02245878[];
extern WifiSetupRect ov13_02245882;
extern u8 ov13_0224588A[][2];
extern s8 ov13_02245894[][2];
extern s8 ov13_02245895[][2];
extern u8 ov13_0224589E[][2];
extern WifiSetupPoint ov13_022458A8[];
extern WifiSetupPoint ov13_022458BC[];
extern WifiSetupPoint ov13_022458D4[];
extern WifiSetupPoint ov13_022458EC[];
extern WifiSetupPoint ov13_02245904[][2];
extern u8 ov13_0224592C[];
extern u8 ov13_02245934[];
extern char *ov13_02246854[];

int WM_Initialize(void *buf, void *callback, u16 dmaNo);
int WM_ReadStatus(void *status);
u16 WM_GetDispersionScanPeriod(void);
int WM_StartScanEx(void *callback, void *param);
int WM_Reset(void *callback);
int WM_End(void *callback);
int MB_Init(void *work, void *user, u32 ggid, u32 tgid, u32 dmaNo);
void MB_SetParentCommParam(u16 sendSize, u16 maxChildren);
void MB_CommSetParentStateCallback(void *callback);
int MB_StartParentFromIdle(u32 channel);
u32 MB_GetSegmentLength(FSFile *file);
BOOL MB_ReadSegment(FSFile *file, void *buf, u32 len);
BOOL MB_RegisterFile(void *file, void *buf);
BOOL MB_CommResponseRequest(u16 aid, u32 ack);
void MB_DisconnectChild(u16 aid);
BOOL MB_CommIsBootable(u16 aid);
void *MB_CommGetChildUser(u16 aid);
void MB_End(void);
extern int memcmp(const void *, const void *, u32);
extern void NNS_SndArcPlayerStartSeqArc(void *, int, int);
extern void NNS_SndPlayerSetVolume(void *, int);
extern void NNS_SndPlayerSetTrackPitch(void *, u32, int);
extern void NNS_SndPlayerStopSeq(void *, int);
extern void NNS_SndMain(void);

int WifiSetupMbParent_GetChildState(void);
void WifiSetupWh_Init(u8 *work);

void WifiSetupBanner_SlideInTask0(void *task, void *arg);
void WifiSetupBanner_SlideInTask1(void *task, void *arg);
void WifiSetupBanner_SlideInTask2(void *task, void *arg);
void WifiSetupBanner_SlideInTask3(void *task, void *arg);
void WifiSetupBanner_SlideInTask4(void *task, void *arg);
void WifiSetupBanner_StopTask(void *task, void *arg);
void WifiSetupBanner_SlideOutTask4(void *task, void *arg);
void WifiSetupBanner_SlideOutTask3(void *task, void *arg);
void WifiSetupBanner_SlideOutTask2(void *task, void *arg);
void WifiSetupBanner_SlideOutTask1(void *task, void *arg);
void WifiSetupBanner_SlideOffTask(void *task, void *arg);
void WifiSetupBanner_LoadScreenTask(void *task, void *arg);
BOOL WifiSetupUsbScan_StartScan(void);
void WifiSetupUsbScan_WmCallback(WifiSetupUsbScanCallbackArg *arg);
void WifiSetupUsbScan_StoreResults(WifiSetupUsbScanCallbackArg *arg);
void WifiSetupUsbScan_CheckResults(WifiSetupUsbScanCallbackArg *arg);
void WifiSetupUsbScan_TimeoutTask(void *task, void *arg);
void WifiSetupDialog_FadeInTask(void *task, void *arg);
void WifiSetupDialog_SlideInTask(void *task, void *arg);
void WifiSetupDialog_InputTask(void *task, void *arg);
void WifiSetupDialog_TimerTask(void *task, void *arg);
void WifiSetupDialog_SetY(int y);
void WifiSetupDialog_ClipBlock(WifiSetupOamBlock *block, int y);
void WifiSetupDialog_PressButton(int index);
void WifiSetupDialog_CloseDelayTask(void *task, void *arg);
void WifiSetupDialog_SlideOutTask(void *task, void *arg);
void WifiSetupDialog_FadeOutTask(void *task, void *arg);
void WifiSetupDialog_FreeTask(void *task, void *arg);

static inline void WifiSetupMbParent_ClearChild(u16 aid) {
    u16 mask = ~(1 << aid);
    OSIntrMode e = OS_DisableInterrupts();
    gWifiSetupMbParent->head.connectedMask &= mask;
    gWifiSetupMbParent->head.requestedMask &= mask;
    gWifiSetupMbParent->head.waitingMask &= mask;
    gWifiSetupMbParent->head.sendingMask &= mask;
    gWifiSetupMbParent->head.sentMask &= mask;
    gWifiSetupMbParent->head.bootableMask &= mask;
    OS_RestoreInterrupts(e);
}

static inline void WifiSetupMbParent_DisconnectChild(u16 aid) {
    WifiSetupMbParent_ClearChild(aid);
    MB_DisconnectChild(aid);
}

WifiSetupMbParentData ov13_022468D8 = {
    { NULL, NULL, NULL, NULL, NULL, NULL },
    {
     0xC0,
     0x68,
     0x24,
     0x02,
     0x4C,
     0x69,
     0x24,
     0x02,
     0x80,
     0x69,
     0x24,
     0x02,
     0x34,
     0x6B,
     0x24,
     0x02,
     0xB4,
     0x6A,
     0x24,
     0x02,
     0x44,
     0x6A,
     0x24,
     0x02,
     0xD4,
     0x69,
     0x24,
     0x02,
     0xB8,
     0x69,
     0x24,
     0x02,
     0x7C,
     0x68,
     0x24,
     0x02,
     0xD4,
     0x6A,
     0x24,
     0x02,
     0x64,
     0x69,
     0x24,
     0x02,
     0x28,
     0x6A,
     0x24,
     0x02,
     0xA8,
     0x68,
     0x24,
     0x02,
     0x60,
     0x6A,
     0x24,
     0x02,
     0x7C,
     0x6A,
     0x24,
     0x02,
     0xF4,
     0x6A,
     0x24,
     0x02,
     0x98,
     0x6A,
     0x24,
     0x02,
     0x14,
     0x6B,
     0x24,
     0x02,
     0x9C,
     0x69,
     0x24,
     0x02,
     0xF0,
     0x69,
     0x24,
     0x02,
     0x68,
     0x68,
     0x24,
     0x02,
     0x90,
     0x68,
     0x24,
     0x02,
     0x0C,
     0x6A,
     0x24,
     0x02,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x49,
     0x44,
     0x4C,
     0x45,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x45,
     0x4E,
     0x54,
     0x52,
     0x59,
     0x00,
     0x00,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x52,
     0x45,
     0x51,
     0x55,
     0x45,
     0x53,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x4F,
     0x52,
     0x00,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x43,
     0x41,
     0x4E,
     0x43,
     0x45,
     0x4C,
     0x00,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x4D,
     0x45,
     0x4D,
     0x42,
     0x45,
     0x52,
     0x5F,
     0x46,
     0x55,
     0x4C,
     0x4C,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x57,
     0x41,
     0x49,
     0x54,
     0x5F,
     0x54,
     0x4F,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x44,
     0x49,
     0x53,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x50,
     0x4C,
     0x45,
     0x54,
     0x45,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x52,
     0x45,
     0x51,
     0x5F,
     0x41,
     0x43,
     0x43,
     0x45,
     0x50,
     0x54,
     0x45,
     0x44,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x5F,
     0x50,
     0x52,
     0x4F,
     0x43,
     0x45,
     0x45,
     0x44,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x42,
     0x4F,
     0x4F,
     0x54,
     0x5F,
     0x52,
     0x45,
     0x51,
     0x55,
     0x45,
     0x53,
     0x54,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x52,
     0x45,
     0x42,
     0x4F,
     0x4F,
     0x54,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x00,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x49,
     0x4E,
     0x49,
     0x54,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x50,
     0x4C,
     0x45,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x50,
     0x4C,
     0x45,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x00,
     0x4D,
     0x42,
     0x5F,
     0x43,
     0x4F,
     0x4D,
     0x4D,
     0x5F,
     0x50,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x42,
     0x4F,
     0x4F,
     0x54,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x41,
     0x42,
     0x4C,
     0x45,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x4D,
     0x42,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x44,
     0x41,
     0x54,
     0x41,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x00,
     0x4E,
     0x2F,
     0x41,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x46,
     0x41,
     0x49,
     0x4C,
     0x45,
     0x44,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4E,
     0x4F,
     0x5F,
     0x44,
     0x41,
     0x54,
     0x41,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x54,
     0x49,
     0x4D,
     0x45,
     0x4F,
     0x55,
     0x54,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x53,
     0x55,
     0x43,
     0x43,
     0x45,
     0x53,
     0x53,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4D,
     0x50,
     0x5F,
     0x49,
     0x4E,
     0x44,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x43,
     0x46,
     0x5F,
     0x54,
     0x45,
     0x53,
     0x54,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4E,
     0x4F,
     0x5F,
     0x45,
     0x4E,
     0x54,
     0x52,
     0x59,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4E,
     0x4F,
     0x5F,
     0x43,
     0x48,
     0x49,
     0x4C,
     0x44,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4F,
     0x50,
     0x45,
     0x52,
     0x41,
     0x54,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x43,
     0x46,
     0x5F,
     0x49,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4D,
     0x50,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x46,
     0x49,
     0x46,
     0x4F,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x4F,
     0x52,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4E,
     0x4F,
     0x5F,
     0x44,
     0x41,
     0x54,
     0x41,
     0x53,
     0x45,
     0x54,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x57,
     0x4D,
     0x5F,
     0x44,
     0x49,
     0x53,
     0x41,
     0x42,
     0x4C,
     0x45,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x4F,
     0x52,
     0x54,
     0x5F,
     0x52,
     0x45,
     0x43,
     0x56,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4D,
     0x50,
     0x41,
     0x43,
     0x4B,
     0x5F,
     0x49,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x5F,
     0x46,
     0x41,
     0x49,
     0x4C,
     0x45,
     0x44,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x43,
     0x46,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x46,
     0x4C,
     0x41,
     0x53,
     0x48,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x4F,
     0x52,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4D,
     0x50,
     0x45,
     0x4E,
     0x44,
     0x5F,
     0x49,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x4F,
     0x52,
     0x54,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x53,
     0x43,
     0x41,
     0x4E,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x46,
     0x49,
     0x46,
     0x4F,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x4F,
     0x52,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x49,
     0x4C,
     0x4C,
     0x45,
     0x47,
     0x41,
     0x4C,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x42,
     0x45,
     0x41,
     0x43,
     0x4F,
     0x4E,
     0x5F,
     0x4C,
     0x4F,
     0x53,
     0x54,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x49,
     0x4E,
     0x56,
     0x41,
     0x4C,
     0x49,
     0x44,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x41,
     0x4D,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x57,
     0x4C,
     0x5F,
     0x4C,
     0x45,
     0x4E,
     0x47,
     0x54,
     0x48,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x42,
     0x45,
     0x41,
     0x43,
     0x4F,
     0x4E,
     0x5F,
     0x52,
     0x45,
     0x43,
     0x56,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x52,
     0x45,
     0x41,
     0x53,
     0x53,
     0x4F,
     0x43,
     0x49,
     0x41,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x42,
     0x45,
     0x41,
     0x43,
     0x4F,
     0x4E,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x54,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x45,
     0x4E,
     0x54,
     0x5F,
     0x46,
     0x4F,
     0x55,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x49,
     0x53,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x53,
     0x54,
     0x4F,
     0x50,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4F,
     0x56,
     0x45,
     0x52,
     0x5F,
     0x4D,
     0x41,
     0x58,
     0x5F,
     0x45,
     0x4E,
     0x54,
     0x52,
     0x59,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x49,
     0x44,
     0x4C,
     0x45,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x49,
     0x53,
     0x41,
     0x53,
     0x53,
     0x4F,
     0x43,
     0x49,
     0x41,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x45,
     0x4E,
     0x54,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x42,
     0x55,
     0x53,
     0x59,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x41,
     0x55,
     0x54,
     0x48,
     0x45,
     0x4E,
     0x54,
     0x49,
     0x43,
     0x41,
     0x54,
     0x45,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x52,
     0x54,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x4F,
     0x52,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x53,
     0x45,
     0x4E,
     0x44,
     0x5F,
     0x51,
     0x55,
     0x45,
     0x55,
     0x45,
     0x5F,
     0x46,
     0x55,
     0x4C,
     0x4C,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x57,
     0x4C,
     0x5F,
     0x49,
     0x4E,
     0x56,
     0x41,
     0x4C,
     0x49,
     0x44,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x41,
     0x4D,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x4E,
     0x4F,
     0x5F,
     0x52,
     0x41,
     0x44,
     0x49,
     0x4F,
     0x00,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x49,
     0x4E,
     0x56,
     0x41,
     0x4C,
     0x49,
     0x44,
     0x5F,
     0x50,
     0x4F,
     0x4C,
     0x4C,
     0x42,
     0x49,
     0x54,
     0x4D,
     0x41,
     0x50,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x53,
     0x43,
     0x41,
     0x4E,
     0x4E,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x00,
     0x00,
     0x57,
     0x4D,
     0x5F,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x45,
     0x4E,
     0x54,
     0x5F,
     0x4E,
     0x4F,
     0x54,
     0x5F,
     0x46,
     0x4F,
     0x55,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x4B,
     0x45,
     0x59,
     0x53,
     0x48,
     0x41,
     0x52,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x44,
     0x41,
     0x54,
     0x41,
     0x53,
     0x48,
     0x41,
     0x52,
     0x49,
     0x4E,
     0x47,
     0x00,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x44,
     0x49,
     0x53,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x45,
     0x44,
     0x00,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x43,
     0x4F,
     0x4E,
     0x4E,
     0x45,
     0x43,
     0x54,
     0x5F,
     0x46,
     0x41,
     0x49,
     0x4C,
     0x00,
     0x00,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x53,
     0x59,
     0x53,
     0x53,
     0x54,
     0x41,
     0x54,
     0x45,
     0x5F,
     0x4D,
     0x45,
     0x41,
     0x53,
     0x55,
     0x52,
     0x45,
     0x43,
     0x48,
     0x41,
     0x4E,
     0x4E,
     0x45,
     0x4C,
     0x00,
     0x44,
     0x57,
     0x43,
     0x69,
     0x5F,
     0x4D,
     0x4F,
     0x56,
     0x5F,
     0x57,
     0x48,
     0x5F,
     0x45,
     0x52,
     0x52,
     0x43,
     0x4F,
     0x44,
     0x45,
     0x5F,
     0x50,
     0x41,
     0x52,
     0x45,
     0x4E,
     0x54,
     0x5F,
     0x4E,
     0x4F,
     0x54,
     0x5F,
     0x46,
     0x4F,
     0x55,
     0x4E,
     0x44,
     0x00,
     0x00,
     0x00,
     0x00,
     },
};

void WifiSetupSound_Play(int seq) {
    NNS_SndArcPlayerStartSeqArc(gWifiSetupSound->handle, 0, seq);
}

void WifiSetupSound_SetVolume(int volume) {
    NNS_SndPlayerSetVolume(gWifiSetupSound->handle, volume);
}

void WifiSetupSound_SetTrackPitch(int mask, int pitch) {
    NNS_SndPlayerSetTrackPitch(gWifiSetupSound->handle, mask, pitch);
}

void WifiSetupSound_Stop(void) {
    NNS_SndPlayerStopSeq(gWifiSetupSound->handle, 0);
}

void WifiSetupSound_UpdateTask(void *task, void *arg) {
    NNS_SndMain();
}

void WifiSetupBanner_Open(int kind) {
    const u8 *res;
    const u8 *anim;
    int i;

    gWifiSetupBanner.banner = WifiSetupHeap_AllocZeroed(0x1c, 4);
    res = ov13_022457EC[kind];
    anim = ov13_022457FB[kind];
    for (i = 0; i < 5; i++) {
        gWifiSetupBanner.banner->letters[i] = WifiSetupOamTemplate_AllocBlock(1, *res++, 1);
        WifiSetupOamBlock_SetPriority(gWifiSetupBanner.banner->letters[i], -1, 0);
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, -0x2a, ov13_0224580A[i].y);
        WifiSetupOamBlock_SetModeAndPalette(gWifiSetupBanner.banner->letters[i], -1, 0, *anim++);
    }
    gWifiSetupBanner.banner->task = WifiSetupTask_Create(0, WifiSetupBanner_SlideInTask0, NULL, 0x78);
    WifiSetupBanner_LoadScreen(kind);
    WifiSetupSound_Play(0xd);
}

void WifiSetupBanner_SlideInTask0(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[0], 0, &x, &y);
    x += 8;
    if (x < 0x20 || x > 0x100) {
        for (i = 0; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x20;
    for (i = 0; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideInTask1);
}

void WifiSetupBanner_SlideInTask1(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[1], 0, &x, &y);
    x += 8;
    if (x < 0x50 || x > 0x100) {
        for (i = 1; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x50;
    for (i = 1; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideInTask2);
}

void WifiSetupBanner_SlideInTask2(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[2], 0, &x, &y);
    x += 8;
    if (x < 0x68 || x > 0x100) {
        for (i = 2; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x68;
    for (i = 2; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideInTask3);
}

void WifiSetupBanner_SlideInTask3(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[3], 0, &x, &y);
    x += 8;
    if (x < 0x98 || x > 0x100) {
        for (i = 3; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x98;
    for (i = 3; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideInTask4);
}

#pragma push
#pragma opt_propagation off

void WifiSetupBanner_SlideInTask4(void *task, void *arg) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[4], 0, &x, &y);
    x += 8;
    if (x < 0xB0 || x > 0x100) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[4], -1, x, 0x21);
        return;
    }
    {
        WifiSetupBannerState *state = &gWifiSetupBanner;
        WifiSetupBanner *banner = state->banner;
        x = 0xB0;
        WifiSetupOamBlock_SetPosition(banner->letters[4], -1, x, 0x21);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_StopTask);
}

#pragma pop

void WifiSetupBanner_StopTask(void *task, void *arg) {
    WifiSetupTask_DestroyLater(0, task);
    gWifiSetupBanner.banner->task = NULL;
}

void WifiSetupBanner_Close(void) {
    gWifiSetupBanner.banner->closing = 1;
    gWifiSetupBanner.banner->task = WifiSetupTask_Create(0, WifiSetupBanner_SlideOutTask4, 0, 0x78);
}

void WifiSetupBanner_SetKind(int n) {
    int i;
    for (i = 0; i < 5; i += 2) {
        WifiSetupOamTemplate_Copy(1, ov13_022457EC[n][i], WifiSetupOamBlock_GetOam(gWifiSetupBanner.banner->letters[i], 0));
        WifiSetupOamBlock_SetPriority(gWifiSetupBanner.banner->letters[i], -1, 0);
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, ov13_0224580A[i].x, ov13_0224580A[i].y);
        WifiSetupOamBlock_SetModeAndPalette(gWifiSetupBanner.banner->letters[i], -1, 0, ov13_022457FB[n][i]);
    }
    WifiSetupBanner_LoadScreen(n);
}

BOOL WifiSetupBanner_IsMoving(void) {
    return gWifiSetupBanner.banner->task != NULL;
}

#pragma push
#pragma opt_propagation off

void WifiSetupBanner_SlideOutTask4(void *task, void *arg) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[4], 0, &x, &y);
    x -= 8;
    if (x > 0x98) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[4], -1, x, 0x21);
        return;
    }
    {
        WifiSetupBannerState *state = &gWifiSetupBanner;
        WifiSetupBanner *banner = state->banner;
        x = 0x98;
        WifiSetupOamBlock_SetPosition(banner->letters[4], -1, x, 0x21);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideOutTask3);
}

#pragma pop

void WifiSetupBanner_SlideOutTask3(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[3], 0, &x, &y);
    x -= 8;
    if (x > 0x68) {
        for (i = 3; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x68;
    for (i = 3; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideOutTask2);
}

void WifiSetupBanner_SlideOutTask2(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[2], 0, &x, &y);
    x -= 8;
    if (x > 0x50) {
        for (i = 2; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x50;
    for (i = 2; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideOutTask1);
}

void WifiSetupBanner_SlideOutTask1(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[1], 0, &x, &y);
    x -= 8;
    if (x > 0x20) {
        for (i = 1; i < 5; i++) {
            WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x20;
    for (i = 1; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    WifiSetupTask_SetFunc(task, WifiSetupBanner_SlideOffTask);
}

void WifiSetupBanner_SlideOffTask(void *task, void *arg) {
    int x;
    int y;
    int i;
    WifiSetupOamBlock_GetPosition(gWifiSetupBanner.banner->letters[0], 0, &x, &y);
    x -= 8;
    for (i = 0; i < 5; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupBanner.banner->letters[i], -1, x, ov13_0224580A[i].y);
    }
    if (x > 0x1D6) {
        return;
    }
    if (x < 0x100) {
        return;
    }
    WifiSetupTask_DestroyLater(0, task);
    for (i = 0; i < 5; i++) {
        WifiSetupOamBlock_Free(gWifiSetupBanner.banner->letters[i]);
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F5B0);
}

BOOL WifiSetupBanner_IsNotClosing(void) {
    if (gWifiSetupBanner.banner == NULL) {
        return TRUE;
    }
    return gWifiSetupBanner.banner->closing == 0;
}

void WifiSetupBanner_LoadScreen(int n) {
    gWifiSetupBanner.screen = WifiSetupFile_Load(WifiSetup_LocalizePath(ov13_02246854[n]), 0, 4);
    WifiSetupTask_Create(1, WifiSetupBanner_LoadScreenTask, 0, 0x78);
}

void WifiSetupBanner_LoadScreenTask(void *task, void *arg) {
    DC_FlushRange(gWifiSetupBanner.screen, 0x600);
    GXS_LoadBG1Scr(gWifiSetupBanner.screen, 0, 0x600);
    WifiSetupFile_Free(gWifiSetupBanner.screen);
    WifiSetupTask_DestroyLater(1, task);
}

BOOL WifiSetupUsbScan_Start(void (*cb)(int)) {
    OSOwnerInfo info;
    WifiSetupUsbScan *w = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupUsbScan), 0x20);
    gWifiSetupUsbScan = w;
    w->callback = cb;
    (*(WifiSetupUsbScan *volatile *)&gWifiSetupUsbScan)->startTick = OS_GetTick();
    if (WM_Initialize(gWifiSetupUsbScan, WifiSetupUsbScan_WmCallback, 3) == 2) {
        do {
            WM_ReadStatus(&gWifiSetupUsbScan->status);
        } while (gWifiSetupUsbScan->status.state != 2);
        gWifiSetupUsbScan->param = ov13_02245834;
        gWifiSetupUsbScan->param.buf = gWifiSetupUsbScan->scanBuf;
        gWifiSetupUsbScan->param.period = WM_GetDispersionScanPeriod();
        OS_GetOwnerInfo(&info);
        MI_CpuCopy8(ov13_02245828, gWifiSetupUsbScan->param.ssidTag, 8);
        gWifiSetupUsbScan->param.ssidFlag = 1;
        MI_CpuCopy8(info.nickName, gWifiSetupUsbScan->param.nickname, info.nickNameLength * 2);
        if (WifiSetupUsbScan_StartScan()) {
            gWifiSetupUsbScan->task = WifiSetupTask_Create(0, WifiSetupUsbScan_TimeoutTask, 0, 0x78);
            return TRUE;
        }
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F5B0[1]);
    return FALSE;
}

BOOL WifiSetupUsbScan_StartScan(void) {
    return WM_StartScanEx(WifiSetupUsbScan_WmCallback, &gWifiSetupUsbScan->param) == 2;
}

BOOL WifiSetupUsbScan_End(void) {
    gWifiSetupUsbScan->endState = 1;
    WM_ReadStatus(&gWifiSetupUsbScan->status);
    if (gWifiSetupUsbScan->status.state != 2) {
        if (WM_Reset(WifiSetupUsbScan_WmCallback) != 2) {
            return FALSE;
        }
        do {
            WM_ReadStatus(&gWifiSetupUsbScan->status);
        } while (gWifiSetupUsbScan->status.state != 2);
    }
    if (WM_End(WifiSetupUsbScan_WmCallback) != 2) {
        return FALSE;
    }
    if (gWifiSetupUsbScan->task != NULL) {
        WifiSetupTask_Destroy(0, gWifiSetupUsbScan->task);
    }
    while (gWifiSetupUsbScan->endState != 2) {
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F5B0[1]);
    return TRUE;
}

void WifiSetupUsbScan_SetCallback(void (*cb)(int)) {
    gWifiSetupUsbScan->callback = cb;
}

void WifiSetupUsbScan_WmCallback(WifiSetupUsbScanCallbackArg *arg) {
    if (arg->errcode != 0) {
        return;
    }
    if (gWifiSetupUsbScan->endState != 0) {
        if (arg->apiid == 2) {
            gWifiSetupUsbScan->endState = 2;
        }
        return;
    }
    if (arg->apiid != 0x26) {
        return;
    }
    switch (arg->state) {
    case 5:
        if (gWifiSetupUsbScan->rescanning) {
            WifiSetupUsbScan_CheckResults(arg);
        } else {
            WifiSetupUsbScan_StoreResults(arg);
        }
        WifiSetupUsbScan_StartScan();
        break;
    case 4:
        WifiSetupUsbScan_StartScan();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void WifiSetupUsbScan_StoreResults(WifiSetupUsbScanCallbackArg *arg) {
    WifiSetupUsbScanBss *bss;
    int i;
    int j;
    WifiSetupUsbScan *w;
    for (i = 0; i < arg->count; i++) {
        bss = arg->bss[i];
        DC_InvalidateRange(bss, 0xC0);
        if (memcmp(bss->ssidTag, ov13_02245828, 8) != 0) {
            continue;
        }
        w = gWifiSetupUsbScan;
        for (j = 0; j < 16; j++) {
            if (memcmp(bss->bssid, w->entries[j].bssid, 6) == 0) {
                if (w->entries[j].flag != 0) {
                    goto next;
                }
                if (!(bss->ssidFlag & 1)) {
                    goto next;
                }
                if (w->callback != NULL) {
                    w->callback(1);
                }
                return;
            }
        }
        for (j = 0; j < 16; j++) {
            if (memcmp(w->entries[j].bssid, ov13_02245820, 6) == 0) {
                MI_CpuCopy8(bss->bssid, w->entries[j].bssid, 6);
                gWifiSetupUsbScan->entries[j].flag = (bss->ssidFlag & 1) ? 1 : 0;
                break;
            }
        }
next:;
    }
}

void WifiSetupUsbScan_CheckResults(WifiSetupUsbScanCallbackArg *arg) {
    u8 *data;
    int i, j;
    WifiSetupUsbScanEntry *entry;
    WifiSetupUsbScan *scan;
    int count;
    if (gWifiSetupUsbScan->found) {
        if (gWifiSetupUsbScan->callback) {
            gWifiSetupUsbScan->callback(gWifiSetupUsbScan->found);
        }
        return;
    }
    DC_InvalidateRange(gWifiSetupUsbScan->scanBuf, 0x400);
    count = arg->count;
    i = 0;
    if (count <= 0) {
        return;
    }
    scan = gWifiSetupUsbScan;
    do {
        data = (u8 *)arg->bss[i];
        if (memcmp(data + 12, ov13_02245828, 8) == 0 && (data[21] & 1)) {
            for (j = 0, entry = scan->entries; j < WIFI_SETUP_USB_SCAN_NUM_ENTRIES; j++, entry++) {
                if (memcmp(data + 4, entry->bssid, 6) == 0) {
                    if (scan->entries[j].flag) {
                        break;
                    }
                    if (!scan->callback) {
                        scan->found = 1;
                        return;
                    }
                    scan->callback(1);
                    return;
                }
            }
        }
    } while (++i < count);
}

void WifiSetupUsbScan_TimeoutTask(void *task, void *arg) {
    int j;
    BOOL a;
    BOOL b;
    WifiSetupUsbScan *w;
    OSTick now = OS_GetTick();
    w = gWifiSetupUsbScan;
    b = FALSE;
    if (now < w->startTick + 0x17F898) {
        return;
    }
    a = FALSE;
    for (j = 0; j < 16; j++) {
        if (memcmp(w->entries[j].bssid, ov13_02245820, 6) != 0) {
            if (w->entries[j].flag) {
                a = TRUE;
            } else {
                b = TRUE;
            }
        }
    }
    if (a && b) {
        if (w->callback != NULL) {
            w->callback(2);
        }
    } else if (a) {
        if (w->callback != NULL) {
            w->callback(1);
        }
    } else if (!b) {
        if (w->callback != NULL) {
            w->callback(0);
        }
    }
    gWifiSetupUsbScan->task = NULL;
    gWifiSetupUsbScan->rescanning = 1;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupDialog_Open(int message, int kind, u8 fade, int digitPos, int digit) {
    u16 *text;
    WifiSetupDialogPlanes tbl = *(WifiSetupDialogPlanes *)(ov13_02245878 + 4);
    u32 numObjs;
    int i;
    text = WifiSetupText_GetStringWithDigit(ov13_0224F4F8, message, digitPos, digit);
    gWifiSetupDialog = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupDialog), 4);
    gWifiSetupDialog->kind = kind;
    gWifiSetupDialog->selected = -2;
    gWifiSetupDialog->fade = fade;
    G2_SetBlendBrightness(0x1F, 0);
    gWifiSetupDialog->frame = WifiSetupOamTemplate_AllocBlock(0, ov13_0224592C[kind], 0);
    WifiSetupOamBlock_SetPosition(gWifiSetupDialog->frame, -1, 0x100, 0);
    WifiSetupOamBlock_SetPriority(gWifiSetupDialog->frame, -1, 0);
    for (i = 0; i < ov13_02245934[kind]; i++) {
        gWifiSetupDialog->buttons[i] = WifiSetupOamTemplate_AllocBlock(0, ov13_0224589E[kind][i], 0);
        WifiSetupOamBlock_SetPosition(gWifiSetupDialog->buttons[i], -1, 0x100, 0);
        WifiSetupOamBlock_SetPriority(gWifiSetupDialog->buttons[i], -1, 0);
    }
    gWifiSetupDialog->canvas = WifiSetupText_AllocObjCanvas(0, 0x20, 0xC, 1, &numObjs, 0);
    gWifiSetupDialog->textBlock = WifiSetupOamBlock_Alloc(0, numObjs, 0);
    WifiSetupText_DrawInRect(gWifiSetupDialog->canvas, 0, 0, ov13_022458BC[kind].x, ov13_022458BC[kind].y, 2, WifiSetup_GetTextFlags(), text);
    WifiSetupText_ShowObjCanvas(gWifiSetupDialog->canvas, 0x100, 0, gWifiSetupDialog->textBlock, 0);
    WifiSetupDisplay_SetWindowPlanes(0, 0, 0x1F, 0);
    WifiSetupDisplay_SetWindowPlanes(0, 1, 0x1F, tbl.planes[gWifiSetupDialog->kind]);
    WifiSetupDisplay_SetWindowPlanes(0, 3, 0x1F, 1);
    WifiSetupDisplay_SetWindowRect(0, 1, &ov13_02245882);
    WifiSetupDialog_SetY(0xC0);
    GX_SetVisibleWnd(3);
    if (fade) {
        gWifiSetupDialog->task = WifiSetupTask_Create(1, WifiSetupDialog_FadeInTask, 0, 0x78);
    } else {
        gWifiSetupDialog->task = WifiSetupTask_Create(1, WifiSetupDialog_SlideInTask, 0, 0x78);
    }
}

void WifiSetupDialog_Close(void) {
    WifiSetupTask_DestroyLater(0, gWifiSetupDialog->task);
    gWifiSetupDialog->task = WifiSetupTask_Create(1, WifiSetupDialog_CloseDelayTask, 0, 0x78);
}

int WifiSetupDialog_GetSelected(void) {
    return gWifiSetupDialog->selected;
}

BOOL WifiSetupDialog_IsOpen(void) {
    return gWifiSetupDialog != NULL;
}

void WifiSetupDialog_FadeInTask(void *task, void *arg) {
    gWifiSetupDialog->brightness--;
    G2x_ChangeBlendBrightness_((u16 *)&reg_G2_BLDCNT, gWifiSetupDialog->brightness);
    if (gWifiSetupDialog->brightness > -12) {
        return;
    }
    WifiSetupTask_SetFunc(task, WifiSetupDialog_SlideInTask);
}

void WifiSetupDialog_SlideInTask(void *task, void *arg) {
    int x;
    int y;
    WifiSetupOamBlock_GetPosition(gWifiSetupDialog->frame, 0, &x, &y);
    y -= 12;
    if (y > ov13_022458D4[gWifiSetupDialog->kind].y) {
        WifiSetupDialog_SetY(y);
        return;
    }
    WifiSetupDialog_SetY(ov13_022458D4[gWifiSetupDialog->kind].y);
    if (gWifiSetupDialog->kind == 5) {
        gWifiSetupDialog->task = WifiSetupTask_Create(0, WifiSetupDialog_TimerTask, 0, 0x78);
    } else {
        gWifiSetupDialog->task = WifiSetupTask_Create(0, WifiSetupDialog_InputTask, 0, 0x78);
    }
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupDialog_InputTask(void *task, void *arg) {
    WifiSetupRect temp;
    int i;
    register WifiSetupDialog *w;
    int j;
    u8 *counts;
    for (i = 0; i < ov13_02245934[gWifiSetupDialog->kind]; i++) {
        WifiSetup_RectFromPosSize(&ov13_02245904[gWifiSetupDialog->kind][ov13_0224588A[gWifiSetupDialog->kind][i]], &ov13_022458A8[gWifiSetupDialog->kind], &temp);
        if (WifiSetupInput_IsTouchPressed(&temp)) {
            gWifiSetupDialog->selected = i;
            break;
        }
    }
    if (WifiSetupInput_IsKeyPressed(1)) {
        gWifiSetupDialog->selected = ov13_02245894[gWifiSetupDialog->kind][0];
    }
    if (WifiSetupInput_IsKeyPressed(2)) {
        gWifiSetupDialog->selected = ov13_02245895[gWifiSetupDialog->kind][0];
    }
    counts = ov13_02245934;
    w = gWifiSetupDialog;
    for (j = 0; j < counts[((volatile WifiSetupDialog *)w)->kind]; j++) {
        if (j == w->selected) {
            WifiSetupDialog_PressButton(j);
            return;
        }
    }
    w->selected = -1;
}

void WifiSetupDialog_TimerTask(void *task, void *arg) {
    gWifiSetupDialog->selected = -1;
    gWifiSetupDialog->timer++;
    if (gWifiSetupDialog->timer < 0x78) {
        return;
    }
    WifiSetupTask_DestroyLater(0, task);
    gWifiSetupDialog->task = WifiSetupTask_Create(1, WifiSetupDialog_CloseDelayTask, 0, 0x78);
}

void WifiSetupDialog_SetY(int y) {
    int i;
    int k;
    int top;
    int bottom;
    int left;
    int right;
    WifiSetupRect win;
    WifiSetupOamBlock_SetPosition(gWifiSetupDialog->frame, -1, ov13_022458D4[gWifiSetupDialog->kind].x, y);
    WifiSetupOamBlock_SetPosition(gWifiSetupDialog->textBlock, -1, ov13_022458D4[gWifiSetupDialog->kind].x + 8, y + 8);
    WifiSetupDialog_ClipBlock(gWifiSetupDialog->frame, y);
    WifiSetupDialog_ClipBlock(gWifiSetupDialog->textBlock, y);
    for (i = 0; i < ov13_02245934[gWifiSetupDialog->kind]; i++) {
        k = ov13_0224588A[gWifiSetupDialog->kind][i];
        WifiSetupOamBlock_SetPosition(gWifiSetupDialog->buttons[i], -1, ov13_02245904[gWifiSetupDialog->kind][k].x, y + ov13_02245904[gWifiSetupDialog->kind][k].y - ov13_022458D4[gWifiSetupDialog->kind].y);
        WifiSetupDialog_ClipBlock(gWifiSetupDialog->buttons[i], y);
    }
    top = (u8)y;
    if (top >= 0xC0) {
        bottom = 0;
        top = 0;
    } else {
        bottom = top + ov13_022458EC[gWifiSetupDialog->kind].y;
    }
    left = ov13_022458D4[gWifiSetupDialog->kind].x;
    right = left + ov13_022458EC[gWifiSetupDialog->kind].x;
    if (bottom > 0xC0) {
        bottom = 0xC0;
    }
    // called without the u16 conversions of the prototype
    ((void (*)(int, int, int, int, WifiSetupRect *))WifiSetup_SetRect)(left, top, right, bottom, &win);
    WifiSetupDisplay_SetWindowRect(0, 0, &win);
}

void WifiSetupDialog_ClipBlock(WifiSetupOamBlock *obj, int y) {
    int n = WifiSetupOamBlock_GetCount(obj);
    int i;
    int x;
    int yy;
    int f;
    for (i = 0; i < n; i++) {
        WifiSetupOamBlock_GetPosition(obj, i, &x, &yy);
        if (yy >= y && yy < 0xC0) {
            f = 0;
        } else {
            f = 0x200;
        }
        WifiSetupOamBlock_SetAffineMode(obj, i, f, 0);
    }
}

void WifiSetupDialog_PressButton(int i) {
    int k;
    WifiSetupOamTemplate_Copy(0, ov13_0224589E[gWifiSetupDialog->kind][i] + 1, WifiSetupOamBlock_GetOam(gWifiSetupDialog->buttons[i], 0));
    k = ov13_0224588A[gWifiSetupDialog->kind][i];
    WifiSetupOamBlock_SetPosition(gWifiSetupDialog->buttons[i], -1, ov13_02245904[gWifiSetupDialog->kind][k].x, ov13_02245904[gWifiSetupDialog->kind][k].y);
    WifiSetupOamBlock_SetPriority(gWifiSetupDialog->buttons[i], -1, 0);
}

void WifiSetupDialog_CloseDelayTask(void *task, void *arg) {
    gWifiSetupDialog->closeDelay++;
    if (gWifiSetupDialog->closeDelay < 8) {
        return;
    }
    WifiSetupTask_SetFunc(task, WifiSetupDialog_SlideOutTask);
}

void WifiSetupDialog_SlideOutTask(void *task, void *arg) {
    int x;
    int y;
    WifiSetupOamBlock_GetPosition(gWifiSetupDialog->frame, 0, &x, &y);
    y += 12;
    WifiSetupDialog_SetY(y);
    if (y < 0xC0) {
        return;
    }
    if (gWifiSetupDialog->fade) {
        WifiSetupTask_SetFunc(task, WifiSetupDialog_FadeOutTask);
    } else {
        WifiSetupTask_SetFunc(task, WifiSetupDialog_FreeTask);
    }
}

void WifiSetupDialog_FadeOutTask(void *task, void *arg) {
    gWifiSetupDialog->brightness++;
    G2x_ChangeBlendBrightness_((u16 *)&reg_G2_BLDCNT, gWifiSetupDialog->brightness);
    if (gWifiSetupDialog->brightness < 0) {
        return;
    }
    WifiSetupTask_SetFunc(task, WifiSetupDialog_FreeTask);
}

void WifiSetupDialog_FreeTask(void *task, void *arg) {
    int i;
    GX_SetVisibleWnd(0);
    WifiSetupOamBlock_Free(gWifiSetupDialog->frame);
    WifiSetupOamBlock_Free(gWifiSetupDialog->textBlock);
    for (i = 0; i < ov13_02245934[gWifiSetupDialog->kind]; i++) {
        if (gWifiSetupDialog->buttons[i] != NULL) {
            WifiSetupOamBlock_Free(gWifiSetupDialog->buttons[i]);
        }
    }
    WifiSetupText_FreeObjCanvas(gWifiSetupDialog->canvas);
    WifiSetupTask_DestroyLater(1, task);
    WifiSetupHeap_FreeAndClear(&ov13_0224F5B0[2]);
}

void WifiSetupMbParent_Init(void *buf) {
    gWifiSetupMbParent = buf;
    WifiSetupWh_Init((u8 *)buf + 0x1B160);
    gWifiSetupMbParent->mbWorkPtr = NULL;
    gWifiSetupMbParent->segmentPtr = NULL;
}

void WifiSetupMbParent_Setup(u32 ggid, u32 tgid) {
    WifiSetupMbParentUser user;
    OSOwnerInfo info;
    OS_GetOwnerInfo(&info);
    user.favoriteColor = info.favoriteColor;
    user.nameLength = info.nickNameLength;
    MI_CpuCopy8(info.nickName, user.name, info.nickNameLength * 2);
    user.playerNo = 0;
    gWifiSetupMbParent->head = (WifiSetupMbParentState) { 0, 0, 0, 0, 0, 0, 0 };
    gWifiSetupMbParent->mbWorkPtr = gWifiSetupMbParent->mbWork;
    if (MB_Init(gWifiSetupMbParent->mbWorkPtr, &user, ggid, tgid, 2) != 0) {
        OS_Terminate();
    }
    MB_SetParentCommParam(0x100, 1);
    MB_CommSetParentStateCallback(WifiSetupMbParent_StateCallback);
    WifiSetupMbParent_SetState(1);
}

void WifiSetupMbParent_Start(WifiSetupMbParentFile *reg, u32 channel) {
    WifiSetupMbParent_SetState(2);
    if (MB_StartParentFromIdle(channel) != 0) {
        WifiSetupMbParent_SetState(7);
        return;
    }
    if (!WifiSetupMbParent_RegisterFile(reg)) {
        OS_Terminate();
    }
}

BOOL WifiSetupMbParent_RegisterFile(WifiSetupMbParentFile *reg) {
    FSFile file;
    FSFile *fp;
    BOOL ok = FALSE;
    if (reg->path == NULL) {
        fp = NULL;
    } else {
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, reg->path)) {
            return FALSE;
        }
        fp = &file;
    }
    if (MB_GetSegmentLength(fp) != 0) {
        gWifiSetupMbParent->segmentPtr = gWifiSetupMbParent->segment;
        if (gWifiSetupMbParent->segmentPtr != NULL && MB_ReadSegment(fp, gWifiSetupMbParent->segmentPtr, 0x10000)) {
            if (MB_RegisterFile(reg, gWifiSetupMbParent->segmentPtr)) {
                ok = TRUE;
            }
        }
    }
    if (fp == &file) {
        FS_CloseFile(&file);
    }
    return ok;
}

void WifiSetupMbParent_AcceptChild(u16 aid) {
    if (MB_CommResponseRequest(aid, 1)) {
        return;
    }
    WifiSetupMbParent_DisconnectChild(aid);
}

void WifiSetupMbParent_KickChild(u16 aid) {
    OSIntrMode e;
    if (!MB_CommResponseRequest(aid, 0)) {
        WifiSetupMbParent_DisconnectChild(aid);
        return;
    }
    e = OS_DisableInterrupts();
    gWifiSetupMbParent->head.requestedMask &= ~(1 << aid);
    gWifiSetupMbParent->head.connectedMask &= ~(1 << aid);
    OS_RestoreInterrupts(e);
}

void WifiSetupMbParent_StartSending(u16 aid) {
    OSIntrMode e;
    if (!MB_CommResponseRequest(aid, 2)) {
        WifiSetupMbParent_DisconnectChild(aid);
        return;
    }
    e = OS_DisableInterrupts();
    gWifiSetupMbParent->head.waitingMask &= ~(1 << aid);
    gWifiSetupMbParent->head.sendingMask |= 1 << aid;
    OS_RestoreInterrupts(e);
}

void WifiSetupMbParent_StartDownload(void) {
    u16 i;
    WifiSetupMbParent_SetState(3);
    for (i = 1; i < 16; i++) {
        if ((gWifiSetupMbParent->head.connectedMask & (1 << i)) && !(gWifiSetupMbParent->head.requestedMask & (1 << i))) {
            if (!(gWifiSetupMbParent->head.waitingMask & (1 << i))) {
                WifiSetupMbParent_DisconnectChild(i);
            } else {
                WifiSetupMbParent_StartSending(i);
            }
        }
    }
}

BOOL WifiSetupMbParent_AreAllBootable(void) {
    u16 i;
    if (gWifiSetupMbParent->head.connectedMask == 0) {
        return FALSE;
    }
    for (i = 1; i < 16; i++) {
        if (gWifiSetupMbParent->head.connectedMask & (1 << i)) {
            if (!MB_CommIsBootable(i)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void WifiSetupMbParent_BootChildren(void) {
    u16 i;
    u16 ok = 0;
    for (i = 1; i < 16; i++) {
        if (gWifiSetupMbParent->head.sentMask & (1 << i)) {
            if (!MB_CommResponseRequest(i, 3)) {
                WifiSetupMbParent_DisconnectChild(i);
            } else {
                ok |= 1 << i;
            }
        }
    }
    if (ok == 0) {
        WifiSetupMbParent_SetState(7);
    } else {
        WifiSetupMbParent_SetState(4);
    }
}

void WifiSetupMbParent_End(void) {
    WifiSetupMbParent_SetState(6);
    MB_End();
}

void WifiSetupMbParent_EndIfAllBooted(void) {
    if (gWifiSetupMbParent->head.state == 4 && gWifiSetupMbParent->head.connectedMask == gWifiSetupMbParent->head.bootableMask) {
        MB_End();
    }
}

void WifiSetupMbParent_StateCallback(u16 aid, u32 status, void *arg) {
    WifiSetupMbParentState *w;
    OSIntrMode e;
    void *user;
    u8 *mac;
    switch (status) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 8:
    case 11:
        break;
    case 2:
        if (WifiSetupMbParent_GetState() != 2) {
            break;
        }
        w = &gWifiSetupMbParent->head;
        e = OS_DisableInterrupts();
        w->connectedMask |= 1 << aid;
        OS_RestoreInterrupts(e);
        mac = gWifiSetupMbParent->children[aid - 1].macAddr;
        mac[0] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[0];
        mac[1] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[1];
        mac[2] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[2];
        mac[3] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[3];
        mac[4] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[4];
        mac[5] = ((WifiSetupMbParentConnectArg *)arg)->macAddr[5];
        gWifiSetupMbParent->children[aid - 1].aid = aid;
        break;
    case 3:
        if (WifiSetupMbParent_GetChildState() == 6) {
            break;
        }
        WifiSetupMbParent_ClearChild(aid);
        WifiSetupMbParent_EndIfAllBooted();
        break;
    case 10:
        if (WifiSetupMbParent_GetState() != 2) {
            WifiSetupMbParent_KickChild(aid);
            break;
        }
        gWifiSetupMbParent->head.requestedMask |= 1 << aid;
        WifiSetupMbParent_AcceptChild(aid);
        user = MB_CommGetChildUser(aid);
        if (user != NULL) {
            MI_CpuCopy8(user, gWifiSetupMbParent->children[aid - 1].user, 0x16);
        }
        break;
    case 14:
        gWifiSetupMbParent->head.requestedMask &= ~(1 << aid);
        gWifiSetupMbParent->head.waitingMask |= 1 << aid;
        if (WifiSetupMbParent_GetState() == 3) {
            WifiSetupMbParent_StartSending(aid);
        }
        break;
    case 7:
        gWifiSetupMbParent->head.sendingMask &= ~(1 << aid);
        gWifiSetupMbParent->head.sentMask |= 1 << aid;
        break;
    case 9:
        gWifiSetupMbParent->head.sentMask &= ~(1 << aid);
        gWifiSetupMbParent->head.bootableMask |= 1 << aid;
        WifiSetupMbParent_EndIfAllBooted();
        break;
    case 12:
        if (WifiSetupMbParent_GetState() == 4) {
            WifiSetupMbParent_SetState(5);
        } else {
            WifiSetupMbParent_SetState(0);
        }
        if (gWifiSetupMbParent->segmentPtr != NULL) {
            gWifiSetupMbParent->segmentPtr = NULL;
        }
        if (gWifiSetupMbParent->mbWorkPtr != NULL) {
            gWifiSetupMbParent->mbWorkPtr = NULL;
        }
        break;
    case 13:
        switch (*(u16 *)arg) {
        case 1:
        case 2:
        case 9:
            WifiSetupMbParent_SetState(7);
            break;
        case 8:
            break;
        }
        break;
    default:
        OS_Terminate();
        break;
    }
}

void WifiSetupMbParent_SetState(int state) {
    gWifiSetupMbParent->head.state = state;
}

u16 WifiSetupMbParent_GetState(void) {
    return gWifiSetupMbParent->head.state;
}

u16 WifiSetupMbParent_GetMask(int index) {
    u8 *base = (u8 *)gWifiSetupMbParent;
    ov13_022468D8.masks[0] = (u16 *)(base + 2);
    ov13_022468D8.masks[1] = (u16 *)(base + 4);
    ov13_022468D8.masks[2] = (u16 *)(base + 6);
    ov13_022468D8.masks[3] = (u16 *)(base + 8);
    ov13_022468D8.masks[4] = (u16 *)(base + 10);
    ov13_022468D8.masks[5] = (u16 *)(base + 12);
    return *ov13_022468D8.masks[index];
}
