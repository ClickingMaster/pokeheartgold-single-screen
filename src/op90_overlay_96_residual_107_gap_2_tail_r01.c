#include "global.h"
#pragma force_active on
#include "font.h"
#include "heap.h"
#include "system.h"
#include "gf_gfx_planes.h"
#include "filesystem.h"
#include "sprite.h"
#include "oam_manager.h"
#include "unk_0200FA24.h"
#include "unk_0203A3B0.h"
#include "obj_char_transfer.h"
#include "pokeathlon/pokeathlon.h"
#include "bg_window.h"
#include "msgdata.h"
#include "message_format.h"
typedef struct PokeathlonCourseSpriteEntry {
    u32 active;
    void *a;
    struct Sprite *sprite;
    struct SysTask *task;
} PokeathlonCourseSpriteEntry;

typedef struct Ov96Mini {
    u8 fields0[6];
    u8 type6,field7,type8;
    u8 fields9[11];
} Ov96Mini;
typedef struct Ov96ParticipantView { u16 species; u16 field2; u16 special; u8 kind; u8 field7; u32 participant; u32 fieldC; } Ov96ParticipantView;

typedef struct Ov96Actor {
    void *object;
    void *sprite1;
    void *sprite2;
    u8 pad0C[4];
    s32 offsetY;
    s32 offsetX;
    s32 halfHeight;
    s32 radius;
    s32 x;
    s32 y;
    s32 center;
    s32 bottom;
    s32 height;
    u32 state;
    u32 anim;
    s32 priority;
    u32 hideSecondary;
} Ov96Actor;




typedef struct Ov96R31Rec {
    u16 active;
    u16 shown;
    struct Sprite *sprite;
    u16 kind;
    u16 id;
} Ov96R31Rec;

typedef struct Ov96R31Config {
    s32 capacity;
    s32 resourceCount;
    u8 managerCounts[4];
} Ov96R31Config;

typedef struct Ov96R31Work {
    u32 heap;
    s32 used;
    Ov96R31Config config;
    void *spriteList;
    G2dRenderer renderer;
    void *resources[4];
    u8 *resourceRecords;
    Ov96R31Rec *records;
} Ov96R31Work;

Ov96R31Work *ov96_021EB180(enum HeapID heap, const Ov96R31Config *config);
extern const Ov96R31Config ov96_0221D238;
extern const Ov96R31Config ov96_0221C2A8;




typedef struct Ov96R110Group {
    u32 unk0;
    void *order0;
    u32 unk8;
    void *order1;
    u32 unk10;
    void *order2;
    void *entries[3];
} Ov96R110Group;

struct Ov96Pair;
typedef struct Ov96R110Arena {
    Ov96R110Group groups[4];
    struct Ov96Pair *pairs;
    u32 field94;
    u8 control[8];
} Ov96R110Arena;
Ov96R110Arena *ov96_02214A24(u32 heapId, s32 count, int mode);
void ov96_02214B74(Ov96R110Arena *, struct Ov96Pair *);
void ov96_02214A6C(Ov96R110Group *groups, s32 index, void *value);
void ov96_02214A9C(Ov96R110Group *groups);






typedef struct Ov96Pair {
    void *one;
    void *two;
    VecFx32 position;
    VecFx32 a, b;
    u8 flags[12];
    u8 f38,f39,f3a,f3b,f3c,f3d,f3e,f3f,f40,f41,f42,f43,f44,f45;
    u16 f46;
    u8 trailing[4];
} Ov96Pair;

typedef struct R27Vec { s32 x, y, z; } R27Vec;
typedef struct R27Work {
    enum HeapID heap;
    void *course;
    struct Sprite *sprites[2];
    BgConfig *bg;
    Window windowA, windowB;
    MsgData *msgData;
    MessageFormat *format;
    NNSG2dScreenData *screenData;
    void *screenAlloc;
    u8 pad044[8];
    void *label;
} R27Work;

typedef struct Ov96ResourceWork { int heap,mask,param; void *man[4]; u8 rest[0x98]; } Ov96ResourceWork;

typedef struct Ov96EA854Work {
    u32 unk00;
    int unk04;
    int unk08;
    u32 unk0C;
    void *unk10;
    void *unk14;
    void *unk18;
    Ov96Actor entries[12];
} Ov96EA854Work;

typedef struct Ov96RunnerWindowData { WindowTemplate window; u16 palettes[4]; } Ov96RunnerWindowData;
extern const Ov96RunnerWindowData ov96_0221D1F8;
typedef struct Ov96D1F4Block { u8 table[4]; Ov96RunnerWindowData data; } Ov96D1F4Block;
extern const Ov96D1F4Block ov96_0221D1F4;



typedef struct Runner { Ov96Actor *actor; Ov96R31Rec *marker,*marks[3]; s32 startX,startY; u32 unused1c; s32 height; s32 nowX,nowY; u32 unused2c; s32 homeX,homeY; u8 unused38[36]; u16 mode; u16 unused5e; u32 unused60; s32 stat64,stat68; u16 stat6c,stat6e; u8 stat70,stat71,stat72,stat73,stat74,stat75,unused76[2]; u32 counter; } Runner;
typedef Ov96Pair Pair;
typedef struct State { Ov96R31Work *manager; BgConfig *background; Window mainWindow,windows[4]; enum HeapID heap; Runner runners[4][3]; Pair pairs[2]; Ov96R31Rec *column[4],*other[4],*label[4]; u8 unused6f4[64]; u8 levels[4]; int event; u8 unused73c[8]; Ov96ResourceWork *resources; Ov96EA854Work *actors; Ov96R110Arena *remote; R27Work *ui; VecFx32 vectors[8]; u32 field7b4; String *strings[7]; u8 panels[4][16]; u8 unused814[8]; void *extra; } State;
typedef Ov96ParticipantView Creature;
typedef struct Setup { u32 leading[5]; int entries[12]; } Setup;
typedef struct Dimension { u32 width,height; } Dimension;
typedef struct Vec4 { VecFx32 v[4]; } Vec4;
typedef struct WorkA { struct { int outY,outX; u16 palettes[4]; } temporary; VecFx32 position,offset,normal; } WorkA;
typedef struct WorkB { u16 coords[3][2]; VecFx32 positions[4],v; Ov96R31Config initial; ObjCharTransferTemplate config; s32 data[30]; Setup setup; Creature records[12]; } WorkB;
typedef struct Work { struct { int outY,outX; u16 palettes[4]; } temporary; VecFx32 position,offset,normal; Dimension dimensions[3]; u16 coords[3][2]; VecFx32 positions[4],v; Ov96R31Config initial; ObjCharTransferTemplate config; s32 data[30]; Setup setup; Creature records[12]; } Work;
extern const VecFx32 ov96_0221D220,ov96_0221D250,ov96_0221D208;
extern const VecFx32 ov96_0221D3D8[4],ov96_0221D4B4[8],ov96_0221D3A8[4];
extern const u16 ov96_0221D408[4][3][2];
void *ov96_021E60C0(u8 *,int,int);
int ov96_021E6108(const Ov96Mini *),ov96_021E6138(const Ov96Mini *),ov96_021E6104(void);
void ov96_021E6168(PokeathlonCourseData *,int,int,Ov96ParticipantView *);
BOOL ov96_021E5F24(PokeathlonCourseData *);
Sprite **ov96_021E6290(PokeathlonCourseData *,int,void *,void *);
void ov96_021E634C(PokeathlonCourseData *,int,void *,void *,u32,u8,const u16 (*)[2]);
Sprite *ov96_021E64F8(PokeathlonCourseData *,void *,void *,SpriteList *,int);
void ov96_021E64B8(PokeathlonCourseData *),ov96_021E6670(u8 *,u32);
u8 *ov96_021E8A20(void *);
u8 ov96_021E90FC(u8 *);
void ov96_021E92B0(ObjCharTransferTemplate *,int,enum HeapID,GXOBJVRamModeChar,GXOBJVRamModeChar);
Ov96ResourceWork *ov96_021E9A78(int,int,int);
void *ov96_021EA854(enum HeapID,int,int,void *,void *);
void ov96_021EA8A8(void *,int,Ov96ParticipantView *,Setup *,int,int);
BOOL ov96_021EAA00(void *);
void *ov96_021EAA04(u8 *,int),*ov96_021EAA20(void **);
void ov96_021EAB38(u8 *,int),ov96_021EABA8(Ov96Actor *,int),ov96_021EABDC(u8 *,u32),ov96_021EAC0C(Ov96Actor *,u32),ov96_021EAF6C(u8 *,u32),ov96_021EAF70(u8 *,u32,u32);
s32 ov96_021EAF8C(void *);
void ov96_021EAF94(Ov96Actor *,s32,s32),ov96_021EB0A4(Ov96Actor *,s32,s32,s32 *,s32 *);
void ov96_021EB29C(Ov96R31Work *,int,int),ov96_021EB2BC(Ov96R31Work *,NarcId,int,int,int),ov96_021EB2F4(Ov96R31Work *,NarcId,int,int,int,u8),ov96_021EB334(Ov96R31Work *,NarcId,int,int),ov96_021EB36C(Ov96R31Work *,NarcId,int,int),ov96_021EB3A4(Ov96R31Work *);
Ov96R31Rec *ov96_021EB3E4(Ov96R31Work *,int,int,int,u8);
void ov96_021EB52C(Ov96R31Rec *,BOOL,BOOL),ov96_021EB564(Ov96R31Rec *,int),ov96_021EB588(Ov96R31Rec *,const void *),ov96_021EB5AC(Ov96R31Rec *,u16,int),ov96_021EB630(Ov96R31Rec *,int);
void ov96_021EB5C8(void *,int,int,int,int);
void *ov96_021EB5E8(u8 *);
void ov96_022117CC(void),ov96_022118C4(void *),ov96_02211A24(void *),ov96_02211DE4(void *,void *);
void ov96_02213444(Ov96Pair *),ov96_0221359C(void *,s32 *,u8,u8,u8 *),ov96_0221362C(u8 *),ov96_022140F4(void *),ov96_02214044(u8 *,u32,u32),ov96_022141F8(u8 *);
R27Work *ov96_0221464C(enum HeapID,BgConfig *,PokeathlonCourseData *);
void ov96_02214718(void *,R27Work *,void *),ov96_022147FC(R27Work *),ov96_0221490C(void *,s32);
int ov96_02210C98(void *);
static inline Runner *RunnerAt40958(State *state,int index) { return &state->runners[index/3][index%3]; }
static inline void ConfigureRunnerModel(PokeathlonCourseData *course,Ov96EA854Work *actors,s32 index,const Dimension *dimensions) { Ov96Actor *actor; int row,col,category; actor=ov96_021EAA04((u8*)actors,(u8)index);ov96_021EAB38((u8*)actor,1);row=index/3;col=index%3;category=ov96_021E6138(ov96_021E60C0((u8*)course,row,col));ov96_021EAF70((u8*)actor,dimensions[category-1].width,dimensions[category-1].height); }
// @fn ov96_02210C98
int ov96_02210C98(void*data)
{
    Dimension dimensions [ 3 ];
    Ov96R31Rec * a;
    int j;
    u8 * image;
    int col4;
    int i;
    u16 palettes[4];
    struct { int outY, outX; } temporary;
    struct { VecFx32 position, offset, normal; } vecs;
    int local;
    int inputX;
    u32 inputY;
    int group;
    Runner(*rows)[3];
    WorkB wb;
    State * t;
    const VecFx32 * position;
    s32 reflectionFactor;
    u16 * palette;
    int mode;
    u32 k;
    const VecFx32 * mark;
    const VecFx32 * other;
    int row4;
    State * s = PokeathlonCourse_GetHeapAllocPtr4(data);
    switch (PokeathlonCourse_GetField1ED(data))
    {
        case 0:
        {
            u8 i0;
            Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_147, 0x50000);
            Main_SetVBlankIntrCB(0, 0);
            Main_SetHBlankIntrCB(0, 0);
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            *(vu32 *)0x04000000 &= 0xffffe0ff;
            *(vu32 *)0x04001000 &= 0xffffe0ff;
            ov96_022117CC();
            s = PokeathlonCourse_AllocPtr4FromHeap(data, 0x820);
            MI_CpuFill8(s, 0, 0x820);
            s->extra = Heap_Alloc(HEAP_ID_147, 40);
            MI_CpuFill8(s->extra, 0, 40);
            s->background = BgConfig_Alloc(HEAP_ID_147);
            ov96_021E6670((u8 *)data, 8);
            wb.config.maxTasks = 133;
            wb.config.sizeMain = 0x40000;
            wb.config.sizeSub = 0x4000;
            wb.config.heapID = HEAP_ID_147;
            ov96_021E92B0(&wb.config, 16, HEAP_ID_147, (GXOBJVRamModeChar)0x300010, (GXOBJVRamModeChar)16);
            NNS_G2dInitOamManagerModule();
            OamManager_Create(0, 126, 0, 32, 1, 126, 0, 32, HEAP_ID_147);
            s->heap = HEAP_ID_147;
            FontID_Alloc((FontID)4, HEAP_ID_147);
            ov96_022118C4(s->background);
            ov96_0221362C((u8 *)s);
            ov96_022140F4(data);
            for (i0 = 0; i0 < 4; i0++)
            {
                ov96_02214044((u8 *)s, i0, 0);
            }
            ov96_022141F8((u8 *)s);
            gSystem.screensFlipped = 1;
            GfGfx_SwapDisplay();
            PokeathlonCourse_IncrementField1ED(data);
            break;
        }
        case 1:
        {
            int count;
            s->ui = ov96_0221464C(s->heap, s->background, data);
            count = PokeathlonCourse_GetParticipantCount(data);
            mode = PokeathlonCourse_GetMode(data);
            s->remote = ov96_02214A24((u32)s->heap, 4 - count, mode);
            s->resources = ov96_021E9A78(s->heap, 2735, 1);
            ov96_021E64B8(data);
            PokeathlonCourse_IncrementField1ED(data);
            break;
        }
        case 2:
        {
            wb.initial = ov96_0221D238;
            s->manager = ov96_021EB180(s->heap, &wb.initial);
            ov96_021EB5C8(s->manager, 0, 0, 0, 0x200000);
            s->actors = ov96_021EA854(s->heap, 12, 8, s->resources, ov96_021EB5E8((u8 *)s->manager));
            ov96_021EB29C(s->manager, 0, 101);
            ov96_021EB29C(s->manager, 1, 102);
            ov96_021EB2BC(s->manager, (NarcId)236, 18, 101, 1);
            ov96_021EB2F4(s->manager, (NarcId)236, 15, 101, 1, 2);
            ov96_021EB334(s->manager, (NarcId)236, 17, 101);
            ov96_021EB36C(s->manager, (NarcId)236, 16, 101);
            ov96_021EB2BC(s->manager, (NarcId)236, 14, 102, 2);
            ov96_021EB2F4(s->manager, (NarcId)236, 11, 102, 2, 1);
            ov96_021EB334(s->manager, (NarcId)236, 13, 102);
            ov96_021EB36C(s->manager, (NarcId)236, 12, 102);
            ov96_021EB3A4(s->manager);
            for (i = 0; i < 12; i++)
            {
                s->runners[i / 3][i % 3].marker = ov96_021EB3E4(s->manager, 1, 1, 101, 2);
                ov96_021EB630(s->runners[i / 3][i % 3].marker, 5);
                for (j = 0; j < 3; j++)
                {
                    s->runners[i / 3][i % 3].marks[j] = ov96_021EB3E4(s->manager, 1, 1, 101, 7);
                    ov96_021EB630(s->runners[i / 3][i % 3].marks[j], 47);
                }
            }
            for (i = 0; i < 2; i++)
            {
                Pair * p = &s->pairs[i];
                memset(&wb.v, 0, sizeof( VecFx32 ));
                p->one = ov96_021EB3E4(s->manager, 1, 1, 101, 3);
                p->two = ov96_021EB3E4(s->manager, 1, 1, 101, 8);
                ov96_021EB564(p->one, 10);
                ov96_021EB564(p->two, 13);
                ov96_021EB630(p->one, 46);
                ov96_021EB630(p->two, 45);
                ov96_02213444(p);
                p->f39 = 0;
                wb.v.x = 0x80000;
                wb.v.y = 0x68000;
                ov96_021EB588(p->one, &wb.v);
                ov96_021EB588(p->two, &wb.v);
                p->position = wb.v;
                p->f42 = 7;
                if (i == 0)
                {
                    ov96_021EB52C(p->one, 1, 1);
                    p->f38 = 1;
                }
            }
            palettes[0] = ov96_0221D1F4.data.palettes[0];
            palettes[1] = ov96_0221D1F4.data.palettes[1];
            palettes[2] = ov96_0221D1F4.data.palettes[2];
            palettes[3] = ov96_0221D1F4.data.palettes[3];
            *(Vec4 *)wb.positions = *(const Vec4 *)ov96_0221D3D8;
            {
                int ci;
            for (ci = 0, t = s, position = wb.positions, palette = palettes, k = 2, mark = ov96_0221D4B4, other = ov96_0221D3A8; ci < 4; t = (State *)((u32 *)t + 1), position++, palette++, k += 2, mark += 2, other++, ci++)
            {
                t->column[0] = ov96_021EB3E4(s->manager, 1, 1, 101, 5);
                ov96_021EB52C(t->column[0], 1, 1);
                ov96_021EB564(t->column[0], 1);
                ov96_021EB588(t->column[0], position);
                ov96_021EB5AC(t->column[0], *palette, 1);
                ov96_021EB630(t->column[0], 3);
                for (j = 0; j < 2; j++)
                {
                    a = ov96_021EB3E4(s->manager, 1, 1, 101, 6);
                    ov96_021EB52C(a, 1, 1);
                    ov96_021EB564(a, j + k);
                    ov96_021EB588(a, mark + j);
                    ov96_021EB630(a, 103);
                }
                t->other[0] = ov96_021EB3E4(s->manager, 1, 1, 101, 11);
                ov96_021EB588(t->other[0], other);
                ov96_021EB630(t->other[0], 2);
                t->label[0] = ov96_021EB3E4(s->manager, 1, 1, 101, 12);
                ov96_021EB588(t->label[0], position);
                ov96_021EB630(t->label[0], 102);
            }
            }
            Sprite_SetDrawPriority(*ov96_021E6290(data, 0, s->resources, s->manager), 1);
            ov96_02214718(s->manager, s->ui, s->resources);
            PokeathlonCourse_IncrementField1ED(data);
            break;
        }
        case 3:
        {
            int col3;
            u32 row3;
            Setup * pSetup;
            pSetup = &wb.setup;
            for (i = 0; i < 12; i++)
            {
                col3 = i % 3;
                row3 = i / 3;
                ov96_021E6168(data, row3, col3, &wb.records[i]);
                pSetup->entries[i] = ov96_021E6108(ov96_021E60C0((u8 *)data, row3, col3));
            }
            wb.setup.leading[0] = 0;
            wb.setup.leading[1] = 1;
            wb.setup.leading[2] = 0;
            wb.setup.leading[3] = 1;
            wb.setup.leading[4] = 1;
            ov96_021EA8A8(s->actors, 12, wb.records, &wb.setup, 0, 0);
            PokeathlonCourse_IncrementField1ED(data);
            break;
        }
        case 4:
        {
            Ov96Actor * actor;
            Runner * r;
            if (!ov96_021EAA00(s->actors))
            {
                break;
            }
            local = ov96_021E5F24(data);
            PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)s->background);
            PokeathlonCourse_SetField1F4(data, 1);
            ReadWholeNarcMemberByIdPair(dimensions, (NarcId)170, 11);
            for (i = 0; i < 12; i++)
            {
                ConfigureRunnerModel(data, s->actors, i, dimensions);
                actor = ov96_021EAA04((u8 *)s->actors, (u8)i);
                image = ov96_021EAA20(&actor->object);
                col4 = i % 3;
                row4 = i / 3;
                r = &s->runners[row4][col4];
                r->actor = actor;
                r->height = ov96_021EAF8C(actor);
                r->counter = 0;
                ov96_021E90FC(image);
                inputX = ov96_0221D408[row4][col4][0];
                inputY = ov96_0221D408[row4][col4][1];
                r->mode = 2;
                ov96_021EAC0C(actor, 2);
                ov96_021EAF94(actor, inputX, inputY);
                ov96_021EAF6C((u8 *)actor, ov96_021E6104());
                ov96_021EB0A4(actor, inputX, inputY, &temporary.outX, &temporary.outY);
                r->startX = (s32)((u32)temporary.outX << 12);
                r->startY = (s32)((u32)temporary.outY << 12);
                r->homeX = (s32)((u32)temporary.outX << 12);
                r->homeY = (s32)((u32)temporary.outY << 12);
                r->nowX = (s32)((u32)temporary.outX << 12);
                r->nowY = (s32)((u32)temporary.outY << 12);
                if (row4 == ov96_021E5F24(data))
                {
                    ov96_021EABA8(actor, 8);
                }
                else
                {
                    ov96_021EABA8(actor, 16);
                }
                ov96_021EABDC((u8 *)actor, 20);
                if (row4 == local)
                {
                    wb.coords[col4][0] = temporary.outX;
                    wb.coords[col4][1] = temporary.outY;
                    ov96_021E64F8(data, actor, s->resources, ov96_021EB5E8((u8 *)s->manager), 1);
                }
            }
            ov96_021E634C(data, 0, s->resources, s->manager, 1, 3, wb.coords);
            if (ov96_021E5F24(data) == 0)
            {
                ReadWholeNarcMemberByIdPair(wb.data, (NarcId)170, 8);
                for (group = 0, rows = s->runners; group < 4; rows++, group++)
                {
                    s->levels[group] = 12;
                    for (j = 0; j < 3; j++)
                    {
                        Runner * r = &(*rows)[j];
                        ov96_0221359C(data, wb.data, (u8)group, (u8)j, (u8 *)r);
                        ov96_02214A6C(s->remote->groups, (u8)(j + group * 3), r);
                    }
                }
                ov96_02214B74(s->remote, s->pairs);
                ov96_02214A9C(s->remote->groups);
                reflectionFactor = -1;
                vecs.normal = ov96_0221D220;
                VEC_Normalize(&vecs.normal, &vecs.normal);
                vecs.offset = ov96_0221D250;
                s->vectors[0] = vecs.offset;
                s->vectors[1] = vecs.offset;
                s->vectors[1].x *= reflectionFactor;
                vecs.position = ov96_0221D208;
                s->vectors[2] = vecs.position;
                s->vectors[3] = vecs.position;
                s->vectors[3].y *= reflectionFactor;
                s->vectors[4] = vecs.normal;
                s->vectors[5] = vecs.normal;
                s->vectors[5].x *= reflectionFactor;
                s->vectors[6] = vecs.normal;
                s->vectors[6].y *= reflectionFactor;
                s->vectors[7] = vecs.normal;
                s->vectors[7].x *= reflectionFactor;
                s->vectors[7].y *= reflectionFactor;
            }
            if (ov96_021E5F24(data) == 0)
            {
                ov96_02211DE4(s, ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(data) + 40));
            }
            ov96_02211A24(s);
            ov96_022147FC(s->ui);
            GfGfx_EngineATogglePlanes(16, 1);
            GfGfx_EngineBTogglePlanes(16, 1);
            s->event = 2700;
            ov96_0221490C(s->ui, s->event);
            sub_0203A994(2);
            BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, s->heap);
            PokeathlonCourse_IncrementField1ED(data);
            break;
        }
        case 5:
        if (IsPaletteFadeFinished())
        {
            return 1;
        }
        break;
    }
    return 0;
}
