typedef unsigned char u8;
typedef unsigned int u32;
#pragma thumb on

extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern int IsPaletteFadeFinished(void);
extern void ov112_021EAA10(void *);
extern int TextPrinterCheckActive(u8);
extern int ManagedSprite_IsAnimated(void *);
extern void ov112_021EA570(void *);
extern void ov112_021EA64C(void *);
extern void ov112_021EA688(void *, int);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
int ov112_021EE52C(void);
int ov112_021EE550(void *);
int ov112_021EE570(void *);
int ov112_021EE594(void *);
int ov112_021EE5B8(void *);

int ov112_021EE52C(void) {
    BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, 0x9a);
    return 0x3f;
}

int ov112_021EE550(void *p) {
    if (IsPaletteFadeFinished() == 1) {
        ov112_021EAA10(p);
        *(u32 *)((u8 *)p + 4) = 0;
        return 0x42;
    }
    return 0x3f;
}

int ov112_021EE570(void *p) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1e524))) {
        return *(u32 *)((u8 *)p + 8) = *(u32 *)((u8 *)p + 12);
    }
    return 0x40;
}

int ov112_021EE594(void *p) {
    if (!ManagedSprite_IsAnimated(*(void **)((u8 *)p + 0x1e530))) {
        ov112_021EA570(p);
        return *(u32 *)((u8 *)p + 8) = *(u32 *)((u8 *)p + 12);
    }
    return 0x41;
}

int ov112_021EE5B8(void *p) {
    ov112_021EA64C(p);
    ov112_021EA688(p, 3);
    ov112_021EA688(p, 4);
    ov112_021EA688(p, 8);
    FillBgTilemapRect(*(void **)((u8 *)p + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)p + 0x18), 1);
    return 0x43;
}
