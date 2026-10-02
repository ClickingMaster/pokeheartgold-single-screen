#pragma force_active on
#include "global.h"
extern int CTRDG_IsAgbCartridge(void);extern void CTRDG_TerminateForPulledOut(void);
void ov74_02236010(void);
void ov74_02236034(int enable);
void ov74_02236010(void){if(((*(volatile u16*)0x27fffa8 & 32768)>>15)==0 && !CTRDG_IsAgbCartridge())CTRDG_TerminateForPulledOut();}
void ov74_02236034(int enable){if(enable==1){(void)*(volatile u16*)0x4000208;*(volatile u16*)0x4000208=0;OS_SetIrqFunction(8192,ov74_02236010);OS_EnableIrqMask(8192);(void)*(volatile u16*)0x4000208;*(volatile u16*)0x4000208=1;}else OS_DisableIrqMask(8192);}
