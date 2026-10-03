#pragma force_active on
#include "global.h"
#include "unk020932e0_opus_partial_internal.h"
extern int System_GetTouchHeld(void),System_GetTouchNew(void);
extern void PlaySE(u32);
extern int sub_02095354(UnkStruct_020932E0*),sub_020954CC(UnkStruct_020932E0*),sub_02094794(UnkStruct_020932E0*,int),sub_02094860(UnkStruct_020932E0*,int);
extern void sub_02095540(UnkStruct_020932E0*),sub_020955EC(UnkStruct_020932E0*),sub_02095238(UnkStruct_020932E0*),sub_020948C4(UnkStruct_020932E0*,int,int),sub_0209569C(UnkStruct_020932E0*),sub_020956B8(UnkStruct_020932E0*),sub_020949F4(UnkStruct_020932E0*),sub_02094A70(UnkStruct_020932E0*);
int sub_02093630(UnkStruct_020932E0*);
int sub_02093630(UnkStruct_020932E0*p){int result=0;int keys=0;int hit; s8 dx,dy;VecFx32 pos; if(!System_GetTouchHeld()&&!p->unk_46C0&&(keys=gSystem.newKeys)!=0){int cursorState;dx=0;dy=0;cursorState=sub_02095DD8(p->cursor);switch(keys){case 32:if(cursorState==5)sub_02095540(p);else dx--;break;case 16:if(cursorState==5)sub_020955EC(p);else dx++;break;case 64:dy--;break;case 128:dy++;break;case 1:result=sub_02095354(p);if(result!=0&&result!=3)PlaySE(0x5DD);break;case 2:result=sub_020954CC(p);if(result!=0&&result!=3)PlaySE(0x5DD);break;}if(dx||dy){int changed=sub_02095D88(p->cursor,dx,dy);sub_02095238(p);if(changed)PlaySE(0x5DC);}}
 if(keys==0&&p->unk_46BC==0){if(System_GetTouchNew()&&p->unk_4680==0){p->unk_46C4=1;hit=TouchscreenHitbox_FindRectAtTouchNew(p->hitboxes);if(hit!=-1){if(hit<30){if(p->unk_0F!=18||hit<6){sub_02095D40(p->cursor,2,(u8)hit);if(sub_02094794(p,hit)==1){PlaySE(0x5EB);sub_020948C4(p,1,hit);result=1;}}}else if(hit>=33&&hit<33+p->count){sub_02095D40(p->cursor,3,(u8)(hit-33));if(sub_02094860(p,hit-33)==1){PlaySE(0x5EB);sub_020948C4(p,2,hit-33);result=1;}}else{switch(hit){case 31:sub_02095540(p);break;case 32:sub_020955EC(p);break;case 30:if(p->unk_13){pos.x=14<<16;pos.y=11<<16;pos.z=0;Sprite_SetMatrix(p->spriteB,&pos);sub_02095D40(p->cursor,1,0);sub_0209569C(p);result=3;}break;}}sub_020956B8(p);}p->unk_46C0=1;}else if(System_GetTouchHeld()&&p->unk_46C4){sub_020949F4(p);p->unk_46C0=1;}else{sub_02094A70(p);p->unk_46C0=0;p->unk_46C4=0;}}
 if(p->unk_0E==p->count)p->state=1;return result;}
