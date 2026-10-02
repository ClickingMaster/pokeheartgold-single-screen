#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x4a0];u16 page;u8 gap[6];int action;} Work;
extern void ov67_021E6490(Work*),ov67_021E6530(Work*),ov67_021E6688(Work*,int),ov67_021E6C14(Work*,int);

void ov67_021E6C60(Work*w,int a,int b);

void ov67_021E6C60(Work*w,int a,int b){w->action=0;if((a==2&&b==0)||(a==5&&b==3)||(a==8&&b==6)||(a==11&&b==9)){if(w->page!=0)w->page--;else w->page=2;ov67_021E6490(w);ov67_021E6530(w);w->action=1;}if((a==0&&b==2)||(a==3&&b==5)||(a==6&&b==8)||(a==9&&b==11)){if(w->page<2)w->page++;else w->page=0;ov67_021E6490(w);ov67_021E6530(w);w->action=2;}ov67_021E6688(w,a==12?30:a+12*w->page);ov67_021E6C14(w,a);}
