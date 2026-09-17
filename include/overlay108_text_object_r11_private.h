#ifndef TO50_OV108_R11_PRIVATE_H
#define TO50_OV108_R11_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "sprite_system.h"
#include "text.h"
#include "unk_02013534.h"

typedef struct Ov108WorkR11 {
    enum HeapID heapID;
    u8 pad004[0x310 - 4];
    String *string;
    u8 pad314[0x350 - 0x314];
    SpriteManager *spriteManager;
    u8 pad354[0x39C - 0x354];
    UnkStruct_02013534 *fontSystem;
    UnkStruct_02013910 *fontLayout;
    TextOBJ *textObj;
    UnkStruct_02021AC8 charTransfer;
    u8 pad3B4[0x4A4 - 0x3B4];
    Window window;
} Ov108WorkR11;

void ov108_021E756C(Ov108WorkR11 *work);
#endif
