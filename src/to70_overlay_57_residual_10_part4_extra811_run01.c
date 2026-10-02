#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct {u8 p[0x3f0];int selection;} State;
extern int ov57_0223B3A4(State *,u8),ov57_0223B478(int *);
int ov57_0223B3F8(State *);
int ov57_0223B3F8(State *s){int selection;if(gSystem.newKeys&2)return ov57_0223B3A4(s,12);if(gSystem.newKeys&1)return ov57_0223B3A4(s,13);if(!(gSystem.newAndRepeatedKeys&240))return 1;selection=s->selection;if(ov57_0223B478(&selection)==0)return 1;return ov57_0223B3A4(s,selection);}
