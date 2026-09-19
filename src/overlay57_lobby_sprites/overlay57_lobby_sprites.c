#include "private.h"

static void ov57_0223927C(SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *paletteData,
    int charFile, int paletteFile, int cellFile, int animFile, int charVram,
    int paletteBuffer, int paletteNum, NARC *narc)
{
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, charFile, TRUE, charVram, charFile + 0x2AF8);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, (PaletteBufferId)paletteBuffer, spriteSystem, spriteManager, narc,
        paletteFile, FALSE, paletteNum, charVram, paletteFile + 0x2AF8);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, cellFile, TRUE, cellFile + 0x2AF8);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, animFile, TRUE, animFile + 0x2AF8);
}

void ov57_022392F4(Ov57R7Work *work, NARC *narc)
{
    s16 coords[4];
    ManagedSpriteTemplate template;
    int i;
    Ov57R7Work *cursor;

    ov57_0223927C(work->spriteSystem, work->spriteManager, work->paletteData, 0x11, 3, 0x13, 0x12, 1, 2, 1, narc);
    ov57_0223927C(work->spriteSystem, work->spriteManager, work->paletteData, 0x20, 5, 0x22, 0x21, 1, 2, 1, narc);

    i = 0;
    cursor = work;
    for (; i < 12; i++, cursor = (Ov57R7Work *)((u8 *)cursor + 12)) {
        template.x = 0;
        template.y = 0;
        template.z = 0;
        template.animation = *(int *)((u8 *)cursor + 0x290);
        template.drawPriority = 0x28 - i;
        template.pal = 0;
        template.vram = (NNS_G2D_VRAM_TYPE)1;
        template.bgPriority = 2;
        template.vramTransfer = 0;
        template.resIdList[0] = 0x2B09;
        template.resIdList[1] = 0x2AFB;
        template.resIdList[2] = 0x2B0B;
        template.resIdList[3] = 0x2B0A;
        template.resIdList[4] = -1;
        template.resIdList[5] = -1;
        *(ManagedSprite **)((u8 *)cursor + 0x294) = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ov57_02238DAC(i, &coords[3], &coords[2]);
        ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)cursor + 0x294), coords[3], coords[2]);
    }

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0x14;
    template.pal = 0;
    template.vram = (NNS_G2D_VRAM_TYPE)1;
    template.bgPriority = 2;
    template.vramTransfer = 0;
    template.resIdList[0] = 0x2B18;
    template.resIdList[1] = 0x2AFD;
    template.resIdList[2] = 0x2B1A;
    template.resIdList[3] = 0x2B19;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    *(ManagedSprite **)((u8 *)work + 0x31C) = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    *(ManagedSprite **)((u8 *)work + 0x320) = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ov57_02238DAC(*(int *)((u8 *)work + 0x3EC), &coords[1], &coords[0]);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)work + 0x31C), coords[1], coords[0]);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)work + 0x320), coords[1], coords[0]);
    ManagedSprite_SetDrawPriority(*(ManagedSprite **)((u8 *)work + 0x320), 0x19);
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)work + 0x31C), 0);
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)work + 0x320), 0);
}

void ov57_022394AC(Ov57R7Work *work)
{
    u32 animOffset;
    int i;
    u32 spriteOffset;

    ov57_02239014(work);
    ov57_02238F48(work);
    animOffset = 0x290;
    i = 0;
    spriteOffset = animOffset + 4;
    for (; i < 12; i++, work = (Ov57R7Work *)((u8 *)work + 12)) {
        ManagedSprite *sprite = *(ManagedSprite **)((u8 *)work + spriteOffset);
        if (sprite != NULL) {
            ManagedSprite_SetAnim(sprite, *(int *)((u8 *)work + animOffset));
        }
    }
}

void ov57_022394D8(Ov57R7Work *work, int a, int b)
{
    CAPSULE **capsuleSlots;
    CAPSULE temp;
    u32 offA = a << 3;
    u32 monA = *(u32 *)((u8 *)work + offA + 4);
    u32 offB = b << 3;
    u32 monB = *(u32 *)((u8 *)work + offB + 4);
    u32 *monSlots;
    CAPSULE **capsuleBase;

    if (monA != 0xFF) {
        int capsuleNo = b + 1;
        SetMonData(*(Pokemon **)((u8 *)work->mons + monA * 4 + 4), MON_DATA_BALL_CAPSULE_ID, &capsuleNo);
    }
    if (monB != 0xFF) {
        int capsuleNo = a + 1;
        SetMonData(*(Pokemon **)((u8 *)work->mons + monB * 4 + 4), MON_DATA_BALL_CAPSULE_ID, &capsuleNo);
    }

    monSlots = (u32 *)((u8 *)work + 4);
    monA = *(u32 *)((u8 *)monSlots + offA);
    monB = *(u32 *)((u8 *)monSlots + offB);
    *(u32 *)((u8 *)monSlots + offA) = monB;
    *(u32 *)((u8 *)monSlots + offB) = monA;

    capsuleBase = (CAPSULE **)((u8 *)work + 8);
    CopyCapsule(*(CAPSULE **)((u8 *)capsuleBase + offA), &temp);
    capsuleSlots = (CAPSULE **)work;
    capsuleSlots = (CAPSULE **)((u8 *)capsuleSlots + 8);
    CopyCapsule(*(CAPSULE **)((u8 *)capsuleSlots + offB), *(CAPSULE **)((u8 *)capsuleBase + offA));
    CopyCapsule(&temp, *(CAPSULE **)((u8 *)capsuleSlots + offB));
    ov57_022394AC(work);
}
