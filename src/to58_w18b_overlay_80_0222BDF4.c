typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma thumb on
extern void *FrontierSystem_GetVarPtr(void *, int);
extern void GF_AssertFail(void);
void *ov80_0222BE24(void *, u32);
void *ov80_0222BE24(void *p, u32 index) {
    if (index < 0x8000) return 0;
    if (index < 0x8008) return (u16 *)((u8 *)p + 8) + (index - 0x8000);
    if (index < 0x8010) return FrontierSystem_GetVarPtr(*(void **)p, index - 0x8008);
    if (index < 0x8014) return (u16 *)((u8 *)p + 0x78) + (index - 0x8010);
    if (index == 0xfd13) return 0;
    if (index == 0xeeee) return 0;
    if (index == 0xeeef) return 0;
    GF_AssertFail();
    return 0;
}
