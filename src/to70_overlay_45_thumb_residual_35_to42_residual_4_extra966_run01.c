#pragma force_active on
#include "global.h"
extern u8 *_022577C0;
extern void ov45_0223040C(void *,const void *,u32),ov45_0223048C(void *,const void *);
void ov45_022300B0(int,const void *,u32),ov45_022300DC(int,const void *);
void ov45_022300B0(int failed,const void *data,u32 size){if(failed==0)ov45_0223040C(_022577C0+0x6a0,data,size);else _022577C0[0x132]=1;}
void ov45_022300DC(int failed,const void *data){if(failed==0)ov45_0223048C(_022577C0+0x6a8,data);else _022577C0[0x132]=1;}
