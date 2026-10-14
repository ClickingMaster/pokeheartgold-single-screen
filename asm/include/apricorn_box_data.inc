#include <nitro/fs/overlay.h>
#pragma once
.public CopyU16StringArrayN
.public StringNotEqual
.public StringFillEOS
.public NewMsgDataFromNarc
.public DestroyMsgData
.public ReadMsgDataIntoString
.public NewString_ReadMsgData
.public Heap_AllocAtEnd
.public GF_AssertFail
.public String_New
.public CopyU16ArrayToString
.public SaveArray_Get
.public PlayerProfile_GetNamePtr
.public PlayerProfile_GetTrainerID
.public PlayerProfile_GetTrainerGender
.public PlayerProfile_GetVersion
.public PlayerProfile_GetLanguage
.public Save_ApricornBox_sizeof
.public ApricornJuiceRecord_sizeof
.public Save_ApricornBox_Get
.public Save_ApricornBox_Init
.public ApricornBox_GiveApricorn
.public ApricornBox_CountApricorn
.public ApricornBox_SetKurtApricorn
.public ApricornBox_GetKurtQuantity
.public ApricornBox_GetKurtBall
.public ApricornBox_SetMixState
.public ApricornBox_GetMixState
.public ApricornBox_CountQueuedApricorns
.public ApricornBox_QueueApricorn
.public ApricornBox_GetMixProgressLevel
.public ApricornBox_AddJuiceMildnessForDays
.public ApricornBox_GetJuiceServings
.public ApricornBox_GetJuice
.public ApricornJuice_GetFlavorTotal
.public ApricornJuice_RankFlavors
.public ApricornJuice_GetTopMixIndex
.public ApricornJuice_GetFlavorNameIndex
.public ApricornBox_UseJuiceServing
.public ApricornBox_UpdateSteps
.public ApricornRecord_New
.public ApricornBox_StoreJuiceRecords
.public ApricornBox_GetJuiceName
.public ApricornBox_GetJuiceFlavorName
.public MI_CpuFill8
.public _u32_div_f
.public InitApricornBox
.public ApricornBox_ClearJuiceRecords
.public ClearApricornJuice
.public ApricornBox_TakeApricorn

.public ApricornJuice_IsValid

.public ApricornRecord_IsSameTrainer

.public ApricornBox_AddJuiceMildness

.public ApricornBox_MixQueuedApricorns

.public ApricornJuice_GetFlavors

.public GetJuiceFlavorLevel

.public PackJuiceFlavor

.public ClearApricornJuiceRecord
.public gApricornFlavorDeltas
.public gApricornJuicePresets
