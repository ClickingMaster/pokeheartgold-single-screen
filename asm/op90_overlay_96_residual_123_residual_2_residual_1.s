	.extern ov96_02215DBC
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
.extern PokeathlonCourse_RunSubStateLoop
.extern PokeathlonCourse_InitStateInfo
.extern PokeathlonCourse_InitPlayerProfiles
.extern PokeathlonCourse_GetPlayerProfile
.extern PokeathlonCourse_GetParticipantCount
.extern PokeathlonCourse_GetParticipantData
.extern PokeathlonCourse_GetParticipantUnk04
.extern PokeathlonCourse_GetSaveData
.extern PokeathlonCourse_GetFieldData
.extern PokeathlonCourse_GetFieldData_AtIndex
.extern PokeathlonCourse_GetFieldBA4
.extern PokeathlonCourse_AllocPtr4FromHeap
.extern PokeathlonCourse_FreePtr4HeapAlloc
.extern PokeathlonCourse_GetHeapAllocPtr4
.extern PokeathlonCourse_GetHeapID
.extern PokeathlonCourse_GetField1ED
.extern PokeathlonCourse_SetField1ED
.extern PokeathlonCourse_IncrementField1ED
.extern PokeathlonCourse_SetField1F4
.extern ov96_021E5E04
.extern PokeathlonCourse_GetField3D8_ForCurrentParticipant
.extern PokeathlonCourse_GetField3D8_AtIndex
.extern ov96_021E5E7C
.extern PokeathlonCourse_GetCurrentParticipantIndex
.extern PokeathlonCourse_GetMode
.extern PokeathlonCourse_GetField1EF
.extern PokeathlonCourse_IncrementField1EF
.extern PokeathlonCourse_ResetField1EF
.extern PokeathlonCourse_GetSystem
.extern ov96_021E5F24
.extern PokeathlonCourse_GetPlayerProfileFromData
.extern PokeathlonCourse_GetField974_AtIndex
.extern PokeathlonCourse_GetDataCopyArea
.extern PokeathlonCourse_ResetDataCopyArea
.extern PokeathlonCourse_SetField3A4
.extern PokeathlonCourse_ResetField3A4
.extern PokeathlonCourse_GetField3A4
.extern PokeathlonCourse_GetUnkConstant4
.extern PokeathlonCourse_SetField5E0_AtIndex
.extern PokeathlonCourse_GetField5F0_AtIndex
.extern PokeathlonCourse_SetStateField07
.extern PokeathlonCourse_SetStateField07_IfDifferent
.extern PokeathlonCourse_SetStateTransitionType
.extern PokeathlonCourse_SetVBlankIntrCB
.extern PokeathlonCourse_GetGraphicsSystem
	thumb_func_start ov96_0221A08C
ov96_0221A08C: ; 0x0221A08C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	add r5, r0, #0
	mov r0, #0
	str r0, [sp, #0x10]
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	add r1, sp, #0x28
	strb r0, [r1]
	ldr r0, [sp, #0x10]
	strb r0, [r1, #1]
	ldr r0, [sp, #0x10]
	strb r0, [r1, #2]
	ldr r0, [sp, #0x10]
	strb r0, [r1, #3]
	ldr r1, [r5, #8]
	ldr r0, [r5]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215DD4
	ldr r1, [r5, #8]
	add r6, r0, #0
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	ldr r0, [r5]
	lsr r1, r1, #0x18
	bl ov96_02215E2C
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r7, #0
	add r4, sp, #0x28
	str r0, [sp, #0x18]
_0221A0D6:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x1e
	lsr r0, r0, #0x1e
	cmp r7, r0
	beq _0221A14A
	lsl r1, r7, #0x18
	ldr r0, [r5]
	lsr r1, r1, #0x18
	bl ov96_02215DD4
	str r0, [sp, #0x14]
	mov r2, #2
	ldr r1, [sp, #0x14]
	add r0, r6, #0
	lsl r2, r2, #0x10
	bl ov96_0221A034
	cmp r0, #0
	beq _0221A11E
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x18]
	add r0, r6, #0
	bl ov96_0221A05C
	cmp r0, #0
	beq _0221A10E
	mov r0, #3
	b _0221A110
_0221A10E:
	mov r0, #1
_0221A110:
	strb r0, [r4]
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0xc]
	b _0221A14A
_0221A11E:
	mov r2, #3
	ldr r1, [sp, #0x14]
	add r0, r6, #0
	lsl r2, r2, #0x10
	bl ov96_0221A034
	cmp r0, #0
	beq _0221A14A
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x18]
	add r0, r6, #0
	bl ov96_0221A05C
	cmp r0, #0
	beq _0221A140
	mov r0, #2
	strb r0, [r4]
_0221A140:
	ldr r0, [sp, #0x10]
	add r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x10]
_0221A14A:
	add r7, r7, #1
	add r4, r4, #1
	cmp r7, #4
	blt _0221A0D6
	mov r4, #0
	add r0, sp, #0x24
	strb r4, [r0]
	strb r4, [r0, #1]
	strb r4, [r0, #2]
	strb r4, [r0, #3]
	add r1, r4, #0
	add r0, sp, #0x28
	add r2, sp, #0x1c
_0221A164:
	ldr r3, [r5, #8]
	lsl r3, r3, #0x1e
	lsr r3, r3, #0x1e
	cmp r1, r3
	beq _0221A17C
	ldrb r6, [r2, #9]
	ldrb r3, [r0]
	cmp r6, r3
	bhs _0221A17C
	strb r1, [r2, #8]
	ldrb r3, [r0]
	strb r3, [r2, #9]
_0221A17C:
	add r1, r1, #1
	add r0, r0, #1
	cmp r1, #4
	blt _0221A164
	bl MTRandom
	mov r1, #0x64
	bl _u32_div_f
	add r2, sp, #0x1c
	ldrb r3, [r2, #9]
	ldr r0, _0221A308 ; =ov96_0221D97C
	ldrb r0, [r0, r3]
	add r0, r4, r0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r1, r0
	bge _0221A1D0
	ldrb r1, [r2, #8]
	ldr r0, [r5]
	bl ov96_02215DD4
	ldr r1, [r0]
	asr r2, r1, #0xc
	add r1, sp, #0x1c
	strh r2, [r1, #4]
	ldr r0, [r0, #4]
	add r2, sp, #0x20
	asr r0, r0, #0xc
	strh r0, [r1, #6]
	ldr r1, [r5, #8]
	ldr r0, [r5]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215EE8
	mov r0, #0x10
	add sp, #0x2c
	strb r0, [r5, #4]
	pop {r4, r5, r6, r7, pc}
_0221A1D0:
	bl MTRandom
	mov r1, #0x64
	bl _u32_div_f
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _0221A1E6
	add r0, r4, #5
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_0221A1E6:
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _0221A1F8
	sub r2, r0, #1
	ldr r0, _0221A30C ; =ov96_0221D978
	ldrb r0, [r0, r2]
	add r0, r4, r0
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_0221A1F8:
	cmp r1, r4
	bge _0221A214
	ldr r1, [r5, #8]
	ldr r0, [r5]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215F2C
	mov r0, #0x14
	add sp, #0x2c
	strb r0, [r5, #4]
	pop {r4, r5, r6, r7, pc}
_0221A214:
	bl MTRandom
	mov r1, #0x64
	bl _u32_div_f
	cmp r1, #0x3c
	bge _0221A2BC
	bl MTRandom
	mov r1, #0x64
	bl _u32_div_f
	ldr r0, [r5, #8]
	str r1, [sp]
	lsl r0, r0, #0x1a
	lsr r0, r0, #0x1f
	bne _0221A302
	mov r7, #0
	mvn r7, r7
	ldr r0, _0221A310 ; =0x000003E7
	str r7, [sp, #8]
	str r0, [sp, #4]
	mov r4, #0
_0221A242:
	lsl r1, r4, #0x18
	ldr r0, [r5]
	lsr r1, r1, #0x18
	bl ov96_02215E48
	add r6, r0, #0
	lsl r1, r4, #0x18
	ldr r0, [r5]
	lsr r1, r1, #0x18
	bl ov96_02215DBC
	ldr r1, [r5, #8]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	cmp r4, r1
	beq _0221A284
	cmp r0, #3
	beq _0221A284
	cmp r0, #4
	beq _0221A284
	ldrb r1, [r5, #7]
	ldr r0, [r5]
	bl ov96_02215E94
	cmp r0, #0
	bne _0221A284
	ldr r0, [sp, #4]
	cmp r0, r6
	ble _0221A284
	str r7, [sp, #8]
	lsl r0, r4, #0x18
	asr r7, r0, #0x18
	str r6, [sp, #4]
_0221A284:
	add r4, r4, #1
	cmp r4, #4
	blt _0221A242
	ldr r0, [sp]
	cmp r0, #0x3c
	bge _0221A294
	strb r7, [r5, #7]
	b _0221A2A2
_0221A294:
	cmp r0, #0x64
	bge _0221A29E
	ldr r0, [sp, #8]
	strb r0, [r5, #7]
	b _0221A2A2
_0221A29E:
	bl GF_AssertFail
_0221A2A2:
	mov r0, #7
	ldrsb r1, [r5, r0]
	sub r0, #8
	cmp r1, r0
	beq _0221A302
	ldr r1, [r5, #8]
	mov r0, #0x20
	orr r0, r1
	str r0, [r5, #8]
	mov r0, #0
	add sp, #0x2c
	strb r0, [r5, #4]
	pop {r4, r5, r6, r7, pc}
_0221A2BC:
	cmp r1, #0x50
	bge _0221A2C8
	mov r0, #0xa
	add sp, #0x2c
	strb r0, [r5, #4]
	pop {r4, r5, r6, r7, pc}
_0221A2C8:
	cmp r1, #0x64
	bge _0221A302
	bl MTRandom
	mov r1, #0x91
	bl _u32_div_f
	add r1, #0x38
	add r0, sp, #0x1c
	strh r1, [r0]
	bl MTRandom
	mov r1, #0x50
	bl _u32_div_f
	add r1, #0x48
	add r0, sp, #0x1c
	strh r1, [r0, #2]
	ldr r1, [r5, #8]
	ldr r0, [r5]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r2, sp, #0x1c
	bl ov96_02215F80
	mov r0, #0xa
	strb r0, [r5, #4]
_0221A302:
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	nop
_0221A308: .word ov96_0221D97C
_0221A30C: .word ov96_0221D978
_0221A310: .word 0x000003E7
	thumb_func_end ov96_0221A08C
