#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Elem {
    u32 a, b;
};

struct Tag {};

struct Vec8 {
    Elem *ptr;
    u32 size, cap;
};

struct Obj {
    u8 pad[0x50];
    Vec8 vec;
};

extern "C" int PPWLobbySchedule_ValidateVipRecords(Obj *o, const void *data, u32 len);
extern "C" void LobbyVipVector_AssignRange(Vec8 *v, const Elem *first, const Elem *last, Tag);
extern "C" int PPWLobbySchedule_SetVipRecords(Obj *o, const void *data, u32 len);

extern "C" int PPWLobbySchedule_SetVipRecords(Obj *o, const void *data, u32 len) {
    PPWLobbySchedule_ValidateVipRecords(o, data, len);
    const Elem *first = (const Elem *)data;
    LobbyVipVector_AssignRange(&o->vec, first, first + len / sizeof(Elem), Tag());
    return 1;
}
