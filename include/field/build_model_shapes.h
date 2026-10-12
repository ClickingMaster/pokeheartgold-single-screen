#ifndef POKEHEARTGOLD_FIELD_BUILD_MODEL_SHAPES_H
#define POKEHEARTGOLD_FIELD_BUILD_MODEL_SHAPES_H

#include "field/area_data.h"

// An area's build model shape list: for each build model, how many of its shapes to draw one by one and where they
// start in the shape list; each shape entry is {material id, shape id}. A model with no shapes is drawn whole.

BuildModelShapes *BuildModelShapes_New(void);
void BuildModelShapes_Load(const char *path, BuildModelShapes *shapes);
void BuildModelShapes_Free(BuildModelShapes *shapes);
void BuildModelShapes_GetShapeCount(int buildModel, BuildModelShapes *shapes, u16 *count);
void BuildModelShapes_GetShapeRange(int buildModel, BuildModelShapes *shapes, u16 *count, u16 *start);
u16 (*BuildModelShapes_GetShapes(int start, BuildModelShapes *shapes))[2];

#endif // POKEHEARTGOLD_FIELD_BUILD_MODEL_SHAPES_H
