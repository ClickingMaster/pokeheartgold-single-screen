#include "global.h"
#include "pokeathlon/pokeathlon_save.h"
#include "pokeathlon/pokeathlon.h"
#include "message_format.h"
#include "sound.h"
#include "sound_playback.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include <string.h>

typedef char prefix_size_checked[sizeof(PokeathlonSavePrefix)==0x2cc?1:-1];
typedef char save_size_checked[sizeof(PokeathlonSave)==0xb80?1:-1];
#pragma force_active on
typedef struct{u32 a:16;u32 rank:8;u32 player:4;u32 other:4;} Rank;
typedef struct {u8 course,player,member,award,state,b6,printed;} ProgressW17;
typedef struct Award {u32 heap;void*bg,*task,*text;u8 gap10[16];void*sprites[4];u8 gap30[80];void*medals[2];void*allocation,*score,*animation;u32 timer;u32 pad98;Rank rank[4];u8 gapac[5];ProgressW17 progress;u32 closing;} Award;
typedef char AwardSizeW17[(sizeof(Award)==0xbc)?1:-1];
typedef u32 (*AwardCallback)(void*,int,int);
typedef struct {u8 data[28];} Entry;
typedef struct Ov96R39Manager Ov96R39Manager;
typedef struct Ov96TextBoxR43 Ov96TextBoxR43;
typedef struct Ov96RankEntryR38 Ov96RankEntryR38;
typedef struct Ov96RankEntry Ov96RankEntry;
extern void *ov96_021EC2E0(u8*);
extern void ov96_021EC458(void*,u32),ov96_021ECA70(void*,int),ov96_021ECB38(void**,void*,int,int,u32);
extern void ov96_021ECBFC(Entry*);
extern int ov96_021ECC14(Entry*);
extern void ov96_021ECC38(u8*,int,u32),ov96_021ECC7C(void*,Ov96RankEntryR38*,const u8*);
extern void *ov96_021ECC4C(u8*,int);
extern void ov96_021ED0C8(Ov96R39Manager*),ov96_021ED158(Ov96R39Manager*,u32);
extern void ov96_021ED578(void*,int,int);
extern u32 ov96_021ED5E0(void*);
u32 ov96_021ED618(void*,int,int),ov96_021ED660(void*,int,int),ov96_021ED6A8(void*,int,int);
extern u8 ov96_021ED6F8(void*,u8);
/* Dependency: actual selected void ED728 must be repaired to forward genuine u32 E95D8 result. */
extern u32 ov96_021ED728(void*,u8,u8);
extern int ov96_021ED748(void*),ov96_021ED74C(void*),ov96_021ED750(void*);
extern u32 ov96_021ED754(void*),ov96_021ED78C(void*),ov96_021ED7C4(void*);
extern void ov96_021ED7FC(void*,u8*);
extern u32 ov96_021EDC38(u32,u32);
extern u8 ov96_021EDCB4(void*);
int ov96_021EDDA4(void*,AwardCallback,int);
extern void ov96_021EDE64(void*,void*),ov96_021EDF3C(void*,s32,u32,u32);
extern void ov96_021EE290(Ov96RankEntry*);
extern BOOL ov96_021EE440(void*),ov96_021EE580(void*),ov96_021EE830(Ov96TextBoxR43*);
extern void ov96_021EE8CC(Ov96TextBoxR43*,u32),ov96_021EE944(u8*);
extern void *ov96_021EE97C(u8*);
extern u32 ov96_021EEA80(u8*);
extern u16 GF_GetCurrentPlayingBGM(void);
int ov96_021EBB64(PokeathlonCourseData*);
int ov96_021EBB64(PokeathlonCourseData*course){Award*d=PokeathlonCourse_GetHeapAllocPtr4(course);int wait=0,msg,i;void*format;u8 rank[4];int count;ov96_021ECBFC(d->score);ov96_021ED0C8(d->animation);if(PokeathlonCourse_GetMode(course)==1)wait=ov96_021EE440(course);ov96_021EE830(d->text);if(ov96_021EEA80(d->text)||wait)goto end;format=ov96_021EE97C(d->text);msg=-1;switch(d->progress.state){
case 0:msg=3;d->progress.state=1;break;case 1:msg=4;d->progress.state=2;break;case 2:for(i=0;i<4;i++){int v=ov96_021ED6F8(course,(u8)i);ov96_021ECC38(d->score,(u8)i,v);}d->progress.state=3;
case 3:if(d->progress.player>=4){d->progress.player=0;msg=6;d->progress.state=4;}else{int v;msg=5;v=ov96_021ED6F8(course,d->progress.player);ov96_021ED578(course,d->progress.player,v);d->progress.player++;}break;
case 4:if(ov96_021ED5E0(course)){msg=7;d->progress.state=5;}else{msg=9;d->progress.state=6;}break;
case 5:if(!ov96_021EC2E0((u8*)d)){if(d->progress.award>=(u32)ov96_021ED754(course)){d->progress.player=0;d->progress.member=0;d->progress.award=0;msg=10;d->progress.state=6;}else if(ov96_021EDDA4(course,ov96_021ED618,ov96_021ED748(d)))msg=8;}break;
case 6:msg=11;d->progress.state=7;break;case 7:if(!ov96_021EC2E0((u8*)d)){if(d->progress.award>=(u32)ov96_021ED78C(course)){d->progress.player=0;d->progress.member=0;d->progress.award=0;msg=10;d->progress.state=8;}else if(ov96_021EDDA4(course,ov96_021ED660,ov96_021ED74C(d)))msg=12;}break;
case 8:if(ov96_021EDCB4(course)!=4){msg=14;d->progress.state=9;}else d->progress.state=11;break;case 9:msg=ov96_021EDCB4(course)+15;d->progress.state=10;break;case 10:if(!ov96_021EC2E0((u8*)d)){if(d->progress.award>=(u32)ov96_021ED7C4(course)){d->progress.player=0;d->progress.member=0;d->progress.award=0;msg=21;d->progress.state=11;}else if(ov96_021EDDA4(course,ov96_021ED6A8,ov96_021ED750(d)))msg=20;}break;
case 11:msg=22;d->progress.state=12;StopBGM(GF_GetCurrentPlayingBGM(),16);PlaySE(1774);break;
case 12:if(ov96_021ECC14(d->score)){if((s32)d->timer++>30){count=PokeathlonCourse_GetMode(course);if(count==0)count=3;else count=4;d->timer=0;if(d->progress.award>=(u8)count){d->progress.award=0;d->progress.state=13;}else{msg=23;ov96_021EDF3C(format,d->progress.award+1,1,0);for(i=0;i<4;i++){int v=ov96_021ED728(course,(u8)i,d->progress.award);ov96_021ECC38(d->score,(u8)i,(u16)v);}d->progress.award++;}}}break;
case 13:ov96_021ED7FC(course,rank);ov96_021ECC7C(d->score,(void*)d->rank,rank);if(d->rank[0].rank==d->rank[1].rank)d->progress.state=14;else d->progress.state=15;break;
case 14:msg=24;ov96_021EE290((void*)d->rank);d->progress.state=15;break;
case 15:msg=25;ov96_021EDF3C(format,(s32)ov96_021ECC4C(d->score,(u8)d->rank[0].player),3,0);ov96_021EDE64(course,d->score);d->progress.state=16;break;
case 16:ov96_021EC458(d->bg,d->heap);d->progress.state=17;
case 17:{int player=d->rank[0].player;msg=26;BufferPlayersName(format,0,PokeathlonCourse_GetPlayerProfileFromData(course,player));ov96_021ECA70(course,(u8)player);ov96_021ED158(d->animation,1);ov96_021ECB38(d->sprites,course,player,2,d->heap);ManagedSprite_SetAnim(d->sprites[player],1);StopSE(1774,0);PlaySE(2274);PlaySE(2272);PlayBGM(1142);if(d->progress.course==10)d->progress.state=22;else d->progress.state=18;break;}
case 22:msg=30;BufferPokeathlonCourseName(format,0,d->progress.course);d->progress.state=20;break;
case 18:msg=27;BufferPlayersName(format,0,PokeathlonCourse_GetPlayerProfileFromData(course,d->rank[0].player));ManagedSprite_SetDrawFlag(d->medals[0],1);BufferPokeathlonMedalName(format,1,ov96_021EDC38(d->progress.course,0));if(d->progress.course>=5){msg=28;ManagedSprite_SetDrawFlag(d->medals[1],1);BufferPokeathlonMedalName(format,2,ov96_021EDC38(d->progress.course,1));}d->progress.state=19;break;
case 19:{u32 mode=d->progress.course,winner;int own,score;PokeathlonSave*save;if(PokeathlonCourse_GetMode(course)!=0)goto name19;winner=d->rank[0].player;own=ov96_021E5F24(course);if(winner!=(u32)own)goto name19;GF_ASSERT(winner==0);save=PokeathlonSave_dummy2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(course)));score=(s32)ov96_021ECC4C(d->score,(u8)d->rank[0].player);{if(save->prefix.nativeHalfwords[mode*22+3]<(u32)score){msg=29;BufferPlayersName(format,0,PokeathlonCourse_GetPlayerProfileFromData(course,d->rank[0].player));BufferPokeathlonCourseName(format,1,mode);}}goto next19;name19:msg=30;BufferPokeathlonCourseName(format,0,mode);next19:d->progress.state=20;break;}
case 20:msg=31;StopSE(2274,120);d->closing=1;d->progress.state=21;break;case 21:if(PokeathlonCourse_GetMode(course)==0||ov96_021EE580(course)){if(ov96_021EC2E0((u8*)d)){Heap_Free(d->allocation);SysTask_Destroy(d->task);}ov96_021EE944(d->text);PokeathlonCourse_SetStateField07(course,2);return 0;}break;default:GF_AssertFail();break;}
if(msg!=-1){ov96_021EE8CC(d->text,msg);d->progress.printed++;}end:return 0;}
