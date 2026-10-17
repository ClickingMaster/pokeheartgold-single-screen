#include "global.h"

#include "constants/sndseq.h"

#include "application/pokedex/pokedex_internal.h"

#include "dpad_menu_box.h"
#include "grid_input_handler.h"
#include "menu_input_state.h"
#include "pokedex.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

static const u8 sSearchWeightCursorAnims[4] = { 62, 62, 35, 35 };

static const u8 sSearchHeightCursorAnims[4] = { 62, 62, 35, 35 };

static const TouchscreenHitbox sPokedexEntryMenuHitboxes[] = {
    { .rect = { 72, 103, 48, 207 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const u8 sSearchAreaCursorAnims[8] = { 38, 38, 38, 38, 35, 35, 0, 0 };

static const u8 sSearchOrderCursorAnims[8] = { 39, 39, 39, 39, 39, 39, 35, 35 };

static const GridCallbacks sSearchWeightGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchWeightGridMove,
    .onTouch = PokedexApp_OnSearchWeightGridTouch,
};

static const GridCallbacks sSearchNameGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchNameGridMove,
    .onTouch = PokedexApp_OnSearchNameGridTouch,
};

static const GridCallbacks sSearchFormGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchFormGridMove,
    .onTouch = PokedexApp_OnSearchFormGridTouch,
};

static const GridCallbacks sSearchOrderGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchOrderGridMove,
    .onTouch = PokedexApp_OnSearchOrderGridTouch,
};

static const GridCallbacks sSearchTypeGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchTypeGridMove,
    .onTouch = PokedexApp_OnSearchTypeGridTouch,
};

static const GridCallbacks sSearchHeightGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchHeightGridMove,
    .onTouch = PokedexApp_OnSearchHeightGridTouch,
};

static const int sPokedexEntryTabHitResults[4] = { 0, 1, 2, 3 };

static const GridCallbacks sSearchMenuGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchMenuGridMove,
    .onTouch = PokedexApp_OnSearchMenuGridTouch,
};

static const GridCallbacks sSearchAreaGridCallbacks = {
    .onButton = PokedexApp_DummyGridCallback,
    .onSwitchToTouchMode = PokedexApp_DummyGridCallback,
    .onKeyMove = PokedexApp_OnSearchAreaGridMove,
    .onTouch = PokedexApp_OnSearchAreaGridTouch,
};

static const TouchscreenHitbox sPokedexSearchSliderHitboxes[] = {
    { .rect = { 49, 70, 230, 245 } },
    { .rect = { 49, 70, 10, 25 } },
    { .rect = { 121, 142, 230, 245 } },
    { .rect = { 121, 142, 10, 25 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sSearchHeightHitboxes[] = {
    { .rect = { 46, 73, 26, 229 } },
    { .rect = { 118, 145, 26, 229 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sSearchWeightHitboxes[] = {
    { .rect = { 46, 73, 26, 229 } },
    { .rect = { 118, 145, 26, 229 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const int sPokedexListMenuHitResults[5] = { 6, 5, 1, 0, 2 };

static const u8 sSearchTypeCursorAnims[20] = { 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 35, 35 };

static const TouchscreenHitbox sPokedexListMenuHitboxes[] = {
    { .rect = { 112, 143, 96, 160 } },
    { .rect = { 0, 151, 0, 255 } },
    { .rect = { 152, 187, 0, 63 } },
    { .rect = { 152, 187, 64, 191 } },
    { .rect = { 152, 187, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sSearchAreaHitboxes[] = {
    { .rect = { 64, 79, 48, 103 } },
    { .rect = { 64, 79, 152, 207 } },
    { .rect = { 120, 135, 48, 103 } },
    { .rect = { 120, 135, 152, 207 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const int sPokedexFormsHitResults[8] = { 4, 5, 6, 7, 8, 9, 10, 11 };

static const DpadMenuBox sSearchHeightDpadBoxes[] = {
    { 52,  60,  0, 0, 2, 1,   0, 0 },
    { 204, 132, 0, 0, 0, 130, 1, 1 },
    { 40,  176, 0, 0, 1, 0,   3, 3 },
    { 216, 176, 0, 0, 1, 0,   2, 2 },
};

static const DpadMenuBox sSearchWeightDpadBoxes[] = {
    { 52,  60,  0, 0, 2, 1,   0, 0 },
    { 204, 132, 0, 0, 0, 130, 1, 1 },
    { 40,  176, 0, 0, 1, 0,   3, 3 },
    { 216, 176, 0, 0, 1, 0,   2, 2 },
};

static const int sPokedexFormCompareHitResults[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

static const TouchscreenHitbox sPokedexFormCompareHitboxes[] = {
    { .rect = { 48, 63, 56, 71 } },
    { .rect = { 120, 136, 56, 71 } },
    { .rect = { 48, 63, 184, 199 } },
    { .rect = { 120, 136, 184, 199 } },
    { .rect = { 160, 183, 8, 119 } },
    { .rect = { 160, 183, 128, 255 } },
    { .rect = { 67, 116, 2, 114 } },
    { .rect = { 67, 116, 130, 253 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sPokedexFormsHitboxes[] = {
    { .rect = { 8, 23, 34, 213 } },
    { .rect = { 32, 47, 34, 213 } },
    { .rect = { 104, 119, 34, 213 } },
    { .rect = { 80, 95, 34, 213 } },
    { .rect = { 5, 20, 231, 252 } },
    { .rect = { 107, 122, 231, 252 } },
    { .rect = { 21, 106, 231, 252 } },
    { .rect = { 134, 154, 134, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sSearchOrderHitboxes[] = {
    { .rect = { 48, 63, 24, 111 } },
    { .rect = { 48, 63, 144, 231 } },
    { .rect = { 80, 95, 24, 111 } },
    { .rect = { 80, 95, 144, 231 } },
    { .rect = { 112, 127, 24, 111 } },
    { .rect = { 112, 127, 144, 231 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const int sPokedexListHitResults[11] = { 0, 14, 9, 11, 13, 7, 8, 2, 3, 4, 5 };

static const TouchscreenHitbox sSearchMenuHitboxes[] = {
    { .rect = { 8, 23, 128, 215 } },
    { .rect = { 32, 47, 56, 127 } },
    { .rect = { 56, 71, 56, 177 } },
    { .rect = { 80, 95, 56, 183 } },
    { .rect = { 104, 119, 56, 239 } },
    { .rect = { 128, 143, 56, 111 } },
    { .rect = { 56, 87, 208, 239 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 92, 163 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sPokedexListHitboxes[] = {
    { .rect = { 4, 123, 27, 226 } },
    { .rect = { 0, 124, 0, 23 } },
    { .rect = { 5, 20, 231, 252 } },
    { .rect = { 131, 146, 231, 252 } },
    { .rect = { 21, 130, 231, 252 } },
    { .rect = { 128, 143, 48, 79 } },
    { .rect = { 128, 143, 168, 199 } },
    { .rect = { 152, 187, 0, 63 } },
    { .rect = { 152, 187, 64, 127 } },
    { .rect = { 152, 187, 128, 191 } },
    { .rect = { 152, 187, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sSearchAreaDpadBoxes[] = {
    { 76,  72,  0, 0, 4, 2, 1, 1 },
    { 180, 72,  0, 0, 5, 3, 0, 0 },
    { 76,  128, 0, 0, 0, 4, 3, 3 },
    { 180, 128, 0, 0, 1, 5, 2, 2 },
    { 40,  176, 0, 0, 2, 0, 5, 5 },
    { 216, 176, 0, 0, 3, 1, 4, 4 },
};

static const int sPokedexAreaHitResults[12] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

static const TouchscreenHitbox sPokedexAreaHitboxes[] = {
    { .rect = { 40, 55, 82, 221 } },
    { .rect = { 64, 79, 82, 221 } },
    { .rect = { 136, 151, 82, 221 } },
    { .rect = { 112, 127, 82, 221 } },
    { .rect = { 37, 52, 231, 252 } },
    { .rect = { 139, 154, 231, 252 } },
    { .rect = { 53, 138, 231, 252 } },
    { .rect = { 6, 25, 70, 129 } },
    { .rect = { 6, 25, 190, 247 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 33, 77, 13 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 33, 109, 13 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 33, 141, 13 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sSearchOrderDpadBoxes[] = {
    { 68,  56,  0, 0, 6, 2, 1, 1 },
    { 188, 56,  0, 0, 7, 3, 0, 0 },
    { 68,  88,  0, 0, 0, 4, 3, 3 },
    { 188, 88,  0, 0, 1, 5, 2, 2 },
    { 68,  120, 0, 0, 2, 6, 5, 5 },
    { 188, 120, 0, 0, 3, 7, 4, 4 },
    { 40,  176, 0, 0, 4, 0, 7, 7 },
    { 216, 176, 0, 0, 5, 1, 6, 6 },
};

static const TouchscreenHitbox sSearchFormHitboxes[] = {
    { .rect = { 40, 71, 16, 47 } },
    { .rect = { 40, 71, 64, 95 } },
    { .rect = { 40, 71, 112, 143 } },
    { .rect = { 40, 71, 160, 191 } },
    { .rect = { 40, 71, 208, 239 } },
    { .rect = { 80, 111, 16, 47 } },
    { .rect = { 80, 111, 64, 95 } },
    { .rect = { 80, 111, 112, 143 } },
    { .rect = { 80, 111, 160, 191 } },
    { .rect = { 80, 111, 208, 239 } },
    { .rect = { 120, 151, 16, 47 } },
    { .rect = { 120, 151, 64, 95 } },
    { .rect = { 120, 151, 112, 143 } },
    { .rect = { 120, 151, 160, 191 } },
    { .rect = { 120, 151, 208, 239 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sSearchMenuDpadBoxes[] = {
    { 172, 16,  0, 0, 0, 1,   0,   0 },
    { 92,  40,  0, 0, 0, 2,   1,   6 },
    { 116, 64,  0, 0, 1, 3,   2,   6 },
    { 112, 88,  0, 0, 2, 4,   3,   6 },
    { 136, 112, 0, 0, 3, 5,   4,   4 },
    { 84,  136, 0, 0, 4, 136, 5,   5 },
    { 224, 72,  0, 0, 0, 4,   129, 6 },
    { 40,  176, 0, 0, 5, 7,   7,   8 },
    { 128, 176, 0, 0, 5, 8,   7,   9 },
    { 216, 176, 0, 0, 5, 9,   8,   9 },
};

static const TouchscreenHitbox sSearchTypeHitboxes[] = {
    { .rect = { 40, 55, 3, 60 } },
    { .rect = { 40, 55, 67, 124 } },
    { .rect = { 40, 55, 131, 188 } },
    { .rect = { 40, 55, 195, 252 } },
    { .rect = { 64, 79, 3, 60 } },
    { .rect = { 64, 79, 67, 124 } },
    { .rect = { 64, 79, 131, 188 } },
    { .rect = { 64, 79, 195, 252 } },
    { .rect = { 88, 104, 3, 60 } },
    { .rect = { 88, 104, 67, 124 } },
    { .rect = { 88, 104, 131, 188 } },
    { .rect = { 88, 104, 195, 252 } },
    { .rect = { 112, 127, 3, 60 } },
    { .rect = { 112, 127, 67, 124 } },
    { .rect = { 112, 127, 131, 188 } },
    { .rect = { 112, 127, 195, 252 } },
    { .rect = { 136, 151, 3, 60 } },
    { .rect = { 136, 151, 67, 124 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sSearchNameHitboxes[] = {
    { .rect = { 40, 55, 24, 39 } },
    { .rect = { 40, 55, 56, 71 } },
    { .rect = { 40, 55, 88, 103 } },
    { .rect = { 40, 55, 120, 135 } },
    { .rect = { 40, 55, 152, 167 } },
    { .rect = { 40, 55, 184, 199 } },
    { .rect = { 40, 55, 216, 231 } },
    { .rect = { 72, 87, 24, 39 } },
    { .rect = { 72, 87, 56, 71 } },
    { .rect = { 72, 87, 88, 103 } },
    { .rect = { 72, 87, 120, 135 } },
    { .rect = { 72, 87, 152, 167 } },
    { .rect = { 72, 87, 184, 199 } },
    { .rect = { 72, 87, 216, 231 } },
    { .rect = { 104, 119, 24, 39 } },
    { .rect = { 104, 119, 56, 71 } },
    { .rect = { 104, 119, 88, 103 } },
    { .rect = { 104, 119, 120, 135 } },
    { .rect = { 104, 119, 152, 167 } },
    { .rect = { 104, 119, 184, 199 } },
    { .rect = { 104, 119, 216, 231 } },
    { .rect = { 136, 151, 24, 39 } },
    { .rect = { 136, 151, 56, 71 } },
    { .rect = { 136, 151, 88, 103 } },
    { .rect = { 136, 151, 120, 135 } },
    { .rect = { 136, 151, 152, 167 } },
    { .rect = { 136, 151, 216, 231 } },
    { .rect = { 164, 187, 4, 75 } },
    { .rect = { 164, 187, 180, 251 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sSearchFormDpadBoxes[] = {
    { 32,  56,  0, 0, 15, 5,  4,  1  },
    { 80,  56,  0, 0, 15, 6,  0,  2  },
    { 128, 56,  0, 0, 15, 7,  1,  3  },
    { 176, 56,  0, 0, 16, 8,  2,  4  },
    { 224, 56,  0, 0, 16, 9,  3,  0  },
    { 32,  96,  0, 0, 0,  10, 9,  6  },
    { 80,  96,  0, 0, 1,  11, 5,  7  },
    { 128, 96,  0, 0, 2,  12, 6,  8  },
    { 176, 96,  0, 0, 3,  13, 7,  9  },
    { 224, 96,  0, 0, 4,  14, 8,  5  },
    { 32,  136, 0, 0, 5,  15, 14, 11 },
    { 80,  136, 0, 0, 6,  15, 10, 12 },
    { 128, 136, 0, 0, 7,  15, 11, 13 },
    { 176, 136, 0, 0, 8,  16, 12, 14 },
    { 224, 136, 0, 0, 9,  16, 13, 10 },
    { 40,  176, 0, 0, 10, 0,  16, 16 },
    { 216, 176, 0, 0, 14, 4,  15, 15 },
};

static const DpadMenuBox sSearchTypeDpadBoxes[] = {
    { 32,  48,  0, 0, 18, 4,  3,  1  },
    { 96,  48,  0, 0, 18, 5,  0,  2  },
    { 160, 48,  0, 0, 19, 6,  1,  3  },
    { 224, 48,  0, 0, 19, 7,  2,  0  },
    { 32,  72,  0, 0, 0,  8,  7,  5  },
    { 96,  72,  0, 0, 1,  9,  4,  6  },
    { 160, 72,  0, 0, 2,  10, 5,  7  },
    { 224, 72,  0, 0, 3,  11, 6,  4  },
    { 32,  96,  0, 0, 4,  12, 11, 9  },
    { 96,  96,  0, 0, 5,  13, 8,  10 },
    { 160, 96,  0, 0, 6,  14, 9,  11 },
    { 224, 96,  0, 0, 7,  15, 10, 8  },
    { 32,  120, 0, 0, 8,  16, 15, 13 },
    { 96,  120, 0, 0, 9,  17, 12, 14 },
    { 160, 120, 0, 0, 10, 19, 13, 15 },
    { 224, 120, 0, 0, 11, 19, 14, 12 },
    { 32,  144, 0, 0, 12, 18, 17, 17 },
    { 96,  144, 0, 0, 13, 18, 16, 16 },
    { 40,  176, 0, 0, 16, 0,  19, 19 },
    { 216, 176, 0, 0, 15, 3,  18, 18 },
};

static const DpadMenuBox sSearchNameDpadBoxes[] = {
    { 32,  48,  0, 0, 27, 7,  6,  1  },
    { 64,  48,  0, 0, 27, 8,  0,  2  },
    { 96,  48,  0, 0, 27, 9,  1,  3  },
    { 128, 48,  0, 0, 27, 10, 2,  4  },
    { 160, 48,  0, 0, 28, 11, 3,  5  },
    { 192, 48,  0, 0, 28, 12, 4,  6  },
    { 224, 48,  0, 0, 28, 13, 5,  0  },
    { 32,  80,  0, 0, 0,  14, 13, 8  },
    { 64,  80,  0, 0, 1,  15, 7,  9  },
    { 96,  80,  0, 0, 2,  16, 8,  10 },
    { 128, 80,  0, 0, 3,  17, 9,  11 },
    { 160, 80,  0, 0, 4,  18, 10, 12 },
    { 192, 80,  0, 0, 5,  19, 11, 13 },
    { 224, 80,  0, 0, 6,  20, 12, 7  },
    { 32,  112, 0, 0, 7,  21, 20, 15 },
    { 64,  112, 0, 0, 8,  22, 14, 16 },
    { 96,  112, 0, 0, 9,  23, 15, 17 },
    { 128, 112, 0, 0, 10, 24, 16, 18 },
    { 160, 112, 0, 0, 11, 25, 17, 19 },
    { 192, 112, 0, 0, 12, 28, 18, 20 },
    { 224, 112, 0, 0, 13, 26, 19, 14 },
    { 32,  144, 0, 0, 14, 27, 26, 22 },
    { 64,  144, 0, 0, 15, 27, 21, 23 },
    { 96,  144, 0, 0, 16, 27, 22, 24 },
    { 128, 144, 0, 0, 17, 27, 23, 25 },
    { 160, 144, 0, 0, 18, 28, 24, 26 },
    { 224, 144, 0, 0, 20, 28, 25, 21 },
    { 40,  176, 0, 0, 21, 0,  28, 28 },
    { 216, 176, 0, 0, 26, 6,  27, 27 },
};

static const u8 sSearchMenuCursorAnims[12] = { 39, 35, 36, 59, 60, 38, 34, 35, 35, 35, 0, 0 };

static const u8 sSearchFormCursorAnims[20] = { 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 0, 0, 0 };

static const TouchscreenHitbox sPokedexEntryTabHitboxes[] = {
    { .rect = { 168, 183, 7, 57 } },
    { .rect = { 168, 183, 71, 121 } },
    { .rect = { 168, 183, 135, 185 } },
    { .rect = { 168, 183, 199, 249 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

// Entry screen tabs and list buttons (touch), A / B / Start (keys). In the national dex (nationalDex) left and
// right switch between the Johto and national lists.
int PokedexApp_HandleListMenuInput(PokedexAppData *pokedexApp) {
    pokedexApp->keyInput = 0;
    if (System_GetTouchNew() == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(sPokedexListMenuHitboxes);
        if (hit == -1) {
            return -1;
        }
        int input = sPokedexListMenuHitResults[hit];
        if (input == 6 && pokedexApp->nationalDex == 0) {
            return 5;
        }
        return input;
    } else {
        pokedexApp->keyInput = 1;
        if (pokedexApp->nationalDex == 1) {
            if (gSystem.newKeys & PAD_KEY_LEFT) {
                if (pokedexApp->dexMode == 0) {
                    return 3;
                }
                return -1;
            }
            if (gSystem.newKeys & PAD_KEY_RIGHT) {
                if (pokedexApp->dexMode == 1) {
                    return 4;
                }
                return -1;
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_A || gSystem.newKeys & PAD_BUTTON_START) {
            return 0;
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            return 2;
        }
        if (gSystem.newKeys & PAD_BUTTON_Y) {
            return 1;
        }
    }
    return -1;
}

// The list: a 5x3 icon grid per page (unk_1859 page, unk_185A cursor).
int PokedexApp_HandleListInput(PokedexAppData *pokedexApp, int mode) {
    u32 x;
    u32 y;
    u32 input;

    pokedexApp->keyInput = 0;
    if (System_GetTouchNewCoords(&x, &y) == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(sPokedexListHitboxes);
        if (hit == -1) {
            return -1;
        }
        input = sPokedexListHitResults[hit];
        if (input == 0) {
            u8 selection;
            x -= 27;
            y -= 4;
            selection = x / 40 + 5 * (y / 40);
            if (selection == pokedexApp->listCursor) {
                return 4;
            }
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 14) {
            PlaySE(SEQ_SE_GS_BUTTON01);
        } else if (input == 2) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 5) {
            PlaySE(SEQ_SE_GS_GEARCANCEL);
        }
        return input;
    }
    pokedexApp->keyInput = 1;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->listCursor < 5) {
            return 10;
        }
        pokedexApp->listCursor -= 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->listCursor >= 10 && pokedexApp->listCursor < 15) {
            return 12;
        }
        pokedexApp->listCursor += 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (pokedexApp->listCursor != 0) {
            pokedexApp->listCursor--;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage != 0) {
            pokedexApp->listCursor = 14;
            return 9;
        }
        return -1;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int selection = pokedexApp->listCursor + 1;
        if (selection != 15) {
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, mode)) {
            pokedexApp->listCursor = 0;
            return 11;
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 6;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 3;
    }
    if (gSystem.newKeys & PAD_BUTTON_Y) {
        PlaySE(SEQ_SE_GS_ZKN04);
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        return 9;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        return 11;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        return 8;
    }
    if (gSystem.newKeys & PAD_BUTTON_START) {
        PlaySE(SEQ_SE_GS_BUTTON01);
        return 1;
    }
    return -1;
}

// The search pages: each has a grid input handler (gridInput) whose callbacks move the cursor sprite 0.

void PokedexApp_CreateSearchMenuGrid(PokedexAppData *pokedexApp, int initialPos) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchMenuHitboxes, sSearchMenuDpadBoxes, &sSearchMenuGridCallbacks, pokedexApp, TRUE, initialPos, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, initialPos);
}

u32 PokedexApp_HandleSearchMenuInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return PAD_BUTTON_START;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

void PokedexApp_UpdateSearchMenuCursor(PokedexAppData *pokedexApp, int target) {
    u8 x;
    u8 y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->gridInput, target), &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchMenuCursorAnims[target]);
}

void PokedexApp_OnSearchMenuGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchMenuGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchMenuCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchOrderGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchOrderHitboxes, sSearchOrderDpadBoxes, &sSearchOrderGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchOrderInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 6;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

void PokedexApp_UpdateSearchOrderCursor(PokedexAppData *pokedexApp, int target) {
    struct {
        u8 y, x;
    } pos;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->gridInput, target), &pos.x, &pos.y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos.x, pos.y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchOrderCursorAnims[target]);
}

void PokedexApp_OnSearchOrderGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchOrderGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchOrderCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchNameGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchNameHitboxes, sSearchNameDpadBoxes, &sSearchNameGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchNameCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchNameInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 27;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

// 27 letter buttons, then OK / cancel
void PokedexApp_UpdateSearchNameCursor(PokedexAppData *pokedexApp, int target) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->gridInput, target);

    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos[1], pos[0]);
    if (target >= 27) {
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 35);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 40);
    }
}

typedef void (*GridSetInputsU8Func)(GridInputHandler *inputHandler, u8 nextInput, u8 lastInput, u8 unk0F);

// Moving up or down from OK / cancel lands in the letter column the cursor left from.
void PokedexApp_OnSearchNameGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    if ((u32)(input - 27) <= 1) {
        int previous = pokedexApp->prevGridInput;
        u8 left = sSearchNameDpadBoxes[previous].left;
        if (target == 0 || target == 6) {
            int i;
            for (i = 0; i <= 26; i++) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    ((GridSetInputsU8Func)GridInputHandler_SetNextLastReturnInputs)(pokedexApp->gridInput, target, input, input);
                    break;
                }
            }
        } else if (target == 21 || target == 26) {
            int i;
            for (i = 26; i >= 0; i--) {
                if (left == sSearchNameDpadBoxes[i].left) {
                    target = i;
                    ((GridSetInputsU8Func)GridInputHandler_SetNextLastReturnInputs)(pokedexApp->gridInput, target, input, input);
                    break;
                }
            }
        }
    }
    PokedexApp_UpdateSearchNameCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchNameGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchNameCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

void PokedexApp_CreateSearchTypeGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchTypeHitboxes, sSearchTypeDpadBoxes, &sSearchTypeGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchTypeInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 18;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

void PokedexApp_UpdateSearchTypeCursor(PokedexAppData *pokedexApp, int target) {
    u8 position[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->gridInput, target);

    DpadMenuBox_GetPosition(box, &position[1], &position[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], position[1], position[0]);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchTypeCursorAnims[target]);
}

// Moving into the OK (18) / cancel (19) buttons and back returns to the column the cursor came from.
void PokedexApp_OnSearchTypeGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    if (input == 18) {
        if (target == 0) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 16 && previous <= 17) {
                target = previous % 4;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 18);
            } else if (previous >= 0 && previous <= 1) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)previous, (u8)input, 18);
            }
        } else if (target == 16) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 1) {
                target = previous + 16;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 18);
            } else if (previous >= 16 && previous <= 17) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)previous, (u8)input, 18);
            }
        }
    } else if (input == 19) {
        if (target == 3) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 14 && previous <= 15) {
                target = previous % 4;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 19);
            } else if (previous >= 2 && previous <= 3) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)previous, (u8)input, 19);
            }
        } else if (target == 15) {
            s32 previous = pokedexApp->prevGridInput;
            if (previous >= 2 && previous <= 3) {
                target = previous + 12;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 19);
            } else if (previous >= 14 && previous <= 15) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)previous, (u8)input, 19);
            }
        }
    }
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchTypeGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

void PokedexApp_CreateSearchHeightGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchHeightHitboxes, sSearchHeightDpadBoxes, &sSearchHeightGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, 0);
}

#pragma opt_common_subs off

// Two slider handles (targets 0 and 1): left / right move the selected one; dragging a handle selects it.
u32 PokedexApp_HandleSearchHeightInput(PokedexAppData *pokedexApp) {
    int next = GridInputHandler_GetNextInput(pokedexApp->gridInput);
    unsigned hit, index;

    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 2;
    }
    if (next == 0) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 5;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 4;
        }
    }
    if (next == 1) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 7;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 6;
        }
    }
    hit = TouchscreenHitbox_FindRectAtTouchHeld(sPokedexSearchSliderHitboxes);
    if (hit != (unsigned)-1) {
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        index = hit >> 1;
        GridInputHandler_SetNextInput(pokedexApp->gridInput, (u8)index);
        PokedexApp_UpdateSearchHeightCursor(pokedexApp, index);
        return hit + 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

#pragma opt_common_subs reset

// The handles sit at their slider values.
void PokedexApp_UpdateSearchHeightCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->gridInput, target), &x, &y);
    if (target == 0) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]);
    } else if (target == 1) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]);
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchHeightCursorAnims[target]);
}

void PokedexApp_OnSearchHeightGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchHeightGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchHeightCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchWeightGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchWeightHitboxes, sSearchWeightDpadBoxes, &sSearchWeightGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, 0);
}

#pragma opt_common_subs off

// Two slider handles (targets 0 and 1): left / right move the selected one; dragging a handle selects it.
u32 PokedexApp_HandleSearchWeightInput(PokedexAppData *pokedexApp) {
    int next = GridInputHandler_GetNextInput(pokedexApp->gridInput);
    unsigned hit, index;

    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 2;
    }
    if (next == 0) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 5;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 4;
        }
    }
    if (next == 1) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            return 7;
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            return 6;
        }
    }
    hit = TouchscreenHitbox_FindRectAtTouchHeld(sPokedexSearchSliderHitboxes);
    if (hit != (unsigned)-1) {
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        index = hit >> 1;
        GridInputHandler_SetNextInput(pokedexApp->gridInput, (u8)index);
        PokedexApp_UpdateSearchWeightCursor(pokedexApp, index);
        return hit + 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

#pragma opt_common_subs reset

// The handles sit at their slider values.
void PokedexApp_UpdateSearchWeightCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(pokedexApp->gridInput, target), &x, &y);
    if (target == 0) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]);
    } else if (target == 1) {
        x = PokedexApp_SliderValueToX(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]);
    }
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchWeightCursorAnims[target]);
}

void PokedexApp_OnSearchWeightGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchWeightGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchWeightCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

u32 PokedexApp_GetTouchHeldX(u32 *x) {
    u32 y;
    return System_GetTouchHeldCoords(x, &y);
}

void PokedexApp_CreateSearchAreaGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchAreaHitboxes, sSearchAreaDpadBoxes, &sSearchAreaGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchAreaInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 4;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

void PokedexApp_UpdateSearchAreaCursor(PokedexAppData *pokedexApp, int target) {
    u8 x, y;
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->gridInput, target);

    DpadMenuBox_GetPosition(box, &x, &y);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], x, y);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchAreaCursorAnims[target]);
}

void PokedexApp_OnSearchAreaGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, target);
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchAreaGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchAreaCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
}

void PokedexApp_CreateSearchFormGrid(PokedexAppData *pokedexApp) {
    pokedexApp->gridInput = GridInputHandler_Create(sSearchFormHitboxes, sSearchFormDpadBoxes, &sSearchFormGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, TRUE);
    PokedexApp_UpdateSearchFormCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchFormInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    if (gSystem.newKeys & PAD_BUTTON_START) {
        return 15;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->gridInput);
}

void PokedexApp_UpdateSearchFormCursor(PokedexAppData *pokedexApp, int target) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->gridInput, target);

    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(pokedexApp->sprites[0], pos[1], pos[0]);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchFormCursorAnims[target]);
}

// Moving into the OK (15) / cancel (16) buttons and back returns to the column the cursor came from.
void PokedexApp_OnSearchFormGridMove(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    int previous;

    if (input == 15) {
        if (target == 0) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 2) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 15);
            } else if (previous >= 10 && previous <= 12) {
                target = previous % 5;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 15);
            }
        } else if (target == 10) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 0 && previous <= 2) {
                target = previous + 10;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 15);
            } else if (previous >= 10 && previous <= 12) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 15);
            }
        }
    } else if (input == 16) {
        if (target == 4) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 3 && previous <= 4) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 16);
            } else if (previous >= 13 && previous <= 14) {
                target = previous % 5;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 16);
            }
        } else if (target == 14) {
            previous = pokedexApp->prevGridInput;
            if (previous >= 3 && previous <= 4) {
                target = previous + 10;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 16);
            } else if (previous >= 13 && previous <= 14) {
                target = previous;
                GridInputHandler_SetNextLastReturnInputs(pokedexApp->gridInput, (u8)target, (u8)input, 16);
            }
        }
    }
    PokedexApp_UpdateSearchFormCursor(pokedexApp, target);
    pokedexApp->prevGridInput = input;
    PlaySE(SEQ_SE_GS_ZKN03);
}

void PokedexApp_OnSearchFormGridTouch(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
    PokedexApp_UpdateSearchFormCursor(pokedexApp, target);
    MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    pokedexApp->prevGridInput = input;
}

// The list in the search results (as PokedexApp_HandleListInput; the first entry of the first page
// leaves the results).
u32 PokedexApp_HandleSearchResultListInput(PokedexAppData *pokedexApp, int mode) {
    u32 x;
    u32 y;
    u32 input;

    if (System_GetTouchNewCoords(&x, &y) == TRUE) {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(sPokedexListHitboxes);
        if (hit == -1) {
            return -1;
        }
        input = sPokedexListHitResults[hit];
        if (input == 0) {
            u8 selection;
            x -= 27;
            y -= 4;
            selection = x / 40 + 5 * (y / 40);
            if (selection == pokedexApp->listCursor) {
                if (pokedexApp->listCursor == 0 && pokedexApp->listPage == 0) {
                    PlaySE(SEQ_SE_GS_ZKN04);
                    return 2;
                }
                return 4;
            }
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 2) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else if (input == 5) {
            PlaySE(SEQ_SE_GS_GEARCANCEL);
        }
        return input;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->listCursor < 5) {
            return 10;
        }
        pokedexApp->listCursor -= 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->listCursor >= 10 && pokedexApp->listCursor < 15) {
            return 12;
        }
        pokedexApp->listCursor += 5;
        PlaySE(SEQ_SE_GS_ZKN03);
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (pokedexApp->listCursor != 0) {
            pokedexApp->listCursor--;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage != 0) {
            pokedexApp->listCursor = 14;
            return 9;
        }
        return -1;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int selection = pokedexApp->listCursor + 1;
        if (selection != 15) {
            pokedexApp->listCursor = selection;
            PlaySE(SEQ_SE_GS_ZKN03);
            return 0;
        }
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, mode)) {
            pokedexApp->listCursor = 0;
            return 11;
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 4;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 6;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 3;
    }
    if (gSystem.newKeys & PAD_BUTTON_Y) {
        PlaySE(SEQ_SE_GS_ZKN04);
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        return 9;
    }
    if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        return 11;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        return 8;
    }
    return -1;
}

void PokedexApp_FreeGridInput(PokedexAppData *pokedexApp) {
    if (pokedexApp->gridInput != NULL) {
        GridInputHandler_Free(pokedexApp->gridInput);
        pokedexApp->gridInput = NULL;
    }
}

void PokedexApp_DummyGridCallback(void *data, int target, int input) {
    PokedexAppData *pokedexApp = data;
}

// The tabs of the entry screens (touch) and B. The size tab needs the species caught.
int PokedexApp_HandleEntryTabTouch(PokedexAppData *pokedexApp, u32 *touched) {
    int hit;
    u32 coords[2];

    if (System_GetTouchNew() == TRUE) {
        hit = TouchscreenHitbox_FindRectAtTouchNew(sPokedexEntryTabHitboxes);
        if (hit == -1) {
            return -1;
        }
        System_GetTouchNewCoords(&coords[1], &coords[0]);
        hit = sPokedexEntryTabHitResults[hit];
        if (hit == 1 && !Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return -1;
        }
        *touched = 1;
        return hit;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        *touched = 0;
        return 3;
    }
    return -1;
}

// Area screen: up / down scroll the area list, A cycles the time of day, Select / L / R switch Johto / Kanto.
u32 PokedexApp_HandleAreaInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindHitboxAtTouchNew(sPokedexAreaHitboxes);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return sPokedexAreaHitResults[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        return 5;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        return 7;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (pokedexApp->areaTimeOfDay == 0) {
            return 14;
        }
        if (pokedexApp->areaTimeOfDay == 1) {
            return 15;
        }
        return 13;
    }
    if (gSystem.newKeys & PAD_BUTTON_SELECT) {
        if (pokedexApp->areaRegion == 0) {
            return 12;
        }
        return 11;
    }
    if (pokedexApp->areaRegion == 0) {
        if (gSystem.newKeys & PAD_BUTTON_R) {
            return 12;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_L) {
        return 11;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        return 16;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return 1;
        }
        return 2;
    }
    return -1;
}

u32 PokedexApp_HandleSizeCheckInput(PokedexAppData *pokedexApp, u32 *touched) {
    int value = PokedexApp_HandleEntryTabTouch(pokedexApp, touched), fallback = -1;

    if (value == fallback) {
        value = 0;
        *touched = 0;
        if (!(gSystem.newAndRepeatedKeys & PAD_KEY_LEFT)) {
            if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
                fallback = 2;
            }
            value = fallback;
        }
    }
    return value;
}

u32 PokedexApp_HandleFormsInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindRectAtTouchNew(sPokedexFormsHitboxes);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return sPokedexFormsHitResults[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        return 5;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        return 7;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies)) {
            return 1;
        }
        return 0;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        return 12;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 11;
    }
    return -1;
}

u32 PokedexApp_HandleFormCompareInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret;

    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindRectAtTouchNew(sPokedexFormCompareHitboxes);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return sPokedexFormCompareHitResults[ret];
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (pokedexApp->compareSide == 0) {
            return 0;
        }
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (pokedexApp->compareSide == 0) {
            return 1;
        }
        return 3;
    }
    if (gSystem.newKeys & PAD_KEY_LEFT) {
        return 6;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        return 7;
    }
    if (gSystem.newKeys & PAD_BUTTON_X) {
        return 4;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return 5;
    }
    return -1;
}

u32 PokedexApp_HandleEntryMenuInput(PokedexAppData *pokedexApp, u32 *touched) {
    int ret = PokedexApp_HandleEntryTabTouch(pokedexApp, touched);
    if (ret != -1) {
        return ret;
    }
    if (System_GetTouchNew() == TRUE) {
        ret = TouchscreenHitbox_FindHitboxAtTouchNew(sPokedexEntryMenuHitboxes);
        if (ret == -1) {
            return -1;
        }
        *touched = 1;
        return 4;
    }
    *touched = 0;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        return 2;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        return 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return 3;
    }
    return -1;
}
