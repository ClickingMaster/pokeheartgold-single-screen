typedef unsigned char u8;
typedef unsigned int u32;
#pragma thumb on
extern u8 *sub_0209680C(void *);
extern int FrontierScriptContext_ReadWord(void *);
extern int FrontierScript_ReadVar(void *);
extern void *NARC_New(int, int);
extern void NARC_Delete(void *);
extern void FrontierMapSprite_LoadResources(u32, u32, void *, u32, u32);
extern void FrontierMap_AddLoadedResource(u8 *, int);
int FrtCmd_042(u8 *);
int FrtCmd_042(u8 *ctx) {
    u8 *base = *(u8 **)ctx;
    u8 *p = sub_0209680C(*(void **)base);
    int delta = FrontierScriptContext_ReadWord(ctx);
    u32 old = *(u32 *)(ctx + 0x1c);
    *(u32 *)(ctx + 0x1c) = old + delta;
    void *narc = NARC_New(0xb8, *(u32 *)(base + 0x34));
    for (;;) {
        int value = FrontierScript_ReadVar(ctx);
        if (value == 0xfd13) break;
        FrontierMapSprite_LoadResources(*(u32 *)(p + 0x34), *(u32 *)(p + 0x38), narc, *(u32 *)(p + 4), value);
        FrontierMap_AddLoadedResource(p, value);
    }
    NARC_Delete(narc);
    *(u32 *)(ctx + 0x1c) = old;
    return 0;
}
