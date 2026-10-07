#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;u8 pad[16];void *numbers,*ui;u8 gap[136];int messages,ready;} Scene;
typedef struct {u32 player;u8 gap[5];u8 team;} Player;
extern Scene *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern int IsFanfarePlaying(void);
extern int PokeathlonCourse_GetMode(void *);
extern int ov96_021F85F4(void *);
extern void ov96_021EE830(void *);
extern int ov96_021EEA80(void *);
extern void *ov96_021EE97C(void *);
extern int ov96_021F83DC(void *);
extern int ov96_021F81CC(void *,void *);
extern void *PokeathlonCourse_GetGraphicsSystem(void *);
extern Player *ov96_021E94EC(void *,u8);
extern void *PokeathlonCourse_GetPlayerProfileFromData(void *,u32);
extern void BufferPlayersName(void *,int,void *);
extern void StartBrightnessTransition(int,int,int,int,int);
extern void UpdateMainScreenBrightnessSurface(int,int);
extern int IsBrightnessTransitionActive(int);
extern void *PokeathlonCourse_GetField3D8_ForCurrentParticipant(void *);
extern int ov96_021E95D8(void *,void *,int);
extern void ov96_021F910C(void *,int,u8);
extern int ov96_021F83FC(void *);
extern int ov96_021F7E64(void *);
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);
extern void GF_SndStartFadeOutBGM(int,int);
extern int IsPaletteFadeFinished(void);
extern void ov96_021EE944(void *);
extern void ov96_021EE8CC(void *,int);
int ov96_021F7E74(void *,u8 *);
int ov96_021F7E74(void *course,u8 *state) {
 int busy=0;int done;Scene *scene=PokeathlonCourse_GetHeapAllocPtr4(course);void *buffer;int message;Player *players[4];int i;
 done=0;IsFanfarePlaying();if(PokeathlonCourse_GetMode(course)==1)busy=ov96_021F85F4(course);
 ov96_021EE830(scene->ui);
 if(!(ov96_021EEA80(scene->ui)||busy)) {
 buffer=ov96_021EE97C(scene->ui);message=-1;
 switch(*state) {
 case 0:message=ov96_021F83DC(course);(*state)++;break;
 case 1:
 if(!ov96_021F81CC((u8 *)scene+28,course))break;
 for(i=0;i<4;i++)players[i]=ov96_021E94EC(PokeathlonCourse_GetGraphicsSystem(course),(u8)i);
 if(players[0]->team==players[1]->team)message=254;
 else {void *profile=PokeathlonCourse_GetPlayerProfileFromData(course,players[0]->player);BufferPlayersName(buffer,0,profile);message=253;}
 (*state)++;break;
 case 2:StartBrightnessTransition(16,-8,0,63,1);UpdateMainScreenBrightnessSurface(63,63);*(volatile u16 *)0x04000052=16;(*state)++;break;
 case 3:if(IsBrightnessTransitionActive(1)){message=255;(*state)++;}break;
 case 4:
 for(i=0;i<4;i++){void *participant=PokeathlonCourse_GetField3D8_ForCurrentParticipant(course);int number=ov96_021E95D8(PokeathlonCourse_GetGraphicsSystem(course),participant,i);if(number>999)number=999;ov96_021F910C(scene->numbers,number,(u8)i);}
 StartBrightnessTransition(16,0,16,63,2);(*state)++;break;
 case 5:if(IsBrightnessTransitionActive(2))(*state)++;break;
 case 6:message=ov96_021F83FC(course);scene->ready=1;(*state)++;break;
 case 7:if(PokeathlonCourse_GetMode(course)==0||ov96_021F7E64(course)) {BeginNormalPaletteFade(0,0,0,32767,12,1,scene->heap);GF_SndStartFadeOutBGM(0,12);(*state)++;}break;
 case 8:if(IsPaletteFadeFinished()){ov96_021EE944(scene->ui);done=1;}break;
 }
 if(message!=-1){ov96_021EE8CC(scene->ui,message);scene->messages++;}
 }
 return done;
}
