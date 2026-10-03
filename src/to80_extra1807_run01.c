#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void ObjCharTransfer_InitEx(int,int,int),ObjPlttTransfer_Init(int,int),ObjCharTransfer_ClearBuffers(void),ObjPlttTransfer_Reset(void);void ov96_021E92B0(int,int,int,int,int);
void ov96_021E92B0(int a,int b,int c,int d,int e){ObjCharTransfer_InitEx(a,d,e);ObjPlttTransfer_Init(b,c);ObjCharTransfer_ClearBuffers();ObjPlttTransfer_Reset();}
