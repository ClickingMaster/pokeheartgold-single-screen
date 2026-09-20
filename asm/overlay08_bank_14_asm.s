#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.public ov08_0221BE20
	.public _0221BE2E
	.public _0221BE88
	.public _0221BE8C
	.public _0221BE90
	.public _0221BE94
	.public ov08_0221BE98
	.public _0221BEA6
	.public _0221BEB2
	.public _0221BEE8
	.public _0221BEF4
	.public _0221BF00
	.public _0221BF0C
	.public _0221BF18
	.public _0221BF24
	.public _0221BF30
	.public _0221BF3C
	.public _0221BF48
	.public _0221BF54
	.public _0221BF60
	.public _0221BF6C
	.public _0221BF78
	.public _0221BF84
	.public _0221BF90
	.public _0221BF9C
	.public _0221BFA8
	.public _0221BFB4
	.public _0221BFC0
	.public _0221BFCC
	.public _0221BFD8
	.public _0221BFE4
	.public _0221BFF0
	.public _0221BFFC
	.public _0221C008
	.public _0221C014
	.public _0221C020
	.public _0221C028
	.public _0221C03C
	.public _0221C040
	.public _0221C044
	.public ov08_0221C048
	.public _0221C068
	.public _0221C06C
	.public _0221C0EC
	.public _0221C106
	.public _0221C13C
	.public _0221C140
	.public _0221C144
	.public _0221C148
	.public ov08_0221C14C
	.public _0221C162
	.public _0221C18E
	.public _0221C1B0
	.public _0221C1BA
	.public _0221C1C0
	.public _0221C1C4
	.public ov08_0221C1C8
	.public _0221C1DA
	.public _0221C1E4
	.public _0221C200
	.public _0221C236
	.public _0221C266
	.public _0221C2D6
	.public _0221C2DC
	.public _0221C2E6
	.public _0221C30C
	.public _0221C310
	.public _0221C314
	.public ov08_0221C318
	.public _0221C330
	.public _0221C338
	.public _0221C354
	.public _0221C35E
	.public _0221C380
	.public _0221C3A2
	.public _0221C3BA
	.public _0221C3C0
	.public _0221C3C4
	.public ov08_0221C3C8
	.public _0221C3E0
	.public _0221C3E8
	.public _0221C416
	.public _0221C442
	.public _0221C45A
	.public _0221C478
	.public _0221C47C
	.public _0221C480
	.public _0221C484
	.public ov08_0221C488
	.public _0221C4A2
	.public _0221C4B2
	.public _0221C4EA
	.public _0221C518
	.public _0221C544
	.public _0221C55C
	.public _0221C57A
	.public _0221C580
	.public _0221C584
	.public _0221C588
	.public ov08_0221C58C
	.public _0221C5A6
	.public _0221C5B0
	.public _0221C5CE
	.public _0221C5DE
	.public _0221C5F6
	.public _0221C5FC
	.public _0221C600
	.public ov08_0221C604
	.public _0221C62E
	.public _0221C634
	.public _0221C644
	.public _0221C652
	.public _0221C67C
	.public _0221C6BC
	.public _0221C6DC
	.public _0221C6E0
	.public _0221C6E4
	.public _0221C6E8
	.public _0221C6EC
	.public _0221C6F0
	.public _0221C6F4
	.public ov08_0221C6F8
	.public _0221C722
	.public _0221C728
	.public _0221C736
	.public _0221C750
	.public _0221C758
	.public _0221C77A
	.public _0221C780
	.public _0221C788
	.public _0221C78E
	.public _0221C792
	.public _0221C7D8
	.public _0221C7F6
	.public _0221C7FC
	.public _0221C800
	.public _0221C804
	.public _0221C808
	.public _0221C80C
	.public _0221C810
	.public ov08_0221C814
	.public _0221C842
	.public _0221C848
	.public _0221C858
	.public _0221C862
	.public _0221C8BA
	.public _0221C8E0
	.public _0221C8FA
	.public _0221C900
	.public _0221C904
	.public _0221C908
	.public _0221C90C
	.public _0221C910
	.public _0221C914
	.public ov08_0221C918
	.public ov08_0221C924
	.public ov08_0221C930
	.public ov08_0221C93C
	.public ov08_0221C948
	.public ov08_0221C954
	.public _0221C968
	.public _0221C96E
	.public _0221C974
	.public ov08_0221C978
	.public _0221C994
	.public _0221C99C
	.public _0221C9A0
	.public ov08_0221C9A4
	.public _0221C9C2
	.public ov08_0221C9C8
	.public _0221C9FE
	.public _0221CA04
	.public ov08_0221CA08
	.public _0221CA1C
	.public ov08_0221CA20
	.public _0221CA30
	.public ov08_0221CA34
	.public _0221CA46
	.public _0221CA4C
	.public ov08_0221CA50
	.public _0221CA66
	.public _0221CA6C
	.public _0221CA70
	.public _0221CA74
	.public ov08_0221CA78
	.public _0221CA88
	.public _0221CA8C
	.public ov08_0221CA90
	.public _0221CAA0
	.public _0221CAAC
	.public _0221CAB6
	.public _0221CB00
	.public _0221CB56
	.public _0221CB72
	.public _0221CB7A
	.public _0221CB9C
	.public _0221CBA2
	.public _0221CBD4
	.public _0221CBDA
	.public _0221CBFA
	.public _0221CC1E
	.public _0221CC24
	.public _0221CC28
	.public _0221CC2C
	.public _0221CC30
	.public _0221CC34
	.public ov08_0221CC38
	.public _0221CC54
	.public _0221CC7C
	.public _0221CCA0
	.public _0221CCBE
	.public _0221CCC6
	.public _0221CCDE
	.public _0221CD0C
	.public _0221CD0E
	.public _0221CD28
	.public _0221CD50
	.public _0221CD58
	.public _0221CD5C
	.public _0221CD60
	.public ov08_0221CD64
	.public _0221CD8C
	.public ov08_0221CD90
	.public _0221CDA8
	.public _0221CDF4
	.public ov08_0221CDF8
	.public _0221CEF4
	.public _0221CEF8
	.public _0221CEFC
	.public _0221CF00
	.public _0221CF04
	.public ov08_0221CF08
	.public ov08_0221CF38
	.public _0221D0F0
	.public ov08_0221D0F4
	.public _0221D13C
	.public _0221D140
	.public _0221D144
	.public _0221D148
	.public ov08_0221D14C
	.public _0221D174
	.public _0221D178
	.public _0221D17C
	.public _0221D180
	.public ov08_0221D184
	.public _0221D19E
	.public _0221D1C8
	.public _0221D278
	.public _0221D27E
	.public _0221D316
	.public _0221D322
	.public _0221D3A0
	.public _0221D40C
	.public _0221D416
	.public _0221D432
	.public ov08_0221D438
	.public _0221D462
	.public _0221D46C
	.public _0221D47C
	.public _0221D484
	.public _0221D494
	.public _0221D4A2
	.public _0221D4A8
	.public _0221D4AC
	.public ov08_0221D4B0
	.public _0221D4DA
	.public _0221D4E4
	.public _0221D4EA
	.public _0221D4F0
	.public _0221D4F4
	.public ov08_0221D4F8
	.public _0221D522
	.public _0221D52C
	.public _0221D532
	.public _0221D538
	.public _0221D53C
	.public ov08_0221D540
	.public _0221D56A
	.public _0221D574
	.public _0221D57A
	.public _0221D580
	.public _0221D584
	.public ov08_0221D588
	.public _0221D5B2
	.public _0221D5BC
	.public _0221D5C2
	.public _0221D5C8
	.public _0221D5CC
	.public ov08_0221D5D0
	.public _0221D5D8
	.public ov08_0221D5DC
	.public _0221D5F2
	.public _0221D608
	.public _0221D60C
	.public _0221D610
	.public ov08_0221D614
	.public _0221D648
	.public _0221D656
	.public _0221D65E
	.public _0221D664
	.public _0221D68C
	.public _0221D694
	.public _0221D69A
	.public _0221D6C0
	.public _0221D6C8
	.public ov08_0221D6CC
	.public _0221D6F6
	.public _0221D6FA
	.public _0221D706
	.public _0221D70E
	.public _0221D716
	.public _0221D72E
	.public _0221D734
	.public _0221D74A
	.public ov08_0221D74C
	.public ov08_0221D77C
	.public ov08_0221D81C
	.public _0221D824
	.public _0221D83C
	.public ov08_0221D840
	.public _0221D8AC
	.public ov08_0221D8B0
	.public _0221D8BE
	.public _0221D918
	.public ov08_0221D91C
	.public _0221D978
	.public _0221D9B4
	.public _0221D9CA
	.public _0221DA00
	.public _0221DA1C
	.public _0221DA6C
	.public _0221DAB4
	.public _0221DAB8
	.public _0221DABC
	.public _0221DAC0
	.public ov08_0221DAC4
	.public _0221DADA
	.public _0221DADC
	.public ov08_0221DAE4
	.public _0221DB00
	.public ov08_0221DB04
	.public _0221DB20
	.public ov08_0221DB24
	.public _0221DB44
	.public _0221DB4C
	.public _0221DB4E
	.public _0221DB50
	.public ov08_0221DB54
	.public _0221DB64
	.public _0221DB72
	.public _0221DB78
	.public ov08_0221DB7C
	.public _0221DBAC
	.public _0221DBB0
	.public ov08_0221DBB4
	.public ov08_0221DBCC
	.public _0221DBF0
	.public ov08_0221DC00
	.public _0221DC0C
	.public _0221DC30
	.public _0221DC34
	.public _0221DC38
	.public ov08_0221DC3C
	.public _0221DC50
	.public _0221DC64
	.public _0221DC6E
	.public _0221DC78
	.public _0221DC82
	.public _0221DC8C
	.public _0221DC96
	.public _0221DCA0
	.public _0221DCAA
	.public _0221DCB4
	.public _0221DCBC
	.public _0221DCD8
	.public _0221DCFA
	.public _0221DCFC
	.public _0221DD00
	.public _0221DD04
	.public _0221DD08
	.public _0221DD0C
	.public _0221DD10
	.public _0221DD14
	.public _0221DD18
	.public _0221DD1C
	.public _0221DD20
	.public _0221DD24
	.public ov08_0221DD28
	.public _0221DD38
	.public _0221DD3C
	.public ov08_0221DD40
	.public _0221DD56
	.public _0221DD68
	.public _0221DD6C
	.public ov08_0221DD70
	.public _0221DD82
	.public _0221DD96
	.public _0221DD9C
	.public _0221DDA2
	.public _0221DDA8
	.public _0221DDAE
	.public _0221DDB4
	.public _0221DDBA
	.public _0221DDC0
	.public _0221DDC6
	.public _0221DDCA
	.public ov08_0221DDCC
	.public _0221DE4E
	.public _0221DE6A
	.public _0221DED2
	.public _0221DEE6
	.public _0221DEEE
	.public _0221DF36
	.public _0221DF4A
	.public _0221DF50
	.public _0221DF5C
	.public _0221DF60
	.public _0221DF64
	.public _0221DF68
	.public _0221DF6C
	.public _0221DF70
	.public _0221DF74
	.public _0221DF78
	.public _0221DF7C
	.public ov08_0221DF80
	.public _0221DFC4
	.public ov08_0221DFC8
	.public _0221E040
	.public _0221E044
	.public ov08_0221E048
	.public _0221E082
	.public _0221E08C
	.public _0221E09E
	.public _0221E0A0
	.public _0221E0A4
	.public _0221E0A6
	.public _0221E11C
	.public ov08_0221E120
	.public _0221E198
	.public _0221E19C
	.public _0221E1A0
	.public _0221E1A4
	.public ov08_0221E1A8
	.public _0221E1CC
	.public _0221E202
	.public _0221E234
	.public _0221E238
	.public _0221E23C
	.public _0221E240
	.public ov08_0221E244
	.public _0221E2A8
	.public _0221E2AA
	.public _0221E2DC
	.public _0221E2E0
	.public _0221E2E4
	.public ov08_0221E2E8
	.public _0221E334
	.public _0221E338
	.public _0221E33C
	.public ov08_0221E340
	.public _0221E398
	.public _0221E39C
	.public _0221E3A0
	.public ov08_0221E3A4
	.public _0221E400
	.public _0221E404
	.public ov08_0221E408
	.public _0221E528
	.public _0221E53C
	.public _0221E5C4
	.public _0221E5C8
	.public _0221E5CC
	.public _0221E5D0
	.public _0221E5D4
	.public _0221E5D8
	.public ov08_0221E5DC
	.public _0221E6C0
	.public _0221E6C4
	.public _0221E6C8
	.public _0221E6CC
	.public _0221E6D0
	.public _0221E6D4
	.public ov08_0221E6D8
	.public _0221E7BC
	.public _0221E7C0
	.public _0221E7C4
	.public _0221E7C8
	.public _0221E7CC
	.public _0221E7D0
	.public ov08_0221E7D4
	.public _0221E8BC
	.public _0221E8C0
	.public _0221E8C4
	.public _0221E8C8
	.public _0221E8CC
	.public _0221E8D0
	.public ov08_0221E8D4
	.public _0221E9BC
	.public _0221E9C0
	.public _0221E9C4
	.public _0221E9C8
	.public _0221E9CC
	.public _0221E9D0
	.public ov08_0221E9D4
	.public _0221EABC
	.public _0221EAC0
	.public _0221EAC4
	.public _0221EAC8
	.public _0221EACC
	.public _0221EAD0
	.public ov08_0221EAD4
	.public _0221EC54
	.public _0221EC58
	.public _0221EC5C
	.public _0221EC60
	.public _0221EC64
	.public _0221EC68
	.public ov08_0221EC6C
	.public _0221ECD8
	.public _0221ECDC
	.public _0221ECE0
	.public _0221ECE4
	.public ov08_0221ECE8
	.public _0221ED28
	.public _0221ED2C
	.public ov08_0221ED30
	.public _0221ED8A
	.public _0221EE00
	.public _0221EE0C
	.public _0221EE10
	.public _0221EE14
	.public ov08_0221EE18
	.public _0221EE58
	.public _0221EE5C
	.public ov08_0221EE60
	.public _0221EEBA
	.public _0221EF30
	.public _0221EF3C
	.public _0221EF40
	.public _0221EF44
	.public ov08_0221EF48
	.public _0221EF9C
	.public _0221EFA0
	.public _0221EFA4
	.public ov08_0221EFA8
	.public _0221F004
	.public _0221F008
	.public ov08_0221F00C
	.public _0221F024
	.public _0221F032
	.public _0221F040
	.public _0221F04C
	.public _0221F074
	.public _0221F078
	.public ov08_0221F07C
	.public _0221F1A0
	.public _0221F1A4
	.public _0221F1A8
	.public _0221F1AC
	.public ov08_0221F1B0
	.public _0221F1D0
	.public _0221F1DA
	.public _0221F218
	.public _0221F21C
	.public ov08_0221F220
	.public _0221F236
	.public _0221F23C
	.public _0221F278
	.public _0221F27C
	.public _0221F280
	.public ov08_0221F284
	.public _0221F3C0
	.public _0221F3C4
	.public _0221F3C8
	.public _0221F3CC
	.public ov08_0221F3D0
	.public _0221F494
	.public _0221F498
	.public _0221F49C
	.public _0221F4A0
	.public ov08_0221F4A4
	.public _0221F4C2
	.public _0221F4FE
	.public _0221F512
	.public _0221F52A
	.public _0221F540
	.public _0221F54C
	.public ov08_0221F550
	.public _0221F5AC
	.public ov08_0221F5B0
	.public _0221F5CC
	.public ov08_0221F5D0
	.public _0221F63E
	.public _0221F654
	.public ov08_0221F658
	.public _0221F6DC
	.public _0221F6EC
	.public _0221F704
	.public _0221F714
	.public _0221F778
	.public _0221F794
	.public _0221F79A
	.public _0221F7A4
	.public _0221F7A8
	.public _0221F7AC
	.public _0221F7B0
	.public _0221F7B4
	.public _0221F7B8
	.public _0221F7BC
	.public ov08_0221F7C0
	.public _0221F85A
	.public _0221F89C
	.public _0221F8F0
	.public _0221F8F4
	.public _0221F8F8
	.public _0221F8FC
	.public ov08_0221F900
	.public _0221FB10
	.public _0221FB14
	.public ov08_0221FB18
	.public _0221FC6C
	.public _0221FC70
	.public _0221FC74
	.public _0221FC78
	.public ov08_0221FC7C
	.public _0221FCE8
	.public _0221FD20
	.public _0221FD98
	.public _0221FD9C
	.public _0221FDA0
	.public ov08_0221FDA4
	.public _0221FEE0
	.public _0221FF56
	.public _0221FF64
	.public _0221FF68
	.public _0221FF6C
	.public ov08_0221FF70
	.public _0221FFD0
	.public _02220008
	.public _0222002A
	.public _02220058
	.public _0222005C
	.public _02220060
	.public ov08_02220064
	.public ov08_02220084
	.public _02220176
	.public _022201A4
	.public _022201B0
	.public _022201B4
	.public _022201B8
	.public _022201BC
	.public ov08_022201C0
	.public _022201E4
	.public ov08_022201E8
	.public _0222021C
	.public _02220220
	.public ov08_02220224
	.public _02220278
	.public _0222028C
	.public _022202A0
	.public _022202B4
	.public _022202C8
	.public _022202DC
	.public _022202F0
	.public _0222033A
	.public _02220396
	.public _022203AE
	.public _022203BE
	.public _022203F6
	.public _0222042C
	.public _02220462
	.public _02220498
	.public _022204CE
	.public _02220504
	.public _0222053A
	.public _0222056A
	.public _02220574
	.public _02220578
	.public ov08_0222057C
	.public _022205D4
	.public _022205D8
	.public _022205DC
	.public ov08_022205E0
	.public ov08_0222061C
	.public _02220660
	.public _02220664
	.public ov08_02220668
	.public _022206E4
	.public _0222070E
	.public _02220734
	.public _02220748
	.public _0222074C
	.public ov08_02220750
	.public _022207F4
	.public _022207F8
	.public _022207FC
	.public ov08_02220800
	.public _0222083C
	.public _02220868
	.public _0222086C
	.public _02220870
	.public _02220874
	.public ov08_02220878
	.public _0222091C
	.public _02220920
	.public _02220924
	.public ov08_02220928
	.public _022209AC
	.public _022209B0
	.public _022209B4
	.public ov08_022209B8
	.public _02220A10
	.public _02220A14
	.public _02220A18
	.public _02220A1C
	.public _02220A20
	.public _02220A24
	.public ov08_02220A28
	.public _02220A32
	.public _02220A4C
	.public ov08_02220A50
	.public _02220A64
	.public _02220A84
	.public _02220A88
	.public ov08_02220A8C
	.public ov08_02220AAC
	.public _02220AB8
	.public _02220AD8
	.public _02220AE8
	.public ov08_02220AEC
	.public _02220B38
	.public ov08_02220B3C
	.public _02220B88
	.public _02220B8C
	.public ov08_02220B90
	.public _02220BB0
	.public ov08_02220BB4
	.public _02220BF4
	.public _02220BF8
	.public _02220BFC
	.public _02220C00
	.public _02220C04
	.public ov08_02220C08
	.public _02220C26
	.public _02220C2E
	.public _02220C38
	.public ov08_02220C3C
	.public _02220C5A
	.public ov08_02220C5C
	.public _02220C68
	.public _02220C8A
	.public _02220C9E
	.public _02220CA6
	.public _02220CAE
	.public _02220CB6
	.public _02220CBE
	.public _02220CC6
	.public _02220CCE
	.public _02220CD6
	.public _02220CDE
	.public _02220CE6
	.public _02220CEC
	.public _02220CF0
	.public ov08_02220CF4
	.public _02220D0C
	.public _02220D5A
	.public _02220D78
	.public _02220D7C
	.public _02220D80
	.public _02220D84
	.public _02220D88
	.public _02220D8C
	.public ov08_02220D90
	.public _02220DE4
	.public ov08_02220DE8
	.public _02220E74
	.public _02220E78
	.public _02220E7C
	.public ov08_02220E80
	.public _02220F10
	.public _02220F36
	.public _02220F44
	.public _02220F48
	.public _02220F4C
	.public _02220F50
	.public _02220F54
	.public ov08_02220F58
	.public _02221010
	.public _02221014
	.public _02221018
	.public ov08_0222101C
	.public _02221080
	.public _02221084
	.public ov08_02221088
	.public _0222111E
	.public _02221132
	.public _02221140
	.public _02221144
	.public _02221148
	.public ov08_0222114C
	.public _022211B0
	.public _022211B4
	.public ov08_022211B8
	.public _0222122C
	.public ov08_02221230
	.public _02221290
	.public ov08_02221294
	.public _022212A8
	.public _022212DC
	.public _02221312
	.public _02221314
	.public _02221318
	.public _0222131C
	.public _02221320
	.public _02221324
	.public ov08_02221328
	.public _0222133C
	.public _0222137A
	.public _022213B2
	.public _022213B4
	.public _022213B8
	.public _022213BC
	.public _022213C0
	.public _022213C4
	.public ov08_022213C8
	.public _022213DA
	.public _022213E0
	.public ov08_022213E4
	.public _02221402
	.public ov08_02221404
	.public _02221412
	.public _02221424
	.public _0222143C
	.public _02221446
	.public _0222144A
	.public _0222144E
	.public _02221452
	.public _02221456
	.public ov08_0222145C
	.public _02221470
	.public _022214A2
	.public _022214B4
	.public _022214B8
	.public ov08_022214BC
	.public _022214E2
	.public ov08_02221500
	.public _02221618
	.public _0222161C
	.public _02221620
	.public _02221624
	.public _02221628
	.public ov08_0222162C
	.public _02221688
	.public _0222168C
	.public _02221690
	.public _02221694
	.public ov08_02221698
	.public _022216C0
	.public _022216C4
	.public _022216C8
	.public ov08_022216CC
	.public _022216E4
	.public _022216F0
	.public ov08_022216F4
	.public _0222170C
	.public _02221718
	.public ov08_0222171C
	.public _02221740
	.public _02221754
	.public _0222176E
	.public _02221784
	.public _02221794
	.public _022217A8
	.public _022217BA
	.public _022217BC
	.public _022217C0
	.public _022217C4
	.public ov08_022217C8
	.public _022217EC
	.public ov08_022217F0
	.public _02221AB0
	.public _02221AB4
	.public _02221AB8
	.public _02221ABC
	.public _02221AC0
	.public _02221AC4
	.public _02221AC8
	.public _02221ACC
	.public _02221AD0
	.public _02221AD4
	.public _02221AD8
	.public _02221ADC
	.public _02221AE0
	.public _02221AE4
	.public _02221AE8
	.public _02221AEC
	.public _02221AF0
	.public _02221AF4
	.public _02221AF8
	.public _02221AFC
	.public _02221B00
	.public _02221B04
	.public _02221B08
	.public _02221B0C
	.public _02221B10
	.public _02221B14
	.public _02221B18
	.public ov08_02221B1C
	.public _02221BC4
	.public _02221BC8
	.public _02221BCC
	.public ov08_02221BD0
	.public _02221BE8
	.public _02221C00
	.public _02221C10
	.public _02221C1C
	.public ov08_02221C20
	.public _02221C30
	.public _02221C74
	.public _02221C86
	.public _02221C92
	.public _02221C9E
	.public _02221CAC
	.public _02221CB8
	.public _02221CC4
	.public _02221CD0
	.public _02221CDE
	.public _02221CEE
	.public _02221CFA
	.public _02221D06
	.public _02221D0C
	.public _02221D10
	.public _02221D14
	.public _02221D18
	.public _02221D1C
	.public _02221D20
	.public _02221D24
	.public _02221D28
	.public ov08_02221D2C
	.public _02221D6E
	.public _02221D7A
	.public _02221D82
	.public _02221DB4
	.public _02221DC4
	.public _02221DE2
	.public _02221DF4
	.public _02221E0C
	.public _02221E24
	.public _02221E3C
	.public _02221E48
	.public _02221E5C
	.public _02221E60
	.public _02221E64
	.public _02221E68
	.public ov08_02221E6C
	.public _02221ED8
	.public _02221EDC
	.public _02221EE0
	.public _02221EE4
	.public ov08_02221EE8
	.public _02221F02
	.public _02221F08
	.public _02221F16
	.public _02221F1A
	.public _02221F1E
	.public _02221F20
	.public _02221F54
	.public _02221F56
	.public _02221F86
	.public _02221F88
	.public _02221F8C
	.public _02221F90
	.public _02221F94
	.public ov08_02221F98
	.public _02221FAE
	.public _02221FE6
	.public _02222024
	.public _02222056
	.public _0222206E
	.public _02222086
	.public _0222209C
	.public _022220A0
	.public _022220A4
	.public _022220A8
	.public ov08_022220AC
	.public _022220D6
	.public _022220F4
	.public _022220F8
	.public ov08_022220FC
	.public _0222211C
	.public _02222150
	.public _02222184
	.public _022221BC
	.public _022221C0
	.public _022221C4
	.public _022221C8
	.public ov08_022221CC
	.public _022221D6
	.public _022221E2
	.public _022221F6
	.public _022221FC
	.public _02222218
	.public _0222222C
	.public _0222223E
	.public _02222260
	.public _0222226E
	.public _022222B0
	.public _022222CA
	.public _022222EC
	.public _02222304
	.public _0222231E
	.public _02222340
	.public _02222358
	.public _0222235E
	.public _02222386
	.public _02222398
	.public _022223BC
	.public _022223CC
	.public _022223EA
	.public _022223FC
	.public _02222408
	.public _0222240E
	.public _02222436
	.public _02222448
	.public _02222460
	.public _022224C0
	.public _022224F0
	.public _0222251E
	.public _02222520
	.public ov08_02222524
	.public _02222546
	.public _02222560
	.public ov08_02222564
	.public _0222256C
	.public _02222586
	.public _02222598
	.public _0222259A
	.public ov08_022225A4
	.public _022225C4
	.public ov08_022225D4
	.public _02222614
	.public _02222662
	.public _02222664
	.public _02222668
	.public _0222266C
	.public ov08_02222670
	.public _02222688
	.public _022226A6
	.public _022226B2
	.public _022226BE
	.public _022226CA
	.public _022226D6
	.public _022226E2
	.public _022226EE
	.public _022226FA
	.public _02222706
	.public _02222712
	.public _0222271E
	.public _0222272A
	.public _02222736
	.public _02222742
	.public _0222274E
	.public _02222756
	.public _02222766
	.public _02222768
	.public ov08_0222276C
	.public _022227F0
	.public _0222282A
	.public _02222830
	.public _02222834
	.public _02222838
	.public _0222283C
	.public ov08_02222840
	.public _02222852
	.public _02222878
	.public _0222287E
	.public _0222288E
	.public _0222289A
	.public _022228BA
	.public _022228EA
	.public _02222908
	.public _0222290C
	.public _02222910
	.public _02222914
	.public ov08_02222918
	.public _02222956
	.public _02222974
	.public _02222976
	.public _02222988
	.public _0222298C
	.public _02222992
	.public _022229A2
	.public _022229B4
	.public _022229EA
	.public _02222A04
	.public _02222A32
	.public _02222A60
	.public _02222A64
	.public _02222A68
	.public _02222A6C
	.public _02222A70
	.public _02222A74
	.public ov08_02222A78
	.public _02222AAE
	.public _02222ABC
	.public _02222AC2
	.public _02222AE8
	.public _02222AEC
	.public ov08_02222AF0
	.public _02222B18
	.public _02222B1E
	.public _02222B28
	.public _02222B5E
	.public _02222B78
	.public _02222B7C
	.public _02222B80
	.public _02222B84
	.public _02222B88
	.public ov08_02222B8C
	.public _02222B9C
	.public _02222C0E
	.public _02222C34
	.public _02222C56
	.public _02222C9C
	.public _02222CB6
	.public _02222CDE
	.public _02222D02
	.public _02222D26
	.public _02222D66
	.public _02222D6C
	.public _02222D70
	.public _02222D74
	.public ov08_02222D78
	.public ov08_02222D84
	.public ov08_02222D90
	.public ov08_02222D9C
	.public ov08_02222DAC
	.public _02222DBE
	.public ov08_02222DC4
	.public _02222DDA
	.public _02222DE0
	.public _02222DE4
	.public _02222DE8
	.public ov08_02222DEC
	.public _02222DFA
	.public _02222E00
	.public ov08_02222E04
	.public _02222E28
	.public ov08_02222E2C
	.public _02222E40
	.public _02222E82
	.public _02222EAC
	.public _02222EC0
	.public ov08_02222EC4
	.public _02222ED6
	.public _02222EE0
	.public _02222EEC
	.public _02222EF6
	.public _02222F2E
	.public _02222F38
	.public _02222F48
	.public _02222F88
	.public _02222F92
	.public _02222FA2
	.public _02222FE2
	.public _02222FEA
	.public _02222FF0
	.public _02222FF4
	.public _02222FF8
	.public _02222FFC
	.public ov08_02223000
	.public _022230BC
	.public _022230C0
	.public _022230C4
	.public _022230C8
	.public ov08_022230CC
	.public ov08_022230F4
	.public _022231E4
	.public ov08_022231E8
	.public ov08_02223228
	.public ov08_02223248
	.public _0222325A
	.public _02223274
	.public _02223290
	.public ov08_022232AC
	.public _022232F6
	.public _022232FC
	.public ov08_02223300
	.public _02223364
	.public ov08_02223368
	.public _02223370
	.public ov08_02223374
	.public ov08_02223390
	.public ov08_022233B8
	.public _022233D4
	.public _022233D8
	.public ov08_022233DC
	.public _022233EE
	.public _022233F8
	.public _02223402
	.public _0222340A
	.public _02223426
	.public _02223446
	.public _02223448
	.public _0222344C
	.public _02223450
	.public ov08_02223454
	.public _02223460
	.public ov08_02223464
	.public ov08_02223480
	.public _02223490
	.public _02223496
	.public _0222349C
	.public ov08_022234A4
	.public ov08_022234FC
	.public _02223508
	.public _022235CC
	.public _022235D0
	.public ov08_022235D4
	.public _02223666
	.public _02223670
	.public _02223674
	.public ov08_02223678
	.public _022236FA
	.public _02223704
	.public _02223708
	.public ov08_0222370C
	.public _02223734
	.public _02223736
	.public _02223770
	.public _02223774
	.public _02223778
	.public ov08_0222377C
	.public _0222379E
	.public ov08_022237C4
	.public _022238EC
	.public _022238F0
	.public ov08_022238F4
	.public _0222391C
	.public _02223924
	.public _02223950
	.public _0222397C
	.public _02223994
	.public _022239A8
	.public _022239AC
	.public _022239B0
	.public ov08_022239B4
	.public ov08_022239CC
	.public _02223A34
	.public _02223A38
	.public ov08_02223A3C
	.public _02223A98
	.public _02223A9C
	.public ov08_02223AA0
	.public _02223AAC
	.public _02223B14
	.public _02223B18
	.public _02223B1C
	.public ov08_02223B20
	.public _02223B44
	.public ov08_02223B48
	.public ov08_02223B78
	.public _02223B88
	.public _02223BA2
	.public ov08_02223BA8
	.public _02223BBC
	.public _02223BE6
	.public _02223BF0
	.public ov08_02223BF4
	.public _02223BFE
	.public _02223C0A
	.public _02223C3A
	.public _02223C6C
	.public _02223C74
	.public _02223C7C
	.public _02223C8A
	.public _02223C9A
	.public _02223CA6
	.public _02223CBC
	.public _02223CC8
	.public _02223CCC
	.public _02223CD0
	.public ov08_02223CD4
	.public _02223CFE
	.public _02223D00
	.public _02223D04
	.public ov08_02223D08
	.public ov08_02223D34
	.public _02223D7C
	.public ov08_02223D80
	.public _02223D9E
	.public _02223E38
	.public ov08_02223E3C
	.public ov08_02223E74
	.public ov08_02223EA4
	.public _02223F00
	.public _02223F04
	.public _02223F08
	.public _02223F0C
	.public _02223F10
	.public ov08_02223F14
	.public _02223F20
	.public ov08_02223F34
	.public _02223F4A
	.public ov08_02223F74
	.public ov08_02223F94
	.public _02223FA2
	.public _02223FC0
	.public _02223FC8
	.public _02223FD0
	.public ov08_02223FD8
	.public _02224006
	.public _02224008
	.public ov08_0222400C
	.public _02224016
	.public _0222404E
	.public _0222405C
	.public _02224060
	.public ov08_02224064
	.public _022240A0
	.public _022240A4
	.public ov08_022240A8
	.public _02224104
	.public ov08_02224108
	.public _02224130
	.public ov08_02224134
	.public _02224152
	.public _0222415E
	.public _02224174
	.public _02224178
	.public ov08_0222417C
	.public ov08_0222419C
	.public _022241F0
	.public ov08_022241F4
	.public _02224218
	.public ov08_0222421C
	.public _02224242
	.public _0222424C
	.public _02224250
	.public ov08_02224254
	.public _02224510
	.public _02224514
	.public _02224518
	.public _0222451C
	.public _02224520
	.public _02224524
	.public _02224528
	.public _0222452C
	.public _02224530
	.public _02224534
	.public _02224538
	.public _0222453C
	.public _02224540
	.public _02224544
	.public _02224548
	.public _0222454C
	.public _02224550
	.public _02224554
	.public _02224558
	.public _0222455C
	.public _02224560
	.public _02224564
	.public _02224568
	.public _0222456C
	.public _02224570
	.public _02224574
	.public _02224578
	.public _0222457C
	.public _02224580
	.public _02224584
	.public _02224588
	.public ov08_0222458C
	.public _022245A4
	.public _022245BC
	.public _022245CC
	.public _022245D8
	.public ov08_022245DC
	.public _022245EC
	.public _0222460E
	.public _0222461E
	.public _0222462C
	.public _02224638
	.public _02224644
	.public _02224650
	.public _0222465C
	.public _0222466A
	.public _02224670
	.public _02224674
	.public _02224678
	.public _0222467C
	.public _02224680
	.public ov08_02224684
	.public _0222468C
	.public _0222469C
	.public _022246BE
	.public _022246C2
	.public _022246C6
	.public _022246CA
	.public _022246D6
	.public _022246E6
	.public _022246EA
	.public _022246F0
	.public ov08_022246F4
	.public _02224704
	.public _02224712
	.public _02224720
	.public _0222472C
	.public _0222472E
	.public _02224738
	.public _02224754
	.public _02224758
	.public _0222475C
	.public _02224760
	.public _02224764
	.public ov08_02224768
	.public _022247AA
	.public _022247C4
	.public _022247D4
	.public _022247D8
	.public _022247DC
	.public _022247E0
	.public ov08_022247E4
	.public _02224848
	.public _0222484C
	.public _02224850
	.public _02224854
	.public ov08_02224858
	.public _02224876
	.public _0222487C
	.public _0222488C
	.public _02224894
	.public _0222489A
	.public _0222489C
	.public _022248C8
	.public _022248CC
	.public ov08_022248D0
	.public _022248EC
	.public _022248F6
	.public _02224910
	.public _0222491E
	.public _0222492A
	.public _02224934
	.public ov08_02224938
	.public _02224970
	.public ov08_02224974
	.public _02224994
	.public _022249CC
	.public _02224A04
	.public _02224A44
	.public _02224A48
	.public _02224A4C
	.public ov08_02224A50
	.public _02224A62
	.public _02224AA6
	.public _02224AB2
	.public _02224AC0
	.public _02224AC4
	.public _02224AE2
	.public _02224AF2
	.public _02224B20
	.public _02224B38
	.public _02224B46
	.public _02224B60
	.public ov08_02224B64
	.public _02224B72
	.public ov08_02224B7C
	.public _02224B80
	.public ov08_02224B84
	.public ov08_02224B88
	.public ov08_02224B8C
	.public ov08_02224B90
	.public ov08_02224B94
	.public ov08_02224B98
	.public _02224BBC
	.public ov08_02224BC0
	.public ov08_02224BCC
	.public _02224BF6
	.public ov08_02224BF8
	.public ov08_02224BFC
	.public _02224C0C
	.public _02224C38
	.public _02224C40
	.public _02224C44
	.public ov08_02224C48
	.public _02224C58
	.public _02224C60
	.public _02224C6C
	.public _02224C78
	.public _02224C84
	.public _02224C90
	.public ov08_02224C94
	.public _02224CAA
	.public _02224CCE
	.public _02224CF0
	.public _02224D12
	.public _02224D34
	.public _02224D36
	.public _02224D50
	.public _02224D56
	.public _02224D58
	.public _02224D88
	.public _02224D8C
	.public _02224D90
	.public _02224DD0
	.public _02224DD4
	.public _02224DEE
	.public _02224DF6
	.public _02224E06
	.public _02224E1A
	.public _02224E20
	.public _02224E24
	.public _02224E28
	.public _02224E2C
	.public ov08_02224E34
	.public ov08_02224E44
	.public ov08_02224E54
	.public ov08_02224E68
	.public ov08_02224E7C
	.public ov08_02224E94
	.public ov08_02224EAC
	.public ov08_02224EC8
	.public ov08_02224EE4
	.public ov08_02224F00
	.public ov08_02224F1C
	.public ov08_02224F3C
	.public ov08_02224F5C
	.public ov08_02224F80
	.public ov08_02224FD0
	.public ov08_02224FE0
	.public ov08_02224FF4
	.public ov08_0222500C
	.public ov08_0222502C
	.public ov08_02225054
	.public ov08_02225084
	.public ov08_022250B4
	.public ov08_022250EC
	.public ov08_02225144
	.public ov08_022251A4
	.public ov08_0222522C
	.public ov08_022253B4
	.public ov08_022253C4
	.public ov08_022253D4
	.public ov08_022253E4
	.public ov08_022253F4
	.public ov08_02225404
	.public ov08_0222541C
	.public ov08_02225434
	.public ov08_02225454
	.public ov08_02225474
	.public ov08_02225494
	.public ov08_022254BC
	.public ov08_022254E4
	.public ov08_0222550C
	.public ov08_02225534
	.public ov08_02225564
	.public ov08_02225594
	.public ov08_022255CC
	.public ov08_02225604
	.public ov08_02225644
	.public ov08_02225648
	.public ov08_0222564C
	.public ov08_02225650
	.public ov08_02225654
	.public ov08_02225950
	.public ov08_02225954
	.public ov08_02225958
	.public ov08_0222595C
	.public ov08_02225960
	.public ov08_02225964
	.public ov08_02225968
	.public ov08_0222596C
	.public ov08_02225970
	.public ov08_02225974
	.public ov08_02225978
	.public ov08_0222597C
	.public ov08_02225980
	.public ov08_02225984
	.public ov08_02225988
	.public ov08_0222598C
	.public ov08_02225990
	.public ov08_02225994
	.public ov08_02225998
	.public ov08_0222599C
	.public ov08_022259A0
	.public ov08_022259A4
	.public ov08_022259A8
	.public ov08_022259AC
	.public ov08_022259B0
	.public ov08_022259B4
	.public ov08_022259B8
	.public ov08_022259BC
	.public ov08_022259C0
	.public ov08_022259C6
	.public ov08_022259CC
	.public ov08_02225A54
	.public ov08_02225A55
	.public ov08_02225A56
	.public ov08_02225A57
	.public ov08_02225ADC
	.public ov08_02225AE8
	.public ov08_02225AF8
	.public ov08_02225B14
	.public ov08_02225B30
	.public ov08_02225B4C
	.public ov08_02225B68
	.public ov08_02225B90
	.public ov08_02225B98
	.public ov08_02225BB8
	.public ov08_02225BE0
	.public ov08_02225BE4
	.public ov08_02225C10
	.public ov08_02225CE0
	.public ov08_02225CE8
	.public ov08_02225CF8
	.public ov08_02225D04
	.public ov08_02225D14
	.public ov08_02225D2C
	.public ov08_02225D30
	.public ov08_02225D44
	.public ov08_02225D74
	.public ov08_02225DA4
	.public ov08_02225DDC
	.public ov08_02225DE0
	.public ov08_02225DE4
	.public ov08_02225DE8
	.public ov08_02225DEC
	.public ov08_02225E54
	.public ov08_02225E58
	.public ov08_02225E5C
	.public ov08_02225E60
	.public ov08_02225E64
	.public ov08_02225E68
	.public ov08_02225E6C
	.public ov08_02225E70
	.public ov08_02225E74
	.public ov08_02225E78
	.public ov08_02225E7C
	.public ov08_02225E80
	.public ov08_02225E84
	.public ov08_02225E88
	.public ov08_02225E8C
	.public ov08_02225E90
	.public ov08_02225E94
	.public ov08_02225E98
	.public ov08_02225E9C
	.public ov08_02225E9D
	.public ov08_02225E9E
	.public ov08_02225E9F
	.public ov08_02225EE0

	.text
	.force_active on


	thumb_func_start ov08_0221DD40
ov08_0221DD40: ; 0x0221DD40
	push {r3, r4, r5, lr}
	ldr r1, _0221DD68 ; =0x00002070
	add r5, r0, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldrb r1, [r5, r1]
	bl WindowArray_Delete
	ldr r0, _0221DD6C ; =0x00002050
	mov r4, #0
	add r5, r5, r0
_0221DD56:
	add r0, r5, #0
	bl RemoveWindow
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #2
	blo _0221DD56
	pop {r3, r4, r5, pc}
	nop
_0221DD68: .word 0x00002070
_0221DD6C: .word 0x00002050
	thumb_func_end ov08_0221DD40



	thumb_func_start ov08_0221DD70
ov08_0221DD70: ; 0x0221DD70
	push {r3, lr}
	cmp r1, #9
	bhi _0221DDCA
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0221DD82: ; jump table
	.short _0221DD96 - _0221DD82 - 2 ; case 0
	.short _0221DD9C - _0221DD82 - 2 ; case 1
	.short _0221DDA2 - _0221DD82 - 2 ; case 2
	.short _0221DDA8 - _0221DD82 - 2 ; case 3
	.short _0221DDAE - _0221DD82 - 2 ; case 4
	.short _0221DDB4 - _0221DD82 - 2 ; case 5
	.short _0221DDBA - _0221DD82 - 2 ; case 6
	.short _0221DDC0 - _0221DD82 - 2 ; case 7
	.short _0221DDBA - _0221DD82 - 2 ; case 8
	.short _0221DDC6 - _0221DD82 - 2 ; case 9
_0221DD96:
	bl ov08_0221F4A4
	pop {r3, pc}
_0221DD9C:
	bl ov08_0221F5D0
	pop {r3, pc}
_0221DDA2:
	bl ov08_0221F900
	pop {r3, pc}
_0221DDA8:
	bl ov08_0221F7C0
	pop {r3, pc}
_0221DDAE:
	bl ov08_0221FB18
	pop {r3, pc}
_0221DDB4:
	bl ov08_0221FF70
	pop {r3, pc}
_0221DDBA:
	bl ov08_0221FC7C
	pop {r3, pc}
_0221DDC0:
	bl ov08_0221FDA4
	pop {r3, pc}
_0221DDC6:
	bl ov08_02220084
_0221DDCA:
	pop {r3, pc}
	thumb_func_end ov08_0221DD70



	thumb_func_start ov08_0221DDCC
ov08_0221DDCC: ; 0x0221DDCC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	add r6, r0, #0
	ldr r0, _0221DF5C ; =0x00002070
	str r3, [sp, #0x14]
	ldr r5, [r6, r0]
	lsl r4, r1, #4
	ldr r0, [sp, #0x14]
	mov r1, #0x50
	mul r1, r0
	add r7, r6, #4
	add r0, r7, r1
	str r1, [sp, #0x24]
	ldr r1, [r6]
	str r0, [sp, #0x20]
	ldr r1, [r1, #0xc]
	mov r0, #0xc
	str r2, [sp, #0x10]
	bl String_New
	ldr r1, [sp, #0x14]
	str r0, [sp, #0x1c]
	ldr r0, _0221DF60 ; =0x00001FA8
	lsl r2, r1, #2
	ldr r1, _0221DF64 ; =ov08_02224FF4
	ldr r0, [r6, r0]
	ldr r1, [r1, r2]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x24]
	ldr r0, [r7, r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _0221DF68 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r6, r0]
	bl BufferBoxMonNickname
	ldr r0, _0221DF68 ; =0x00001FAC
	ldr r1, [sp, #0x1c]
	ldr r0, [r6, r0]
	ldr r2, [sp, #0x18]
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x10]
	add r3, sp, #0x38
	cmp r0, #0
	bne _0221DE4E
	ldrb r7, [r3, #0x14]
	mov r0, #0xff
	ldr r1, [sp, #0x10]
	str r7, [sp]
	str r0, [sp, #4]
	ldr r0, _0221DF6C ; =0x000F0E00
	ldr r2, [sp, #0x1c]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldrb r3, [r3, #0x10]
	add r0, r5, r4
	bl AddTextPrinterParameterizedWithColor
	b _0221DE6A
_0221DE4E:
	ldrb r7, [r3, #0x14]
	mov r0, #0xff
	ldr r1, [sp, #0x10]
	str r7, [sp]
	str r0, [sp, #4]
	ldr r0, _0221DF70 ; =0x00070809
	ldr r2, [sp, #0x1c]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldrb r3, [r3, #0x10]
	add r0, r5, r4
	bl AddTextPrinterParameterizedWithColor
_0221DE6A:
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, [sp, #0x1c]
	bl String_Delete
	ldr r0, [sp, #0x20]
	ldrb r0, [r0, #0x16]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1f
	bne _0221DF50
	ldr r0, [sp, #0x20]
	ldrb r0, [r0, #0x17]
	lsl r1, r0, #0x18
	lsr r1, r1, #0x1f
	bne _0221DF50
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1d
	bne _0221DEEE
	ldr r0, _0221DF60 ; =0x00001FA8
	mov r1, #0x10
	ldr r0, [r6, r0]
	bl NewString_ReadMsgData
	add r6, r0, #0
	add r0, r5, r4
	bl GetWindowWidth
	str r0, [sp, #0x28]
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	ldr r1, [sp, #0x28]
	str r7, [sp]
	lsl r1, r1, #3
	sub r3, r1, r0
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0221DED2
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221DF74 ; =0x00070800
	mov r1, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r2, r6, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	b _0221DEE6
_0221DED2:
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221DF78 ; =0x000A0B00
	mov r1, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r2, r6, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
_0221DEE6:
	add r0, r6, #0
	bl String_Delete
	b _0221DF50
_0221DEEE:
	cmp r0, #1
	bne _0221DF50
	ldr r0, _0221DF60 ; =0x00001FA8
	mov r1, #0x11
	ldr r0, [r6, r0]
	bl NewString_ReadMsgData
	add r6, r0, #0
	add r0, r5, r4
	bl GetWindowWidth
	str r0, [sp, #0x2c]
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	ldr r1, [sp, #0x2c]
	str r7, [sp]
	lsl r1, r1, #3
	sub r3, r1, r0
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0221DF36
	mov r0, #0xff
	str r0, [sp, #4]
	mov r0, #0xc1
	lsl r0, r0, #0xa
	str r0, [sp, #8]
	mov r1, #0
	add r0, r5, r4
	add r2, r6, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	b _0221DF4A
_0221DF36:
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221DF7C ; =0x000C0D00
	mov r1, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r2, r6, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
_0221DF4A:
	add r0, r6, #0
	bl String_Delete
_0221DF50:
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221DF5C: .word 0x00002070
_0221DF60: .word 0x00001FA8
_0221DF64: .word ov08_02224FF4
_0221DF68: .word 0x00001FAC
_0221DF6C: .word 0x000F0E00
_0221DF70: .word 0x00070809
_0221DF74: .word 0x00070800
_0221DF78: .word 0x000A0B00
_0221DF7C: .word 0x000C0D00
	thumb_func_end ov08_0221DDCC



	thumb_func_start ov08_0221DF80
ov08_0221DF80: ; 0x0221DF80
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _0221DFC4 ; =0x00002070
	lsl r4, r1, #4
	ldr r1, [r5, r0]
	add r3, #8
	add r1, r1, r4
	str r1, [sp, #4]
	str r3, [sp, #8]
	add r1, sp, #0x10
	ldrb r1, [r1, #0x10]
	mov r3, #0x50
	mul r3, r2
	str r1, [sp, #0xc]
	add r2, r5, r3
	ldrb r2, [r2, #0x1a]
	sub r0, #0xcc
	ldr r0, [r5, r0]
	lsl r2, r2, #0x19
	mov r1, #1
	lsr r2, r2, #0x19
	mov r3, #3
	bl sub_0200CE7C
	ldr r0, _0221DFC4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221DFC4: .word 0x00002070
	thumb_func_end ov08_0221DF80



	thumb_func_start ov08_0221DFC8
ov08_0221DFC8: ; 0x0221DFC8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0x50
	add r7, r3, #0
	mul r0, r2
	add r3, r5, #4
	add r0, r3, r0
	str r0, [sp, #0xc]
	add r0, sp, #0x18
	ldrb r6, [r0, #0x10]
	ldr r0, _0221E040 ; =0x00002070
	lsl r4, r1, #4
	ldr r1, [r5, r0]
	sub r0, #0xcc
	add r1, r1, r4
	str r1, [sp]
	str r7, [sp, #4]
	str r6, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r0, [r5, r0]
	ldrh r1, [r1, #0x10]
	mov r2, #3
	mov r3, #1
	bl PrintUIntOnWindow
	add r3, r7, #0
	ldr r2, _0221E044 ; =0x00001FA4
	str r6, [sp]
	ldr r0, [r5, r2]
	add r2, #0xcc
	ldr r2, [r5, r2]
	mov r1, #0
	add r2, r2, r4
	add r3, #0x18
	bl sub_0200CDAC
	ldr r0, _0221E040 ; =0x00002070
	add r7, #0x20
	ldr r1, [r5, r0]
	sub r0, #0xcc
	add r1, r1, r4
	str r1, [sp]
	str r7, [sp, #4]
	str r6, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r0, [r5, r0]
	ldrh r1, [r1, #0x12]
	mov r2, #3
	mov r3, #0
	bl PrintUIntOnWindow
	ldr r0, _0221E040 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221E040: .word 0x00002070
_0221E044: .word 0x00001FA4
	thumb_func_end ov08_0221DFC8



	thumb_func_start ov08_0221E048
ov08_0221E048: ; 0x0221E048
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r4, r0, #0
	mov r0, #0x50
	str r1, [sp, #8]
	mul r0, r2
	add r1, r4, #4
	str r3, [sp, #0xc]
	add r5, r1, r0
	ldrh r0, [r5, #0x10]
	ldrh r1, [r5, #0x12]
	mov r2, #0x30
	mov r7, #1
	bl CalculateHpBarPixelsLength
	str r0, [sp, #0x14]
	ldrh r0, [r5, #0x10]
	ldrh r1, [r5, #0x12]
	mov r2, #0x30
	bl CalculateHpBarColor
	cmp r0, #4
	bhi _0221E0A6
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0221E082: ; jump table
	.short _0221E08C - _0221E082 - 2 ; case 0
	.short _0221E0A4 - _0221E082 - 2 ; case 1
	.short _0221E0A0 - _0221E082 - 2 ; case 2
	.short _0221E09E - _0221E082 - 2 ; case 3
	.short _0221E09E - _0221E082 - 2 ; case 4
_0221E08C:
	ldr r0, _0221E11C ; =0x00002070
	ldr r1, [r4, r0]
	ldr r0, [sp, #8]
	lsl r0, r0, #4
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
_0221E09E:
	b _0221E0A6
_0221E0A0:
	mov r7, #3
	b _0221E0A6
_0221E0A4:
	mov r7, #5
_0221E0A6:
	add r0, sp, #0x20
	ldrb r6, [r0, #0x10]
	add r0, r7, #1
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	lsl r5, r0, #4
	ldr r0, [sp, #0x14]
	add r3, r6, #1
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E11C ; =0x00002070
	lsl r1, r1, #0x18
	ldr r0, [r4, r0]
	lsl r3, r3, #0x10
	ldr r2, [sp, #0xc]
	add r0, r0, r5
	lsr r1, r1, #0x18
	lsr r3, r3, #0x10
	bl FillWindowPixelRect
	ldr r0, [sp, #0x14]
	add r3, r6, #2
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _0221E11C ; =0x00002070
	lsl r3, r3, #0x10
	ldr r0, [r4, r0]
	ldr r2, [sp, #0xc]
	add r0, r0, r5
	add r1, r7, #0
	lsr r3, r3, #0x10
	bl FillWindowPixelRect
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x10]
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E11C ; =0x00002070
	add r3, r6, #4
	ldr r0, [r4, r0]
	lsl r1, r1, #0x18
	lsl r3, r3, #0x10
	ldr r2, [sp, #0xc]
	add r0, r0, r5
	lsr r1, r1, #0x18
	lsr r3, r3, #0x10
	bl FillWindowPixelRect
	ldr r0, _0221E11C ; =0x00002070
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E11C: .word 0x00002070
	thumb_func_end ov08_0221E048



	thumb_func_start ov08_0221E120
ov08_0221E120: ; 0x0221E120
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r5]
	mov r0, #0x10
	ldr r1, [r1, #0xc]
	add r7, r2, #0
	bl String_New
	add r6, r0, #0
	ldr r0, _0221E198 ; =0x00001FA8
	mov r1, #8
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r2, #0x50
	mul r2, r7
	str r0, [sp, #0x10]
	ldr r0, _0221E19C ; =0x00001FAC
	add r2, r5, r2
	ldrh r2, [r2, #0x1c]
	ldr r0, [r5, r0]
	mov r1, #0
	bl BufferAbilityName
	ldr r0, _0221E19C ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E1A0 ; =0x000F0E00
	lsl r4, r4, #4
	str r0, [sp, #8]
	ldr r0, _0221E1A4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r2, r6, #0
	add r0, r0, r4
	add r3, r1, #0
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E1A4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E198: .word 0x00001FA8
_0221E19C: .word 0x00001FAC
_0221E1A0: .word 0x000F0E00
_0221E1A4: .word 0x00002070
	thumb_func_end ov08_0221E120



	thumb_func_start ov08_0221E1A8
ov08_0221E1A8: ; 0x0221E1A8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #0x50
	add r7, r1, #0
	add r1, r4, #4
	mul r0, r2
	add r5, r1, r0
	ldrh r0, [r5, #0x1a]
	cmp r0, #0
	bne _0221E1CC
	ldr r0, _0221E234 ; =0x00001FA8
	mov r1, #0x14
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	add r6, r0, #0
	b _0221E202
_0221E1CC:
	ldr r1, [r4]
	mov r0, #0x12
	ldr r1, [r1, #0xc]
	bl String_New
	add r6, r0, #0
	ldr r0, _0221E234 ; =0x00001FA8
	mov r1, #9
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x10]
	ldr r0, _0221E238 ; =0x00001FAC
	ldrh r2, [r5, #0x1a]
	ldr r0, [r4, r0]
	mov r1, #0
	bl BufferItemName
	ldr r0, _0221E238 ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r4, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x10]
	bl String_Delete
_0221E202:
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E23C ; =0x000F0E00
	lsl r5, r7, #4
	str r0, [sp, #8]
	ldr r0, _0221E240 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r4, r0]
	add r2, r6, #0
	add r0, r0, r5
	add r3, r1, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E240 ; =0x00002070
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E234: .word 0x00001FA8
_0221E238: .word 0x00001FAC
_0221E23C: .word 0x000F0E00
_0221E240: .word 0x00002070
	thumb_func_end ov08_0221E1A8



	thumb_func_start ov08_0221E244
ov08_0221E244: ; 0x0221E244
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	str r1, [sp, #0x10]
	ldr r1, [r5]
	ldr r0, _0221E2DC ; =0x00002070
	ldr r1, [r1, #0xc]
	ldr r4, [r5, r0]
	mov r0, #0x10
	str r3, [sp, #0x14]
	lsl r6, r2, #4
	bl String_New
	add r7, r0, #0
	ldr r0, _0221E2E0 ; =0x00001FA8
	ldr r1, [sp, #0x14]
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	ldr r0, _0221E2E4 ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	mov r1, #0
	bl BufferMoveName
	ldr r0, _0221E2E4 ; =0x00001FAC
	ldr r2, [sp, #0x18]
	ldr r0, [r5, r0]
	add r1, r7, #0
	bl StringExpandPlaceholders
	add r0, sp, #0x20
	ldrh r0, [r0, #0x10]
	cmp r0, #4
	bne _0221E2A8
	add r0, r4, r6
	bl GetWindowWidth
	add r5, r0, #0
	add r0, sp, #0x20
	ldrh r0, [r0, #0x10]
	add r1, r7, #0
	mov r2, #0
	bl FontID_String_GetWidth
	lsl r1, r5, #3
	sub r0, r1, r0
	lsr r3, r0, #1
	b _0221E2AA
_0221E2A8:
	mov r3, #0
_0221E2AA:
	add r1, sp, #0x20
	ldrh r0, [r1, #0x14]
	add r2, r7, #0
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0x38]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldrh r1, [r1, #0x10]
	add r0, r4, r6
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x18]
	bl String_Delete
	add r0, r7, #0
	bl String_Delete
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E2DC: .word 0x00002070
_0221E2E0: .word 0x00001FA8
_0221E2E4: .word 0x00001FAC
	thumb_func_end ov08_0221E244



	thumb_func_start ov08_0221E2E8
ov08_0221E2E8: ; 0x0221E2E8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r4, r0, #0
	ldr r0, _0221E334 ; =0x00001FA8
	str r2, [sp, #0x10]
	add r5, r1, #0
	ldr r0, [r4, r0]
	add r6, r3, #0
	mov r1, #0xe
	bl NewString_ReadMsgData
	add r7, r0, #0
	str r6, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E338 ; =0x000F0E00
	mov r1, #0
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, _0221E33C ; =0x00002070
	lsl r5, r5, #4
	ldr r0, [r4, r0]
	ldr r3, [sp, #0x10]
	add r0, r0, r5
	add r2, r7, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	ldr r0, _0221E33C ; =0x00002070
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_0221E334: .word 0x00001FA8
_0221E338: .word 0x000F0E00
_0221E33C: .word 0x00002070
	thumb_func_end ov08_0221E2E8



	thumb_func_start ov08_0221E340
ov08_0221E340: ; 0x0221E340
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r0, #0
	ldr r0, _0221E398 ; =0x00002050
	add r5, r1, #0
	mov r1, #1
	add r0, r4, r0
	add r2, r1, #0
	mov r3, #0xe
	bl DrawFrameAndWindow2
	ldr r0, _0221E398 ; =0x00002050
	mov r1, #0xf
	add r0, r4, r0
	bl FillWindowPixelBuffer
	ldr r0, _0221E39C ; =0x00001FA8
	add r1, r5, #0
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	add r5, r0, #0
	mov r3, #0
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E3A0 ; =0x00010200
	mov r1, #1
	str r0, [sp, #8]
	ldr r0, _0221E398 ; =0x00002050
	add r2, r5, #0
	add r0, r4, r0
	str r3, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r5, #0
	bl String_Delete
	ldr r0, _0221E398 ; =0x00002050
	add r0, r4, r0
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221E398: .word 0x00002050
_0221E39C: .word 0x00001FA8
_0221E3A0: .word 0x00010200
	thumb_func_end ov08_0221E340



	thumb_func_start ov08_0221E3A4
ov08_0221E3A4: ; 0x0221E3A4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r3, _0221E400 ; =0x00002070
	lsl r4, r1, #4
	ldr r5, [r0, r3]
	sub r3, #0xc8
	ldr r0, [r0, r3]
	add r1, r2, #0
	bl NewString_ReadMsgData
	add r7, r0, #0
	mov r0, #4
	add r1, r7, #0
	mov r2, #0
	bl FontID_String_GetWidth
	add r6, r0, #0
	add r0, r5, r4
	bl GetWindowWidth
	add r3, r0, #0
	mov r0, #5
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E404 ; =0x00070809
	lsl r3, r3, #3
	str r0, [sp, #8]
	mov r0, #0
	sub r3, r3, r6
	str r0, [sp, #0xc]
	add r0, r5, r4
	mov r1, #4
	add r2, r7, #0
	lsr r3, r3, #1
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221E400: .word 0x00002070
_0221E404: .word 0x00070809
	thumb_func_end ov08_0221E3A4



	thumb_func_start ov08_0221E408
ov08_0221E408: ; 0x0221E408
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r0, #0
	mov r0, #0x50
	mul r0, r1
	add r2, r5, #4
	add r4, r2, r0
	ldr r1, _0221E5C4 ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r5, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	ldr r0, [r5, r1]
	mov r1, #0x17
	bl NewString_ReadMsgData
	mov r1, #0
	add r7, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r2, _0221E5C8 ; =0x000F0E00
	add r0, #0x31
	str r2, [sp, #8]
	ldr r2, _0221E5CC ; =0x00002070
	str r1, [sp, #0xc]
	ldr r2, [r5, r2]
	add r3, r1, #0
	add r0, r2, r0
	add r2, r7, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	ldr r0, _0221E5D0 ; =0x00001FA8
	mov r1, #0x18
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x18]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	str r0, [sp, #0x1c]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldrb r2, [r4, #0x16]
	ldr r0, _0221E5D4 ; =0x00001FAC
	mov r3, #3
	lsl r2, r2, #0x19
	ldr r0, [r5, r0]
	lsr r2, r2, #0x19
	bl BufferIntegerAsString
	ldr r0, _0221E5D4 ; =0x00001FAC
	ldr r1, [sp, #0x1c]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x18]
	bl StringExpandPlaceholders
	mov r1, #0
	add r0, r6, #0
	add r0, #0xb
	lsl r7, r0, #4
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E5C8 ; =0x000F0E00
	ldr r2, [sp, #0x1c]
	str r0, [sp, #8]
	ldr r0, _0221E5CC ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, r0, r7
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, [sp, #0x1c]
	bl String_Delete
	ldr r0, _0221E5D0 ; =0x00001FA8
	mov r1, #0x19
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	str r0, [sp, #0x20]
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r2, _0221E5C8 ; =0x000F0E00
	add r0, #0x41
	str r2, [sp, #8]
	ldr r2, _0221E5CC ; =0x00002070
	str r1, [sp, #0xc]
	ldr r2, [r5, r2]
	add r3, r1, #0
	add r0, r2, r0
	ldr r2, [sp, #0x20]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x20]
	bl String_Delete
	ldr r0, _0221E5D0 ; =0x00001FA8
	mov r1, #0x1a
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x14]
	ldr r1, [r1, #0xc]
	mov r0, #0xe
	bl String_New
	str r0, [sp, #0x10]
	ldrb r0, [r4, #0x16]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0x64
	bhs _0221E528
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0221E5D4 ; =0x00001FAC
	ldr r2, [r4, #0x24]
	ldr r3, [r4, #0x1c]
	ldr r0, [r5, r0]
	sub r2, r2, r3
	mov r1, #0
	mov r3, #6
	bl BufferIntegerAsString
	b _0221E53C
_0221E528:
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0221E5D4 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	add r2, r1, #0
	mov r3, #6
	bl BufferIntegerAsString
_0221E53C:
	ldr r0, _0221E5D4 ; =0x00001FAC
	ldr r1, [sp, #0x10]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x14]
	bl StringExpandPlaceholders
	ldr r0, _0221E5CC ; =0x00002070
	add r6, #0xc
	ldr r0, [r5, r0]
	lsl r4, r6, #4
	add r0, r0, r4
	bl GetWindowWidth
	add r6, r0, #0
	mov r0, #0
	ldr r1, [sp, #0x10]
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r1, r6, #3
	sub r0, r1, r0
	lsl r0, r0, #0x10
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E5D8 ; =0x00010200
	ldr r2, [sp, #0x10]
	str r0, [sp, #8]
	ldr r0, _0221E5CC ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x14]
	bl String_Delete
	ldr r0, [sp, #0x10]
	bl String_Delete
	ldr r0, _0221E5CC ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #0x13
	lsl r0, r0, #4
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E5CC ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r7
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E5CC ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #5
	lsl r0, r0, #6
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E5CC ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E5C4: .word 0x00002075
_0221E5C8: .word 0x000F0E00
_0221E5CC: .word 0x00002070
_0221E5D0: .word 0x00001FA8
_0221E5D4: .word 0x00001FAC
_0221E5D8: .word 0x00010200
	thumb_func_end ov08_0221E408



	thumb_func_start ov08_0221E5DC
ov08_0221E5DC: ; 0x0221E5DC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r1, #0
	add r5, r0, #0
	ldr r1, _0221E6C0 ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r5, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, [r5, r1]
	mov r1, #0x20
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E6C4 ; =0x000F0E00
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E6C8 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, #0xe0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E6CC ; =0x00001FA8
	mov r1, #0x21
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x10]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	mov r2, #0x50
	add r6, r0, #0
	mul r2, r7
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E6D0 ; =0x00001FAC
	add r2, r5, r2
	ldrh r2, [r2, #0xa]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221E6D0 ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	add r0, r4, #5
	lsl r4, r0, #4
	ldr r0, _0221E6C8 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #0x18
	mov r1, #0
	lsr r3, r0, #0x18
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E6D4 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E6C8 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E6C8 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0xe0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E6C8 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_0221E6C0: .word 0x00002075
_0221E6C4: .word 0x000F0E00
_0221E6C8: .word 0x00002070
_0221E6CC: .word 0x00001FA8
_0221E6D0: .word 0x00001FAC
_0221E6D4: .word 0x00010200
	thumb_func_end ov08_0221E5DC



	thumb_func_start ov08_0221E6D8
ov08_0221E6D8: ; 0x0221E6D8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r1, #0
	add r5, r0, #0
	ldr r1, _0221E7BC ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r5, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, [r5, r1]
	mov r1, #0x22
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E7C0 ; =0x000F0E00
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E7C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, #0xf0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E7C8 ; =0x00001FA8
	mov r1, #0x23
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x10]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	mov r2, #0x50
	add r6, r0, #0
	mul r2, r7
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E7CC ; =0x00001FAC
	add r2, r5, r2
	ldrh r2, [r2, #0xc]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221E7CC ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	add r0, r4, #6
	lsl r4, r0, #4
	ldr r0, _0221E7C4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #0x18
	mov r1, #0
	lsr r3, r0, #0x18
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E7D0 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E7C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E7C4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0xf0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E7C4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_0221E7BC: .word 0x00002075
_0221E7C0: .word 0x000F0E00
_0221E7C4: .word 0x00002070
_0221E7C8: .word 0x00001FA8
_0221E7CC: .word 0x00001FAC
_0221E7D0: .word 0x00010200
	thumb_func_end ov08_0221E6D8



	thumb_func_start ov08_0221E7D4
ov08_0221E7D4: ; 0x0221E7D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r1, #0
	add r5, r0, #0
	ldr r1, _0221E8BC ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r5, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, [r5, r1]
	mov r1, #0x28
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r2, #0xff
	str r2, [sp, #4]
	ldr r0, _0221E8C0 ; =0x000F0E00
	add r2, r2, #1
	str r0, [sp, #8]
	ldr r0, _0221E8C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, r0, r2
	add r2, r6, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E8C8 ; =0x00001FA8
	mov r1, #0x29
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x10]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	mov r2, #0x50
	add r6, r0, #0
	mul r2, r7
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E8CC ; =0x00001FAC
	add r2, r5, r2
	ldrh r2, [r2, #0xe]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221E8CC ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	add r0, r4, #7
	lsl r4, r0, #4
	ldr r0, _0221E8C4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #0x18
	mov r1, #0
	lsr r3, r0, #0x18
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E8D0 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E8C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E8C4 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #1
	lsl r0, r0, #8
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E8C4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E8BC: .word 0x00002075
_0221E8C0: .word 0x000F0E00
_0221E8C4: .word 0x00002070
_0221E8C8: .word 0x00001FA8
_0221E8CC: .word 0x00001FAC
_0221E8D0: .word 0x00010200
	thumb_func_end ov08_0221E7D4



	thumb_func_start ov08_0221E8D4
ov08_0221E8D4: ; 0x0221E8D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r1, #0
	add r4, r0, #0
	ldr r1, _0221E9BC ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r4, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, [r4, r1]
	mov r1, #0x24
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r2, _0221E9C0 ; =0x000F0E00
	add r0, #0x11
	str r2, [sp, #8]
	ldr r2, _0221E9C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r2, [r4, r2]
	add r3, r1, #0
	add r0, r2, r0
	add r2, r6, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E9C8 ; =0x00001FA8
	mov r1, #0x25
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	ldr r1, [r4]
	str r0, [sp, #0x10]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	mov r2, #0x50
	add r6, r0, #0
	mul r2, r7
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221E9CC ; =0x00001FAC
	add r2, r4, r2
	ldrh r2, [r2, #0x10]
	ldr r0, [r4, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221E9CC ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r4, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	ldr r0, _0221E9C4 ; =0x00002070
	add r5, #8
	ldr r0, [r4, r0]
	lsl r5, r5, #4
	add r0, r0, r5
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #0x18
	mov r1, #0
	lsr r3, r0, #0x18
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221E9D0 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221E9C4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221E9C4 ; =0x00002070
	ldr r1, [r4, r0]
	mov r0, #0x11
	lsl r0, r0, #4
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221E9C4 ; =0x00002070
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E9BC: .word 0x00002075
_0221E9C0: .word 0x000F0E00
_0221E9C4: .word 0x00002070
_0221E9C8: .word 0x00001FA8
_0221E9CC: .word 0x00001FAC
_0221E9D0: .word 0x00010200
	thumb_func_end ov08_0221E8D4



	thumb_func_start ov08_0221E9D4
ov08_0221E9D4: ; 0x0221E9D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r1, #0
	add r4, r0, #0
	ldr r1, _0221EABC ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r4, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, [r4, r1]
	mov r1, #0x26
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r2, _0221EAC0 ; =0x000F0E00
	add r0, #0x21
	str r2, [sp, #8]
	ldr r2, _0221EAC4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r2, [r4, r2]
	add r3, r1, #0
	add r0, r2, r0
	add r2, r6, #0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221EAC8 ; =0x00001FA8
	mov r1, #0x27
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	ldr r1, [r4]
	str r0, [sp, #0x10]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	mov r2, #0x50
	add r6, r0, #0
	mul r2, r7
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221EACC ; =0x00001FAC
	add r2, r4, r2
	ldrh r2, [r2, #0x12]
	ldr r0, [r4, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221EACC ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r4, r0]
	add r1, r6, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	ldr r0, _0221EAC4 ; =0x00002070
	add r5, #9
	ldr r0, [r4, r0]
	lsl r5, r5, #4
	add r0, r0, r5
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #0x18
	mov r1, #0
	lsr r3, r0, #0x18
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EAD0 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221EAC4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221EAC4 ; =0x00002070
	ldr r1, [r4, r0]
	mov r0, #0x12
	lsl r0, r0, #4
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221EAC4 ; =0x00002070
	ldr r0, [r4, r0]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221EABC: .word 0x00002075
_0221EAC0: .word 0x000F0E00
_0221EAC4: .word 0x00002070
_0221EAC8: .word 0x00001FA8
_0221EACC: .word 0x00001FAC
_0221EAD0: .word 0x00010200
	thumb_func_end ov08_0221E9D4



	thumb_func_start ov08_0221EAD4
ov08_0221EAD4: ; 0x0221EAD4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	mov r0, #0x50
	mul r0, r1
	add r2, r5, #4
	add r7, r2, r0
	ldr r1, _0221EC54 ; =0x00002075
	mov r0, #0x16
	ldrb r2, [r5, r1]
	sub r1, #0xcd
	mul r0, r2
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, [r5, r1]
	mov r1, #0x1c
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EC58 ; =0x000F0E00
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221EC5C ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, #0xd0
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	ldr r0, _0221EC60 ; =0x00001FA8
	mov r1, #0x1f
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x10]
	mov r0, #0
	ldr r1, [sp, #0x10]
	add r2, r0, #0
	bl FontID_String_GetWidth
	str r0, [sp, #0x14]
	ldr r0, _0221EC5C ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x40
	bl GetWindowWidth
	lsl r1, r0, #3
	ldr r0, [sp, #0x14]
	ldr r2, [sp, #0x10]
	sub r0, r1, r0
	lsl r0, r0, #0xf
	lsr r6, r0, #0x10
	add r0, r4, #4
	mov r1, #0
	lsl r4, r0, #4
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EC64 ; =0x00010200
	add r3, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221EC5C ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	ldr r0, _0221EC60 ; =0x00001FA8
	mov r1, #0x1d
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x18]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	str r0, [sp, #0x1c]
	mov r1, #0
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221EC68 ; =0x00001FAC
	ldrh r2, [r7, #0x10]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221EC68 ; =0x00001FAC
	ldr r1, [sp, #0x1c]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x18]
	bl StringExpandPlaceholders
	mov r0, #0
	ldr r1, [sp, #0x1c]
	add r2, r0, #0
	bl FontID_String_GetWidth
	add r3, r0, #0
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EC64 ; =0x00010200
	ldr r2, [sp, #0x1c]
	str r0, [sp, #8]
	ldr r0, _0221EC5C ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	sub r3, r6, r3
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, [sp, #0x1c]
	bl String_Delete
	ldr r0, _0221EC60 ; =0x00001FA8
	mov r1, #0x1e
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x20]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	str r0, [sp, #0x24]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221EC68 ; =0x00001FAC
	ldrh r2, [r7, #0x12]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221EC68 ; =0x00001FAC
	ldr r1, [sp, #0x24]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x20]
	bl StringExpandPlaceholders
	mov r1, #0
	ldr r3, [sp, #0x14]
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EC64 ; =0x00010200
	ldr r2, [sp, #0x24]
	str r0, [sp, #8]
	ldr r0, _0221EC5C ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r6, r3
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x20]
	bl String_Delete
	ldr r0, [sp, #0x24]
	bl String_Delete
	ldr r0, _0221EC5C ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0xd0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221EC5C ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221EC54: .word 0x00002075
_0221EC58: .word 0x000F0E00
_0221EC5C: .word 0x00002070
_0221EC60: .word 0x00001FA8
_0221EC64: .word 0x00010200
_0221EC68: .word 0x00001FAC
	thumb_func_end ov08_0221EAD4



	thumb_func_start ov08_0221EC6C
ov08_0221EC6C: ; 0x0221EC6C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, _0221ECD8 ; =0x00002075
	add r6, r1, #0
	ldrb r1, [r5, r0]
	ldr r3, [r5]
	mov r0, #0x16
	add r4, r1, #0
	mul r4, r0
	ldr r2, _0221ECDC ; =0x000002D2
	ldr r3, [r3, #0xc]
	mov r0, #1
	mov r1, #0x1b
	bl NewMsgDataFromNarc
	mov r1, #0x50
	mul r1, r6
	add r1, r5, r1
	ldrh r1, [r1, #0x1c]
	add r7, r0, #0
	bl NewString_ReadMsgData
	add r6, r0, #0
	mov r1, #0
	add r0, r4, #2
	lsl r4, r0, #4
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221ECE0 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	ldr r0, _0221ECE4 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r7, #0
	bl DestroyMsgData
	ldr r0, _0221ECE4 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221ECD8: .word 0x00002075
_0221ECDC: .word 0x000002D2
_0221ECE0: .word 0x00010200
_0221ECE4: .word 0x00002070
	thumb_func_end ov08_0221EC6C



	thumb_func_start ov08_0221ECE8
ov08_0221ECE8: ; 0x0221ECE8
	push {r4, r5, r6, lr}
	sub sp, #0x10
	ldr r2, _0221ED28 ; =0x00002070
	lsl r4, r1, #4
	ldr r5, [r0, r2]
	sub r2, #0xc8
	ldr r0, [r0, r2]
	mov r1, #0x33
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221ED2C ; =0x000F0E00
	add r2, r6, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_0221ED28: .word 0x00002070
_0221ED2C: .word 0x000F0E00
	thumb_func_end ov08_0221ECE8



	thumb_func_start ov08_0221ED30
ov08_0221ED30: ; 0x0221ED30
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r2, [sp, #0x10]
	add r5, r0, #0
	ldr r2, _0221EE0C ; =0x00002070
	ldr r0, [sp, #0x10]
	lsl r6, r1, #4
	ldr r4, [r5, r2]
	cmp r0, #0
	bne _0221ED8A
	sub r2, #0xc8
	ldr r0, [r5, r2]
	mov r1, #0x32
	bl NewString_ReadMsgData
	add r7, r0, #0
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r4, r6
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r5
	lsl r0, r0, #0x10
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EE10 ; =0x00010200
	add r2, r7, #0
	str r0, [sp, #8]
	add r0, r4, r6
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	b _0221EE00
_0221ED8A:
	sub r2, #0xc8
	ldr r0, [r5, r2]
	mov r1, #0x34
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x14]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	add r7, r0, #0
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221EE14 ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221EE14 ; =0x00001FAC
	ldr r2, [sp, #0x14]
	ldr r0, [r5, r0]
	add r1, r7, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r4, r6
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r5
	lsl r0, r0, #0x10
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EE10 ; =0x00010200
	add r2, r7, #0
	str r0, [sp, #8]
	add r0, r4, r6
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x14]
	bl String_Delete
	add r0, r7, #0
	bl String_Delete
_0221EE00:
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221EE0C: .word 0x00002070
_0221EE10: .word 0x00010200
_0221EE14: .word 0x00001FAC
	thumb_func_end ov08_0221ED30



	thumb_func_start ov08_0221EE18
ov08_0221EE18: ; 0x0221EE18
	push {r4, r5, r6, lr}
	sub sp, #0x10
	ldr r2, _0221EE58 ; =0x00002070
	lsl r4, r1, #4
	ldr r5, [r0, r2]
	sub r2, #0xc8
	ldr r0, [r0, r2]
	mov r1, #0x30
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EE5C ; =0x000F0E00
	add r2, r6, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_0221EE58: .word 0x00002070
_0221EE5C: .word 0x000F0E00
	thumb_func_end ov08_0221EE18



	thumb_func_start ov08_0221EE60
ov08_0221EE60: ; 0x0221EE60
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r2, [sp, #0x10]
	add r5, r0, #0
	ldr r2, _0221EF3C ; =0x00002070
	ldr r0, [sp, #0x10]
	lsl r6, r1, #4
	ldr r4, [r5, r2]
	cmp r0, #1
	bhi _0221EEBA
	sub r2, #0xc8
	ldr r0, [r5, r2]
	mov r1, #0x32
	bl NewString_ReadMsgData
	add r7, r0, #0
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r4, r6
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r5
	lsl r0, r0, #0x10
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EF40 ; =0x00010200
	add r2, r7, #0
	str r0, [sp, #8]
	add r0, r4, r6
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	b _0221EF30
_0221EEBA:
	sub r2, #0xc8
	ldr r0, [r5, r2]
	mov r1, #0x31
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x14]
	ldr r1, [r1, #0xc]
	mov r0, #8
	bl String_New
	mov r1, #0
	add r7, r0, #0
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221EF44 ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221EF44 ; =0x00001FAC
	ldr r2, [sp, #0x14]
	ldr r0, [r5, r0]
	add r1, r7, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r4, r6
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r5
	lsl r0, r0, #0x10
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EF40 ; =0x00010200
	add r2, r7, #0
	str r0, [sp, #8]
	add r0, r4, r6
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x14]
	bl String_Delete
	add r0, r7, #0
	bl String_Delete
_0221EF30:
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221EF3C: .word 0x00002070
_0221EF40: .word 0x00010200
_0221EF44: .word 0x00001FAC
	thumb_func_end ov08_0221EE60



	thumb_func_start ov08_0221EF48
ov08_0221EF48: ; 0x0221EF48
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r3, r0, #0
	ldr r0, _0221EF9C ; =0x00002070
	add r6, r2, #0
	ldr r5, [r3, r0]
	ldr r3, [r3]
	lsl r4, r1, #4
	ldr r2, _0221EFA0 ; =0x000002ED
	ldr r3, [r3, #0xc]
	mov r0, #1
	mov r1, #0x1b
	bl NewMsgDataFromNarc
	add r1, r6, #0
	add r7, r0, #0
	bl NewString_ReadMsgData
	mov r1, #0
	add r6, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EFA4 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r7, #0
	bl DestroyMsgData
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221EF9C: .word 0x00002070
_0221EFA0: .word 0x000002ED
_0221EFA4: .word 0x00010200
	thumb_func_end ov08_0221EF48



	thumb_func_start ov08_0221EFA8
ov08_0221EFA8: ; 0x0221EFA8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r2, _0221F004 ; =0x00002070
	lsl r4, r1, #4
	ldr r5, [r0, r2]
	sub r2, #0xc8
	ldr r0, [r0, r2]
	mov r1, #0x35
	bl NewString_ReadMsgData
	add r7, r0, #0
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r5, r4
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r1, r0, r6
	lsr r0, r1, #0x1f
	add r0, r1, r0
	lsl r0, r0, #0xf
	mov r1, #0
	lsr r3, r0, #0x10
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F008 ; =0x000F0E00
	add r2, r7, #0
	str r0, [sp, #8]
	add r0, r5, r4
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F004: .word 0x00002070
_0221F008: .word 0x000F0E00
	thumb_func_end ov08_0221EFA8



	thumb_func_start ov08_0221F00C
ov08_0221F00C: ; 0x0221F00C
	push {r4, r5, r6, lr}
	sub sp, #0x10
	ldr r3, _0221F074 ; =0x00002070
	lsl r4, r1, #4
	ldr r5, [r0, r3]
	cmp r2, #0
	beq _0221F024
	cmp r2, #1
	beq _0221F032
	cmp r2, #2
	beq _0221F040
	b _0221F04C
_0221F024:
	sub r3, #0xc8
	ldr r0, [r0, r3]
	mov r1, #0x36
	bl NewString_ReadMsgData
	add r6, r0, #0
	b _0221F04C
_0221F032:
	sub r3, #0xc8
	ldr r0, [r0, r3]
	mov r1, #0x38
	bl NewString_ReadMsgData
	add r6, r0, #0
	b _0221F04C
_0221F040:
	sub r3, #0xc8
	ldr r0, [r0, r3]
	mov r1, #0x37
	bl NewString_ReadMsgData
	add r6, r0, #0
_0221F04C:
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F078 ; =0x00010200
	add r2, r6, #0
	str r0, [sp, #8]
	add r0, r5, r4
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0221F074: .word 0x00002070
_0221F078: .word 0x00010200
	thumb_func_end ov08_0221F00C



	thumb_func_start ov08_0221F07C
ov08_0221F07C: ; 0x0221F07C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	add r5, r0, #0
	ldr r0, _0221F1A0 ; =0x00002070
	lsl r6, r1, #4
	ldr r4, [r5, r0]
	sub r0, #0xc8
	ldr r0, [r5, r0]
	mov r1, #0x2e
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	mov r0, #0
	ldr r1, [sp, #0x18]
	add r2, r0, #0
	bl FontID_String_GetWidth
	str r0, [sp, #0x1c]
	add r0, r4, r6
	bl GetWindowWidth
	lsl r1, r0, #3
	ldr r0, [sp, #0x1c]
	ldr r2, [sp, #0x18]
	sub r0, r1, r0
	lsr r7, r0, #1
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F1A4 ; =0x000F0E00
	add r3, r7, #0
	str r0, [sp, #8]
	add r0, r4, r6
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, _0221F1A8 ; =0x00001FA8
	mov r1, #0x2c
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x20]
	ldr r1, [r1, #0xc]
	mov r0, #6
	bl String_New
	mov r1, #0
	str r0, [sp, #0x24]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221F1AC ; =0x00001FAC
	ldr r2, [sp, #0x10]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221F1AC ; =0x00001FAC
	ldr r1, [sp, #0x24]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x20]
	bl StringExpandPlaceholders
	mov r0, #0
	ldr r1, [sp, #0x24]
	add r2, r0, #0
	bl FontID_String_GetWidth
	add r3, r0, #0
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F1A4 ; =0x000F0E00
	ldr r2, [sp, #0x24]
	str r0, [sp, #8]
	add r0, r4, r6
	sub r3, r7, r3
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x20]
	bl String_Delete
	ldr r0, [sp, #0x24]
	bl String_Delete
	ldr r0, _0221F1A8 ; =0x00001FA8
	mov r1, #0x2d
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	ldr r1, [r5]
	str r0, [sp, #0x28]
	ldr r1, [r1, #0xc]
	mov r0, #6
	bl String_New
	mov r1, #0
	str r0, [sp, #0x2c]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, _0221F1AC ; =0x00001FAC
	ldr r2, [sp, #0x14]
	ldr r0, [r5, r0]
	mov r3, #3
	bl BufferIntegerAsString
	ldr r0, _0221F1AC ; =0x00001FAC
	ldr r1, [sp, #0x2c]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x28]
	bl StringExpandPlaceholders
	mov r1, #0
	ldr r3, [sp, #0x1c]
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F1A4 ; =0x000F0E00
	ldr r2, [sp, #0x2c]
	str r0, [sp, #8]
	add r0, r4, r6
	add r3, r7, r3
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x28]
	bl String_Delete
	ldr r0, [sp, #0x2c]
	bl String_Delete
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F1A0: .word 0x00002070
_0221F1A4: .word 0x000F0E00
_0221F1A8: .word 0x00001FA8
_0221F1AC: .word 0x00001FAC
	thumb_func_end ov08_0221F07C



	thumb_func_start ov08_0221F1B0
ov08_0221F1B0: ; 0x0221F1B0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	lsl r4, r1, #4
	ldr r1, [r0]
	ldr r2, _0221F218 ; =0x00002070
	add r1, #0x34
	ldrb r1, [r1]
	ldr r5, [r0, r2]
	cmp r1, #4
	bne _0221F1D0
	sub r2, #0xc8
	ldr r0, [r0, r2]
	mov r1, #0x3b
	bl NewString_ReadMsgData
	b _0221F1DA
_0221F1D0:
	sub r2, #0xc8
	ldr r0, [r0, r2]
	mov r1, #0x3a
	bl NewString_ReadMsgData
_0221F1DA:
	add r6, r0, #0
	mov r0, #4
	add r1, r6, #0
	mov r2, #0
	bl FontID_String_GetWidth
	add r7, r0, #0
	mov r0, #5
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F21C ; =0x00070809
	mov r3, #0x60
	str r0, [sp, #8]
	mov r0, #0
	sub r3, r3, r7
	str r0, [sp, #0xc]
	add r0, r5, r4
	mov r1, #4
	add r2, r6, #0
	lsr r3, r3, #1
	bl AddTextPrinterParameterizedWithColor
	add r0, r6, #0
	bl String_Delete
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F218: .word 0x00002070
_0221F21C: .word 0x00070809
	thumb_func_end ov08_0221F1B0



	thumb_func_start ov08_0221F220
ov08_0221F220: ; 0x0221F220
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r0, #0
	ldr r0, _0221F278 ; =0x0000207A
	ldrb r1, [r4, r0]
	cmp r1, #7
	bne _0221F236
	sub r0, #0xa
	ldr r5, [r4, r0]
	add r5, #0x80
	b _0221F23C
_0221F236:
	sub r0, #0xa
	ldr r5, [r4, r0]
	add r5, #0x50
_0221F23C:
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221F27C ; =0x00001FA8
	mov r1, #0x3c
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	add r4, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F280 ; =0x00010200
	add r2, r4, #0
	str r0, [sp, #8]
	add r0, r5, #0
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r4, #0
	bl String_Delete
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221F278: .word 0x0000207A
_0221F27C: .word 0x00001FA8
_0221F280: .word 0x00010200
	thumb_func_end ov08_0221F220



	thumb_func_start ov08_0221F284
ov08_0221F284: ; 0x0221F284
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	str r1, [sp, #0x10]
	ldr r1, [r5]
	ldr r0, _0221F3C0 ; =0x00002070
	ldr r1, [r1, #0xc]
	ldr r4, [r5, r0]
	mov r0, #6
	lsl r6, r2, #4
	bl String_New
	add r7, r0, #0
	ldr r0, _0221F3C4 ; =0x00001FA8
	mov r1, #0x2b
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x14]
	mov r0, #0x18
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F3C8 ; =0x000F0E00
	ldr r2, [sp, #0x14]
	str r0, [sp, #8]
	mov r1, #0
	add r0, r4, r6
	mov r3, #0x28
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x14]
	bl String_Delete
	ldr r0, _0221F3C4 ; =0x00001FA8
	mov r1, #0x2e
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	mov r0, #0
	ldr r1, [sp, #0x18]
	add r2, r0, #0
	bl FontID_String_GetWidth
	str r0, [sp, #0x1c]
	mov r0, #0x18
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F3C8 ; =0x000F0E00
	ldr r2, [sp, #0x18]
	str r0, [sp, #8]
	mov r1, #0
	add r0, r4, r6
	mov r3, #0x50
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, _0221F3C4 ; =0x00001FA8
	mov r1, #0x2d
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	str r0, [sp, #0x20]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r2, [sp, #0x10]
	ldr r0, _0221F3CC ; =0x00001FAC
	ldrb r2, [r2, #3]
	ldr r0, [r5, r0]
	mov r3, #2
	bl BufferIntegerAsString
	ldr r0, _0221F3CC ; =0x00001FAC
	ldr r2, [sp, #0x20]
	ldr r0, [r5, r0]
	add r1, r7, #0
	bl StringExpandPlaceholders
	mov r0, #0x18
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F3C8 ; =0x000F0E00
	ldr r3, [sp, #0x1c]
	str r0, [sp, #8]
	mov r1, #0
	add r3, #0x50
	add r0, r4, r6
	add r2, r7, #0
	str r1, [sp, #0xc]
	str r3, [sp, #0x1c]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x20]
	bl String_Delete
	ldr r0, _0221F3C4 ; =0x00001FA8
	mov r1, #0x2c
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	str r0, [sp, #0x24]
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r2, [sp, #0x10]
	ldr r0, _0221F3CC ; =0x00001FAC
	ldrb r2, [r2, #2]
	ldr r0, [r5, r0]
	mov r3, #2
	bl BufferIntegerAsString
	ldr r0, _0221F3CC ; =0x00001FAC
	ldr r2, [sp, #0x24]
	ldr r0, [r5, r0]
	add r1, r7, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r7, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	add r5, r0, #0
	mov r0, #0x18
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F3C8 ; =0x000F0E00
	mov r3, #0x50
	str r0, [sp, #8]
	mov r1, #0
	add r0, r4, r6
	add r2, r7, #0
	sub r3, r3, r5
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x24]
	bl String_Delete
	add r0, r7, #0
	bl String_Delete
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F3C0: .word 0x00002070
_0221F3C4: .word 0x00001FA8
_0221F3C8: .word 0x000F0E00
_0221F3CC: .word 0x00001FAC
	thumb_func_end ov08_0221F284



	thumb_func_start ov08_0221F3D0
ov08_0221F3D0: ; 0x0221F3D0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	str r1, [sp, #0x10]
	ldr r1, [r5]
	ldr r0, _0221F494 ; =0x00002070
	ldr r1, [r1, #0xc]
	ldr r4, [r5, r0]
	mov r0, #6
	lsl r6, r2, #4
	bl String_New
	str r0, [sp, #0x14]
	ldr r0, _0221F498 ; =0x00001FA8
	mov r1, #0x2b
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	str r0, [sp, #0x18]
	mov r0, #0
	ldr r1, [sp, #0x18]
	add r2, r0, #0
	bl FontID_String_GetWidth
	add r7, r0, #0
	ldr r0, [sp, #0x18]
	bl String_Delete
	add r7, #0x28
	mov r0, #0x50
	sub r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	mov r0, #0x10
	lsl r2, r7, #0x10
	str r0, [sp, #4]
	add r0, r4, r6
	mov r1, #0
	lsr r2, r2, #0x10
	mov r3, #0x18
	bl FillWindowPixelRect
	ldr r0, _0221F498 ; =0x00001FA8
	mov r1, #0x2c
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	add r7, r0, #0
	str r1, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r2, [sp, #0x10]
	ldr r0, _0221F49C ; =0x00001FAC
	ldrb r2, [r2, #2]
	ldr r0, [r5, r0]
	mov r3, #2
	bl BufferIntegerAsString
	ldr r0, _0221F49C ; =0x00001FAC
	ldr r1, [sp, #0x14]
	ldr r0, [r5, r0]
	add r2, r7, #0
	bl StringExpandPlaceholders
	mov r0, #0
	ldr r1, [sp, #0x14]
	add r2, r0, #0
	bl FontID_String_GetWidth
	add r5, r0, #0
	mov r0, #0x18
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F4A0 ; =0x000F0E00
	mov r3, #0x50
	str r0, [sp, #8]
	mov r1, #0
	ldr r2, [sp, #0x14]
	add r0, r4, r6
	sub r3, r3, r5
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, #0
	bl String_Delete
	ldr r0, [sp, #0x14]
	bl String_Delete
	add r0, r4, r6
	bl ScheduleWindowCopyToVram
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_0221F494: .word 0x00002070
_0221F498: .word 0x00001FA8
_0221F49C: .word 0x00001FAC
_0221F4A0: .word 0x000F0E00
	thumb_func_end ov08_0221F3D0



	thumb_func_start ov08_0221F4A4
ov08_0221F4A4: ; 0x0221F4A4
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, [r5]
	mov r4, #0
	ldr r0, [r0]
	bl Party_GetCount
	cmp r0, #0
	ble _0221F52A
	add r0, r4, #0
	add r7, r5, #0
	str r0, [sp, #8]
	add r6, r5, #0
	add r7, #0x1b
_0221F4C2:
	ldr r0, _0221F54C ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #8]
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldrh r0, [r6, #8]
	cmp r0, #0
	beq _0221F512
	mov r0, #0x20
	str r0, [sp]
	mov r0, #7
	lsl r3, r4, #0x10
	str r0, [sp, #4]
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #4
	lsr r3, r3, #0x10
	bl ov08_0221DDCC
	ldrb r0, [r7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1f
	bne _0221F4FE
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl ov08_0221F550
_0221F4FE:
	ldr r0, [r6, #4]
	bl Pokemon_GetStatusIconId
	cmp r0, #7
	bne _0221F512
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl ov08_0221F5B0
_0221F512:
	ldr r0, [sp, #8]
	add r6, #0x50
	add r0, #0x10
	str r0, [sp, #8]
	ldr r0, [r5]
	add r7, #0x50
	ldr r0, [r0]
	add r4, r4, #1
	bl Party_GetCount
	cmp r4, r0
	blt _0221F4C2
_0221F52A:
	ldr r0, [r5]
	add r0, #0x35
	ldrb r0, [r0]
	cmp r0, #2
	bne _0221F540
	add r0, r5, #0
	mov r1, #7
	bl ov08_0221E340
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_0221F540:
	add r0, r5, #0
	mov r1, #6
	bl ov08_0221E340
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F54C: .word 0x00002070
	thumb_func_end ov08_0221F4A4



	thumb_func_start ov08_0221F550
ov08_0221F550: ; 0x0221F550
	push {r4, r5, r6, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0x18
	str r0, [sp]
	mov r0, #8
	add r6, r1, #0
	str r0, [sp, #4]
	ldr r0, _0221F5AC ; =0x00002070
	lsl r4, r6, #4
	ldr r0, [r5, r0]
	mov r1, #0
	add r0, r0, r4
	mov r2, #0x38
	mov r3, #0x20
	bl FillWindowPixelRect
	mov r2, #0x40
	str r2, [sp]
	mov r0, #8
	str r0, [sp, #4]
	ldr r0, _0221F5AC ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	mov r3, #0x18
	add r0, r0, r4
	bl FillWindowPixelRect
	mov r0, #0x20
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	add r2, r6, #0
	mov r3, #0x38
	bl ov08_0221DFC8
	mov r0, #0x18
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	add r2, r6, #0
	mov r3, #0x40
	bl ov08_0221E048
	add sp, #8
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0221F5AC: .word 0x00002070
	thumb_func_end ov08_0221F550



	thumb_func_start ov08_0221F5B0
ov08_0221F5B0: ; 0x0221F5B0
	push {r3, lr}
	mov r2, #0x50
	mul r2, r1
	add r2, r0, r2
	ldrb r2, [r2, #0x1b]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x1f
	bne _0221F5CC
	mov r2, #0x20
	str r2, [sp]
	add r2, r1, #0
	mov r3, #0
	bl ov08_0221DF80
_0221F5CC:
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov08_0221F5B0



	thumb_func_start ov08_0221F5D0
ov08_0221F5D0: ; 0x0221F5D0
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _0221F654 ; =0x00002070
	mov r1, #0
	ldr r0, [r4, r0]
	bl FillWindowPixelBuffer
	ldr r0, _0221F654 ; =0x00002070
	mov r1, #0
	ldr r0, [r4, r0]
	add r0, #0x10
	bl FillWindowPixelBuffer
	ldr r0, _0221F654 ; =0x00002070
	mov r1, #0
	ldr r0, [r4, r0]
	add r0, #0x20
	bl FillWindowPixelBuffer
	ldr r0, _0221F654 ; =0x00002070
	mov r1, #0
	ldr r0, [r4, r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r1, [r4]
	add r0, r4, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221F658
	add r0, r4, #0
	mov r1, #1
	mov r2, #0xf
	bl ov08_0221E3A4
	ldr r0, [r4]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	mul r0, r1
	add r0, r4, r0
	ldrb r0, [r0, #0x1b]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1f
	bne _0221F63E
	add r0, r4, #0
	mov r1, #2
	mov r2, #0x12
	bl ov08_0221E3A4
	add r0, r4, #0
	mov r1, #3
	mov r2, #0x13
	bl ov08_0221E3A4
	pop {r4, pc}
_0221F63E:
	ldr r0, _0221F654 ; =0x00002070
	ldr r0, [r4, r0]
	add r0, #0x20
	bl ScheduleWindowCopyToVram
	ldr r0, _0221F654 ; =0x00002070
	ldr r0, [r4, r0]
	add r0, #0x30
	bl ScheduleWindowCopyToVram
	pop {r4, pc}
	.balign 4, 0
_0221F654: .word 0x00002070
	thumb_func_end ov08_0221F5D0



	thumb_func_start ov08_0221F658
ov08_0221F658: ; 0x0221F658
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r4, r0, #0
	ldr r0, _0221F7A4 ; =0x00002070
	add r6, r1, #0
	ldr r0, [r4, r0]
	ldr r1, [r4]
	str r0, [sp, #0x18]
	mov r0, #0x50
	add r7, r6, #0
	add r5, r4, #4
	mul r7, r0
	add r0, r5, r7
	str r0, [sp, #0x14]
	ldr r1, [r1, #0xc]
	mov r0, #0xc
	bl String_New
	str r0, [sp, #0x10]
	ldr r0, _0221F7A8 ; =0x00001FA8
	ldr r1, _0221F7AC ; =ov08_02224FF4
	lsl r2, r6, #2
	ldr r0, [r4, r0]
	ldr r1, [r1, r2]
	bl NewString_ReadMsgData
	add r6, r0, #0
	ldr r0, [r5, r7]
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _0221F7B0 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r4, r0]
	bl BufferBoxMonNickname
	ldr r0, _0221F7B0 ; =0x00001FAC
	ldr r1, [sp, #0x10]
	ldr r0, [r4, r0]
	add r2, r6, #0
	bl StringExpandPlaceholders
	add r0, r6, #0
	bl String_Delete
	ldr r0, [sp, #0x14]
	mov r5, #0
	ldrb r0, [r0, #0x16]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1f
	bne _0221F6EC
	ldr r0, [sp, #0x14]
	ldrb r0, [r0, #0x17]
	lsl r1, r0, #0x18
	lsr r1, r1, #0x1f
	bne _0221F6EC
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1d
	bne _0221F6DC
	ldr r0, _0221F7A8 ; =0x00001FA8
	mov r1, #0x10
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	add r5, r0, #0
	b _0221F6EC
_0221F6DC:
	cmp r0, #1
	bne _0221F6EC
	ldr r0, _0221F7A8 ; =0x00001FA8
	mov r1, #0x11
	ldr r0, [r4, r0]
	bl NewString_ReadMsgData
	add r5, r0, #0
_0221F6EC:
	ldr r1, [sp, #0x10]
	mov r0, #4
	mov r2, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r5, #0
	bne _0221F704
	mov r7, #0
	add r4, r7, #0
	b _0221F714
_0221F704:
	mov r0, #0
	add r1, r5, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r4, #8
_0221F714:
	ldr r0, [sp, #0x18]
	bl GetWindowWidth
	lsl r0, r0, #3
	sub r0, r0, r6
	sub r0, r0, r7
	sub r1, r0, r4
	lsr r0, r1, #0x1f
	add r0, r1, r0
	lsl r0, r0, #0x17
	lsr r7, r0, #0x18
	mov r0, #7
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F7B4 ; =0x00070809
	ldr r2, [sp, #0x10]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x18]
	mov r1, #4
	add r3, r7, #0
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [sp, #0x10]
	bl String_Delete
	cmp r5, #0
	beq _0221F79A
	ldr r0, [sp, #0x14]
	ldrb r0, [r0, #0x17]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1d
	bne _0221F778
	mov r0, #8
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F7B8 ; =0x000A0B00
	add r3, r7, r6
	str r0, [sp, #8]
	mov r1, #0
	ldr r0, [sp, #0x18]
	add r2, r5, #0
	add r3, r4, r3
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	b _0221F794
_0221F778:
	mov r0, #8
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221F7BC ; =0x000C0D00
	add r3, r7, r6
	str r0, [sp, #8]
	mov r1, #0
	ldr r0, [sp, #0x18]
	add r2, r5, #0
	add r3, r4, r3
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
_0221F794:
	add r0, r5, #0
	bl String_Delete
_0221F79A:
	ldr r0, [sp, #0x18]
	bl ScheduleWindowCopyToVram
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221F7A4: .word 0x00002070
_0221F7A8: .word 0x00001FA8
_0221F7AC: .word ov08_02224FF4
_0221F7B0: .word 0x00001FAC
_0221F7B4: .word 0x00070809
_0221F7B8: .word 0x000A0B00
_0221F7BC: .word 0x000C0D00
	thumb_func_end ov08_0221F658



	thumb_func_start ov08_0221F7C0
ov08_0221F7C0: ; 0x0221F7C0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	ldr r0, _0221F8F0 ; =0x00002075
	mov r1, #6
	ldrb r2, [r5, r0]
	sub r0, r0, #5
	mul r1, r2
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	ldr r1, [r5, r0]
	lsl r0, r6, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #1
	lsl r0, r0, #4
	str r0, [sp, #0x18]
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x18]
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #2
	lsl r0, r0, #4
	str r0, [sp, #0x14]
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x14]
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #3
	lsl r0, r0, #4
	str r0, [sp, #0x10]
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x10]
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #4
	lsl r0, r0, #4
	str r0, [sp, #0xc]
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0xc]
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221F8F4 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x50
	bl FillWindowPixelBuffer
	mov r2, #0
	str r2, [sp]
	str r2, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r1, r6, #0
	bl ov08_0221DDCC
	add r0, r5, #0
	str r0, [sp, #0x1c]
	add r0, #0x34
	mov r4, #0
	add r7, r6, #1
	str r0, [sp, #0x1c]
_0221F85A:
	ldr r0, [r5]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #0x1c]
	lsl r1, r4, #3
	add r0, r0, r2
	str r1, [sp, #0x20]
	ldrh r1, [r0, r1]
	str r0, [sp, #0x24]
	cmp r1, #0
	beq _0221F89C
	mov r0, #4
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	ldr r0, _0221F8F8 ; =0x00070809
	ldr r3, _0221F8FC ; =ov08_02224FE0
	lsl r6, r4, #2
	str r0, [sp, #8]
	ldr r3, [r3, r6]
	add r0, r5, #0
	add r2, r7, r4
	bl ov08_0221E244
	ldr r2, [sp, #0x20]
	ldr r1, [sp, #0x24]
	add r0, r5, #0
	add r1, r1, r2
	add r2, r7, r4
	bl ov08_0221F284
_0221F89C:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #4
	blo _0221F85A
	add r0, r5, #0
	mov r1, #5
	mov r2, #0x12
	bl ov08_0221E3A4
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x18]
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x14]
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0x10]
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r0, _0221F8F4 ; =0x00002070
	ldr r1, [r5, r0]
	ldr r0, [sp, #0xc]
	add r0, r1, r0
	bl ScheduleWindowCopyToVram
	ldr r1, _0221F8F0 ; =0x00002075
	mov r0, #1
	ldrb r2, [r5, r1]
	eor r0, r2
	strb r0, [r5, r1]
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221F8F0: .word 0x00002075
_0221F8F4: .word 0x00002070
_0221F8F8: .word 0x00070809
_0221F8FC: .word ov08_02224FE0
	thumb_func_end ov08_0221F7C0



	thumb_func_start ov08_0221F900
ov08_0221F900: ; 0x0221F900
	push {r3, r4, r5, lr}
	sub sp, #8
	add r5, r0, #0
	ldr r0, _0221FB10 ; =0x00002075
	mov r1, #0x16
	ldrb r2, [r5, r0]
	sub r0, r0, #5
	add r4, r2, #0
	mul r4, r1
	ldr r1, [r5, r0]
	mov r0, #0x13
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #5
	lsl r0, r0, #6
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xe0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xf0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #1
	lsl r0, r0, #8
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #0x11
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #0x12
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xd0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	mov r0, #0x15
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	lsl r0, r4, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #0
	add r0, #0xa
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #0
	add r0, #0xb
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #0
	add r0, #0xc
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #5
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #6
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #7
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #0
	add r0, #8
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #0
	add r0, #9
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #4
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #1
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #2
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FB14 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #3
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r2, #0
	str r2, [sp]
	str r2, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r1, r4, #0
	bl ov08_0221DDCC
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221EAD4
	mov r3, #0
	str r3, [sp]
	ldr r2, [r5]
	add r1, r4, #0
	ldrb r2, [r2, #0x11]
	add r0, r5, #0
	add r1, #0xa
	bl ov08_0221E048
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E408
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E5DC
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E6D8
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E7D4
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E8D4
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221E9D4
	ldr r2, [r5]
	add r0, r5, #0
	ldrb r2, [r2, #0x11]
	add r1, r4, #1
	bl ov08_0221E120
	ldr r2, [r5]
	add r0, r5, #0
	ldrb r2, [r2, #0x11]
	add r1, r4, #3
	bl ov08_0221E1A8
	ldr r1, [r5]
	add r0, r5, #0
	ldrb r1, [r1, #0x11]
	bl ov08_0221EC6C
	add r0, r5, #0
	mov r1, #0x15
	mov r2, #0x13
	bl ov08_0221E3A4
	ldr r1, _0221FB10 ; =0x00002075
	mov r0, #1
	ldrb r2, [r5, r1]
	eor r0, r2
	strb r0, [r5, r1]
	add sp, #8
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221FB10: .word 0x00002075
_0221FB14: .word 0x00002070
	thumb_func_end ov08_0221F900



	thumb_func_start ov08_0221FB18
ov08_0221FB18: ; 0x0221FB18
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	ldr r0, _0221FC6C ; =0x00002075
	mov r1, #0xb
	ldrb r2, [r5, r0]
	sub r0, r0, #5
	ldr r0, [r5, r0]
	add r4, r2, #0
	mul r4, r1
	add r0, #0x60
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x70
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x80
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x90
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xa0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #1
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	lsl r0, r4, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #2
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #3
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #5
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _0221FC70 ; =0x00002070
	ldr r1, [r5, r0]
	add r0, r4, #4
	lsl r0, r0, #4
	add r0, r1, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, [r5]
	add r2, r5, #0
	ldrb r3, [r0, #0x11]
	add r0, #0x34
	ldrb r0, [r0]
	mov r1, #0x50
	add r2, #0x34
	mul r1, r3
	add r1, r2, r1
	lsl r0, r0, #3
	add r7, r1, r0
	mov r2, #0
	str r2, [sp]
	add r0, r5, #0
	mov r1, #6
	str r2, [sp, #4]
	bl ov08_0221DDCC
	mov r2, #0
	add r0, r5, #0
	mov r1, #7
	add r3, r2, #0
	bl ov08_0221E2E8
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0221FC74 ; =0x000F0E00
	add r2, r4, #0
	str r0, [sp, #8]
	ldr r3, [r5]
	ldrh r1, [r7]
	add r3, #0x34
	ldrb r3, [r3]
	add r0, r5, #0
	lsl r6, r3, #2
	ldr r3, _0221FC78 ; =ov08_02224FE0
	ldr r3, [r3, r6]
	bl ov08_0221E244
	add r0, r5, #0
	mov r1, #8
	bl ov08_0221ECE8
	ldrb r2, [r7, #6]
	add r0, r5, #0
	add r1, r4, #2
	bl ov08_0221ED30
	add r0, r5, #0
	mov r1, #9
	bl ov08_0221EE18
	ldrb r2, [r7, #7]
	add r0, r5, #0
	add r1, r4, #3
	bl ov08_0221EE60
	ldrh r2, [r7]
	add r0, r5, #0
	add r1, r4, #4
	bl ov08_0221EF48
	add r0, r5, #0
	mov r1, #0xa
	bl ov08_0221EFA8
	ldrb r2, [r7, #5]
	add r0, r5, #0
	add r1, r4, #5
	bl ov08_0221F00C
	ldrb r2, [r7, #2]
	ldrb r3, [r7, #3]
	add r0, r5, #0
	add r1, r4, #1
	bl ov08_0221F07C
	ldr r1, _0221FC6C ; =0x00002075
	mov r0, #1
	ldrb r2, [r5, r1]
	eor r0, r2
	strb r0, [r5, r1]
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221FC6C: .word 0x00002075
_0221FC70: .word 0x00002070
_0221FC74: .word 0x000F0E00
_0221FC78: .word ov08_02224FE0
	thumb_func_end ov08_0221FB18



	thumb_func_start ov08_0221FC7C
ov08_0221FC7C: ; 0x0221FC7C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	bl FillWindowPixelBuffer
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x10
	bl FillWindowPixelBuffer
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x20
	bl FillWindowPixelBuffer
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x40
	bl FillWindowPixelBuffer
	ldr r0, _0221FD98 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x50
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r2, r1, #0
	bl ov08_0221DDCC
	add r0, r5, #0
	mov r6, #0
	str r0, [sp, #0x10]
	add r0, #0x34
	ldr r7, _0221FD9C ; =ov08_02224FE0
	add r4, r6, #0
	str r0, [sp, #0x10]
_0221FCE8:
	ldr r0, [r5]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #0x10]
	add r0, r0, r2
	ldrh r1, [r0, r4]
	str r0, [sp, #0xc]
	cmp r1, #0
	beq _0221FD20
	mov r0, #4
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	ldr r0, _0221FDA0 ; =0x00070809
	add r2, r6, #1
	str r0, [sp, #8]
	ldr r3, [r7]
	add r0, r5, #0
	bl ov08_0221E244
	ldr r1, [sp, #0xc]
	add r0, r5, #0
	add r1, r1, r4
	add r2, r6, #1
	bl ov08_0221F284
_0221FD20:
	add r6, r6, #1
	add r4, #8
	add r7, r7, #4
	cmp r6, #4
	blo _0221FCE8
	mov r0, #4
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	ldr r0, _0221FDA0 ; =0x00070809
	mov r2, #5
	str r0, [sp, #8]
	ldr r1, [r5]
	add r0, r5, #0
	ldrh r1, [r1, #0x24]
	mov r3, #0x49
	bl ov08_0221E244
	ldr r0, [r5]
	mov r1, #5
	ldrh r0, [r0, #0x24]
	bl GetMoveAttr
	add r1, sp, #0x14
	strb r0, [r1, #2]
	ldrb r0, [r1, #2]
	mov r2, #5
	strb r0, [r1, #3]
	add r0, r5, #0
	add r1, sp, #0x14
	bl ov08_0221F284
	ldr r0, _0221FD98 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x10
	bl ScheduleWindowCopyToVram
	ldr r0, _0221FD98 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x20
	bl ScheduleWindowCopyToVram
	ldr r0, _0221FD98 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x30
	bl ScheduleWindowCopyToVram
	ldr r0, _0221FD98 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x40
	bl ScheduleWindowCopyToVram
	ldr r0, _0221FD98 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x50
	bl ScheduleWindowCopyToVram
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_0221FD98: .word 0x00002070
_0221FD9C: .word ov08_02224FE0
_0221FDA0: .word 0x00070809
	thumb_func_end ov08_0221FC7C



	thumb_func_start ov08_0221FDA4
ov08_0221FDA4: ; 0x0221FDA4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x20
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x10
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x40
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x60
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x50
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x70
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x90
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xa0
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x80
	bl FillWindowPixelBuffer
	ldr r0, _0221FF64 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0xb0
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r2, r1, #0
	bl ov08_0221DDCC
	mov r2, #0
	add r0, r5, #0
	mov r1, #2
	add r3, r2, #0
	bl ov08_0221E2E8
	add r0, r5, #0
	mov r1, #4
	bl ov08_0221ECE8
	add r0, r5, #0
	mov r1, #5
	bl ov08_0221EE18
	add r0, r5, #0
	mov r1, #9
	bl ov08_0221EFA8
	ldr r1, [r5]
	add r0, r1, #0
	add r0, #0x34
	ldrb r3, [r0]
	cmp r3, #4
	bhs _0221FEE0
	ldrb r1, [r1, #0x11]
	add r2, r5, #0
	mov r0, #0x50
	add r2, #0x34
	mul r0, r1
	add r7, r2, r0
	lsl r0, r3, #3
	str r0, [sp, #0xc]
	add r4, r7, r0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0221FF68 ; =0x000F0E00
	lsl r6, r3, #2
	str r0, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r3, _0221FF6C ; =ov08_02224FE0
	ldrh r1, [r7, r1]
	ldr r3, [r3, r6]
	add r0, r5, #0
	mov r2, #1
	bl ov08_0221E244
	ldrb r2, [r4, #6]
	add r0, r5, #0
	mov r1, #6
	bl ov08_0221ED30
	ldrb r2, [r4, #7]
	add r0, r5, #0
	mov r1, #7
	bl ov08_0221EE60
	ldr r2, [sp, #0xc]
	add r0, r5, #0
	ldrh r2, [r7, r2]
	mov r1, #8
	bl ov08_0221EF48
	ldrb r2, [r4, #5]
	add r0, r5, #0
	mov r1, #0xa
	bl ov08_0221F00C
	ldrb r2, [r4, #2]
	ldrb r3, [r4, #3]
	add r0, r5, #0
	mov r1, #3
	bl ov08_0221F07C
	b _0221FF56
_0221FEE0:
	ldrh r0, [r1, #0x24]
	mov r1, #5
	bl GetMoveAttr
	add r4, r0, #0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _0221FF68 ; =0x000F0E00
	mov r2, #1
	str r0, [sp, #8]
	ldr r1, [r5]
	add r0, r5, #0
	ldrh r1, [r1, #0x24]
	mov r3, #0x49
	bl ov08_0221E244
	ldr r2, [r5]
	add r0, r5, #0
	ldrh r2, [r2, #0x24]
	mov r1, #8
	bl ov08_0221EF48
	ldr r0, [r5]
	mov r1, #4
	ldrh r0, [r0, #0x24]
	bl GetMoveAttr
	add r2, r0, #0
	add r0, r5, #0
	mov r1, #6
	bl ov08_0221ED30
	ldr r0, [r5]
	mov r1, #2
	ldrh r0, [r0, #0x24]
	bl GetMoveAttr
	add r2, r0, #0
	add r0, r5, #0
	mov r1, #7
	bl ov08_0221EE60
	ldr r0, [r5]
	mov r1, #1
	ldrh r0, [r0, #0x24]
	bl GetMoveAttr
	add r2, r0, #0
	add r0, r5, #0
	mov r1, #0xa
	bl ov08_0221F00C
	add r0, r5, #0
	mov r1, #3
	add r2, r4, #0
	add r3, r4, #0
	bl ov08_0221F07C
_0221FF56:
	add r0, r5, #0
	mov r1, #0xb
	bl ov08_0221F1B0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221FF64: .word 0x00002070
_0221FF68: .word 0x000F0E00
_0221FF6C: .word ov08_02224FE0
	thumb_func_end ov08_0221FDA4



	thumb_func_start ov08_0221FF70
ov08_0221FF70: ; 0x0221FF70
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, _02220058 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	bl FillWindowPixelBuffer
	ldr r0, _02220058 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x10
	bl FillWindowPixelBuffer
	ldr r0, _02220058 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x20
	bl FillWindowPixelBuffer
	ldr r0, _02220058 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r0, _02220058 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x40
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r2, r1, #0
	bl ov08_0221DDCC
	add r0, r5, #0
	mov r6, #0
	str r0, [sp, #0x10]
	add r0, #0x34
	ldr r7, _0222005C ; =ov08_02224FE0
	add r4, r6, #0
	str r0, [sp, #0x10]
_0221FFD0:
	ldr r0, [r5]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #0x10]
	add r0, r0, r2
	ldrh r1, [r0, r4]
	str r0, [sp, #0xc]
	cmp r1, #0
	beq _02220008
	mov r0, #4
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	ldr r0, _02220060 ; =0x00070809
	add r2, r6, #1
	str r0, [sp, #8]
	ldr r3, [r7]
	add r0, r5, #0
	bl ov08_0221E244
	ldr r1, [sp, #0xc]
	add r0, r5, #0
	add r1, r1, r4
	add r2, r6, #1
	bl ov08_0221F284
_02220008:
	add r6, r6, #1
	add r4, #8
	add r7, r7, #4
	cmp r6, #4
	blo _0221FFD0
	ldr r2, [r5]
	mov r1, #0x25
	ldrh r0, [r2, #0x22]
	ldr r2, [r2, #0xc]
	bl GetItemAttr
	cmp r0, #0
	bne _0222002A
	add r0, r5, #0
	mov r1, #0x5e
	bl ov08_0221E340
_0222002A:
	ldr r0, _02220058 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x10
	bl ScheduleWindowCopyToVram
	ldr r0, _02220058 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x20
	bl ScheduleWindowCopyToVram
	ldr r0, _02220058 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x30
	bl ScheduleWindowCopyToVram
	ldr r0, _02220058 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x40
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02220058: .word 0x00002070
_0222005C: .word ov08_02224FE0
_02220060: .word 0x00070809
	thumb_func_end ov08_0221FF70



	thumb_func_start ov08_02220064
ov08_02220064: ; 0x02220064
	push {r3, r4, r5, lr}
	ldr r4, [r0]
	add r3, r1, #0
	ldrb r5, [r4, #0x11]
	add r1, r0, #0
	mov r4, #0x50
	add r1, #0x34
	mul r4, r5
	add r4, r1, r4
	lsl r1, r2, #3
	add r1, r4, r1
	add r2, r3, #0
	bl ov08_0221F3D0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov08_02220064



	thumb_func_start ov08_02220084
ov08_02220084: ; 0x02220084
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x20
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x30
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x10
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x40
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x50
	bl FillWindowPixelBuffer
	ldr r0, _022201B0 ; =0x00002070
	mov r1, #0
	ldr r0, [r5, r0]
	add r0, #0x60
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r3, [r5]
	add r0, r5, #0
	ldrb r3, [r3, #0x11]
	add r2, r1, #0
	bl ov08_0221DDCC
	mov r2, #0
	add r0, r5, #0
	mov r1, #2
	add r3, r2, #0
	bl ov08_0221E2E8
	ldr r0, _022201B4 ; =0x00001FA8
	mov r1, #0x39
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	mov r1, #0
	add r4, r0, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _022201B8 ; =0x000F0E00
	add r2, r4, #0
	str r0, [sp, #8]
	ldr r0, _022201B0 ; =0x00002070
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	add r3, r1, #0
	add r0, #0x40
	bl AddTextPrinterParameterizedWithColor
	add r0, r4, #0
	bl String_Delete
	ldr r0, _022201B0 ; =0x00002070
	ldr r0, [r5, r0]
	add r0, #0x40
	bl ScheduleWindowCopyToVram
	ldr r1, [r5]
	add r0, r1, #0
	add r0, #0x34
	ldrb r3, [r0]
	cmp r3, #4
	bhs _02220176
	ldrb r1, [r1, #0x11]
	add r2, r5, #0
	mov r0, #0x50
	mul r0, r1
	add r2, #0x34
	add r2, r2, r0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _022201B8 ; =0x000F0E00
	lsl r1, r3, #3
	str r0, [sp, #8]
	lsl r6, r3, #2
	ldr r3, _022201BC ; =ov08_02224FE0
	add r4, r2, r1
	ldrh r1, [r2, r1]
	ldr r3, [r3, r6]
	add r0, r5, #0
	mov r2, #1
	bl ov08_0221E244
	ldrb r2, [r4, #2]
	ldrb r3, [r4, #3]
	add r0, r5, #0
	mov r1, #3
	bl ov08_0221F07C
	b _022201A4
_02220176:
	ldrh r0, [r1, #0x24]
	mov r1, #5
	bl GetMoveAttr
	add r4, r0, #0
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _022201B8 ; =0x000F0E00
	mov r2, #1
	str r0, [sp, #8]
	ldr r1, [r5]
	add r0, r5, #0
	ldrh r1, [r1, #0x24]
	mov r3, #0x49
	bl ov08_0221E244
	add r0, r5, #0
	mov r1, #3
	add r2, r4, #0
	add r3, r4, #0
	bl ov08_0221F07C
_022201A4:
	add r0, r5, #0
	mov r1, #6
	bl ov08_0221F1B0
	add sp, #0x10
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022201B0: .word 0x00002070
_022201B4: .word 0x00001FA8
_022201B8: .word 0x000F0E00
_022201BC: .word ov08_02224FE0
	thumb_func_end ov08_02220084



	thumb_func_start ov08_022201C0
ov08_022201C0: ; 0x022201C0
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _022201E4 ; =0x00002060
	mov r1, #1
	add r0, r4, r0
	add r2, r1, #0
	mov r3, #0xe
	bl DrawFrameAndWindow2
	ldr r0, _022201E4 ; =0x00002060
	mov r1, #0xf
	add r0, r4, r0
	bl FillWindowPixelBuffer
	add r0, r4, #0
	bl ov08_022201E8
	pop {r4, pc}
	.balign 4, 0
_022201E4: .word 0x00002060
	thumb_func_end ov08_022201C0



	thumb_func_start ov08_022201E8
ov08_022201E8: ; 0x022201E8
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	mov r0, #1
	bl TextFlags_SetCanABSpeedUpPrint
	ldr r0, [r4]
	ldr r0, [r0, #8]
	bl BattleSystem_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	str r0, [sp, #4]
	ldr r2, _0222021C ; =0x00002060
	str r3, [sp, #8]
	add r0, r4, r2
	sub r2, #0xb0
	ldr r2, [r4, r2]
	mov r1, #1
	bl AddTextPrinterParameterized
	ldr r1, _02220220 ; =0x0000207B
	strb r0, [r4, r1]
	add sp, #0xc
	pop {r3, r4, pc}
	nop
_0222021C: .word 0x00002060
_02220220: .word 0x0000207B
	thumb_func_end ov08_022201E8



	thumb_func_start ov08_02220224
ov08_02220224: ; 0x02220224
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, [r5]
	mov r1, #0
	str r0, [sp, #0xc]
	ldr r2, [sp, #0xc]
	ldrh r0, [r0, #0x22]
	ldr r2, [r2, #0xc]
	bl LoadItemDataOrGfx
	ldr r2, [sp, #0xc]
	add r6, r0, #0
	ldrb r3, [r2, #0x11]
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0xc]
	add r2, r2, r3
	add r2, #0x2c
	ldrb r2, [r2]
	ldr r0, [r0, #8]
	ldr r1, [r1, #0x28]
	bl BattleSystem_GetPartyMon
	mov r1, #0xa3
	mov r2, #0
	add r7, r0, #0
	bl GetMonData
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	add r0, r6, #0
	mov r1, #0xf
	mov r4, #0
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _02220278
	mov r0, #1
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_02220278:
	add r0, r6, #0
	mov r1, #0x10
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _0222028C
	mov r0, #2
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_0222028C:
	add r0, r6, #0
	mov r1, #0x11
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022202A0
	mov r0, #4
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_022202A0:
	add r0, r6, #0
	mov r1, #0x12
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022202B4
	mov r0, #8
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_022202B4:
	add r0, r6, #0
	mov r1, #0x13
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022202C8
	mov r0, #0x10
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_022202C8:
	add r0, r6, #0
	mov r1, #0x14
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022202DC
	mov r0, #0x20
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_022202DC:
	add r0, r6, #0
	mov r1, #0x15
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022202F0
	mov r0, #0x40
	orr r0, r4
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_022202F0:
	ldr r0, [sp, #0xc]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	mul r0, r1
	add r0, r5, r0
	ldrh r1, [r0, #0x14]
	cmp r1, #0
	bne _0222033A
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _0222033A
	ldr r0, _02220574 ; =0x00001FA8
	mov r1, #0x58
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_0222033A:
	ldr r0, [sp, #8]
	cmp r0, r1
	beq _02220396
	ldr r0, _02220574 ; =0x00001FA8
	mov r1, #0x52
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	mov r0, #0
	str r0, [sp]
	mov r1, #1
	ldr r2, [sp, #0xc]
	str r1, [sp, #4]
	ldrb r3, [r2, #0x11]
	mov r2, #0x50
	ldr r0, _02220578 ; =0x00001FAC
	mul r2, r3
	add r2, r5, r2
	ldrh r3, [r2, #0x14]
	ldr r2, [sp, #8]
	ldr r0, [r5, r0]
	sub r2, r2, r3
	mov r3, #3
	bl BufferIntegerAsString
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_02220396:
	add r0, r6, #0
	mov r1, #0x24
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	bne _022203AE
	add r0, r6, #0
	mov r1, #0x25
	bl GetItemAttr_PreloadedItemData
	cmp r0, #0
	beq _022203BE
_022203AE:
	ldr r2, _02220574 ; =0x00001FA8
	mov r1, #0x57
	ldr r0, [r5, r2]
	add r2, #8
	ldr r2, [r5, r2]
	bl ReadMsgDataIntoString
	b _0222056A
_022203BE:
	cmp r4, #1
	ldr r0, _02220574 ; =0x00001FA8
	bne _022203F6
	ldr r0, [r5, r0]
	mov r1, #0x5c
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_022203F6:
	cmp r4, #2
	bne _0222042C
	ldr r0, [r5, r0]
	mov r1, #0x53
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_0222042C:
	cmp r4, #4
	bne _02220462
	ldr r0, [r5, r0]
	mov r1, #0x55
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_02220462:
	cmp r4, #8
	bne _02220498
	ldr r0, [r5, r0]
	mov r1, #0x56
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_02220498:
	cmp r4, #0x10
	bne _022204CE
	ldr r0, [r5, r0]
	mov r1, #0x54
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_022204CE:
	cmp r4, #0x20
	bne _02220504
	ldr r0, [r5, r0]
	mov r1, #0x5a
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_02220504:
	cmp r4, #0x40
	bne _0222053A
	ldr r0, [r5, r0]
	mov r1, #0x5b
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	b _0222056A
_0222053A:
	ldr r0, [r5, r0]
	mov r1, #0x59
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r7, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _02220578 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r1, _02220578 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
_0222056A:
	add r0, r6, #0
	bl Heap_Free
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02220574: .word 0x00001FA8
_02220578: .word 0x00001FAC
	thumb_func_end ov08_02220224



	thumb_func_start ov08_0222057C
ov08_0222057C: ; 0x0222057C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r3, [r5]
	ldrb r2, [r3, #0x11]
	ldr r0, [r3, #8]
	ldr r1, [r3, #0x28]
	add r2, r3, r2
	add r2, #0x2c
	ldrb r2, [r2]
	bl BattleSystem_GetPartyMon
	add r6, r0, #0
	ldr r0, _022205D4 ; =0x00001FA8
	mov r1, #0x5f
	ldr r0, [r5, r0]
	bl NewString_ReadMsgData
	add r4, r0, #0
	add r0, r6, #0
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, _022205D8 ; =0x00001FAC
	mov r1, #0
	ldr r0, [r5, r0]
	bl BufferBoxMonNickname
	ldr r0, _022205D8 ; =0x00001FAC
	ldr r2, _022205DC ; =MOVE_EMBARGO
	ldr r0, [r5, r0]
	mov r1, #1
	bl BufferMoveName
	ldr r1, _022205D8 ; =0x00001FAC
	add r2, r4, #0
	ldr r0, [r5, r1]
	add r1, r1, #4
	ldr r1, [r5, r1]
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022205D4: .word 0x00001FA8
_022205D8: .word 0x00001FAC
_022205DC: .word MOVE_EMBARGO
	thumb_func_end ov08_0222057C



	thumb_func_start ov08_022205E0
ov08_022205E0: ; 0x022205E0
	push {r4, lr}
	add r4, r0, #0
	bl ov08_0222061C
	add r0, r4, #0
	bl ov08_02220668
	add r0, r4, #0
	bl ov08_02220750
	add r0, r4, #0
	bl ov08_02220800
	add r0, r4, #0
	bl ov08_02220878
	add r0, r4, #0
	bl ov08_02220928
	add r0, r4, #0
	bl ov08_02220A28
	add r0, r4, #0
	bl ov08_0222162C
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	pop {r4, pc}
	thumb_func_end ov08_022205E0



	thumb_func_start ov08_0222061C
ov08_0222061C: ; 0x0222061C
	push {r3, r4, r5, lr}
	sub sp, #0x18
	ldr r3, _02220660 ; =ov08_0222541C
	add r2, sp, #0
	add r5, r0, #0
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	add r4, r0, #0
	bl SpriteManager_New
	ldr r1, _02220664 ; =0x00001FB4
	mov r2, #0x2b
	str r0, [r5, r1]
	ldr r1, [r5, r1]
	add r0, r4, #0
	bl SpriteSystem_InitSprites
	ldr r1, _02220664 ; =0x00001FB4
	add r0, r4, #0
	ldr r1, [r5, r1]
	add r2, sp, #0
	bl SpriteSystem_InitManagerWithCapacities
	add sp, #0x18
	pop {r3, r4, r5, pc}
	nop
_02220660: .word ov08_0222541C
_02220664: .word 0x00001FB4
	thumb_func_end ov08_0222061C



	thumb_func_start ov08_02220668
ov08_02220668: ; 0x02220668
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r6, r0, #0
	ldr r1, [r6]
	mov r0, #0x14
	ldr r1, [r1, #0xc]
	bl NARC_New
	add r7, r0, #0
	ldr r0, [r6]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	str r0, [sp, #0x18]
	bl sub_02074490
	str r7, [sp]
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r1, #3
	str r1, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	ldr r0, _02220748 ; =0x0000B007
	ldr r3, _0222074C ; =0x00001FB4
	str r0, [sp, #0x14]
	mov r0, #0x7a
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	ldr r2, [sp, #0x18]
	ldr r3, [r6, r3]
	bl SpriteSystem_LoadPaletteBufferFromOpenNarc
	bl sub_02074498
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _02220748 ; =0x0000B007
	ldr r1, _0222074C ; =0x00001FB4
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	bl sub_020744A4
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _02220748 ; =0x0000B007
	ldr r1, _0222074C ; =0x00001FB4
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	mov r4, #0
	add r5, r6, #0
_022206E4:
	ldrh r0, [r5, #8]
	cmp r0, #0
	beq _0222070E
	ldr r0, [r5, #4]
	bl Pokemon_GetIconNaix
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _02220748 ; =0x0000B007
	ldr r1, _0222074C ; =0x00001FB4
	add r0, r4, r0
	str r0, [sp, #8]
	ldr r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	b _02220734
_0222070E:
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	bl GetMonIconNaixEx
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _02220748 ; =0x0000B007
	ldr r1, _0222074C ; =0x00001FB4
	add r0, r4, r0
	str r0, [sp, #8]
	ldr r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl SpriteSystem_LoadCharResObjFromOpenNarc
_02220734:
	add r4, r4, #1
	add r5, #0x50
	cmp r4, #6
	blo _022206E4
	add r0, r7, #0
	bl NARC_Delete
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_02220748: .word 0x0000B007
_0222074C: .word 0x00001FB4
	thumb_func_end ov08_02220668



	thumb_func_start ov08_02220750
ov08_02220750: ; 0x02220750
	push {r4, r5, r6, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldr r0, [r5]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	ldr r1, [r5]
	add r4, r0, #0
	ldr r1, [r1, #0xc]
	mov r0, #0x27
	bl NARC_New
	add r6, r0, #0
	bl sub_0208AD58
	str r6, [sp]
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	ldr r0, _022207F4 ; =0x0000B008
	ldr r3, _022207F8 ; =0x00001FB4
	str r0, [sp, #0x14]
	mov r0, #0x7a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r3, [r5, r3]
	mov r1, #3
	add r2, r4, #0
	bl SpriteSystem_LoadPaletteBufferFromOpenNarc
	bl sub_0208AD5C
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _022207F4 ; =0x0000B008
	ldr r1, _022207F8 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r4, #0
	add r2, r6, #0
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	bl sub_0208AD60
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _022207F4 ; =0x0000B008
	ldr r1, _022207F8 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r4, #0
	add r2, r6, #0
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	bl sub_0208AD54
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _022207FC ; =0x0000B00D
	ldr r1, _022207F8 ; =0x00001FB4
	str r0, [sp, #8]
	ldr r1, [r5, r1]
	add r0, r4, #0
	add r2, r6, #0
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	add r0, r6, #0
	bl NARC_Delete
	add sp, #0x18
	pop {r4, r5, r6, pc}
	nop
_022207F4: .word 0x0000B008
_022207F8: .word 0x00001FB4
_022207FC: .word 0x0000B00D
	thumb_func_end ov08_02220750



	thumb_func_start ov08_02220800
ov08_02220800: ; 0x02220800
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	ldr r0, [r5]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	add r6, r0, #0
	mov r0, #2
	str r0, [sp]
	ldr r0, _02220868 ; =0x0000B009
	ldr r3, _0222086C ; =0x00001FB4
	str r0, [sp, #4]
	mov r0, #0x7a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r3, [r5, r3]
	mov r1, #3
	add r2, r6, #0
	bl sub_02077720
	ldr r1, _0222086C ; =0x00001FB4
	ldr r2, _02220868 ; =0x0000B009
	ldr r1, [r5, r1]
	add r0, r6, #0
	add r3, r2, #0
	bl sub_0207775C
	ldr r4, _02220870 ; =0x0000B00E
	add r7, r4, #6
_0222083C:
	ldr r1, _0222086C ; =0x00001FB4
	str r4, [sp]
	ldr r1, [r5, r1]
	add r0, r6, #0
	mov r2, #2
	mov r3, #0
	bl sub_020776B8
	add r4, r4, #1
	cmp r4, r7
	bls _0222083C
	ldr r0, _02220874 ; =0x0000B015
	ldr r1, _0222086C ; =0x00001FB4
	str r0, [sp]
	ldr r1, [r5, r1]
	add r0, r6, #0
	mov r2, #2
	mov r3, #0
	bl sub_02077834
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02220868: .word 0x0000B009
_0222086C: .word 0x00001FB4
_02220870: .word 0x0000B00E
_02220874: .word 0x0000B015
	thumb_func_end ov08_02220800



	thumb_func_start ov08_02220878
ov08_02220878: ; 0x02220878
	push {r4, r5, r6, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldr r1, [r5]
	mov r0, #0x15
	ldr r1, [r1, #0xc]
	bl NARC_New
	add r4, r0, #0
	ldr r0, [r5]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	add r6, r0, #0
	bl sub_0207CAA0
	str r4, [sp]
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	ldr r0, _0222091C ; =0x0000B00A
	ldr r3, _02220920 ; =0x00001FB4
	str r0, [sp, #0x14]
	mov r0, #0x7a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r3, [r5, r3]
	mov r1, #3
	add r2, r6, #0
	bl SpriteSystem_LoadPaletteBufferFromOpenNarc
	bl sub_0207CAA4
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _0222091C ; =0x0000B00A
	ldr r1, _02220920 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r6, #0
	add r2, r4, #0
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	bl sub_0207CAA8
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	ldr r0, _0222091C ; =0x0000B00A
	ldr r1, _02220920 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r6, #0
	add r2, r4, #0
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	bl sub_0207CA9C
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _02220924 ; =0x0000B016
	ldr r1, _02220920 ; =0x00001FB4
	str r0, [sp, #8]
	ldr r1, [r5, r1]
	add r0, r6, #0
	add r2, r4, #0
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	add r0, r4, #0
	bl NARC_Delete
	add sp, #0x18
	pop {r4, r5, r6, pc}
	nop
_0222091C: .word 0x0000B00A
_02220920: .word 0x00001FB4
_02220924: .word 0x0000B016
	thumb_func_end ov08_02220878



	thumb_func_start ov08_02220928
ov08_02220928: ; 0x02220928
	push {r3, r4, r5, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldr r0, [r5]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	add r4, r0, #0
	mov r0, #0x47
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	ldr r0, _022209AC ; =0x0000B00B
	ldr r3, _022209B0 ; =0x00001FB4
	str r0, [sp, #0x14]
	mov r0, #0x7a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r3, [r5, r3]
	mov r1, #3
	add r2, r4, #0
	bl SpriteSystem_LoadPaletteBuffer
	mov r0, #0
	str r0, [sp]
	ldr r0, _022209AC ; =0x0000B00B
	ldr r1, _022209B0 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r4, #0
	mov r2, #0x47
	mov r3, #0x19
	bl SpriteSystem_LoadCellResObj
	mov r0, #0
	str r0, [sp]
	ldr r0, _022209AC ; =0x0000B00B
	ldr r1, _022209B0 ; =0x00001FB4
	str r0, [sp, #4]
	ldr r1, [r5, r1]
	add r0, r4, #0
	mov r2, #0x47
	mov r3, #0x18
	bl SpriteSystem_LoadAnimResObj
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, _022209B4 ; =0x0000B017
	ldr r1, _022209B0 ; =0x00001FB4
	str r0, [sp, #8]
	ldr r1, [r5, r1]
	add r0, r4, #0
	mov r2, #0x47
	mov r3, #0x1a
	bl SpriteSystem_LoadCharResObj
	add sp, #0x18
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022209AC: .word 0x0000B00B
_022209B0: .word 0x00001FB4
_022209B4: .word 0x0000B017
	thumb_func_end ov08_02220928



	thumb_func_start ov08_022209B8
ov08_022209B8: ; 0x022209B8
	push {r4, r5, lr}
	sub sp, #0x34
	add r5, r0, #0
	ldr r0, [r5]
	add r4, r1, #0
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	mov r2, #0
	add r1, sp, #0
	strh r2, [r1]
	strh r2, [r1, #2]
	strh r2, [r1, #4]
	strh r2, [r1, #6]
	mov r1, #0x14
	ldr r3, _02220A10 ; =ov08_02225654
	mul r1, r4
	ldr r3, [r3, r1]
	str r2, [sp, #0xc]
	str r3, [sp, #8]
	mov r3, #2
	str r3, [sp, #0x10]
	ldr r3, _02220A14 ; =ov08_02225644
	str r2, [sp, #0x30]
	ldr r3, [r3, r1]
	add r2, sp, #0
	str r3, [sp, #0x14]
	ldr r3, _02220A18 ; =ov08_02225648
	ldr r3, [r3, r1]
	str r3, [sp, #0x18]
	ldr r3, _02220A1C ; =ov08_0222564C
	ldr r3, [r3, r1]
	str r3, [sp, #0x1c]
	ldr r3, _02220A20 ; =ov08_02225650
	ldr r1, [r3, r1]
	str r1, [sp, #0x20]
	mov r1, #1
	str r1, [sp, #0x2c]
	ldr r1, _02220A24 ; =0x00001FB4
	ldr r1, [r5, r1]
	bl SpriteSystem_NewSprite
	add sp, #0x34
	pop {r4, r5, pc}
	.balign 4, 0
_02220A10: .word ov08_02225654
_02220A14: .word ov08_02225644
_02220A18: .word ov08_02225648
_02220A1C: .word ov08_0222564C
_02220A20: .word ov08_02225650
_02220A24: .word 0x00001FB4
	thumb_func_end ov08_022209B8



	thumb_func_start ov08_02220A28
ov08_02220A28: ; 0x02220A28
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r7, _02220A4C ; =0x00001FB8
	mov r4, #0
	add r5, r6, #0
_02220A32:
	add r0, r6, #0
	add r1, r4, #0
	bl ov08_022209B8
	str r0, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #0x26
	blo _02220A32
	add r0, r6, #0
	bl ov08_02220AAC
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02220A4C: .word 0x00001FB8
	thumb_func_end ov08_02220A28



	thumb_func_start ov08_02220A50
ov08_02220A50: ; 0x02220A50
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	ldr r0, [r7]
	ldr r0, [r0, #8]
	bl BattleSystem_GetSpriteSystem
	ldr r6, _02220A84 ; =0x00001FB8
	str r0, [sp]
	mov r4, #0
	add r5, r7, #0
_02220A64:
	ldr r0, [r5, r6]
	bl Sprite_DeleteAndFreeResources
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #0x26
	blo _02220A64
	add r0, r7, #0
	bl ov08_02221698
	ldr r1, _02220A88 ; =0x00001FB4
	ldr r0, [sp]
	ldr r1, [r7, r1]
	bl SpriteSystem_FreeResourcesAndManager
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02220A84: .word 0x00001FB8
_02220A88: .word 0x00001FB4
	thumb_func_end ov08_02220A50
