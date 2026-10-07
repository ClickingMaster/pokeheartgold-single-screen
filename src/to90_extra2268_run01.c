#pragma force_active on
#include "global.h"
#pragma force_active on
typedef VecFx32 Vec;
typedef MtxFx33 Matrix;
typedef struct {void*a,*b,*c,*d;u8 pad[16];} Record;typedef struct {u8 header[0x48];Record records[1];} Entry;
extern Vec ov96_0221C748,ov96_0221C754;
extern void ov96_021EB588(void*,Vec*);
void ov96_0220050C(Entry*,int,int);
void ov96_0220050C(Entry*w,int index,int angle){Vec result;Vec first;Vec second;Matrix matrix;int folded=angle%1024;u16 a=folded*65536/1024;a=65536-a;first=ov96_0221C748;second=ov96_0221C754;MTX_RotZ33_(&matrix,FX_SinCosTable_[(a>>4)*2],FX_SinCosTable_[(a>>4)*2+1]);MTX_MultVec33(&first,&matrix,&first);VEC_Add(&second,&first,&result);ov96_021EB588(w->records[index].a,&result);ov96_021EB588(w->records[index].d,&result);ov96_021EB588(w->records[index].b,&result);ov96_021EB588(w->records[index].c,&result);}
