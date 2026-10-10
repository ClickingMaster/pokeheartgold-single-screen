#pragma force_active on
#include <nitro/types.h>

#include "SND_work.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma dollar_identifiers on
typedef char ChannelExtent[sizeof(struct SNDExChannel) == 0x54 ? 1 : -1];
extern const u8 attack_table$3790[19];
void SND_SetExChannelAttack(void *, int);

void SND_SetExChannelAttack(void *channel, int attack) {
    u8 *bytes = channel;
    if (attack < 109) {
        bytes[0x1c] = (u8)(255u - (u32)attack);
    } else {
        bytes[0x1c] = attack_table$3790[127 - attack];
    }
}
