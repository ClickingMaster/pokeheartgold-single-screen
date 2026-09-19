#ifndef TO50_OV59_R10_GAP5_PRIVATE_H
#define TO50_OV59_R10_GAP5_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
typedef struct Ov59R10Gap5Work {
    enum HeapID heap;
    u8 pad4[0x50];
    BgConfig *bgConfig;
    u8 pad58[0x15C];
    Window windows[10];
    YesNoPrompt *prompt;
} Ov59R10Gap5Work;
extern const WindowTemplate ov59_0223CA40[];
void ov59_0223B740(Ov59R10Gap5Work *);
#endif
