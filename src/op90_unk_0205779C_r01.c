#pragma force_active on
#include "global.h"
typedef struct { u16 x, z; u8 facing; u8 unk5; u8 unk6; u8 unk7; } Pos;
typedef struct { u8 pad[0x74]; Pos remote[8]; } Mgr;
extern Mgr *_021D41C4;
void sub_02057184(u8 id);
void sub_0205779C(int a, int b, const u8 *data);
// @fn sub_0205779C
void sub_0205779C(int a, int b, const u8 *data) {
Pos *pos; int id = data[0] & 0xF; if (_021D41C4 != NULL) { pos = _021D41C4->remote; pos += id; if (data[2] & 0x80) { pos->unk7 = 1; } else { pos->unk7 = 0; } pos->x = 0; pos->z = 0; pos->x += data[1]; pos->x += (data[2] << 8) & 0x100; pos->z += data[3]; pos->z += (data[2] << 7) & 0x100; pos->facing = (data[0] >> 4) & 3; pos->unk5 = (data[0] >> 6) & 3; sub_02057184(id); }
}
