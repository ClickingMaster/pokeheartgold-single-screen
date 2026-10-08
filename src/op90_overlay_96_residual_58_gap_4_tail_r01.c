#pragma force_active on
#include "global.h"
#include "system.h"
#include "sprite.h"
#include "pokeathlon/pokeathlon.h"

typedef struct State {
    int velocity;
    int unk04;
    int height;
    int unk0C;
    int active;
    int enabled;
    int oldVelocity;
} State;

typedef struct Mon {
    void *sprite;
    u32 unk04;
    int fixedX;
    int fixedY;
    u8 unk10[8];
    int unk18;
    int x;
    int y;
    u16 unk24;
    u8 unk26;
    u8 unk27;
    u8 flag;
    u8 hit[3];
    int unk2C;
    int speed;
    int scale;
} Mon;

typedef struct Work {
    void *bg;
    u8 unk04[0x50];
    enum HeapID heap;
    u8 unk58[8];
    void *scene;
    u8 unk64[0x28];
    void *obj;
    Mon mons[3];
    void *rival;
    int timer;
    u8 count;
    u8 unk141[0x47];
    void *shared;
    void *manager;
    u8 unk190[0x10];
    int table[7][128];
    int index[3];
    State states[3];
    void *extra;
} Work;

typedef struct Config {
    int data[3];
} Config;

typedef struct SpriteConfig {
    int narc;
    int sizeMain;
    int sizeSub;
    int heap;
} SpriteConfig;

typedef struct IconInfo {
    int data[17];
} IconInfo;

typedef struct Lookup {
    u32 acceleration[5];
    u32 velocity[5];
    u32 scale[5];
} Lookup;

typedef struct Pic {
    u32 data[4];
} Pic;

typedef struct Pair {
    int x;
    int y;
} Pair;

typedef struct Coord {
    u16 x;
    u16 y;
} Coord;

extern Config ov96_0221C0B8;

void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void ov96_021F5630(void);
void *BgConfig_Alloc(int heapID);
void ov96_021E6670(void *course, int value);
void ov96_021E92B0(SpriteConfig *config, int a, int heapID, int b, int c);
void OamManager_Create(int a, int b, int c, int d, int e, int f, int g, int h, int heapID);
void FontID_Alloc(int id, int heapID);
void ov96_021F584C(void *bg);
void ov96_021F6138(Work *work);
void GfGfx_SwapDisplay(void);
void GF_CreateVramTransferManager(int count, int heapID);
void *sub_02020654(int count, int heapID);
void *ov96_021F74A4(int heapID);
void *ov96_021F7684(u32 heapID, int count, u32 mode, u32 data, void *course);
void *ov96_021EB180(int heapID, Config *config);
void ov96_021EB5C8(void *scene, int a, int b, int c, int d);
void *ov96_021EB5E8(void *scene);
void *ov96_021EA854(enum HeapID heapID, void *a, void *b, void *c, void *d);
void ov96_021EB29C(void *scene, int index, int value);
void ov96_021F6C18(void *scene);
void ov96_021EB3A4(void *scene);
void ov96_021E6168(void *course, int participant, int member, Pic *pic);
void *ov96_021E60C0(void *course, int participant, int member);
int ov96_021E6108(void *mon);
void ov96_021EA8A8(void *manager, int count, Pic *pics, IconInfo *info, int a, int b);
int ov96_021EAA00(void *manager);
void *ov96_021E9A78(int heapID, int id, int count);
void *ov96_021E6290(void *course, int a, int b, void *scene);
void *ov96_021EAA04(void *manager, u8 index);
void ov96_021EAB38(void *sprite, int value);
int ov96_021E6138(void *mon);
void ov96_021EAF70(void *sprite, int x, int y);
void ov96_021EAC0C(void *sprite, int anim);
void ov96_021EAF94(void *sprite, int x, int y);
int ov96_021E6104(void);
void ov96_021EAF6C(void *sprite, int value);
void ov96_021EB0A4(void *sprite, int x, int y, int *outX, int *outY);
void ov96_021E634C(void *course, int a, int b, void *scene, u32 c, u8 d, u32 e);
void ov96_021F5980(Work *work);
void ov96_021F6C5C(Work *work, void *scene);
void ov96_021F7050(Work *work);
void ov96_021F6DA4(Work *work, u8 selected);
void GfGfx_EngineATogglePlanes(int planes, int enable);
void GfGfx_EngineBTogglePlanes(int planes, int enable);
void ov96_021F715C(void *course, Lookup *lookup, void *index, Mon *mon);
void ov96_021F6F3C(Work *work, u16 timer);
void ov96_021F6F80(void *course, Work *work);
void sub_0203A994(int value);
void ov96_021F6E38(Work *work);
int ov96_021F5018(void *course);

// @fn ov96_021F5018
int ov96_021F5018(void *course) {
    IconInfo info;
    SpriteConfig spriteConfig;
    Config config;
    Pic pics[3];
    Pair pairs[3];
    Coord coords[3];
    Lookup lookup;
    int outX, outY;
    Work *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i, mode, count, spriteIndex, x, setupIndex, x2, clearIndex;
    u8 participant;
    void *sprite;

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create((enum HeapID)92, (enum HeapID)143, 0x50000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(GX_PLANEMASK_NONE);
        GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
        ov96_021F5630();
        work = PokeathlonCourse_AllocPtr4FromHeap(course, 0x1004);
        MI_CpuFill8(work, 0, 0x1004);
        work->bg = BgConfig_Alloc(143);
        ov96_021E6670(course, 8);
        spriteConfig.narc = 213;
        spriteConfig.sizeMain = 0x40000;
        spriteConfig.sizeSub = 0x4000;
        spriteConfig.heap = PokeathlonCourse_GetHeapID(course);
        ov96_021E92B0(&spriteConfig, 15, 143, 0x300010, 0x300010);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, 143);
        work->heap = (enum HeapID)143;
        FontID_Alloc(4, work->heap);
        ov96_021F584C(work->bg);
        ov96_021F6138(work);
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        GF_CreateVramTransferManager(12, work->heap);
        work->extra = sub_02020654(12, work->heap);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        work->obj = ov96_021F74A4(work->heap);
        count = PokeathlonCourse_GetParticipantCount(course);
        mode = PokeathlonCourse_GetMode(course);
        work->rival = ov96_021F7684(work->heap, 4 - count, mode, (u32)PokeathlonCourse_GetDataCopyArea(course), course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        config = ov96_0221C0B8;
        work->scene = ov96_021EB180(work->heap, &config);
        ov96_021EB5C8(work->scene, 0, 0xC0000, 0, 0);
        work->manager = ov96_021EA854(work->heap, (void *)3, (void *)10, NULL, ov96_021EB5E8(work->scene));
        ov96_021EB29C(work->scene, 0, 101);
        ov96_021F6C18(work->scene);
        ov96_021EB3A4(work->scene);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        participant = ov96_021E5F24(course);
        for (i = 0; i < 3; i++) {
            ov96_021E6168(course, participant, i, &pics[i]);
            info.data[5 + i] = ov96_021E6108(ov96_021E60C0(course, participant, i));
        }
        info.data[0] = 0;
        info.data[1] = 1;
        info.data[2] = 1;
        info.data[3] = 3;
        info.data[4] = 3;
        ov96_021EA8A8(work->manager, 3, pics, &info, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 4:
        if (!ov96_021EAA00(work->manager)) {
            break;
        }
        work->shared = ov96_021E9A78(work->heap, 929, 1);
        {
            Sprite **spriteSlot = ov96_021E6290(course, 192, (int)work->shared, work->scene);
            Sprite_SetDrawPriority(*spriteSlot, 1);
        }
        PokeathlonCourse_SetVBlankIntrCB(work->bg);
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(pairs, (NarcId)170, 16);
        for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
            Mon *mon = &work->mons[spriteIndex];

            x = 64 + spriteIndex * 64;
            sprite = ov96_021EAA04(work->manager, spriteIndex);
            ov96_021EAB38(sprite, 1);
            mon->sprite = sprite;
            count = ov96_021E6138(ov96_021E60C0(course, 0, spriteIndex));
            ov96_021EAF70(sprite, pairs[count - 1].x, pairs[count - 1].y);
            ov96_021EAC0C(sprite, 2);
            ov96_021EAF94(sprite, x, 288);
            ov96_021EAF6C(sprite, ov96_021E6104());
            mon->fixedX = (u32)x << 12;
            mon->fixedY = 288 << 12;
            mon->x = (u32)x << 12;
            ov96_021EB0A4(sprite, x, 288, &outX, &outY);
            coords[spriteIndex].x = outX;
            coords[spriteIndex].y = outY;
        }
        ov96_021E634C(course, 0, (int)work->shared, work->scene, 1, 3, (u32)coords);
        work->count = 3;
        ov96_021F5980(work);
        ov96_021F6C5C(work, work->scene);
        ov96_021F7050(work);
        ov96_021F6DA4(work, ov96_021E5F24(course));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        GfGfx_EngineATogglePlanes(1, 1);
        GfGfx_EngineATogglePlanes(16, 1);
        GfGfx_EngineBTogglePlanes(16, 1);
        ReadWholeNarcMemberByIdPair(&lookup, (NarcId)170, 6);
        for (i = 0; i < 3; i++) {
            ov96_021F715C(course, &lookup, (void *)(u8)i, &work->mons[i]);
        }
        ov96_021E5F24(course);
        work->timer = 1800;
        ov96_021F6F3C(work, work->timer);
        ov96_021F6F80(course, work);
        for (clearIndex = 0; clearIndex < 128; clearIndex++) {
            work->table[0][clearIndex] = 0;
            work->table[1][clearIndex] = 0;
            work->table[2][clearIndex] = 0;
            work->table[3][clearIndex] = 0;
            work->table[4][clearIndex] = 0;
            work->table[5][clearIndex] = 0;
            work->table[6][clearIndex] = 0;
        }
        for (setupIndex = 0; setupIndex < 3; setupIndex++) {
            work->states[setupIndex].height = 0;
            x2 = setupIndex * 64 + 64;
            work->index[setupIndex] = x2 / 2;
            work->states[setupIndex].unk04 = 4096.0 * ((float)x2 / 64.0 - 2.0);
            work->mons[setupIndex].flag = TRUE;
        }
        sub_0203A994(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        ov96_021F6E38(work);
        return 1;
    }
    return 0;
}
