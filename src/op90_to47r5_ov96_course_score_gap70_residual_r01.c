#pragma force_active on
#include "global.h"
#include "font.h"
#include "heap.h"
#include "system.h"
#include "gf_gfx_planes.h"
#include "filesystem.h"
#include "sprite.h"
#include "oam_manager.h"
#include "unk_0200FA24.h"
#include "wireless_status_icon.h"
#include "obj_char_transfer.h"
#include "pokeathlon/pokeathlon.h"

typedef struct Ov96R31Work Ov96R31Work;
typedef struct Ov96ResourceWork Ov96ResourceWork;
typedef struct Ov96EA854Work Ov96EA854Work;
typedef struct Ov96R31Rec Ov96R31Rec;
typedef struct Ov96Mini Ov96Mini;

typedef struct Ov96R31Config {
    s32 capacity;
    s32 resourceCount;
    u8 managerCounts[4];
} Ov96R31Config;

typedef struct Ov96ParticipantView {
    u16 species;
    u16 form;
    u16 special;
    u8 kind;
    u8 unk07;
    u32 participant;
    u32 unk0C;
} Ov96ParticipantView;

typedef struct PokeathlonParticipantMon {
    u16 species;
    u16 form;
    u8 unk04[36];
} PokeathlonParticipantMon;

Ov96R31Work *ov96_021EB180(enum HeapID heap, const Ov96R31Config *config);

/* Proposed selected-provider shared headers come from the immutable family-r2. */
typedef struct ScoreDisplay {
    u16 x,y,width,height;
    u8 visible;
    u8 reserved09[11];
    u16 paletteId,characterId;
    u8 index;
    u8 reserved19[7];
    void *module;
    u8 reserved24[64];
    u32 animation;
    u16 stage,reserved6a;
} ScoreDisplay;
typedef struct ScoreSettings { u32 values[3][2]; } ScoreSettings;
typedef struct ScoreCourseRecord { s32 rows[4][5]; } ScoreCourseRecord;
typedef struct ScoreCoordinates { u32 values[3]; } ScoreCoordinates;
typedef struct Ov96R75Row Ov96R75Row;
typedef struct Work {
    enum HeapID heap;
    BgConfig *bg;
    u8 pad08[0xd0];
    u8 *module;
    Ov96R75Row *other;
    ScoreDisplay display[3];
    Ov96R31Work *manager;
    u8 *cursor;
    u8 rest22c[0x174];
    Ov96ResourceWork *resource;
    Ov96EA854Work *loader;
    Ov96R31Rec *mainSprite;
    Sprite *sprites[6];
    u32 tail;
} Work;
extern const Ov96R31Config ov96_0221C404;
extern const ScoreCoordinates ov96_0221C410;
extern const u8 ov96_0221C3F4[8],ov96_0221C3EC[8];
void ov96_021F9E3C(void);
void ov96_021F9E5C(BgConfig *);
void ov96_021FB7C8(void *);
void *ov96_021FC028(u32),*ov96_021FC618(u32);
void *ov96_021FC0E4(u8 *),*ov96_021FC0E8(u8 *);
int ov96_021FC0F4(u8 *module);
void ov96_021E6670(u8 *,u32);
void ov96_021E92B0(ObjCharTransferTemplate *,int,enum HeapID,GXOBJVRamModeChar,GXOBJVRamModeChar);
void ov96_021EB5C8(void *,int,int,int,int);
void ov96_021EB29C(Ov96R31Work *,int,int);
void ov96_021EB2BC(Ov96R31Work *,NarcId,int,int,int);
void ov96_021EB2F4(Ov96R31Work *,NarcId,int,int,int,u8);
void ov96_021EB334(Ov96R31Work *,NarcId,int,int);
void ov96_021EB36C(Ov96R31Work *,NarcId,int,int);
void ov96_021EB3A4(Ov96R31Work *);
Ov96ResourceWork *ov96_021E9A78(int,int,int);
void *ov96_021EB5E8(u8 *);
void *ov96_021EA854(enum HeapID,int,int,void *,void *);
Sprite *ov96_021EB408(Ov96R31Work *,int,int,int,u8);
Ov96R31Rec *ov96_021EB3E4(Ov96R31Work *,int,int,int,u8);
void ov96_021EB564(Ov96R31Rec *,int);
void ov96_021EB588(Ov96R31Rec *,const void *);
void ov96_021EB52C(Ov96R31Rec *,BOOL,BOOL);
void ov96_021EB630(Ov96R31Rec *,int);
Sprite *ov96_021EA374(void *,SpriteList *,int,enum HeapID);
void ov96_021FC630(Ov96R75Row *,void *,int);
void *ov96_021FC188(int);
void ov96_021FC214(u8 *,int);
void ov96_021FC2B4(u8 *,BOOL);
BOOL ov96_021E5F24(PokeathlonCourseData *);
void ov96_021E6168(PokeathlonCourseData *,int,int,Ov96ParticipantView *);
void *ov96_021E60C0(u8 *,int,int);
int ov96_021E6108(const Ov96Mini *);
void ov96_021EA8A8(void *,int,Ov96ParticipantView *,u32 *,int,int);
BOOL ov96_021EAA00(void *);
void ov96_021F9FE8(BgConfig *,enum HeapID),ov96_021FA020(BgConfig *,enum HeapID);
void *ov96_021EAA04(u8 *,int);
void ov96_021EAB38(u8 *,int);
void ov96_021FC144(u8 *),ov96_021FC07C(u8 **,void *);
Sprite **ov96_021E6290(PokeathlonCourseData *,int,void *,void *);
void ov96_021FA0E8(PokeathlonCourseData *,const ScoreCourseRecord *,const ScoreSettings *,void *,void *,void *,u8,ScoreDisplay *);
void ov96_021E634C(PokeathlonCourseData *,int,void *,void *,u32,u8,const u16 (*)[2]);
void ov96_021FBBB4(PokeathlonCourseData *,const ScoreCourseRecord *);
u8 *ov96_021E8A20(void *);
BOOL ov96_021F94A8(PokeathlonCourseData *,int);

// @fn ov96_021F94A8
BOOL ov96_021F94A8(PokeathlonCourseData *course, int unused) {
    u32 options[17];
    ScoreCourseRecord record;
    ObjCharTransferTemplate gfx;
    Ov96R31Config templ;
    VecFx32 pos2;
    VecFx32 pos1;
    u16 names[12][2];
    Ov96ParticipantView pokemon[3];
    ScoreCoordinates numbers;
    u16 coords[3][2];
    ScoreSettings settings;
    Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i, j;
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        {
            void *a, *b;
            int c;
            Heap_Create((enum HeapID)92, HEAP_ID_138, 0x48000);
            Main_SetVBlankIntrCB(0, 0);
            Main_SetHBlankIntrCB(0, 0);
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            reg_GX_DISPCNT &= 0xffffe0ff;
            reg_GXS_DB_DISPCNT &= 0xffffe0ff;
            ov96_021F9E3C();
            w = PokeathlonCourse_AllocPtr4FromHeap(course, 0x3c8);
            MI_CpuFill8(w, 0, 0x3c8);
            w->bg = BgConfig_Alloc(HEAP_ID_138);
            w->module = ov96_021FC028(w->heap);
            w->other = ov96_021FC618(w->heap);
            a = ov96_021FC0E4(w->module);
            b = ov96_021FC0E8(w->module);
            c = ov96_021FC0F4(w->module);
            PokeathlonCourse_SetField3A4(course, (u32)a, (u32)b, (u32)c);
            ov96_021E6670((u8 *)course, 8);
            gfx.maxTasks = 107;
            gfx.sizeMain = 0x40000;
            gfx.sizeSub = 0x4000;
            gfx.heapID = HEAP_ID_138;
            ov96_021E92B0(&gfx, 18, HEAP_ID_138, (GXOBJVRamModeChar)0x300010, (GXOBJVRamModeChar)0x300010);
            NNS_G2dInitOamManagerModule();
            OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_138);
            w->heap = HEAP_ID_138;
            FontID_Alloc(4, HEAP_ID_138);
            ov96_021F9E5C(w->bg);
            ov96_021FB7C8(w);
            gSystem.screensFlipped = 1;
            GfGfx_SwapDisplay();
            PokeathlonCourse_IncrementField1ED(course);
            break;
        }
    case 1:
        templ = ov96_0221C404;
        w->manager = ov96_021EB180(w->heap, &templ);
        ov96_021EB5C8(w->manager, 0, 0x110000, 0, 0);
        ov96_021EB29C(w->manager, 0, 101);
        ov96_021EB29C(w->manager, 1, 102);
        ov96_021EB29C(w->manager, 2, 103);
        ov96_021EB29C(w->manager, 3, 104);
        ov96_021EB29C(w->manager, 4, 105);
        ov96_021EB29C(w->manager, 5, 106);
        ov96_021EB2BC(w->manager, (NarcId)156, 9, 101, 3);
        ov96_021EB2F4(w->manager, (NarcId)156, 6, 101, 3, 1);
        ov96_021EB334(w->manager, (NarcId)156, 8, 101);
        ov96_021EB36C(w->manager, (NarcId)156, 7, 101);
        ov96_021EB2BC(w->manager, (NarcId)156, 16, 102, 1);
        ov96_021EB2F4(w->manager, (NarcId)156, 13, 102, 1, 1);
        ov96_021EB334(w->manager, (NarcId)156, 15, 102);
        ov96_021EB36C(w->manager, (NarcId)156, 14, 102);
        ov96_021EB2BC(w->manager, (NarcId)156, 12, 103, 1);
        ov96_021EB2F4(w->manager, (NarcId)156, 6, 103, 1, 1);
        ov96_021EB334(w->manager, (NarcId)156, 11, 103);
        ov96_021EB36C(w->manager, (NarcId)156, 10, 103);
        ov96_021EB2BC(w->manager, (NarcId)156, 20, 105, 1);
        ov96_021EB2F4(w->manager, (NarcId)156, 17, 105, 1, 1);
        ov96_021EB334(w->manager, (NarcId)156, 19, 105);
        ov96_021EB36C(w->manager, (NarcId)156, 18, 105);
        ov96_021EB2BC(w->manager, (NarcId)156, 28, 104, 1);
        ov96_021EB2F4(w->manager, (NarcId)156, 25, 104, 1, 1);
        ov96_021EB334(w->manager, (NarcId)156, 27, 104);
        ov96_021EB36C(w->manager, (NarcId)156, 26, 104);
        ov96_021EB2BC(w->manager, (NarcId)156, 32, 106, 1);
        ov96_021EB2F4(w->manager, (NarcId)156, 29, 106, 1, 1);
        ov96_021EB334(w->manager, (NarcId)156, 31, 106);
        ov96_021EB36C(w->manager, (NarcId)156, 30, 106);
        ov96_021EB3A4(w->manager);
        w->resource = ov96_021E9A78((int)w->heap, 743, 1);
        w->loader = ov96_021EA854(w->heap, 3, 4, w->resource, ov96_021EB5E8((u8 *)w->manager));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        for (i = 0; i < 12; i++) {
            Sprite_SetDrawPriority(ov96_021EB408(w->manager, 1, 3, 101, 2), 27);
            Sprite_SetDrawPriority(ov96_021EB408(w->manager, 1, 1, 103, 6), 27);
        }
        for (i = 0; i < 3; i++) {
            ov96_021EB408(w->manager, 1, 1, 102, 5);
            ov96_021EB408(w->manager, 1, 1, 104, 7);
            ov96_021EB408(w->manager, 1, 1, 105, 8);
        }
        w->mainSprite = ov96_021EB3E4(w->manager, 1, 1, 106, 9);
        ov96_021EB564(w->mainSprite, 0);
        pos2.z = 0;
        pos2.x = 0xa0000;
        pos2.y = 0x1c8000;
        ov96_021EB588(w->mainSprite, &pos2);
        ov96_021EB52C(w->mainSprite, 1, 1);
        ov96_021EB630(w->mainSprite, 1);
        for (i = 0; i < 6; i++) {
            w->sprites[i] = ov96_021EA374(w->resource, ov96_021EB5E8((u8 *)w->manager), 0, w->heap);
            Sprite_SetDrawFlag(w->sprites[i], 1);
            pos1.x = ov96_0221C3F4[i] << 12;
            pos1.y = 0x1c8000;
            pos1.z = 0;
            Sprite_SetMatrix(w->sprites[i], &pos1);
            Sprite_SetAnimCtrlSeq(w->sprites[i], ov96_0221C3EC[i]);
        }
        ov96_021FC630(w->other, w->manager, 1);
        w->cursor = ov96_021FC188(w->heap);
        {
            u16 (*out)[2];
            i = 0;
            out = names;
            for (; i < 4; i++) {
                PokeathlonParticipantMon *in = (PokeathlonParticipantMon *) PokeathlonCourse_GetParticipantUnk04(course, i);
                for (j = 0; j < 3; j++) {
                    (*out)[0] = in->species;
                    (*out)[1] = in->form;
                    in++;
                    out++;
                }
            }
        }
        ov96_021FC214(w->cursor, (int)names);
        ov96_021FC2B4(w->cursor, 0);
        for (i = 0; i < 3; i++) {
            ov96_021E6168(course, ov96_021E5F24(course), i, &pokemon[i]);
            options[i + 5] = (u32)ov96_021E6108(ov96_021E60C0((u8 *)course, ov96_021E5F24(course), i));
        }
        options[0] = 0;
        options[1] = 1;
        options[2] = 0;
        options[3] = 1;
        options[4] = 1;
        ov96_021EA8A8(w->loader, 3, pokemon, options, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 3:
        if (ov96_021EAA00(w->loader)) {
            PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)w->bg);
            ov96_021F9FE8(w->bg, w->heap);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 4:
        ov96_021FA020(w->bg, w->heap);
        {
            reg_G2_BG0OFS = 0x1400000;
            reg_G2_BG1OFS = 0x1400000;
            reg_G2_BG2OFS = 0x1400000;
            reg_G2S_DB_BG0OFS = 0x300000;
            reg_G2S_DB_BG1OFS = 0x300000;
            reg_G2S_DB_BG2OFS = 0x300000;
        }
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
        GfGfx_EngineATogglePlanes(16, 1);
        GfGfx_EngineBTogglePlanes(16, 1);
        PokeathlonCourse_SetField1F4(course, 1);
        for (i = 0; i < 3; i++) ov96_021EAB38(ov96_021EAA04((u8 *)w->loader, (u8)i), 1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        if (ov96_021E5F24(course) == 0) ov96_021FC144(w->module);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 7:
        {
            u8 *copy;
            ov96_021FC07C((u8 **)w->module, w->manager);
            Sprite_SetDrawPriority(*ov96_021E6290(course, 272, w->resource, w->manager), 1);
            numbers = ov96_0221C410;
            ReadWholeNarcMemberByIdPair(&settings, (NarcId)170, 10);
            ReadWholeNarcMemberByIdPair(&record, (NarcId)170, 0);
            {
                int k;
                for (k = 0; k < 3; k++) {
                    ov96_021FA0E8(course, &record, &settings, w->module, w->manager, w->loader, (u8)k, &w->display[k]);
                    coords[k][0] = numbers.values[k];
                    coords[k][1] = 392;
                }
            }
            ov96_021E634C(course, 0, w->resource, w->manager, 1, 3, coords);
            if (ov96_021E5F24(course) == 0) ov96_021FBBB4(course, &record);
            copy = PokeathlonCourse_GetDataCopyArea(course);
            {
                u8 *p = ov96_021E8A20(copy + 240);
                u8 k;
                for (k = 0; k < 6; k++) p[28 + k] = 17;
            }
            if (ov96_021E5F24(course) == 0) {
                u8 *p = ov96_021E8A20(copy + 40);
                u8 k;
                for (k = 0; k < 6; k++) p[28 + k] = 17;
            }
            WirelessStatusIcon_CreateOnScreenIfConnected(1);
            BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, w->heap);
            PokeathlonCourse_IncrementField1ED(course);
            break;
        }
    case 8:
        if (IsPaletteFadeFinished()) return 1;
        break;
    }
    return 0;
}
