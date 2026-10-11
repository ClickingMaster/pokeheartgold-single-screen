#include "global.h"

void DressUpButton_MoveBy(void *work, int x, int y);

typedef struct { s32 x, y, z; } Ov41Matrix;

const Ov41Matrix *Sprite_GetMatrixPtr(void *);
void Sprite_SetMatrix(void *, const Ov41Matrix *);

void DressUpButton_MoveBy(void *work, int x, int y)
{
    Ov41Matrix matrix = *Sprite_GetMatrixPtr(*(void **)work);
    matrix.x += x << 12;
    matrix.y += y << 12;
    Sprite_SetMatrix(*(void **)work, &matrix);
}
