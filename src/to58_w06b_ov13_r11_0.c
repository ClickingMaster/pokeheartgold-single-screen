#include "global.h"

extern int SOC_Socket(int, int, int);
extern int SOC_Bind(int, void *, int);
extern int SOC_Close(int);

int ov13_02222A00(int a, int b, int c);
int ov13_02222A08(int a, u8 *b, int c);
int ov13_02222A14(int a);
u32 ov13_02222A1C(u32 a);
u16 ov13_02222A44(u16 a);
u32 ov13_02222A5C(u32 a);
u16 ov13_02222A84(u16 a);

int ov13_02222A00(int a, int b, int c) { return SOC_Socket(a, b, c); }
int ov13_02222A08(int a, u8 *b, int c) { *b = c; return SOC_Bind(a, b, c); }
int ov13_02222A14(int a) { return SOC_Close(a); }
