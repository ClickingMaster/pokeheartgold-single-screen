#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 row,rows,limit;u8 side,col;int direction;u16 end;} State;
extern u32 ov102_021E8798(State *);extern int ov102_021E8C3C(State *),ov102_021E8C64(State *);
int ov102_021E8B40(State *,u32);
int ov102_021E8B40(State *s,u32 keys){u32 n;if(keys&128){if(s->end)return 4;if(s->col<4){s->col++;n=ov102_021E8798(s);if(n<s->limit)return 1;if(n==s->limit&&(n&1)){s->side=0;return 1;}s->col--;s->end=1;return 1;}else if(s->row<s->rows){s->direction=1;s->row++;n=ov102_021E8798(s);if(n<s->limit)return 2;s->side=0;return 3;}s->end=1;return 1;}else if(keys&64){if(s->end){s->end=0;return 1;}if(s->col){s->col--;return 1;}if(s->row){s->direction=-1;s->row--;return 2;}return 4;}else if(keys&48){s->side^=1;n=ov102_021E8798(s);if(n<s->limit)return 1;s->side^=1;return 4;}else if(keys&512)return ov102_021E8C3C(s);else if(keys&256)return ov102_021E8C64(s);return 0;}
