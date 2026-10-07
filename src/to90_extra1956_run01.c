#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(w,o) ((u8*)(w)+(o))
#define U(w,o) (*(u32*)P(w,o))
#define V(w,o) (*(void**)P(w,o))
extern void TouchHitboxController_Destroy(void*),GfGfx_EngineBTogglePlanes(int,int),GfGfx_EngineATogglePlanes(int,int),ov40_02230964(void*,int),ov40_0223064C(void*,void*),ov40_0222E7B8(void*,void*),ov40_02232FCC(void*),ov40_0222DA84(void*,int),ov40_022330B8(void*),PaletteData_BlendPalettes(void*,int,int,u8,u16),ov40_022326DC(void*),ov40_0222DED0(void*,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int),ov40_02230944(void*),ov40_0222DF60(void*,int),SaveGameNormal(void*),ov40_0222DFB0(void*),ov40_022327BC(void*),ov40_0222BF80(void*,int);
extern int ov40_0222DA00(void*,void*,int,int),TouchscreenHitbox_TouchNewIsIn(void*),Save_NumModifiedPCBoxesIsMany(void*);
extern u8 ov40_0224512C[],ov40_02245130[];
int ov40_022327F0(void*);
int ov40_022327F0(void*w){void*d=V(w,0x860);switch(U(w,8)){
case 0:TouchHitboxController_Destroy(V(d,0x174));GfGfx_EngineBTogglePlanes(4,0);GfGfx_EngineATogglePlanes(4,0);ov40_02230964(w,1);if(U(d,12)==0)ov40_0223064C(P(d,0x9c),w);else ov40_0222E7B8(P(d,0x10),w);ov40_02230964(w,0);ov40_02232FCC(w);U(d,0x198)=0;U(w,8)++;break;
case 1:ov40_0222DA84(P(d,8),1);if(ov40_0222DA00(d,P(d,4),1,0)){ov40_02230964(w,1);ov40_022330B8(w);ov40_02230964(w,0);U(w,8)++;}PaletteData_BlendPalettes(V(w,0x28),3,12,(u8)U(d,8),(u16)U(w,0x58));break;
case 2:ov40_022326DC(w);ov40_0222DED0(w,0x128);GfGfxLoader_LoadScrnDataFromOpenNarc(V(w,0x14),84,V(w,0x24),7,0,0,0,109);GfGfx_EngineATogglePlanes(8,1);GfGfx_EngineBTogglePlanes(8,1);U(w,8)++;break;
case 3:ov40_0222DA84(P(d,8),0);if(ov40_0222DA00(d,P(d,4),0,2)){GfGfx_EngineATogglePlanes(4,1);GfGfx_EngineBTogglePlanes(4,1);U(w,8)++;}PaletteData_BlendPalettes(V(w,0x28),3,12,(u8)U(d,8),(u16)U(w,0x58));break;
case 4:if(TouchscreenHitbox_TouchNewIsIn(ov40_0224512C)!=0){U(d,0x198)=1;ov40_02230944(w);if(Save_NumModifiedPCBoxesIsMany(V(w,0x830)))ov40_0222DF60(w,0x12a);else ov40_0222DF60(w,0x12b);U(w,8)++;}else if(TouchscreenHitbox_TouchNewIsIn(ov40_02245130)!=0){U(d,0x198)=2;ov40_02230944(w);U(w,8)=6;}break;
case 5:SaveGameNormal(V(w,0x830));U(w,8)++;break;
case 6:ov40_0222DFB0(w);ov40_022327BC(w);U(w,8)++;break;
case 7:ov40_0222DA84(P(d,8),1);if(ov40_0222DA00(d,P(d,4),1,2))U(w,8)++;PaletteData_BlendPalettes(V(w,0x28),3,12,(u8)U(d,8),(u16)U(w,0x58));break;
default:if(U(d,0x198)==1)ov40_0222BF80(w,7);else{U(d,0x198)=0;U(d,12)=0;ov40_0222BF80(w,2);}break;}return 0;}
