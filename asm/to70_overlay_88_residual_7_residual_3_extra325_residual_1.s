	.extern _02259904
	.extern ov88_02259508
	.include "asm/macros.inc"
	.include "overlay_88.inc"
	.include "global.inc"

.text
	thumb_func_start ov88_022592EC
ov88_022592EC: ; 0x022592EC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	mov r7, #3
	add r5, r0, #0
	ldr r6, _022593C4 ; =_02259904
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	str r2, [sp, #0x1c]
	mov r4, #0
	str r0, [sp, #0x20]
	lsl r7, r7, #8
	add r5, #0x60
_02259304:
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x18]
	ldr r3, [sp, #0x1c]
	add r2, r4, #0
	bl ov88_02259508
	ldrb r0, [r6]
	add r1, r5, #0
	mov r2, #3
	str r0, [sp]
	mov r0, #0x1e
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	add r0, r4, #7
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0xc]
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x18]
	mov r3, #1
	ldr r0, [r0]
	bl AddWindowParameterized
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	ldr r0, [sp, #0x20]
	add r4, r4, #1
	add r0, #0x20
	str r0, [sp, #0x20]
	add r7, #0x3c
	add r6, r6, #1
	add r5, #0x10
	cmp r4, #3
	blt _02259304
	mov r0, #1
	ldr r1, [sp, #0x1c]
	lsl r0, r0, #8
	bl String_New
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x14]
	add r1, #0x90
	str r0, [r1]
	mov r1, #5
	ldr r0, [sp, #0x18]
	lsl r1, r1, #6
	ldr r0, [r0, r1]
	ldr r3, [sp, #0x1c]
	mov r1, #0
	add r2, #0x98
	bl GfGfxLoader_GetPlttDataFromOpenNarc
	ldr r1, [sp, #0x14]
	mov r2, #4
	add r1, #0x94
	str r0, [r1]
	ldr r1, [sp, #0x14]
	mov r0, #0
	add r1, #0x98
	ldr r1, [r1]
	mov r3, #0xfa
	ldr r1, [r1, #0xc]
	add r1, #0xee
	bl BG_LoadPlttData
	ldr r1, [sp, #0x14]
	ldr r3, _022593C8 ; =0x0000011A
	add r1, #0x98
	ldr r1, [r1]
	mov r0, #0
	ldr r1, [r1, #0xc]
	mov r2, #4
	add r1, #0xee
	bl BG_LoadPlttData
	ldr r1, [sp, #0x14]
	ldr r3, _022593CC ; =0x0000013A
	add r1, #0x98
	str r1, [sp, #0x14]
	ldr r1, [r1]
	mov r0, #0
	ldr r1, [r1, #0xc]
	mov r2, #4
	add r1, #0xee
	bl BG_LoadPlttData
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022593C4: .word _02259904
_022593C8: .word 0x0000011A
_022593CC: .word 0x0000013A
	thumb_func_end ov88_022592EC
