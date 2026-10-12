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

int sub_02094860(BoxMonSelector *ptr, int index) {
    int ret = sub_02094894(ptr, index);
    if (index < ptr->count) {
        Sprite_SetMatrix(ptr->spriteB, Sprite_GetMatrixPtr(ptr->entries[index].sprite));
        return ret;
    }
    return ret;
}

int sub_02094894(BoxMonSelector *ptr, int index) {
    if (index >= ptr->count) {
        return 3;
    }
    if (ptr->entries[index].sprite != NULL && !Sprite_GetDrawFlag(ptr->entries[index].sprite)) {
        return 3;
    }
    return 1;
}
