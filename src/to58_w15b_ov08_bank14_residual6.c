#include "to57_overlay08_bank14_private.h"
#pragma force_active on

extern void *LoadItemDataOrGfx(u16, int, int);
extern int GetItemAttr_PreloadedItemData(void *, int);
extern int GetMonData(void *, int, void *);
extern void BufferIntegerAsString(void *, int, int, int, int, int);
extern void ReadMsgDataIntoString(void *, int, void *);
extern void Heap_Free(void *);
void ov08_02220224(void *);

#define NICK_MSG(MSG) do { \
    string = NewString_ReadMsgData(work->msgData, MSG); \
    BufferBoxMonNickname(*(void **)((u8 *)work + 0x1FAC), 0, Mon_GetBoxMon(mon)); \
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), *(void **)((u8 *)work + 0x1FB0), string); \
    String_Delete(string); \
} while (0)

void ov08_02220224(void *arg) {
    Ov08R2W11Work *work = arg;
    u8 *root = work->root;
    void *mon;
    void *itemData;
    void *string;
    u16 data;
    u8 flags;
    u16 count;

    itemData = LoadItemDataOrGfx(*(u16 *)(root + 0x22), 0, *(int *)(root + 0xC));
    mon = BattleSystem_GetPartyMon(*(void **)(root + 8), *(int *)(root + 0x28), (root + root[0x11])[0x2C]);
    data = GetMonData(mon, 0xA3, 0);
    flags = 0;
    if (GetItemAttr_PreloadedItemData(itemData, 0xF)) flags |= 1;
    if (GetItemAttr_PreloadedItemData(itemData, 0x10)) flags |= 2;
    if (GetItemAttr_PreloadedItemData(itemData, 0x11)) flags |= 4;
    if (GetItemAttr_PreloadedItemData(itemData, 0x12)) flags |= 8;
    if (GetItemAttr_PreloadedItemData(itemData, 0x13)) flags |= 0x10;
    if (GetItemAttr_PreloadedItemData(itemData, 0x14)) flags |= 0x20;
    if (GetItemAttr_PreloadedItemData(itemData, 0x15)) flags |= 0x40;

    count = *(u16 *)((u8 *)work + root[0x11] * 0x50 + 0x14);
    if (count == 0 && data != 0) {
        NICK_MSG(0x58);
    } else if (data != count) {
        string = NewString_ReadMsgData(work->msgData, 0x52);
        BufferBoxMonNickname(*(void **)((u8 *)work + 0x1FAC), 0, Mon_GetBoxMon(mon));
        BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 1, data - *(u16 *)((u8 *)work + root[0x11] * 0x50 + 0x14), 3, 0, 1);
        StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), *(void **)((u8 *)work + 0x1FB0), string);
        String_Delete(string);
    } else if (GetItemAttr_PreloadedItemData(itemData, 0x24) || GetItemAttr_PreloadedItemData(itemData, 0x25)) {
        ReadMsgDataIntoString(work->msgData, 0x57, work->message);
    } else if (flags == 1) {
        NICK_MSG(0x5C);
    } else if (flags == 2) {
        NICK_MSG(0x53);
    } else if (flags == 4) {
        NICK_MSG(0x55);
    } else if (flags == 8) {
        NICK_MSG(0x56);
    } else if (flags == 0x10) {
        NICK_MSG(0x54);
    } else if (flags == 0x20) {
        NICK_MSG(0x5A);
    } else if (flags == 0x40) {
        NICK_MSG(0x5B);
    } else {
        NICK_MSG(0x59);
    }
    Heap_Free(itemData);
}
