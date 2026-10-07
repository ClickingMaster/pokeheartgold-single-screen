#pragma force_active on
#include "global.h"
#pragma force_active on
#include "item.h"
typedef struct{u8 pad[0x11];u8 row;u8 pad12[0x12];u16 move;u8 pad26[14];u8 selected;} Selection;
typedef struct{u16 move;u8 pad[6];} Slot;
typedef struct{Selection *selection;u8 pad4[0x30];Slot moves[1];} Work;
BOOL ov08_0221DB54(Work*);
BOOL ov08_0221DB54(Work *w){u16 move;if(w->selection->selected==4)move=w->selection->move;else move=((Slot*)((u8*)w+0x50*w->selection->row+0x34))[w->selection->selected].move;return MoveIsHM(move);}
