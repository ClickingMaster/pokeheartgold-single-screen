#pragma force_active on
#include "global.h"
#include "follow_mon.h"
#include "field_system.h"
#include "map_object.h"
#include "party.h"
#include "pokemon.h"
#include "constants/pokemon.h"
typedef struct FollowObjData {
    Sprite *sprite;
    u8 unk4[0x13];
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
} FollowObjData;

void ov01_021F8E70(LocalMapObject *object, u32 dir, VecFx32 *out);
BOOL MapObject_HasFollowMonSprite(LocalMapObject *object);
void MapObject_SetBillboardSepia(int a0, LocalMapObject *object, Sprite *sprite);
BOOL MapObject_IsShinyPokemon(LocalMapObject *object);
void ov01_02205808(int a0, LocalMapObject *object, Sprite *sprite);
void ov01_021F9630(Sprite *sprite, void *a1);
int ov01_021FA3E8(LocalMapObject *object, Sprite *sprite);
BOOL ov01_021FA2D4(LocalMapObject *object);
void ov01_021F9510(LocalMapObject *object, FollowObjData *data);
void sub_0205FCD4(LocalMapObject *object);
void MapObject_ClearHeldMovement(LocalMapObject *object);
void sub_0205F484(LocalMapObject *object);
Pokemon *GetFirstAliveMonInParty_CrashIfNone(Party *party);
void ov01_021F7504(LocalMapObject *object);
// @fn ov01_021F7504
void ov01_021F7504(LocalMapObject *object) {
    VecFx32 pos;
    VecFx32 facing;
    FollowObjData *data = (FollowObjData *)sub_0205F40C(object);
    if (!data->bit1 && MapObject_GetID(object) == 0xFD) {
        FieldSystem *fieldSystem = MapObjectManager_GetFieldSystem(MapObject_GetManager(object));
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        int species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        int form = GetMonData(mon, MON_DATA_FORM, NULL);
        u8 gender = GetMonGender(mon);
        u32 spriteId = FollowMon_GetSpriteID(species, form, gender);
        u8 shiny = MonIsShiny(mon);
        int x, y, z;
        data->bit1 = TRUE;
        FollowMon_SetObjectParams(object, species, form, (u32)shiny);
        MapObject_SetSpriteID(object, spriteId);
        sub_0205FCD4(object);
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_START_MOVEMENT);
        MapObject_CopyPositionVector(object, &pos);
        x = MapObject_GetXCoord(object);
        pos.x = FX32_CONST(16) * x + FX32_CONST(8);
        MapObject_SetPreviousX(object, x);
        y = MapObject_GetYCoord(object);
        MapObject_SetPreviousY(object, y);
        z = MapObject_GetZCoord(object);
        pos.z = FX32_CONST(16) * z + FX32_CONST(8);
        MapObject_SetPreviousZ(object, z);
        MapObject_SetPositionVector(object, &pos);
        MapObject_ClearHeldMovement(object);
        if (fieldSystem->followMon.species != species || fieldSystem->followMon.form != form || fieldSystem->followMon.shiny != shiny || fieldSystem->followMon.gender != gender) {
            FieldSystem_SetFollowerPokeParam(fieldSystem, species, form, shiny, gender);
            MapObject_SetFacingDirectionDirect(object, DIR_SOUTH);
        }
        FieldSystem_UnkSub108_Set(fieldSystem->unk108, mon, species, GetMonData(mon, MON_DATA_PERSONALITY, NULL));
    }
    if (ov01_021FA2D4(object) == TRUE) {
        return;
    }
    if (data->sprite == NULL) {
        ov01_021F9510(object, data);
    }
    if (data->sprite != NULL) {
        data->bit0 = TRUE;
        {
        VecFx32 offset = {0, 0, 0};
        MapObject_CopyFacingVector(object, &facing);
        MapObject_GetSpriteID(object);
        ov01_021F8E70(object, MapObject_GetFacingDirection(object), &offset);
        facing.x = offset.x;
        facing.z = offset.z;
        MapObject_SetFacingVector(object, &facing);
        if (MapObject_HasFollowMonSprite(object)) {
            MapObject_SetBillboardSepia(1, object, data->sprite);
        } else if (MapObject_IsShinyPokemon(object)) {
            ov01_02205808(1, object, data->sprite);
        }
        ov01_021F9630(data->sprite, data->unk4);
        ov01_021FA3E8(object, data->sprite);
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
        sub_0205F484(object);
        }
    }
}
