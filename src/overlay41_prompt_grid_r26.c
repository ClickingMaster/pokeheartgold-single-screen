#include "overlay41_prompt_grid_r26_internal.h"
#define PTR(p, o) (*(void **)((p) + (o)))
#define U32(p, o) (*(u32 *)((p) + (o)))

typedef struct PromptTemplateR26 {
    void *bg;
    u32 a4;
    u32 a8;
    u32 ac;
    u8 x10;
    u8 x11;
    u8 x12 : 4;
    u8 x12_hi : 4;
    u8 pad13;
} PromptTemplateR26;

void ov41_02247D64(u8 *work) {
    PromptTemplateR26 t;
    vu16 *reg = (vu16 *)0x04000008;
    MI_CpuFill8(&t, 0, sizeof(t));
    t.bg = PTR(work, 0x40);
    t.a4 = 3;
    t.a8 = 0;
    t.ac = 5;
    t.x10 = 0x19;
    t.x11 = 4;
    t.x12 = U32(work, 0x6ec);
    YesNoPrompt_Reset(PTR(work, 0x6b8));
    YesNoPrompt_InitFromTemplate(PTR(work, 0x6b8), &t);
    ov41_02247D34(work, 2);
    reg[0] = (reg[0] & ~3) | 2;
    reg[1] = (reg[1] & ~3) | 1;
    reg[2] = (reg[2] & ~3) | 3;
    reg[3] &= ~3;
}

int ov41_02247DF8(u8 *work) {
    int result = YesNoPrompt_HandleInput(PTR(work, 0x6b8));
    switch (result) {
    case 0:
        return 5;
    case 1:
        result = 6;
        break;
    case 2:
        result = 7;
        break;
    }
    U32(work, 0x6ec) = YesNoPrompt_IsInTouchMode(PTR(work, 0x6b8));
    return result;
}

void ov41_02247E34(u32 minX, u32 maxX, u32 minY, u32 maxY, u32 index, u32 *outY, u32 *outX) {
    u32 dy = (maxY - minY) / 3;
    u32 y1 = minY + dy;
    u32 y2 = minY + dy * 2;
    u32 dx = (maxX - minX) / 3;
    u32 x1 = minX + dx;
    u32 x2 = minX + dx * 2;
    switch (index) {
    case 0:
        *outY = minY;
        *outX = minX;
        return;
    case 1:
        *outY = minY;
        *outX = x1;
        return;
    case 2:
        *outY = minY;
        *outX = x2;
        return;
    case 3:
        *outY = minY;
        *outX = maxX;
        return;
    case 4:
        *outY = y1;
        *outX = minX;
        return;
    case 5:
        *outY = y1;
        *outX = x1;
        return;
    case 6:
        *outY = y1;
        *outX = x2;
        return;
    case 7:
        *outY = y1;
        *outX = maxX;
        return;
    case 8:
        *outY = y2;
        *outX = minX;
        return;
    case 9:
        *outY = y2;
        *outX = x1;
        return;
    case 10:
        *outY = y2;
        *outX = x2;
        return;
    case 11:
        *outY = y2;
        *outX = maxX;
        return;
    case 12:
        *outY = maxY;
        *outX = minX;
        return;
    case 13:
        *outY = maxY;
        *outX = x1;
        return;
    case 14:
        *outY = maxY;
        *outX = x2;
        return;
    case 15:
        *outY = maxY;
        *outX = maxX;
        return;
    }
}

void ov41_02247F3C(u8 *work, const u32 *args) {
    U32(work, 0x2c) = args[0];
    U32(work, 0x30) = args[1];
    U32(work, 0x34) = args[2];
    U32(work, 0x38) = args[3];
    U32(work, 0x3c) = args[4];
    U32(work, 0x40) = args[5];
    U32(work, 0x44) = args[6];
    U32(work, 0x74) = 0;
    U32(work, 0) = args[7];
    PTR(work, 0xc) = work + 4;
    PTR(work, 0x10) = work + 4;
    PTR(work, 0x1c) = work + 0x14;
    PTR(work, 0x20) = work + 0x14;
    U32(work, 0x24) = 0;
    U32(work, 0x28) = args[8];
    ov41_02249A50(ov41_022499F0(PTR(work, 0), work + 0x78, 3), work + 0x14);
}
