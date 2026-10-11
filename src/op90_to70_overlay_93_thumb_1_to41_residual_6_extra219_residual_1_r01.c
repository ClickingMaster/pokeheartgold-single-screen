#pragma force_active on
#include "global.h"
typedef struct Data{u8 pad[0x2c];u8 ids[4];u8 count;}Data;typedef struct W{Data*data;u8 pad[0x28];void*bg;u8 windows[4][16];}W;
extern void FillBgTilemapRect(void*,int,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void*,int);void ov93_0225D5AC(W*,int);
extern int Comm_GetLocalNetId(void),ov93_0225E3C4(W*,int),ov90_022588A4(Data*,int),FontID_String_GetWidth(int,void*,int);extern void*ov90_022588CC(Data*,int),*PlayerProfile_GetPlayerName_NewString(void*,int);extern const u8 ov93_02262A68[];extern void Heap_Free(void*),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*);
void ov93_0225D4EC(W*);
#pragma opt_common_subs off
#pragma opt_rotateloops off
// @fn ov93_0225D4EC
void ov93_0225D4EC(W *w) {
    int position;
    int width;
    void *name;
    int self;
    u8 *windows;
    int i;
    int color;
    int x;
    self = Comm_GetLocalNetId();
    windows = (u8 *)w;
    windows += 0x30;
    for (i = 0; i < w->data->count; i++) {
        if (self != w->data->ids[i]) {
            name = PlayerProfile_GetPlayerName_NewString(ov90_022588CC(w->data, w->data->ids[i]), 117);
            position = ov93_0225E3C4(w, w->data->ids[i]);
            if (ov90_022588A4(w->data, w->data->ids[i]) == 1) {
                color = 0x7080f;
            } else {
                color = 0x1020f;
            }
            width = FontID_String_GetWidth(0, name, 0);
            x = 40 - width / 2;
            if (width & 1) {
                x--;
            }
            AddTextPrinterParameterizedWithColor(windows + 16 * (ov93_02262A68 + 4 * w->data->count)[position], 0, name, x, 0, 0, color, 0);
            Heap_Free(name);
        }
    }
}
