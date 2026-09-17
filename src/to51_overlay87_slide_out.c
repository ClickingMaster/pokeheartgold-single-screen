#include "global.h"

typedef struct {
    u8 raw[12];
    void *sprite;
} SpriteWrap;

typedef struct {
    u8 pad[0x2F8];
    SpriteWrap *movingSprites[4];
    SpriteWrap *gridSprites[9];
    SpriteWrap *unused32C[4];
    SpriteWrap *singleSprite;
    SpriteWrap *unused340;
    SpriteWrap *hiddenSprites[3];
} Ov87Work;

typedef struct {
    s16 x;
    s16 y;
} Ov87Position;

#define OV87_U8(p, offset)  (*(u8 *)((u8 *)(p) + (offset)))
#define OV87_S16(p, offset) (*(s16 *)((u8 *)(p) + (offset)))
#define OV87_PTR(p, offset) (*(void **)((u8 *)(p) + (offset)))

extern const Ov87Position ov87_021E82E4[];

void GfGfx_EngineATogglePlanes(int plane, int enabled);
void GfGfx_EngineBTogglePlanes(int plane, int enabled);
int PlaySE(int sequence);
void BgSetPosTextAndCommit(void *config, int bgId, int op, int value);
void ov87_021E7FD4(SpriteWrap *sprite, int visible);
void ov87_021E7FEC(SpriteWrap *sprite, int x, int y);

int ov87_021E64F8(Ov87Work *p);

int ov87_021E64F8(Ov87Work *p) {
    switch (OV87_U8(p, 8)) {
    case 0:
        GfGfx_EngineATogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(1, 0);
        ov87_021E7FD4(p->singleSprite, 0);
        OV87_S16(p, 0x10) = 0;
        OV87_S16(p, 0x12) = 0;
        PlaySE(0x560);
        OV87_U8(p, 8) = 1;
        break;
    case 1:
        BgSetPosTextAndCommit(OV87_PTR(p, 0x58), 6, 0, OV87_S16(p, 0x12));
        BgSetPosTextAndCommit(OV87_PTR(p, 0x58), 0, 0, OV87_S16(p, 0x12));
        BgSetPosTextAndCommit(OV87_PTR(p, 0x58), 1, 0, OV87_S16(p, 0x12));
        OV87_S16(p, 0x12) -= 16;
        if (OV87_S16(p, 0x10) >= 256) {
            OV87_U8(p, 8) = 2;
        } else {
            int i;
            int y;
            i = 0;
            y = 26;
            for (; i < 4; y += 42, i++) {
                ov87_021E7FEC(p->movingSprites[i], OV87_S16(p, 0x10) + 60, y);
            }
            for (i = 0; i < 3; i++) {
                ov87_021E7FD4(p->hiddenSprites[i], 0);
            }
            for (i = 0; i < 9; i++) {
                ov87_021E7FEC(p->gridSprites[i], OV87_S16(p, 0x10) + ov87_021E82E4[i].x, ov87_021E82E4[i].y);
            }
        }
        OV87_S16(p, 0x10) += 16;
        break;
    case 2:
        OV87_S16(p, 0x10) = 0;
        OV87_S16(p, 0x12) = 0;
        return TRUE;
    }
    return FALSE;
}
