#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
typedef struct GeoPoint69 {
    s16 x;
    s16 y;
    u8 unk04[0x26];
    u16 country;
    u16 region;
    u8 unk2E[2];
} GeoPoint69;
typedef struct Geonet69 {
    enum HeapID heapId;
    u8 unk04[0xC];
    GeoPoint69 points[1];
    u8 unk40[0xBFD4];
    Window window;
    u8 unkC024[0x4C];
    MsgData *msgData;
    u8 unkC074[0x250];
    int cursorX;
    int cursorY;
    u8 unkC2CC[0x6C];
    int active;
} Geonet69;
BOOL ov69_021E6C30(Geonet69 *work, int *index);
void ov69_021E7520(u16 country, u16 region, String *countryName, String *regionName, enum HeapID heapId);
void ov69_021E6D5C(Geonet69 *work);
// @fn ov69_021E6D5C
void ov69_021E6D5C(Geonet69 *work) {
    String *countryName;
    String *regionName;
    int index;
    if (work->active == 0) {
        String *string = String_New(1024, work->heapId);
        FillWindowPixelRect(&work->window, 15, 0, 0, 216, 48);
        ReadMsgDataIntoString(work->msgData, 15, string);
        AddTextPrinterParameterized(&work->window, 1, string, 0, 0, 0, NULL);
        String_Delete(string);
        return;
    }
    if (ov69_021E6C30(work, &index)) {
        countryName = String_New(64, work->heapId);
        regionName = String_New(64, work->heapId);
        FillWindowPixelRect(&work->window, 15, 0, 0, 216, 48);
        ov69_021E7520(work->points[index].country, work->points[index].region, countryName, regionName, work->heapId);
        if (work->points[index].region != 0) {
            AddTextPrinterParameterized(&work->window, 1, regionName, 0, 16, TEXT_SPEED_NOTRANSFER, NULL);
        }
        AddTextPrinterParameterized(&work->window, 1, countryName, 0, 0, 0, NULL);
        String_Delete(regionName);
        String_Delete(countryName);
        work->cursorX = work->points[index].x;
        work->cursorY = work->points[index].y;
    } else {
        work->active = 0;
    }
}
