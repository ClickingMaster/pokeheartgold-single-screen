#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p0[8];int state,amount;u8 p10[0x28-0x10];void*palettes;u8 p2c[0x54-0x2c];int done;u8 p58[0x6f0-0x58];void*menu,*menu2;}Work;
extern void OrbitCursor_SetVisible(void*,int),ov40_02230964(Work*,int),PaletteData_BlendPalettes(void*,int,u16,u8,u16),BeginNormalPaletteFade(int,int,int,int,int,int,int);extern int ov40_0222DAF0(Work*),IsPaletteFadeFinished(void);
int ov40_02231748(Work*);
int ov40_02231748(Work*w){switch(w->state){case 0:w->done=0;w->amount=0;OrbitCursor_SetVisible(w->menu,0);OrbitCursor_SetVisible(w->menu2,0);ov40_02230964(w,1);w->state++;break;case 1:if(w->amount!=16){w->amount+=2;PaletteData_BlendPalettes(w->palettes,2,65534,(u8)w->amount,(u16)ov40_0222DAF0(w));PaletteData_BlendPalettes(w->palettes,0,65535,(u8)w->amount,(u16)ov40_0222DAF0(w));PaletteData_BlendPalettes(w->palettes,3,16382,(u8)w->amount,(u16)ov40_0222DAF0(w));PaletteData_BlendPalettes(w->palettes,1,65535,(u8)w->amount,(u16)ov40_0222DAF0(w));}else{w->amount=16;w->state++;}break;case 2:if(w->amount!=0)w->amount-=4;else{BeginNormalPaletteFade(0,0,0,0,6,1,109);w->state++;}break;default:if(IsPaletteFadeFinished()==1){ov40_02230964(w,0);return 1;}break;}return 0;}
