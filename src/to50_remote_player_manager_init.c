#include "global.h"

#include "to50_remote_player_manager_init_private.h"

const u16 __attribute__((aligned(4))) _020FC7A0[4] = { 4, 7, 11, 7 };
const u16 _020FC7A8[8] = { 4, 6, 11, 6, 4, 8, 11, 8 };
RemotePlayerManager *_021D41C4;

BOOL sub_02056D7C(RemotePlayerManager *data, void *fieldSystem, int unused) {
    int i;

    if (_021D41C4 != NULL) {
        return FALSE;
    }
    if (_021D41C4 == NULL) {
        _021D41C4 = data;
        MI_CpuFill8(data, 0, sizeof(RemotePlayerManager));
    }
    _021D41C4->fieldSystem = fieldSystem;
    for (i = 0; i < 8; i++) {
        _021D41C4->slots2[i].value4 = -1;
        _021D41C4->slots2[i].value0 = 0xFFFF;
        _021D41C4->slots2[i].value2 = 0xFFFF;
        _021D41C4->slots2[i].value5 = 2;
        _021D41C4->slots1[i].value4 = -1;
        _021D41C4->slots1[i].value0 = 0xFFFF;
        _021D41C4->slots1[i].value2 = 0xFFFF;
        _021D41C4->slots1[i].value5 = 2;
        _021D41C4->states1[i] = 0;
        _021D41C4->states2[i] = 0;
    }
    _021D41C4->valueED = 0;
    _021D41C4->valueEF = 0;
    _021D41C4->task = SysTask_CreateOnMainQueue(sub_020572DC, _021D41C4->fieldSystem, 0xC8);
    sub_020374E4();
    sub_02056EF4();
    return TRUE;
}
