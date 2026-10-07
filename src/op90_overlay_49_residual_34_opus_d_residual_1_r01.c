#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

typedef struct Ov49Sprite {
    u8 unk0[0x54];
    Sprite *sprite;
    SpriteResource *resources[4];
} Ov49Sprite;

typedef struct Ov49ResMan {
    u8 unk0[0x130];
    GF_2DGfxResMan *resMan[4];
} Ov49ResMan;

void ov49_0225C148(Ov49Sprite *data, Ov49ResMan *resMan);

// @fn ov49_0225C148
void ov49_0225C148(Ov49Sprite *data, Ov49ResMan *resMan) {
    int i;

    Sprite_Delete(data->sprite);
    data->sprite = NULL;
    SpriteTransfer_DeleteCharTransferTask(data->resources[0]);
    SpriteTransfer_DeletePlttTransferTask(data->resources[1]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(resMan->resMan[i], data->resources[i]);
    }
}
