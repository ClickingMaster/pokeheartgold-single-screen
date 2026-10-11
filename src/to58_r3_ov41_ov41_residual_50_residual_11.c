#include "global.h"

void DressUpHud_PrintMsgInstant(void *work, void *a, void *b, void *c);
void DressUpHud_PrintMsg(void *work, void *a, void *b, void *c);

void GF_AssertFail(void);
void ov41_0224AD90(void *, void *, void *, void *, int, int, int);
void ov41_0224ADD8(void *, void *, void *, void *, int, int, u32, void *);

void DressUpHud_PrintMsgInstant(void *work, void *a, void *b, void *c)
{
    if ((*(u32 *)((u8 *)work + 0x138) & 4) == 0) {
        GF_AssertFail();
    }
    ov41_0224AD90(*(void **)((u8 *)work + 0x2c), a, b, c, 0, 0, 0xff);
}

void DressUpHud_PrintMsg(void *work, void *a, void *b, void *c)
{
    if ((*(u32 *)((u8 *)work + 0x138) & 4) == 0) {
        GF_AssertFail();
    }
    ov41_0224ADD8(*(void **)((u8 *)work + 0x2c), a, b, c, 0, 0,
                   *(u32 *)((u8 *)work + 0x140), (u8 *)work + 0x144);
}
