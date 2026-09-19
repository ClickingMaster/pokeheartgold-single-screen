	.include "asm/macros.inc"
	.include "overlay_07.inc"
	.include "global.inc"

	.extern _02234B80
	.extern _02237840
	.extern ov07_02233EB8
	.extern ov07_02233EBC
	.extern ov07_022344B4
	.extern ov07_022344C0
	.extern ov07_02234530
	.extern ov07_0223474C
	.extern ov07_0223475C
	.extern ov07_0223494C
	.extern ov07_02234A20
	.extern ov07_02234B8C
	.extern ov07_02234B98
	.extern ov07_02234BA8
	.extern ov07_02234BC0
	.extern ov07_02234BD8
	.extern ov07_02234BF0
	.extern ov07_02234C08
	.extern ov07_02234C20
	.extern ov07_02234C40
	.extern ov07_02234C64
	.extern ov07_02234CC8
	.extern ov07_02234D58
	.extern ov07_02234DE8
	.extern ov07_02234F48
	.extern ov07_022353E4
	.extern ov07_02235440
	.extern ov07_02235488
	.extern ov07_022354D0
	.extern ov07_022354E0
	.extern ov07_02235510
	.extern ov07_02235540
	.extern ov07_02235578
	.extern ov07_022355B0
	.extern ov07_02235620
	.extern ov07_02235624
	.extern ov07_02235628
	.extern ov07_022357D0
	.extern ov07_02235980
	.extern ov07_02235984
	.extern ov07_02235988
	.extern ov07_02235B30
	.extern ov07_02235B34
	.extern ov07_02235B38
	.extern ov07_02235CE0
	.extern ov07_02235CE4
	.extern ov07_02235CE8
	.extern ov07_02235E90
	.extern ov07_02235E94
	.extern ov07_02235E98
	.extern ov07_02236040
	.extern ov07_02236044
	.extern ov07_02236048
	.extern ov07_022361F0
	.extern ov07_022361F4
	.extern ov07_022361F8
	.extern ov07_022363A0
	.extern ov07_022363D8
	.extern ov07_02236414
	.extern ov07_02236444
	.extern ov07_02236468
	.extern ov07_0223646A
	.extern ov07_0223649C
	.extern ov07_02236520
	.extern ov07_02236670
	.extern ov07_022366C8
	.extern ov07_022366CC
	.extern ov07_022366CD
	.extern ov07_022366CE
	.extern ov07_022366D2
	.extern ov07_022366D8
	.extern ov07_022366D9
	.extern ov07_022366DA
	.extern ov07_022366E1
	.extern ov07_022366EB
	.extern ov07_022366EC
	.extern ov07_022366ED
	.extern ov07_022366EE
	.extern ov07_022366EF
	.extern ov07_022366F5
	.extern ov07_022366F6
	.extern ov07_022366F7
	.extern ov07_022366F8
	.extern ov07_022366F9
	.extern ov07_02236704
	.extern ov07_02236705
	.extern ov07_02236706
	.extern ov07_02236707
	.extern ov07_02236708
	.extern ov07_02236713
	.extern ov07_02236714
	.extern ov07_02236715
	.extern ov07_02236716
	.extern ov07_02236717
	.extern ov07_02236722
	.extern ov07_02236732
	.extern ov07_02236734
	.extern ov07_02236736
	.extern ov07_0223674C
	.extern ov07_02236750
	.extern ov07_02236774
	.extern ov07_02236778
	.extern ov07_0223677E
	.extern ov07_02236780
	.extern ov07_02236796
	.extern ov07_022367AE
	.extern ov07_022367B0
	.extern ov07_022367B2
	.extern ov07_022367B4
	.extern ov07_022367CE
	.extern ov07_02236800
	.extern ov07_0223680C
	.extern ov07_02236810
	.extern ov07_02236814
	.extern ov07_02236858
	.extern ov07_02236864
	.extern ov07_0223687C
	.extern ov07_0223717C
	.extern ov07_02237188
	.extern ov07_022371B8
	.extern ov07_02237200
	.extern ov07_02237228
	.extern ov07_02237254
	.extern ov07_0223729C
	.extern ov07_0223730E
	.extern ov07_02237310
	.extern ov07_0223738C
	.extern ov07_02237390
	.extern ov07_0223746C
	.extern ov07_02237470
	.extern ov07_02237474
	.extern ov07_022375BC
	.extern ov07_02237870
	.extern ov07_0223789C
	.extern ov07_022378B0

	.text

	.extern ov07_0221BE20
	.extern ov07_0221BE44
	.extern ov07_0221BE68
	.extern ov07_0221BEA4
	.extern ov07_0221BEDC
	.extern ov07_0221BFC0
	.extern ov07_0221BFD0
	.extern ov07_0221BFE0
	.extern ov07_0221C01C
	.extern ov07_0221C394
	.extern ov07_0221C3B0
	.extern ov07_0221C3C0
	.extern ov07_0221C3DC
	.extern ov07_0221C3F4
	.extern ov07_0221C410
	.extern ov07_0221C42C
	.extern ov07_0221C448
	.extern ov07_0221C458
	.extern ov07_0221C468
	.extern ov07_0221C470
	.extern ov07_0221C478
	.extern ov07_0221C488
	.extern ov07_0221C494
	.extern ov07_0221C4A0
	.extern ov07_0221C4A8
	.extern ov07_0221C4C0
	.extern ov07_0221C4E8
	.extern ov07_0221C514
	.extern ov07_0221C528
	.extern ov07_0221C530
	.extern ov07_0221C53C
	.extern ov07_0221C56C
	.extern ov07_0221C584
	.extern ov07_0221C5C4
	.extern ov07_0221C5C8
	.extern ov07_0221C624
	.extern ov07_0221C62C
	.extern ov07_0221C634
	.extern ov07_0221C674
	.extern ov07_0221C69C
	.extern ov07_0221C6B4
	.extern ov07_0221C6EC
	.extern ov07_0221C70C
	.extern ov07_0221C72C
	.extern ov07_0221C74C
	.extern ov07_0221C768
	.extern ov07_0221C7B8
	.extern ov07_0221C8CC
	.extern ov07_0221C910
	.extern ov07_0221C934
	.extern ov07_0221C974
	.extern ov07_0221C9C0
	.extern ov07_0221CBC0
	.extern ov07_0221CC54
	.extern ov07_0221CCF4
	.extern ov07_0221CD4C
	.extern ov07_0221CDA4
	.extern ov07_0221CE5C
	.extern ov07_0221CF14
	.extern ov07_0221CF54
	.extern ov07_0221CFA8
	.extern ov07_0221D000
	.extern ov07_0221D02C
	.extern ov07_0221D05C
	.extern ov07_0221D088
	.extern ov07_0221D0C0
	.extern ov07_0221D0E8
	.extern ov07_0221D148
	.extern ov07_0221D174
	.extern ov07_0221D1B8
	.extern ov07_0221D214
	.extern ov07_0221D23C
	.extern ov07_0221D270
	.extern ov07_0221D2B0
	.extern ov07_0221D2E4
	.extern ov07_0221D2F4
	.extern ov07_0221D330
	.extern ov07_0221D374
	.extern ov07_0221D3CC
	.extern ov07_0221D4B0
	.extern ov07_0221D4FC
	.extern ov07_0221D55C
	.extern ov07_0221D5A8
	.extern ov07_0221D5AC
	.extern ov07_0221D5B0
	.extern ov07_0221D718
	.extern ov07_0221D740
	.extern ov07_0221D7B8
	.extern ov07_0221D874
	.extern ov07_0221DA74
	.extern ov07_0221DAA0
	.extern ov07_0221DAD0
	.extern ov07_0221DAF4
	.extern ov07_0221DCA8
	.extern ov07_0221DCC0
	.extern ov07_0221DCD4
	.extern ov07_0221DCF0
	.extern ov07_0221DD0C
	.extern ov07_0221DD10
	.extern ov07_0221DD14
	.extern ov07_0221DD38
	.extern ov07_0221DD8C
	.extern ov07_0221DDB0
	.extern ov07_0221DE04
	.extern ov07_0221DEB0
	.extern ov07_0221DEC0
	.extern ov07_0221DF1C
	.extern ov07_0221DFB4
	.extern ov07_0221E0B4
	.extern ov07_0221E280
	.extern ov07_0221E3B8
	.extern ov07_0221E5E0
	.extern ov07_0221E5EC
	.extern ov07_0221E600
	.extern ov07_0221E664
	.extern ov07_0221E6C8
	.extern ov07_0221E6D0
	.extern ov07_0221E6D4
	.extern ov07_0221E788
	.extern ov07_0221E868
	.extern ov07_0221E87C
	.extern ov07_0221E914
	.extern ov07_0221E9D4
	.extern ov07_0221EB84
	.extern ov07_0221EB98
	.extern ov07_0221EBA4
	.extern ov07_0221EC7C
	.extern ov07_0221ECE8
	.extern ov07_0221ED00
	.extern ov07_0221ED14
	.extern ov07_0221ED44
	.extern ov07_0221ED94
	.extern ov07_0221EE74
	.extern ov07_0221EEC0
	.extern ov07_0221EF14
	.extern ov07_0221EF34
	.extern ov07_0221EF54
	.extern ov07_0221F024
	.extern ov07_0221F084
	.extern ov07_0221F088
	.extern ov07_0221F08C
	.extern ov07_0221F090
	.extern ov07_0221F094
	.extern ov07_0221F098
	.extern ov07_0221F0B0
	.extern ov07_0221F0CC
	.extern ov07_0221F104
	.extern ov07_0221F120
	.extern ov07_0221F1C0
	.extern ov07_0221F238
	.extern ov07_0221F2DC
	.extern ov07_0221F340
	.extern ov07_0221F398
	.extern ov07_0221F39C
	.extern ov07_0221F3F4
	.extern ov07_0221F424
	.extern ov07_0221F42C
	.extern ov07_0221F430
	.extern ov07_0221F4CC
	.extern ov07_0221F514
	.extern ov07_0221F574
	.extern ov07_0221F5BC
	.extern ov07_0221F604
	.extern ov07_0221F710
	.extern ov07_0221F7C4
	.extern ov07_0221F7F0
	.extern ov07_0221F814
	.extern ov07_0221F81C
	.extern ov07_0221F880
	.extern ov07_0221F8AC
	.extern ov07_0221F8B0
	.extern ov07_0221F8C4
	.extern ov07_0221F8C8
	.extern ov07_0221F980
	.extern ov07_0221F9A8
	.extern ov07_0221F9E8
	.extern ov07_0221FA04
	.extern ov07_0221FA10
	.extern ov07_0221FA1C
	.extern ov07_0221FA2C
	.extern ov07_0221FA38
	.extern ov07_0221FA48
	.extern ov07_0221FA78
	.extern ov07_0221FA80
	.extern ov07_0221FA90
	.extern ov07_0221FAA0
	.extern ov07_0221FAB0
	.extern ov07_0221FAC8
	.extern ov07_0221FAE8
	.extern ov07_0221FAEC
	.extern ov07_0221FAF8
	.extern ov07_0221FB04
	.extern ov07_0221FB30
	.extern ov07_0221FB58
	.extern ov07_0221FB78
	.extern ov07_0221FB7C
	.extern ov07_0221FB90
	.extern ov07_0221FDFC
	.extern ov07_0221FE08
	.extern ov07_0221FE3C
	.extern ov07_0221FE70
	.extern ov07_0221FE84
	.extern ov07_0221FE88
	.extern ov07_0221FEB0
	.extern ov07_0221FEDC
	.extern ov07_0221FEE4
	.extern ov07_0221FF18
	.extern ov07_0221FF2C
	.extern ov07_0221FF34
	.extern ov07_0221FF74
	.extern ov07_0221FF78
	.extern ov07_02220030
	.extern ov07_0222008C
	.extern ov07_022200D0
	.extern ov07_02220114
	.extern ov07_02220160
	.extern ov07_022201C8
	.extern ov07_02220230
	.extern ov07_022202D8
	.extern ov07_0222036C
	.extern ov07_0222043C
	.extern ov07_0222050C
	.extern ov07_022207D0
	.extern ov07_02220800
	.extern ov07_0222080C
	.extern ov07_02220818
	.extern ov07_02220824
	.extern ov07_02220830
	.extern ov07_0222083C
	.extern ov07_02220848
	.extern ov07_02220854
	.extern ov07_02220860
	.extern ov07_0222086C
	.extern ov07_02220878
	.extern ov07_02220884
	.extern ov07_022208B4
	.extern ov07_022208F8
	.extern ov07_02220938
	.extern ov07_02220978
	.extern ov07_02220CAC
	.extern ov07_0222150C
	.extern ov07_02221540
	.extern ov07_02221550
	.extern ov07_022215B4
	.extern ov07_02221664
	.extern ov07_022216A8
	.extern ov07_02221734
	.extern ov07_022217A4
	.extern ov07_022217B0
	.extern ov07_022217B4
	.extern ov07_02221898
	.extern ov07_0222189C
	.extern ov07_022218A0
	.extern ov07_02221968
	.extern ov07_022219EC
	.extern ov07_02221A6C
	.extern ov07_02221AD4
	.extern ov07_02221AD8
	.extern ov07_02221ADC
	.extern ov07_02221AE0
	.extern ov07_02221AE4
	.extern ov07_02221BF0
	.extern ov07_02221C74
	.extern ov07_02221C8C
	.extern ov07_02221CB4
	.extern ov07_02221CE0
	.extern ov07_02221CEC
	.extern ov07_02221D10
	.extern ov07_02221D4C
	.extern ov07_02221D88
	.extern ov07_02221DC4
	.extern ov07_02221E24
	.extern ov07_02221E60
	.extern ov07_02221E9C
	.extern ov07_02221EC4
	.extern ov07_02221EEC
	.extern ov07_02221F04
	.extern ov07_02221F38
	.extern ov07_02221F80
	.extern ov07_02221FF0
	.extern ov07_02222004
	.extern ov07_0222202C
	.extern ov07_02222040
	.extern ov07_0222204C
	.extern ov07_0222207C
	.extern ov07_0222209C
	.extern ov07_022220B8
	.extern ov07_022220DC
	.extern ov07_022220FC
	.extern ov07_0222212C
	.extern ov07_02222180
	.extern ov07_02222218
	.extern ov07_02222240
	.extern ov07_02222268
	.extern ov07_022222B4
	.extern ov07_022222F0
	.extern ov07_02222314
	.extern ov07_02222338
	.extern ov07_02222384
	.extern ov07_022223CC
	.extern ov07_022223F0
	.extern ov07_02222418
	.extern ov07_02222440
	.extern ov07_02222468
	.extern ov07_02222494
	.extern ov07_022224AC
	.extern ov07_02222508
	.extern ov07_02222558
	.extern ov07_02222590
	.extern ov07_0222260C
	.extern ov07_02222644
	.extern ov07_02222674
	.extern ov07_022226C4
	.extern ov07_022226FC
	.extern ov07_02222748
	.extern ov07_02222768
	.extern ov07_02222788
	.extern ov07_022227A8
	.extern ov07_022227D8
	.extern ov07_0222283C
	.extern ov07_02222864
	.extern ov07_02222914
	.extern ov07_02222A44
	.extern ov07_02222A7C
	.extern ov07_02222AC4
	.extern ov07_02222AF4
	.extern ov07_02222AF8
	.extern ov07_02222B14
	.extern ov07_02222B30
	.extern ov07_02222B70
	.extern ov07_02222B94
	.extern ov07_02222BA4
	.extern ov07_02222BC8
	.extern ov07_02222BDC
	.extern ov07_02222BE4
	.extern ov07_02222C60
	.extern ov07_02222C84
	.extern ov07_02222C98
	.extern ov07_02222CAC
	.extern ov07_02222CC0
	.extern ov07_02222CCC
	.extern ov07_02222D3C
	.extern ov07_02222D60
	.extern ov07_02222D74
	.extern ov07_02222D88
	.extern ov07_02222D90
	.extern ov07_02222DC8
	.extern ov07_02222DE4
	.extern ov07_02222E0C
	.extern ov07_02222E48
	.extern ov07_02222E74
	.extern ov07_02222EE8
	.extern ov07_02222EF8
	.extern ov07_02222F10
	.extern ov07_02222F7C
	.extern ov07_02222FC4
	.extern ov07_02222FF4
	.extern ov07_02223038
	.extern ov07_0222304C
	.extern ov07_02223060
	.extern ov07_02223064
	.extern ov07_0222308C
	.extern ov07_022230B4
	.extern ov07_022230DC
	.extern ov07_02223104
	.extern ov07_02223128
	.extern ov07_0222314C
	.extern ov07_022231BC
	.extern ov07_02223224
	.extern ov07_02223240
	.extern ov07_02223350
	.extern ov07_022233D8
	.extern ov07_02223550
	.extern ov07_022236F0
	.extern ov07_02223864
	.extern ov07_02223A1C
	.extern ov07_02223A94
	.extern ov07_02223B70
	.extern ov07_02223BC0
	.extern ov07_02223CAC
	.extern ov07_02223CE4
	.extern ov07_02223D28
	.extern ov07_02223DC4
	.extern ov07_02223E08
	.extern ov07_02223E94
	.extern ov07_02223EB4
	.extern ov07_02223F5C
	.extern ov07_02224008
	.extern ov07_022241D8
	.extern ov07_02224348
	.extern ov07_0222446C
	.extern ov07_022245BC
	.extern ov07_022247C8
	.extern ov07_022247F4
	.extern ov07_022248A0
	.extern ov07_022248F4
	.extern ov07_02224948
	.extern ov07_02224998
	.extern ov07_02224A20
	.extern ov07_02224B14
	.extern ov07_02224BAC
	.extern ov07_02224CB8
	.extern ov07_02224E58
	.extern ov07_02224EF4
	.extern ov07_02225098
	.extern ov07_0222519C
	.extern ov07_022251CC
	.extern ov07_0222525C
	.extern ov07_022252F8
	.extern ov07_02225444
	.extern ov07_0222548C
	.extern ov07_02225504
	.extern ov07_0222554C
	.extern ov07_02225640
	.extern ov07_02225768
	.extern ov07_02225810
	.extern ov07_02225838
	.extern ov07_02225904
	.extern ov07_02225990
	.extern ov07_02225AE0
	.extern ov07_02225B38
	.extern ov07_02225BC4
	.extern ov07_02225C50
	.extern ov07_02225CC4
	.extern ov07_02225DC4
	.extern ov07_02225E9C
	.extern ov07_02225F18
	.extern ov07_02225F94
	.extern ov07_0222600C
	.extern ov07_022260DC
	.extern ov07_022260F8
	.extern ov07_022261B4
	.extern ov07_022262E8
	.extern ov07_0222647C
	.extern ov07_022264E0
	.extern ov07_02226628
	.extern ov07_02226698
	.extern ov07_02226804
	.extern ov07_02226838
	.extern ov07_02226880
	.extern ov07_022269D8
	.extern ov07_02226A68
	.extern ov07_02226A9C
	.extern ov07_02226B2C
	.extern ov07_02226C4C
	.extern ov07_02226C90
	.extern ov07_02226CB0
	.extern ov07_02226F04
	.extern ov07_02226F80
	.extern ov07_0222719C
	.extern ov07_022271DC
	.extern ov07_02227248
	.extern ov07_022272D0
	.extern ov07_02227300
	.extern ov07_022274F8
	.extern ov07_02227630
	.extern ov07_02227824
	.extern ov07_022278B0
	.extern ov07_02227AEC
	.extern ov07_02227B58
	.extern ov07_02227D1C
	.extern ov07_02227FA4
	.extern ov07_02228070
	.extern ov07_022280A8
	.extern ov07_02228174
	.extern ov07_022281C0
	.extern ov07_02228384
	.extern ov07_022283D8
	.extern ov07_022284D8
	.extern ov07_02228510
	.extern ov07_02228638
	.extern ov07_02228684
	.extern ov07_02228750
	.extern ov07_02228834
	.extern ov07_02228A50
	.extern ov07_02228B38
	.extern ov07_02228C8C
	.extern ov07_02228D08
	.extern ov07_02228D64
	.extern ov07_02228E70
	.extern ov07_02228F14
	.extern ov07_02228F88
	.extern ov07_02229024
	.extern ov07_022290BC
	.extern ov07_0222915C
	.extern ov07_02229220
	.extern ov07_0222928C
	.extern ov07_022292E4
	.extern ov07_02229480
	.extern ov07_02229630
	.extern ov07_022297B8
	.extern ov07_02229BFC
	.extern ov07_02229D40
	.extern ov07_02229E50
	.extern ov07_02229FC4
	.extern ov07_0222A1F0
	.extern ov07_0222A328
	.extern ov07_0222A4B4
	.extern ov07_0222A58C
	.extern ov07_0222A710
	.extern ov07_0222A838
	.extern ov07_0222A8D8
	.extern ov07_0222A8EC
	.extern ov07_0222A988
	.extern ov07_0222AA20
	.extern ov07_0222ACAC
	.extern ov07_0222AE14
	.extern ov07_0222AEA0
	.extern ov07_0222AFAC
	.extern ov07_0222B130
	.extern ov07_0222B2A8
	.extern ov07_0222B320
	.extern ov07_0222B4E0
	.extern ov07_0222B640
	.extern ov07_0222B898
	.extern ov07_0222B988
	.extern ov07_0222BADC
	.extern ov07_0222BAF4
	.extern ov07_0222BC7C
	.extern ov07_0222BD90
	.extern ov07_0222BE00
	.extern ov07_0222BF18
	.extern ov07_0222C074
	.extern ov07_0222C18C
	.extern ov07_0222C1FC
	.extern ov07_0222C208
	.extern ov07_0222C214
	.extern ov07_0222C2D4
	.extern ov07_0222C4C0
	.extern ov07_0222C610
	.extern ov07_0222C6A8
	.extern ov07_0222C780
	.extern ov07_0222C850
	.extern ov07_0222C870
	.extern ov07_0222C918
	.extern ov07_0222CA8C
	.extern ov07_0222CBFC
	.extern ov07_0222CCBC
	.extern ov07_0222CE10
	.extern ov07_0222CEC0
	.extern ov07_0222CFCC
	.extern ov07_0222D050
	.extern ov07_0222D148
	.extern ov07_0222D2B0
	.extern ov07_0222D3BC
	.extern ov07_0222D4F4
	.extern ov07_0222D56C
	.extern ov07_0222D578
	.extern ov07_0222D590
	.extern ov07_0222D60C
	.extern ov07_0222D6E0
	.extern ov07_0222D7A4
	.extern ov07_0222D974
	.extern ov07_0222DA60
	.extern ov07_0222DB14
	.extern ov07_0222DBD8
	.extern ov07_0222DC54
	.extern ov07_0222DCD8
	.extern ov07_0222DD20
	.extern ov07_0222DE30
	.extern ov07_0222DEEC
	.extern ov07_0222E020
	.extern ov07_0222E07C
	.extern ov07_0222E108
	.extern ov07_0222E2EC
	.extern ov07_0222E3A4
	.extern ov07_0222E3E8
	.extern ov07_0222E4F8
	.extern ov07_0222E54C
	.extern ov07_0222E5E8
	.extern ov07_0222E6A8
	.extern ov07_0222E6F4
	.extern ov07_0222E754
	.extern ov07_0222E7BC
	.extern ov07_0222E894
	.extern ov07_0222E90C
	.extern ov07_0222EA48
	.extern ov07_0222EAB4
	.extern ov07_0222EB00
	.extern ov07_0222EB6C
	.extern ov07_0222EBB0
	.extern ov07_0222ED7C
	.extern ov07_0222EDF0
	.extern ov07_0222EE14
	.extern ov07_0222EF0C
	.extern ov07_0222EF58
	.extern ov07_0222EF88
	.extern ov07_0222F0B0
	.extern ov07_0222F210
	.extern ov07_0222F378
	.extern ov07_0222F408
	.extern ov07_0222F434
	.extern ov07_0222F4D4
	.extern ov07_0222F600
	.extern ov07_0222F6C0
	.extern ov07_0222F764
	.extern ov07_0222F848
	.extern ov07_0222F9B8
	.extern ov07_0222FA08
	.extern ov07_0222FA64
	.extern ov07_0222FB24
	.extern ov07_0222FBEC
	.extern ov07_0222FC44
	.extern ov07_0222FD14
	.extern ov07_0222FE04
	.extern ov07_0222FEB0
	.extern ov07_0222FF04
	.extern ov07_0222FFD0
	.extern ov07_02230058
	.extern ov07_02230094
	.extern ov07_02230170
	.extern ov07_022301C4
	.extern ov07_022302A8
	.extern ov07_022303A4
	.extern ov07_0223049C
	.extern ov07_02230600
	.extern ov07_0223061C
	.extern ov07_022306D8
	.extern ov07_02230714
	.extern ov07_022308E4
	.extern ov07_02230960
	.extern ov07_02230994
	.extern ov07_02230A70
	.extern ov07_02230BA0
	.extern ov07_02230BF8
	.extern ov07_02230D1C
	.extern ov07_02230D74
	.extern ov07_02230E20
	.extern ov07_02230F0C
	.extern ov07_02230F6C
	.extern ov07_02230F98
	.extern ov07_02230FE8
	.extern ov07_022310E0
	.extern ov07_022311B0
	.extern ov07_02231204
	.extern ov07_02231234
	.extern ov07_022312A8
	.extern ov07_02231300
	.extern ov07_02231378
	.extern ov07_02231468
	.extern ov07_02231514
	.extern ov07_0223151C
	.extern ov07_02231524
	.extern ov07_022315C0
	.extern ov07_0223172C
	.extern ov07_0223174C
	.extern ov07_02231874
	.extern ov07_022318A0
	.extern ov07_022318CC
	.extern ov07_022318F8
	.extern ov07_02231924
	.extern ov07_0223192C
	.extern ov07_02231958
	.extern ov07_0223197C
	.extern ov07_022319E0
	.extern ov07_02231A20
	.extern ov07_02231A50
	.extern ov07_02231A70
	.extern ov07_02231AD0
	.extern ov07_02231AE0
	.extern ov07_02231AF0
	.extern ov07_02231B00
	.extern ov07_02231B10
	.extern ov07_02231B20
	.extern ov07_02231B30
	.extern ov07_02231B40
	.extern ov07_02231B50
	.extern ov07_02231B60
	.extern ov07_02231B70
	.extern ov07_02231B80
	.extern ov07_02231B90
	.extern ov07_02231BC0
	.extern ov07_02231BF0
	.extern ov07_02231C20
	.extern ov07_02231C50
	.extern ov07_02231C80
	.extern ov07_02231CB0
	.extern ov07_02231CE0
	.extern ov07_02231D10
	.extern ov07_02231D40
	.extern ov07_02231D70
	.extern ov07_02231DA0
	.extern ov07_02231DD0
	.extern ov07_02231DE0
	.extern ov07_02231DE8
	.extern ov07_02231E08
	.extern ov07_02231E44
	.extern ov07_02231E74
	.extern ov07_02231EC0
	.extern ov07_02231FA0
	.extern ov07_02231FD8
	.extern ov07_02231FE4
	.extern ov07_02232020
	.extern ov07_022323FC
	.extern ov07_022324D8
	.extern ov07_02232508
	.extern ov07_0223251C
	.extern ov07_02232540
	.extern ov07_02232580
	.extern ov07_022325BC
	.extern ov07_022325F4
	.extern ov07_02232608
	.extern ov07_0223261C
	.extern ov07_02232630
	.extern ov07_02232644
	.extern ov07_02232658
	.extern ov07_02232670
	.extern ov07_02232694
	.extern ov07_022326C0
	.extern ov07_02232730
	.extern ov07_022329B0
	.extern ov07_02232A04
	.extern ov07_02232A14
	.extern ov07_02232A44
	.extern ov07_02232A54
	.extern ov07_02232A5C
	.extern ov07_02232AB8
	.extern ov07_02232AC0
	.extern ov07_02232AFC
	.extern ov07_02232BB0
	.extern ov07_02232C28
	.extern ov07_02232C64
	.extern ov07_02232CD8
	.extern ov07_02232D20
	.extern ov07_02232D80
	.extern ov07_02232DF4
	.extern ov07_02232E10
	.extern ov07_02232E18
	.extern ov07_02232E40
	.extern ov07_02232E68
	.extern ov07_02232E90
	.extern ov07_02232EB8
	.extern ov07_02232EE0
	.extern ov07_02232F08
	.extern ov07_02232F30
	.extern UnkBallData_SetBallAnimation
	.extern ov07_02232F60
	.extern ov07_02232F74
	.extern ov07_02232F80
	.extern ov07_02232F84
	.extern ov07_02232F9C
	.extern ov07_02232FA8
	.extern ov07_02233088
	.extern ov07_0223308C
	.extern ov07_022330E0
	.extern ov07_022330E4
	.extern ov07_022330F0
	.extern ov07_022330FC
	.extern ov07_022331A4
	.extern ov07_022331B0
	.extern ov07_02233224
	.extern ov07_02233228
	.extern ov07_022332C4
	.extern ov07_022332C8
	.extern ov07_022332E8
	.extern ov07_022332F4
	.extern ov07_0223338C
	.extern ov07_02233390
	.extern ov07_0223339C
	.extern ov07_022333A8
	.extern ov07_02233498
	.extern ov07_0223349C
	.extern ov07_02233500
	.extern ov07_0223350C
	.extern ov07_02233590
	.extern ov07_0223359C
	.extern ov07_022335A0
	.extern ov07_022335B4
	.extern ov07_02233874
	.extern ov07_02233B04
	.extern ov07_02233C98


	thumb_func_start ov07_0221C634
ov07_0221C634: ; 0x0221C634
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldrb r1, [r5, #4]
	add r0, r1, #1
	strb r0, [r5, #4]
	ldrb r0, [r5, #3]
	cmp r1, r0
	bhs _0221C648
	mov r0, #1
	pop {r3, r4, r5, pc}
_0221C648:
	mov r0, #0
	strb r0, [r5, #4]
	ldrb r0, [r5, #0x18]
	mov r4, #1
	sub r0, r0, #1
	strb r0, [r5, #0x18]
	ldrh r0, [r5, #0x1a]
	bl PlaySE
	ldrh r0, [r5, #0x1a]
	ldr r1, _0221C670 ; =0x0000FFFF
	ldr r2, [r5, #0x14]
	bl sub_020061B4
	ldrb r0, [r5, #0x18]
	cmp r0, #0
	bne _0221C66C
	mov r4, #0
_0221C66C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221C670: .word 0x0000FFFF
	thumb_func_end ov07_0221C634

	thumb_func_start ov07_0221C674
ov07_0221C674: ; 0x0221C674
	push {r4, lr}
	add r4, r0, #0
	ldrb r2, [r4, #3]
	mov r0, #1
	sub r1, r2, #1
	strb r1, [r4, #3]
	cmp r2, #0
	bne _0221C696
	ldrh r0, [r4, #0x1a]
	bl PlaySE
	ldrh r0, [r4, #0x1a]
	ldr r1, _0221C698 ; =0x0000FFFF
	ldr r2, [r4, #0x14]
	bl sub_020061B4
	mov r0, #0
_0221C696:
	pop {r4, pc}
	.balign 4, 0
_0221C698: .word 0x0000FFFF
	thumb_func_end ov07_0221C674

	thumb_func_start ov07_0221C69C
ov07_0221C69C: ; 0x0221C69C
	push {r3, lr}
	ldr r0, _0221C6B0 ; =0x04000050
	mov r3, #8
	mov r1, #0
	mov r2, #0x3f
	str r3, [sp]
	bl G2x_SetBlendAlpha_
	pop {r3, pc}
	nop
_0221C6B0: .word 0x04000050
	thumb_func_end ov07_0221C69C

	thumb_func_start ov07_0221C6B4
ov07_0221C6B4: ; 0x0221C6B4
	push {r3, r4}
	add r2, r0, #0
	mov r1, #1
	add r2, #0x8d
	strb r1, [r2]
	ldr r3, _0221C6E8 ; =gSystem
	lsl r2, r1, #9
	ldr r4, [r3, #0x44]
	tst r2, r4
	beq _0221C6E4
	add r2, r1, #0
	add r2, #0xff
	tst r2, r4
	beq _0221C6E4
	ldr r2, [r3, #0x48]
	lsl r1, r1, #0xa
	tst r1, r2
	beq _0221C6E4
	ldr r1, [r0, #0x18]
	add r1, r1, #4
	str r1, [r0, #0x18]
	mov r1, #0
	add r0, #0x8d
	strb r1, [r0]
_0221C6E4:
	pop {r3, r4}
	bx lr
	.balign 4, 0
_0221C6E8: .word gSystem
	thumb_func_end ov07_0221C6B4

	thumb_func_start ov07_0221C6EC
ov07_0221C6EC: ; 0x0221C6EC
	ldr r1, [r0, #0x18]
	add r1, r1, #4
	str r1, [r0, #0x18]
	ldr r2, [r1]
	add r1, r0, #0
	add r1, #0x8d
	strb r2, [r1]
	ldr r1, [r0, #0x18]
	add r1, r1, #4
	str r1, [r0, #0x18]
	ldr r1, _0221C708 ; =ov07_0221BE20
	add r0, #0xbc
	str r1, [r0]
	bx lr
	.balign 4, 0
_0221C708: .word ov07_0221BE20
	thumb_func_end ov07_0221C6EC

	thumb_func_start ov07_0221C70C
ov07_0221C70C: ; 0x0221C70C
	add r1, r0, #0
	add r1, #0x8e
	ldrh r1, [r1]
	cmp r1, #0
	bne _0221C724
	ldr r1, [r0, #0x18]
	add r1, r1, #4
	str r1, [r0, #0x18]
	mov r1, #0
	add r0, #0x8d
	strb r1, [r0]
	bx lr
_0221C724:
	mov r1, #1
	add r0, #0x8d
	strb r1, [r0]
	bx lr
	thumb_func_end ov07_0221C70C

	thumb_func_start ov07_0221C72C
ov07_0221C72C: ; 0x0221C72C
	ldr r1, [r0, #0x18]
	add r1, r1, #4
	str r1, [r0, #0x18]
	ldr r3, [r1]
	add r1, r1, #4
	str r1, [r0, #0x18]
	ldr r2, [r1]
	add r1, r1, #4
	str r1, [r0, #0x18]
	cmp r3, #0xa
	bhs _0221C74A
	lsl r1, r3, #2
	add r0, r0, r1
	add r0, #0x94
	str r2, [r0]
_0221C74A:
	bx lr
	thumb_func_end ov07_0221C72C

	thumb_func_start ov07_0221C74C
ov07_0221C74C: ; 0x0221C74C
	ldr r1, [r0, #0x18]
	mov r3, #0
	add r1, r1, #4
	str r1, [r0, #0x18]
	add r2, r3, #0
_0221C756:
	add r1, r0, #0
	add r1, #0x94
	add r3, r3, #1
	add r0, r0, #4
	str r2, [r1]
	cmp r3, #0xa
	blt _0221C756
	bx lr
	.balign 4, 0
	thumb_func_end ov07_0221C74C

	thumb_func_start ov07_0221C768
ov07_0221C768: ; 0x0221C768
	cmp r1, #5
	bhi _0221C7B4
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0221C778: ; jump table
	.short _0221C784 - _0221C778 - 2 ; case 0
	.short _0221C78C - _0221C778 - 2 ; case 1
	.short _0221C794 - _0221C778 - 2 ; case 2
	.short _0221C79C - _0221C778 - 2 ; case 3
	.short _0221C7A4 - _0221C778 - 2 ; case 4
	.short _0221C7AC - _0221C778 - 2 ; case 5
_0221C784:
	add r0, #0xc0
	ldr r0, [r0]
	ldr r0, [r0, #4]
	bx lr
_0221C78C:
	add r0, #0xc0
	ldr r0, [r0]
	ldrh r0, [r0, #8]
	bx lr
_0221C794:
	add r0, #0xc0
	ldr r0, [r0]
	ldrh r0, [r0, #0xa]
	bx lr
_0221C79C:
	add r0, #0xc0
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	bx lr
_0221C7A4:
	add r0, #0xc0
	ldr r0, [r0]
	ldrh r0, [r0, #0x10]
	bx lr
_0221C7AC:
	add r0, #0xc0
	ldr r0, [r0]
	ldrh r0, [r0, #0x12]
	bx lr
_0221C7B4:
	mov r0, #0
	bx lr
	thumb_func_end ov07_0221C768

	thumb_func_start ov07_0221C7B8
ov07_0221C7B8: ; 0x0221C7B8
	push {r3, r4, r5, r6, r7, lr}
	add r3, r0, #0
	mov r0, #0
	str r2, [sp]
	add r6, r0, #0
	add r5, r1, #0
	mov ip, r0
	add r7, r1, #0
_0221C7C8:
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r2, r2, r6
	add r2, #0xb0
	ldr r2, [r2]
	str r2, [r5, #8]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r2, r2, r6
	add r2, #0xc4
	ldr r2, [r2]
	str r2, [r5, #0x18]
	add r2, r3, #0
	add r2, #0xc0
	ldr r4, [r2]
	mov r2, ip
	add r2, r4, r2
	add r2, #0xd8
	ldrh r2, [r2]
	strh r2, [r7, #0x28]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r7, r7, #2
	add r2, r2, r0
	add r2, #0xe0
	ldrb r4, [r2]
	add r2, r1, r0
	add r2, #0x30
	strb r4, [r2]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r2, r2, r0
	add r2, #0xe4
	ldrb r4, [r2]
	add r2, r1, r0
	add r2, #0x34
	strb r4, [r2]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r2, r2, r0
	add r2, #0xe8
	ldrb r4, [r2]
	add r2, r1, r0
	add r2, #0x38
	strb r4, [r2]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r2, r2, r6
	add r2, #0xec
	ldr r2, [r2]
	add r6, r6, #4
	str r2, [r5, #0x3c]
	add r2, r3, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r5, r5, #4
	add r2, r2, r0
	add r2, #0xc0
	ldrb r4, [r2]
	add r2, r1, r0
	add r2, #0x4c
	strb r4, [r2]
	mov r2, ip
	add r2, r2, #2
	add r0, r0, #1
	mov ip, r2
	cmp r0, #4
	blt _0221C7C8
	ldr r0, [sp]
	cmp r0, #3
	bhi _0221C8C6
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0221C86E: ; jump table
	.short _0221C876 - _0221C86E - 2 ; case 0
	.short _0221C88A - _0221C86E - 2 ; case 1
	.short _0221C89E - _0221C86E - 2 ; case 2
	.short _0221C8B2 - _0221C86E - 2 ; case 3
_0221C876:
	add r0, r3, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r3, #0xc0
	ldrh r0, [r0, #0x14]
	str r0, [r1]
	ldr r0, [r3]
	ldrh r0, [r0, #0x16]
	str r0, [r1, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0221C88A:
	add r0, r3, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r3, #0xc0
	ldrh r0, [r0, #0x14]
	str r0, [r1]
	ldr r0, [r3]
	ldrh r0, [r0, #0x14]
	str r0, [r1, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0221C89E:
	add r0, r3, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r3, #0xc0
	ldrh r0, [r0, #0x14]
	str r0, [r1]
	ldr r0, [r3]
	ldrh r0, [r0, #0x14]
	str r0, [r1, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0221C8B2:
	add r0, r3, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r3, #0xc0
	ldrh r0, [r0, #0x14]
	str r0, [r1]
	ldr r0, [r3]
	ldrh r0, [r0, #0x14]
	str r0, [r1, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0221C8C6:
	bl GF_AssertFail
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov07_0221C7B8

	thumb_func_start ov07_0221C8CC
ov07_0221C8CC: ; 0x0221C8CC
	push {r3, r4, r5, lr}
	sub sp, #0x50
	add r5, r0, #0
	ldr r1, [r5, #0x18]
	add r1, r1, #4
	str r1, [r5, #0x18]
	ldr r4, [r1]
	add r1, r1, #4
	str r1, [r5, #0x18]
	add r1, sp, #0
	add r2, r4, #0
	bl ov07_0221C7B8
	cmp r4, #2
	add r0, sp, #0
	bne _0221C8F6
	ldr r1, [r5]
	bl ov07_02234A20
	add sp, #0x50
	pop {r3, r4, r5, pc}
_0221C8F6:
	cmp r4, #3
	bne _0221C904
	ldr r1, [r5]
	bl ov07_0223475C
	add sp, #0x50
	pop {r3, r4, r5, pc}
_0221C904:
	ldr r1, [r5]
	bl ov07_0223474C
	add sp, #0x50
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_0221C8CC

	thumb_func_start ov07_0221C910
ov07_0221C910: ; 0x0221C910
	push {r4, lr}
	sub sp, #0x50
	add r4, r0, #0
	ldr r1, [r4, #0x18]
	add r1, r1, #4
	str r1, [r4, #0x18]
	ldr r2, [r1]
	add r1, r1, #4
	str r1, [r4, #0x18]
	add r1, sp, #0
	bl ov07_0221C7B8
	ldr r1, [r4]
	add r0, sp, #0
	bl ov07_0223494C
	add sp, #0x50
	pop {r4, pc}
	thumb_func_end ov07_0221C910

	thumb_func_start ov07_0221C934
ov07_0221C934: ; 0x0221C934
	ldr r1, [r0, #0x18]
	mov r2, #0
	add r1, r1, #4
	str r1, [r0, #0x18]
	add r3, r0, #0
_0221C93E:
	ldr r1, [r3, #0x30]
	cmp r1, #1
	beq _0221C96A
	mov r1, #0xc
	mul r1, r2
	mov r2, #1
	add r1, r0, r1
	str r2, [r1, #0x30]
	add r2, r1, #0
	mov r3, #0
	add r2, #0x2c
	strb r3, [r2]
	ldr r2, [r0, #0x18]
	ldr r3, [r2]
	add r2, r1, #0
	add r2, #0x2d
	strb r3, [r2]
	ldr r2, [r0, #0x18]
	add r2, r2, #4
	str r2, [r0, #0x18]
	str r2, [r1, #0x28]
	bx lr
_0221C96A:
	add r2, r2, #1
	add r3, #0xc
	cmp r2, #3
	blt _0221C93E
	bx lr
	thumb_func_end ov07_0221C934

	thumb_func_start ov07_0221C974
ov07_0221C974: ; 0x0221C974
	push {r3, r4}
	ldr r1, [r0, #0x18]
	add r3, r0, #0
	add r1, r1, #4
	add r3, #0x18
	str r1, [r0, #0x18]
	mov r2, #2
_0221C982:
	ldr r1, [r3, #0x30]
	cmp r1, #0
	beq _0221C9B6
	add r4, r2, #0
	mov r1, #0xc
	add r2, r0, #0
	mul r4, r1
	add r2, #0x2c
	ldrb r1, [r2, r4]
	add r1, r1, #1
	strb r1, [r2, r4]
	ldrb r3, [r2, r4]
	add r2, r0, r4
	add r1, r2, #0
	add r1, #0x2d
	ldrb r1, [r1]
	cmp r3, r1
	bne _0221C9AE
	mov r0, #0
	str r0, [r2, #0x30]
	pop {r3, r4}
	bx lr
_0221C9AE:
	ldr r1, [r2, #0x28]
	str r1, [r0, #0x18]
	pop {r3, r4}
	bx lr
_0221C9B6:
	sub r3, #0xc
	sub r2, r2, #1
	bpl _0221C982
	pop {r3, r4}
	bx lr
	thumb_func_end ov07_0221C974

	thumb_func_start ov07_0221C9C0
ov07_0221C9C0: ; 0x0221C9C0
	push {r3, r4, r5, r6, r7, lr}
	ldr r1, _0221CBAC ; =0x0000017E
	add r5, r0, #0
	ldrb r0, [r5, r1]
	mov r6, #0
	cmp r0, #1
	bhs _0221C9DE
	add r0, r5, #0
	mov r2, #1
	add r0, #0x8d
	strb r2, [r0]
	ldrb r0, [r5, r1]
	add r0, r0, #1
	strb r0, [r5, r1]
	pop {r3, r4, r5, r6, r7, pc}
_0221C9DE:
	add r7, r6, #0
	add r4, r6, #0
_0221C9E2:
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	cmp r0, #0
	beq _0221C9F6
	bl sub_020154B0
	add r6, r6, r0
_0221C9F6:
	add r7, r7, #1
	add r4, r4, #4
	cmp r7, #0x10
	blt _0221C9E2
	cmp r6, #0
	bne _0221CA16
	add r0, r5, #0
	add r0, #0x8e
	ldrh r0, [r0]
	cmp r0, #0
	bne _0221CA16
	add r0, r5, #0
	add r0, #0x90
	ldrh r0, [r0]
	cmp r0, #0
	beq _0221CA26
_0221CA16:
	add r0, r5, #0
	mov r1, #1
	add r0, #0x8d
	strb r1, [r0]
	ldr r0, _0221CBB0 ; =0x0000017D
	mov r1, #0
	strb r1, [r5, r0]
	pop {r3, r4, r5, r6, r7, pc}
_0221CA26:
	bl GF_IsAnySEPlaying
	cmp r0, #0
	beq _0221CA50
	ldr r0, _0221CBB0 ; =0x0000017D
	ldrb r1, [r5, r0]
	add r1, r1, #1
	strb r1, [r5, r0]
	ldrb r1, [r5, r0]
	cmp r1, #0x5a
	bls _0221CA48
	mov r1, #0
	strb r1, [r5, r0]
	add r0, r5, #0
	add r0, #0x8d
	strb r1, [r0]
	b _0221CA50
_0221CA48:
	mov r0, #1
	add r5, #0x8d
	strb r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
_0221CA50:
	ldr r0, _0221CBB0 ; =0x0000017D
	mov r1, #0
	strb r1, [r5, r0]
	add r0, r0, #1
	strb r1, [r5, r0]
	add r0, r5, #0
	add r2, r1, #0
_0221CA5E:
	add r1, r1, #1
	str r2, [r0, #0x1c]
	add r0, r0, #4
	cmp r1, #3
	blt _0221CA5E
	add r1, r5, #0
	mov r4, #0
_0221CA6C:
	add r0, r1, #0
	str r4, [r1, #0x28]
	add r0, #0x2c
	strb r4, [r0]
	add r0, r1, #0
	add r0, #0x2d
	strb r4, [r0]
	str r4, [r1, #0x30]
	add r2, r2, #1
	add r1, #0xc
	cmp r2, #3
	blt _0221CA6C
	add r7, r5, #0
	mov r6, #0
_0221CA88:
	add r0, r7, #0
	add r0, #0xcc
	ldr r1, [r0]
	cmp r1, #0
	beq _0221CAA0
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r0, #0xac
	ldr r0, [r0]
	bl SpriteSystem_FreeResourcesAndManager
_0221CAA0:
	add r0, r7, #0
	add r0, #0xcc
	add r4, r4, #1
	add r7, r7, #4
	str r6, [r0]
	cmp r4, #4
	blt _0221CA88
_0221CAAE:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221D55C
	add r6, r6, #1
	cmp r6, #5
	blt _0221CAAE
	mov r6, #0
	add r4, r6, #0
	add r7, r6, #0
_0221CAC2:
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	cmp r0, #0
	beq _0221CADE
	bl ov07_0221FF2C
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r0, r0, r4
	str r7, [r0, #0x1c]
_0221CADE:
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #0x10
	blt _0221CAC2
	mov r0, #6
	lsl r0, r0, #6
	add r0, r5, r0
	mov r1, #5
	bl ov07_0221DD14
	bl BattleSystem_SetDefaultBlend
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221FAEC
	add r4, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	add r3, r0, #0
	lsl r0, r4, #0x18
	mov r1, #1
	lsr r0, r0, #0x18
	lsl r1, r1, #0xe
	mov r2, #0
	bl BG_ClearCharDataRange
	add r0, r5, #0
	bl ov07_0221C4A0
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221FAEC
	add r1, r0, #0
	lsl r1, r1, #0x18
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl BgClearTilemapBufferAndCommit
	mov r0, #2
	mov r1, #1
	bl ToggleBgLayer
	mov r1, #0x6b
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	mov r0, #0
	bl SetBgPriority
	ldr r1, _0221CBB4 ; =0x000001AD
	mov r0, #1
	ldrb r1, [r5, r1]
	bl SetBgPriority
	ldr r1, _0221CBB8 ; =0x000001AE
	mov r0, #2
	ldrb r1, [r5, r1]
	bl SetBgPriority
	ldr r1, _0221CBBC ; =0x000001AF
	mov r0, #3
	ldrb r1, [r5, r1]
	bl SetBgPriority
	add r0, r5, #0
	add r0, #0xc4
	mov r2, #0
	ldr r0, [r0]
	mov r1, #2
	add r3, r2, #0
	bl BgSetPosTextAndCommit
	add r0, r5, #0
	add r0, #0xc4
	ldr r0, [r0]
	mov r1, #2
	mov r2, #3
	mov r3, #0
	bl BgSetPosTextAndCommit
	add r0, r5, #0
	add r0, #0xc4
	mov r2, #0
	ldr r0, [r0]
	mov r1, #3
	add r3, r2, #0
	bl BgSetPosTextAndCommit
	add r0, r5, #0
	add r0, #0xc4
	mov r1, #3
	ldr r0, [r0]
	add r2, r1, #0
	mov r3, #0
	bl BgSetPosTextAndCommit
	mov r0, #0
	str r0, [r5, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221CBAC: .word 0x0000017E
_0221CBB0: .word 0x0000017D
_0221CBB4: .word 0x000001AD
_0221CBB8: .word 0x000001AE
_0221CBBC: .word 0x000001AF
	thumb_func_end ov07_0221C9C0

	thumb_func_start ov07_0221CBC0
ov07_0221CBC0: ; 0x0221CBC0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r4, [r0]
	add r1, r0, #4
	str r1, [r5, #0x18]
	ldr r0, [r1]
	str r0, [sp]
	add r0, r1, #4
	str r0, [r5, #0x18]
	ldr r7, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	str r4, [r0, #0x18]
	add r0, r5, r4
	add r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0221CC1E
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	add r1, sp, #4
	bl sub_020154D4
	mov r0, #0
	ldr r1, [sp, #8]
	mvn r0, r0
	mul r0, r1
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r1, sp, #4
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	bl sub_020154E4
_0221CC1E:
	add r0, r5, #0
	add r0, #0xc0
	add r1, r5, r4
	add r1, #0x6c
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldrb r1, [r1]
	ldr r0, [r0, #0x1c]
	bl sub_02015528
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	ldr r1, [sp]
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	add r2, r7, #0
	add r3, r5, #0
	bl ov07_0221FF18
	add r5, #0xc0
	ldr r1, [r5]
	str r0, [r1, #0x5c]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_0221CBC0

	thumb_func_start ov07_0221CC54
ov07_0221CC54: ; 0x0221CC54
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r4, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r7, [r0]
	add r1, r0, #4
	str r1, [r5, #0x18]
	ldr r0, [r1]
	add r1, r1, #4
	str r1, [r5, #0x18]
	str r0, [sp, #4]
	ldr r0, [r1]
	str r0, [sp]
	add r0, r1, #4
	str r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	str r4, [r0, #0x18]
	add r0, r5, r4
	add r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0221CCBA
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	add r1, sp, #8
	bl sub_020154D4
	mov r0, #0
	ldr r1, [sp, #0xc]
	mvn r0, r0
	mul r0, r1
	str r0, [sp, #0xc]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r1, sp, #8
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	bl sub_020154E4
_0221CCBA:
	add r0, r5, #0
	add r0, #0xc0
	add r1, r5, r4
	add r1, #0x6c
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldrb r1, [r1]
	ldr r0, [r0, #0x1c]
	bl sub_02015528
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	ldr r1, [sp, #4]
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	ldr r2, [sp]
	add r3, r5, #0
	bl ov07_0221FF18
	add r5, #0xc0
	ldr r2, [r5]
	lsl r1, r7, #2
	add r1, r2, r1
	str r0, [r1, #0x5c]
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_0221CC54

	thumb_func_start ov07_0221CCF4
ov07_0221CCF4: ; 0x0221CCF4
	push {r4, r5, r6, lr}
	sub sp, #0x90
	ldr r5, _0221CD48 ; =ov07_02234CC8
	add r4, r0, #0
	add r3, sp, #0
	mov r2, #0x12
_0221CD00:
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0221CD00
	add r0, r4, #0
	bl ov07_0221C468
	add r5, r0, #0
	add r0, r4, #0
	bl ov07_0221C470
	add r6, r0, #0
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_02231924
	add r5, r0, #0
	add r0, r4, #0
	add r1, r6, #0
	bl ov07_02231924
	mov r1, #0x18
	add r3, r5, #0
	mul r3, r1
	add r2, sp, #0
	lsl r1, r0, #2
	add r0, r2, r3
	ldr r4, [r1, r0]
	cmp r4, #0xff
	bne _0221CD40
	bl GF_AssertFail
_0221CD40:
	sub r0, r4, #1
	add sp, #0x90
	pop {r4, r5, r6, pc}
	nop
_0221CD48: .word ov07_02234CC8
	thumb_func_end ov07_0221CCF4

	thumb_func_start ov07_0221CD4C
ov07_0221CD4C: ; 0x0221CD4C
	push {r4, r5, r6, lr}
	sub sp, #0x90
	ldr r5, _0221CDA0 ; =ov07_02234D58
	add r4, r0, #0
	add r3, sp, #0
	mov r2, #0x12
_0221CD58:
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0221CD58
	add r0, r4, #0
	bl ov07_0221C468
	add r5, r0, #0
	add r0, r4, #0
	bl ov07_0221C470
	add r6, r0, #0
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_02231924
	add r5, r0, #0
	add r0, r4, #0
	add r1, r6, #0
	bl ov07_02231924
	mov r1, #0x18
	add r3, r5, #0
	mul r3, r1
	add r2, sp, #0
	lsl r1, r0, #2
	add r0, r2, r3
	ldr r4, [r1, r0]
	cmp r4, #0xff
	bne _0221CD98
	bl GF_AssertFail
_0221CD98:
	add r0, r4, #0
	add sp, #0x90
	pop {r4, r5, r6, pc}
	nop
_0221CDA0: .word ov07_02234D58
	thumb_func_end ov07_0221CD4C

	thumb_func_start ov07_0221CDA4
ov07_0221CDA4: ; 0x0221CDA4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	ldr r1, [r5, #0x18]
	add r0, #0x18
	add r1, r1, #4
	str r1, [r5, #0x18]
	ldr r4, [r1]
	add r1, r1, #4
	str r1, [r5, #0x18]
	mov r1, #0
	add r2, sp, #0x10
_0221CDBC:
	ldr r3, [r5, #0x18]
	add r1, r1, #1
	ldr r3, [r3]
	str r3, [r2]
	ldr r3, [r0]
	add r2, r2, #4
	add r3, r3, #4
	str r3, [r0]
	cmp r1, #6
	blt _0221CDBC
	ldr r1, [r5, #0x18]
	ldr r1, [r1]
	str r1, [sp]
	ldr r1, [r0]
	add r1, r1, #4
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	str r4, [r0, #0x18]
	add r0, r5, r4
	add r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0221CE1A
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	add r1, sp, #4
	bl sub_020154D4
	mov r0, #0
	ldr r1, [sp, #8]
	mvn r0, r0
	mul r0, r1
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r1, sp, #4
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	bl sub_020154E4
_0221CE1A:
	add r0, r5, #0
	bl ov07_0221CCF4
	add r7, r0, #0
	add r0, r5, #0
	add r0, #0xc0
	add r1, r5, r4
	add r1, #0x6c
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldrb r1, [r1]
	ldr r0, [r0, #0x1c]
	bl sub_02015528
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r2, r7, #2
	add r0, r0, r6
	add r1, sp, #0x10
	ldr r1, [r1, r2]
	ldr r0, [r0, #0x1c]
	ldr r2, [sp]
	add r3, r5, #0
	bl ov07_0221FF18
	add r5, #0xc0
	ldr r1, [r5]
	str r0, [r1, #0x5c]
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_0221CDA4

	thumb_func_start ov07_0221CE5C
ov07_0221CE5C: ; 0x0221CE5C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r5, r0, #0
	ldr r1, [r5, #0x18]
	add r0, #0x18
	add r1, r1, #4
	str r1, [r5, #0x18]
	ldr r4, [r1]
	add r1, r1, #4
	str r1, [r5, #0x18]
	mov r1, #0
	add r2, sp, #0x10
_0221CE74:
	ldr r3, [r5, #0x18]
	add r1, r1, #1
	ldr r3, [r3]
	str r3, [r2]
	ldr r3, [r0]
	add r2, r2, #4
	add r3, r3, #4
	str r3, [r0]
	cmp r1, #4
	blt _0221CE74
	ldr r1, [r5, #0x18]
	ldr r1, [r1]
	str r1, [sp]
	ldr r1, [r0]
	add r1, r1, #4
	str r1, [r0]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	str r4, [r0, #0x18]
	add r0, r5, r4
	add r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0221CED2
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	add r1, sp, #4
	bl sub_020154D4
	mov r0, #0
	ldr r1, [sp, #8]
	mvn r0, r0
	mul r0, r1
	str r0, [sp, #8]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r1, sp, #4
	add r0, r0, r6
	ldr r0, [r0, #0x1c]
	bl sub_020154E4
_0221CED2:
	add r0, r5, #0
	bl ov07_0221CD4C
	add r7, r0, #0
	add r0, r5, #0
	add r0, #0xc0
	add r1, r5, r4
	add r1, #0x6c
	ldr r0, [r0]
	lsl r6, r4, #2
	add r0, r0, r6
	ldrb r1, [r1]
	ldr r0, [r0, #0x1c]
	bl sub_02015528
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r2, r7, #2
	add r0, r0, r6
	add r1, sp, #0x10
	ldr r1, [r1, r2]
	ldr r0, [r0, #0x1c]
	ldr r2, [sp]
	add r3, r5, #0
	bl ov07_0221FF18
	add r5, #0xc0
	ldr r1, [r5]
	str r0, [r1, #0x5c]
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_0221CE5C

	thumb_func_start ov07_0221CF14
ov07_0221CF14: ; 0x0221CF14
	push {r3, r4, r5, r6, r7, lr}
	mov r6, #0
	add r5, r0, #0
	add r7, r6, #0
	add r4, r6, #0
_0221CF1E:
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	cmp r0, #0
	beq _0221CF32
	bl sub_020154B0
	add r6, r6, r0
_0221CF32:
	add r7, r7, #1
	add r4, r4, #4
	cmp r7, #0x10
	blt _0221CF1E
	cmp r6, #0
	bne _0221CF4C
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	mov r0, #0
	add r5, #0x8d
	strb r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
_0221CF4C:
	mov r0, #1
	add r5, #0x8d
	strb r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov07_0221CF14

	thumb_func_start ov07_0221CF54
ov07_0221CF54: ; 0x0221CF54
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r1, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r4, r1, #2
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	cmp r0, #0
	beq _0221CF78
	bl GF_AssertFail
_0221CF78:
	ldr r0, [r5, #0x18]
	mov r2, #0
	ldr r1, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r0, [r5]
	bl ov07_0221FE88
	add r1, r5, #0
	add r1, #0xc0
	ldr r1, [r1]
	add r1, r1, r4
	str r0, [r1, #0x1c]
	add r0, r5, #0
	mov r1, #2
	add r0, #0x8d
	strb r1, [r0]
	ldr r0, _0221CFA4 ; =ov07_0221BE20
	add r5, #0xbc
	str r0, [r5]
	pop {r3, r4, r5, pc}
	nop
_0221CFA4: .word ov07_0221BE20
	thumb_func_end ov07_0221CF54

	thumb_func_start ov07_0221CFA8
ov07_0221CFA8: ; 0x0221CFA8
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r1, [r0]
	add r0, r0, #4
	add r0, r0, #4
	str r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r4, r1, #2
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	cmp r0, #0
	beq _0221CFCE
	bl GF_AssertFail
_0221CFCE:
	ldr r0, [r5, #0x18]
	mov r1, #0x60
	ldr r2, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r0, [r5]
	mov r3, #0
	bl ov07_0221FEB0
	add r1, r5, #0
	add r1, #0xc0
	ldr r1, [r1]
	add r1, r1, r4
	str r0, [r1, #0x1c]
	add r0, r5, #0
	mov r1, #2
	add r0, #0x8d
	strb r1, [r0]
	ldr r0, _0221CFFC ; =ov07_0221BE20
	add r5, #0xbc
	str r0, [r5]
	pop {r3, r4, r5, pc}
	nop
_0221CFFC: .word ov07_0221BE20
	thumb_func_end ov07_0221CFA8

	thumb_func_start ov07_0221D000
ov07_0221D000: ; 0x0221D000
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	ldr r1, [r0]
	add r0, r0, #4
	str r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0xc0
	ldr r0, [r0]
	lsl r4, r1, #2
	add r0, r0, r4
	ldr r0, [r0, #0x1c]
	bl ov07_0221FF2C
	add r5, #0xc0
	ldr r0, [r5]
	mov r1, #0
	add r0, r0, r4
	str r1, [r0, #0x1c]
	pop {r3, r4, r5, pc}
	thumb_func_end ov07_0221D000

	thumb_func_start ov07_0221D02C
ov07_0221D02C: ; 0x0221D02C
	ldr r1, [r0, #0x18]
	mov r3, #0
	add r1, r1, #4
	str r1, [r0, #0x18]
	add r2, r0, #0
_0221D036:
	ldr r1, [r2, #0x1c]
	cmp r1, #0
	bne _0221D052
	ldr r1, [r0, #0x18]
	add r2, r1, #4
	lsl r1, r3, #2
	add r1, r0, r1
	str r2, [r1, #0x1c]
	ldr r2, [r0, #0x18]
	ldr r1, [r2]
	lsl r1, r1, #2
	add r1, r2, r1
	str r1, [r0, #0x18]
	bx lr
_0221D052:
	add r3, r3, #1
	add r2, r2, #4
	cmp r3, #3
	blt _0221D036
	bx lr
	thumb_func_end ov07_0221D02C

	thumb_func_start ov07_0221D05C
ov07_0221D05C: ; 0x0221D05C
	ldr r1, [r0, #0x18]
	add r3, r0, #0
	add r1, r1, #4
	str r1, [r0, #0x18]
	mov r2, #2
	add r3, #8
_0221D068:
	ldr r1, [r3, #0x1c]
	cmp r1, #0
	beq _0221D07E
	add r3, r0, #0
	add r3, #0x1c
	lsl r2, r2, #2
	ldr r1, [r3, r2]
	str r1, [r0, #0x18]
	mov r0, #0
	str r0, [r3, r2]
	bx lr
_0221D07E:
	sub r3, r3, #4
	sub r2, r2, #1
	bpl _0221D068
	bx lr
	.balign 4, 0
	thumb_func_end ov07_0221D05C

	thumb_func_start ov07_0221D088
ov07_0221D088: ; 0x0221D088
	push {r3, r4}
	ldr r2, [r0, #0x18]
	add r1, r0, #0
	add r2, r2, #4
	str r2, [r0, #0x18]
	ldr r4, [r2]
	add r2, r2, #4
	str r2, [r0, #0x18]
	ldr r3, [r2]
	add r2, r2, #4
	str r2, [r0, #0x18]
	lsl r2, r4, #2
	add r2, r0, r2
	add r2, #0x94
	ldr r2, [r2]
	add r1, #0x18
	cmp r3, r2
	bne _0221D0B6
	ldr r1, [r0, #0x18]
	ldr r1, [r1]
	str r1, [r0, #0x18]
	pop {r3, r4}
	bx lr
_0221D0B6:
	ldr r0, [r1]
	add r0, r0, #4
	str r0, [r1]
	pop {r3, r4}
	bx lr
	thumb_func_end ov07_0221D088

	thumb_func_start ov07_0221D0C0
ov07_0221D0C0: ; 0x0221D0C0
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x18]
	add r0, r0, #4
	str r0, [r4, #0x18]
	add r0, r4, #0
	add r0, #0xc0
	ldr r0, [r0]
	bl ov07_0221F8C4
	cmp r0, #0
	ldr r0, [r4, #0x18]
	beq _0221D0E0
	ldr r0, [r0]
	str r0, [r4, #0x18]
	pop {r4, pc}
_0221D0E0:
	add r0, r0, #4
	str r0, [r4, #0x18]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D0C0

	thumb_func_start ov07_0221D0E8
ov07_0221D0E8: ; 0x0221D0E8
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x18]
	add r1, r0, #4
	str r1, [r4, #0x18]
	ldr r0, [r1]
	add r1, r1, #4
	str r1, [r4, #0x18]
	bl ov07_02223038
	add r7, r0, #0
	ldr r0, [r4, #0x18]
	add r2, r4, #0
	ldr r5, [r0]
	add r0, r0, #4
	add r2, #0x18
	mov r3, #0
	str r0, [r4, #0x18]
	cmp r5, #0
	bls _0221D12A
	add r6, r4, #0
_0221D112:
	ldr r0, [r4, #0x18]
	add r3, r3, #1
	ldr r1, [r0]
	add r0, r6, #0
	add r0, #0x94
	str r1, [r0]
	ldr r0, [r2]
	add r6, r6, #4
	add r0, r0, #4
	str r0, [r2]
	cmp r3, r5
	blo _0221D112
_0221D12A:
	cmp r3, #0xa
	bge _0221D142
	lsl r0, r3, #2
	add r2, r4, r0
	mov r1, #0
_0221D134:
	add r0, r2, #0
	add r0, #0x94
	add r3, r3, #1
	add r2, r2, #4
	str r1, [r0]
	cmp r3, #0xa
	blt _0221D134
_0221D142:
	add r0, r4, #0
	blx r7
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov07_0221D0E8

	thumb_func_start ov07_0221D148
ov07_0221D148: ; 0x0221D148
	ldr r2, [r0, #0x18]
	add r1, r0, #0
	add r2, r2, #4
	str r2, [r0, #0x18]
	add r2, r0, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r1, #0x18
	ldrh r3, [r2, #0x10]
	mov r2, #1
	tst r2, r3
	beq _0221D166
	ldr r2, [r1]
	add r2, r2, #4
	str r2, [r1]
_0221D166:
	ldr r2, [r0, #0x18]
	ldr r1, [r2]
	lsl r1, r1, #2
	add r1, r2, r1
	str r1, [r0, #0x18]
	bx lr
	.balign 4, 0
	thumb_func_end ov07_0221D148

	thumb_func_start ov07_0221D174
ov07_0221D174: ; 0x0221D174
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #0x18]
	add r1, r1, #4
	str r1, [r4, #0x18]
	ldr r2, [r1]
	add r1, r1, #4
	str r1, [r4, #0x18]
	cmp r2, #0
	bne _0221D196
	add r1, r4, #0
	add r1, #0xc0
	ldr r1, [r1]
	ldrh r1, [r1, #0x14]
	bl ov07_0223192C
	b _0221D1A2
_0221D196:
	add r1, r4, #0
	add r1, #0xc0
	ldr r1, [r1]
	ldrh r1, [r1, #0x16]
	bl ov07_0223192C
_0221D1A2:
	cmp r0, #4
	bne _0221D1AC
	ldr r0, [r4, #0x18]
	add r0, r0, #4
	str r0, [r4, #0x18]
_0221D1AC:
	ldr r1, [r4, #0x18]
	ldr r0, [r1]
	lsl r0, r0, #2
	add r0, r1, r0
	str r0, [r4, #0x18]
	pop {r4, pc}
	thumb_func_end ov07_0221D174

	thumb_func_start ov07_0221D1B8
ov07_0221D1B8: ; 0x0221D1B8
	push {r4, r5}
	sub sp, #0x10
	ldr r5, _0221D210 ; =ov07_02234B98
	add r2, sp, #0
	add r4, r0, #0
	add r3, r2, #0
	ldmia r5!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r1, [r4, #0x18]
	add r0, r4, #0
	add r1, r1, #4
	str r1, [r4, #0x18]
	add r1, r4, #0
	add r1, #0xc0
	ldr r1, [r1]
	add r0, #0x18
	ldr r1, [r1, #0xc]
	cmp r1, #0
	beq _0221D1FE
	ldr r2, [r0]
	add r2, r2, #4
	str r2, [r0]
	mov r2, #0
_0221D1EA:
	ldr r5, [r3]
	tst r5, r1
	bne _0221D1FE
	ldr r5, [r0]
	add r2, r2, #1
	add r5, r5, #4
	add r3, r3, #4
	str r5, [r0]
	cmp r2, #4
	blo _0221D1EA
_0221D1FE:
	ldr r1, [r4, #0x18]
	ldr r0, [r1]
	lsl r0, r0, #2
	add r0, r1, r0
	str r0, [r4, #0x18]
	add sp, #0x10
	pop {r4, r5}
	bx lr
	nop
_0221D210: .word ov07_02234B98
	thumb_func_end ov07_0221D1B8

	thumb_func_start ov07_0221D214
ov07_0221D214: ; 0x0221D214
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #0x18]
	add r1, r1, #4
	str r1, [r4, #0x18]
	bl ov07_0221BFC0
	cmp r0, #1
	bne _0221D232
	ldr r1, [r4, #0x18]
	ldr r0, [r1]
	lsl r0, r0, #2
	add r0, r1, r0
	str r0, [r4, #0x18]
	pop {r4, pc}
_0221D232:
	ldr r0, [r4, #0x18]
	add r0, r0, #4
	str r0, [r4, #0x18]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D214

	thumb_func_start ov07_0221D23C
ov07_0221D23C: ; 0x0221D23C
	ldr r2, [r0, #0x18]
	add r1, r0, #0
	add r2, r2, #4
	str r2, [r0, #0x18]
	add r2, r0, #0
	add r2, #0xc0
	ldr r3, [r2]
	mov r2, #0x46
	lsl r2, r2, #2
	ldr r2, [r3, r2]
	add r1, #0x18
	lsl r2, r2, #0x1e
	asr r2, r2, #0x1f
	beq _0221D266
	ldr r0, [r0, #0x18]
	ldr r2, [r1]
	ldr r0, [r0]
	lsl r0, r0, #2
	add r0, r2, r0
	str r0, [r1]
	bx lr
_0221D266:
	ldr r0, [r1]
	add r0, r0, #4
	str r0, [r1]
	bx lr
	.balign 4, 0
	thumb_func_end ov07_0221D23C

	thumb_func_start ov07_0221D270
ov07_0221D270: ; 0x0221D270
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r1, [r5, #0x18]
	add r1, r1, #4
	str r1, [r5, #0x18]
	add r1, r5, #0
	add r1, #0xc0
	ldr r1, [r1]
	ldrh r1, [r1, #0x14]
	bl ov07_0223192C
	add r1, r5, #0
	add r1, #0xc0
	ldr r1, [r1]
	add r4, r0, #0
	ldrh r1, [r1, #0x16]
	add r0, r5, #0
	bl ov07_0223192C
	cmp r4, r0
	bne _0221D2A6
	ldr r1, [r5, #0x18]
	ldr r0, [r1]
	lsl r0, r0, #2
	add r0, r1, r0
	str r0, [r5, #0x18]
	pop {r3, r4, r5, pc}
_0221D2A6:
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D270

	thumb_func_start ov07_0221D2B0
ov07_0221D2B0: ; 0x0221D2B0
	ldr r2, [r0, #0x18]
	add r1, r0, #0
	add r2, r2, #4
	str r2, [r0, #0x18]
	ldr r3, [r2]
	add r2, r2, #4
	str r2, [r0, #0x18]
	add r2, r0, #0
	add r2, #0xc0
	ldr r2, [r2]
	add r1, #0x18
	ldrh r2, [r2, #0x10]
	cmp r3, r2
	bne _0221D2DA
	ldr r0, [r0, #0x18]
	ldr r2, [r1]
	ldr r0, [r0]
	lsl r0, r0, #2
	add r0, r2, r0
	str r0, [r1]
	bx lr
_0221D2DA:
	ldr r0, [r1]
	add r0, r0, #4
	str r0, [r1]
	bx lr
	.balign 4, 0
	thumb_func_end ov07_0221D2B0

	thumb_func_start ov07_0221D2E4
ov07_0221D2E4: ; 0x0221D2E4
	ldr r1, [r0, #0x18]
	add r2, r1, #4
	str r2, [r0, #0x18]
	ldr r1, [r2]
	lsl r1, r1, #2
	add r1, r2, r1
	str r1, [r0, #0x18]
	bx lr
	thumb_func_end ov07_0221D2E4

	thumb_func_start ov07_0221D2F4
ov07_0221D2F4: ; 0x0221D2F4
	push {r3, lr}
	cmp r1, #0
	beq _0221D314
	mov r1, #1
	str r1, [sp]
	ldr r0, [r0, #4]
	mov r1, #0x10
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r2, #0
	mov r3, #0x3c
	bl StartBrightnessTransition
	pop {r3, pc}
_0221D314:
	mov r1, #1
	str r1, [sp]
	ldr r0, [r0, #4]
	mov r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r2, #0x10
	mov r3, #0x3c
	bl StartBrightnessTransition
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D2F4

	thumb_func_start ov07_0221D330
ov07_0221D330: ; 0x0221D330
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl DoAllScreenBrightnessTransitionStep
	mov r0, #1
	bl IsBrightnessTransitionActive
	cmp r0, #0
	beq _0221D370
	ldr r0, [r4, #4]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x1f
	beq _0221D35E
	add r0, r4, #0
	bl Heap_Free
	add r0, r5, #0
	bl SysTask_Destroy
	bl ov07_0221C69C
	pop {r3, r4, r5, pc}
_0221D35E:
	add r0, r4, #0
	mov r1, #0
	bl ov07_0221D2F4
	mov r0, #1
	ldr r1, [r4, #4]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #4]
_0221D370:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D330

	thumb_func_start ov07_0221D374
ov07_0221D374: ; 0x0221D374
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5]
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	mov r1, #0
	mov r2, #8
	bl MI_CpuFill8
	str r5, [r4]
	ldr r0, [r5, #0x18]
	mov r1, #0xff
	add r2, r0, #4
	str r2, [r5, #0x18]
	ldr r0, [r4, #4]
	bic r0, r1
	ldr r1, [r2]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	orr r0, r1
	str r0, [r4, #4]
	ldr r0, [r5, #0x18]
	add r0, r0, #4
	str r0, [r5, #0x18]
	bl ScreenBrightnessData_InitAll
	add r0, r4, #0
	mov r1, #1
	bl ov07_0221D2F4
	ldr r0, _0221D3C4 ; =ov07_0221D330
	ldr r2, _0221D3C8 ; =0x00001001
	add r1, r4, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0221D3C4: .word ov07_0221D330
_0221D3C8: .word 0x00001001
	thumb_func_end ov07_0221D374

	thumb_func_start ov07_0221D3CC
ov07_0221D3CC: ; 0x0221D3CC
	push {r4, r5, r6, lr}
	add r4, r0, #0
	cmp r1, #7
	bhi _0221D4AA
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0221D3E0: ; jump table
	.short _0221D3F0 - _0221D3E0 - 2 ; case 0
	.short _0221D3F8 - _0221D3E0 - 2 ; case 1
	.short _0221D400 - _0221D3E0 - 2 ; case 2
	.short _0221D40E - _0221D3E0 - 2 ; case 3
	.short _0221D41C - _0221D3E0 - 2 ; case 4
	.short _0221D442 - _0221D3E0 - 2 ; case 5
	.short _0221D468 - _0221D3E0 - 2 ; case 6
	.short _0221D48A - _0221D3E0 - 2 ; case 7
_0221D3F0:
	add r4, #0xc0
	ldr r0, [r4]
	ldrh r6, [r0, #0x14]
	b _0221D4AA
_0221D3F8:
	add r4, #0xc0
	ldr r0, [r4]
	ldrh r6, [r0, #0x16]
	b _0221D4AA
_0221D400:
	add r4, #0xc0
	ldr r1, [r4]
	ldrh r1, [r1, #0x14]
	bl ov07_0223197C
	add r6, r0, #0
	b _0221D4AA
_0221D40E:
	add r4, #0xc0
	ldr r1, [r4]
	ldrh r1, [r1, #0x16]
	bl ov07_0223197C
	add r6, r0, #0
	b _0221D4AA
_0221D41C:
	mov r6, #0xff
	mov r5, #0
_0221D420:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #0
	beq _0221D430
	cmp r0, #2
	bne _0221D434
_0221D430:
	add r6, r5, #0
	b _0221D43A
_0221D434:
	add r5, r5, #1
	cmp r5, #4
	blt _0221D420
_0221D43A:
	cmp r6, #0xff
	bne _0221D4AA
	mov r6, #0
	b _0221D4AA
_0221D442:
	mov r6, #0xff
	mov r5, #0
_0221D446:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #1
	beq _0221D456
	cmp r0, #3
	bne _0221D45A
_0221D456:
	add r6, r5, #0
	b _0221D460
_0221D45A:
	add r5, r5, #1
	cmp r5, #4
	blt _0221D446
_0221D460:
	cmp r6, #0xff
	bne _0221D4AA
	mov r6, #0
	b _0221D4AA
_0221D468:
	mov r6, #0xff
	mov r5, #0
_0221D46C:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #4
	bne _0221D47C
	add r6, r5, #0
	b _0221D482
_0221D47C:
	add r5, r5, #1
	cmp r5, #4
	blt _0221D46C
_0221D482:
	cmp r6, #0xff
	bne _0221D4AA
	mov r6, #0
	b _0221D4AA
_0221D48A:
	mov r6, #0xff
	mov r5, #0
_0221D48E:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #5
	bne _0221D49E
	add r6, r5, #0
	b _0221D4A4
_0221D49E:
	add r5, r5, #1
	cmp r5, #4
	blt _0221D48E
_0221D4A4:
	cmp r6, #0xff
	bne _0221D4AA
	mov r6, #0
_0221D4AA:
	add r0, r6, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov07_0221D3CC

	thumb_func_start ov07_0221D4B0
ov07_0221D4B0: ; 0x0221D4B0
	push {r4, r5, r6, lr}
	add r5, r1, #0
	ldrb r0, [r5, #5]
	cmp r0, #0
	beq _0221D4CC
	ldrb r0, [r5, #4]
	add r0, r0, #1
	strb r0, [r5, #4]
	ldrb r1, [r5, #4]
	ldrb r0, [r5, #5]
	cmp r1, r0
	bne _0221D4FA
	mov r0, #0
	strb r0, [r5, #4]
_0221D4CC:
	ldr r0, [r5, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	lsl r0, r0, #0x10
	asr r6, r0, #0x10
	ldr r0, [r5, #8]
	mov r1, #1
	bl Pokepic_GetAttr
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	ldr r0, [r5, #8]
	mov r1, #0x29
	bl Pokepic_GetAttr
	sub r0, r4, r0
	lsl r0, r0, #0x10
	asr r2, r0, #0x10
	ldr r0, [r5]
	add r1, r6, #0
	bl ManagedSprite_SetPositionXY
_0221D4FA:
	pop {r4, r5, r6, pc}
	thumb_func_end ov07_0221D4B0

	thumb_func_start ov07_0221D4FC
ov07_0221D4FC: ; 0x0221D4FC
	push {r4, r5, r6, lr}
	add r5, r1, #0
	ldrb r0, [r5, #5]
	cmp r0, #0
	beq _0221D518
	ldrb r0, [r5, #4]
	add r0, r0, #1
	strb r0, [r5, #4]
	ldrb r1, [r5, #4]
	ldrb r0, [r5, #5]
	cmp r1, r0
	bne _0221D55A
	mov r0, #0
	strb r0, [r5, #4]
_0221D518:
	ldr r0, [r5, #8]
	mov r1, #0
	bl Pokepic_GetAttr
	lsl r0, r0, #0x10
	asr r6, r0, #0x10
	ldr r0, [r5, #8]
	mov r1, #1
	bl Pokepic_GetAttr
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	ldr r0, [r5, #8]
	mov r1, #0x29
	bl Pokepic_GetAttr
	sub r0, r4, r0
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	sub r6, #0x28
	ldr r0, [r5]
	mov r1, #2
	mov r2, #0
	neg r3, r6
	bl BgSetPosTextAndCommit
	sub r4, #0x28
	ldr r0, [r5]
	mov r1, #2
	mov r2, #3
	neg r3, r4
	bl BgSetPosTextAndCommit
_0221D55A:
	pop {r4, r5, r6, pc}
	thumb_func_end ov07_0221D4FC

	thumb_func_start ov07_0221D55C
ov07_0221D55C: ; 0x0221D55C
	push {r3, r4, r5, lr}
	add r4, r0, #0
	cmp r1, #4
	bne _0221D588
	mov r0, #0x5e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0221D5A6
	ldr r0, [r0, #0xc]
	bl SysTask_Destroy
	mov r0, #0x5e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Heap_Free
	mov r0, #0x5e
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	pop {r3, r4, r5, pc}
_0221D588:
	mov r0, #0x59
	lsl r0, r0, #2
	lsl r5, r1, #2
	add r4, r4, r0
	ldr r0, [r4, r5]
	cmp r0, #0
	beq _0221D5A6
	ldr r0, [r0, #0xc]
	bl SysTask_Destroy
	ldr r0, [r4, r5]
	bl Heap_Free
	mov r0, #0
	str r0, [r4, r5]
_0221D5A6:
	pop {r3, r4, r5, pc}
	thumb_func_end ov07_0221D55C
