#pragma force_active on
#include "global.h"
extern void*PokeathlonCourse_GetGraphicsSystem(void*);extern void PokeathlonCourseScoreList_Init(void*),PokeathlonCourseScoreList_Rank(void*,int),PokeathlonEvent_CalcPoints(void*,int),PokeathlonCourseScoreList_AddPointsToTotals(void*),PokeathlonCourseScoreList_SaveEventPoints(void*,int);extern int PokeathlonCourse_GetField5F0_AtIndex(void*,u8);extern void PokeathlonCourse_AddScoreEntry2(void*,void*,int,int,int,int,int,int,int,int,int);
int ov96_021FCD84(void*w);
int ov96_021FCD84(void*w){void*g=PokeathlonCourse_GetGraphicsSystem(w);int i;PokeathlonCourseScoreList_Init(g);for(i=0;i<4;i++){int v=(u16)PokeathlonCourse_GetField5F0_AtIndex(w,(u8)i);PokeathlonCourse_AddScoreEntry2(w,g,i,v,(u32)v>>10,(v%1024)*10/1024,0,1,2,1,297);}PokeathlonCourseScoreList_Rank(g,1);PokeathlonEvent_CalcPoints(g,6);PokeathlonCourseScoreList_AddPointsToTotals(g);PokeathlonCourseScoreList_SaveEventPoints(g,6);return 1;}
