#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_45_thumb.inc"
	.include "global.inc"

	.public WiFiPlazaPlayerObject_ShowStandingSprite
	.public WiFiPlazaPlayerObject_PlayStandingSequence
	.public WiFiPlazaPlayerObject_HoldWalkFrameThenRestart
	.public WiFiPlazaPlayerObject_PlayWalkingSequence
	.public sWiFiPlazaPlayerObjectActions

	.data

sWiFiPlazaPlayerObjectActions: ; 0x02254F28
	.word WiFiPlazaPlayerObject_ShowStandingSprite
	.word WiFiPlazaPlayerObject_HoldWalkFrameThenRestart
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayWalkingSequence
	.word WiFiPlazaPlayerObject_ShowStandingSprite
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word WiFiPlazaPlayerObject_PlayStandingSequence
	.word 0
