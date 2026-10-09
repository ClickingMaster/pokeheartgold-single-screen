#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 blockList;
    u32 szByte;
    u32 szByteFor4x4;
    void *pWork;
    u32 szWork;
} TexMgrInfo;

typedef struct {
    u32 szFree;
    u32 szNrm;
    u32 sz4x4;
} TexSlot;

typedef struct {
    TexSlot slot[4];
} TexSlotTbl;

u32 _021D84E8;
u32 _021D84E4;
TexMgrInfo _021D84EC;

extern const TexSlotTbl _021092A0;

extern u32 NNSi_GfdInitLnkVramBlockPool(void *, u32);
extern void NNSi_GfdInitLnkVramMan(void *);
extern void NNSi_GfdAddNewFreeBlock(void *, void *, u32, u32);
extern void NNSi_GfdMergeAllFreeBlocks(void *, void *);
void NNS_GfdResetLnkTexVramState(void);

// @fn NNS_GfdResetLnkTexVramState
void NNS_GfdResetLnkTexVramState(void)
{
    u32 restNrm;
    TexSlotTbl tbl = _021092A0;
    TexSlot *t;
    u32 j;
    TexSlot *s;
    u32 rest4x4 = ((u32 *)&_021D84E4)[4];
    restNrm = ((u32 *)&_021D84E4)[3] - (rest4x4 + rest4x4 / 2);
    u32 pltt = rest4x4 / 2;
    u32 i;
    u32 n;
    for (i = 0, s = tbl.slot; i < 4; i++, s++)
    {
        if (i == 0 || i == 2)
        {
            if (s->szFree != 0 && rest4x4 != 0)
            {
                n = s->szFree;
                if (n > rest4x4)
                {
                    n = rest4x4;
                }
                s->sz4x4 += n;
                rest4x4 -= n;
                s->szFree -= n;
            }
        }
    }
    tbl.slot[1].szFree -= pltt;
    for (j = 0, t = tbl.slot; j < 4; j++, t++)
    {
        if (t->szFree != 0 && restNrm != 0)
        {
            n = t->szFree;
            if (n > restNrm)
            {
                n = restNrm;
            }
            t->szNrm += n;
            t->szFree -= n;
            restNrm -= n;
        }
    }
    NNSi_GfdInitLnkVramMan(&_021D84E4);
    NNSi_GfdInitLnkVramMan(&_021D84E8);
    _021D84EC.blockList = NNSi_GfdInitLnkVramBlockPool(_021D84EC.pWork, _021D84EC.szWork / 16);
    if (tbl.slot[0].sz4x4 != 0)
    {
        NNSi_GfdAddNewFreeBlock(&_021D84E8, &_021D84EC, 0, tbl.slot[0].sz4x4);
    }
    {
        u32 sz = tbl.slot[0].szNrm;
        u32 ofs = tbl.slot[0].sz4x4;
        if (sz != 0)
            NNSi_GfdAddNewFreeBlock(&_021D84E4, &_021D84EC, ofs, sz);
    }
    if (tbl.slot[2].sz4x4 != 0)
    {
        NNSi_GfdAddNewFreeBlock(&_021D84E8, &_021D84EC, 0x40000, tbl.slot[2].sz4x4);
    }
    {
        u32 sz = tbl.slot[2].szNrm;
        u32 ofs = tbl.slot[2].sz4x4;
        if (sz != 0)
            NNSi_GfdAddNewFreeBlock(&_021D84E4, &_021D84EC, 0x40000 + ofs, sz);
    }
    if (tbl.slot[3].szNrm != 0)
    {
        NNSi_GfdAddNewFreeBlock(&_021D84E4, &_021D84EC, 0x60000, tbl.slot[3].szNrm);
    }
    if (tbl.slot[1].szNrm != 0)
    {
        NNSi_GfdAddNewFreeBlock(&_021D84E4, &_021D84EC, pltt + 0x20000, tbl.slot[1].szNrm);
    }
    NNSi_GfdMergeAllFreeBlocks(&_021D84E4, &_021D84EC);
    NNSi_GfdMergeAllFreeBlocks(&_021D84E8, &_021D84EC);
}
