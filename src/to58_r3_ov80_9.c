#include "overlay_80_command_helpers_internal.h"

extern void FrontierMap_GetObjectById(void *, u16, void **, void **);
extern void ov42_02229200(void *, u16);
int FrtCmd_038(void *);

int FrtCmd_038(void *c) {
    void *map = sub_0209680C(APP(c));
    u16 id = FrontierScript_ReadVar(c);
    u8 *script = PTR(c, 0x1c);
    PTR(c, 0x1c) = script + 1;
    u8 state = *script;
    void *out = NULL;
    FrontierMap_GetObjectById(map, id, NULL, &out);
    if (out == NULL) {
        GF_AssertFail();
    }
    ov42_02229200(out, state);
    return 0;
}
