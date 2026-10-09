#pragma force_active on
#include "global.h"
#include "assert.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "string_util.h"

typedef struct Ov39Config {
    u32 data[11];
} Ov39Config;

typedef struct Ov39Args {
    u32 unk0;
    enum HeapID heapID;
    u32 unk8;
    Ov39Config config;
    u32 unk38;
    u32 unk3C;
} Ov39Args;

typedef struct Ov39Work {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u8 version;
    u8 unk11;
    u8 unk12[0x132];
    enum HeapID heapID;
    u8 unk148[4];
    int unk14C;
    Ov39Config config;
    u8 unk17C[0x10];
    void *unk18C;
    u8 unk190[0x224];
    void *buffer;
    int unk3B8;
    u8 unk3BC[8];
    int unk3C4;
    u8 unk3C8[0x20];
    u32 unk3E8;
    u32 unk3EC;
    u8 unk3F0[4];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    u8 unk400[0x14];
} Ov39Work;

u32 ov39_0222A2C0(int a0);
void *ov39_02227DEC(enum HeapID heapID);
BOOL ov39_0222A110(u32 *a0);
BOOL ov39_02227208(Ov39Work *work, Ov39Args *args);

// @fn ov39_02227208
BOOL ov39_02227208(Ov39Work *work, Ov39Args *args) {
    MI_CpuFill8(work, 0, sizeof(Ov39Work));
    work->heapID = args->heapID;
    work->unk0 = args->unk8;
    work->config = args->config;
    work->unk8 = args->unk3C;
    work->unk4 = args->unk38;
    work->unk14C = -1;
    work->unk3E8 = 0x59DC;
    work->unk3EC = 0x59DC;
    work->buffer = Heap_Alloc(args->heapID, ov39_0222A2C0(-1));
    MI_CpuFill8(work->buffer, 0, ov39_0222A2C0(-1));
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 800, args->heapID);
    work->msgFormat = MessageFormat_New(args->heapID);
    work->string = String_New(256, args->heapID);
    work->unk18C = ov39_02227DEC(args->heapID);
    work->unkC = args->unk0;
    work->version = GAME_VERSION;
    work->unk11 = 2;
    GF_ASSERT(ov39_0222A110(&work->unkC) == TRUE);
    work->unk3C4 = 1;
    work->unk3B8 = 1;
    return TRUE;
}
