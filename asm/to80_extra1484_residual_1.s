	.extern ov12_022612A4
	.extern ov12_02261B80
	.extern ov12_02261CA8
	.extern ov12_02261F38
	.extern ov12_0226D120
	.extern ov12_0226D128
	.extern ov12_0226D15A
#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

.text
	thumb_func_start ov12_0225B494
ov12_0225B494: ; 0x0225B494
	push {r4, r5, r6, lr}
	sub sp, #0x70
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	ldrb r1, [r4, #0x12]
	add r5, r0, #0
	cmp r1, #4
	bls _0225B4AC
	b _0225B798
_0225B4AC:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225B4B8: ; jump table
	.short _0225B4C2 - _0225B4B8 - 2 ; case 0
	.short _0225B4CC - _0225B4B8 - 2 ; case 1
	.short _0225B4DA - _0225B4B8 - 2 ; case 2
	.short _0225B728 - _0225B4B8 - 2 ; case 3
	.short _0225B77C - _0225B4B8 - 2 ; case 4
_0225B4C2:
	mov r0, #0x1c
	str r0, [r4, #0x20]
	ldrb r0, [r4, #0x12]
	add r0, r0, #1
	strb r0, [r4, #0x12]
_0225B4CC:
	ldr r0, [r4, #0x20]
	sub r0, r0, #1
	str r0, [r4, #0x20]
	bne _0225B5A0
	ldrb r0, [r4, #0x12]
	add r0, r0, #1
	strb r0, [r4, #0x12]
_0225B4DA:
	ldrb r0, [r4, #0x13]
	add r1, sp, #0x14
	cmp r0, #2
	ldr r0, [r4, #0xc]
	beq _0225B4E6
	b _0225B61A
_0225B4E6:
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x1c]
	cmp r0, #1
	beq _0225B4FA
	cmp r0, #3
	bne _0225B51C
_0225B4FA:
	add r1, sp, #0x14
	mov r0, #2
	ldrsh r0, [r1, r0]
	cmp r0, #0xc0
	ldr r0, [r4, #0xc]
	bge _0225B512
	ldr r0, [r0]
	mov r1, #8
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225B51C
_0225B512:
	ldr r0, [r0]
	mov r1, #0xc0
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
_0225B51C:
	ldr r0, [r4, #0xc]
	add r1, sp, #0x14
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x1c]
	cmp r0, #1
	bne _0225B540
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_SetAttr
	b _0225B590
_0225B540:
	cmp r0, #3
	bne _0225B56A
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r1, [r3, r2]
	sub r0, r0, r1
	strh r0, [r3, #2]
	ldrsh r0, [r3, r2]
	mov r1, #0
	sub r0, #0x18
	strh r0, [r3, #2]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	neg r2, r2
	bl Pokepic_AddAttr
	b _0225B590
_0225B56A:
	cmp r0, #5
	bne _0225B590
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r1, [r3, r2]
	sub r0, r1, r0
	strh r0, [r3, #2]
	ldrsh r0, [r3, r2]
	mov r1, #0
	sub r0, #0x10
	strh r0, [r3, #2]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	bl Pokepic_AddAttr
_0225B590:
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	mov r1, #0x14
	ldrsh r1, [r4, r1]
	cmp r0, r1
	bge _0225B5A2
_0225B5A0:
	b _0225B7B4
_0225B5A2:
	ldr r0, [r4, #8]
	mov r1, #0x2c
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #0x2d
	mov r2, #0
	bl Pokepic_SetAttr
	mov r2, #0x14
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_SetAttr
	mov r0, #0x69
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	str r0, [sp]
	ldrh r0, [r4, #0x16]
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x2c
	ldrb r0, [r0]
	str r0, [sp, #8]
	ldrb r0, [r4, #0x13]
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x18]
	str r0, [sp, #0x10]
	ldrb r1, [r4, #0x11]
	ldr r0, [r4]
	ldr r2, [r4, #0x1c]
	ldr r3, [r4, #8]
	bl ov12_02261F38
	ldr r0, [r4, #0x1c]
	cmp r0, #1
	beq _0225B5F6
	cmp r0, #3
	bne _0225B602
_0225B5F6:
	ldr r0, [r4, #0xc]
	mov r1, #0xc0
	ldr r0, [r0]
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
_0225B602:
	mov r2, #0
	str r2, [sp]
	ldr r0, [r4, #8]
	mov r1, #8
	add r3, r2, #0
	bl Pokepic_StartPaletteFade
	ldrb r0, [r4, #0x12]
	add sp, #0x70
	add r0, r0, #1
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B61A:
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x1c]
	cmp r0, #0
	beq _0225B62E
	cmp r0, #2
	bne _0225B650
_0225B62E:
	add r0, sp, #0x14
	mov r1, #2
	ldrsh r0, [r0, r1]
	cmp r0, #0x40
	ldr r0, [r4, #0xc]
	ble _0225B646
	ldr r0, [r0]
	sub r1, #0xa
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225B650
_0225B646:
	ldr r0, [r0]
	mov r1, #0x40
	mov r2, #0x88
	bl ManagedSprite_SetPositionXY
_0225B650:
	ldr r0, [r4, #0xc]
	add r1, sp, #0x14
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x1c]
	cmp r0, #0
	bne _0225B674
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_SetAttr
	b _0225B6C4
_0225B674:
	cmp r0, #2
	bne _0225B69C
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r1, [r3, r2]
	sub r0, r1, r0
	strh r0, [r3, #2]
	ldrsh r0, [r3, r2]
	mov r1, #0
	sub r0, #0x18
	strh r0, [r3, #2]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	bl Pokepic_AddAttr
	b _0225B6C4
_0225B69C:
	cmp r0, #4
	bne _0225B6C4
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	add r3, sp, #0x14
	mov r2, #2
	ldrsh r1, [r3, r2]
	sub r0, r0, r1
	strh r0, [r3, #2]
	ldrsh r0, [r3, r2]
	mov r1, #0
	sub r0, #0x10
	strh r0, [r3, #2]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #8]
	neg r2, r2
	bl Pokepic_AddAttr
_0225B6C4:
	mov r0, #0x14
	ldrsh r5, [r4, r0]
	ldr r0, [r4, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	cmp r0, r5
	bgt _0225B7B4
	ldr r0, [r4, #8]
	mov r1, #0
	add r2, r5, #0
	bl Pokepic_SetAttr
	mov r0, #0x69
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	str r0, [sp]
	ldrh r0, [r4, #0x16]
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x2c
	ldrb r0, [r0]
	str r0, [sp, #8]
	ldrb r0, [r4, #0x13]
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x18]
	str r0, [sp, #0x10]
	ldrb r1, [r4, #0x11]
	ldr r0, [r4]
	ldr r2, [r4, #0x1c]
	ldr r3, [r4, #8]
	bl ov12_02261F38
	ldr r0, [r4, #0x1c]
	cmp r0, #0
	beq _0225B712
	cmp r0, #2
	bne _0225B71E
_0225B712:
	ldr r0, [r4, #0xc]
	mov r1, #0x40
	ldr r0, [r0]
	mov r2, #0x88
	bl ManagedSprite_SetPositionXY
_0225B71E:
	ldrb r0, [r4, #0x12]
	add sp, #0x70
	add r0, r0, #1
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B728:
	ldr r0, [r4]
	bl ov12_0223B750
	ldrb r1, [r4, #0x11]
	bl sub_02017068
	cmp r0, #1
	bne _0225B7B4
	ldr r0, [r4, #8]
	bl Pokepic_IsAnimFinished
	cmp r0, #0
	bne _0225B7B4
	ldr r0, [r4, #0x28]
	cmp r0, #0
	beq _0225B774
	ldrb r1, [r4, #0x11]
	mov r0, #0xb
	add r2, sp, #0x18
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x18
	bl ov12_02261B80
	mov r0, #4
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B774:
	mov r0, #0xff
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B77C:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225B7B4
	add r0, r5, #0
	bl ov07_0221C3C0
	mov r0, #0xff
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B798:
	mov r0, #0
	bl sub_02005B58
	ldrb r1, [r4, #0x11]
	ldrb r2, [r4, #0x10]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225B7B4:
	add sp, #0x70
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_0225B494

	thumb_func_start ov12_0225B7B8
ov12_0225B7B8: ; 0x0225B7B8
	push {r4, r5, r6, lr}
	sub sp, #0x70
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	ldrb r1, [r4, #0x12]
	add r5, r0, #0
	cmp r1, #4
	bls _0225B7D0
	b _0225B940
_0225B7D0:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225B7DC: ; jump table
	.short _0225B7E6 - _0225B7DC - 2 ; case 0
	.short _0225B7F0 - _0225B7DC - 2 ; case 1
	.short _0225B800 - _0225B7DC - 2 ; case 2
	.short _0225B8D0 - _0225B7DC - 2 ; case 3
	.short _0225B924 - _0225B7DC - 2 ; case 4
_0225B7E6:
	mov r0, #0x1c
	str r0, [r4, #0x20]
	ldrb r0, [r4, #0x12]
	add r0, r0, #1
	strb r0, [r4, #0x12]
_0225B7F0:
	ldr r0, [r4, #0x20]
	sub r0, r0, #1
	str r0, [r4, #0x20]
	beq _0225B7FA
	b _0225B95C
_0225B7FA:
	ldrb r0, [r4, #0x12]
	add r0, r0, #1
	strb r0, [r4, #0x12]
_0225B800:
	ldr r0, [r4, #0xc]
	add r1, sp, #0x14
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x1c]
	cmp r0, #1
	beq _0225B818
	cmp r0, #3
	bne _0225B83A
_0225B818:
	add r1, sp, #0x14
	mov r0, #2
	ldrsh r0, [r1, r0]
	cmp r0, #0xc0
	ldr r0, [r4, #0xc]
	bge _0225B830
	ldr r0, [r0]
	mov r1, #8
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225B83A
_0225B830:
	ldr r0, [r0]
	mov r1, #0xc0
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
_0225B83A:
	ldr r0, [r4, #0xc]
	add r1, sp, #0x14
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #8]
	mov r1, #1
	mov r2, #4
	bl Pokepic_AddAttr
	ldr r0, [r4, #8]
	mov r1, #1
	bl Pokepic_GetAttr
	mov r1, #0x14
	ldrsh r1, [r4, r1]
	cmp r0, r1
	blt _0225B95C
	ldr r0, [r4, #8]
	mov r1, #0x2c
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #0x2d
	mov r2, #0
	bl Pokepic_SetAttr
	mov r2, #0x14
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #8]
	mov r1, #1
	bl Pokepic_SetAttr
	mov r0, #0x69
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	str r0, [sp]
	ldrh r0, [r4, #0x16]
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x2c
	ldrb r0, [r0]
	str r0, [sp, #8]
	ldrb r0, [r4, #0x13]
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x18]
	str r0, [sp, #0x10]
	ldrb r1, [r4, #0x11]
	ldr r0, [r4]
	ldr r2, [r4, #0x1c]
	ldr r3, [r4, #8]
	bl ov12_02261F38
	ldr r0, [r4, #0xc]
	mov r1, #0xc0
	ldr r0, [r0]
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
	mov r2, #0
	str r2, [sp]
	ldr r0, [r4, #8]
	mov r1, #8
	add r3, r2, #0
	bl Pokepic_StartPaletteFade
	ldrb r0, [r4, #0x12]
	add sp, #0x70
	add r0, r0, #1
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B8D0:
	ldr r0, [r4]
	bl ov12_0223B750
	ldrb r1, [r4, #0x11]
	bl sub_02017068
	cmp r0, #1
	bne _0225B95C
	ldr r0, [r4, #8]
	bl Pokepic_IsAnimFinished
	cmp r0, #0
	bne _0225B95C
	ldr r0, [r4, #0x28]
	cmp r0, #0
	beq _0225B91C
	ldrb r1, [r4, #0x11]
	mov r0, #0xb
	add r2, sp, #0x18
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x18
	bl ov12_02261B80
	mov r0, #4
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B91C:
	mov r0, #0xff
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B924:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225B95C
	add r0, r5, #0
	bl ov07_0221C3C0
	mov r0, #0xff
	add sp, #0x70
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225B940:
	mov r0, #0
	bl sub_02005B58
	ldrb r1, [r4, #0x11]
	ldrb r2, [r4, #0x10]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225B95C:
	add sp, #0x70
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_0225B7B8
