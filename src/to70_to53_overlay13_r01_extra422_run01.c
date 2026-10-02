#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {u8 pad[4];u16 len;u8 string[256];s16 a,b,c,d,e;u8 pad110[6];u8 error;} State;
extern void *ov13_0224DD80,*ov13_0224DD8C;extern u8 _0224CF98[];extern int ov13_022459B4;
void *ov13_022208E8(u32);void ov13_02221428(void),ov13_022217D0(int),ov13_022208F8(void *),ov13_02222A14(int);int ov13_022209F8(State *),ov13_02220908(State *);
int ov13_02220908(State *s){int valid;if(s->a==0||s->a<-1||s->b<-1||s->c==0||s->c<-1||s->d<-1||s->e<-1||s->len==0||s->len>256||s->string[s->len-1]!=0)valid=-1;else valid=0;if(!ov13_0224DD80||!ov13_0224DD8C)valid=-1;if(valid==-1){s->error=15;ov13_02221428();return -1;}*(void **)(_0224CF98+20)=ov13_022208E8(1528);if(*(void **)(_0224CF98+20)==NULL){s->error=15;ov13_02221428();return -1;}ov13_022217D0(-1);int result=ov13_022209F8(s);ov13_022208F8(*(void **)(_0224CF98+20));ov13_02221428();if(ov13_022459B4!=-1)ov13_02222A14(ov13_022459B4);return result;}
