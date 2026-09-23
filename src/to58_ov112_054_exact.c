typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma thumb on

extern void ov112_021EA9BC(void *, void *);
extern void ov112_021EAA34(void *, int, int);
extern void ov112_021EAA10(void *);
extern int sub_02074490(void);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern int GetMonIconNaixEx(int, int, int);
extern void *GfGfxLoader_GetCharData(int, int, int, void *, int);
extern void DC_FlushRange(void *, int);
extern void Heap_Free(void *);
extern void GX_LoadOBJ(void *, int, int);
extern int GetMonIconPaletteEx(int, int, int);
extern void ManagedSprite_SetPaletteOverride(void *, int);

void ov112_021EAA98(void *, u8 *, int);
int ov112_021EAAE4(void *, u8 *);
void ov112_021EAB58(void);
void ov112_021EAB78(u8 *, int, int, int);
void ov112_021EABE8(void *, int, int, int, int, void *);

int ov112_021EAAE4(void *arg, u8 *state) {
    s8 step = (s8)state[2];
    if (step < 0) {
        *(s16 *)(state + 6) += step;
        if (*(s16 *)(state + 6) < -110) state[2] = 0;
        ov112_021EAA34(arg, *(s16 *)(state + 4), *(s16 *)(state + 6));
        return 0;
    }
    if (step > 0 && step <= 100) {
        *(s16 *)(state + 6) += step;
        if (*(s16 *)(state + 6) > 108) state[2] = 101;
        ov112_021EAA34(arg, *(s16 *)(state + 4), *(s16 *)(state + 6));
        return 0;
    }
    if (step != 0) {
        ov112_021EAA34(arg, *(s16 *)(state + 4), *(s16 *)(state + 6));
        return 0;
    }
    ov112_021EAA10(arg);
    return 1;
}

void ov112_021EAB58(void) {
    int naix = sub_02074490();
    GfGfxLoader_GXLoadPal(0x14, naix, 1, 0xa0, 0x60, 0x9a);
}

void ov112_021EAB78(u8 *state, int param, int mon, int form) {
    int icon = GetMonIconNaixEx(mon, 0, form);
    void *data = GfGfxLoader_GetCharData(0x14, icon, 0, state + 0x1ea64, 0x9a);
    *(void **)(state + 0x1ea60) = data;
    DC_FlushRange(*(void **)(*(u8 **)(state + 0x1ea64) + 0x14), 0x400);
    ov112_021EABE8(*(void **)(state + 0x1ea64), mon, form, 0, param, *(void **)(state + ((param + 9) << 2) + 0x1e530));
    Heap_Free(*(void **)(state + 0x1ea60));
}

void ov112_021EABE8(void *charData, int mon, int form, int unused, int index, void *sprite) {
    GX_LoadOBJ(*(void **)((u8 *)charData + 0x14), ((index << 5) + 0xc0) << 5, 0x400);
    ManagedSprite_SetPaletteOverride(sprite, GetMonIconPaletteEx(mon, form, unused) + 5);
}
