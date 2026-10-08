// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct W36016Cache {u8 bytes[0x6c];u32 available;} W36016Cache;
typedef struct W36016Data{u8 prefix[0x1e8];W36016Cache part1;u8 part2[];} W36016Data;
extern u8 *mbc;extern u8 ov13_02247484;
typedef struct W36016Registration {u8 prefix[0x1f];u8 number;u8 gap20[0x24];W36016Data *data;u8 gap48[6];u16 active;u8 gap50[2];u8 used;u8 gap53;W36016Cache *cache;u8 *payload;} W36016Registration;
typedef struct GameInfo {u8 prefix[0x4b3];u8 number,padding,index;u8 gap4b6[6];struct GameInfo *next;} GameInfo;
typedef struct{u8 download[0xe4];GameInfo game;u8 blocks[20];void*file;W36016Data*data;u8 gap5c0[6];u16 active;u8 gap5c8[2],used,gap5cb;W36016Cache*cache;u8*payload;} Slot;
typedef struct {u8 prefix[0x1300];u8 gameUser[0x224];u8 count;u8 gap1525[0x263];Slot slots[16];u8 gap74c8[24];u8 taskThread[0x800];u8 taskInfo[0x20];} Context;
static inline Slot*GetSlot(u32 n){return (Slot*)(mbc+0x1788+n*0x5d4);}
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
int MBi_IsStarted(void);void MBi_MakeDownloadFileInfo(void*,void*);int MBi_MakeBlockInfoTable(void*,void*);void MBi_MakeGameInfo(void*,void*,void*);void MB_AddGameInfo(void*);int MBi_IsTaskAvailable(void);void MBi_InitTaskInfo(void*);void MBi_InitTaskThread(void*,u32);int MB_RegisterFile();
// @fn MB_RegisterFile
int MB_RegisterFile(register void*file,W36016Data*data) {
    u8 free = 255;
    OSIntrMode flags;
    u8 i;
    u32 offset;
    W36016Cache *cache;
    void *info;
    flags = OS_DisableInterrupts();
    if (!MBi_IsStarted()) {
        OS_RestoreInterrupts(flags);
        return 0;
    }
    if (B(mbc, 0x1524) + 1 > 16) {
        OS_RestoreInterrupts(flags);
        return 0;
    }
    for (i = 0; i < 16; i++) {
        u8 *cursor = (u8 *)(Context *)mbc + i * 0x5d4;
        if (*(void **)(cursor + 0x1d40) == file) {
            OS_RestoreInterrupts(flags);
            return 0;
        }
        if (!cursor[0x1d52]) {
            free = i;
            break;
        }
    }
    if (i == 16) {
        OS_RestoreInterrupts(flags);
        return 0;
    }
    offset = free * 0x5d4;
    GetSlot(free)->file = file;
    info = GetSlot(free)->download;
    MBi_MakeDownloadFileInfo(info, data);
    MI_CpuCopy8((u8 *)file + 0x1c, (u8 *)info + 0xc4, 32);
    if (!MBi_MakeBlockInfoTable(mbc + 0x1d2c + offset, info)) {
        OS_RestoreInterrupts(flags);
        return 0;
    }
    MBi_MakeGameInfo((GameInfo *)(mbc + 0x186c + offset), file, mbc + 0x1300);
    GetSlot(free)->game.index = free;
    MB_AddGameInfo((GameInfo *)(mbc + 0x186c + offset));
    {
        s16 number;
        number = ov13_02247484;
        GetSlot(free)->game.number = number;
        GetSlot(free)->active = 1;
        {
            cache = &data->part1;
            GetSlot(free)->data = data;
            u8 *payload = data->part2;
            GetSlot(free)->cache = cache;
            number = number + 1;
            GetSlot(free)->payload = payload;
            ov13_02247484 = number;
        }
    }
    if (GetSlot(free)->cache->available && !MBi_IsTaskAvailable()) {
        MBi_InitTaskInfo(mbc + 0x7ce0);
        MBi_InitTaskThread(mbc + 0x74e0, 0x800);
    }
    GetSlot(free)->used = 1;
    B(mbc, 0x1524)++;
    OS_RestoreInterrupts(flags);
    return 1;
}
