#include "communication_player_movement_internal.h"
#include "to50_comm_player_force_position_internal.h"

void CommPlayerManager_ForcePosition(void) {
    int i;
    if (_021D41C4 == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        void *object;
        int x;
        int z;
        int facing;

        if (((CommPlayerManagerLayout *)_021D41C4)->avatars[i] != NULL && i != sub_0203769C()) {
            object = PlayerAvatar_GetMapObject(((CommPlayerManagerLayout *)_021D41C4)->avatars[i]);
            x = ((CommPlayerPositionLayout *)_021D41C4)->remote[i].x;
            z = ((CommPlayerPositionLayout *)_021D41C4)->remote[i].z;
            facing = ((CommPlayerPositionLayout *)_021D41C4)->remote[i].facing;
            MapObject_SetCurrentX(object, x);
            MapObject_SetCurrentZ(object, z);
            MapObject_SetPositionFromXYZAndDirection(object, x, 0, z, facing);
        }
    }
}
