#include "overlay108_text_object_r11_private.h"

void ov108_021E756C(Ov108WorkR11 *work) {
    TextOBJTemplate template;
    u32 size;
    u32 width;
    u32 x;

    work->fontLayout = sub_02013910(&work->window, work->heapID);
    size = sub_02013948(work->fontLayout, NNS_G2D_VRAM_TYPE_2DMAIN);
    width = GetWindowWidth(&work->window);
    x = (width * 8 - FontID_String_GetWidth(0, work->string, 0)) / 2;
    AddTextPrinterParameterizedWithColor(&work->window, 0, work->string, x, 0, TEXT_SPEED_NOTRANSFER, 0xB0600, NULL);

    template.fontSystem = work->fontSystem;
    template.window = &work->window;
    template.spriteList = SpriteManager_GetSpriteList(work->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(work->spriteManager, 0);
    template.unk_20 = 3;
    template.unk_24 = 0x80;
    template.x = 0xC8;
    template.y = 0xAC;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = work->heapID;
    sub_02021AC8(size, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, &work->charTransfer);
    template.offset = work->charTransfer.offset;
    template.sprite = NULL;
    work->textObj = TextOBJ_Create(&template, work->fontLayout);
    TextOBJ_SetSpritesDrawFlag(work->textObj, TRUE);
    TextOBJ_SetPaletteNum(work->textObj, 4);
}
