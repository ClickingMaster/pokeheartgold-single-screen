#ifndef POKEHEARTGOLD_TO51_OVERLAY57_A8FC_PRIVATE_H
#define POKEHEARTGOLD_TO51_OVERLAY57_A8FC_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "render_window.h"
#include "yes_no_prompt.h"
typedef struct Ov57A8FCWork {
    u8 pad000[0xe4];
    BgConfig *bgConfig;
    PaletteData *paletteData;
    u8 pad0ec[0x40];
    Window window;
    u8 pad13c[0x108];
    YesNoPrompt *prompt;
    u8 pad248[0x1bc];
    int state;
    int result;
    int touchMode;
    int delay;
} Ov57A8FCWork;
int ov57_0223A8FC(void *);
BOOL ov57_022384C0(void *);
void ov57_022397B0(BgConfig *, Window *, int, int, int, int, int, int);
void ov57_02239728(void *, u32, u32, u32);
void ov57_0223B948(void *, BOOL);

void ov57_0223B774(void *);
void ov57_0223B7A8(void *);
void ov57_022399F8(void *);
void ov57_022383AC(void *);
void ov57_02237F3C(void *);
void ov57_022383D0(void *, BOOL);
#endif
