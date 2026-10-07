#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x1044];
    u8 clients[0x2c];
    int state, status, error;
    u8 pad107c[8];
    void *handle;
    u8 pad1088[8];
    int other, client, selection;
    struct { void *ready; u8 pad[0x34]; u16 flag; } r;
} Work;
extern Work *_0221A680;
void ov00_021F1284(void), ov00_021EE278(void), ov00_021E5E34(void);
int sub_0203993C(void);
int sub_02034084(void);
void ov00_021E69A8(void *);
int ov00_021E6690(void), ov00_021E6850(void);
int ov00_021E5E54(int success);
// @fn ov00_021E5E54
int ov00_021E5E54(int success) {
    switch (_0221A680->state) {
    case 5:
        if (success) _0221A680->state = 6;
        if (_0221A680->status == 2 && _0221A680->client >= 0 && _0221A680->clients[_0221A680->client] != 6) _0221A680->state = 10;
        break;
    case 6:
    case 10:
        if (!_0221A680->status) ov00_021F1284();
        else { ov00_021EE278(); ov00_021E5E34(); }
        break;
    case 7:
        sub_0203993C();
        if (!sub_02034084()) ov00_021E69A8(_0221A680->handle);
        { Work *w = _0221A680; if (w->r.ready) w->r.flag = 1; else w->r.flag = 0; }
        _0221A680->state = 8;
        return 20;
    case 9: _0221A680->state = 4; _0221A680->error = 0; _0221A680->selection = -1; return 21;
    case 11: _0221A680->state = 4; _0221A680->error = 0; _0221A680->selection = -1; return 22;
    case 12: return ov00_021E6690();
    case 16:
        if (!_0221A680->other) { ov00_021EE278(); _0221A680->state = 17; }
        break;
    }
    return ov00_021E6850();
}
