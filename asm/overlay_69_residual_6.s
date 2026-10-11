	.include "asm/macros.inc"
	.public GeonetGlobe_Init
	.public GeonetGlobe_Main
	.public GeonetGlobe_Exit
	.public GeonetGlobe_SetGfxBanks
	.public GeonetGlobe_SetGraphicsModes
	.public GeonetGlobe_LoadLocations
	.public GeonetGlobe_SetLocation
	.public GeonetGlobe_MarkPlayerLocation
	.public GeonetGlobe_GetPlaceIndex
	.public GeonetGlobe_HandleTouch
	.public GeonetGlobe_GetTouchDrag
	.public GeonetGlobe_InitBgs
	.public GeonetGlobe_FreeBgs
	.public GeonetGlobe_PrintMessage
	.public GeonetGlobe_ListMenuMoveCursor
	.public GeonetGlobe_CreateMenu
	.public GeonetGlobe_CreatePlaceList
	.public GeonetGlobe_DestroyListMenu
	.public GeonetGlobe_ShowPlayerLocation
	.public GeonetGlobe_ShowLocation
	.public GeonetGlobe_HideLocationWindow
	.public GeonetGlobe_FindNearestLocation
	.public GeonetGlobe_UpdateLocationName
	.public GeonetGlobe_LoadModels
	.public GeonetGlobe_FreeModels
	.public GeonetGlobe_ResetTransform
	.public GeonetGlobe_InitCamera
	.public GeonetGlobe_InitLight
	.public GeonetGlobe_CalcGlobeRotation
	.public GeonetGlobe_CalcLocationRotation
	.public GeonetGlobe_HandleRotationInput
	.public GeonetGlobe_StepZoom
	.public GeonetGlobe_Draw
	.public GeonetGlobe_GetLocationNames
	.public GeonetGlobe_CountryHasRegions
	.public GeonetGlobe_WrapPosition
	.public GeonetGlobe_CalcDistance
	.public _021E7644
	.public sGeonetLocationWindowTemplate
	.public sGeonetInfoButtonWindowTemplate
	.public sGeonetExitButtonWindowTemplate
	.public sGeonetYesNoWindowTemplate
	.public sGeonetMessageWindowTemplate
	.public sGeonetPlaceListWindowTemplate
	.public sGeonetCameraPosition
	.public sGeonetGraphicsModes
	.public sGeonetMenuEntries
	.public sGeonetBgTemplate_Text
	.public sGeonetBgTemplate_Frame
	.public sGeonetMenuTemplate
	.public sGeonetPlaceListTemplate
	.public sGeonetGlobeRotationInit
	.public sGeonetMarkerRotationInit
	.public sGeonetLocationRotationInit
	.public sGeonetCursorRotation
	.public sGeonetGraphicsBanks
	.include "overlay_69.inc"
	.include "global.inc"

	.text
	.public GeonetGlobe_Exit
	.public GeonetGlobe_GetPlaceIndex
	.public GeonetGlobe_FreeBgs
	.public GeonetGlobe_ListMenuMoveCursor
	.public GeonetGlobe_DestroyListMenu
	.public GeonetGlobe_HideLocationWindow
	.public GeonetGlobe_FreeModels
	.public GeonetGlobe_ResetTransform
	.public GeonetGlobe_InitLight
	.public GeonetGlobe_CountryHasRegions
	.public GeonetGlobe_CalcDistance
	.public functions
	.public module
	.public totals

	thumb_func_start GeonetGlobe_ShowPlayerLocation
GeonetGlobe_ShowPlayerLocation: ; 0x021E6A8C
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #1
	ldr r1, [r5]
	lsl r0, r0, #0xa
	bl String_New
	add r4, r0, #0
	mov r0, #1
	ldr r1, [r5]
	lsl r0, r0, #0xa
	bl String_New
	ldr r1, _021E6B40 ; =0x0000C010
	add r6, r0, #0
	ldr r0, [r5, r1]
	add r1, #0x44
	ldr r2, _021E6B44 ; =sGeonetLocationWindowTemplate
	add r1, r5, r1
	bl AddWindow
	mov r0, #0xd8
	str r0, [sp]
	mov r0, #0x30
	str r0, [sp, #4]
	ldr r0, _021E6B48 ; =0x0000C054
	mov r2, #0
	add r0, r5, r0
	mov r1, #0xf
	add r3, r2, #0
	bl FillWindowPixelRect
	ldr r0, _021E6B48 ; =0x0000C054
	ldr r2, _021E6B4C ; =0x000001D9
	add r0, r5, r0
	mov r1, #0
	mov r3, #7
	bl DrawFrameAndWindow1
	ldr r0, _021E6B50 ; =0x0000C080
	ldr r2, _021E6B54 ; =0x0000C324
	ldr r0, [r5, r0]
	ldr r2, [r5, r2]
	mov r1, #0
	bl BufferCountryName
	ldr r3, _021E6B54 ; =0x0000C324
	ldr r0, _021E6B50 ; =0x0000C080
	ldr r2, [r5, r3]
	add r3, r3, #4
	ldr r0, [r5, r0]
	ldr r3, [r5, r3]
	mov r1, #1
	bl BufferCityName
	ldr r0, _021E6B58 ; =0x0000C070
	mov r1, #0xe
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl ReadMsgDataIntoString
	ldr r0, _021E6B50 ; =0x0000C080
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl StringExpandPlaceholders
	mov r1, #0
	ldr r0, _021E6B48 ; =0x0000C054
	str r1, [sp]
	str r1, [sp, #4]
	add r0, r5, r0
	add r2, r4, #0
	add r3, r1, #0
	str r1, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r6, #0
	bl String_Delete
	add r0, r4, #0
	bl String_Delete
	ldr r0, _021E6B48 ; =0x0000C054
	add r0, r5, r0
	bl CopyWindowToVram
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_021E6B40: .word 0x0000C010
_021E6B44: .word sGeonetLocationWindowTemplate
_021E6B48: .word 0x0000C054
_021E6B4C: .word 0x000001D9
_021E6B50: .word 0x0000C080
_021E6B54: .word 0x0000C324
_021E6B58: .word 0x0000C070
	thumb_func_end GeonetGlobe_ShowPlayerLocation




	thumb_func_start GeonetGlobe_ShowLocation
GeonetGlobe_ShowLocation: ; 0x021E6B5C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	str r1, [sp, #0xc]
	ldr r1, [r5]
	mov r0, #0x40
	add r4, r2, #0
	bl String_New
	add r7, r0, #0
	ldr r1, [r5]
	mov r0, #0x40
	bl String_New
	ldr r1, _021E6C04 ; =0x0000C010
	add r6, r0, #0
	ldr r0, [r5, r1]
	add r1, #0x44
	ldr r2, _021E6C08 ; =sGeonetLocationWindowTemplate
	add r1, r5, r1
	bl AddWindow
	mov r0, #0xd8
	str r0, [sp]
	mov r0, #0x30
	str r0, [sp, #4]
	ldr r0, _021E6C0C ; =0x0000C054
	mov r2, #0
	add r0, r5, r0
	mov r1, #0xf
	add r3, r2, #0
	bl FillWindowPixelRect
	ldr r0, _021E6C0C ; =0x0000C054
	ldr r2, _021E6C10 ; =0x000001D9
	add r0, r5, r0
	mov r1, #0
	mov r3, #7
	bl DrawFrameAndWindow1
	ldr r0, [r5]
	add r1, r4, #0
	str r0, [sp]
	ldr r0, [sp, #0xc]
	add r2, r7, #0
	add r3, r6, #0
	bl GeonetGlobe_GetLocationNames
	cmp r4, #0
	beq _021E6BD8
	mov r0, #0x10
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	mov r1, #0
	ldr r0, _021E6C0C ; =0x0000C054
	add r2, r6, #0
	add r0, r5, r0
	add r3, r1, #0
	str r1, [sp, #8]
	bl AddTextPrinterParameterized
_021E6BD8:
	mov r1, #0
	ldr r0, _021E6C0C ; =0x0000C054
	str r1, [sp]
	str r1, [sp, #4]
	add r0, r5, r0
	add r2, r7, #0
	add r3, r1, #0
	str r1, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r6, #0
	bl String_Delete
	add r0, r7, #0
	bl String_Delete
	ldr r0, _021E6C0C ; =0x0000C054
	add r0, r5, r0
	bl CopyWindowToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E6C04: .word 0x0000C010
_021E6C08: .word sGeonetLocationWindowTemplate
_021E6C0C: .word 0x0000C054
_021E6C10: .word 0x000001D9
	thumb_func_end GeonetGlobe_ShowLocation
