#pragma force_active on
#include "global.h"
#include "unk_02035900.h"
#include "bg_window.h"
#include "system.h"
#include "touchscreen.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern const TouchscreenHitbox ov73_021EA534[];extern void ov73_021E64B0(void*),ov73_021E756C(void*,int,int),ov73_021E670C(void*,int),ov73_021E781C(void*,int),PlaySE(int),ov73_021E71E4(void*,int,int);extern int sub_0203769C(void),ov73_021E746C(void),sub_02037454(void);void ov73_021E6508(void*);
#pragma opt_common_subs off
void ov73_021E6508(void*w){u8 touch=0;struct{u8 send[3];u16 pixel;}packet;int index;ov73_021E64B0(w);index=TouchscreenHitbox_FindRectAtTouchNew(ov73_021EA534);packet.pixel=0xffd5;if(index!=-1&&DoesPixelAtScreenXYMatchPtrVal(P(w,0),1,gSystem.touchX,gSystem.touchY,&packet.pixel))touch=(u8)(index+1);if((gSystem.newKeys&1)||touch==1){if(!sub_0203769C()){if(W(w,0x4a1c)==ov73_021E746C()&&W(w,0x4a24)==0){packet.send[2]=1;ov73_021E756C(w,3,0);ov73_021E670C(w,22);sub_02037030(117,&packet.send[2],1);ov73_021E781C(w,0);PlaySE(1500);ov73_021E71E4(w,0,1);ScheduleBgTilemapBufferTransfer(P(w,0),1);}else PlaySE(1522);}}else if((gSystem.newKeys&2)||touch==2){if(sub_0203769C()){if(W(w,0x4a10)==0){ov73_021E756C(w,4,0);ov73_021E670C(w,4);PlaySE(2368);}else if(W(w,0x4a38)<0){W(w,0x4a38)=2;PlaySE(1522);}}else{if(W(w,0x4a1c)==sub_02037454()&&W(w,0x4a24)==0){packet.send[1]=1;ov73_021E756C(w,4,0);ov73_021E670C(w,4);sub_02037030(117,&packet.send[1],1);ov73_021E781C(w,0);PlaySE(2368);ov73_021E71E4(w,1,1);ScheduleBgTilemapBufferTransfer(P(w,0),1);}else PlaySE(1522);}}else if(W(w,0x4a20)==0&&!sub_0203769C()&&W(w,0x4a1c)==sub_02037454()){packet.send[0]=0;sub_02037030(117,packet.send,1);}if(W(w,0x4a38)>=0)W(w,0x4a38)--;if(W(w,0x4a38)==0){ov73_021E71E4(w,1,0);ScheduleBgTilemapBufferTransfer(P(w,0),1);}}
