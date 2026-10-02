#pragma force_active on
#include "global.h"
#pragma force_active on
struct Msg{u16 pad,status,kind;u8 rest[12];u16 id;};struct State{u8 pad[24],type;};void ov74_0223144C(int);int ov74_02231064(void);struct State*ov74_02231184(void);void ov74_02230714(struct Msg*);void ov74_02230818(struct Msg*);void ov74_02231124(int);void ov74_02231130(int);int ov74_02231118(void);void ov74_0222FEA0(int);void ov74_022308E0(struct Msg*);
void ov74_022308E0(struct Msg*m){ov74_0223144C(m->kind);if(m->status==0){switch(m->kind){case 21:switch(ov74_02231064()){case 10:case 11:switch(ov74_02231184()->type){case 1:ov74_02230714(m);break;case 2:ov74_02230818(m);break;}break;}break;case 7:ov74_02231124(1);ov74_02231130(0);break;case 9:if(ov74_02231118())ov74_02231130(1);switch(ov74_02231064()){case 10:ov74_0222FEA0(m->id);break;case 11:ov74_02231124(0);break;}break;}}}
