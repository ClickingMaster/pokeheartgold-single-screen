#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u32 offset; u32 size; u8 *data; u32 state; } CacheEntry;
typedef struct { u32 busy; u8 pad[0x2c]; CacheEntry entries[4]; } Cache;
typedef struct { u8 type; u8 pad; u16 child; u16 block; u16 pad2; } Header;
typedef struct { u32 f0; u32 size; u32 offset; u8 seg; u8 pad[3]; } Block;
typedef struct { u8 pad[0x10]; void *arg; void *arg2; } Task;
typedef struct {
    u8 pad0[0x1788];
    u8 download[0x1d2c - 0x1788];
    u32 table[7];          /* 0x1d2c */
    u16 sendBlock;         /* 0x1d48 */
    u16 reqBlock;
    u16 totalBlock;        /* 0x1d4c */
    u8 pad4e[4];
    u8 active;             /* 0x1d52 */
    u8 pad53;
    Cache *cache;          /* 0x1d54 */
    u32 *base;             /* 0x1d58 */
} Slot;
extern u8 *mbc;
extern void MBi_calc_sendblock(int);
extern int MBi_get_blockinfo(Block *, void *, u16, void *);
extern void *MBi_MakeParentSendBuffer(Header *, void *);
extern BOOL MBi_ReadFromCache(Cache *, u32, void *, u32);
extern int MBi_IsTaskBusy(void *);
extern void MBi_SetTask(void *, void *, void *, int);
extern void MBi_ReloadCache(void *);
extern int MBi_BlockHeaderEnd(int, u16, void *);
int MBi_CommParentSendBlock(void);
typedef struct {
    u8 download[0x5a4];
    u32 table[7];
    u16 sendBlock;
    u16 reqBlock;
    u16 totalBlock;
    u8 pad[4];
    u8 active;
    u8 pad2;
    Cache *cache;
    u32 *base;
} Child;
typedef struct {
    u8 pad[0x1788];
    Child child[16];
} Work;
#define CH(i) (((Work *)mbc)->child[i])
#define SLOT(i) ((Slot *)(mbc + (i) * 0x5d4))
#define CUR (mbc[0x1525])
typedef struct {
    u8 prefix[0x1524];
    u8 active;
    u8 cursor;
    u8 gap[0x1788 - 0x1526];
    Child child[16];
} ParentView;
// @fn MBi_CommParentSendBlock
int MBi_CommParentSendBlock(void) {
    Header hdr;
    Block info;
    u8 i;
    void *buf;
    u32 addr;
    Cache *cache;
    if (!((ParentView *)mbc)->active) {
        return 0x15;
    }
    for (i = 0; i < 16; i++) {
        ((ParentView *)mbc)->cursor = (((ParentView *)mbc)->cursor + 1) % 16;
        if (CH(CUR).active && CH(CUR).totalBlock) {
            break;
        }
    }
    if (i == 16) {
        return 0x15;
    }
    MBi_calc_sendblock(CUR);
    if (!MBi_get_blockinfo(&info, CH(CUR).table, CH(CUR).sendBlock, CH(CUR).download)) {
        return 0x15;
    }
    hdr.type = 4;
    hdr.child = CUR;
    hdr.block = SLOT(CUR)->sendBlock;
    buf = MBi_MakeParentSendBuffer(&hdr, mbc);
    addr = info.offset - SLOT(CUR)->table[info.seg] + SLOT(CUR)->base[info.seg];
    cache = SLOT(CUR)->cache;
    if (!MBi_ReadFromCache(cache, addr, buf, info.size)) {
        Task *task = (Task *)(mbc + 0x7ce0);
        if (!MBi_IsTaskBusy(task)) {
            if (cache->busy != 0) {
                cache->busy--;
            } else {
                CacheEntry *entry = NULL;
                CacheEntry *entries = cache->entries;
                int j;
                for (j = 0; j < 4; j++) {
                    if (entries[j].state == 2 && (entry == NULL || entry->offset > entries[j].offset)) {
                        entry = &entries[j];
                    }
                }
                if (entry == NULL) {
                    OS_Terminate();
                }
                cache->busy = 2;
                entry->state = 1;
                entry->offset = addr & ~0x1f;
                task->arg = entry;
                task->arg2 = cache;
                MBi_SetTask(task, MBi_ReloadCache, NULL, 4);
            }
        }
        return 0x15;
    }
    return MBi_BlockHeaderEnd(info.size + 6, SLOT(CUR)->totalBlock, mbc);
}
