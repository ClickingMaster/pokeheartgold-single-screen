#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "heap.h"
#include "message_format.h"
#include "communication_error.h"
typedef struct Ov70Sub {
    void *unk00;
    u8 pad04[4];
    void *unk08;
    void *unk0c;
    u8 pad10[0x18 - 0x10];
    void *unk18;
    u8 pad1c[0x20 - 0x1c];
    void *unk20;
    void *unk24;
    void *unk28;
} Ov70Sub;

typedef struct Ov70Gm {
    u32 gm[4];
} Ov70Gm;

typedef struct Ov70Bt {
    u32 bt[7];
} Ov70Bt;

typedef struct Ov70Work {
    Ov70Sub *unk00;
    void *bgConfig;
    u8 pad08[0x18 - 0x08];
    u32 unk18;
    u32 unk1c;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 state2c;
    u8 pad30[0x36 - 0x30];
    u16 value36;
    u8 pad38[0x3c - 0x38];
    s32 value3c;
    u8 pad40[0x120 - 0x40];
    u16 unk120;
    u16 unk122;
    u8 pad124[0x12c - 0x124];
    u32 unk12c;
    u8 pad130[4];
    s32 unk134;
    s32 unk138;
    u8 unk13c[0x25d - 0x13c];
    s8 unk25d;
    u8 pad25e[0xa5c - 0x25e];
    u8 unkA5C[0xb9c - 0xa5c];
    void *unkB9C;
    void *ptrBA0;
    u8 padBA4[0xbbc - 0xba4];
    void *strBBC;
    u8 padBC0[0xbf0 - 0xbc0];
    u32 printerBF0;
    u8 padBF4[0xf18 - 0xbf4];
    u8 windowF18[0x11dc - 0xf18];
    u16 unk11DC;
    u8 pad11DE[0x11f8 - 0x11de];
    u16 unk11F8;
    u8 pad11FA[2];
    u32 unk11FC;
    u8 pad1200[0x1604 - 0x1200];
    u32 timer1604;
} Ov70Work;



#pragma force_active off
struct SaveBlock22;
extern BOOL GtsServer_IsRequestDone(void);
extern int GtsServer_GetResult(void);
extern int ov70_02240D00(Ov70Work *,void *);
extern void Gts_DeleteWaitingIcon(u8 *);
extern void ov70_02244FA4(u8 *,void *,int,int,int);
struct Ov70State;
extern void Gts_SetState(struct Ov70State *,u32,u32);
extern u16 sub_0202DB54(struct SaveBlock22 *);
extern void sub_0202DB64(struct SaveBlock22 *,void *);
extern u16 sub_0202DBA0(struct SaveBlock22 *);
extern void sub_0202DB5C(struct SaveBlock22 *,u16);
extern void ov70_02240A7C(Ov70Work *,void *,int,int);
extern void ov70_022404D4(Ov70Work *);
extern void sub_020399EC(void);
int ov70_02240240(Ov70Work *);
typedef char whole_work_extent[sizeof(Ov70Work)==0x1608?1:-1];
#pragma force_active on
int ov70_02240240(Ov70Work *w){
 if(GtsServer_IsRequestDone()){
  int result=GtsServer_GetResult();w->timer1604=0;
  switch(result){
   case 0:w->state2c=26;break;
   case 1:{int r;w->value36=1;r=ov70_02240D00(w,w->unk13c);
    switch(r){
     case 1:Gts_DeleteWaitingIcon((u8 *)(void *)w);ov70_02244FA4((u8 *)(void *)w,w->ptrBA0,29,1,0xf0f);Gts_SetState((struct Ov70State *)(void *)w,37,28);break;
     case 2:Gts_DeleteWaitingIcon((u8 *)(void *)w);ov70_02244FA4((u8 *)(void *)w,w->ptrBA0,35,1,0xf0f);Gts_SetState((struct Ov70State *)(void *)w,37,28);break;
     case 0:w->state2c=18;w->unk11FC=1;break;
    }break;}
   case -3:{Pokemon *mon;w->value36=0;
    if(sub_0202DB54(w->unk00->unk00)){
     mon=AllocMonZeroed(HEAP_ID_61);sub_0202DB64(w->unk00->unk00,mon);
     BufferBoxMonNickname(w->unkB9C,0,Mon_GetBoxMon(mon));w->unk28=2;w->state2c=34;
     ov70_02240A7C(w,mon,sub_0202DBA0(w->unk00->unk00),0);sub_0202DB5C(w->unk00->unk00,0);Heap_Free(mon);
    }else ov70_022404D4(w);break;}
   case -4:{Pokemon *mon;w->value36=0;
    if(sub_0202DB54(w->unk00->unk00)){
     mon=AllocMonZeroed(HEAP_ID_61);sub_0202DB64(w->unk00->unk00,mon);
     BufferBoxMonNickname(w->unkB9C,0,Mon_GetBoxMon(mon));w->unk28=3;w->state2c=34;
     sub_0202DB5C(w->unk00->unk00,0);Heap_Free(mon);
    }break;}
   case -15:case -14:case -12:case -2:w->value3c=result;w->state2c=38;break;
   case -13:ShowCommunicationError(HEAP_ID_3,1,0);for(;;){};
  }
 }else{w->timer1604++;if(w->timer1604==3600)sub_020399EC();}
 return 3;
}
