#include "global.h"

extern void *AreaDataManager_GetMapTexture(void *);
extern void *ov01_021F676C(void *, u32, u32, void *, void *, void *, void *, void *, void *);
extern void *Bdhc_StartLoadTask(void *, u32, u32, void *, void *, void *);
extern void *ov01_021F67B4(void *, u32, void *, void *, void *);
extern void ov01_021EA3B0(void *);
extern void Bdhc_Load(void *, u32, u32, u32);
extern int ov01_02204698(void *);
extern int sub_02054E20(void *);
extern void ov01_02204678(void *, void *);
extern void MapPropManager_LoadFromNARC(void *, u32, u32, u32);
extern void *Save_SafariZone_Get(void *);
extern int sub_0202F620(void);
extern void *SafariZone_GetAreaSet(void *, int);
extern void *Save_PlayerData_GetProfile(void *);
extern int PlayerProfile_GetTrainerGender(void *);
extern int SafariZone_GetLinkLeaderGender(void *);
extern void MapPropManager_LoadFromSafariZone(void *, u32, u32, void *, void *, int);
void ov01_021F4B1C(void *manager, void *load, void *slot, int mapId);
void ov01_021F4BE8(void *manager, void *load, void *slot, void *area, void *task, u32 arg5, int flag);
void ov01_021F4C6C(void *manager, void *load, void *slot, void *area, void *arg4, u32 arg5, int arg6, int arg7);

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))

void ov01_021F4B1C(void *manager, void *load, void *slot, int mapId) {
    u8 *areas;
    int width = W(manager, 0xc4);
    u16 x = mapId % width;
    u16 y = mapId / width;
    if (x < 1 || x > 3 || y < 1 || y > 2) {
        MapPropManager_LoadFromNARC((void *)W(manager, 0x100), W(slot, 0xc), W(load, 0x868), W(manager, 0xf4));
    } else {
        void *zone = Save_SafariZone_Get((void *)W(manager, 0x104));
        int link = sub_0202F620();
        areas = SafariZone_GetAreaSet(zone, link);
        int gender;
        if (link == 0) {
            gender = (u8)PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile((void *)W(manager, 0x104)));
        } else {
            gender = SafariZone_GetLinkLeaderGender(zone);
        }
        u16 xoff = x - 1;
        u16 yoff = y - 1;
        MapPropManager_LoadFromSafariZone((void *)W(manager, 0x100), W(load, 0x868), W(manager, 0xf4), areas + 0x7a * (xoff + 3 * yoff), load, gender);
    }
}

void ov01_021F4BE8(void *manager, void *load, void *slot, void *area, void *task, u32 arg5, int flag) {
    void *texture;
    W(task, 0x10)++;
    texture = AreaDataManager_GetMapTexture(area);
    W(task, 8) = (u32)ov01_021F676C((void *)W(manager, 0x100), arg5, W(slot, 4), (u8 *)load + 0x800, (u8 *)load + 0x854, texture, (void *)W(manager, 0xf8), (u8 *)load + 0x864, (u8 *)task + 0x10);
    if (flag) {
        W(task, 0x14)++;
        W(task, 0xc) = (u32)Bdhc_StartLoadTask((void *)W(manager, 0x100), W(slot, 8), W(load, 0x85c), (u8 *)task + 0x14, (u8 *)load + 0x858, (u8 *)task + 0x10);
    }
}

void ov01_021F4C6C(void *manager, void *load, void *slot, void *area, void *arg4, u32 arg5, int arg6, int arg7) {
    void *texture = AreaDataManager_GetMapTexture(area);
    void *obj = ov01_021F67B4((void *)W(manager, 0x100), W(slot, 4), (u8 *)load + 0x800, (u8 *)load + 0x854, texture);
    if (arg6) {
        ov01_021EA3B0(obj);
    }
    W(load, 0x864) = 1;
    W(load, 0x860) = arg5;
    if (arg7) {
        Bdhc_Load((void *)W(manager, 0x100), W(slot, 8), W(load, 0x85c), W(load, 0x858));
        if (W(manager, 0x108) != 0) {
            ((void (*)(void *, u32, u32))W(manager, 0x108))((void *)W(manager, 0x10c), arg5, W(load, 0x868));
        }
        if (ov01_02204698((void *)W(manager, 0xf8)) && !sub_02054E20(arg4)) {
            ov01_02204678((void *)W(manager, 0xf8), (u8 *)load + 0x800);
        }
    }
}
