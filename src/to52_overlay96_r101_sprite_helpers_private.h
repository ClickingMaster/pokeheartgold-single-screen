#ifndef TO52_OV96_R101_REMAIN_PRIVATE_H
#define TO52_OV96_R101_REMAIN_PRIVATE_H
#include "global.h"
#include "bg_window.h"
#include "sprite_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "font.h"

typedef struct Ov96R101SetupWork {
    u32 heapId;
    u8 pad04[4];
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    MessageFormat *messageFormat;
    MsgData *msgData;
} Ov96R101SetupWork;

extern const GraphicsModes ov96_0221CF0C;
extern const u32 ov96_0221CF58[7];
extern const ManagedSpriteTemplate ov96_0221CFEC;
extern const ManagedSpriteTemplate ov96_0221D020;
extern const ManagedSpriteTemplate ov96_0221D054;
typedef struct Ov96BgTemplates7 { BgTemplate items[7]; } Ov96BgTemplates7;
extern const Ov96BgTemplates7 ov96_0221D088;

void GF_AssertFail(void);
ManagedSprite *ov96_0220ECA4(SpriteSystem *, SpriteManager *, u16, u16, u16, u16, BOOL);
ManagedSprite *ov96_0220ED34(SpriteSystem *, SpriteManager *, u16, u16, u16, u16);
void ov96_0220EB3C(Ov96R101SetupWork *);
void ov96_0220ED9C(Ov96R101SetupWork *);
#endif
