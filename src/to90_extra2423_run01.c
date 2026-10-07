#pragma force_active on
#include "global.h"
typedef struct ProfileData { u32 trainer; u16 name[8]; u8 language; char gap[3]; } ProfileData;
typedef struct Stats { u16 id,value[3]; } Stats;
typedef struct Team { Stats stats[5]; u32 counter; ProfileData profiles[5]; } Team;
typedef struct Entry { Stats stats; ProfileData profile; } Entry;
extern u8 _0221A7D8[];
extern int ov96_021E5F24(void*);
extern void *PokeathlonCourse_GetPlayerProfileFromData(void*,u32);
extern u32 PlayerProfile_GetTrainerID(void*);
extern u16 *PlayerProfile_GetNamePtr(void*);
extern u8 PlayerProfile_GetLanguage(void*);
extern void ov96_021E7C04(u32,Entry*,Entry*),ov96_021E7CC8(void*,Team*,Entry*);
void ov96_021E7A2C(char*,Team*);
#pragma force_active on
static inline void CopyProfile(Entry *dst,Team *team,u8 j){u8 k;dst->profile.trainer=team->profiles[j].trainer;for(k=0;k<8;k++)dst->profile.name[k]=team->profiles[j].name[k];dst->profile.language=team->profiles[j].language;}
void ov96_021E7A2C(char*ctx,Team *teams) {
 struct{char *ids;s32 start;} f; f.start=(u8)ov96_021E5F24(ctx);u8 i,j,k,n; Entry opponents[5],self;
 for(i=0;i<4;i++) {
 u8 index=(u8)*(u32*)(ctx+0x3d8+4*i); u32 slot;Team *team=&teams[index];
 if(team->counter<9999999) team->counter++;
 slot=_0221A7D8[index];
 for(j=0;j<5;j++) { opponents[j].stats.id=team->stats[j].id; for(k=0;k<3;k++) opponents[j].stats.value[k]=team->stats[j].value[k]; CopyProfile(&opponents[j],team,j); }
 n=0; {f.ids=ctx+i*2; for(;n<4;n++) {
 u8 player=(f.start+n)%4; char *records; void *profile;
 self.stats.id=*(u16*)(f.ids+0x8d4+player*8);records=ctx+0x3f0+player*124;
 for(k=0;k<3;k++) self.stats.value[k]=(*(u16*)(records+k*40+2)<<10)+*(u16*)(records+k*40);
 profile=PokeathlonCourse_GetPlayerProfileFromData(ctx,player); self.profile.trainer=PlayerProfile_GetTrainerID(profile);
 {u16 *name=PlayerProfile_GetNamePtr(profile); for(k=0;k<8;k++) self.profile.name[k]=name[k]; }
 self.profile.language=PlayerProfile_GetLanguage(profile);
 ov96_021E7C04(slot,&self,opponents);
 }
 } ov96_021E7CC8(ctx,team,opponents);
 }
}
