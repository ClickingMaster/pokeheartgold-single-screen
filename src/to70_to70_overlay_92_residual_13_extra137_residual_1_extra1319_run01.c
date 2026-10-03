#pragma force_active on
#include "global.h"
extern void ov92_022630E8(void*),ov92_02260428(void*,int,int,int,int,float,int);
void ov92_0225EBE0(char*);
void ov92_0225EBE0(char*w) {
 int i; char *a,*b,*c;
 *(int*)(w+0x1fc8)=0; *(int*)(w+0x1fcc)=0x64000; *(int*)(w+0x1fd0)=0; *(int*)(w+0x51c)=0; *(int*)(w+0x520)=0; *(int*)(w+0x310)=0; *(int*)(w+0x314)=0; *(int*)(w+0x518)=0; *(int*)(w+0x30c)=0;
 ov92_022630E8(w+0x2a4); ov92_022630E8(w+0x2b4); ov92_022630E8(w+0x4b0); ov92_022630E8(w+0x4c0); ov92_022630E8(w+0xad4); ov92_022630E8(w+0xae4);
 i=0; a=w+0xce0; b=w+0xcf0; c=w+0xb50;
 do { ov92_022630E8(a); ov92_022630E8(b); ov92_02260428(c,0,0,5,5,0.8f,0); ov92_02260428(c,0,0,-5,-5,0.8f,0); a+=0x20c; b+=0x20c; c+=0x20c; } while(++i<8);
 ov92_02260428(w+0x114,0,0,5,5,1.88f,0); ov92_02260428(w+0x114,0,0,-5,-5,1.88f,0);
 ov92_02260428(w+0x320,0,0,5,5,0.8f,0); ov92_02260428(w+0x320,0,0,-5,-5,0.8f,0);
 ov92_02260428(w+0x944,0,0,5,5,1.88f,0); ov92_02260428(w+0x944,0,0,-5,-5,0.8f,0);
}
