#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u16 state;          /* 0x00 */
    u8 pad2[0xc6 - 2];
    u16 xc6;            /* 0xc6 */
    u8 padc8[0xe6 - 0xc8];
    u16 xe6;            /* 0xe6 */
} WmspStatus;
typedef struct { u8 pad[0x1550]; WmspStatus *status; } WmspWork;
extern WmspWork wmspW;
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuFill8(void *dst, u8 data, u32 size);
extern u16 *WMSP_WL_DevGetStationState(u16 *buf);
extern int WMSP_SetAllParams(u16 cmd, u16 *buf);
extern u16 *WMSP_WL_DevClass1(u16 *buf);
extern u16 *WMSP_WL_DevIdle(u16 *buf);
extern u16 *WMSP_WL_MlmePowerManagement(u16 *buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims);
extern u16 *WMSP_WL_MlmeMeasureChannel(u16 *buf, u16 ccaMode, u16 edThreshold, u16 measureTime, u8 *channelList);
extern void WmspError_06005BB8(u16 code, u16 result);
void WMSP_MeasureChannel(void *msg);
// @fn WMSP_MeasureChannel
void WMSP_MeasureChannel(void *msg) {
    u16 *req = msg;
    u8 channelList[16];
    u16 bufArr[256];
    u16 *buf = bufArr;
    WmspStatus *status;
    u16 *rsp;
    u16 *cb;
    u16 ccaMode, edThreshold, measureTime;
    u16 ccaBusyRatio, channel;

    if (wmspW.status->state != 2) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 0x1e;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status = wmspW.status;
    rsp = WMSP_WL_DevGetStationState(buf);
    if (rsp[2] != 0) {
        WmspError_06005BB8(0x308, rsp[2]);
        return;
    }
    {
        u16 staState = rsp[3];
        status->xe6 = 2;
        if (staState == 0x10) {
            if (!WMSP_SetAllParams(10, buf)) {
                return;
            }
            rsp = WMSP_WL_DevClass1(buf);
            if (rsp[2] != 0) {
                WmspError_06005BB8(0x303, rsp[2]);
                return;
            }
            status->state = 3;
            rsp = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);
            if (rsp[2] != 0) {
                WmspError_06005BB8(1, rsp[2]);
                return;
            }
            status->xc6 = 1;
        }
    }
    {
        u16 ch;
        ccaMode = req[1];
        edThreshold = req[2];
        ch = req[3];
        measureTime = req[4];
        __VENEER_MI_CpuFill8(channelList, 0, 16);
        channelList[0] = ch;
    }
    rsp = WMSP_WL_MlmeMeasureChannel(buf, ccaMode, edThreshold, measureTime, channelList);
    if (rsp[2] != 0) {
        WmspError_06005BB8(10, rsp[2]);
        return;
    }
    ccaBusyRatio = rsp[4] & 0xff;
    channel = rsp[4] >> 8;
    rsp = WMSP_WL_DevIdle(buf);
    if (rsp[2] != 0) {
        WmspError_06005BB8(0x302, rsp[2]);
        return;
    }
    status->state = 2;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x1e;
    cb[1] = 0;
    cb[4] = ccaBusyRatio;
    cb[5] = channel;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}
