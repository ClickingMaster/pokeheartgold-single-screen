#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u8 pad0[4];
    void **sock;
    u32 timeout;
    u8 retry;
    u8 index;
    u8 pad0e[0x14 - 0x0e];
    volatile u8 cur;
    u8 pad15[0xf4 - 0x15];
    u32 handles[(0x1a0 - 0xf4) / 4];
    int state;
    u8 pad1a4[0x200 - 0x1a4];
    u32 id;
    u8 pad204[0x210 - 0x204];
    u32 addrs[0x20];
    u16 ports[0x20];
    u8 pad2d0_[0x2d0 - 0x2d0];
    u8 aids[0x20];
} MatchCtx;
typedef struct {
    u8 index;
    u8 aid;
    u16 unk2;
    u32 unk4;
} ConnSlot;
extern const char ov00_02217A64[];
extern MatchCtx *ov00_021F955C(void);
extern int ov00_021F6100(u32 handle);
extern int ov00_0220F724(u32 addr, u16 port, int flag);
extern int ov00_0220D018(void *sock, int a, int b, char *name, int c, int d, u32 e, int f);
extern void ov00_021F8514(void);
extern int ov00_021EE6D0(void);
extern void **ov00_021EE72C(int index);
extern ConnSlot *ov00_021EE7A4(int index);
extern void ov00_021F2EE4(int error, int code);
extern void ov00_0220D2A4(void *conn, ConnSlot *slot);
extern void ov00_021F64C0(int flag);
void ov00_021F2BBC(void *conn, int result);
// @fn ov00_021F2BBC
void ov00_021F2BBC(void *conn, int result) {
    char name[12];
    int ret;
    if (ov00_021F955C() == NULL) {
        return;
    }
    if (ov00_021F955C()->state != 7 && ov00_021F955C()->state != 12) {
        return;
    }
    switch (result) {
    case 5:
        return;
    case 6:
        ov00_021F955C()->retry++;
        if (ov00_021F955C()->retry > 5) {
            ov00_021F955C()->retry = 0;
            ov00_021F6100(ov00_021F955C()->handles[ov00_021F955C()->cur]);
            return;
        }
        OS_SNPrintf(name, sizeof(name), ov00_02217A64, ov00_021F955C()->id);
        ret = ov00_0220D018(*ov00_021F955C()->sock, 0,
            ov00_0220F724(ov00_021F955C()->addrs[ov00_021F955C()->cur], ov00_021F955C()->ports[ov00_021F955C()->cur], 0),
            name, -1, 5000, ov00_021F955C()->timeout, 0);
        if (ret == 1) {
            ov00_021F8514();
            return;
        }
        if (ret == 0) {
            return;
        }
        if (ov00_021F6100(ov00_021F955C()->handles[ov00_021F955C()->cur])) {
            return;
        }
        break;
    default:
        if (ov00_021F6100(ov00_021F955C()->handles[ov00_021F955C()->index + 1])) {
            return;
        }
        break;
    case 0:
        int index;
        void **owner;
        ConnSlot *slot;
        if ((index = ov00_021EE6D0()) == -1) {
            ov00_021F2EE4(6, -87100);
            return;
        }
        owner = ov00_021EE72C(index);
        slot = ov00_021EE7A4(index);
        *owner = conn;
        ov00_021F955C()->index++;
        slot->index = index;
        slot->unk2 = 0;
        slot->unk4 = 0;
        slot->aid = ov00_021F955C()->aids[ov00_021F955C()->index];
        ov00_0220D2A4(conn, slot);
        if (ov00_021F955C()->state == 12) {
            ov00_021F64C0(0);
        } else {
            ov00_021F64C0(1);
        }
        break;
    }
}
