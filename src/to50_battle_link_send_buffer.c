#include "to50_battle_link_send_buffer_internal.h"

const u32 _020FFE30[] = {
    (u32)sub_02075534,
    (u32)sub_020342B8,
    0,
    (u32)sub_02075028,
    (u32)sub_020342B8,
    0,
    (u32)sub_020750B4,
    (u32)sub_02074ED8,
    0,
    (u32)sub_0207513C,
    (u32)sub_02074EDC,
    (u32)sub_02074EF8,
    (u32)sub_020751A8,
    (u32)sub_02074EF4,
    (u32)sub_02074F18,
    (u32)sub_02075210,
    (u32)sub_02074EE4,
    (u32)sub_02074F38,
    (u32)sub_0207530C,
    (u32)sub_02074EEC,
    (u32)sub_02074F54,
    (u32)sub_02075398,
    (u32)sub_02074EF4,
    (u32)sub_02074F74,
    (u32)sub_02075398,
    (u32)sub_02074EF4,
    (u32)sub_02074F7C,
    (u32)sub_02075424,
    (u32)sub_02074EE4,
    (u32)sub_02074F84,
    (u32)sub_02075424,
    (u32)sub_02074EE4,
    (u32)sub_02074F8C,
    (u32)sub_020755B4,
    (u32)sub_020755E4,
    (u32)sub_02074F94,
};

void sub_02074F9C(BattleSystem *battle, int command, int battler, u8 *data, u8 size) {
    u8 *header;
    u8 *buffer;
    u16 *write;
    u16 *end;
    int i;

    header = Heap_Alloc((enum HeapID)5, 4);
    buffer = BattleSystem_GetSendBufferPtr(battle);
    write = ov12_0223A960(battle);
    end = ov12_0223A96C(battle);
    if ((u32)(*write + 5 + size) > 0x1000) {
        *end = *write;
        *write = 0;
    }
    header[0] = command;
    header[1] = battler;
    *(u16 *)&header[2] = size;
    for (i = 0; (u32)i < 4; i++) {
        buffer[*write] = header[i];
        (*write)++;
    }
    for (i = 0; i < size; i++) {
        buffer[*write] = data[i];
        (*write)++;
    }
    Heap_Free(header);
}
