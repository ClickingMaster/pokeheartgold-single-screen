#include "global.h"

typedef struct GraphicsObject GraphicsObject;
typedef struct RawPoint {
    u16 x;
    u16 y;
} RawPoint;

typedef struct Ov41Obj {
    u32 unk0;
    GraphicsObject *graphicsObject;
} Ov41Obj;

extern RawPoint GraphicsObject_GetPosition(GraphicsObject *object);
extern RawPoint GraphicsObject_GetSize(GraphicsObject *object);

void ov41_02245FA8(Ov41Obj *object, s32 *x, s32 *y);
void ov41_02245FD8(Ov41Obj *object, s32 *x, s32 *y);

void ov41_02245FA8(Ov41Obj *object, s32 *x, s32 *y) {
    RawPoint point;
    point = GraphicsObject_GetPosition(object->graphicsObject);
    *x = (s16)point.x;
    *y = (s16)point.y;
}

void ov41_02245FD8(Ov41Obj *object, s32 *x, s32 *y) {
    RawPoint point;
    point = GraphicsObject_GetSize(object->graphicsObject);
    *x = (s16)point.x;
    *y = (s16)point.y;
}
