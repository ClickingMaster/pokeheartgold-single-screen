	.include "asm/macros.inc"
	.include "overlay_92.inc"
	.include "global.inc"

.text
	thumb_func_start ov92_0225E9B4
ov92_0225E9B4: ; 0x0225E9B4
	push {r4, r5, r6, lr}
	sub sp, #0x10
	ldr r5, [r0, #0x58]
	ldr r4, [r0, #0x48]
	ldr r6, [r0, #0x5c]
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x48
	add r2, r5, #0
	mov r3, #4
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x4b
	add r2, r5, #0
	mov r3, #5
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x4a
	add r2, r5, #0
	mov r3, #4
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x4c
	add r2, r5, #0
	mov r3, #5
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x4d
	add r2, r5, #0
	mov r3, #6
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0x4e
	add r2, r5, #0
	mov r3, #7
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #1
	str r0, [sp]
	mov r0, #0xa0
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	add r0, r6, #0
	mov r1, #0xc1
	mov r2, #0x49
	mov r3, #0x71
	bl PaletteData_LoadNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #5
	add r2, r5, #0
	mov r3, #3
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #8
	add r2, r5, #0
	mov r3, #1
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #7
	add r2, r5, #0
	mov r3, #3
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #9
	add r2, r5, #0
	mov r3, #1
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x71
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #0xa
	add r2, r5, #0
	mov r3, #2
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r1, #0
	str r1, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	str r1, [sp, #8]
	add r0, r6, #0
	mov r1, #0xc1
	mov r2, #6
	mov r3, #0x71
	bl PaletteData_LoadNarc
	mov r0, #1
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0xe0
	str r0, [sp, #8]
	add r0, r6, #0
	mov r1, #0x10
	mov r2, #8
	mov r3, #0x71
	bl PaletteData_LoadNarc
	bl sub_0200E3D8
	add r2, r0, #0
	mov r0, #1
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0xd0
	str r0, [sp, #8]
	add r0, r6, #0
	mov r1, #0x26
	mov r3, #0x71
	bl PaletteData_LoadNarc
	mov r0, #0
	str r0, [sp]
	mov r0, #0x71
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #7
	mov r2, #0xb4
	mov r3, #0xd
	bl LoadUserFrameGfx1
	add sp, #0x10
	pop {r4, r5, r6, pc}
	thumb_func_end ov92_0225E9B4
