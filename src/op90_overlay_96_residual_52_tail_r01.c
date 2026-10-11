#pragma force_active on
#include "global.h"
typedef struct {u32 a,b,c;} Triple;
typedef struct {u32 a,b,c,d;} Gfx;
typedef struct {u32 a,b,c,d,e;void*data[12];} Options;
typedef struct {u16 a,b;} Pair;
typedef struct {u16 species,field2,special;u8 kind,field7;u32 participant,fieldC;} Pokemon;
typedef struct {u16 a,b;u8 rest[36];} NameEntry;
typedef struct {u8 bytes[108];} Display;
typedef struct {void*render,*a,*b;s32 x,y,pad14;u32 active;s32 x1,y1,pad24,x2,y2;u8 pad30[16];u8 flag;u8 pad41[7];u16 palette[16],gray[16];u16 p88,id,p8c;u8 p8e,p8f;} Participant;
typedef struct {u8 prefix[32];Participant p;} PartView;
typedef struct {void*bg;u8 pad[16];u32 heap;void*manager,*sprite;Participant participants[4][3];u8 pad6e0[64];u8 counts[4];u8 pad724[8];void*cursor;u16 current,next;u8 pad734[0x34];void*resource,*loader,*rank,*score;u8 pad778[0x54];void*cursorCopy;u32 pad7d0,remaining;u8 pad7d8[0x18];void*extra;} Work;
typedef struct {s32 x,y;} Coord;
typedef struct {u8 pad[0x24];void*a,*b;u8 rest[0x90-0x2c];} Slot;

extern u8 gSystem[];extern Triple ov96_0221BC70;extern Pair ov96_0221BDD4[4][3];extern u8 ov96_0221C3F4[],ov96_0221C3EC[];
Work*PokeathlonCourse_GetHeapAllocPtr4(void*);u32 PokeathlonCourse_GetField1ED(void*);void*PokeathlonCourse_AllocPtr4FromHeap(void*,u32);void PokeathlonCourse_IncrementField1ED(void*);void PokeathlonCourse_SetField3A4(void*,u32,u32,u32);void PokeathlonCourse_SetField1F4(void*,u32);void PokeathlonCourse_SetVBlankIntrCB(void*);void*PokeathlonCourse_GetDataCopyArea(void*);NameEntry*PokeathlonCourse_GetParticipantUnk04(void*,int);
void Heap_Create(int,int,u32);void Main_SetVBlankIntrCB(void*,void*);void Main_SetHBlankIntrCB(void*,void*);void GfGfx_DisableEngineAPlanes(void);void GfGfx_DisableEngineBPlanes(void);void GfGfx_SwapDisplay(void);void*BgConfig_Alloc(int);void FontID_Alloc(int,int);void NNS_G2dInitOamManagerModule(void);void OamManager_Create(int,int,int,int,int,int,int,int,int);void GfGfx_EngineATogglePlanes(int,int);void GfGfx_EngineBTogglePlanes(int,int);void WirelessStatusIcon_CreateOnScreenIfConnected(int);void BeginNormalPaletteFade(int,int,int,int,int,int,int);int IsPaletteFadeFinished(void);
void ov96_021F9E3C(void);void ov96_021F9E5C(void*);void ov96_021FB7C8(Work*);void*ov96_021FC028(u32);void*ov96_021FC618(u32);u32 ov96_021FC0E4(void*);u32 ov96_021FC0E8(void*);u32 ov96_021FC0F4(void*);void ov96_021E6670(void*,int);void ov96_021E92B0(Gfx*,int,int,u32,u32);
void*ov96_021EB180(u32,Triple*);void ov96_021EB5C8(void*,int,int,int,int);void ov96_021EB29C(void*,int,int);void ov96_021EB2BC(void*,int,int,int,int);void ov96_021EB2F4(void*,int,int,int,int,int);void ov96_021EB334(void*,int,int,int);void ov96_021EB36C(void*,int,int,int);void ov96_021EB3A4(void*);void*ov96_021E9A78(u32,int,int);void*ov96_021EB5E8(void*);void*ov96_021EA854(u32,int,int,void*,void*);void*ov96_021EB408(void*,int,int,int,int);void*ov96_021EB3E4(void*,int,int,int,int);void ov96_021EB564(void*,int);void ov96_021EB588(void*,Triple*);void ov96_021EB52C(void*,int,int);void ov96_021EB630(void*,int);
void*ov96_021EA374(void*,void*,int,u32);void Sprite_SetDrawPriority(void*,int);void Sprite_SetDrawFlag(void*,int);void Sprite_SetMatrix(void*,Triple*);void Sprite_SetAnimCtrlSeq(void*,int);void ov96_021FC630(void*,void*,int);void*ov96_021FC188(u32);void ov96_021FC214(void*,Pair*);void ov96_021FC2B4(void*,int);int ov96_021E5F24(void*);void ov96_021E6168(void*,int,int,Pokemon*);void*ov96_021E60C0(void*,int,int);void*ov96_021E6108(void*);void ov96_021EA8A8(void*,int,Pokemon*,Options*,int,int);int ov96_021EAA00(void*);void ov96_021F9FE8(void*,u32);void ov96_021FA020(void*,u32);void*ov96_021EAA04(void*,u8);void ov96_021EAB38(void*,int);void ov96_021FC144(void*);void ov96_021FC07C(void*,void*);void**ov96_021E6290(void*,int,void*,void*);void ReadWholeNarcMemberByIdPair(void*,int,int);void ov96_021FA0E8(void*,void*,void*,void*,void*,void*,u8,Display*);void ov96_021E634C(void*,int,void*,void*,int,int,Pair*);void ov96_021FBBB4(void*,void*);u8*ov96_021E8A20(void*);

void ov96_021F0A5C(void);void ov96_021F0BD4(void*);void ov96_021F2EC8(Work*);void*Heap_Alloc(int,u32);void*ov96_021F3BF0(u32,void*,void*);int PokeathlonCourse_GetParticipantCount(void*);int PokeathlonCourse_GetMode(void*);void*ov96_021F3390(u32,int,int);void*ov96_021F30A4(u32);void ov96_021F3E58(void*);void ov96_021E64B8(void*);void ov96_021F3E60(void*,void*);void ov96_021F3EC0(void*,void*);void PokeathlonCirclePushDisplay_CreateSprites(void*,void*);void ov96_021F30C4(void*,void*);void*ov96_021EB4F4(void*,int,int);void*ov96_021EB5B8(void*);void ov96_021E8BB4(Pokemon*,u32,void*);void TintPalette_GrayScale(u16*,u32);void ov96_021EAB38(void*,int);void ov96_021EAF70(void*,u32,u32);void*ov96_021EAA20(void*);int ov96_021E90FC(void*);void*ov96_021E8BB0(void*);void ov96_021EAC0C(void*,int);void*ov96_021EAF94(void*,int,int);int ov96_021E6104(void);void ov96_021EAF6C(void*,u32);void ov96_021EB0A4(void*,int,int,int*,int*);void ov96_021EABA8(void*,int);void ov96_021EABF4(void*,Triple*);void ov96_021F0F04(Work*,void*);void*ov96_021E64F8(void*,void*,void*,void*,int);void ov96_021F2B24(void*,void*,u8,u8,Participant*);void ov96_021F33E0(void*,u8,u16,void*,void*,void*,void*,void*);void ov96_021F31F0(void*,u8);void ov96_021F0D60(Work*);void PokeathlonCirclePushDisplay_PrintPlayerNames(void*);u32 ov96_021E6138(void*);static inline int GetCourseType(void *course, int idx) {
    return ov96_021E6138(ov96_021E60C0(course, idx / 3, idx % 3));
}
static inline void *LoadRosterMon(void *course, int idx, Pokemon *mon) {
    int col = idx % 3;
    int row = idx / 3;
    ov96_021E6168(course, row, col, &mon[idx]);
    return ov96_021E6108(ov96_021E60C0(course, row, col));
}
int ov96_021F010C(void*);
// @fn ov96_021F010C
int ov96_021F010C(void *course) {
    Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0: {
        Gfx gfx;
        Heap_Create(92, 140, 0x50000);
        Main_SetVBlankIntrCB(0, 0);
        Main_SetHBlankIntrCB(0, 0);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(volatile u32 *)0x04000000 &= 0xffffe0ff;
        *(volatile u32 *)0x04001000 &= 0xffffe0ff;
        ov96_021F0A5C();
        w = PokeathlonCourse_AllocPtr4FromHeap(course, 0x7f4);
        MI_CpuFill8(w, 0, 0x7f4);
        w->extra = Heap_Alloc(140, 40);
        MI_CpuFill8(w->extra, 0, 40);
        w->bg = BgConfig_Alloc(140);
        ov96_021E6670(course, 8);
        gfx.a = 104;
        gfx.b = 0x40000;
        gfx.c = 0x4000;
        gfx.d = 140;
        ov96_021E92B0(&gfx, 22, 140, 0x300010, 16);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, 140);
        w->heap = 140;
        FontID_Alloc(4, 140);
        ov96_021F0BD4(w->bg);
        ov96_021F2EC8(w);
        gSystem[0x69] = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 1: {
        int count;
        w->score = ov96_021F3BF0(w->heap, w->bg, course);
        count = PokeathlonCourse_GetParticipantCount(course);
        w->rank = ov96_021F3390(w->heap, 4 - count, PokeathlonCourse_GetMode(course));
        w->resource = ov96_021E9A78(w->heap, 2727, 1);
        w->cursor = ov96_021F30A4(w->heap);
        w->cursorCopy = w->cursor;
        ov96_021F3E58(w->score);
        ov96_021E64B8(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 2:{int i;Triple templ=ov96_0221BC70;w->manager=ov96_021EB180(w->heap,&templ);ov96_021EB5C8(w->manager,0,0,0,0x200000);w->loader=ov96_021EA854(w->heap,12,5,w->resource,ov96_021EB5E8(w->manager));
ov96_021EB29C(w->manager,0,101);
ov96_021EB29C(w->manager,1,104);
ov96_021EB29C(w->manager,2,102);
ov96_021EB29C(w->manager,3,103);
ov96_021EB29C(w->manager,4,105);
ov96_021EB2BC(w->manager,167,3,101,1);ov96_021EB2F4(w->manager,167,0,101,1,1);ov96_021EB334(w->manager,167,2,101);ov96_021EB36C(w->manager,167,1,101);
ov96_021EB2BC(w->manager,167,28,104,1);ov96_021EB2F4(w->manager,167,25,104,1,1);ov96_021EB334(w->manager,167,27,104);ov96_021EB36C(w->manager,167,26,104);
ov96_021EB2BC(w->manager,167,32,105,2);ov96_021EB2F4(w->manager,167,29,105,2,5);ov96_021EB334(w->manager,167,31,105);ov96_021EB36C(w->manager,167,30,105);
ov96_021F3E60(w->manager,w->score);ov96_021EB3A4(w->manager);for(i=0;i<8;i++)ov96_021EB408(w->manager,2,1,101,2);for(i=0;i<12;i++){ov96_021EB408(w->manager,1,1,104,4);ov96_021EB408(w->manager,1,1,104,7);}ov96_021F30C4(w->cursor,w->manager);for(i=0;i<12;i++){Slot*p=&((Slot(*)[3])w)[i/3][i%3];p->a=ov96_021EB4F4(w->manager,104,4);Sprite_SetDrawPriority(ov96_021EB5B8(p->a),2);p->b=ov96_021EB4F4(w->manager,104,7);Sprite_SetDrawPriority(ov96_021EB5B8(p->b),4);}w->sprite=*ov96_021E6290(course,0,w->resource,w->manager);Sprite_SetDrawPriority(w->sprite,1);ov96_021F3EC0(w->manager,w->score);PokeathlonCirclePushDisplay_CreateSprites(w->manager,w->score);PokeathlonCourse_IncrementField1ED(course);break;}

    case 3: {
        Pokemon roster[12];
        Options options;
        s32 i;
        for (i = 0; i < 12; i++) {
            Participant *p;
            options.data[i] = LoadRosterMon(course, i, roster);
            p = &w->participants[i / 3][i % 3];
            ov96_021E8BB4(&roster[i], w->heap, p->palette);
            MI_CpuCopy8(p->palette, p->gray, 32);
            TintPalette_GrayScale(p->gray, 16);
        }
        options.a = 0;
        options.b = 1;
        options.c = 0;
        options.d = 1;
        options.e = 1;
        ov96_021EA8A8(w->loader, 12, roster, &options, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 4: {
        s32 i;
        int own;
        Pair coords[3];
        Coord table[3];
        Triple pos;
        Triple *pp = &pos;
        u8 courseData[60];
        int outX, outY;
        if (!ov96_021EAA00(w->loader)) {
            break;
        }
        own = ov96_021E5F24(course);
        PokeathlonCourse_SetVBlankIntrCB(w->bg);
        PokeathlonCourse_SetField1F4(course, 1);
        ReadWholeNarcMemberByIdPair(table, 170, 11);
        for (i = 0; i < 12; i++) {
            void *render = ov96_021EAA04(w->loader, i);
            void *data;
            int row, col;
            Participant *p;
            int height;
            int x, y;
            ov96_021EAB38(render, 1);
            {
                Coord *c = &table[GetCourseType(course, i)];
                ov96_021EAF70(render, c[-1].x, c[-1].y);
            }
            render = ov96_021EAA04(w->loader, i);
            data = ov96_021EAA20(render);
            pp->a = 0;
            pp->b = 0;
            pp->c = 0;
            row = i / 3;
            col = i % 3;
            p = &w->participants[row][col];
            p->render = render;
            p->active = 1;
            height = ov96_021E90FC(data);
            if (*(u16 *)((u8 *)ov96_021E8BB0(data) + 4)) {
                pos.a = 0x20000;
                pos.b = (height + (64 - height) / 2) << 12;
            } else {
                pos.a = 0x10000;
                pos.b = (height + (32 - height) / 2) << 12;
            }
            x = ov96_0221BDD4[row][col].a;
            y = ov96_0221BDD4[row][col].b;
            p->flag = 2;
            ov96_021EAC0C(render, 2);
            ov96_021EAF94(render, x, y);
            ov96_021EAF6C(render, ov96_021E6104());
            ov96_021EB0A4(render, x, y, &outX, &outY);
            p->x = outX << 12;
            p->y = outY << 12;
            p->x2 = outX << 12;
            p->y2 = outY << 12;
            p->x1 = outX << 12;
            p->y1 = outY << 12;
            if (row == ov96_021E5F24(course)) {
                ov96_021EABA8(render, 5);
            } else {
                ov96_021EABA8(render, 6);
            }
            ov96_021EABF4(render, &pos);
            if (ov96_021E5F24(course) == 0) {
                ov96_021F0F04(w, ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course) + 40));
            }
            if (row == own) {
                coords[col].a = outX;
                coords[col].b = outY;
                Sprite_SetDrawPriority(ov96_021E64F8(course, render, w->resource, ov96_021EB5E8(w->manager), 1), 3);
            }
        }
        ov96_021E634C(course, 0, w->resource, w->manager, 1, 3, coords);
        ReadWholeNarcMemberByIdPair(courseData, 170, 1);
        {
        int k, j;
        for (k = 0; k < 4; k++) {
            w->counts[k] = 12;
            for (j = 0; j < 3; j++) {
                Participant *p = &w->participants[k][j];
                ov96_021F2B24(course, courseData, k, j, p);
                ov96_021F33E0(w->rank, j + k * 3, p->id, &p->p8e, &p->x2, &p->active, &p->x1, p->render);
            }
        }
        }
        w->current = 0;
        w->next = 0;
        w->remaining = 12;
        ov96_021F31F0(w->cursor, w->current);
        G2x_SetBlendAlpha_(0x04000050, 0, 1, 12, 4);
        *((u8 *)w + 0x74b) = 12;
        ov96_021F0D60(w);
        PokeathlonCirclePushDisplay_PrintPlayerNames(w->score);
        GfGfx_EngineATogglePlanes(16, 1);
        GfGfx_EngineBTogglePlanes(16, 1);
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 5:
        return 1;
    }
    return 0;
}
