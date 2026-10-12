#include "box_mon_selector_internal.h"

void PlaySE(u32 seq);
void GF_RunVramTransferTasks(void);
void OamManager_ApplyAndResetBuffers(void);

extern const BgTemplate _021083E4;
extern const BgTemplate _021083C8;
extern const BgTemplate _021083AC;
extern const BgTemplate _02108400;

void sub_020956B8(BoxMonSelector *ptr);
int sub_02093630(BoxMonSelector *ptr);
void sub_0209389C(BoxMonSelector *ptr);
void sub_02093908(BoxMonSelector *ptr);
void sub_0209396C(BoxMonSelector *ptr);
void sub_020939B8(BoxMonSelector *ptr);
void sub_0209511C(BoxMonSelector *ptr);
void sub_0209501C(BoxMonSelector *ptr, int a1, int a2);
void sub_02095780(void);
void sub_02095794(void);
void sub_02094F5C(BoxMonSelector *ptr, int a1);
void sub_020950C0(BoxMonSelector *ptr);
void sub_02094A90(BoxMonSelector *ptr, int a1);
int sub_02093CE4(BoxMonSelector *ptr);
int sub_020947C0(BoxMonSelector *ptr, int a1);
int sub_02094894(BoxMonSelector *ptr, int a1);
int sub_02094528(BoxMonSelector *ptr);
int sub_02094400(BoxMonSelector *ptr);

void BoxMonSelector_VBlank(BoxMonSelector *ptr);
int BoxMonSelector_Run(BoxMonSelector *ptr);
void BoxMonSelector_RenderSprites(BoxMonSelector *ptr);
void sub_02093B40(BoxMonSelector *ptr);
void sub_02094150(SpriteTemplate *tmpl, BoxMonSelector *ptr, NNS_G2D_VRAM_TYPE vram);
void sub_0209428C(TouchscreenHitbox *rect, int x, int y, int width, int height);
int sub_020943EC(BoxMonSelector *ptr);
void sub_02094758(UnkGfxBuf_02094758 *buf);
int sub_02094794(BoxMonSelector *ptr, int index);
int sub_02094860(BoxMonSelector *ptr, int index);
void sub_02094A70(BoxMonSelector *ptr);
void sub_02094F14(BoxMonSelector *ptr, int index, MonDisplayInfo *dest);
void sub_020950D4(BoxMonSelector *ptr);
void sub_020950F8(Window *window, int a1);
void sub_0209569C(BoxMonSelector *ptr);
Sprite *sub_0209417C(BoxMonSelector *ptr, int x, int y, int animSeq, BOOL drawFlag);
void sub_02094C6C(u32 fileId, NNSG2dCharacterData **ppChar, void *dest, NARC *narc);
void sub_02094F14(BoxMonSelector *ptr, int slot, MonDisplayInfo *dest);
Sprite *Sprite_CreateAffine(const SpriteTemplate *template);
void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL flag);
void sub_02094DF8(BgConfig *bgConfig);
void sub_02094D9C(BgConfig *bgConfig, enum HeapID heapId);
void sub_02094D1C(BoxMonSelector *ptr);
int sub_020954CC(BoxMonSelector *ptr);
void sub_02094EB4(BoxMonSelector *ptr);

void sub_0209389C(BoxMonSelector *ptr) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = ptr->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 0x19;
    template.y = 0xF;
    YesNoPrompt_InitFromTemplate(ptr->yesNoPrompt, &template);
    if (ptr->count == 1) {
        sub_020950F8(&ptr->window1, 0);
        sub_0209501C(ptr, 3, 1);
    } else {
        sub_0209501C(ptr, 2, 1);
    }
    sub_02095780();
    ptr->state = 2;
}

void sub_02093908(BoxMonSelector *ptr) {
    switch (YesNoPrompt_HandleInput(ptr->yesNoPrompt)) {
    case 1:
        sub_020950F8(&ptr->window1, 1);
        YesNoPrompt_Reset(ptr->yesNoPrompt);
        ptr->state = 6;
        break;
    case 2:
        sub_020950F8(&ptr->window1, 1);
        YesNoPrompt_Reset(ptr->yesNoPrompt);
        sub_02094F5C(ptr, ptr->unk_46B0);
        sub_020950C0(ptr);
        sub_02095794();
        sub_020956B8(ptr);
        ptr->state = 0;
        break;
    }
}

void sub_0209396C(BoxMonSelector *ptr) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = ptr->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 0x19;
    template.y = 0xF;
    YesNoPrompt_InitFromTemplate(ptr->yesNoPrompt, &template);
    sub_0209501C(ptr, 4, 0);
    sub_02095780();
    ptr->state = 5;
}

void sub_020939B8(BoxMonSelector *ptr) {
    switch (YesNoPrompt_HandleInput(ptr->yesNoPrompt)) {
    case 1:
        sub_020950F8(&ptr->window1, 1);
        YesNoPrompt_Reset(ptr->yesNoPrompt);
        sub_02094F5C(ptr, ptr->unk_4684);
        Sprite_SetAnimCtrlSeq(ptr->spriteB, 0x2D);
        sub_02094A90(ptr, 2);
        ptr->unk_46BC = 0;
        sub_02095794();
        sub_020956B8(ptr);
        ptr->state = 0;
        break;
    case 2:
        sub_020950F8(&ptr->window1, 1);
        YesNoPrompt_Reset(ptr->yesNoPrompt);
        sub_02094A90(ptr, 0);
        sub_02095794();
        ptr->state = 0;
        break;
    }
}

void BoxMonSelector_RenderSprites(BoxMonSelector *ptr) {
    if (ptr->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(ptr->spriteList);
    }
}
