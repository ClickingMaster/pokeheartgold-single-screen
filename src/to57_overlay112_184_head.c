#include "global.h"
#pragma require_prototypes off

typedef struct Ov112SpriteTemplate {
    u16 x;
    u16 y;
    u16 z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} Ov112SpriteTemplate;

extern const Ov112SpriteTemplate ov112_021FF400;
extern void *SpriteSystem_NewSpriteWithYOffset(void *, void *, const Ov112SpriteTemplate *, int);
extern void ManagedSprite_SetAnimateFlag(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void *ov112_021F1AF4(void *, void *, int, int, int, int);

void ov112_021F2000(void *out, void *system, void *manager, u16 x, u16 y, u8 anim, int resource) {
    Ov112SpriteTemplate template = ov112_021FF400;
    int i;
    for (i = 0; i < 2; i++) {
        template.x = x;
        template.y = y;
        template.drawPriority = 3;
        template.animation = anim;
        template.resIdList[0] = resource + 2 + i * 2;
        template.resIdList[1] = resource + 2;
        template.resIdList[2] = i + 2;
        template.resIdList[3] = i + 2;
        ((void **)out)[2 + i] = SpriteSystem_NewSpriteWithYOffset(system, manager, &template, 0x20C000);
        ManagedSprite_SetAnimateFlag(((void **)out)[2 + i], 1);
        ManagedSprite_SetDrawFlag(((void **)out)[2 + i], 0);
    }
    ((void **)out)[4] = ov112_021F1AF4(system, manager, x, y, 0x17, 4);
}
