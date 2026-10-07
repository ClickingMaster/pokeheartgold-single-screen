// Encoded instruction bodies and native relocation fields remain assembly-owned.
.include "asm_macro.inc"
.extern RC4_InitAndDecryptInstructions
.extern RC4_InitAndEncryptInstructions
.extern to91_dsprot_clear_cache
.text
arm_func_start Encryptor_DecryptFunction
Encryptor_DecryptFunction:
	.word 0x197b25ae
	.word 0x121bb546
	.word 0x15c95522
	.word 0x11f63557
	.word 0x12d57945
	.word 0x10160557
	.word 0x11f62554
	.word 0x10122557
	.word 0x11f64970
	.word 0x11f66170
	.word 0x11d66950
	.word 0x1072a556
	.word 0x10701552
	.word 0x11f67d70
	.word 0x11d45150
	.word 0x11d74d50
	.word 0x10720555
	.word 0x10728554
	.word 0x12137945
	.word 0x12db6556
	.word 0x11f64557
	.word 0x11f65552
	.word 0x15db1556
	.word 0x15db0552
	.word 0x15db855e
	.word 0x15dba55a
	b RC4_InitAndDecryptInstructions + 0x1308
	.word 0x11f67552
	.word 0x12136945
	b to91_dsprot_clear_cache + 0x1308
	.word 0x12136945
	.word 0x12dbb546
	.word 0x18ebe5ae
	.word DSProt_BSS
arm_func_end Encryptor_DecryptFunction
arm_func_start Encryptor_EncryptFunction
Encryptor_EncryptFunction:
	.word 0x197b25ae
	.word 0x121bb546
	.word 0x15c9552a
	.word 0x11f61556
	.word 0x12d52945
	.word 0x11f60557
	.word 0x10116552
	.word 0x10d61f70
	.word 0x11f63554
	.word 0x11f64971
	.word 0x10133552
	.word 0x11f66171
	.word 0x11d45151
	.word 0x11f67d71
	.word 0x11d74d51
	.word 0x10738555
	.word 0x1071a553
	.word 0x10735554
	.word 0x12107945
	.word 0x15db555e
	.word 0x11d66951
	.word 0x15dba556
	.word 0x1073a556
	.word 0x12db6556
	.word 0x11f64557
	.word 0x11f65553
	.word 0x15db8552
	.word 0x15dba55a
	b RC4_InitAndEncryptInstructions + 0x1308
	.word 0x11f67553
	.word 0x12106945
	b to91_dsprot_clear_cache + 0x1308
	.word 0x10d16552
	.word 0x12dbb546
	.word 0x18ebe5ae
	.word DSProt_BSS
arm_func_end Encryptor_EncryptFunction
