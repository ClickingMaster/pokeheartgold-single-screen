#include "global.h"

#include "constants/save_arrays.h"

#include "save_gts.h"
typedef struct GtsSave {
    u8 storedMon[0xEC]; // the deposited Pokemon (party format), kept until it is traded or taken back
    u16 hasStoredMon;
    u16 storedMonBox;
    u32 depositDate; // packed RTCDate: year << 24 | month << 16 | day << 8 | week
    u32 tradeDate;
} GtsSave;
int Save_Gts_Get(SaveData *save);
u32 Save_Gts_sizeof(void);
void Save_Gts_Init(GtsSave *data);
u16 GtsSave_HasStoredMon(GtsSave *data);
void GtsSave_SetHasStoredMon(GtsSave *data, u16 value);
void GtsSave_GetStoredMon(GtsSave *data, void *out);
void GtsSave_SetStoredMon(GtsSave *data, void *src);
u32 GtsSave_GetDepositDate(GtsSave *data);
void GtsSave_SetDepositDate(GtsSave *data, u32 value);
u32 GtsSave_GetTradeDate(GtsSave *data);
void GtsSave_SetTradeDate(GtsSave *data, u32 value);
u16 GtsSave_GetStoredMonBox(GtsSave *data);
int Save_Gts_Get(SaveData *save) {
    return (int)SaveArray_Get(save, SAVE_GTS);
}
u32 Save_Gts_sizeof(void) {
    return sizeof(GtsSave);
}
void Save_Gts_Init(GtsSave *data) {
    MI_CpuClearFast(data, sizeof(*data));
}
u16 GtsSave_HasStoredMon(GtsSave *data) {
    return data->hasStoredMon;
}
void GtsSave_SetHasStoredMon(GtsSave *data, u16 value) {
    data->hasStoredMon = value;
}
void GtsSave_GetStoredMon(GtsSave *data, void *out) {
    MI_CpuCopyFast(data->storedMon, out, 0xEC);
}
void GtsSave_SetStoredMon(GtsSave *data, void *src) {
    MI_CpuCopyFast(src, data->storedMon, 0xEC);
}
u32 GtsSave_GetDepositDate(GtsSave *data) {
    return data->depositDate;
}
void GtsSave_SetDepositDate(GtsSave *data, u32 value) {
    data->depositDate = value;
}
u32 GtsSave_GetTradeDate(GtsSave *data) {
    return data->tradeDate;
}
void GtsSave_SetTradeDate(GtsSave *data, u32 value) {
    data->tradeDate = value;
}
u16 GtsSave_GetStoredMonBox(GtsSave *data) {
    return data->storedMonBox;
}
