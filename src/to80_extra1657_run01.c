#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 pad[2]; int mode; u32 gap; int selector; } Obj;
typedef struct { u32 pad[14]; int valid; } Record;
extern u8 *_022577C4;
Record *ov45_0223DEC4(void *,int); void ov45_0223B1C8(void *);
static inline BOOL RecordValid(Record *r) { return r && r->valid!=0; }
int ov45_0223454C(Obj *,int,int,void *,int);
int ov45_0223454C(Obj *obj,int id,int flag,void *payload,int message) {
 Record *r; BOOL valid;
 if(flag) return 1;
 switch(obj->selector) {
 case 1:
 if(id==*(int*)(_022577C4+0x4c)) break;
 r=ov45_0223DEC4(_022577C4+0xe8,id); valid=RecordValid(r);
 switch(valid) { case 0: goto done; default: break; }
 if(obj->mode==1 && message==0x44) (*(void(**)(int,void*))(_022577C4+0x28))(id,payload);
 break;
 case 2:
 if(id==*(int*)(_022577C4+0x4c)) break;
 r=ov45_0223DEC4(_022577C4+0xe8,id); valid=RecordValid(r);
 switch(valid) { case 0: goto done; default: break; }
 if(obj->mode==1 && message==0x44) (*(void(**)(int,void*))(_022577C4+0x2c))(id,payload);
 break;
 case 0: if(obj->mode==1 && message==4) ov45_0223B1C8(_022577C4+0x144); break;
 }
done:
 return 1;
}
