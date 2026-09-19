#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"

#define PTR(p, o, type) (*(type **)((u8 *)(p) + (o)))

BOOL FieldSystem_ShouldDrawStartMenuIcon(void *, int);
extern const u32 ov27_0225CED8[];
typedef struct Ov27MenuMessage {
    u16 resource;
    u16 message : 15;
    u16 flag : 1;
} Ov27MenuMessage;
extern const Ov27MenuMessage ov27_0225CF94[];
extern const u8 ov27_0225CFC8[][8];

typedef struct Ov27MenuBits {
    u32 bit0 : 1;
    u32 mode : 4;
    u32 rest : 27;
} Ov27MenuBits;

void ov27_0225BC34(void *arg);

typedef struct Ov27MenuTextSlot {
    String *string;
    u32 unused;
} Ov27MenuTextSlot;
typedef struct Ov27MenuTextWork {
    u8 pad000[0x474];
    Ov27MenuTextSlot menu[7];
    u8 pad4AC[4];
    String *messages[5];
    String *expanded12;
    String *expanded13;
} Ov27MenuTextWork;

void ov27_0225BC34(void *arg) {
    Ov27MenuTextWork *work = arg;
    int i;

    String_Delete(work->expanded13);
    String_Delete(work->expanded12);
    for (i = 0; i < 5; i++) {
        String_Delete(work->messages[i]);
    }
    for (i = 0; i < 7; i++) {
        if (work->menu[i].string != NULL) {
            String_Delete(work->menu[i].string);
        }
    }
}

typedef struct Ov27WindowWork {
    u8 pad000[0x10];
    void *fieldSystem;
    u8 pad014[0x3C8 - 0x14];
    Sprite *menuSprite;
    u8 pad3CC[0x3E0 - 0x3CC];
    Window mainWindow;
    Window windows[8];
} Ov27WindowWork;
