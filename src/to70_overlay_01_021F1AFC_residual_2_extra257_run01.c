#pragma force_active on
#include "global.h"
#pragma force_active on
struct E{u16 map,x,z,id,flag;};extern const struct E ov01_02206A14[];int MapObject_GetMapID(void*);int MapObject_GetID(void*);int MapObject_GetXCoord(void*);int MapObject_GetZCoord(void*);int MapObject_GetEventFlag(void*);void MapObject_Delete(void*);void FieldSystem_FlagSet(void*,u16);void FieldSystem_FlagClear(void*,u16);int ov01_021F1F8C(void*,void*);
int ov01_021F1F8C(void*f,void*m){u32 i;for(i=0;i<4;i++){if(ov01_02206A14[i].map==MapObject_GetMapID(m)&&ov01_02206A14[i].id==MapObject_GetID(m)&&ov01_02206A14[i].x==MapObject_GetXCoord(m)&&ov01_02206A14[i].z==MapObject_GetZCoord(m)){FieldSystem_FlagSet(f,(u16)MapObject_GetEventFlag(m));MapObject_Delete(m);{const struct E*entry=ov01_02206A14+i;FieldSystem_FlagClear(f,entry->flag);}return 1;}}return 0;}
