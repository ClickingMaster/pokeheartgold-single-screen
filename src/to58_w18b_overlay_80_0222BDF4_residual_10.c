typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma thumb on
extern void *Heap_Alloc(int, int);
extern void MI_CpuFill8(void *, int, int);
extern void GF_AssertFail(void);
extern void ov80_0222EEC8(void *, void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *, void *), void *, int);
extern u8 *sub_0209680C(void *);
extern int FrontierScript_ReadVar(void *);
extern int FrontierScriptContext_ReadWord(void *);
extern void *ov80_02239938(void *, int);
int FrtCmd_040(u8 *);
void ov80_0222C7B0(int, u8 *, void *, void *, void *, int);
int FrtCmd_040(u8 *ctx) {
    u8 *base = *(u8 **)ctx;
    u8 *p = sub_0209680C(*(void **)base);
    int id = FrontierScript_ReadVar(ctx);
    int delta = FrontierScriptContext_ReadWord(ctx);
    void *owner = ov80_02239938(*(void **)base, id);
    if (owner == 0) GF_AssertFail();
    ov80_0222C7B0(id, owner, (u8 *)*(u32 *)(ctx + 0x1c) + delta, *(void **)(p + 0x30), base + 0x59, *(u32 *)(base + 0x34));
    base[0x59]++;
    return 0;
}
void ov80_0222C7B0(int id, u8 *owner, void *third, void *fourth, void *fifth, int heapId) {
    u8 *task = Heap_Alloc(heapId, 0x18);
    MI_CpuFill8(task, 0, 0x18);
    *(void **)(task + 0x14) = owner;
    *(void **)(task + 0xc) = third;
    *(void **)(task + 0x10) = fourth;
    *(u16 *)(task + 4) = id;
    *(void **)(task + 8) = fifth;
    if (*(void **)(owner + 0x38)) GF_AssertFail();
    *(void **)(owner + 0x38) = SysTask_CreateOnMainQueue(ov80_0222EEC8, task, 0xeac4);
}
