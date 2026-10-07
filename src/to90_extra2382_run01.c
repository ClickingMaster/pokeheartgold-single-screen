#pragma force_active on
#include "global.h"
#pragma force_active on

extern void GF_3DVramMan_InitFrameTexVramManager(u16,BOOL),GF_3DVramMan_InitFramePlttVramManager(u32,BOOL);
extern void PokepicManager_SetCharBaseAddrAndSize(void*,int,int),PokepicManager_SetPlttBaseAddrAndSize(void*,int,int);
#define R31791 (*(volatile u16*)0x04000060)
typedef struct{u32 gap[3];void*manager;} W31791;
void ov71_02247ED0(W31791*);
void ov71_02247ED0(W31791*w){u32 tex,pltt;NNS_G3dInit();G3X_InitMtxStack();R31791&=~0x3002;R31791=(R31791&~0x3000)|16;R31791&=0xcffb;R31791=(R31791&~0x3000)|8;G3X_SetClearColor(0,0,0x7fff,63,FALSE);*(volatile u32*)0x04000540=2;*(volatile u32*)0x04000580=0xbfff0000;GF_3DVramMan_InitFrameTexVramManager(1,TRUE);GF_3DVramMan_InitFramePlttVramManager(16384,TRUE);tex=NNS_GfdDefaultFuncAllocTexVram(16384,FALSE,0);pltt=NNS_GfdDefaultFuncAllocPlttVram(128,FALSE,1);PokepicManager_SetCharBaseAddrAndSize(w->manager,(tex&0xffff)<<3,((tex&0x7fff0000)>>16)<<4);PokepicManager_SetPlttBaseAddrAndSize(w->manager,(pltt&0xffff)<<3,((pltt&0xffff0000)>>16)<<3);}
