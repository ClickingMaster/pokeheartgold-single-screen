#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct Ov40Pos {
    s16 x;
    s16 y;
} Ov40Pos;

typedef struct Ov40PosTable {
    Ov40Pos pos[30];
} Ov40PosTable;

typedef struct Ov40Icon {
    ManagedSprite *sprite;
    u8 unk4[4];
} Ov40Icon;

typedef struct Ov40Sub {
    u8 unk0[0x2090];
    Ov40Icon icons[5][6];
} Ov40Sub;

typedef struct Ov40Unk {
    u8 unk0[0x860];
    Ov40Sub *sub;
} Ov40Unk;

extern const Ov40PosTable ov40_0224557C;

void ov40_0223A3BC(Ov40Unk *data);

// @fn ov40_0223A3BC
void ov40_0223A3BC(Ov40Unk *data) {
    Ov40PosTable table = ov40_0224557C;
    int i, j;
    Ov40Sub *sub = data->sub;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            if (sub->icons[i][j].sprite != NULL) {
                ManagedSprite_SetPositionXY(sub->icons[i][j].sprite, table.pos[i * 6 + j].x, table.pos[i * 6 + j].y);
            }
        }
    }
}
