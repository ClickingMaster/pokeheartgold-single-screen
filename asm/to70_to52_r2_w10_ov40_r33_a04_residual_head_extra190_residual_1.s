	.extern AddTextPrinterParameterizedWithColor
	.extern AddTextWindowTopLeftCorner
	.extern BufferString
	.extern InitWindow
	.extern ManagedSprite_SetAnim
	.extern ManagedSprite_SetPositionXY
	.extern MessageFormat_Delete
	.extern NewString_ReadMsgData
	.extern RemoveWindow
	.extern StringExpandPlaceholders
	.extern String_Delete
	.extern String_New
	.extern TextOBJ_CopyFromBGWindow
	.extern TextOBJ_SetSpritesDrawFlag
	.extern ov40_0222D5AC
	.extern ov40_0222D66C
	.extern ov40_0222D78C
	.extern ov40_0222D800
	.extern ov40_0222DAB0
	.extern ov40_02230DCC
	.extern ov40_022454C0
	.extern ov40_022454D0
	.extern sub_020136B4
	.extern sub_020315B8
	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"
	.extern ov40_0223B29C

.text
	thumb_func_start ov40_0223B374
ov40_0223B374: ; 0x0223B374
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	mov r1, #0x86
	lsl r1, r1, #4
	ldr r3, _0223B444 ; =ov40_022454C0
	ldr r4, [r0, r1]
	str r0, [sp]
	add r2, sp, #0x20
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r3, _0223B448 ; =ov40_022454D0
	add r2, sp, #0x10
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [sp]
	mov r1, #2
	bl ov40_0222D78C
	mov r0, #0x45
	lsl r0, r0, #2
	add r5, r4, r0
	add r0, sp, #0x20
	str r0, [sp, #8]
	mov r0, #0x69
	str r0, [sp, #4]
	ldr r0, [sp]
	mov r6, #0
	str r0, [sp, #0xc]
	add r0, #0x14
	add r7, sp, #0x10
	str r0, [sp, #0xc]
_0223B3BA:
	ldr r0, [sp]
	mov r1, #2
	bl ov40_0222D800
	mov r1, #0x46
	lsl r1, r1, #2
	str r0, [r4, r1]
	ldr r1, [sp, #0xc]
	add r0, r5, #0
	mov r2, #2
	bl ov40_0222D5AC
	ldr r2, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r2, [r2]
	add r0, r5, #0
	bl ov40_0222D66C
	cmp r6, #3
	beq _0223B3EC
	ldr r1, [sp]
	add r0, r5, #0
	add r2, r6, #1
	bl ov40_0223B29C
_0223B3EC:
	mov r0, #0x46
	ldr r2, [sp, #4]
	lsl r0, r0, #2
	lsl r2, r2, #0x10
	ldr r0, [r4, r0]
	mov r1, #0x32
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	mov r0, #0x46
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	ldr r1, [r7]
	bl ManagedSprite_SetAnim
	mov r0, #0x47
	lsl r0, r0, #2
	mov r1, #0x24
	add r2, r1, #0
	ldr r0, [r4, r0]
	sub r2, #0x2c
	bl sub_020136B4
	mov r0, #0x47
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl TextOBJ_SetSpritesDrawFlag
	ldr r0, [sp, #8]
	add r6, r6, #1
	add r0, r0, #4
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	add r4, #0x1c
	add r0, #0x24
	add r5, #0x1c
	add r7, r7, #4
	str r0, [sp, #4]
	cmp r6, #4
	blt _0223B3BA
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0223B444: .word ov40_022454C0
_0223B448: .word ov40_022454D0
	thumb_func_end ov40_0223B374
