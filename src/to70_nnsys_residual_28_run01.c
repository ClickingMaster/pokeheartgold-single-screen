#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Matrix {s32 _00,_01,_10,_11,_20,_21;} Matrix;
void NNSi_G2dMtxConcat32(const Matrix*,const Matrix*,Matrix*);
void NNSi_G2dMtxConcat32(const Matrix *a,const Matrix *b,Matrix *out){Matrix temp;Matrix *dst=out;if(out==b)dst=&temp;dst->_00=(s32)(((s64)a->_00*b->_00+(s64)a->_01*b->_10)>>12);dst->_01=(s32)(((s64)a->_00*b->_01+(s64)a->_01*b->_11)>>12);dst->_10=(s32)(((s64)a->_10*b->_00+(s64)a->_11*b->_10)>>12);dst->_11=(s32)(((s64)a->_10*b->_01+(s64)a->_11*b->_11)>>12);dst->_20=b->_20+(s32)(((s64)a->_20*b->_00+(s64)a->_21*b->_10)>>12);dst->_21=b->_21+(s32)(((s64)a->_20*b->_01+(s64)a->_21*b->_11)>>12);if(dst==&temp)*out=temp;}
