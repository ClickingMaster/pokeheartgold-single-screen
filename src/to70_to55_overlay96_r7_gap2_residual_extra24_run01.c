#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Profile {u16 species,attrs[3];u32 id;u16 name[8];u8 lang,pad[3];} Profile;
typedef struct Row {u16 species,attrs[3];} Row;
typedef struct Small {Row rows[5];u32 count;} Small;
typedef struct NameRec {u32 id;u16 name[8];u8 lang,pad[3];} NameRec;
typedef struct Big {Row rows[5];u32 count;NameRec names[5];} Big;
typedef struct Entry {u16 a,b;u8 pad[36];} Entry;
typedef struct Player {Entry entries[3];u32 pad;} Player;
typedef struct Work {u8 pad[0x3d8];u32 ids[6];Player players[4];u8 gap[0x2f4];u16 species[4][4];} Work;
extern u8 _0221A7D8[];
extern int ov96_021E5F24(Work*);
extern void ov96_021E7C04(int,Profile*,Profile*),ov96_021E7C94(Work*,Small*,Profile*),ov96_021E7CC8(Work*,Big*,Profile*);
extern void *PokeathlonCourse_GetPlayerProfileFromData(Work*);
extern u32 PlayerProfile_GetTrainerID(void*);extern const u16 *PlayerProfile_GetNamePtr(void*);extern u8 PlayerProfile_GetLanguage(void*);
void ov96_021E7938(Work*,Small*);void ov96_021E7A2C(Work*,Big*);
void ov96_021E7938(Work*w,Small*all){Profile p;Profile rows[5];for(u8 i=0;i<3;i++){u8 id=w->ids[i];Small*r=&all[id];if(r->count<9999999)r->count++;int kind=_0221A7D8[id];for(u8 j=0;j<5;j++){rows[j].species=r->rows[j].species;for(u8 k=0;k<3;k++)rows[j].attrs[k]=r->rows[j].attrs[k];}p.species=w->species[(u8)ov96_021E5F24(w)][i];u8 curr=ov96_021E5F24(w);Player*player=(Player*)((u8*)w+0x3f0+curr*0x7c);for(u8 j=0;j<3;j++)p.attrs[j]=(player->entries[j].b<<10)+player->entries[j].a;ov96_021E7C04(kind,&p,rows);ov96_021E7C94(w,r,rows);}}
