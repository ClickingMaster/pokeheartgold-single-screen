#ifndef POKEHEARTGOLD_SLOP_R1_OVERLAY29_NUMBER_ENTRY_WINDOWS_INTERNAL_H
#define POKEHEARTGOLD_SLOP_R1_OVERLAY29_NUMBER_ENTRY_WINDOWS_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

typedef struct Ov29NumberEntryDataSlopR1 {
    BgConfig *bgConfig;
    u8 pad004[0x19C];
    Window windows[3];
} Ov29NumberEntryDataSlopR1;

extern const WindowTemplate ov29_0225E178[3];

void ov29_0225D714(Ov29NumberEntryDataSlopR1 *data);

#endif
