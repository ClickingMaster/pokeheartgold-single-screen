#include "global.h"

#include "constants/sndseq.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "assert.h"
#include "font.h"
#include "font_sprite.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"

// Button panel of the Dress-Up editor: next / previous page, the accessory and backdrop tabs (a radio pair) and
// DONE (a text button), driven by touch or by the keys.

extern const u8 ov41_0224C094[2];

void DressUpButton_Delete(DressUpButton *button) {
    Sprite_Delete(button->sprite);
    memset(button, 0, sizeof(DressUpButton));
}

void DressUpButton_DeleteWithText(DressUpTextButton *button) {
    DressUpButton_Delete(&button->button);
    FontOAM_Delete(button->textObj);
    memset(button, 0, sizeof(DressUpTextButton));
}

void DressUpButton_TryCallback(DressUpButton *button, u32 state) {
    if (state == button->trigger) {
        button->callback(button, button->arg);
    }
}

// the two tab buttons (2, 3): the selected one shows pressed (or selected), the other normal
void DressUp_UpdateCategoryTabs(DressUpButtonPanel *panel, int selection, int state) {
    int i = 2;
    DressUpButton *button = &panel->buttons[2];
    for (; i <= 3; i++, button++) {
        if (i != selection) {
            DressUpButton_SetAnimNormal(button);
        } else if (state == 0) {
            DressUpButton_SetAnimPressed(button);
            PlaySE(SEQ_SE_DP_PASO);
        } else if (state == 2) {
            DressUpButton_SetAnimSelected(button);
        } else {
            DressUpButton_SetAnimPressed(button);
        }
    }
}

void DressUpButton_SetCallback(DressUpButton *button, DressUpButtonCallback callback, void *arg, u32 trigger) {
    if (button == NULL) {
        GF_AssertFail();
    }
    button->trigger = trigger;
    button->arg = arg;
    button->callback = callback;
}

void ov41_0224A254(void *button, void *arg) {
}

void DressUpButton_SetAnimSelected(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 2);
}

void DressUpButton_SetAnimNormal(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 0);
}

void DressUpButton_SetAnimPressed(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 1);
}

#pragma force_active on

void DressUpButtonPanel_Create(DressUpButtonPanel *panel, DressUpGfx *gfx, MenuInputState *inputState) {
    panel->inputState = inputState;
    panel->fontSystem = FontSystem_NewInit(1, HEAP_ID_13);
    DressUpButtonPanel_LoadResources(gfx);
    DressUpButton_CreateSprite(&panel->buttons[0], 0, gfx, 48, 144, 40, 32);
    DressUp_SetHitboxRect(panel->hitboxes, 0, 48, 152, 40, 24);
    DressUpButton_CreateSprite(&panel->buttons[1], 1, gfx, 8, 144, 40, 32);
    DressUp_SetHitboxRect(panel->hitboxes, 1, 8, 152, 40, 24);
    DressUpButton_CreateSprite(&panel->buttons[2], 2, gfx, 96, 144, 40, 42);
    DressUp_SetHitboxRect(panel->hitboxes, 2, 96, 156, 40, 34);
    DressUpButton_CreateSprite(&panel->buttons[3], 3, gfx, 136, 144, 40, 42);
    DressUp_SetHitboxRect(panel->hitboxes, 3, 136, 156, 40, 34);
    FontID_Alloc(2, HEAP_ID_14);
    {
        Window *label = DressUpButton_CreateTextBitmap(gfx, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00000, 9, 5);
        DressUpButton_CreateTextSprite(&panel->done, 4, gfx, panel->fontSystem, label, 184, 144, 72, 42);
        DressUp_SetHitboxRect(panel->hitboxes, 4, 184, 156, 72, 34);
        DressUpButton_DeleteTextBitmap(label);
    }
    FontID_Release(2);
    panel->visible = TRUE;
    DressUpButtonPanel_FreeResources(gfx);
    DressUpButton_SetAnimSelected(&panel->buttons[2]);
    panel->controller = TouchHitboxController_Create(panel->hitboxes, 5, DressUpButtonPanel_OnTouch, panel, HEAP_ID_13);
}

#pragma force_active reset

void DressUpButtonPanel_Destroy(DressUpButtonPanel *panel, DressUpGfx *gfx) {
    int i;
    for (i = 0; i < 5; i++) {
        DressUpGfx_FreeCellRes(gfx, i);
        DressUpGfx_FreeAnimRes(gfx, i);
        ObjCharTransfer_ResetTransferTasksByResID(i);
    }
    ObjPlttTransfer_FreeTaskByID(0);
    ObjPlttTransfer_FreeTaskByID(1);
    i = 0;
    {
        DressUpButton *button = panel->buttons;
        do {
            DressUpButton_Delete(button);
            i++;
            button++;
        } while (i < 4);
    }
    ObjCharTransfer_FreeCharSpace(&panel->done.charAlloc);
    DressUpButton_DeleteWithText(&panel->done);
    FontSystem_Delete(panel->fontSystem);
    TouchHitboxController_Destroy(panel->controller);
    panel->controller = NULL;
}

// A / B press DONE, up / down the page buttons, left / right the tabs; a pressed key shows for 2 frames
void DressUpButtonPanel_HandleKeys(DressUpButtonPanel *panel) {
    int key = 0xFF;
    u32 keys;
    if (!panel->controller) {
        GF_AssertFail();
    }
    if (panel->keyRepeat) {
        panel->keyRepeat--;
        DressUpButtonPanel_OnTouch(panel->key, ov41_0224C094[panel->keyRepeat ^ 1], panel);
        return;
    }
    keys = gSystem.newKeys;
    if (keys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        key = 4;
    } else if (keys & PAD_KEY_UP) {
        key = 1;
    } else if (keys & PAD_KEY_DOWN) {
        key = 0;
    } else if (keys & PAD_KEY_LEFT) {
        key = 2;
    } else if (keys & PAD_KEY_RIGHT) {
        key = 3;
    }
    if (key != 0xFF) {
        panel->key = key;
        panel->keyRepeat = 2;
        DressUpButtonPanel_OnTouch(panel->key, 0, panel);
    }
}

#pragma force_active on

// switches the shared input mode between touch and keys
int DressUpButtonPanel_UpdateInputMode(DressUpButtonPanel *panel) {
    if (panel->keyRepeat) {
        return 0;
    }
    if (*panel->inputState == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return 0;
        }
        if (gSystem.heldKeys) {
            *panel->inputState = MENU_INPUT_STATE_BUTTONS;
        }
    } else {
        if (gSystem.heldKeys) {
            return 0;
        }
        if (System_GetTouchHeld()) {
            *panel->inputState = MENU_INPUT_STATE_TOUCH;
        }
    }
    return 0;
}

void DressUpButtonPanel_Update(DressUpButtonPanel *panel) {
    if (!panel->controller) {
        GF_AssertFail();
    }
    if (DressUpButtonPanel_UpdateInputMode(panel) == 0) {
        if (*panel->inputState == MENU_INPUT_STATE_BUTTONS) {
            DressUpButtonPanel_HandleKeys(panel);
            return;
        }
        TouchHitboxController_IsTriggered(panel->controller);
    }
}

#pragma force_active reset

void ov41_0224A580(DressUpButtonPanel *panel) {
    DressUpButton_SetAnimByTouchState(&panel->done.button, 3);
    DressUpButton_SetTextByState(&panel->done, 3);
    DressUp_UpdateCategoryTabs(panel, 2, 3);
}

void DressUpButtonPanel_MoveBy(DressUpButtonPanel *panel, int dx, int dy) {
    DressUpButtonPanel *base[1] = { panel };
    int i = 0;
    DressUpButton *button = panel->buttons;
    do {
        DressUpButton_MoveBy(button, dx, dy);
        i++;
        button++;
    } while (i < 4);
    DressUpButton_MoveTextBy(&base[0]->done, dx, dy);
}

void DressUpButtonPanel_SetCallback(DressUpButtonPanel *panel, int button, DressUpButtonCallback callback, void *arg, u32 trigger) {
    if (!panel) {
        GF_AssertFail();
    }
    if (button < 4) {
        DressUpButton_SetCallback(&panel->buttons[button], callback, arg, trigger);
    } else {
        DressUpButton_SetCallback(&panel->done.button, callback, arg, trigger);
    }
}

// touch controller callback (also used for keys): button `which` in touch state `state`
void DressUpButtonPanel_OnTouch(u32 which, u32 state, void *arg) {
    DressUpButtonPanel *panel = arg;
    DressUpButton *button = (DressUpButton *)panel;
    switch (which) {
    case 0:
        DressUpButton_SetAnimByTouchState(button, state);
        DressUpButton_PlaySE(button, SEQ_SE_DP_MAZYO3, 0, state);
        DressUpButton_TryCallback(button, state);
        break;
    case 1:
        button++;
        DressUpButton_SetAnimByTouchState(button, state);
        DressUpButton_PlaySE(button, SEQ_SE_DP_MAZYO3, 0, state);
        DressUpButton_TryCallback(button, state);
        break;
    case 2:
    case 3:
        DressUp_UpdateCategoryTabs(panel, which, state);
        DressUpButton_TryCallback(button + which, state);
        break;
    case 4:
        if (panel->visible == TRUE) {
            DressUpButton_SetAnimByTouchState(&panel->done.button, state);
            DressUpButton_SetTextByState(&panel->done, state);
            DressUpButton_PlaySE(&panel->done.button, SEQ_SE_DP_PIRORIRO, 0, state);
            DressUpButton_TryCallback(&panel->done.button, state);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

#pragma force_active on

void DressUpButton_CreateSprite(DressUpButton *button, int resId, DressUpGfx *gfx, int x, int y, int width, int height) {
    DressUpButtonTemplate template;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;
    CreateSpriteResourcesHeader(&header, resId, 0, resId, resId, -1, -1, 0, 0, gfx->charResMan, gfx->plttResMan, gfx->cellResMan, gfx->animResMan, NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = (u32)x << FX32_SHIFT;
    spriteTemplate.position.y = (u32)y << FX32_SHIFT;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    template.spriteTemplate = &spriteTemplate;
    template.trigger = 1;
    template.callback = NULL;
    template.arg = NULL;
    DressUpButton_Init(button, &template);
}

void DressUpButton_CreateTextSprite(DressUpTextButton *button, int resId, DressUpGfx *gfx, FontSystem *fontSystem, Window *label, int x, int y, int width, int height) {
    DressUpTextButtonTemplate template;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;
    CreateSpriteResourcesHeader(&header, resId, 0, resId, resId, -1, -1, 0, 0, gfx->charResMan, gfx->plttResMan, gfx->cellResMan, gfx->animResMan, NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = x * FX32_ONE;
    spriteTemplate.position.y = y * FX32_ONE;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    template.button.spriteTemplate = &spriteTemplate;
    template.button.callback = NULL;
    template.button.arg = NULL;
    template.button.trigger = 1;
    template.fontSystem = fontSystem;
    template.window = label;
    template.x = 0;
    template.y = 19;
    template.plttProxy = SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(gfx->plttResMan, 1), NULL);
    GF_ASSERT(ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(label, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_13), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &button->charAlloc));
    template.charOffset = button->charAlloc.offset;
    DressUpButton_InitWithText(button, &template);
}

#pragma force_active reset

// hit box rect i = (x, y, width, height)
void DressUp_SetHitboxRect(TouchscreenHitbox *hitboxes, int i, int x, int y, int width, int height) {
    u8 *rect = (u8 *)hitboxes;
    i *= 4;
    rect[i] = y;
    rect += i;
    rect[2] = x;
    rect[1] = y + height;
    rect[3] = x + width;
}

void DressUpButtonPanel_LoadResources(DressUpGfx *gfx) {
    int i = 0;
    int charFileId = 0x6B, cellFileId = 0x6A, animFileId = 0x69;
    do {
        DressUpGfx_LoadCharRes(gfx, gfx->narc, charFileId, FALSE, 1, i);
        DressUpGfx_LoadCellRes(gfx, gfx->narc, cellFileId, FALSE, i);
        DressUpGfx_LoadAnimRes(gfx, gfx->narc, animFileId, FALSE, i);
        charFileId += 3;
        cellFileId += 3;
        animFileId += 3;
        i++;
    } while (i < 5);
    DressUpGfx_LoadPlttRes(gfx, gfx->narc, 0x68, FALSE, 1, 3, 0);
    DressUpGfx_LoadPlttRes(gfx, gfx->narc, 0x78, FALSE, 1, 2, 1);
}

void DressUpButtonPanel_FreeResources(DressUpGfx *gfx) {
    int i;
    for (i = 0; i < 5; i++) {
        DressUpGfx_FreeCharRes(gfx, i);
    }
    DressUpGfx_FreePlttRes(gfx, 0);
    DressUpGfx_FreePlttRes(gfx, 1);
}

void DressUpButton_SetAnimByTouchState(DressUpButton *button, u32 state) {
    if (state == 0) {
        DressUpButton_SetAnimPressed(button);
    } else if (state == 2) {
        DressUpButton_SetAnimSelected(button);
    } else if (state == 1 || state == 3) {
        DressUpButton_SetAnimNormal(button);
    }
}

void DressUpButton_SetTextByState(DressUpTextButton *button, u32 state) {
    if (state == 0) {
        TextOBJ_SetPosition(button->textObj, 0, 15);
        TextOBJ_SetPaletteNum(button->textObj, 4);
    } else {
        if (state == 1) {
            TextOBJ_SetPosition(button->textObj, 0, 19);
        }
        if (state == 3) {
            TextOBJ_SetPosition(button->textObj, 0, 19);
            TextOBJ_SetPaletteNum(button->textObj, 3);
        }
    }
}
