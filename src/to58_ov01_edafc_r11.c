#include "global.h"
typedef struct Bits { unsigned char lo:6; unsigned char a:1; unsigned char b:1; } Bits;
void ov01_021EF018(void *p, int v);
void ov01_021EF034(void *p, int v);
void ov01_021EF018(void *p, int v) { ((Bits *)((u8 *)p + 0x97))->a = (u8)v; }
void ov01_021EF034(void *p, int v) { ((Bits *)((u8 *)p + 0x97))->b = (u8)v; }
