#ifndef TO52_OV96_R59_R11_H
#define TO52_OV96_R59_R11_H
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "string_util.h"
#include "text.h"

typedef struct Ov96R59UiWork {
    BgConfig *bgConfig;
    Window windows[5];
    u32 heapId;
} Ov96R59UiWork;
extern const WindowTemplate ov96_0221C1CC[];
extern u32 ov96_021E5F24(void *);
extern PlayerProfile *PokeathlonCourse_GetPlayerProfileFromData(void *, u32);
void ov96_021F6F80(void *, Ov96R59UiWork *);
void ov96_021F7050(Ov96R59UiWork *);
void ov96_021F70AC(Ov96R59UiWork *, int);
#endif
