#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov70_02246814[],ov70_02246268[];extern struct {u8 p[64];int arg;} ov70_02246900;extern int _02246800[2];extern void GameStats_Start(void),GameStats_Shutdown(void);extern int ov70_02238360(void *,void *,int,int,int);
void ov70_022381A4(u8 *,int);
void ov70_022381A4(u8 *data,int arg){u32 i;u8 *src,*dst;ov70_02246900.arg=arg;GameStats_Start();src=data;dst=ov70_02246814;i=8;do{*dst++=*src++;i--;}while(i!=0);if(ov70_02238360(ov70_02246268,ov70_02246814,8,arg,((s8*)data)[6]*292)){_02246800[0]=14;return;}_02246800[0]=24;_02246800[1]=-13;GameStats_Shutdown();}
