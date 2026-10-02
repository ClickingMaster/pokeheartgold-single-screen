	.extern ov41_02249E40
	.extern ov41_0224A094
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02249FFC
ov41_02249FFC: ; 0x02249FFC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r0, #0
	str r1, [sp, #4]
	ldr r0, [sp, #0x24]
	mov r1, #0x14
	add r5, r2, #0
	add r6, r3, #0
	bl Heap_Alloc
	add r4, r0, #0
	add r2, r4, #0
	mov r1, #0x14
	mov r0, #0
_0224A018:
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _0224A018
	ldr r0, [sp, #0x24]
	ldr r1, [sp, #4]
	str r0, [sp]
	add r0, r7, #0
	mov r2, #0
	add r3, r4, #4
	bl GfGfxLoader_GetCharData
	str r0, [r4, #8]
	str r5, [r4]
	ldr r0, [sp, #0x20]
	str r6, [r4, #0xc]
	str r0, [r4, #0x10]
	ldr r0, _0224A048 ; =ov41_0224A094
	add r1, r4, #0
	mov r2, #0x80
	bl SysTask_CreateOnVWaitQueue
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0224A048: .word ov41_0224A094
	thumb_func_end ov41_02249FFC
