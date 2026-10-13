	.include "asm/macros.inc"
	.public _0225EEA8
	.public _0225EF20
	.public _0225EF70
	.public _0225EFD4
	.public _0225F264
	.public _0225F2C8
	.public _0225F2DC
	.public _0225F2EC
	.public _0225F300
	.public _0225F314
	.public _0225F32C
	.public _0225F340
	.public _0225F358
	.public _0225F36C
	.public _0225F3E4
	.public _0225F3EC
	.public _0225F43C
	.public _0225F440
	.public _0225F444
	.public _0225F448
	.public _0225F4BC
	.public _0225F4E8
	.public _0225F4F4
	.public _0225F51C
	.public _0225F540
	.public _0225F544
	.public _0225F5A0
	.public _0225F5B4
	.public _0225F5E0
	.public _0225F608
	.public _0225F680
	.public _0225F69C
	.public _0225F6C0
	.public _0225F6E0
	.public _0225F700
	.public _0225F724
	.public _0225F7D0
	.public _0225F7E4
	.public _0225F8A4
	.public _0225F8A8
	.public _0225F948
	.public _0225FAB4
	.public _0225FAB8
	.public _0225FAF8
	.public _0225FAFC
	.public _0225FB68
	.public _0225FB94
	.public _0225FBE0
	.public _0225FBEC
	.public _02262C04
	.public WobbuffetPopBalloon_LoadShininessTable
	.public WobbuffetPopBalloon_InitMeshVertices
	.public WobbuffetPopBalloon_InitMeshTexCoords
	.public WobbuffetPopBalloon_DrawMesh
	.public WobbuffetPopBalloon_ToVtx16
	.public WobbuffetPopBalloon_TryGrabHandle
	.public WobbuffetPopBalloon_UpdateDrag
	.public WobbuffetPopBalloon_DeformMesh
	.public WobbuffetPopBalloon_GetPushSide
	.public WobbuffetPopBalloon_CalcSidePush
	.public WobbuffetPopBalloon_CalcMeshArea
	.public WobbuffetPopBalloon_CalcMeshAreaPercent
	.public WobbuffetPop_LoadHandleSpriteResources
	.public WobbuffetPop_FreeHandleSpriteResources
	.public WobbuffetPop_CreateHandleSprite
	.public WobbuffetPopBalloon_UpdateHandleSprite
	.public WobbuffetPop_DeleteHandleSprite
	.public sWobbuffetPopBalloonTouchLeft
	.public sWobbuffetPopBalloonTouchRight
	.public sWobbuffetPopBalloonTouchTop
	.public sWobbuffetPopBalloonTouchBottom
	.public sWobbuffetPopHandleSpriteTemplate
	.include "overlay_93_arm.inc"
	.include "global.inc"

	.text
	.public WobbuffetPopBalloon_LoadShininessTable
	.public WobbuffetPopBalloon_InitMeshVertices
	.public WobbuffetPopBalloon_InitMeshTexCoords
	.public WobbuffetPopBalloon_GetPushSide
	.public WobbuffetPopBalloon_CalcMeshArea
	.public WobbuffetPopBalloon_CalcMeshAreaPercent
	.public WobbuffetPop_LoadHandleSpriteResources
	.public WobbuffetPop_FreeHandleSpriteResources
	.public WobbuffetPop_CreateHandleSprite
	.public WobbuffetPopBalloon_UpdateHandleSprite
	.public WobbuffetPop_DeleteHandleSprite
	.rodata

_02262C04:
	.byte 0x80

sWobbuffetPopBalloonTouchLeft: ; 0x02262C05
	.byte 0x40

sWobbuffetPopBalloonTouchRight: ; 0x02262C06
	.byte 0xC0

sWobbuffetPopBalloonTouchTop: ; 0x02262C07
	.byte 0x10

sWobbuffetPopBalloonTouchBottom: ; 0x02262C08
	.byte 0xA8, 0x28, 0xD8, 0x00, 0xA8, 0x54, 0xAC, 0x08
	.byte 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0x0F
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00
	.byte 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sWobbuffetPopHandleSpriteTemplate: ; 0x02262C38
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x13, 0x27, 0x00, 0x00
	.byte 0x15, 0x27, 0x00, 0x00, 0x13, 0x27, 0x00, 0x00, 0x13, 0x27, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
