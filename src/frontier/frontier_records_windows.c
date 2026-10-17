#include "frontier/frontier_records.h"

#include "global.h"

typedef struct FrontierRecordsWindowSet {
    const WindowTemplate *templates;
    u32 count;
} FrontierRecordsWindowSet;

static const WindowTemplate sArcadeWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2, 1,  28, 2, 13, 1    },
    { GF_BG_LYR_MAIN_0, 2, 7,  23, 2, 13, 0x39 },
    { GF_BG_LYR_MAIN_0, 2, 10, 23, 2, 13, 0x67 },
};

static const WindowTemplate sCastleWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2,  1,  28, 2, 13, 1    },
    { GF_BG_LYR_MAIN_0, 16, 7,  14, 2, 13, 0x39 },
    { GF_BG_LYR_MAIN_0, 2,  11, 28, 2, 13, 0x55 },
    { GF_BG_LYR_MAIN_0, 2,  14, 28, 2, 13, 0x8D },
};

static const WindowTemplate sFactoryWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2, 1,  28, 2, 13, 1     },
    { GF_BG_LYR_MAIN_0, 2, 4,  28, 2, 13, 0x39  },
    { GF_BG_LYR_MAIN_0, 2, 7,  28, 2, 13, 0x71  },
    { GF_BG_LYR_MAIN_0, 2, 9,  28, 2, 13, 0xA9  },
    { GF_BG_LYR_MAIN_0, 2, 12, 28, 2, 13, 0xE1  },
    { GF_BG_LYR_MAIN_0, 2, 15, 28, 2, 13, 0x119 },
    { GF_BG_LYR_MAIN_0, 2, 17, 28, 2, 13, 0x151 },
};

static const WindowTemplate sTowerWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2, 1,  28, 2, 13, 1     },
    { GF_BG_LYR_MAIN_0, 2, 4,  16, 2, 13, 0x39  },
    { GF_BG_LYR_MAIN_0, 2, 7,  23, 2, 13, 0x59  },
    { GF_BG_LYR_MAIN_0, 2, 10, 23, 2, 13, 0x87  },
    { GF_BG_LYR_MAIN_0, 2, 7,  23, 2, 13, 0x59  },
    { GF_BG_LYR_MAIN_0, 2, 9,  23, 2, 13, 0x87  },
    { GF_BG_LYR_MAIN_0, 2, 12, 16, 2, 13, 0xB5  },
    { GF_BG_LYR_MAIN_0, 2, 15, 23, 2, 13, 0xD5  },
    { GF_BG_LYR_MAIN_0, 2, 17, 23, 2, 13, 0x103 },
};

static const WindowTemplate sHallWindowTemplates[] = {
    { GF_BG_LYR_MAIN_0, 2,  1,  28, 2,  13, 1     },
    { GF_BG_LYR_MAIN_0, 2,  7,  18, 2,  13, 0x39  },
    { GF_BG_LYR_MAIN_0, 2,  10, 23, 2,  13, 0x5D  },
    { GF_BG_LYR_MAIN_0, 2,  13, 23, 2,  13, 0x8B  },
    { GF_BG_LYR_MAIN_0, 1,  5,  29, 14, 13, 0xB9  },
    { GF_BG_LYR_MAIN_0, 6,  5,  9,  2,  13, 0x24F },
    { GF_BG_LYR_MAIN_0, 19, 5,  9,  2,  13, 0x261 },
    { GF_BG_LYR_MAIN_0, 6,  9,  9,  2,  13, 0x273 },
    { GF_BG_LYR_MAIN_0, 19, 9,  9,  2,  13, 0x285 },
    { GF_BG_LYR_MAIN_0, 6,  13, 9,  2,  13, 0x297 },
    { GF_BG_LYR_MAIN_0, 19, 13, 9,  2,  13, 0x2A9 },
    { GF_BG_LYR_MAIN_0, 6,  17, 9,  2,  13, 0x2BB },
    { GF_BG_LYR_MAIN_0, 19, 17, 9,  2,  13, 0x2CD },
    { GF_BG_LYR_MAIN_0, 6,  5,  9,  2,  13, 0x2DF },
    { GF_BG_LYR_MAIN_0, 19, 5,  9,  2,  13, 0x2F1 },
    { GF_BG_LYR_MAIN_0, 6,  9,  9,  2,  13, 0x303 },
    { GF_BG_LYR_MAIN_0, 19, 9,  9,  2,  13, 0x315 },
    { GF_BG_LYR_MAIN_0, 6,  13, 9,  2,  13, 0x327 },
    { GF_BG_LYR_MAIN_0, 19, 13, 9,  2,  13, 0x339 },
    { GF_BG_LYR_MAIN_0, 6,  17, 9,  2,  13, 0x34B },
    { GF_BG_LYR_MAIN_0, 19, 17, 9,  2,  13, 0x35D },
    { GF_BG_LYR_MAIN_0, 2,  2,  2,  2,  13, 0x36F },
    { GF_BG_LYR_MAIN_0, 15, 21, 4,  2,  13, 0x373 },
    { GF_BG_LYR_SUB_0,  2,  19, 27, 4,  13, 1     },
};

static const FrontierRecordsWindowSet sWindowSets[] = {
    { sFactoryWindowTemplates, NELEMS(sFactoryWindowTemplates) },
    { sHallWindowTemplates,    NELEMS(sHallWindowTemplates)    },
    { sCastleWindowTemplates,  NELEMS(sCastleWindowTemplates)  },
    { sArcadeWindowTemplates,  NELEMS(sArcadeWindowTemplates)  },
    { sTowerWindowTemplates,   NELEMS(sTowerWindowTemplates)   },
};

void FrontierRecordsApp_AddWindows(BgConfig *bgConfig, Window *windows, int set) {
    u8 i;
    const WindowTemplate *templates = sWindowSets[set].templates;
    u32 count = sWindowSets[set].count;

    for (i = 0; i < count; i++) {
        AddWindow(bgConfig, &windows[i], &templates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void FrontierRecordsApp_RemoveWindows(Window *windows, int set) {
    u16 i;
    u32 count = sWindowSets[set].count;

    for (i = 0; i < count; i++) {
        RemoveWindow(&windows[i]);
    }
}

void FrontierRecordsApp_AddReturnButtonWindow(BgConfig *bgConfig, Window *window) {
    AddWindowParameterized(bgConfig, window, GF_BG_LYR_MAIN_0, 25, 21, 6, 2, 13, 0x3F4);
}

void FrontierRecordsApp_RemoveReturnButtonWindow(Window *window) {
    RemoveWindow(window);
}
