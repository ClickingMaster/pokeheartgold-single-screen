#pragma force_active on
#include "global.h"
typedef struct {
    u16 species;
    u16 x6;
    int x8;
    u8 stats[5];
    u8 pad11[3];
    u8 x14;
    u8 gender;
    u16 name[11];
} MonEntry;
typedef struct {
    int id;
    MonEntry mons[3];
} Participant;
typedef struct {
    u16 species[3];
    u8 nameId;
    u8 x7;
} TeamEntry;
typedef struct {
    u8 pad[0xc];
    u8 xc;
    u8 pad2[2];
    u8 xf;
} Setup;
typedef struct {
    u8 pad[0x1f8];
    Setup *setup;
} Course;
int PokeathlonCourse_GetHeapID(Course *course);
void *NewMsgDataFromNarc(int type, int narcId, int fileId, int heapId);
void ov96_021E860C(int count, u8 a, u8 b, u8 *ids);
void *NARC_New(int narcId, int heapId);
Participant *PokeathlonCourse_GetParticipantData(Course *course, int index);
void NARC_ReadWholeMember(void *narc, u32 fileId, void *dest);
void *NewString_ReadMsgData(void *msg, u32 id);
void *PokeathlonCourse_GetPlayerProfileFromData(Course *course, int index);
const u16 *String_cstr(void *str);
void Save_Profile_PlayerName_Set(void *profile, const u16 *name);
void String_Delete(void *str);
u32 GetMonBaseStat(int species, int stat);
int ov96_021E679C(int species, int base);
void *GetSpeciesName(u16 species, int heapId);
void CopyU16StringArrayN(u16 *dest, const u16 *src, u32 n);
void NARC_Delete(void *narc);
void DestroyMsgData(void *msg);
void ov96_021E8484(Course *course, int count);
// @fn ov96_021E8484
void ov96_021E8484(Course *course, int count) {
    u8 stats[21];
    u8 ids[3];
    TeamEntry entry;
    void *name;
    int j;
    int k;
    void *msg;
    void *narcStats;
    void *narcTeams;
    s32 i;
    msg = NewMsgDataFromNarc(1, 0x1b, 0x136, PokeathlonCourse_GetHeapID(course));
    i = 4 - count;
    ov96_021E860C(count, course->setup->xc, course->setup->xf, ids);
    narcStats = NARC_New(0xa9, PokeathlonCourse_GetHeapID(course));
    narcTeams = NARC_New(0x104, PokeathlonCourse_GetHeapID(course));
    for (j = 0; i < 4; i++, j++) {
        Participant *p = PokeathlonCourse_GetParticipantData(course, i);
        void *str;
        p->id = ids[j];
        GF_ASSERT(p->id != 0);
        NARC_ReadWholeMember(narcTeams, p->id - 1, &entry);
        str = NewString_ReadMsgData(msg, entry.nameId);
        Save_Profile_PlayerName_Set(PokeathlonCourse_GetPlayerProfileFromData(course, i), String_cstr(str));
        String_Delete(str);
        for (k = 0; k < 3; k++) {
            u8 gender;
            p->mons[k].species = entry.species[k];
            p->mons[k].x6 = 0;
            p->mons[k].x14 = 0;
            p->mons[k].x8 = 0;
            switch ((u8)GetMonBaseStat(p->mons[k].species, 0x12)) {
            case 0:
                gender = 0;
                break;
            case 0xfe:
                gender = 1;
                break;
            case 0xff:
                gender = 2;
                break;
            default:
                gender = 0;
                break;
            }
            p->mons[k].gender = gender;
            NARC_ReadWholeMember(narcStats, ov96_021E679C(p->mons[k].species, 0), stats);
            p->mons[k].stats[0] = stats[0];
            p->mons[k].stats[1] = stats[1];
            p->mons[k].stats[2] = stats[2];
            p->mons[k].stats[3] = stats[3];
            p->mons[k].stats[4] = stats[4];
            name = GetSpeciesName(entry.species[k], PokeathlonCourse_GetHeapID(course));
            CopyU16StringArrayN(p->mons[k].name, String_cstr(name), 11);
            String_Delete(name);
        }
    }
    NARC_Delete(narcTeams);
    NARC_Delete(narcStats);
    DestroyMsgData(msg);
}
