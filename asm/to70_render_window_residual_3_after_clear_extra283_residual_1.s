	.include "asm/macros.inc"
	.include "render_window.inc"
	.include "global.inc"
	.extern sub_0200EA24

.text
	thumb_func_start sub_0200EA68
sub_0200EA68: ; 0x0200EA68
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x48
	str r2, [sp, #0x24]
	str r3, [sp, #0x28]
	str r0, [sp, #0x20]
	ldr r0, [r0]
	add r7, r1, #0
	bl BgConfig_GetHeapId
	add r6, r0, #0
	ldr r0, [sp, #0x20]
	bl GetWindowBgId
	str r0, [sp, #0x2c]
	ldr r0, [sp, #0x28]
	lsl r0, r0, #7
	str r0, [sp, #0x30]
	ldr r1, [sp, #0x30]
	add r0, r6, #0
	bl Heap_Alloc
	add r5, r0, #0
	ldr r0, [sp, #0x2c]
	bl BgGetCharPtr
	add r4, r0, #0
	str r6, [sp]
	mov r0, #0x26
	add r1, r7, #0
	mov r2, #0
	add r3, sp, #0x44
	bl GfGfxLoader_GetCharData
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x44]
	mov r7, #0
	ldr r0, [r0, #0x14]
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x28]
	cmp r0, #0
	ble _0200EB12
	ldr r0, [sp, #0x24]
	add r0, #0xa
	lsl r0, r0, #5
	str r0, [sp, #0x3c]
	ldr r0, [sp, #0x24]
	add r0, #0xb
	lsl r0, r0, #5
	str r0, [sp, #0x40]
_0200EACA:
	ldr r1, [sp, #0x3c]
	lsl r6, r7, #7
	add r0, r5, r6
	add r1, r4, r1
	mov r2, #0x20
	bl memcpy
	add r0, r6, #0
	ldr r1, [sp, #0x40]
	add r0, #0x20
	add r0, r5, r0
	add r1, r4, r1
	mov r2, #0x20
	bl memcpy
	add r0, r6, #0
	ldr r1, [sp, #0x3c]
	add r0, #0x40
	add r0, r5, r0
	add r1, r4, r1
	mov r2, #0x20
	bl memcpy
	ldr r1, [sp, #0x40]
	add r6, #0x60
	add r0, r5, r6
	add r1, r4, r1
	mov r2, #0x20
	bl memcpy
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	ldr r0, [sp, #0x28]
	cmp r7, r0
	blt _0200EACA
_0200EB12:
	add r1, sp, #0x50
	ldrb r2, [r1, #0x14]
	mov r3, #0x10
	ldr r0, [sp, #0x28]
	sub r4, r3, r2
	mul r0, r4
	ldrb r1, [r1, #0x10]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	sub r3, r3, r1
	lsl r3, r3, #0x18
	str r0, [sp]
	lsr r3, r3, #0x18
	str r5, [sp, #4]
	str r3, [sp, #8]
	str r0, [sp, #0xc]
	mov r4, #0
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r3, [sp, #0x18]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x38]
	bl sub_0200EA24
	ldr r0, [sp, #0x24]
	ldr r3, [sp, #0x30]
	add r0, #0x12
	str r0, [sp, #0x24]
	str r0, [sp]
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x2c]
	ldr r0, [r0]
	add r2, r5, #0
	bl BG_LoadCharTilesData
	ldr r0, [sp, #0x34]
	bl Heap_Free
	add r0, r5, #0
	bl Heap_Free
	add sp, #0x48
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end sub_0200EA68
