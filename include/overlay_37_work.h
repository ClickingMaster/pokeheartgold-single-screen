#ifndef OVERLAY_37_WORK_H
#define OVERLAY_37_WORK_H
#include "global.h"
#include <stddef.h>
typedef struct Ov37Window {u8 dummy[0x10];} Ov37Window;
typedef struct Ov37SpriteHolder {u8 unk_00[0x20];void *unk_20;} Ov37SpriteHolder;
typedef struct Ov37Pair {u32 cur,prev;} Ov37Pair;
typedef struct Ov37Slot {u16 unk_00,unk_02;} Ov37Slot;
typedef struct Ov37Rec10 {u8 flags,pad[9];} Ov37Rec10;
typedef struct Ov37Entry {u8 xs[4],ys[4];u8 unk_08_0:3;u8 count:3;u8 unk_08_6:2;u8 unk_09;} Ov37Entry;
typedef struct Ov37Point {u8 x,y;u16 count;} Ov37Point;
typedef struct Ov37Work {
 void *bgConfig;
 u8 unk_0004[4];void *args,*msgFormat,*msgData;void *strbuf[5];void *unk_0028,*unk_002C;u32 printerId;void *spriteList;
 u8 unk_0038[0x160-0x38];void *resObjMan[4],*charTask0,*plttTask0;
 u8 unk_0178[8];void *charTask1,*plttTask1;
 u8 unk_0188[0x1d8-0x188];void *sprites[5];
 u8 unk_01EC[0x248-0x1ec];Ov37SpriteHolder holder;
 u8 unk_026C[0x278-0x26c];Ov37Window windows[5],winB2,winB6,winBA;
 u8 unk_02F8[8];u32 curState,prevState,state;u8 unk_030C[4];u32 counter,unk_0314,unk_0318,unk_031C,unk_0320;
 u8 unk_0324[16];Ov37Pair pair[5];
 u8 unk_035C[0x4374-0x35c];u16 unk_4374;u8 unk_4376,unk_4377;
 u8 unk_4378[8];u8 flags,unk_4381;
 union {u8 unk_4382[50];Ov37Entry entries[5];};
 union {Ov37Slot unk_43B4[5];Ov37Point points[5];};
 void *unk_43C8;u8 unk_43CC[0x93b0-0x43cc];u32 unk_93B0,unk_93B4;
 u16 player;union {u16 unk_93BA;s16 networkCounter;};u8 unk_93BC;u8 unk_93BD[0x93f0-0x93bd];
 void *yesNoPrompt;u32 unk_93F4;union {u32 unk_93F8;u32 yesNoActive;};
 u32 unk_93FC,unk_9400,unk_9404,unk_9408;
} Ov37Work;
typedef struct Ov37StateEntry {u32 (*main)(Ov37Work *,u32);u32 unk_04;} Ov37StateEntry;
typedef char Ov37WorkSize[(sizeof(Ov37Work)==0x940c)?1:-1];
typedef char Ov37EntrySize[(sizeof(Ov37Entry)==10)?1:-1];
typedef char Ov37PointSize[(sizeof(Ov37Point)==4)?1:-1];
typedef char Ov37Offset_args[(offsetof(Ov37Work,args)==0x8)?1:-1];
typedef char Ov37Offset_spriteList[(offsetof(Ov37Work,spriteList)==0x34)?1:-1];
typedef char Ov37Offset_resObjMan[(offsetof(Ov37Work,resObjMan)==0x160)?1:-1];
typedef char Ov37Offset_sprites[(offsetof(Ov37Work,sprites)==0x1d8)?1:-1];
typedef char Ov37Offset_holder[(offsetof(Ov37Work,holder)==0x248)?1:-1];
typedef char Ov37Offset_windows[(offsetof(Ov37Work,windows)==0x278)?1:-1];
typedef char Ov37Offset_winB2[(offsetof(Ov37Work,winB2)==0x2c8)?1:-1];
typedef char Ov37Offset_curState[(offsetof(Ov37Work,curState)==0x300)?1:-1];
typedef char Ov37Offset_prevState[(offsetof(Ov37Work,prevState)==0x304)?1:-1];
typedef char Ov37Offset_state[(offsetof(Ov37Work,state)==0x308)?1:-1];
typedef char Ov37Offset_counter[(offsetof(Ov37Work,counter)==0x310)?1:-1];
typedef char Ov37Offset_pair[(offsetof(Ov37Work,pair)==0x334)?1:-1];
typedef char Ov37Offset_unk_4374[(offsetof(Ov37Work,unk_4374)==0x4374)?1:-1];
typedef char Ov37Offset_flags[(offsetof(Ov37Work,flags)==0x4380)?1:-1];
typedef char Ov37Offset_entries[(offsetof(Ov37Work,entries)==0x4382)?1:-1];
typedef char Ov37Offset_unk_4382[(offsetof(Ov37Work,unk_4382)==0x4382)?1:-1];
typedef char Ov37Offset_unk_43B4[(offsetof(Ov37Work,unk_43B4)==0x43b4)?1:-1];
typedef char Ov37Offset_points[(offsetof(Ov37Work,points)==0x43b4)?1:-1];
typedef char Ov37Offset_unk_93B4[(offsetof(Ov37Work,unk_93B4)==0x93b4)?1:-1];
typedef char Ov37Offset_player[(offsetof(Ov37Work,player)==0x93b8)?1:-1];
typedef char Ov37Offset_unk_93BA[(offsetof(Ov37Work,unk_93BA)==0x93ba)?1:-1];
typedef char Ov37Offset_networkCounter[(offsetof(Ov37Work,networkCounter)==0x93ba)?1:-1];
typedef char Ov37Offset_yesNoPrompt[(offsetof(Ov37Work,yesNoPrompt)==0x93f0)?1:-1];
typedef char Ov37Offset_unk_93F8[(offsetof(Ov37Work,unk_93F8)==0x93f8)?1:-1];
typedef char Ov37Offset_yesNoActive[(offsetof(Ov37Work,yesNoActive)==0x93f8)?1:-1];
typedef char Ov37Offset_unk_9408[(offsetof(Ov37Work,unk_9408)==0x9408)?1:-1];
#endif
