#include "global.h"

extern void *FrontierData_GetWifiWinCount(void *);
extern void BattleRoomDownload_GetTrainersRoom(void *, void *);
extern void sub_02069528(void *, u32, void *);
extern void NintendoWfcHttp_RequestUpload(u8, u8, void *, void *);
int ov73_021E95CC(void *);

int ov73_021E95CC(void *work) {
    void *record = FrontierData_GetWifiWinCount(*(void **)*(void **)work);
    u8 pair[4];
    BattleRoomDownload_GetTrainersRoom(*(void **)((u8 *)*(void **)work + 4), pair);
    sub_02069528(*(void **)((u8 *)*(void **)work + 12), 1, (u8 *)work + 0xAD8);
    NintendoWfcHttp_RequestUpload(pair[0], pair[1], record, (u8 *)work + 0xAD8);
    *(u32 *)((u8 *)work + 0x1c) = 27;
    *(u32 *)((u8 *)work + 0xF94) = 0;
    return 3;
}
