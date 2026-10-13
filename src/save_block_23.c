#include "global.h"

#include "save.h"
#include "unk_0202E41C.h"

u32 SaveBlock23_sizeof(void);
void SaveBlock23_Init(UnkStruct_0202E474 *data);
void SaveBlock23_ClearArraysAndReset(UnkStruct_0202E474 *data);

void SaveBlock23_SetByte8(UnkStruct_0202E474 *data, u8 value);
void SaveBlock23_ClearHeaderBytes(UnkStruct_0202E474 *data);
void SaveBlock23_Reset(UnkStruct_0202E474 *data);

u32 SaveBlock23_sizeof(void) {
    return sizeof(UnkStruct_0202E474);
}
void SaveBlock23_Init(UnkStruct_0202E474 *data) {
    MI_CpuClearFast(data, sizeof(*data));
    SaveSubstruct_UpdateCRC(23);
}
void SaveBlock23_ClearArraysAndReset(UnkStruct_0202E474 *data) {
    MI_CpuClearFast(data->unk_00C, sizeof(data->unk_00C));
    MI_CpuClearFast(data->unk_0CC, sizeof(data->unk_0CC));
    MI_CpuClearFast(data->unk_18C, sizeof(data->unk_18C));
    SaveBlock23_Reset(data);
    SaveSubstruct_UpdateCRC(23);
}
void SaveBlock23_Reset(UnkStruct_0202E474 *data) {
    SaveBlock23_SetByte8(data, 0);
    SaveBlock23_ClearHeaderBytes(data);
    SaveSubstruct_UpdateCRC(23);
}
void SaveBlock23_SetByte8(UnkStruct_0202E474 *data, u8 value) {
    data->filler_000[8] = value;
    SaveSubstruct_UpdateCRC(23);
}
void SaveBlock23_ClearHeaderBytes(UnkStruct_0202E474 *data) {
    int i;
    for (i = 0; i < 4; i++) {
        data->filler_000[i] = 0;
    }
    SaveSubstruct_UpdateCRC(23);
}
