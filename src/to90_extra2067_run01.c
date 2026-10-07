#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x58];fx32 x,y;u8 pad60[0x3d];u8 pairFlags;u8 pad9e[8];u8 flagA,flagB;u8 pada8[2];u8 category,singleFlag;u16 angle;u8 padae[2];u8 size,form;u8 padb2[6];} Actor;
typedef struct {u32 value;u8 pad[16];} Score;
typedef struct {u8 pad[0x24];Actor actor[4];u8 pad304[0x208];u16 extra;u8 highBit;u8 pad50f[10];u8 otherFlag;u8 pad51a;u8 flagA;u16 secondY;u16 secondX;u16 firstY;u16 firstX;u8 pad524[0x4c];Score scores[5];u8 pad5d4[0xe8];u8 switches[4];} Model;
typedef struct {u32 coords[4],first,second;u16 attributes[5],extra;} Packed;
void ov96_02205AFC(Model*,Packed*);
void ov96_02205AFC(Model *m,Packed *out){
    u8 pair=0,single=0,enabled=0;int i,j;Actor *a;u8 *coords;u16 *attribute;
    for(i=0;i<5;i++)out->attributes[i]=m->scores[i].value;
    a=m->actor;coords=(u8*)out;attribute=(u16*)out;
    for(i=0,j=0;i<4;i++,j+=2,a++){
        *(u32*)coords=(a->x/4096)|((a->y/4096)<<9)|(a->angle<<18)|(a->category<<28);
        attribute[12] |=a->form<<5;
        attribute[12] |=(a->size-1)<<7;
        attribute[12] |=a->flagA<<9;
        attribute[12] |=a->flagB<<11;
        coords+=4;attribute++;
        single|=a->singleFlag<<i;
        pair|=a->pairFlags<<j;
        enabled|=m->switches[i]<<i;
    }
    out->second=0;out->second|=m->secondX;out->second|=m->secondY<<9;out->second|=m->otherFlag<<18;out->second|=enabled<<19;
    out->first=0;out->first|=m->firstX;out->first|=m->firstY<<9;out->first|=m->flagA<<18;out->first|=single<<19;out->first|=pair<<23;out->first|=m->highBit<<31;
    out->extra=m->extra;
}
