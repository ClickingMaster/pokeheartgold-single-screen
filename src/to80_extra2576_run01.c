#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "pokepic.h"
#include "gf_gfx_planes.h"
#include "unk_0200FA24.h"
#include "sound_playback.h"
#include "unk_020379A0.h"
#pragma force_active on
typedef struct {
 u8 p0[8];u8 state,mode;u8 pa[2];int x;u16 p10;u8 count,p13;
 u32 duration;u8 p18,phase;u8 p1a[0x4c-0x1a];BgConfig *bg;
 u8 p50[0x1a8-0x50];PokepicManager *picManager;Pokepic *pics[3];
 u8 p1b8[0x360-0x1b8];void **actors[6];void **extraActors[2];
 void **specialActor;u8 p384[0x47c-0x384];u32 picCount;u8 tail[12];
} Work;
typedef char Work_size[(sizeof(Work)==0x48c)?1:-1];
extern BOOL ov80_02237254(int);
/* FUTURE SOURCE DEPENDENCY: proposed canonical getter entry; not installed. */
extern BOOL ov81_022404B4(void *);
extern BOOL ov81_02242DD8(void **),ov81_02240F08(u8 *,u8);
extern void *ov81_02242F30(void **);
extern void ov81_02242F60(u8 *),ov81_02242F94(void **,int);
extern void ov81_02240658(void *,int),ov81_02241398(void *);
BOOL ov81_0223E318(u8 *);
BOOL ov81_0223E318(u8 *raw){Work *w=(Work*)raw;int i;u32 j;switch(w->state){case 0:if(ov80_02237254(w->mode)==1){sub_02037BEC();sub_02037AC0(237);}w->state++;break;case 1:if(ov80_02237254(w->mode)==1){if(sub_02037B38(237)==1){sub_02037BEC();w->state++;}}else w->state++;break;case 2:for(i=0;i<w->count;i++){ov81_02242F30(w->actors[i]);ov81_02242F60((u8*)w->actors[i]);}BgSetPosTextAndCommit(w->bg,6,(enum BgPosAdjustOp)0,264);Pokepic_SetAttr(w->pics[0],6,1);BeginNormalPaletteFade(0,1,1,0,6,3,(enum HeapID)100);w->state++;break;case 3:if(!IsPaletteFadeFinished())break;PlaySE(1553);w->phase=0;w->state++;break;case 4:if(ov81_022404B4(raw)!=1)break;StopSE(1553,0);PlaySE(1656);for(i=0;i<w->count;i++)ov81_02242F94(w->actors[i],4);ov81_02240658(w,0);w->duration=8;w->phase=0;w->state++;break;case 5:if(ov81_02242DD8(w->specialActor)==1)break;w->phase=0;w->state++;break;case 6:if(w->phase==0){GfGfx_EngineATogglePlanes(2,0);if(ov81_02240F08(raw,0)==1){Pokepic_SetAttr(w->pics[0],6,0);Pokepic_StartPaletteFadeAll(w->picManager,16,0,1,65535);}else{for(j=0;j<w->picCount;j++)Pokepic_SetAttr(w->pics[j],6,0);Pokepic_StartPaletteFadeAll(w->picManager,16,0,1,65535);}}Pokepic_ResumePaletteFade(w->pics[0]);w->phase++;if(w->phase>=2){w->phase=0;ov81_02241398(w);return 1;}break;}return 0;}
