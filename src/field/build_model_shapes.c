#include "global.h"

#include "field/build_model_shapes.h"

#include "filesystem.h"
#include "heap.h"

struct BuildModelShapes {
    u16 (*models)[2]; // {shape count, first shape} per build model
    u16 (*shapes)[2]; // {material id, shape id}
};

BuildModelShapes *BuildModelShapes_New(void) {
    BuildModelShapes *shapes = Heap_Alloc(HEAP_ID_FIELD1, sizeof(BuildModelShapes));
    if (shapes == NULL) {
        GF_ASSERT(FALSE);
    }
    return shapes;
}

// The file: u16 model count, u16 shape count, the model entries, the shape entries.
void BuildModelShapes_Load(const char *path, BuildModelShapes *shapes) {
    u16 counts[2];
    FSFile file;
    u32 modelsSize;
    u32 shapesSize;
    FS_InitFile(&file);
    if (FS_OpenFile(&file, path)) {
        if (FS_ReadFile(&file, &counts[1], sizeof(u16)) < 0) {
            GF_ASSERT(FALSE);
        }
        if (FS_ReadFile(&file, &counts[0], sizeof(u16)) < 0) {
            GF_ASSERT(FALSE);
        }
        modelsSize = counts[1] * sizeof(*shapes->models);
        shapesSize = counts[0] * sizeof(*shapes->shapes);
        shapes->models = Heap_Alloc(HEAP_ID_FIELD1, modelsSize);
        if (shapes->models == NULL) {
            GF_ASSERT(FALSE);
        }
        shapes->shapes = Heap_Alloc(HEAP_ID_FIELD1, shapesSize);
        if (shapes->shapes == NULL) {
            GF_ASSERT(FALSE);
        }
        if (FS_ReadFile(&file, shapes->models, modelsSize) < 0) {
            GF_ASSERT(FALSE);
        }
        if (FS_ReadFile(&file, shapes->shapes, shapesSize) < 0) {
            GF_ASSERT(FALSE);
        }
        FS_CloseFile(&file);
        return;
    }
    GF_ASSERT(FALSE);
}

void BuildModelShapes_Free(BuildModelShapes *shapes) {
    Heap_Free(shapes->shapes);
    Heap_Free(shapes->models);
    Heap_Free(shapes);
}

void BuildModelShapes_GetShapeCount(int buildModel, BuildModelShapes *shapes, u16 *count) {
    *count = shapes->models[buildModel][0];
}

void BuildModelShapes_GetShapeRange(int buildModel, BuildModelShapes *shapes, u16 *count, u16 *start) {
    *count = shapes->models[buildModel][0];
    *start = shapes->models[buildModel][1];
}

u16 (*BuildModelShapes_GetShapes(int start, BuildModelShapes *shapes))[2] {
    return &shapes->shapes[start];
}
