	.extern ov71_022473D0
	.extern ov71_0224744C
	.extern ov71_022474CC
	.extern ov71_02247610
	.extern ov71_022476EC
	.extern ov71_02247ED0
	.extern ov71_02247F9C
	.extern ov71_02247FF8
	.extern ov71_0224BC5C
	.extern ov71_0224BC74
	.extern ov71_0224BC84
	.extern ov71_0224BCA0
	.extern ov71_0224BCBC
	.include "asm/macros.inc"
	.include "overlay_71.inc"
	.include "global.inc"

.text
	thumb_func_start ov71_02247A10
ov71_02247A10: ; 0x02247A10
	push {r3, r4, lr}
	sub sp, #0x1c
	add r4, r0, #0
	ldr r0, _02247C0C ; =ov71_0224BCBC
	bl GfGfx_SetBanks
	ldr r2, _02247C10 ; =0x04000304
	ldr r0, _02247C14 ; =0xFFFF7FFF
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	ldr r0, _02247C18 ; =ov71_0224BC74
	bl SetBothScreensModesAndDisable
	ldr r0, [r4, #0x54]
	ldr r2, _02247C1C ; =ov71_0224BC84
	mov r1, #1
	mov r3, #0
	bl InitBgFromTemplate
	ldr r0, [r4, #0x54]
	ldr r2, _02247C20 ; =ov71_0224BCA0
	mov r1, #2
	mov r3, #0
	bl InitBgFromTemplate
	ldr r0, [r4, #0x54]
	ldr r2, _02247C20 ; =ov71_0224BCA0
	mov r1, #6
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	ldr r2, _02247C24 ; =0x04000008
	mov r0, #3
	ldrh r3, [r2]
	mov r1, #1
	bic r3, r0
	add r0, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r2, #0
	str r2, [sp]
	ldr r0, [r4, #0x54]
	add r3, r1, #0
	bl BG_FillCharDataRange
	mov r2, #0
	str r2, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	str r0, [sp, #8]
	str r2, [sp, #0xc]
	ldr r0, [r4, #0x54]
	mov r1, #1
	add r3, r2, #0
	bl FillBgTilemapRect
	ldr r0, [r4]
	bl ov71_022473D0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x39
	str r0, [sp, #4]
	ldr r0, [r4, #0x54]
	mov r1, #1
	mov r2, #0x6d
	mov r3, #2
	bl LoadUserFrameGfx2
	mov r0, #0x13
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	add r1, r4, #0
	str r0, [sp, #8]
	mov r2, #1
	str r2, [sp, #0xc]
	str r2, [sp, #0x10]
	ldr r0, [r4, #0x54]
	add r1, #0x58
	mov r3, #2
	bl AddWindowParameterized
	add r0, r4, #0
	add r0, #0x58
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r3, #0x20
	str r3, [sp]
	mov r0, #0x39
	str r0, [sp, #4]
	mov r0, #0x10
	mov r1, #8
	mov r2, #0
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x39
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x54]
	mov r0, #0x59
	mov r1, #0x16
	mov r3, #2
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x39
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x54]
	mov r0, #0x59
	mov r1, #0x15
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x39
	mov r2, #0
	str r0, [sp, #4]
	mov r0, #0x59
	mov r1, #0x17
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x39
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x54]
	mov r0, #0x59
	mov r1, #0x16
	mov r3, #6
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x39
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x54]
	mov r0, #0x59
	mov r1, #0x15
	mov r3, #6
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x39
	str r0, [sp, #4]
	mov r0, #0x59
	mov r1, #0x17
	mov r2, #4
	mov r3, #0
	bl GfGfxLoader_GXLoadPal
	ldr r0, [r4, #0x54]
	mov r1, #1
	bl BgCommitTilemapBufferToVram
	add r0, r4, #0
	bl ov71_02247ED0
	mov r1, #0
	mov r0, #1
	add r2, r1, #0
	add r3, r1, #0
	bl ov71_0224744C
	str r0, [r4, #0x70]
	add r0, r4, #0
	bl ov71_02247F9C
	str r0, [r4, #0x10]
	mov r1, #0
	mov r0, #0x6a
	str r1, [sp]
	lsl r0, r0, #0xa
	str r0, [sp, #4]
	ldr r0, _02247C28 ; =0x00073800
	mov r2, #0x59
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4, #0x70]
	mov r3, #0x1b
	bl ov71_02247610
	ldr r2, _02247C2C ; =ov71_0224BC5C
	str r0, [r4, #0x74]
	ldrh r3, [r2]
	add r0, sp, #0x14
	add r1, sp, #0x14
	strh r3, [r0]
	ldrh r3, [r2, #2]
	ldrh r2, [r2, #4]
	strh r3, [r0, #2]
	strh r2, [r0, #4]
	ldr r0, [r4, #0x74]
	bl ov71_022476EC
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	add r0, r4, #0
	bl ov71_02247FF8
	ldr r0, _02247C30 ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	bl sub_0203A880
	mov r0, #1
	mov r1, #0x38
	bl sub_0203A948
	mov r0, #0x10
	str r0, [sp]
	mov r1, #1
	str r1, [sp, #4]
	mov r0, #0x39
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	mov r0, #1
	add sp, #0x1c
	pop {r3, r4, pc}
	.balign 4, 0
_02247C0C: .word ov71_0224BCBC
_02247C10: .word 0x04000304
_02247C14: .word 0xFFFF7FFF
_02247C18: .word ov71_0224BC74
_02247C1C: .word ov71_0224BC84
_02247C20: .word ov71_0224BCA0
_02247C24: .word 0x04000008
_02247C28: .word 0x00073800
_02247C2C: .word ov71_0224BC5C
_02247C30: .word 0x04000050
	thumb_func_end ov71_02247A10
