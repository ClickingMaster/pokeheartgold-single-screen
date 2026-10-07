#ifndef W19_PARTICLE_MOTION_OWNER_H
#define W19_PARTICLE_MOTION_OWNER_H
#include "global.h"
#include "library/spl_emitter.h"
#include "overlay07_script_slot_private.h"
typedef struct W02LinearMotion { s16 x,y; int duration,dx,dy,curX,curY; } W02LinearMotion;
typedef struct W02SinMotion { s16 x,y; int duration,angleX,ampX,angleY,ampY,stepX,stepY; } W02SinMotion;
typedef struct W02ScalarMotion { int value,duration,step,current; } W02ScalarMotion;
typedef struct Ov07LinearSlot { W02LinearMotion motion; u8 reserved18[12]; } Ov07LinearSlot;
typedef struct Ov07SinSlot { W02SinMotion motion; u8 reserved20[4]; } Ov07SinSlot;
typedef struct Ov07ScalarSlot { W02ScalarMotion motion; u8 reserved10[4]; } Ov07ScalarSlot;
typedef struct Ov07Work104 {
 s32 arg0; s16 delay; u16 arg6; s32 arg8; s16 argC,argE; s32 arg10;
 u32 handleA,handleB; s32 unk1C,unk20,unk24,counter,angle,wave;
 void *resource; SPLEmitter *node; Ov07T10Work header;
 u8 reserved58[0x50]; Ov07LinearSlot interpA; Ov07SinSlot interpB; Ov07ScalarSlot interpC;
} Ov07Work104;
typedef char Ov07Work104Size[(sizeof(Ov07Work104)==0x104)?1:-1];
#endif
