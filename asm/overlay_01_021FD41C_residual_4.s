	.include "asm/macros.inc"
	.include "overlay_01_021FD41C.inc"
	.include "global.inc"

	.text
	.public ov01_02208E90
	.public ov01_02208EA0
	.public ov01_02208EB4
	.public ov01_02208EC8
	.public ov01_02208EDC
	.public ov01_02208EF0
	.public ov01_02208F14
	.public ov01_02208F38
	.public FieldShadow_New
	.public FieldShadow_Free
	.public FieldShadow_StartTask
	.public FieldShadow_StopTask
	.public FieldShadow_GetScale
	.public FieldShadow_FxToAlpha
	.public FieldShadow_SetTimeOfDayParams
	.public FieldShadow_StepTowards
	.public FieldShadow_UpdateTask
	.public FieldShadow_LoadModels
	.public FieldShadow_FreeModels
	.public FieldShadow_SetAlpha
	.public FieldShadow_CreateMapObjectShadow
	.public ov01_021FD684
	.public FieldShadowEffect_Init
	.public FieldShadowEffect_Destroy
	.public FieldShadowEffect_Update
	.public ov01_021FD784
	.public FieldShadowEffect_Draw
	.public ov01_021FD838
	.public ov01_021FD8E8
	.public ov01_021FD92C
	.public ov01_021FD980
	.public FieldShadow_OffsetByFacing

	.public FieldShadow_New
	.public FieldShadow_Free
	.public FieldShadow_StopTask
	.public FieldShadow_GetScale
	.public FieldShadow_FxToAlpha
	.public FieldShadow_SetTimeOfDayParams
	.public FieldShadow_StepTowards
	.public FieldShadow_UpdateTask
	.public FieldShadow_LoadModels
	.public FieldShadow_FreeModels
	.public FieldShadow_SetAlpha
	.public FieldShadow_CreateMapObjectShadow
	.public ov01_021FD684
	.public FieldShadowEffect_Init
	.public FieldShadowEffect_Destroy
	.public FieldShadowEffect_Update
	.public ov01_021FD784
	.public FieldShadowEffect_Draw
	.public ov01_021FD8E8
	.public ov01_021FD92C
	.public FieldShadow_OffsetByFacing
	.public ov01_02208E90
	.public ov01_02208EA0
	.public ov01_02208EB4
	.public ov01_02208EC8
	.public ov01_02208EDC
	.public ov01_02208EF0
	.public ov01_02208F14
	.public ov01_02208F38

	.rodata

ov01_02208E90: ; 0x02208E90
	.word 0x1F
	.word 0x21
	.word 0x22
	.word 0x20

ov01_02208EA0: ; 0x02208EA0
	.word 0x00000020
	.word FieldShadowEffect_Init
	.word FieldShadowEffect_Destroy
	.word FieldShadowEffect_Update
	.word FieldShadowEffect_Draw

ov01_02208EB4: ; 0x02208EB4
	.byte 0x00, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00
	.byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00

ov01_02208EC8: ; 0x02208EC8
	.word 0x00000020
	.word ov01_021FD784
	.word FieldShadowEffect_Destroy
	.word FieldShadowEffect_Update
	.word ov01_021FD838

ov01_02208EDC: ; 0x02208EDC
	.word 0x00000020
	.word FieldShadowEffect_Init
	.word FieldShadowEffect_Destroy
	.word ov01_021FD92C
	.word ov01_021FD980

ov01_02208EF0: ; 0x02208EF0
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00

ov01_02208F14: ; 0x02208F14
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00

ov01_02208F38: ; 0x02208F38
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00
	.byte 0x00, 0x14, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x0E, 0x00, 0x00
