#ifndef TO52_R6_W19_PRIVATE_H
#define TO52_R6_W19_PRIVATE_H
#include "global.h"

#include "frontier/frontier_script_context.h"

typedef struct FrontierSystemR6W19 {
    void *unk0;
    u8 pad04[0x40];
    void *unk44;
    u8 pad48[0x18];
    void *unk60;
} FrontierSystemR6W19;

u16 *ov80_0222BE24(FrontierScriptContext *, u16);
void *ov80_0222E920(FrontierSystemR6W19 *, u8, u8, u8, u8, u16 *, void *, MsgData *);
BOOL ov80_0222C33C(FrontierScriptContext *);
BOOL FrtCmd_026(FrontierScriptContext *);
BOOL FrtCmd_027(FrontierScriptContext *);

#endif
