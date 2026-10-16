#include "save_block_23.h"

#include "global.h"

#include "constants/save_arrays.h"

#include "save.h"

static void SaveBlock23_SetByte8(SaveBlock23 *data, u8 value);
static void SaveBlock23_ClearHeaderBytes(SaveBlock23 *data);

u32 SaveBlock23_sizeof(void);
void SaveBlock23_Init(SaveBlock23 *data);
void SaveBlock23_ClearArraysAndReset(SaveBlock23 *data);

void SaveBlock23_Reset(SaveBlock23 *data);

u32 SaveBlock23_sizeof(void) {
    return sizeof(SaveBlock23);
}
void SaveBlock23_Init(SaveBlock23 *data) {
    MI_CpuClearFast(data, sizeof(*data));
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}
void SaveBlock23_ClearArraysAndReset(SaveBlock23 *data) {
    MI_CpuClearFast(data->list0, sizeof(data->list0));
    MI_CpuClearFast(data->list1, sizeof(data->list1));
    MI_CpuClearFast(data->list2, sizeof(data->list2));
    SaveBlock23_Reset(data);
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}
void SaveBlock23_Reset(SaveBlock23 *data) {
    SaveBlock23_SetByte8(data, 0);
    SaveBlock23_ClearHeaderBytes(data);
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}
static void SaveBlock23_SetByte8(SaveBlock23 *data, u8 value) {
    data->filler_000[8] = value;
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}
static void SaveBlock23_ClearHeaderBytes(SaveBlock23 *data) {
    int i;
    for (i = 0; i < 4; i++) {
        data->filler_000[i] = 0;
    }
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}
