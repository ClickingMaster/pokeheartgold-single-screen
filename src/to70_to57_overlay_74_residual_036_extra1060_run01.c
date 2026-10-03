#pragma force_active on
#include "global.h"
typedef struct {int a,b,c,heap;} Template;extern Template _0223B710;extern struct {u8 p[8];int heap;} ov74_0223D454;
extern void ObjCharTransfer_InitEx(Template*,int,int);extern void ObjPlttTransfer_Init(int,int);extern void ObjCharTransfer_ClearBuffers(void);extern void ObjPlttTransfer_Reset(void);
void ov74_0223563C(void);
void ov74_0223563C(void) {Template t=_0223B710;t.heap=ov74_0223D454.heap;ObjCharTransfer_InitEx(&t,16,16);ObjPlttTransfer_Init(30,ov74_0223D454.heap);ObjCharTransfer_ClearBuffers();ObjPlttTransfer_Reset();}
