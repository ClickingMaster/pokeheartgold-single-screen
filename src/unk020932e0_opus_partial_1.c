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

void BoxMonSelector_VBlank(BoxMonSelector *ptr) {
    if (ptr->callback != NULL) {
        ptr->callback((u8 *)ptr + 0x8D4);
        ptr->callback = NULL;
        sub_020956B8(ptr);
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(ptr->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

int BoxMonSelector_Run(BoxMonSelector *ptr) {
    switch (ptr->state) {
    case 0:
        return sub_02093630(ptr);
    case 1:
        sub_0209389C(ptr);
        return 0;
    case 2:
        sub_02093908(ptr);
        return 0;
    case 3:
        break;
    case 4:
        sub_0209396C(ptr);
        return 0;
    case 5:
        sub_020939B8(ptr);
        return 0;
    case 6:
        sub_0209511C(ptr);
        return 2;
    }
}
