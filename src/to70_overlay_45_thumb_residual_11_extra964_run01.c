#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x3c];u16 value;u8 flag;} W;extern int ov45_0222D79C(int,int);
u16 ov45_0222AA84(W*);u8 ov45_0222AAA8(W*);
u16 ov45_0222AA84(W*w) {if(w->value>=234)return 0;if(ov45_0222D79C(w->value,w->flag)==0)return 0;return w->value;}
u8 ov45_0222AAA8(W*w) {if(ov45_0222D79C(w->value,w->flag)==0)return 0;return w->flag;}
