#include "global.h"
#include "sprite_system.h"
#pragma force_active on
void ov40_02243EB0(u8 *,int),ov40_02243EEC(u8 *,int),ov40_02243E80(u8 *,int,int),ov40_02243D64(void *);
#define P(o) (*(ManagedSprite **)(w+(o)))
#define W(o) (*(int *)(w+(o)))
#define H(o) (*(u16 *)(w+(o)))
typedef struct Record {s32 index,id,enabled;ManagedSprite *sprite;u8 native10[4];u16 reset0,reset1;u8 native18[4];} Record;
typedef struct Parent {Record normal[12],boundary[2],extra[3];u8 nativeTail[0x2d8-17*28];} Parent;
static inline void InitializeOverlaySprite(u8 *w,Record *record) {
    record->reset0=0;
    record->reset1=0;
    record->index=1;
    ov40_02243EEC(w,0);
    ManagedSprite_SetAnim(record->sprite,record->index);
    ManagedSprite_TickFrame(record->sprite);
    ManagedSprite_SetOamMode(record->sprite,GX_OAM_MODE_XLU);
}
void ov40_02243D64(void *object){u8 *w=object;SpriteSystem *system=*(SpriteSystem **)(w+548);SpriteManager *manager=*(SpriteManager **)(w+552);ManagedSpriteTemplate t;
 t.x=0;t.y=0;t.z=0;t.animation=0;t.vram=NNS_G2D_VRAM_TYPE_2DMAIN;t.resIdList[0]=1001;t.resIdList[1]=1001;t.resIdList[2]=1001;t.resIdList[3]=1001;t.drawPriority=0;t.bgPriority=0;t.vramTransfer=0;t.pal=0;t.resIdList[4]=-1;t.resIdList[5]=-1;
 P(404)=SpriteSystem_NewSprite(system,manager,&t);P(432)=SpriteSystem_NewSprite(system,manager,&t);P(460)=SpriteSystem_NewSprite(system,manager,&t);ov40_02243EB0(w,0);ManagedSprite_SetAnim(P(404),0);ManagedSprite_TickFrame(P(404));InitializeOverlaySprite(w,&((Parent *)w)->extra[1]);InitializeOverlaySprite(w,&((Parent *)w)->extra[2]);ov40_02243E80(w,1,0);ov40_02243E80(w,2,0);
}
