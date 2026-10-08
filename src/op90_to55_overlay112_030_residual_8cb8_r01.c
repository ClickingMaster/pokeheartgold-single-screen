#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"

typedef struct WalkerMon {
    u16 species;
    u16 item;
    u16 moves[4];
    u8 level;
    u8 form : 5;
    u8 gender : 2;
    u8 unk7 : 1;
    u8 ability : 1;
    u8 shiny : 1;
    u8 unkE : 6;
    u8 padF;
} WalkerMon;

typedef struct WalkerRouteItem {
    u16 item;
    u16 steps;
    u16 chance;
} WalkerRouteItem;

typedef struct WalkerRoute {
    u8 header[8];
    u8 mons[6][0x14];
    WalkerRouteItem items[10];
    u8 unkBC[4];
} WalkerRoute;

typedef struct WalkerReport {
    WalkerMon mon;
    u16 nickname[11];
    u8 friendship;
    u8 courseType;
    u16 courseName[0x15];
    WalkerMon routeMons[3];
    u16 routeMonSteps[3];
    u8 routeMonChances[3];
    u8 pad8B;
    u16 items[10];
    u16 itemSteps[10];
    u8 itemChances[10];
    u8 unkBE[0xC0];
    u8 courseNameImage[0x140];
    u8 monIcon[0x180];
    u8 monImage[0x600];
    u8 nicknameImage[0x140];
    u8 routeMonIcons[3 * 0x180];
    u8 routeMonImage[0x600];
    u8 routeMonNameImages[3 * 0x140];
    u8 itemNameImages[10 * 0x180];
} WalkerReport;

extern WalkerRoute ov112_021F4138[];
typedef struct WalkerRouteFrom4 {
    int type;
    u8 rest[0xBC];
} WalkerRouteFrom4;

extern const WalkerRouteFrom4 ov112_021F413C[];
extern const u8 ov112_021F5578[];
extern const void *ov112_021FF528[];
extern const u8 ov112_021FE498[];

void ov112_021E8B74(WalkerMon *out, u16 *steps, u8 *chances, int route, u8 *chosen);
void ov112_021E8C5C(const u8 *values, void *boxmon, u16 *stats);
void ov112_021E9148(void *windows, void *work, void *data, const u8 *indices);
u32 ov112_021E9464(void *boxMon);
u32 ov112_021E9470(u16 species);
int ov112_021E9480(int species, int form, int gender);
void ov112_021E9A30(void *dst, const void *src, int value);
int ov112_021E93BC(u8 *src, int width, int unused, u8 *dst, int x, int y, int w, int h);
void ov112_021E8CB8(WalkerReport *out, u8 *work);

#define WORK_U8(o)  (*(u8 *)(work + (o)))
#define WORK_U16(o) (*(u16 *)(work + (o)))
#define WORK_U32(o) (*(u32 *)(work + (o)))
#define WORK_PTR(o) (*(void **)(work + (o)))

// @fn ov112_021E8CB8
void ov112_021E8CB8(WalkerReport *out, u8 *work) {
    Window windows[15];
    u8 chosen[4];
    u8 *data;
    u32 i;
    u8 course;
    u8 *buf;
    int j;
    u16 *p;
    BASE_STATS *personal;
    String *str;

    course = WORK_U32(0x1D798);
    buf = Heap_Alloc(HEAP_ID_154, 0x180);
    for (i = 0; i < NELEMS(windows); i++) {
        AddWindowParameterized(WORK_PTR(0x18), &windows[i], 1, 0, 0, ov112_021F5578[i], 2, 0, 0);
        FillWindowPixelBuffer(&windows[i], 0);
    }
    out->mon.species = WORK_U16(0x1D77C);
    AcquireBoxMonLock(WORK_PTR(0x1E430));
    for (j = 0, p = &out->mon.species; j < 4; j++, p++) {
        p[2] = GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_MOVE1 + j, NULL);
    }
    out->mon.level = GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_LEVEL, NULL);
    out->mon.form = GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_FORM, NULL);
    out->mon.unk7 = 0;
    out->mon.shiny = BoxMonIsShiny(WORK_PTR(0x1E430));
    out->friendship = GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_FRIENDSHIP, NULL);
    GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_NICKNAME, out->nickname);
    ReleaseBoxMonLock(WORK_PTR(0x1E430), TRUE);
    personal = AllocAndLoadMonPersonal(out->mon.species, HEAP_ID_154);
    out->mon.ability = GetPersonalAttr(personal, 0x1C);
    FreeMonPersonal(personal);
    out->mon.gender = GetBoxMonGender(WORK_PTR(0x1E430));
    out->courseType = ov112_021F413C[course].type - 1;
    str = NewString_ReadMsgData(WORK_PTR(0x1E44C), WORK_U32(0x1D798) + 0x5B);
    CopyStringToU16Array(str, out->courseName, 0x15);
    String_Delete(str);
    ov112_021E8B74(out->routeMons, out->routeMonSteps, out->routeMonChances, course, chosen);
    ov112_021E8C5C(ov112_021F4138[course].unkBC, WORK_PTR(0x1E430), out->routeMonSteps);
    ov112_021E9148(windows, work, &ov112_021F4138[course], chosen);
    for (j = 0; j < 3; j++) {
        data = GfGfxLoader_LoadFromNarc(NARC_a_2_4_8, ov112_021E9470(out->routeMons[j].species), TRUE, HEAP_ID_154, TRUE);
        MI_CpuCopy8(data, &out->routeMonIcons[j * 0x180], 0x180);
        Heap_Free(data);
    }
    data = GfGfxLoader_LoadFromNarc(NARC_a_2_5_6, ov112_021E9480(out->routeMons[2].species, out->routeMons[2].gender, 0), TRUE, HEAP_ID_154, TRUE);
    MI_CpuCopy8(data, out->routeMonImage, 0x600);
    Heap_Free(data);
    for (j = 0; j < 3; j++) {
        ov112_021E93BC(windows[10 + j].pixelBuffer, 10, 2, buf, 0, 0, 0x50, 0x10);
        MI_CpuCopy8(buf, &out->routeMonNameImages[j * 0x140], 0x140);
    }
    for (j = 0; j < 10; j++) {
        out->items[j] = ((WalkerRoute *)(u8 *)ov112_021F4138)[course].items[j].item;
        out->itemSteps[j] = ((WalkerRoute *)(u8 *)ov112_021F4138)[course].items[j].steps;
        out->itemChances[j] = ((WalkerRoute *)(u8 *)ov112_021F4138)[course].items[j].chance;
    }
    MI_CpuCopy8(ov112_021FF528[ov112_021F413C[course].type], out->unkBE, 0xC0);
    ov112_021E93BC(windows[13].pixelBuffer, 10, 2, buf, 0, 0, 0x50, 0x10);
    MI_CpuCopy8(buf, out->courseNameImage, 0x140);
    data = GfGfxLoader_LoadFromNarc(NARC_a_2_4_8, ov112_021E9464(WORK_PTR(0x1E430)), TRUE, HEAP_ID_154, TRUE);
    MI_CpuCopy8(data, out->monIcon, 0x180);
    Heap_Free(data);
    GF_ASSERT(WORK_U16(0x1D77C) != 0);
    data = GfGfxLoader_LoadFromNarc(NARC_a_2_5_6, ov112_021E9480(WORK_U16(0x1D77C), WORK_U8(0x1D796), WORK_U8(0x1D794)), TRUE, HEAP_ID_154, TRUE);
    if (WORK_U16(0x1D77C) == SPECIES_SPINDA) {
        ov112_021E9A30(data, ov112_021FE498, GetBoxMonData(WORK_PTR(0x1E430), MON_DATA_PERSONALITY, NULL));
    }
    MI_CpuCopy8(data, out->monImage, 0x600);
    Heap_Free(data);
    ov112_021E93BC(windows[14].pixelBuffer, 10, 2, buf, 0, 0, 0x50, 0x10);
    MI_CpuCopy8(buf, out->nicknameImage, 0x140);
    for (j = 0; j < 10; j++) {
        ov112_021E93BC(windows[j].pixelBuffer, 12, 2, buf, 0, 0, 0x60, 0x10);
        MI_CpuCopy8(buf, &out->itemNameImages[j * 0x180], 0x180);
    }
    for (i = 0; i < NELEMS(windows); i++) {
        RemoveWindow(&windows[i]);
    }
    Heap_Free(buf);
}
