#ifndef OVERLAY07_OBJ_WINDOW_WORK_PRIVATE_H
#define OVERLAY07_OBJ_WINDOW_WORK_PRIVATE_H
#include "global.h"
#include "overlay07_obj_window_init_private.h"
#include "sprite_system.h"
#include "overlay07_script_slot_private.h"

typedef struct Ov07ObjWindowWork {
    Ov07Init init;
    Ov07T10Work prefix;
    ManagedSprite *sprites[3];
    u8 timer;
} Ov07ObjWindowWork;
typedef char Ov07ObjWindowInitSize[sizeof(Ov07Init)==24?1:-1];
typedef char Ov07ObjWindowWorkSize[sizeof(Ov07ObjWindowWork)==68?1:-1];
void ov07_0223174C(void *, const Ov07Init *);
void ov07_022315C0(SysTask *, void *);
#endif
