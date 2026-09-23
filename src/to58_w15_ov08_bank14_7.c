#include "global.h"
#pragma force_active on

typedef struct SpriteTemplate {
    s16 x, y, z;
    u16 anim;
    int priority;
    int pal;
    int vram;
    int resources[6];
    int bgPriority;
    int transfer;
} SpriteTemplate;
typedef struct WorkWithManager { u8 pad[0x1FB4]; void *manager; } WorkWithManager;

typedef struct SixCounts { int n[6]; } SixCounts;
extern const SixCounts ov08_0222541C;
extern const int ov08_02225644[];
extern const int ov08_02225648[];
extern const int ov08_0222564C[];
extern const int ov08_02225650[];
extern const int ov08_02225654[];
extern void *BattleSystem_GetSpriteSystem(void *);
extern void *SpriteManager_New(void);
extern int SpriteSystem_InitSprites(void *, void *, int);
extern int SpriteSystem_InitManagerWithCapacities(void *, void *, void *);
extern void *NARC_New(int, int);
extern void NARC_Delete(void *);
extern int sub_02074490(void);
extern int sub_02074498(void);
extern int sub_020744A4(void);
extern int sub_0208AD58(void);
extern int sub_0208AD5C(void);
extern int sub_0208AD60(void);
extern int sub_0208AD54(void);
extern int sub_0207CAA0(void);
extern int sub_0207CAA4(void);
extern int sub_0207CAA8(void);
extern int sub_0207CA9C(void);
extern int Pokemon_GetIconNaix(void *);
extern int GetMonIconNaixEx(int, int, int);
extern int SpriteSystem_LoadPaletteBufferFromOpenNarc(void *, int, void *, void *, void *, int, int, int, int, int);
extern int SpriteSystem_LoadCellResObjFromOpenNarc(void *, void *, void *, int, int, int);
extern int SpriteSystem_LoadAnimResObjFromOpenNarc(void *, void *, void *, int, int, int);
extern int SpriteSystem_LoadCharResObjFromOpenNarc(void *, void *, void *, int, int, int, int);
extern int SpriteSystem_LoadPaletteBuffer(void *, int, void *, void *, int, int, int, int, int, int);
extern int SpriteSystem_LoadCellResObj(void *, void *, int, int, int, int);
extern int SpriteSystem_LoadAnimResObj(void *, void *, int, int, int, int);
extern int SpriteSystem_LoadCharResObj(void *, void *, int, int, int, int, int);
extern void sub_02077720(void *, int, void *, void *, int, int);
extern void sub_0207775C(void *, void *, int, int);
extern void sub_020776B8(void *, void *, int, int, int);
extern void sub_02077834(void *, void *, int, int, int);
extern void *SpriteSystem_NewSprite(void *, void *, const SpriteTemplate *);

#define ROOT(w) (*(void **)(w))
#define AT(w, n) (*(void **)((u8 *)(w) + (n)))

void ov08_0222061C(void *);
void ov08_02220668(void *);
void ov08_02220750(void *);
void ov08_02220800(void *);
void ov08_02220878(void *);
void ov08_02220928(void *);
void *ov08_022209B8(void *, int);

void ov08_0222061C(void *work) {
    SixCounts capacities = ov08_0222541C;
    void *system;
    void *manager;
    system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    manager = SpriteManager_New();
    AT(work, 0x1FB4) = manager;
    SpriteSystem_InitSprites(system, AT(work, 0x1FB4), 43);
    SpriteSystem_InitManagerWithCapacities(system, AT(work, 0x1FB4), &capacities);
}

void ov08_02220668(void *work) {
    void *narc = NARC_New(20, *(int *)((u8 *)ROOT(work) + 12));
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    u32 i;
    u8 *slot;
    SpriteSystem_LoadPaletteBufferFromOpenNarc(AT(work, 0x1E8), 3, system, ((WorkWithManager *)work)->manager, narc, sub_02074490(), 0, 3, 2, 0xB007);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, ((WorkWithManager *)work)->manager, narc, sub_02074498(), 0, 0xB007);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, ((WorkWithManager *)work)->manager, narc, sub_020744A4(), 0, 0xB007);
    for (i = 0, slot = work; i < 6; i++, slot += 0x50) {
        if (*(u16 *)(slot + 8)) {
            SpriteSystem_LoadCharResObjFromOpenNarc(system, ((WorkWithManager *)work)->manager, narc, Pokemon_GetIconNaix(AT(slot, 4)), 0, 2, 0xB007 + i);
        } else {
            SpriteSystem_LoadCharResObjFromOpenNarc(system, ((WorkWithManager *)work)->manager, narc, GetMonIconNaixEx(0, 0, 0), 0, 2, 0xB007 + i);
        }
    }
    NARC_Delete(narc);
}

void ov08_02220750(void *work) {
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    void *narc = NARC_New(39, *(int *)((u8 *)ROOT(work) + 12));
    SpriteSystem_LoadPaletteBufferFromOpenNarc(AT(work, 0x1E8), 3, system, AT(work, 0x1FB4), narc, sub_0208AD58(), 0, 1, 2, 0xB008);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0208AD5C(), 0, 0xB008);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0208AD60(), 0, 0xB008);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0208AD54(), 0, 2, 0xB00D);
    NARC_Delete(narc);
}

void ov08_02220800(void *work) {
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    u32 id;
    u32 end;
    sub_02077720(AT(work, 0x1E8), 3, system, ((WorkWithManager *)work)->manager, 2, 0xB009);
    sub_0207775C(system, ((WorkWithManager *)work)->manager, 0xB009, 0xB009);
    end = 0xB00E;
    id = end;
    end += 6;
    do { sub_020776B8(system, ((WorkWithManager *)work)->manager, 2, 0, id); id++; } while (id <= end);
    sub_02077834(system, ((WorkWithManager *)work)->manager, 2, 0, 0xB015);
}

void ov08_02220878(void *work) {
    void *narc = NARC_New(21, *(int *)((u8 *)ROOT(work) + 12));
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    SpriteSystem_LoadPaletteBufferFromOpenNarc(AT(work, 0x1E8), 3, system, AT(work, 0x1FB4), narc, sub_0207CAA0(), 0, 1, 2, 0xB00A);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0207CAA4(), 0, 0xB00A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0207CAA8(), 0, 0xB00A);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, AT(work, 0x1FB4), narc, sub_0207CA9C(), 0, 2, 0xB016);
    NARC_Delete(narc);
}

void ov08_02220928(void *work) {
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    SpriteSystem_LoadPaletteBuffer(AT(work, 0x1E8), 3, system, AT(work, 0x1FB4), 0x47, 0x1B, 0, 1, 2, 0xB00B);
    SpriteSystem_LoadCellResObj(system, AT(work, 0x1FB4), 0x47, 0x19, 0, 0xB00B);
    SpriteSystem_LoadAnimResObj(system, AT(work, 0x1FB4), 0x47, 0x18, 0, 0xB00B);
    SpriteSystem_LoadCharResObj(system, AT(work, 0x1FB4), 0x47, 0x1A, 0, 2, 0xB017);
}

void *ov08_022209B8(void *work, int index) {
    SpriteTemplate tmpl;
    int off;
    void *system = BattleSystem_GetSpriteSystem(AT(ROOT(work), 8));
    tmpl.x = 0; tmpl.y = 0; tmpl.z = 0; tmpl.anim = 0;
    off = index;
    off *= 20;
    tmpl.priority = *(int *)((u8 *)ov08_02225654 + off);
    tmpl.pal = 0;
    tmpl.vram = 2;
    tmpl.resources[0] = *(int *)((u8 *)ov08_02225644 + off);
    tmpl.resources[1] = *(int *)((u8 *)ov08_02225648 + off);
    tmpl.resources[2] = *(int *)((u8 *)ov08_0222564C + off);
    tmpl.resources[3] = *(int *)((u8 *)ov08_02225650 + off);
    tmpl.bgPriority = 1;
    tmpl.transfer = 0;
    return SpriteSystem_NewSprite(system, AT(work, 0x1FB4), &tmpl);
}
