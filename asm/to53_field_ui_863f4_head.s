#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_020863F4.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text
	.public _021027C8
	.public _021027DC
	.public _021027F4
	.public sub_020863F4
	.public sub_02086490
	.public sub_020866CC
	.public sub_02086758
	.public sub_020868A0
	.public sub_020869BC
	.public sub_02086AB4
	.public sub_02086AE4
	.public sub_02086B2C
	.public sub_02086B6C
	.public sub_02086BB4
	.public sub_02086C80
	.public sub_02086C8C
	.public sub_02086D98
	.public sub_02086DA4
	.public sub_02086DE4
	.public sub_02086F44
	.public sub_02086FCC
	.public sub_02086FE8
	.public sub_02087028
	.public sub_02087064
	.public sub_02087090
	.public sub_020871C4
	.public sub_02087230

	thumb_func_start sub_02086758
sub_02086758: ; 0x02086758
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #0x2f
	lsl r0, r0, #4
	ldr r1, [sp]
	add r2, r0, #4
	ldr r1, [r1, r0]
	str r1, [sp, #8]
	ldr r1, [sp]
	ldr r1, [r1, r2]
	add r2, sp, #0x10
	str r1, [sp, #4]
	ldr r1, [sp, #0xc]
	strh r1, [r2]
	strh r1, [r2, #2]
	strh r1, [r2, #4]
	strh r1, [r2, #6]
	mov r1, #0xa
	str r1, [sp, #0x18]
	ldr r1, [sp, #0xc]
	mov r2, #1
	str r1, [sp, #0x3c]
	str r1, [sp, #0x40]
	str r1, [sp, #0x1c]
	add r1, r0, #0
	add r1, #0xf8
	str r1, [sp, #0x24]
	str r1, [sp, #0x28]
	str r1, [sp, #0x2c]
	str r1, [sp, #0x30]
	sub r1, r2, #2
	str r1, [sp, #0x34]
	str r1, [sp, #0x38]
	ldr r1, [sp]
	str r2, [sp, #0x20]
	add r0, #0xf0
	ldr r4, [r1, r0]
	mov r1, #0x3f
	ldr r0, [sp]
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	sub r6, r0, #1
	bmi _020867DC
	mov r0, #0x1c
	add r1, r6, #0
	mul r1, r0
	ldr r0, [sp]
	mov r7, #0xa
	add r5, r0, r1
_020867C0:
	add r0, r4, #0
	add r1, r7, #0
	bl _u32_div_f
	add r0, r1, #1
	str r0, [r5]
	add r0, r4, #0
	mov r1, #0xa
	bl _u32_div_f
	add r4, r0, #0
	sub r5, #0x1c
	sub r6, r6, #1
	bpl _020867C0
_020867DC:
	mov r1, #0xfb
	ldr r0, [sp]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	mov r2, #0x2d
	ldr r1, [sp]
	lsl r2, r2, #4
	ldr r1, [r1, r2]
	mov r6, #0
	add r1, r1, r0
	cmp r1, #0
	ble _0208689C
	ldr r4, [sp]
	mov r7, #0x4c
	add r5, r4, #0
_020867FA:
	cmp r0, #0
	beq _0208684C
	mov r0, #7
	lsl r0, r0, #6
	ldr r1, [r4, r0]
	ldr r0, [sp, #0xc]
	add r0, r0, r1
	add r0, r0, #1
	cmp r6, r0
	bne _0208684C
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	add r2, sp, #0x10
	bl SpriteSystem_NewSprite
	mov r1, #0x73
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r0, r1, #0
	lsl r1, r7, #0x10
	ldr r0, [r4, r0]
	asr r1, r1, #0x10
	mov r2, #0x18
	bl ManagedSprite_SetPositionXY
	mov r0, #0x73
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0x16
	bl ManagedSprite_SetAnim
	mov r0, #0x73
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ManagedSprite_TickFrame
	ldr r0, [sp, #0xc]
	add r4, #0x1c
	add r0, r0, #1
	str r0, [sp, #0xc]
	b _02086882
_0208684C:
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	add r2, sp, #0x10
	bl SpriteSystem_NewSprite
	lsl r1, r7, #0x10
	str r0, [r5, #0xc]
	asr r1, r1, #0x10
	mov r2, #0x18
	bl ManagedSprite_SetPositionXY
	ldr r0, [r5]
	ldr r1, [r5, #8]
	bl sub_02086D98
	add r1, r0, #0
	ldr r0, [r5, #0xc]
	bl ManagedSprite_SetAnim
	ldr r0, [r5, #0xc]
	mov r1, #2
	bl ManagedSprite_SetAffineOverwriteMode
	ldr r0, [r5, #0xc]
	bl ManagedSprite_TickFrame
	add r5, #0x1c
_02086882:
	ldr r1, [sp]
	mov r0, #0xfb
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	add r2, r1, #0
	mov r1, #0x2d
	lsl r1, r1, #4
	ldr r1, [r2, r1]
	add r6, r6, #1
	add r1, r1, r0
	add r7, #8
	cmp r6, r1
	blt _020867FA
_0208689C:
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	thumb_func_end sub_02086758
