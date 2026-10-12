#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "asm/macros.inc"
	.public CreateFieldEscapeRopeTaskEnv
	.public FieldMoveTask_CreateDigEnvironment
	.public FieldMoveTask_CreateTeleportEnvironment
	.public FieldSystem_FollowMonInteract
	.public FollowMon_ExpandInteractionMessage
	.public FollowMon_PlaceholdersSet
	.public FollowMon_TryPrintInteractionMessage
	.public PlayerStepEvent_RepelCounterDecrement
	.public PokecenterAnimCreate
	.public PokecenterAnimRun
	.public SafariDecoration_CreateArgs
	.public Task_FieldDig
	.public Task_FieldEscapeRope
	.public Task_FieldTeleport
	.public Task_FollowMonInteract
	.public _022487F4
	.public _02248804
	.public _02248818
	.public _02248824
	.public _02248838
	.public _02248844
	.public _0224885A
	.public _02248868
	.public _0224887C
	.public _02248892
	.public _0224889C
	.public _022488A8
	.public _022488BE
	.public _022488C8
	.public _022488D4
	.public _022488EA
	.public _022488F4
	.public _02248900
	.public _02248916
	.public _02248920
	.public _022489A2
	.public _022489DE
	.public _022489E6
	.public _02248A04
	.public _02248A16
	.public _02248A1E
	.public _02248A38
	.public _02248A4A
	.public _02248A52
	.public _02248A7A
	.public _02248AB8
	.public _02248AC0
	.public _02248ADC
	.public _02248AEE
	.public _02248AF6
	.public _02248B10
	.public _02248B22
	.public _02248B2A
	.public _02248B52
	.public _02248B8E
	.public _02248B96
	.public _02248BC2
	.public _02248BFE
	.public _02248C06
	.public _02248C2A
	.public _02248C92
	.public _02248D14
	.public _02248D2C
	.public _02248D88
	.public _02248DD0
	.public _02248DDA
	.public _02248DEC
	.public _02248DFC
	.public _02248E0C
	.public _02248F74
	.public _02248F78
	.public _02248F7C
	.public _02248F80
	.public _02248F84
	.public _02248FA0
	.public _02248FB2
	.public _02248FBE
	.public _02249006
	.public _02249018
	.public _0224902A
	.public _0224907A
	.public _02249080
	.public _022490E0
	.public _02249124
	.public _02249156
	.public _02249168
	.public _022491A2
	.public _022491E2
	.public _02249284
	.public _02249288
	.public _0224928C
	.public _022492B0
	.public _022492BA
	.public _02249302
	.public _02249314
	.public _02249326
	.public _0224934C
	.public _02249390
	.public _02249398
	.public _022493BA
	.public _022493C8
	.public _022493E6
	.public _022493F8
	.public _0224941C
	.public _02249440
	.public _02249490
	.public _022494AC
	.public _022494B8
	.public _022494BC
	.public _022494C0
	.public _02249538
	.public _02249580
	.public _0224958A
	.public _022495AC
	.public _022495B2
	.public _022495B4
	.public _022495FE
	.public _02249650
	.public _02249654
	.public _0224968C
	.public _022496B4
	.public _022496FE
	.public _0224970A
	.public _02249716
	.public _0224974E
	.public _0224976C
	.public _02249770
	.public _022497AC
	.public _02249802
	.public _02249834
	.public _02249850
	.public _02249854
	.public _0224986C
	.public _022498AC
	.public _022498B4
	.public _022498B8
	.public _022498EA
	.public _022498F6
	.public _02249902
	.public _0224993A
	.public _0224998A
	.public _022499AC
	.public _022499B2
	.public _022499B4
	.public _022499BE
	.public _022499E0
	.public _022499E6
	.public _022499E8
	.public _02249A02
	.public _02249A54
	.public _02249A58
	.public _02249A72
	.public _02249AC0
	.public _02249B02
	.public _02249B48
	.public _02249B7A
	.public _02249BA2
	.public _02249BB8
	.public _02249BD4
	.public _02249C06
	.public _02249C12
	.public _02249C1E
	.public _02249C4A
	.public _02249C6C
	.public _02249C92
	.public _02249C96
	.public _02249CAC
	.public _02249CBA
	.public _02249CC8
	.public _02249CEA
	.public _02249D14
	.public _02249D3C
	.public _02249D5A
	.public _02249D72
	.public _02249D7C
	.public _02249D8C
	.public _02249D96
	.public _02249DAE
	.public _02249DBE
	.public _02249DD2
	.public _02249DD4
	.public _02249DF0
	.public _02249DFA
	.public _02249E0A
	.public _02249E14
	.public _02249E32
	.public _02249E48
	.public _02249E56
	.public _02249E8A
	.public _02249E8C
	.public _02249EBE
	.public _02249F68
	.public _02249FD0
	.public _0224A024
	.public _0224A070
	.public _0224A07C
	.public _0224A196
	.public _0224A278
	.public _0224A27C
	.public _0224A280
	.public _0224A284
	.public _0224A294
	.public _0224A29E
	.public _0224A2AE
	.public _0224A2B8
	.public _0224A2C8
	.public _0224A2D2
	.public _0224A2E2
	.public _0224A2EC
	.public _0224A338
	.public _0224A384
	.public _0224A3BE
	.public _0224A3EA
	.public _0224A426
	.public _0224A4CC
	.public _0224A4E4
	.public _0224A510
	.public _0224A518
	.public _0224A520
	.public _0224A528
	.public _0224A554
	.public _0224A55C
	.public _0224A564
	.public _0224A644
	.public _0224A6BA
	.public _0224A6CC
	.public _0224A6E2
	.public _0224A6FC
	.public _0224A730
	.public _0224A748
	.public _0224A796
	.public _0224A79C
	.public _0224A7A0
	.public _0224A7A4
	.public _0224A7B4
	.public _0224A7D4
	.public _0224A80C
	.public _0224A8FE
	.public _0224A912
	.public _0224A91A
	.public _0224A922
	.public _0224A948
	.public _0224A95C
	.public _0224A964
	.public _0224A96C
	.public _0224A992
	.public _0224A9A6
	.public _0224A9AE
	.public _0224A9B6
	.public _0224A9E8
	.public _0224AA40
	.public _0224AA7C
	.public _0224AAD0
	.public _0224AB38
	.public _0224AB42
	.public _0224AB4E
	.public _0224AB88
	.public _0224ABB6
	.public _0224ABC0
	.public _0224AC00
	.public _0224AC10
	.public _0224AC20
	.public _0224ACD8
	.public _0224ACDC
	.public _0224AD00
	.public _0224AD4A
	.public _0224AD5C
	.public _0224AD6E
	.public _0224AD90
	.public _0224ADDA
	.public _0224ADE0
	.public _0224ADE4
	.public _0224ADE8
	.public _0224AF5C
	.public _0224AF60
	.public _0224AF64
	.public _0224AF68
	.public _0224AF6C
	.public _0224AF86
	.public _0224AF98
	.public _0224AFA4
	.public _0224AFEE
	.public _0224B000
	.public _0224B012
	.public _0224B03A
	.public _0224B07C
	.public _0224B082
	.public _0224B08E
	.public _0224B0B8
	.public _0224B0C8
	.public _0224B0D8
	.public _0224B0DC
	.public _0224B17C
	.public _0224B1C0
	.public _0224B1D2
	.public _0224B1E4
	.public _0224B20A
	.public _0224B25E
	.public _0224B272
	.public _0224B2BC
	.public _0224B3A8
	.public _0224B3AC
	.public _0224B3F4
	.public _0224B3F8
	.public _0224B40A
	.public _0224B410
	.public _0224B438
	.public _0224B462
	.public _0224B484
	.public _0224B48E
	.public _0224B490
	.public _0224B57C
	.public _0224B5DC
	.public _0224B64E
	.public _0224B6A0
	.public _0224B716
	.public _0224B720
	.public _0224B764
	.public _0224B782
	.public _0224B7C8
	.public _0224B824
	.public _0224B82C
	.public _0224B832
	.public _0224B854
	.public _0224B878
	.public _0224B888
	.public _0224B992
	.public _0224BA14
	.public _0224BA2E
	.public _0224BA42
	.public _0224BA50
	.public _0224BA6C
	.public _0224BA78
	.public _0224BA9C
	.public _0224BAB0
	.public _0224BAD4
	.public _0224BB0E
	.public _0224BB14
	.public _0224BB84
	.public _0224BB8C
	.public _0224BBAC
	.public _0224BBB8
	.public _0224BBC4
	.public _0224BBF6
	.public _0224BBF8
	.public _0224BC02
	.public _0224BC04
	.public _0224BC22
	.public _0224BC24
	.public _0224BC2E
	.public _0224BC30
	.public _0224BC66
	.public _0224BC98
	.public _0224BCFA
	.public _0224BD06
	.public _0224BD1E
	.public _0224BD42
	.public _0224BD64
	.public _0224BDA0
	.public _0224BDBA
	.public _0224BDC2
	.public _0224BDCC
	.public _0224BDD4
	.public _0224BDD8
	.public _0224BDDC
	.public _0224BDE0
	.public _0224BDE4
	.public _0224BE1A
	.public _0224BE20
	.public _0224BE40
	.public _0224BE4C
	.public _0224BE56
	.public _0224BE7C
	.public _0224BE7E
	.public _0224BE88
	.public _0224BE8A
	.public _0224BE9E
	.public _0224BED2
	.public _0224BEE0
	.public _0224BEE8
	.public _0224BF00
	.public _0224BF2C
	.public _0224BF3E
	.public _0224BF48
	.public _0224BF50
	.public _0224BF54
	.public _0224BFB4
	.public _0224BFBC
	.public _0224BFC8
	.public _0224BFD4
	.public _0224C006
	.public _0224C016
	.public _0224C01A
	.public _0224C032
	.public _0224C04E
	.public _0224C058
	.public _0224C092
	.public _0224C0A4
	.public _0224C0A8
	.public _0224C0AC
	.public _0224C0C8
	.public _0224C0F0
	.public _0224C100
	.public _0224C120
	.public _0224C138
	.public _0224C144
	.public _0224C148
	.public _0224C184
	.public _0224C190
	.public _0224C1A2
	.public _0224C1B4
	.public _0224C20C
	.public _0224C228
	.public _0224C230
	.public _0224C25A
	.public _0224C270
	.public _0224C29C
	.public _0224C2A0
	.public _0224C2A4
	.public _0224C2C6
	.public _0224C2D2
	.public _0224C2E8
	.public _0224C2FE
	.public _0224C31C
	.public _0224C330
	.public _0224C334
	.public _0224C34A
	.public _0224C358
	.public _0224C3A4
	.public _0224C3A8
	.public _0224C3D8
	.public _0224C3E0
	.public _0224C3F0
	.public _0224C3F8
	.public _0224C41E
	.public _0224C420
	.public _0224C42A
	.public _0224C456
	.public _0224C45C
	.public _0224C46E
	.public _0224C482
	.public _0224C494
	.public _0224C49E
	.public _0224C4A2
	.public _0224C4A8
	.public _0224C4AC
	.public _0224C4B0
	.public _0224C4EA
	.public _0224C534
	.public _0224C54E
	.public _0224C552
	.public _0224C584
	.public _0224C58C
	.public _0224C59C
	.public _0224C5A4
	.public _0224C5CA
	.public _0224C5CC
	.public _0224C5D6
	.public _0224C602
	.public _0224C608
	.public _0224C61A
	.public _0224C62E
	.public _0224C640
	.public _0224C64A
	.public _0224C64E
	.public _0224C654
	.public _0224C658
	.public _0224C65C
	.public _0224C670
	.public _0224C6D0
	.public _0224C6D4
	.public _0224C6D8
	.public _0224C6F0
	.public _0224C718
	.public _0224C750
	.public _0224C754
	.public _0224C758
	.public _0224C774
	.public _0224C79C
	.public _0224C7AC
	.public _0224C7D0
	.public _0224C80C
	.public _0224C818
	.public _0224C82A
	.public _0224C83C
	.public _0224C8B2
	.public _0224C8C4
	.public _0224C8C8
	.public _0224C8CC
	.public _0224C8E4
	.public _0224C90E
	.public _0224C928
	.public _0224C92E
	.public _0224C938
	.public _0224C954
	.public _0224C97C
	.public _0224C98C
	.public _0224C9B0
	.public _0224C9B4
	.public _0224C9F2
	.public _0224C9FE
	.public _0224CA18
	.public _0224CA24
	.public _0224CA34
	.public _0224CA6C
	.public _0224CA7E
	.public _0224CA9A
	.public _0224CAB4
	.public _0224CB06
	.public _0224CB14
	.public _0224CB30
	.public _0224CB34
	.public _0224CB44
	.public _0224CB48
	.public _0224CB58
	.public _0224CB5A
	.public _0224CB76
	.public _0224CBB4
	.public _0224CBEE
	.public _0224CC46
	.public _0224CC54
	.public _0224CC70
	.public _0224CC74
	.public _0224CC84
	.public _0224CC88
	.public _0224CC98
	.public _0224CC9A
	.public _0224CCB6
	.public _0224CCF4
	.public _0224CCFC
	.public _0224CD30
	.public _0224CD34
	.public _0224CE1C
	.public _0224CE24
	.public _0224CE44
	.public _0224CE50
	.public _0224CE5C
	.public _0224CE82
	.public _0224CE84
	.public _0224CE8E
	.public _0224CE90
	.public _0224CEBA
	.public _0224CF1C
	.public _0224CF28
	.public _0224CF40
	.public _0224CF64
	.public _0224CF76
	.public _0224CF94
	.public _0224CFAE
	.public _0224CFB6
	.public _0224CFC0
	.public _0224CFC8
	.public _0224CFCC
	.public _0224CFD0
	.public _0224CFD4
	.public _0224D010
	.public _0224D018
	.public _0224D01C
	.public _0224D020
	.public _0224D024
	.public _0224D028
	.public _0224D02C
	.public _0224D078
	.public _0224D080
	.public _0224D084
	.public _0224D088
	.public _0224D08C
	.public _0224D090
	.public _0224D094
	.public _0224D114
	.public _0224D13E
	.public _0224D160
	.public _0224D176
	.public _0224D18C
	.public _0224D1A6
	.public _0224D1C2
	.public _0224D1D8
	.public _0224D1E0
	.public _0224D228
	.public _0224D274
	.public _0224D284
	.public _0224D29A
	.public _0224D2B8
	.public _0224D2C4
	.public _0224D2D4
	.public _0224D2D8
	.public _0224D2E8
	.public _0224D2EC
	.public _0224D2F4
	.public _0224D30A
	.public _0224D354
	.public _0224D3A0
	.public _0224D3B0
	.public _0224D3C6
	.public _0224D3E4
	.public _0224D3F0
	.public _0224D400
	.public _0224D404
	.public _0224D414
	.public _0224D418
	.public _0224D420
	.public _0224D436
	.public _0224D464
	.public _0224D474
	.public _0224D4A4
	.public _0224D4AC
	.public _0224D50A
	.public _0224D522
	.public _0224D55E
	.public _0224D570
	.public _0224D574
	.public _0224D578
	.public _0224D57C
	.public _0224D586
	.public _0224D5A4
	.public _0224D5A8
	.public _0224D5B0
	.public _0224D640
	.public _0224D644
	.public _0224D654
	.public _0224D678
	.public _0224D688
	.public _0224D68C
	.public _0224D694
	.public _0224D6B0
	.public _0224D6D8
	.public _0224D6FC
	.public _0224D716
	.public _0224D73A
	.public _0224D75E
	.public _0224D794
	.public _0224D7F8
	.public _0224D806
	.public _0224D81C
	.public _0224D82C
	.public _0224D858
	.public _0224D870
	.public _0224D8B4
	.public _0224D8D8
	.public _0224D90C
	.public _0224D910
	.public _0224D920
	.public _0224D93C
	.public _0224D94C
	.public _0224D980
	.public _0224D986
	.public _0224D988
	.public _0224D994
	.public _0224D9B0
	.public _0224D9B4
	.public _0224D9BC
	.public _0224DA46
	.public _0224DA4E
	.public _0224DA5A
	.public _0224DA66
	.public _0224DA72
	.public _0224DA7C
	.public _0224DA9C
	.public _0224DAA0
	.public _0224DB2C
	.public _0224DB34
	.public _0224DB40
	.public _0224DB4C
	.public _0224DB58
	.public _0224DB62
	.public _0224DB84
	.public _0224DB88
	.public _0224DB98
	.public _0224DBB0
	.public _0224DBF4
	.public _0224DC12
	.public _0224DC1C
	.public _0224DC38
	.public _0224DC4C
	.public _0224DC50
	.public _0224DC54
	.public _0224DC60
	.public _0224DC70
	.public _0224DC74
	.public _0224DC84
	.public _0224DC88
	.public _0224DC90
	.public _0224DCA6
	.public _0224DCAC
	.public _0224DCE0
	.public _0224DD02
	.public _0224DD28
	.public _0224DD2C
	.public _0224DD30
	.public _0224DD34
	.public _0224DD48
	.public _0224DD58
	.public _0224DD72
	.public _0224DD84
	.public _0224DD88
	.public _0224DDBC
	.public _0224DDC2
	.public _0224DDC4
	.public _0224DDD0
	.public _0224DDEC
	.public _0224DDF0
	.public _0224DE00
	.public _0224DE04
	.public _0224DE0C
	.public _0224DE26
	.public _0224DE46
	.public _0224DE68
	.public _0224DE82
	.public _0224DEA6
	.public _0224DECA
	.public _0224DF00
	.public _0224DF5A
	.public _0224DF80
	.public _0224DF88
	.public _0224DF90
	.public _0224DF94
	.public _0224DFBC
	.public _0224DFC6
	.public _0224DFCA
	.public _0224DFF2
	.public _0224DFFA
	.public _0224E004
	.public _0224E010
	.public _0224E032
	.public _0224E04A
	.public _0224E068
	.public _0224E06C
	.public _0224E070
	.public _0224E08C
	.public _0224E0B4
	.public _0224E0B8
	.public _0224E0D0
	.public _0224E108
	.public _0224E114
	.public _0224E120
	.public _0224E130
	.public _0224E156
	.public _0224E1AA
	.public _0224E1C6
	.public _0224E1F6
	.public _0224E21E
	.public _0224E23C
	.public _0224E246
	.public _0224E256
	.public _0224E25E
	.public _0224E264
	.public _0224E268
	.public _0224E27E
	.public _0224E286
	.public _0224E28A
	.public _0224E28E
	.public _0224E292
	.public _0224E296
	.public _0224E2B2
	.public _0224E2BA
	.public _0224E2BE
	.public _0224E2C2
	.public _0224E2C6
	.public _0224E2CA
	.public _0224E2E8
	.public _0224E2F0
	.public _0224E2F4
	.public _0224E2F8
	.public _0224E2FC
	.public _0224E300
	.public _0224E312
	.public _0224E318
	.public _0224E33C
	.public _0224E33E
	.public _0224E38E
	.public _0224E39E
	.public _0224E3AE
	.public _0224E3BE
	.public _0224E3F8
	.public _0224E3FE
	.public _0224E40E
	.public _0224E414
	.public _0224E43C
	.public _0224E458
	.public _0224E468
	.public _0224E482
	.public _0224E490
	.public _0224E4BA
	.public _0224E4C4
	.public _0224E4D6
	.public _0224E4E6
	.public _0224E528
	.public _0224E52A
	.public _0224E584
	.public _0224E5D4
	.public _0224E606
	.public _0224E638
	.public _0224E672
	.public _0224E67C
	.public _0224E692
	.public _0224E706
	.public _0224E716
	.public _0224E726
	.public _0224E74E
	.public _0224E7D4
	.public _0224E80A
	.public _0224E810
	.public _0224E81C
	.public _0224E85C
	.public _0224E868
	.public _0224E870
	.public _0224E87E
	.public _0224E880
	.public _0224E8A4
	.public _0224E8F4
	.public _0224E904
	.public _0224E91C
	.public _0224E92A
	.public _0224E95C
	.public _0224E972
	.public _0224E984
	.public _0224E98A
	.public _0224E992
	.public _0224E99C
	.public _0224E99E
	.public _0224E9DC
	.public _0224E9EC
	.public _0224EA0A
	.public _0224EA18
	.public _0224EA56
	.public _0224EA5C
	.public _0224EA6A
	.public _0224EAAE
	.public _0224EABE
	.public _0224EAD6
	.public _0224EAE4
	.public _0224EB32
	.public _0224EB3E
	.public _0224EB44
	.public _0224EB7A
	.public _0224EB86
	.public _0224EB8E
	.public _0224EB96
	.public _0224EB98
	.public _0224EBD8
	.public _0224EBE8
	.public _0224EC04
	.public _0224EC12
	.public _0224EC50
	.public _0224EC56
	.public _0224EC64
	.public _0224ECA4
	.public _0224ECB4
	.public _0224ECD0
	.public _0224ECDE
	.public _0224ED2C
	.public _0224ED3A
	.public _0224ED4E
	.public _0224ED58
	.public _0224ED70
	.public _0224EDB4
	.public _0224EDC4
	.public _0224EDE0
	.public _0224EDEE
	.public _0224EE20
	.public _0224EE42
	.public _0224EE48
	.public _0224EE8E
	.public _0224EE96
	.public _0224EEAE
	.public _0224EEE6
	.public _0224EEEA
	.public _0224EEF2
	.public _0224EEFC
	.public _0224EF14
	.public _0224EF46
	.public _0224EF4A
	.public _0224EF58
	.public _0224EF60
	.public _0224EF68
	.public _0224EF78
	.public _0224EF8C
	.public _0224EF90
	.public _0224EFE2
	.public _0224EFF8
	.public _0224F00A
	.public _0224F018
	.public _0224F044
	.public _0224F100
	.public _0224F104
	.public _0224F124
	.public _0224F13E
	.public _0224F154
	.public _0224F15E
	.public _0224F16E
	.public _0224F194
	.public _0224F19C
	.public _0224F1B2
	.public _0224F1BA
	.public _0224F1D0
	.public _0224F1D8
	.public _0224F1EA
	.public _0224F200
	.public _0224F20E
	.public _0224F212
	.public _0224F22A
	.public _0224F23C
	.public _0224F24C
	.public _0224F25C
	.public _0224F26E
	.public _0224F280
	.public _0224F298
	.public _0224F2AA
	.public _0224F2C0
	.public _0224F2D2
	.public _0224F2E6
	.public _0224F2FA
	.public _0224F30C
	.public _0224F31E
	.public _0224F34E
	.public _0224F354
	.public _0224F380
	.public _0224F38A
	.public _0224F394
	.public _0224F39E
	.public _0224F3A2
	.public _0224F3B8
	.public _0224F3C4
	.public _0224F3D0
	.public _0224F3DC
	.public _0224F3E8
	.public _0224F3F2
	.public _0224F3FA
	.public _0224F410
	.public _0224F41C
	.public _0224F420
	.public _0224F474
	.public _0224F476
	.public _0224F490
	.public _0224F4B4
	.public _0224F4B8
	.public _0224F4FC
	.public _0224F52C
	.public _0224F536
	.public _0224F540
	.public _0224F568
	.public _0224F57A
	.public _0224F5A4
	.public _0224F5BE
	.public _0224F5C6
	.public _0224F5E8
	.public _0224F5EE
	.public _0224F5F4
	.public _0224F63A
	.public _0224F640
	.public _0224F664
	.public _0224F66E
	.public _0224F674
	.public _0224F67A
	.public _0224F680
	.public _0224F686
	.public _0224F68C
	.public _0224F6F4
	.public _0224F704
	.public _0224F714
	.public _0224F722
	.public _0224F744
	.public _0224F74C
	.public _0224F752
	.public _0224F758
	.public _0224F75E
	.public _0224F764
	.public _0224F77C
	.public _0224F782
	.public _0224F798
	.public _0224F7AE
	.public _0224F7D2
	.public _0224F7D6
	.public _0224F7DA
	.public _0224F7DE
	.public _0224F7E2
	.public _0224F7E6
	.public _0224F7EA
	.public _0224F7EE
	.public _0224F7F2
	.public _0224F7F6
	.public _0224F7FA
	.public _0224F7FE
	.public _0224F802
	.public _0224F806
	.public _0224F80A
	.public _0224F80E
	.public _0224F812
	.public _0224F816
	.public _0224F832
	.public _0224F840
	.public _0224F844
	.public _0224F848
	.public _0224F84C
	.public _0224F850
	.public _0224F854
	.public _0224F858
	.public _0224F85C
	.public _0224F87C
	.public _0224F8BE
	.public _0224F8CC
	.public _0224F8E4
	.public _0224F8E8
	.public _0224F8EC
	.public _0224F8F0
	.public _0224F8F8
	.public _0224F920
	.public _0224F92E
	.public _0224F93A
	.public _0224F93E
	.public _0224F962
	.public _0224F970
	.public _0224F97C
	.public _0224F980
	.public _0224F98C
	.public _0224F998
	.public _0224F9A8
	.public _0224F9CA
	.public _0224F9E8
	.public _0224FA06
	.public _0224FA1A
	.public _0224FA3E
	.public _0224FA50
	.public _0224FAA8
	.public _0224FABA
	.public _0224FAEE
	.public _0224FB1A
	.public _0224FB28
	.public _0224FB2C
	.public _0224FB30
	.public _0224FB34
	.public _0224FB38
	.public _0224FB3C
	.public _0224FB40
	.public _0224FB4E
	.public _0224FB74
	.public _0224FB82
	.public _0224FB92
	.public _0224FB98
	.public _0224FC00
	.public _0224FC04
	.public _0224FC6C
	.public _0224FC70
	.public _0224FC82
	.public _0224FC88
	.public _0224FD30
	.public _0224FD3E
	.public _0224FD70
	.public _0224FD72
	.public _0224FDBC
	.public _0224FDDC
	.public _0224FDEC
	.public _0224FE16
	.public _0224FE18
	.public _0224FE2C
	.public _0224FE32
	.public _0224FE38
	.public _0224FE3C
	.public _0224FE6C
	.public _0224FE9E
	.public _0224FEE0
	.public _0224FEFC
	.public _0224FF00
	.public _0224FF2C
	.public _0224FF34
	.public _0224FF3C
	.public _0224FF44
	.public _0224FF4C
	.public _0224FF54
	.public _0224FFA8
	.public _0224FFC6
	.public _0224FFCC
	.public _0224FFD0
	.public _0224FFD4
	.public _0224FFE4
	.public _0224FFF6
	.public _02250000
	.public _02250016
	.public _0225002A
	.public _02250034
	.public _02250040
	.public _02250082
	.public _02250094
	.public _02250098
	.public _022500AE
	.public _02250102
	.public _02250108
	.public _0225010C
	.public _0225012A
	.public _02250136
	.public _02250142
	.public _0225015C
	.public _02250178
	.public _0225018E
	.public _02250198
	.public _022501A2
	.public _022501BE
	.public _022501C6
	.public _022501DE
	.public _02250218
	.public _02250246
	.public _02250270
	.public _02250272
	.public _022502BA
	.public _022502C4
	.public _0225030A
	.public _02250314
	.public _02250332
	.public _02250336
	.public _02250364
	.public _02250372
	.public _02250378
	.public _02250392
	.public _0225039E
	.public _022503A2
	.public _022503A6
	.public _022503AA
	.public _022503AE
	.public _022503B2
	.public _022503BA
	.public _02250404
	.public _02250412
	.public _02250418
	.public _0225047E
	.public _02250484
	.public _02250488
	.public _0225048C
	.public _02250490
	.public _02250494
	.public _02250498
	.public _0225049C
	.public _02250538
	.public _02250542
	.public _02250576
	.public _0225057E
	.public _0225058C
	.public _02250590
	.public _022505A4
	.public _022505BA
	.public _022505C2
	.public _022505CE
	.public _022505DA
	.public _022505E6
	.public _022505F2
	.public _022505FE
	.public _0225060A
	.public _02250612
	.public _0225061A
	.public _02250622
	.public _02250638
	.public _0225064E
	.public _02250656
	.public _02250662
	.public _0225066E
	.public _0225067A
	.public _0225068A
	.public _0225069C
	.public _022506AE
	.public _022506BA
	.public _022506C2
	.public _022506CE
	.public _022506E2
	.public _022506F4
	.public _022506FE
	.public _02250706
	.public _0225070E
	.public _0225071A
	.public _02250722
	.public _0225072A
	.public _02250732
	.public _02250760
	.public _0225076C
	.public _0225077C
	.public _022507AC
	.public _022507B0
	.public _022507E0
	.public _022507E4
	.public _02250814
	.public _0225081E
	.public _0225082E
	.public _0225084C
	.public _02250874
	.public _0225089A
	.public _022508A0
	.public _022508AA
	.public _022508B0
	.public _022508D4
	.public _022508FC
	.public _02250908
	.public _02250910
	.public _0225099A
	.public _022509AE
	.public _022509CE
	.public _02250A2C
	.public _02250A3A
	.public _02250A44
	.public _02250A4C
	.public _02250A50
	.public _02250A54
	.public _02250A58
	.public _02250A5C
	.public _02253E20
	.public SpriteGfxManager_Init
	.public SpriteGfxManager_Free
	.public SpriteGfxManager_LoadChar
	.public SpriteGfxManager_TransferChar
	.public SpriteGfxManager_ReleaseCharData
	.public SpriteGfxManager_LoadPltt
	.public SpriteGfxManager_TransferPltt
	.public SpriteGfxManager_ReleasePlttData
	.public SpriteGfxManager_LoadCell
	.public SpriteGfxManager_LoadAnim
	.public SpriteGfxManager_CreateSprite
	.public Sprite_CopyPositionVector
	.public FlyLanding_CreateFlyerSprite
	.public FlyLanding_CreateTrainerSprite
	.public FlyLandingEffect_Create
	.public FlyLandingEffect_GetStatus
	.public FlyLandingEffect_Init
	.public FlyLandingEffect_Delete
	.public FlyLandingEffect_Destroy
	.public FlyLandingEffect_Update
	.public FlyLandingEffect_Idle
	.public FlyLandingEffect_Start
	.public FlyLandingEffect_SwoopIn
	.public FlyLandingEffect_StartClimb
	.public FlyLandingEffect_Climb
	.public FlyLandingEffect_HideFlyer
	.public FlyLandingEffect_StartFlyAway
	.public FlyLandingEffect_FlyAway
	.public FlyLandingEffect_ShowPlayer
	.public FlyLandingEffect_Done
	.public FlyLanding_OpenNarc
	.public FieldMove_FadeInFromWhite
	.public FieldMove_FadeOutToWhite
	.public FieldMove_SetPlayerVisible
	.public ov02_02249458
	.public ov02_022494C4
	.public ov02_0224953C
	.public ov02_02249548
	.public ov02_0224955C
	.public ov02_0224957C
	.public ov02_02249584
	.public ov02_022495B8
	.public ov02_022495D0
	.public ov02_022495E8
	.public ov02_02249658
	.public ov02_02249690
	.public ov02_022496D0
	.public ov02_02249754
	.public ov02_02249774
	.public ov02_022497C0
	.public ov02_02249838
	.public ov02_02249858
	.public ov02_022498BC
	.public ov02_02249940
	.public ov02_02249954
	.public ov02_02249968
	.public ov02_0224997C
	.public ov02_02249984
	.public ov02_022499B8
	.public ov02_022499EC
	.public ov02_02249A5C
	.public ov02_02249AC4
	.public ov02_02249AD8
	.public ov02_02249AF0
	.public ov02_02249B10
	.public ov02_02249B38
	.public ov02_02249B60
	.public ov02_02249B80
	.public ov02_02249BA8
	.public ov02_02249BD8
	.public ov02_02249C74
	.public ov02_02249CD8
	.public ov02_02249CF0
	.public ov02_02249D18
	.public ov02_02249D40
	.public ov02_02249D5C
	.public ov02_02249DD8
	.public ov02_02249E58
	.public ov02_02249E90
	.public ov02_02249EC0
	.public ov02_02249F6C
	.public ov02_02249FD4
	.public ov02_0224A028
	.public ov02_0224A074
	.public ov02_0224A080
	.public ov02_0224A288
	.public ov02_0224A32C
	.public ov02_0224A33C
	.public ov02_0224A3F0
	.public ov02_0224A418
	.public ov02_0224A450
	.public ov02_0224A468
	.public ov02_0224A4D0
	.public ov02_0224A570
	.public ov02_0224A598
	.public ov02_0224A5D0
	.public ov02_0224A63C
	.public ov02_0224A648
	.public ov02_0224A66C
	.public ov02_0224A674
	.public ov02_0224A67C
	.public ov02_0224A690
	.public ov02_0224A69C
	.public ov02_0224A6A8
	.public ov02_0224A6D0
	.public Task_FieldMoveCutIn_UpdateWindow
	.public FieldMoveCutIn_GetMonPicIds
	.public FieldMoveCutIn_LoadMonChar
	.public FieldMoveCutIn_LoadMonPltt
	.public FieldMoveCutIn_AddMonCharResource
	.public FieldMoveCutIn_TransferMonChar
	.public FieldMoveCutIn_AddMonPlttResource
	.public FieldMoveCutIn_TransferMonPltt
	.public FieldMoveCutIn_FreeMonResources
	.public FieldMoveCutIn_CreateMonSprite
	.public FieldMoveCutIn_CreateScrollSprites
	.public FieldMoveCutIn_CreateScrollSprite
	.public CutInScrollSprite_Init
	.public CutInScrollSprite_Destroy
	.public CutInScrollSprite_Update
	.public CutInScrollSprite_Draw
	.public FieldMoveCutIn_CreateFlyer
	.public FieldMoveCutIn_GetFlyerStatus
	.public FieldMoveCutIn_DestroyFlyer
	.public CutInFlyer_Init
	.public CutInFlyer_Destroy
	.public CutInFlyer_Update
	.public CutInFlyer_Draw
	.public CutInFlyer_Hide
	.public FieldMoveCutIn_StartFlyerTakeOff
	.public CutInFlyer_TakeOff
	.public CutInFlyer_Idle
	.public FieldMoveCutIn_StartFlyerPickUp
	.public CutInFlyer_SwoopDown
	.public CutInFlyer_PickUpTrainer
	.public CutInFlyer_FlyAway
	.public CutInFlyer_Done
	.public FieldMoveCutIn_CreateTrainerJumpEffect
	.public TrainerJumpEffect_Stop
	.public TrainerJumpEffect_StartJump
	.public TrainerJumpEffect_StartHop
	.public TrainerJumpEffect_Init
	.public TrainerJumpEffect_Jump
	.public TrainerJumpEffect_Hop
	.public TrainerJumpEffect_Update
	.public FlyLanding_Start
	.public FlyLanding_IsDone
	.public FlyLanding_Delete
	.public Task_FlyLanding
	.public FlyLanding_SetPlayerVisible
	.public FlyLanding_LoadGraphics
	.public FlyLanding_CreateSprites
	.public FlyLanding_WaitLanding
	.public FlyLanding_FreeGraphics
	.public FlyLanding_Idle
	.public FlyLanding_AllocZeroed
	.public FieldMoveCutIn_SetPlayerVisible
	.public TrainerClipEffect_Init
	.public TrainerClipEffect_Update
	.public FieldMoveCutIn_CreateTrainerClipEffect
	.public FieldMoveCutIn_DestroyTrainerClipEffect
	.public FieldMoveCutIn_CreateModelEffect
	.public CutInModelEffect_Init
	.public CutInModelEffect_Destroy
	.public CutInModelEffect_Update
	.public CutInModelEffect_Draw
	.public FieldMoveCutIn_LoadModel
	.public FieldMoveCutIn_UnloadModel
	.public FieldMoveCutIn_StepShowOtherObject
	.public FieldMoveCutIn_StepWaitModelEffect
	.public Roamers_GetActiveRoamerBySpecies
	.public ElevatorAnimCreate
	.public ElevatorAnimRun
	.public ov02_0224BF58
	.public ov02_0224BFC0
	.public ov02_0224BFCC
	.public FieldEscapeRope_SpinStart
	.public FieldEscapeRope_SpinAndFadeOut
	.public FieldEscapeRope_WaitFadeOut
	.public FieldEscapeRope_Warp
	.public FieldMoveTask_CreateArrivalEnvironment
	.public Task_FieldMoveArrival
	.public FieldMoveArrival_FadeIn
	.public FieldMoveArrival_WaitFadeIn
	.public FieldMoveArrival_Spin
	.public FieldMoveArrival_Finish
	.public FieldMove_ShowCutIn
	.public FieldMove_WaitCutIn
	.public FieldMoveTaskEnvironment_New
	.public FieldTeleport_FollowMonFlash
	.public FieldTeleport_FollowMonSpinStart
	.public FieldTeleport_FollowMonWaitSpin
	.public FieldTeleport_SpinStart
	.public FieldTeleport_SpinAndFadeOut
	.public FieldTeleport_WaitFadeOut
	.public FieldTeleport_Warp
	.public FieldDig_SinkStart
	.public FieldDig_DigHole
	.public FieldDig_SpinAndFadeOut
	.public FieldDig_WaitFadeOut
	.public FieldDig_Warp
	.public PrimoPassword_RotateBitsRight
	.public PrimoPassword_DecodeWallpaper
	.public PrimoPassword_DecodeCheck2
	.public PrimoPassword_CheckWallpaper
	.public PrimoPassword_Check2
	.public HallOfFame_StartBallAnim
	.public Task_HallOfFameBallAnim
	.public Field3dObject_SetPosInFrontOfMapObject
	.public Field3dObject_SetPosInFrontOfPlayer
	.public Field3dObject_SetPosAtPlayer
	.public FieldMoveEffectModel_Load
	.public FieldMoveEffectModel_Unload
	.public FieldMoveEffectModel_AnimateOnce
	.public FieldMoveEffectModel_AnimateLoop
	.public FieldMoveEffectModel_Draw
	.public CutEffect_InitAtPlayer
	.public CutEffect_InitAtFollower
	.public CutEffect_Destroy
	.public CutEffect_Update
	.public CutEffect_Render
	.public CutEffect_CreateAtPlayer
	.public CutEffect_CreateAtFollower
	.public CutEffect_Delete
	.public CutEffect_IsFinished
	.public RockSmashEffect_InitAtPlayer
	.public RockSmashEffect_InitAtFollower
	.public RockSmashEffect_Destroy
	.public RockSmashEffect_Update
	.public RockSmashEffect_Render
	.public RockSmashEffect_CreateAtPlayer
	.public RockSmashEffect_CreateAtFollower
	.public RockSmashEffect_Delete
	.public RockSmashEffect_IsFinished
	.public WaterfallEffect_Init
	.public WaterfallEffect_Destroy
	.public WaterfallEffect_Update
	.public WaterfallEffect_Render
	.public WaterfallEffect_Create
	.public WaterfallEffect_Delete
	.public WhirlpoolEffect_Init
	.public WhirlpoolEffect_Destroy
	.public WhirlpoolEffect_Update
	.public WhirlpoolEffect_Render
	.public WhirlpoolEffect_Create
	.public WhirlpoolEffect_Delete
	.public RockClimbDebris_SpawnPiece
	.public RockClimbDebris_UpdatePiece
	.public RockClimbDebris_InitPiece
	.public RockClimbDebris_FreePiece
	.public RockClimbDebris_UpdateOffsets
	.public RockClimbDebris_Spawn
	.public RockClimbDebris_UpdatePieces
	.public RockClimbDebris_Init
	.public RockClimbDebris_Destroy
	.public RockClimbDebris_Update
	.public RockClimbDebris_Render
	.public RockClimbDebris_Create
	.public RockClimbDebris_Delete
	.public HeadbuttEffect_InitAtPlayer
	.public HeadbuttEffect_InitAtFollower
	.public HeadbuttEffect_Destroy
	.public HeadbuttEffect_Update
	.public HeadbuttEffect_Render
	.public HeadbuttEffect_CreateAtPlayer
	.public HeadbuttEffect_CreateAtFollower
	.public HeadbuttEffect_Delete
	.public HeadbuttEffect_IsFinished
	.public RockDebris_Init
	.public RockDebris_InitWithFollower
	.public RockDebris_Destroy
	.public RockDebris_Update
	.public RockDebris_Render
	.public RockDebris_Create
	.public RockDebris_CreateWithFollower
	.public RockDebris_Delete
	.public RockDebris_SpawnPiece
	.public RockDebris_UpdatePiece
	.public RockDebris_InitPiece
	.public RockDebris_FreePiece
	.public RockDebris_Spawn
	.public RockDebris_UpdatePieces
	.public Task_ScriptFieldMoveEffect
	.public ScriptFieldMoveEffect_Start
	.public MapObjects_SwapPositions
	.public SwapPositions_CreateTaskEnv
	.public Task_SwapMapObjectPositions
	.public SwapPositions_GetStepTowards
	.public SwapPositions_GetFaceMovement
	.public SwapPositions_GetTurnAroundMovement
	.public SwapPositions_GetOppositeStep
	.public MapId_IsSafariZone
	.public SafariZone_GetAreaIndexAt
	.public FieldSystem_GetSafariAreaIndex
	.public FieldSystem_CanPlaceSafariObject
	.public SafariZone_CanPlaceObjectOnLand
	.public SafariZone_CanPlaceObjectOnWater
	.public ov02_0224E640
	.public FieldSystem_CheckSafariObjectPlacement
	.public FieldSystem_GetFacingSafariObject
	.public SafariZone_FindSpotFor1x2Object
	.public SafariZone_FindSpotFor2x1Object
	.public SafariZone_FindSpotFor2x2Object
	.public SafariZone_CanPlaceObjectOn
	.public FollowMon_SelectInteraction
	.public FollowMon_GetSituation
	.public FollowMonRule_Matches
	.public FollowMonSituation_SetMonCondition
	.public FollowMonSituation_SetNearbyObjects
	.public FollowMonSituation_SetHiddenItems
	.public FollowMonSituation_SetWeather
	.public FollowMonSituation_SetTerrain
	.public FollowMonSituation_SetMapId
	.public FollowMonSituation_SetTimeOfDay
	.public FollowMonSituation_SetMood
	.public FollowMonSituation_SetBestPokeathlonStat
	.public FollowMonSituation_SetFacing
	.public FollowMonSituation_SetSpeciesAttr
	.public FollowMonSituation_ConvertType
	.public FollowMonSituation_ConvertItemPocket
	.public FollowMonInteraction_New
	.public FollowMonInteraction_LoadScript
	.public FollowMonInteraction_Delete
	.public FollowMonInteraction_Run
	.public FollowMonInteraction_StepHasMovement
	.public FollowMonInteraction_TryShowEmote
	.public FollowMonInteraction_PrintGiftMessage
	.public FollowMonInteraction_StepHasWait
	.public FollowMon_ExpandGiftMessage
	.public FollowMonMovement_ApplyOffset
	.public FollowMonMovement_PlaySound
	.public FollowMonMovement_ApplyFacing
	.public FollowMon_TryShakeGrass
	.public FollowMon_GetCoordsInDirection
	.public FollowMonMovement_Step
	.public FollowMonMovement_HasNextStep
	.public FollowMonInteraction_RunMovement
	.public FollowMon_ApplyInteractionEffects
	.public FollowMonRule_CheckFriendship
	.public FollowMonRule_CheckMood
	.public FollowMonRule_CheckSpeciesAttr
	.public FollowMonRule_CheckShinyLeaf
	.public FieldSystem_FirstAliveMonHasType
	.public ov02_022507B4
	.public ov02_022507E8
	.public ov02_022508B4
	.public ov02_022508D8
	.public sFlyLandingIdleSteps
	.public ov02_022532FC
	.public ov02_02253300
	.public ov02_02253304
	.public ov02_0225330A
	.public ov02_02253310
	.public sFlyLandingPhases
	.public ov02_02253330
	.public ov02_02253348
	.public ov02_02253354
	.public sFlyLandingFlyerScale
	.public ov02_0225336C
	.public ov02_02253378
	.public ov02_02253384
	.public ov02_02253390
	.public sFlyLandingSwoopCenter
	.public sFlyLandingTrainerScale
	.public sFlyLandingFlyAwayScale
	.public ov02_022533C0
	.public sFlyLandingSwoopScale
	.public sFlyLandingFlyAwayCenter
	.public ov02_022533E4
	.public ov02_022533F0
	.public ov02_022533FC
	.public ov02_02253408
	.public ov02_02253414
	.public ov02_02253420
	.public ov02_02253430
	.public ov02_02253440
	.public sFlyLandingEffectTemplate
	.public ov02_02253468
	.public ov02_0225347C
	.public ov02_02253490
	.public ov02_022534A4
	.public ov02_022534B8
	.public sFlyLandingSteps
	.public ov02_022534F0
	.public ov02_02253520
	.public ov02_02253550
	.public ov02_02253588
	.public ov02_022535E4
	.public ov02_022536E8
	.public ov02_022536F0
	.public ov02_02253700
	.public ov02_02253710
	.public ov02_02253724
	.public ov02_0225373C
	.public ov02_02253754
	.public ov02_02253770
	.public ov02_02253794
	.public ov02_022537B8
	.public ov02_022537DC
	.public ov02_02253820
	.public ov02_02253884
	.public sRideEffectFileIds
	.public sRockSmashEffectAtPlayerTemplate
	.public sWhirlpoolEffectTemplate
	.public sHeadbuttEffectAtFollowerTemplate
	.public sWaterfallEffectTemplate
	.public sRockClimbDebrisTemplate
	.public sCutEffectAtPlayerTemplate
	.public sRockSmashEffectAtFollowerTemplate
	.public sHeadbuttEffectAtPlayerTemplate
	.public sCutEffectAtFollowerTemplate
	.public sRockDebrisTemplate
	.public sRockDebrisWithFollowerTemplate
	.public sScriptFieldMoveEffectDeleteFuncs
	.public sScriptFieldMoveEffectCreateFuncs
	.public sScriptFieldMoveEffectIsFinishedFuncs
	.public ov02_02253A4C
	.public sShinyLeafMasks
	.public sShinyLeafMonDataIds
	.public ov02_02253A70
	.public sNatureGroups
	.public ov02_02253B24
	.public ov02_02253B30
	.public ov02_02253D90
	.public ov02_02253D94
	.public ov02_02253D98
	.public sHallOfFameBallOffsets
	.public ov02_02253DDC
	.public ov02_02253DE0
	.public ov02_BattleExit_HandleRoamerAction
	.public ov02_RepelActiveRoamersFromMapNo
	.include "overlay_02.inc"
	.include "global.inc"

	.text
	.public FieldSystem_FollowMonInteract
	.public Sprite_CopyPositionVector
	.public FlyLandingEffect_GetStatus
	.public FlyLandingEffect_Destroy
	.public FlyLandingEffect_Idle
	.public FlyLandingEffect_Done
	.public FlyLanding_OpenNarc
	.public FieldMove_FadeInFromWhite
	.public FieldMove_FadeOutToWhite
	.public FieldMove_SetPlayerVisible
	.public ov02_0224953C
	.public ov02_02249548
	.public ov02_0224957C
	.public ov02_022495B8
	.public ov02_022495D0
	.public ov02_02249940
	.public ov02_02249954
	.public ov02_02249968
	.public ov02_0224997C
	.public ov02_02249AC4
	.public ov02_02249AD8
	.public ov02_02249AF0
	.public ov02_02249B10
	.public ov02_02249B38
	.public ov02_02249CD8
	.public ov02_02249CF0
	.public ov02_02249D18
	.public ov02_02249D40
	.public ov02_0224A074
	.public ov02_0224A32C
	.public ov02_0224A450
	.public ov02_0224A63C
	.public ov02_0224A648
	.public ov02_0224A66C
	.public ov02_0224A674
	.public ov02_0224A67C
	.public ov02_0224A690
	.public ov02_0224A69C
	.public ov02_0224A6A8
	.public ov02_0224A6D0
	.public FieldMoveCutIn_LoadMonPltt
	.public CutInScrollSprite_Destroy
	.public CutInScrollSprite_Draw
	.public FieldMoveCutIn_GetFlyerStatus
	.public FieldMoveCutIn_DestroyFlyer
	.public CutInFlyer_Destroy
	.public CutInFlyer_Draw
	.public CutInFlyer_Hide
	.public CutInFlyer_Idle
	.public CutInFlyer_Done
	.public TrainerJumpEffect_Stop
	.public TrainerJumpEffect_Init
	.public FlyLanding_Start
	.public FlyLanding_IsDone
	.public FlyLanding_Delete
	.public FlyLanding_SetPlayerVisible
	.public FlyLanding_Idle
	.public FlyLanding_AllocZeroed
	.public FieldMoveCutIn_SetPlayerVisible
	.public FieldMoveCutIn_DestroyTrainerClipEffect
	.public CutInModelEffect_Destroy
	.public CutInModelEffect_Draw
	.public ov02_0224BFC0
	.public ov02_0224BFCC
	.public FieldEscapeRope_Warp
	.public FieldMoveTask_CreateArrivalEnvironment
	.public FieldMove_WaitCutIn
	.public FieldMoveTaskEnvironment_New
	.public FieldTeleport_FollowMonFlash
	.public FieldDig_Warp
	.public Field3dObject_SetPosAtPlayer
	.public FieldMoveEffectModel_Draw
	.public CutEffect_Destroy
	.public CutEffect_Render
	.public CutEffect_CreateAtPlayer
	.public CutEffect_CreateAtFollower
	.public CutEffect_Delete
	.public CutEffect_IsFinished
	.public RockSmashEffect_Destroy
	.public RockSmashEffect_Render
	.public RockSmashEffect_CreateAtPlayer
	.public RockSmashEffect_CreateAtFollower
	.public RockSmashEffect_Delete
	.public RockSmashEffect_IsFinished
	.public WaterfallEffect_Create
	.public WaterfallEffect_Delete
	.public WhirlpoolEffect_Destroy
	.public WhirlpoolEffect_Update
	.public WhirlpoolEffect_Render
	.public WhirlpoolEffect_Create
	.public WhirlpoolEffect_Delete
	.public RockClimbDebris_Create
	.public RockClimbDebris_Delete
	.public HeadbuttEffect_Destroy
	.public HeadbuttEffect_Render
	.public HeadbuttEffect_CreateAtPlayer
	.public HeadbuttEffect_CreateAtFollower
	.public HeadbuttEffect_Delete
	.public HeadbuttEffect_IsFinished
	.public RockDebris_InitWithFollower
	.public RockDebris_Create
	.public RockDebris_CreateWithFollower
	.public RockDebris_Delete
	.public MapObjects_SwapPositions
	.public SwapPositions_CreateTaskEnv
	.public MapId_IsSafariZone
	.public SafariZone_GetAreaIndexAt
	.public FieldSystem_GetSafariAreaIndex
	.public SafariZone_CanPlaceObjectOnLand
	.public SafariZone_CanPlaceObjectOnWater
	.public FollowMonSituation_SetMapId
	.public FollowMonSituation_SetMood
	.public FollowMonInteraction_New
	.public FollowMonInteraction_Delete
	.public FollowMonInteraction_StepHasMovement
	.public FollowMonInteraction_StepHasWait
	.public ov02_022508B4

	thumb_func_start PrimoPassword_RotateBitsRight
PrimoPassword_RotateBitsRight: ; 0x0224CA58
	push {r4, r5, r6, r7}
	add r3, r0, #0
	add r0, r2, #0
	sub r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r2, #0
	beq _0224CAB4
	add r6, r3, r1
	sub r7, r1, #1
_0224CA6C:
	sub r1, r6, #1
	ldrb r2, [r1]
	mov r1, #1
	add r5, r7, #0
	and r1, r2
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	cmp r7, #0
	ble _0224CA9A
_0224CA7E:
	ldrb r1, [r3, r5]
	add r2, r3, r5
	sub r2, r2, #1
	asr r1, r1, #1
	strb r1, [r3, r5]
	ldrb r2, [r2]
	ldrb r1, [r3, r5]
	lsl r2, r2, #0x1f
	lsr r2, r2, #0x18
	orr r1, r2
	strb r1, [r3, r5]
	sub r5, r5, #1
	cmp r5, #0
	bgt _0224CA7E
_0224CA9A:
	ldrb r1, [r3, r5]
	asr r1, r1, #1
	strb r1, [r3, r5]
	ldrb r2, [r3, r5]
	lsl r1, r4, #7
	orr r1, r2
	strb r1, [r3, r5]
	add r1, r0, #0
	sub r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r1, #0
	bne _0224CA6C
_0224CAB4:
	pop {r4, r5, r6, r7}
	bx lr
	thumb_func_end PrimoPassword_RotateBitsRight


	thumb_func_start PrimoPassword_DecodeWallpaper
PrimoPassword_DecodeWallpaper: ; 0x0224CAB8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	add r6, r2, #0
	str r1, [sp]
	add r7, r3, #0
	bl WallpaperPasswordBank_GetCount
	add r4, r0, #0
	add r0, r5, #0
	add r1, r6, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #4]
	add r0, r5, #0
	add r1, r7, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #6]
	add r1, sp, #0x18
	ldrh r1, [r1, #0x10]
	add r0, r5, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #8]
	add r1, sp, #0x18
	ldrh r1, [r1, #0x14]
	add r0, r5, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	mov r2, #0
	strh r0, [r1, #0xa]
	add r3, sp, #8
	add r5, sp, #4
	add r7, r2, #0
_0224CB06:
	ldrsh r0, [r3, r7]
	cmp r0, #0
	bge _0224CB14
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CB14:
	cmp r2, #0
	ble _0224CB48
	mov r6, #1
	mvn r6, r6
	ldrsh r6, [r3, r6]
	cmp r0, r6
	blt _0224CB34
	sub r0, r0, r6
	cmp r0, #0xff
	ble _0224CB30
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CB30:
	strb r0, [r5]
	b _0224CB5A
_0224CB34:
	sub r0, r6, r0
	sub r0, r4, r0
	cmp r0, #0xff
	ble _0224CB44
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CB44:
	strb r0, [r5]
	b _0224CB5A
_0224CB48:
	mov r0, #4
	ldrsh r0, [r1, r0]
	cmp r0, #0xff
	ble _0224CB58
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CB58:
	strb r0, [r1]
_0224CB5A:
	add r2, r2, #1
	add r3, r3, #2
	add r5, r5, #1
	cmp r2, #4
	blt _0224CB06
	add r0, sp, #4
	mov r1, #4
	mov r2, #5
	bl PrimoPassword_RotateBitsRight
	mov r2, #0
	add r1, sp, #4
	add r4, sp, #4
	mov r3, #0xf0
_0224CB76:
	ldrb r5, [r4, #3]
	ldrb r0, [r1]
	add r2, r2, #1
	asr r6, r5, #4
	and r5, r3
	orr r5, r6
	eor r0, r5
	strb r0, [r1]
	add r1, r1, #1
	cmp r2, #3
	blt _0224CB76
	add r2, sp, #4
	ldrb r3, [r2, #3]
	mov r2, #0xf
	add r0, sp, #4
	and r2, r3
	lsl r2, r2, #0x18
	mov r1, #3
	lsr r2, r2, #0x18
	bl PrimoPassword_RotateBitsRight
	add r1, sp, #4
	ldrb r5, [r1]
	mov r0, #0xf
	add r4, r5, #0
	and r4, r0
	cmp r4, #8
	blt _0224CBB4
	add sp, #0x10
	sub r0, #0x10
	pop {r3, r4, r5, r6, r7, pc}
_0224CBB4:
	ldrb r0, [r1, #1]
	eor r0, r5
	strb r0, [r1, #1]
	ldrb r0, [r1, #2]
	eor r0, r5
	strb r0, [r1, #2]
	ldrb r2, [r1, #1]
	ldrb r3, [r1, #2]
	lsl r0, r2, #8
	add r6, r0, #0
	ldr r0, [sp]
	orr r6, r3
	cmp r0, r6
	bne _0224CBEE
	mov r0, #0xf0
	and r0, r5
	asr r0, r0, #4
	cmp r0, #6
	bne _0224CBEE
	ldrb r0, [r1, #3]
	add r1, r5, r2
	mul r1, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	cmp r0, r1
	bne _0224CBEE
	add sp, #0x10
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
_0224CBEE:
	mov r0, #0
	mvn r0, r0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PrimoPassword_DecodeWallpaper


	thumb_func_start PrimoPassword_DecodeCheck2
PrimoPassword_DecodeCheck2: ; 0x0224CBF8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	add r6, r2, #0
	str r1, [sp]
	add r7, r3, #0
	bl WallpaperPasswordBank_GetCount
	add r4, r0, #0
	add r0, r5, #0
	add r1, r6, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #4]
	add r0, r5, #0
	add r1, r7, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #6]
	add r1, sp, #0x18
	ldrh r1, [r1, #0x10]
	add r0, r5, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	strh r0, [r1, #8]
	add r1, sp, #0x18
	ldrh r1, [r1, #0x14]
	add r0, r5, #0
	bl WallpaperPasswordBank_GetIndexOfWord
	add r1, sp, #4
	mov r2, #0
	strh r0, [r1, #0xa]
	add r3, sp, #8
	add r5, sp, #4
	add r7, r2, #0
_0224CC46:
	ldrsh r0, [r3, r7]
	cmp r0, #0
	bge _0224CC54
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CC54:
	cmp r2, #0
	ble _0224CC88
	mov r6, #1
	mvn r6, r6
	ldrsh r6, [r3, r6]
	cmp r0, r6
	blt _0224CC74
	sub r0, r0, r6
	cmp r0, #0xff
	ble _0224CC70
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CC70:
	strb r0, [r5]
	b _0224CC9A
_0224CC74:
	sub r0, r6, r0
	sub r0, r4, r0
	cmp r0, #0xff
	ble _0224CC84
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CC84:
	strb r0, [r5]
	b _0224CC9A
_0224CC88:
	mov r0, #4
	ldrsh r0, [r1, r0]
	cmp r0, #0xff
	ble _0224CC98
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CC98:
	strb r0, [r1]
_0224CC9A:
	add r2, r2, #1
	add r3, r3, #2
	add r5, r5, #1
	cmp r2, #4
	blt _0224CC46
	add r0, sp, #4
	mov r1, #4
	mov r2, #5
	bl PrimoPassword_RotateBitsRight
	mov r2, #0
	add r1, sp, #4
	add r4, sp, #4
	mov r3, #0xf0
_0224CCB6:
	ldrb r5, [r4, #3]
	ldrb r0, [r1]
	add r2, r2, #1
	asr r6, r5, #4
	and r5, r3
	orr r5, r6
	eor r0, r5
	strb r0, [r1]
	add r1, r1, #1
	cmp r2, #3
	blt _0224CCB6
	add r2, sp, #4
	ldrb r3, [r2, #3]
	mov r2, #0xf
	add r0, sp, #4
	and r2, r3
	lsl r2, r2, #0x18
	mov r1, #3
	lsr r2, r2, #0x18
	bl PrimoPassword_RotateBitsRight
	add r1, sp, #4
	ldrb r4, [r1]
	mov r0, #0xf
	and r0, r4
	lsl r2, r0, #0x18
	lsr r2, r2, #0x18
	cmp r2, #8
	blo _0224CCF4
	cmp r2, #0xb
	blo _0224CCFC
_0224CCF4:
	mov r0, #0
	add sp, #0x10
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_0224CCFC:
	ldrb r2, [r1, #1]
	eor r2, r4
	strb r2, [r1, #1]
	ldrb r2, [r1, #2]
	eor r2, r4
	strb r2, [r1, #2]
	ldrb r2, [r1, #1]
	ldrb r3, [r1, #2]
	lsl r5, r2, #8
	add r6, r5, #0
	ldr r5, [sp]
	orr r6, r3
	cmp r5, r6
	bne _0224CD30
	mov r5, #0xf0
	and r5, r4
	asr r5, r5, #4
	cmp r5, #6
	bne _0224CD30
	add r2, r4, r2
	mul r2, r3
	lsl r2, r2, #0x18
	ldrb r1, [r1, #3]
	lsr r2, r2, #0x18
	cmp r1, r2
	beq _0224CD34
_0224CD30:
	mov r0, #0
	mvn r0, r0
_0224CD34:
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end PrimoPassword_DecodeCheck2


	thumb_func_start PrimoPassword_CheckWallpaper
PrimoPassword_CheckWallpaper: ; 0x0224CD38
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r6, r0, #0
	ldr r0, [sp, #0x24]
	add r7, r1, #0
	str r2, [sp, #8]
	add r5, r3, #0
	bl WallpaperPasswordBank_Create
	add r4, r0, #0
	add r0, r6, #0
	bl PlayerProfile_GetTrainerID_VisibleHalf
	add r1, r0, #0
	str r5, [sp]
	add r0, sp, #0x10
	ldrh r0, [r0, #0x10]
	add r2, r7, #0
	str r0, [sp, #4]
	ldr r3, [sp, #8]
	add r0, r4, #0
	bl PrimoPassword_DecodeWallpaper
	add r5, r0, #0
	add r0, r4, #0
	bl WallpaperPasswordBank_Delete
	add r0, r5, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end PrimoPassword_CheckWallpaper


	thumb_func_start PrimoPassword_Check2
PrimoPassword_Check2: ; 0x0224CD74
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r6, r0, #0
	ldr r0, [sp, #0x24]
	add r7, r1, #0
	str r2, [sp, #8]
	add r5, r3, #0
	bl WallpaperPasswordBank_Create
	add r4, r0, #0
	add r0, r6, #0
	bl PlayerProfile_GetTrainerID_VisibleHalf
	add r1, r0, #0
	str r5, [sp]
	add r0, sp, #0x10
	ldrh r0, [r0, #0x10]
	add r2, r7, #0
	str r0, [sp, #4]
	ldr r3, [sp, #8]
	add r0, r4, #0
	bl PrimoPassword_DecodeCheck2
	add r5, r0, #0
	add r0, r4, #0
	bl WallpaperPasswordBank_Delete
	add r0, r5, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end PrimoPassword_Check2


	thumb_func_start HallOfFame_StartBallAnim
HallOfFame_StartBallAnim: ; 0x0224CDB0
	push {r4, r5, r6, lr}
	sub sp, #0x20
	add r6, r1, #0
	mov r1, #0x26
	add r2, sp, #4
	add r3, sp, #0
	add r5, r0, #0
	bl FieldSystem_FindMapProp
	cmp r0, #0
	beq _0224CE1C
	mov r0, #4
	mov r1, #0x18
	bl Heap_AllocAtEnd
	add r4, r0, #0
	strb r6, [r4, #0xc]
	mov r0, #0
	strb r0, [r4, #0xd]
	strb r0, [r4, #0xe]
	strb r0, [r4, #0xf]
	ldr r0, [r5, #0x30]
	bl MapMatrix_GetWidth
	add r1, r0, #0
	ldr r0, [sp]
	add r2, sp, #0x14
	bl Terrain_GetChunkOrigin
	ldr r1, [sp, #4]
	add r0, sp, #8
	bl MapProp_GetTranslation
	add r3, sp, #8
	ldmia r3!, {r0, r1}
	add r2, r4, #0
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r1, [r4]
	ldr r0, [sp, #0x14]
	add r2, r4, #0
	add r0, r1, r0
	str r0, [r4]
	ldr r1, [r4, #8]
	ldr r0, [sp, #0x1c]
	add r0, r1, r0
	str r0, [r4, #8]
	ldr r0, [r5, #0x10]
	ldr r1, _0224CE24 ; =Task_HallOfFameBallAnim
	bl TaskManager_Call
	add sp, #0x20
	pop {r4, r5, r6, pc}
_0224CE1C:
	bl GF_AssertFail
	add sp, #0x20
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0224CE24: .word Task_HallOfFameBallAnim
	thumb_func_end HallOfFame_StartBallAnim


	thumb_func_start Task_HallOfFameBallAnim
Task_HallOfFameBallAnim: ; 0x0224CE28
	push {r4, r5, r6, lr}
	sub sp, #0x30
	add r4, r0, #0
	bl TaskManager_GetFieldSystem
	add r6, r0, #0
	add r0, r4, #0
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	ldrb r1, [r4, #0xf]
	cmp r1, #5
	bls _0224CE44
	b _0224CFC0
_0224CE44:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0224CE50: ; jump table
	.short _0224CE5C - _0224CE50 - 2 ; case 0
	.short _0224CEBA - _0224CE50 - 2 ; case 1
	.short _0224CF1C - _0224CE50 - 2 ; case 2
	.short _0224CF64 - _0224CE50 - 2 ; case 3
	.short _0224CF76 - _0224CE50 - 2 ; case 4
	.short _0224CFB6 - _0224CE50 - 2 ; case 5
_0224CE5C:
	ldr r1, [r6, #0x34]
	mov r0, #0x6b
	bl AreaDataManager_GetMapPropModel
	ldr r0, [r0]
	bl NNS_G3dGetMdlSet
	cmp r0, #0
	beq _0224CE8E
	add r2, r0, #0
	add r2, #8
	beq _0224CE82
	ldrb r1, [r0, #9]
	cmp r1, #0
	bls _0224CE82
	ldrh r1, [r0, #0xe]
	add r1, r2, r1
	add r1, r1, #4
	b _0224CE84
_0224CE82:
	mov r1, #0
_0224CE84:
	cmp r1, #0
	beq _0224CE8E
	ldr r1, [r1]
	add r5, r0, r1
	b _0224CE90
_0224CE8E:
	mov r5, #0
_0224CE90:
	ldr r0, [r6, #0x34]
	bl AreaDataManager_GetMapPropTexture
	mov r1, #0
	str r1, [sp]
	str r5, [sp, #4]
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	str r0, [sp, #0x10]
	str r1, [sp, #0x14]
	ldr r0, [r6, #0x54]
	ldr r1, [r6, #0x58]
	mov r2, #0x10
	mov r3, #0x6b
	bl MapPropOneShotAnimationManager_LoadPropAnimations
	ldrb r0, [r4, #0xf]
	add r0, r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CEBA:
	add r1, sp, #0x18
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
	str r0, [r1, #8]
	ldrb r2, [r4, #0xd]
	mov r1, #0xc
	ldr r0, [r4]
	add r3, r2, #0
	ldr r2, _0224CFC8 ; =sHallOfFameBallOffsets
	mul r3, r1
	ldr r2, [r2, r3]
	add r0, r0, r2
	str r0, [sp, #0x24]
	ldrb r2, [r4, #0xd]
	ldr r0, [r4, #4]
	add r3, r2, #0
	ldr r2, _0224CFCC ; =ov02_02253DDC
	mul r3, r1
	ldr r2, [r2, r3]
	add r0, r0, r2
	str r0, [sp, #0x28]
	ldrb r2, [r4, #0xd]
	ldr r0, [r4, #8]
	add r3, r2, #0
	mul r3, r1
	ldr r1, _0224CFD0 ; =ov02_02253DE0
	ldr r1, [r1, r3]
	add r0, r0, r1
	str r0, [sp, #0x2c]
	ldr r0, _0224CFD4 ; =SEQ_SE_DP_BOWA
	bl PlaySE
	ldr r0, [r6, #0x54]
	add r6, #0x9c
	str r0, [sp]
	ldr r0, [r6]
	mov r1, #0x6b
	add r2, sp, #0x24
	add r3, sp, #0x18
	bl MapPropManager_LoadOne
	ldrb r1, [r4, #0xd]
	add r1, r4, r1
	strb r0, [r1, #0x10]
	ldrb r0, [r4, #0xf]
	add r0, r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CF1C:
	ldrb r0, [r4, #0xe]
	cmp r0, #0xf
	bhs _0224CF28
	add r0, r0, #1
	strb r0, [r4, #0xe]
	b _0224CFC0
_0224CF28:
	mov r0, #0
	strb r0, [r4, #0xe]
	ldrb r0, [r4, #0xd]
	add r0, r0, #1
	strb r0, [r4, #0xd]
	ldrb r1, [r4, #0xd]
	ldrb r0, [r4, #0xc]
	cmp r1, r0
	bhs _0224CF40
	mov r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CF40:
	add r0, r6, #0
	add r0, #0x9c
	ldrb r1, [r4, #0x10]
	ldr r0, [r0]
	bl MapPropManager_GetMapPropByIndex_Checked_RequireActive
	bl MapProp_GetRenderSurface
	add r3, r0, #0
	ldr r0, [r6, #0x58]
	mov r1, #0x10
	mov r2, #0
	bl MapPropOneShotAnimationManager_SetAnimationRenderObj
	ldrb r0, [r4, #0xf]
	add r0, r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CF64:
	ldr r0, [r6, #0x58]
	mov r1, #0x10
	mov r2, #0
	bl MapPropOneShotAnimationManager_PlayAnimation
	ldrb r0, [r4, #0xf]
	add r0, r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CF76:
	ldr r0, [r6, #0x58]
	mov r1, #0x10
	bl MapPropOneShotAnimationManager_IsAnimationLoopFinished
	cmp r0, #0
	beq _0224CFC0
	ldr r0, [r6, #0x54]
	ldr r1, [r6, #0x58]
	mov r2, #0x10
	bl MapPropOneShotAnimationManager_UnloadAnimation
	ldrb r0, [r4, #0xc]
	mov r5, #0
	cmp r0, #0
	bls _0224CFAE
_0224CF94:
	add r0, r4, r5
	add r1, r6, #0
	add r1, #0x9c
	ldrb r0, [r0, #0x10]
	ldr r1, [r1]
	bl MapPropManager_RemoveMapPropByIndex
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldrb r0, [r4, #0xc]
	cmp r5, r0
	blo _0224CF94
_0224CFAE:
	ldrb r0, [r4, #0xf]
	add r0, r0, #1
	strb r0, [r4, #0xf]
	b _0224CFC0
_0224CFB6:
	bl Heap_Free
	add sp, #0x30
	mov r0, #1
	pop {r4, r5, r6, pc}
_0224CFC0:
	mov r0, #0
	add sp, #0x30
	pop {r4, r5, r6, pc}
	nop
_0224CFC8: .word sHallOfFameBallOffsets
_0224CFCC: .word ov02_02253DDC
_0224CFD0: .word ov02_02253DE0
_0224CFD4: .word SEQ_SE_DP_BOWA
	thumb_func_end Task_HallOfFameBallAnim


	thumb_func_start Field3dObject_SetPosInFrontOfMapObject
Field3dObject_SetPosInFrontOfMapObject: ; 0x0224CFD8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r2, #0
	bl MapObjectManager_GetFirstActiveObjectByID
	add r6, r0, #0
	add r1, sp, #0
	bl MapObject_CopyPositionVector
	add r0, r6, #0
	bl MapObject_GetXCoord
	add r5, r0, #0
	add r0, r6, #0
	bl MapObject_GetZCoord
	add r4, r0, #0
	add r0, r6, #0
	bl MapObject_GetFacingDirection
	cmp r0, #3
	bhi _0224D028
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0224D010: ; jump table
	.short _0224D018 - _0224D010 - 2 ; case 0
	.short _0224D01C - _0224D010 - 2 ; case 1
	.short _0224D020 - _0224D010 - 2 ; case 2
	.short _0224D024 - _0224D010 - 2 ; case 3
_0224D018:
	sub r4, r4, #1
	b _0224D02C
_0224D01C:
	add r4, r4, #1
	b _0224D02C
_0224D020:
	sub r5, r5, #1
	b _0224D02C
_0224D024:
	add r5, r5, #1
	b _0224D02C
_0224D028:
	bl GF_AssertFail
_0224D02C:
	mov r3, #2
	lsl r1, r5, #0x10
	lsl r3, r3, #0xe
	ldr r2, [sp, #4]
	lsl r4, r4, #0x10
	add r1, r1, r3
	add r0, r7, #0
	add r3, r4, r3
	bl Field3dObject_SetPosEx
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end Field3dObject_SetPosInFrontOfMapObject


	thumb_func_start Field3dObject_SetPosInFrontOfPlayer
Field3dObject_SetPosInFrontOfPlayer: ; 0x0224D044
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r1, #0
	add r6, r0, #0
	add r1, sp, #0
	bl PlayerAvatar_CopyPositionVector
	add r0, r6, #0
	bl PlayerAvatar_GetXCoord
	add r5, r0, #0
	add r0, r6, #0
	bl PlayerAvatar_GetZCoord
	add r4, r0, #0
	add r0, r6, #0
	bl PlayerAvatar_GetFacingDirection
	cmp r0, #3
	bhi _0224D090
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0224D078: ; jump table
	.short _0224D080 - _0224D078 - 2 ; case 0
	.short _0224D084 - _0224D078 - 2 ; case 1
	.short _0224D088 - _0224D078 - 2 ; case 2
	.short _0224D08C - _0224D078 - 2 ; case 3
_0224D080:
	sub r4, r4, #1
	b _0224D094
_0224D084:
	add r4, r4, #1
	b _0224D094
_0224D088:
	sub r5, r5, #1
	b _0224D094
_0224D08C:
	add r5, r5, #1
	b _0224D094
_0224D090:
	bl GF_AssertFail
_0224D094:
	mov r3, #2
	lsl r1, r5, #0x10
	lsl r3, r3, #0xe
	ldr r2, [sp, #4]
	lsl r4, r4, #0x10
	add r1, r1, r3
	add r0, r7, #0
	add r3, r4, r3
	bl Field3dObject_SetPosEx
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end Field3dObject_SetPosInFrontOfPlayer
