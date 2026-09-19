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


	thumb_func_start ov07_02223064
ov07_02223064: ; 0x02223064
	push {r4, lr}
	add r4, r1, #0
	add r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _02223076
	cmp r0, #1
	beq _0222307C
	pop {r4, pc}
_02223076:
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, pc}
_0222307C:
	ldr r0, [r4, #4]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	thumb_func_end ov07_02223064

	thumb_func_start ov07_0222308C
ov07_0222308C: ; 0x0222308C
	push {r4, lr}
	add r4, r0, #0
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r2, r0, #0
	mov r0, #0
	strb r0, [r2]
	mov r3, #1
	ldr r1, _022230B0 ; =ov07_02223064
	add r0, r4, #0
	lsl r3, r3, #0xc
	str r4, [r2, #4]
	bl ov07_0221C3F4
	pop {r4, pc}
	nop
_022230B0: .word ov07_02223064
	thumb_func_end ov07_0222308C

	thumb_func_start ov07_022230B4
ov07_022230B4: ; 0x022230B4
	push {r4, lr}
	add r4, r1, #0
	add r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _022230C6
	cmp r0, #1
	beq _022230CC
	pop {r4, pc}
_022230C6:
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, pc}
_022230CC:
	ldr r0, [r4, #4]
	add r1, r2, #0
	bl ov07_0221C458
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	thumb_func_end ov07_022230B4

	thumb_func_start ov07_022230DC
ov07_022230DC: ; 0x022230DC
	push {r4, lr}
	add r4, r0, #0
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r2, r0, #0
	mov r0, #0
	strb r0, [r2]
	mov r3, #1
	ldr r1, _02223100 ; =ov07_022230B4
	add r0, r4, #0
	lsl r3, r3, #0xc
	str r4, [r2, #4]
	bl ov07_0221C42C
	pop {r4, pc}
	nop
_02223100: .word ov07_022230B4
	thumb_func_end ov07_022230DC

	thumb_func_start ov07_02223104
ov07_02223104: ; 0x02223104
	push {r4, lr}
	add r4, r1, #0
	ldrb r1, [r4]
	cmp r1, #0
	beq _02223114
	cmp r1, #1
	beq _0222311A
	pop {r4, pc}
_02223114:
	add r0, r1, #1
	strb r0, [r4]
	pop {r4, pc}
_0222311A:
	bl SysTask_Destroy
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov07_02223104

	thumb_func_start ov07_02223128
ov07_02223128: ; 0x02223128
	push {r4, lr}
	add r4, r0, #0
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r1, r0, #0
	mov r0, #0
	strb r0, [r1]
	mov r2, #1
	ldr r0, _02223148 ; =ov07_02223104
	lsl r2, r2, #0xc
	str r4, [r1, #4]
	bl SysTask_CreateOnMainQueue
	pop {r4, pc}
	.balign 4, 0
_02223148: .word ov07_02223104
	thumb_func_end ov07_02223128

	thumb_func_start ov07_0222314C
ov07_0222314C: ; 0x0222314C
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r6, r0, #0
	ldrb r0, [r5]
	cmp r0, #0
	beq _0222315E
	cmp r0, #1
	beq _022231A4
	pop {r4, r5, r6, pc}
_0222315E:
	ldr r0, [r5, #0x14]
	ldr r0, [r0]
	bl Sprite_TickFrame
	ldr r0, [r5, #0x18]
	ldr r0, [r0]
	bl Sprite_TickFrame
	ldr r0, [r5, #0x1c]
	ldr r0, [r0]
	bl Sprite_TickFrame
	ldr r0, [r5, #0x14]
	mov r1, #1
	ldr r0, [r0]
	mov r2, #0
	bl Sprite_OffsetPositionXY
	ldr r0, [r5, #0x18]
	mov r1, #0
	ldr r0, [r0]
	mvn r1, r1
	mov r2, #0
	bl Sprite_OffsetPositionXY
	ldr r0, [r5, #0x1c]
	mov r1, #1
	ldr r0, [r0]
	add r2, r1, #0
	bl Sprite_OffsetPositionXY
	ldr r0, [r5, #0xc]
	bl SpriteSystem_DrawSprites
	pop {r4, r5, r6, pc}
_022231A4:
	ldr r0, [r5, #0x10]
	ldr r4, [r5, #4]
	bl Sprite_DeleteAndFreeResources
	add r0, r5, #0
	bl Heap_Free
	add r0, r4, #0
	add r1, r6, #0
	bl ov07_0221C448
	pop {r4, r5, r6, pc}
	thumb_func_end ov07_0222314C

	thumb_func_start ov07_022231BC
ov07_022231BC: ; 0x022231BC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	add r6, r1, #0
	mov r1, #0x73
	str r0, [sp]
	mov r0, #5
	lsl r1, r1, #2
	add r7, r2, #0
	add r5, r3, #0
	bl Heap_Alloc
	add r4, r0, #0
	bne _022231DA
	bl GF_AssertFail
_022231DA:
	mov r0, #0
	strb r0, [r4]
	str r6, [r4, #8]
	str r7, [r4, #0xc]
	ldr r0, [sp]
	str r5, [r4, #0x10]
	str r0, [r4, #4]
	ldr r1, [sp]
	add r0, sp, #4
	bl ov07_0221F9E8
	str r5, [r4, #0x14]
	mov r6, #1
	add r5, r4, #4
	add r7, sp, #4
_022231F8:
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	add r2, r7, #0
	bl SpriteSystem_NewSprite
	str r0, [r5, #0x14]
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #3
	blt _022231F8
	mov r3, #1
	ldr r0, [sp]
	ldr r1, _02223220 ; =ov07_0222314C
	add r2, r4, #0
	lsl r3, r3, #0xc
	bl ov07_0221C3F4
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223220: .word ov07_0222314C
	thumb_func_end ov07_022231BC

	thumb_func_start ov07_02223224
ov07_02223224: ; 0x02223224
	push {r4, lr}
	mov r1, #0xc
	add r2, r1, #0
	add r2, #0xf4
	add r4, r0, #0
	bl Pokepic_SetAttr
	mov r1, #0xd
	add r2, r1, #0
	add r0, r4, #0
	add r2, #0xf3
	bl Pokepic_SetAttr
	pop {r4, pc}
	thumb_func_end ov07_02223224

	thumb_func_start ov07_02223240
ov07_02223240: ; 0x02223240
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r2, r0, #0
	ldrb r0, [r5, #4]
	cmp r0, #0
	beq _02223252
	cmp r0, #1
	beq _02223274
	b _0222333E
_02223252:
	mov r0, #0x26
	ldrsh r0, [r5, r0]
	mov r1, #0x20
	mov r2, #0x22
	str r0, [sp]
	mov r3, #0x24
	add r0, r5, #0
	ldrsh r1, [r5, r1]
	ldrsh r2, [r5, r2]
	ldrsh r3, [r5, r3]
	add r0, #0x7c
	bl ov07_022227A8
	ldrb r0, [r5, #4]
	add r0, r0, #1
	strb r0, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
_02223274:
	add r0, r5, #0
	add r0, #0x7c
	bl ov07_022227D8
	cmp r0, #0
	bne _02223288
	ldrb r0, [r5, #4]
	add r0, r0, #1
	strb r0, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
_02223288:
	ldr r2, [r5, #0x28]
	mov r1, #1
	lsl r1, r1, #8
	add r0, r2, #0
	and r0, r1
	cmp r0, r1
	bne _022232D4
	ldr r0, [r5]
	mov r6, #0
	cmp r0, #0
	ble _02223322
	add r4, r5, #0
	mov r7, #0x32
_022232A2:
	ldr r0, [r4, #0x34]
	cmp r0, #0
	beq _022232C8
	mov r2, #0x7c
	mov r3, #0x30
	ldrsh r2, [r5, r2]
	ldrsh r3, [r4, r3]
	mov r1, #0
	add r2, r2, r3
	bl Pokepic_SetAttr
	mov r2, #0x7e
	ldrsh r3, [r5, r2]
	ldrsh r2, [r4, r7]
	ldr r0, [r4, #0x34]
	mov r1, #1
	add r2, r3, r2
	bl Pokepic_SetAttr
_022232C8:
	ldr r0, [r5]
	add r6, r6, #1
	add r4, #0x14
	cmp r6, r0
	blt _022232A2
	b _02223322
_022232D4:
	lsl r0, r1, #1
	and r2, r0
	cmp r2, r0
	bne _02223322
	ldr r0, [r5]
	mov r6, #0
	cmp r0, #0
	ble _0222331C
	add r4, r5, #0
	mov r7, #0x32
_022232E8:
	ldr r0, [r4, #0x38]
	cmp r0, #0
	beq _02223312
	mov r1, #0x7c
	mov r2, #0x30
	ldrsh r1, [r5, r1]
	ldrsh r2, [r4, r2]
	add r1, r1, r2
	mov r2, #0x7e
	ldrsh r3, [r5, r2]
	ldrsh r2, [r4, r7]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add r2, r3, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	ldr r0, [r4, #0x38]
	bl ManagedSprite_TickFrame
_02223312:
	ldr r0, [r5]
	add r6, r6, #1
	add r4, #0x14
	cmp r6, r0
	blt _022232E8
_0222331C:
	ldr r0, [r5, #0x10]
	bl SpriteSystem_DrawSprites
_02223322:
	mov r0, #1
	ldr r1, [r5, #0x28]
	lsl r0, r0, #0xa
	and r1, r0
	cmp r1, r0
	bne _0222334C
	mov r3, #0x7c
	ldrsh r3, [r5, r3]
	ldr r0, [r5, #0x18]
	mov r1, #3
	mov r2, #0
	bl BgSetPosTextAndCommit
	pop {r3, r4, r5, r6, r7, pc}
_0222333E:
	ldr r0, [r5, #8]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r5, #0
	bl Heap_Free
_0222334C:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_02223240

	thumb_func_start ov07_02223350
ov07_02223350: ; 0x02223350
	push {r3, r4, r5, lr}
	mov r1, #0xa0
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	add r0, r5, #0
	add r1, r4, #4
	bl ov07_02231FE4
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	strh r0, [r4, #0x20]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	strh r0, [r4, #0x22]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	strh r0, [r4, #0x24]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	strh r0, [r4, #0x26]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	mov r2, #1
	lsl r2, r2, #8
	str r0, [r4, #0x28]
	add r1, r0, #0
	and r0, r2
	cmp r0, r2
	bne _022233B0
	add r2, r4, #0
	add r0, r5, #0
	add r2, #0x2c
	add r3, r4, #0
	bl ov07_02232020
	b _022233C6
_022233B0:
	lsl r0, r2, #1
	add r3, r1, #0
	and r3, r0
	cmp r3, r0
	bne _022233C6
	add r2, r4, #0
	add r0, r5, #0
	add r2, #0x2c
	add r3, r4, #0
	bl ov07_022323FC
_022233C6:
	ldr r0, [r4, #8]
	ldr r1, _022233D4 ; =ov07_02223240
	add r2, r4, #0
	bl ov07_0221C410
	pop {r3, r4, r5, pc}
	nop
_022233D4: .word ov07_02223240
	thumb_func_end ov07_02223350

	thumb_func_start ov07_022233D8
ov07_022233D8: ; 0x022233D8
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldrb r0, [r4, #0x1c]
	cmp r0, #3
	bhi _022234A2
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_022233F0: ; jump table
	.short _022233F8 - _022233F0 - 2 ; case 0
	.short _02223416 - _022233F0 - 2 ; case 1
	.short _02223458 - _022233F0 - 2 ; case 2
	.short _02223464 - _022233F0 - 2 ; case 3
_022233F8:
	ldrh r0, [r4, #0x18]
	mov r1, #4
	mov r2, #0x39
	str r0, [sp]
	ldrh r3, [r4, #0x16]
	ldr r0, _02223544 ; =0x04000050
	bl G2x_SetBlendAlpha_
	mov r0, #2
	mov r1, #1
	bl ToggleBgLayer
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_02223416:
	ldrh r1, [r4, #8]
	ldrh r2, [r4, #0x16]
	mov r0, #0
	sub r1, r1, #2
	cmp r2, r1
	bge _02223428
	add r1, r2, #2
	strh r1, [r4, #0x16]
	b _0222342A
_02223428:
	add r0, r0, #1
_0222342A:
	ldrh r1, [r4, #0x18]
	cmp r1, #9
	bls _02223436
	sub r1, r1, #2
	strh r1, [r4, #0x18]
	b _02223438
_02223436:
	add r0, r0, #1
_02223438:
	cmp r0, #2
	bne _0222344A
	ldrh r0, [r4, #8]
	strh r0, [r4, #0x16]
	mov r0, #7
	strh r0, [r4, #0x18]
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_0222344A:
	ldrh r0, [r4, #0x18]
	ldrh r1, [r4, #0x16]
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _02223548 ; =0x04000052
	strh r1, [r0]
	b _022234BA
_02223458:
	ldr r1, [r4, #4]
	cmp r1, #0
	beq _022234BA
	add r0, r0, #1
	strb r0, [r4, #0x1c]
	b _022234BA
_02223464:
	ldrh r0, [r4, #0x16]
	mov r1, #0
	cmp r0, #2
	bls _02223472
	sub r0, r0, #2
	strh r0, [r4, #0x16]
	b _02223474
_02223472:
	add r1, r1, #1
_02223474:
	ldrh r0, [r4, #0x18]
	cmp r0, #0x1d
	bhs _02223480
	add r0, r0, #2
	strh r0, [r4, #0x18]
	b _02223482
_02223480:
	add r1, r1, #1
_02223482:
	cmp r1, #2
	bne _02223494
	mov r0, #0
	strh r0, [r4, #0x16]
	mov r0, #0x1f
	strh r0, [r4, #0x18]
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_02223494:
	ldrh r0, [r4, #0x18]
	ldrh r1, [r4, #0x16]
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _02223548 ; =0x04000052
	strh r1, [r0]
	b _022234BA
_022234A2:
	mov r0, #2
	mov r1, #0
	bl ToggleBgLayer
	ldr r0, [r4, #0x20]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, pc}
_022234BA:
	mov r1, #0x10
	ldrsh r3, [r4, r1]
	cmp r3, #0
	ble _022234CC
	mov r0, #0xc
	ldrsh r2, [r4, r0]
	lsl r0, r1, #5
	cmp r2, r0
	bge _022234DA
_022234CC:
	cmp r3, #0
	bge _022234DE
	mov r0, #0xc
	ldrsh r1, [r4, r0]
	ldr r0, _0222354C ; =0xFFFFFE64
	cmp r1, r0
	bgt _022234DE
_022234DA:
	mov r0, #1
	str r0, [r4, #4]
_022234DE:
	mov r0, #0xa
	ldrsh r1, [r4, r0]
	mov r0, #0xe
	ldrsh r0, [r4, r0]
	add r0, r1, r0
	strh r0, [r4, #0xa]
	mov r0, #0xc
	ldrsh r2, [r4, r0]
	mov r0, #0x10
	ldrsh r1, [r4, r0]
	add r1, r2, r1
	strh r1, [r4, #0xc]
	ldrh r2, [r4, #0x12]
	ldrh r1, [r4, #0x14]
	cmp r2, r1
	bhs _02223522
	ldrsh r0, [r4, r0]
	cmp r0, #0
	bge _02223508
	add r0, r0, #1
	b _0222350A
_02223508:
	mov r0, #0
_0222350A:
	strh r0, [r4, #0x10]
	mov r0, #0xe
	ldrsh r0, [r4, r0]
	cmp r0, #0
	bge _02223518
	add r0, r0, #1
	b _0222351A
_02223518:
	mov r0, #0
_0222351A:
	strh r0, [r4, #0xe]
	mov r0, #0
	strh r0, [r4, #0x14]
	b _02223526
_02223522:
	add r0, r1, #1
	strh r0, [r4, #0x14]
_02223526:
	mov r3, #0xa
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #0
	bl BgSetPosTextAndCommit
	mov r3, #0xc
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #3
	bl BgSetPosTextAndCommit
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02223544: .word 0x04000050
_02223548: .word 0x04000052
_0222354C: .word 0xFFFFFE64
	thumb_func_end ov07_022233D8

	thumb_func_start ov07_02223550
ov07_02223550: ; 0x02223550
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	mov r1, #0xb8
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x1c
	bl ov07_02231FE4
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	strh r0, [r4, #0xa]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	strh r0, [r4, #0xc]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	strh r0, [r4, #0xe]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	strh r0, [r4, #0x10]
	add r0, r5, #0
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r4]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	strh r0, [r4, #8]
	add r0, r5, #0
	mov r1, #7
	bl ov07_0221C4A8
	strh r0, [r4, #0x12]
	mov r0, #0
	strh r0, [r4, #0x14]
	ldr r0, [r4]
	cmp r0, #0
	beq _022235FC
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223192C
	cmp r0, #4
	bne _022235FC
	mov r0, #0xa
	ldrsh r1, [r4, r0]
	sub r0, #0xb
	mul r0, r1
	strh r0, [r4, #0xa]
	mov r1, #0xc
	add r0, r1, #0
	ldrsh r2, [r4, r1]
	sub r0, #0xd
	mul r0, r2
	strh r0, [r4, #0xc]
	mov r0, #0xe
	ldrsh r2, [r4, r0]
	sub r0, #0xf
	mul r0, r2
	strh r0, [r4, #0xe]
	mov r0, #0x10
	ldrsh r2, [r4, r0]
	sub r0, #0x11
	mul r0, r2
	strh r0, [r4, #0x10]
	ldrsh r0, [r4, r1]
	sub r0, #0x54
	strh r0, [r4, #0xc]
	b _02223604
_022235FC:
	mov r0, #0xc
	ldrsh r0, [r4, r0]
	add r0, #0x54
	strh r0, [r4, #0xc]
_02223604:
	mov r1, #0
	strh r1, [r4, #0x16]
	mov r0, #0x1f
	strh r0, [r4, #0x18]
	mov r0, #2
	str r1, [r4, #4]
	bl ToggleBgLayer
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	mov r1, #0
	add r7, r0, #0
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x30]
	mov r0, #7
	add r1, r6, #0
	mov r3, #2
	bl GfGfxLoader_LoadCharData
	add r0, r7, #0
	mov r1, #1
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0x90
	str r0, [sp, #8]
	ldr r0, [r4, #0x34]
	mov r1, #7
	add r2, r6, #0
	bl PaletteData_LoadNarc
	ldr r0, [r4, #0x30]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	add r0, r5, #0
	mov r6, #2
	bl ov07_0221BFC0
	cmp r0, #1
	bne _02223684
	mov r6, #4
	b _02223698
_02223684:
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223192C
	cmp r0, #4
	bne _02223698
	mov r6, #3
_02223698:
	add r0, r7, #0
	add r1, r6, #0
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x30]
	mov r0, #7
	add r1, r6, #0
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r3, #0xa
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #0
	bl BgSetPosTextAndCommit
	mov r3, #0xc
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #3
	bl BgSetPosTextAndCommit
	ldr r0, [r4, #0x20]
	ldr r1, _022236EC ; =ov07_022233D8
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022236EC: .word ov07_022233D8
	thumb_func_end ov07_02223550

	thumb_func_start ov07_022236F0
ov07_022236F0: ; 0x022236F0
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldrb r0, [r4, #0x1c]
	cmp r0, #3
	bhi _022237B6
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02223708: ; jump table
	.short _02223710 - _02223708 - 2 ; case 0
	.short _0222372E - _02223708 - 2 ; case 1
	.short _0222376C - _02223708 - 2 ; case 2
	.short _02223778 - _02223708 - 2 ; case 3
_02223710:
	ldrh r0, [r4, #0x18]
	mov r1, #4
	mov r2, #0x39
	str r0, [sp]
	ldrh r3, [r4, #0x16]
	ldr r0, _02223858 ; =0x04000050
	bl G2x_SetBlendAlpha_
	mov r0, #2
	mov r1, #1
	bl ToggleBgLayer
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_0222372E:
	ldrh r0, [r4, #0x16]
	mov r1, #0
	cmp r0, #0x10
	bhs _0222373C
	add r0, r0, #2
	strh r0, [r4, #0x16]
	b _0222373E
_0222373C:
	add r1, r1, #1
_0222373E:
	ldrh r0, [r4, #0x18]
	cmp r0, #4
	bls _0222374A
	sub r0, r0, #2
	strh r0, [r4, #0x18]
	b _0222374C
_0222374A:
	add r1, r1, #1
_0222374C:
	cmp r1, #2
	bne _0222375E
	mov r0, #0x10
	strh r0, [r4, #0x16]
	mov r0, #4
	strh r0, [r4, #0x18]
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_0222375E:
	ldrh r0, [r4, #0x18]
	ldrh r1, [r4, #0x16]
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _0222385C ; =0x04000052
	strh r1, [r0]
	b _022237CE
_0222376C:
	ldr r1, [r4, #4]
	cmp r1, #0
	beq _022237CE
	add r0, r0, #1
	strb r0, [r4, #0x1c]
	b _022237CE
_02223778:
	ldrh r0, [r4, #0x16]
	mov r1, #0
	cmp r0, #2
	bls _02223786
	sub r0, r0, #2
	strh r0, [r4, #0x16]
	b _02223788
_02223786:
	add r1, r1, #1
_02223788:
	ldrh r0, [r4, #0x18]
	cmp r0, #0x10
	bhs _02223794
	add r0, r0, #2
	strh r0, [r4, #0x18]
	b _02223796
_02223794:
	add r1, r1, #1
_02223796:
	cmp r1, #2
	bne _022237A8
	mov r0, #0
	strh r0, [r4, #0x16]
	mov r0, #0x1f
	strh r0, [r4, #0x18]
	ldrb r0, [r4, #0x1c]
	add r0, r0, #1
	strb r0, [r4, #0x1c]
_022237A8:
	ldrh r0, [r4, #0x18]
	ldrh r1, [r4, #0x16]
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _0222385C ; =0x04000052
	strh r1, [r0]
	b _022237CE
_022237B6:
	mov r0, #2
	mov r1, #0
	bl ToggleBgLayer
	ldr r0, [r4, #0x20]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, pc}
_022237CE:
	mov r1, #0x10
	ldrsh r3, [r4, r1]
	cmp r3, #0
	ble _022237E0
	mov r0, #0xc
	ldrsh r2, [r4, r0]
	lsl r0, r1, #5
	cmp r2, r0
	bge _022237EE
_022237E0:
	cmp r3, #0
	bge _022237F2
	mov r0, #0xc
	ldrsh r1, [r4, r0]
	ldr r0, _02223860 ; =0xFFFFFE64
	cmp r1, r0
	bgt _022237F2
_022237EE:
	mov r0, #1
	str r0, [r4, #4]
_022237F2:
	mov r0, #0xa
	ldrsh r1, [r4, r0]
	mov r0, #0xe
	ldrsh r0, [r4, r0]
	add r0, r1, r0
	strh r0, [r4, #0xa]
	mov r0, #0xc
	ldrsh r2, [r4, r0]
	mov r0, #0x10
	ldrsh r1, [r4, r0]
	add r1, r2, r1
	strh r1, [r4, #0xc]
	ldrh r2, [r4, #0x12]
	ldrh r1, [r4, #0x14]
	cmp r2, r1
	bhs _02223836
	ldrsh r0, [r4, r0]
	cmp r0, #0
	bge _0222381C
	add r0, r0, #1
	b _0222381E
_0222381C:
	mov r0, #0
_0222381E:
	strh r0, [r4, #0x10]
	mov r0, #0xe
	ldrsh r0, [r4, r0]
	cmp r0, #0
	bge _0222382C
	add r0, r0, #1
	b _0222382E
_0222382C:
	mov r0, #0
_0222382E:
	strh r0, [r4, #0xe]
	mov r0, #0
	strh r0, [r4, #0x14]
	b _0222383A
_02223836:
	add r0, r1, #1
	strh r0, [r4, #0x14]
_0222383A:
	mov r3, #0xa
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #0
	bl BgSetPosTextAndCommit
	mov r3, #0xc
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #3
	bl BgSetPosTextAndCommit
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02223858: .word 0x04000050
_0222385C: .word 0x04000052
_02223860: .word 0xFFFFFE64
	thumb_func_end ov07_022236F0

	thumb_func_start ov07_02223864
ov07_02223864: ; 0x02223864
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	mov r1, #0xb8
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x1c
	bl ov07_02231FE4
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	strh r0, [r4, #0xa]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	strh r0, [r4, #0xc]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	strh r0, [r4, #0xe]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	strh r0, [r4, #0x10]
	add r0, r5, #0
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r4]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	strh r0, [r4, #8]
	add r0, r5, #0
	mov r1, #7
	bl ov07_0221C4A8
	strh r0, [r4, #0x12]
	mov r0, #0
	strh r0, [r4, #0x14]
	ldr r0, [r4]
	cmp r0, #0
	beq _02223910
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223192C
	cmp r0, #4
	bne _02223910
	mov r0, #0xa
	ldrsh r1, [r4, r0]
	sub r0, #0xb
	mul r0, r1
	strh r0, [r4, #0xa]
	mov r1, #0xc
	add r0, r1, #0
	ldrsh r2, [r4, r1]
	sub r0, #0xd
	mul r0, r2
	strh r0, [r4, #0xc]
	mov r0, #0xe
	ldrsh r2, [r4, r0]
	sub r0, #0xf
	mul r0, r2
	strh r0, [r4, #0xe]
	mov r0, #0x10
	ldrsh r2, [r4, r0]
	sub r0, #0x11
	mul r0, r2
	strh r0, [r4, #0x10]
	ldrsh r0, [r4, r1]
	sub r0, #0x54
	strh r0, [r4, #0xc]
	b _02223918
_02223910:
	mov r0, #0xc
	ldrsh r0, [r4, r0]
	add r0, #0x54
	strh r0, [r4, #0xc]
_02223918:
	add r0, r5, #0
	bl ov07_0221BFC0
	cmp r0, #1
	bne _0222392C
	mov r0, #0xe
	ldrsh r1, [r4, r0]
	sub r0, #0xf
	mul r0, r1
	strh r0, [r4, #0xe]
_0222392C:
	mov r0, #4
	strh r0, [r4, #0x16]
	mov r0, #0x10
	strh r0, [r4, #0x18]
	mov r1, #0
	strh r1, [r4, #0x16]
	strh r0, [r4, #0x18]
	mov r0, #2
	str r1, [r4, #4]
	bl ToggleBgLayer
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	mov r1, #0
	add r7, r0, #0
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x30]
	mov r0, #7
	add r1, r6, #0
	mov r3, #2
	bl GfGfxLoader_LoadCharData
	add r0, r7, #0
	mov r1, #1
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	add r3, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #0x90
	str r0, [sp, #8]
	ldr r0, [r4, #0x34]
	mov r1, #7
	add r2, r6, #0
	bl PaletteData_LoadNarc
	ldr r0, [r4, #0x30]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	add r0, r5, #0
	mov r6, #2
	bl ov07_0221BFC0
	cmp r0, #1
	bne _022239B2
	mov r6, #4
	b _022239C6
_022239B2:
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223192C
	cmp r0, #4
	bne _022239C6
	mov r6, #3
_022239C6:
	add r0, r7, #0
	add r1, r6, #0
	bl ov07_0221FB7C
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x30]
	mov r0, #7
	add r1, r6, #0
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r3, #0xa
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #0
	bl BgSetPosTextAndCommit
	mov r3, #0xc
	ldrsh r3, [r4, r3]
	ldr r0, [r4, #0x30]
	mov r1, #2
	mov r2, #3
	bl BgSetPosTextAndCommit
	ldr r0, [r4, #0x20]
	ldr r1, _02223A18 ; =ov07_022236F0
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02223A18: .word ov07_022236F0
	thumb_func_end ov07_02223864

	thumb_func_start ov07_02223A1C
ov07_02223A1C: ; 0x02223A1C
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _02223A32
	cmp r0, #1
	beq _02223A32
	cmp r0, #2
	beq _02223A64
	pop {r3, r4, r5, pc}
_02223A32:
	add r0, r4, #0
	add r0, #0xc
	bl ov07_02222440
	cmp r0, #1
	bne _02223A4E
	ldr r2, [r4, #0xc]
	ldr r0, [r4, #8]
	lsl r2, r2, #0x10
	mov r1, #9
	lsr r2, r2, #0x10
	bl Pokepic_SetAttr
	pop {r3, r4, r5, pc}
_02223A4E:
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #0x20]
	ldr r3, [r4, #0x24]
	add r0, #0xc
	bl ov07_022223F0
	ldrb r0, [r4]
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, pc}
_02223A64:
	ldr r0, [r4, #8]
	mov r1, #0xa
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #0xb
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #9
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_02223A1C

	thumb_func_start ov07_02223A94
ov07_02223A94: ; 0x02223A94
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	bl ov07_0221BFD0
	mov r1, #0x2c
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	strb r0, [r4]
	str r5, [r4, #4]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [r4, #0x24]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	str r0, [r4, #0x20]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	add r7, r0, #0
	beq _02223ACE
	cmp r7, #2
	bne _02223AD8
_02223ACE:
	ldr r0, [r4, #4]
	bl ov07_0221C470
	str r0, [sp]
	b _02223AE0
_02223AD8:
	ldr r0, [r4, #4]
	bl ov07_0221C468
	str r0, [sp]
_02223AE0:
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r2, r0, #0
	add r0, r4, #0
	ldr r1, [r4, #0x20]
	ldr r3, [r4, #0x24]
	add r0, #0xc
	bl ov07_022223F0
	ldr r1, [sp]
	add r0, r5, #0
	bl ov07_02222004
	add r6, r0, #0
	ldr r0, [r4, #0x14]
	add r1, r0, #0
	mul r1, r6
	str r1, [r4, #0x14]
	cmp r7, #2
	bne _02223B12
	mov r0, #0
	mvn r0, r0
	mul r6, r0
_02223B12:
	ldr r0, [r4, #4]
	ldr r1, [sp]
	bl ov07_0221FA48
	str r0, [r4, #8]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	cmp r0, #1
	bne _02223B3C
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	strh r0, [r4, #0x28]
	add r0, r5, #0
	mov r1, #5
	bl ov07_0221C4A8
	b _02223B44
_02223B3C:
	mov r0, #0x28
	add r1, r6, #0
	mul r1, r0
	strh r1, [r4, #0x28]
_02223B44:
	strh r0, [r4, #0x2a]
	mov r2, #0x28
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #8]
	mov r1, #0xa
	bl Pokepic_SetAttr
	mov r2, #0x2a
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #8]
	mov r1, #0xb
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	ldr r1, _02223B6C ; =ov07_02223A1C
	add r2, r4, #0
	bl ov07_0221C410
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223B6C: .word ov07_02223A1C
	thumb_func_end ov07_02223A94

	thumb_func_start ov07_02223B70
ov07_02223B70: ; 0x02223B70
	push {r3, r4, r5, lr}
	add r4, r1, #0
	mov r1, #0x2c
	mov r2, #0x2e
	add r5, r0, #0
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	ldr r3, [r4, #0x28]
	add r0, r4, #4
	bl ov07_02222240
	cmp r0, #0
	bne _02223BBC
	mov r2, #0x2c
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x28]
	mov r1, #0
	bl Pokepic_SetAttr
	mov r2, #0x2e
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x28]
	mov r1, #1
	sub r2, #8
	bl Pokepic_SetAttr
	ldr r0, [r4, #0x28]
	mov r1, #9
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
_02223BBC:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_02223B70

	thumb_func_start ov07_02223BC0
ov07_02223BC0: ; 0x02223BC0
	push {r4, r5, r6, lr}
	add r5, r0, #0
	bl ov07_0221BFD0
	mov r1, #0x30
	bl Heap_Alloc
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	str r5, [r4]
	bl ov07_0221C4A8
	add r6, r0, #0
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r2, r0, #0
	add r0, r4, #4
	add r1, r6, #0
	bl ov07_02222A44
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	cmp r0, #8
	bgt _02223C0A
	cmp r0, #2
	blt _02223C7C
	beq _02223C10
	cmp r0, #4
	beq _02223C2C
	cmp r0, #8
	beq _02223C50
	b _02223C7C
_02223C0A:
	cmp r0, #0x10
	beq _02223C62
	b _02223C7C
_02223C10:
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	str r0, [r4, #0x28]
	mov r0, #0
	ldr r1, [r4, #0x10]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x10]
	b _02223C80
_02223C2C:
	ldr r0, [r4]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0223197C
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	str r0, [r4, #0x28]
	mov r0, #0
	ldr r1, [r4, #0x10]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x10]
	b _02223C80
_02223C50:
	add r0, r5, #0
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	str r0, [r4, #0x28]
	b _02223C80
_02223C62:
	ldr r0, [r4]
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0223197C
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	str r0, [r4, #0x28]
	b _02223C80
_02223C7C:
	bl GF_AssertFail
_02223C80:
	ldr r0, [r4, #0x28]
	mov r1, #0
	bl Pokepic_GetAttr
	strh r0, [r4, #0x2c]
	ldr r0, [r4, #0x28]
	mov r1, #1
	bl Pokepic_GetAttr
	strh r0, [r4, #0x2e]
	mov r0, #0x2e
	ldrsh r0, [r4, r0]
	ldr r1, _02223CA8 ; =ov07_02223B70
	add r2, r4, #0
	add r0, #8
	strh r0, [r4, #0x2e]
	ldr r0, [r4]
	bl ov07_0221C410
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02223CA8: .word ov07_02223B70
	thumb_func_end ov07_02223BC0

	thumb_func_start ov07_02223CAC
ov07_02223CAC: ; 0x02223CAC
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	ldr r0, [r5, #0x14]
	add r4, r1, #0
	mov r1, #1
	add r6, r2, #0
	bl ManagedSprite_SetDrawFlag
	ldr r0, [r5, #0x14]
	add r1, r4, #0
	add r2, r6, #0
	bl ManagedSprite_SetPositionXY
	mov r0, #0
	str r0, [r5, #0x40]
	mov r0, #4
	mov r1, #0xa
	add r5, #0x18
	str r0, [sp]
	add r0, r5, #0
	add r2, r1, #0
	mov r3, #0xe
	bl ov07_02222508
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov07_02223CAC

	thumb_func_start ov07_02223CE4
ov07_02223CE4: ; 0x02223CE4
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	ldr r1, [r4, #0x14]
	add r0, #0x18
	bl ov07_02222748
	cmp r0, #0
	bne _02223D20
	ldr r0, [r4, #0x40]
	cmp r0, #0
	beq _02223D0A
	ldr r0, [r4, #0x14]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	add sp, #4
	mov r0, #1
	pop {r3, r4, pc}
_02223D0A:
	mov r0, #1
	str r0, [r4, #0x40]
	mov r0, #2
	add r4, #0x18
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0xe
	mov r2, #0xa
	mov r3, #0xc
	bl ov07_02222508
_02223D20:
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end ov07_02223CE4

	thumb_func_start ov07_02223D28
ov07_02223D28: ; 0x02223D28
	push {r3, r4, r5, lr}
	add r4, r0, #0
	ldr r1, [r4, #0x48]
	mov r5, #0
	cmp r1, #4
	bhi _02223DC0
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02223D40: ; jump table
	.short _02223D4A - _02223D40 - 2 ; case 0
	.short _02223D70 - _02223D40 - 2 ; case 1
	.short _02223D80 - _02223D40 - 2 ; case 2
	.short _02223DAE - _02223D40 - 2 ; case 3
	.short _02223DBE - _02223D40 - 2 ; case 4
_02223D4A:
	mov r1, #0x4c
	ldrsh r3, [r4, r1]
	ldr r2, [r4, #0x3c]
	mov r1, #0x18
	mul r1, r2
	mov r2, #0x4e
	ldrsh r2, [r4, r2]
	add r1, r3, r1
	lsl r1, r1, #0x10
	sub r2, #0x10
	lsl r2, r2, #0x10
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	bl ov07_02223CAC
	ldr r0, [r4, #0x48]
	add r0, r0, #1
	str r0, [r4, #0x48]
	b _02223DC0
_02223D70:
	bl ov07_02223CE4
	cmp r0, #0
	beq _02223DC0
	ldr r0, [r4, #0x48]
	add r0, r0, #1
	str r0, [r4, #0x48]
	b _02223DC0
_02223D80:
	ldr r1, [r4, #0x44]
	sub r1, r1, #1
	str r1, [r4, #0x44]
	bpl _02223DC0
	ldr r1, [r4, #0x48]
	add r1, r1, #1
	str r1, [r4, #0x48]
	mov r1, #0x4c
	ldrsh r3, [r4, r1]
	ldr r2, [r4, #0x3c]
	mov r1, #0x18
	mul r1, r2
	mov r2, #0x4e
	ldrsh r2, [r4, r2]
	sub r1, r3, r1
	lsl r1, r1, #0x10
	sub r2, #0x18
	lsl r2, r2, #0x10
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	bl ov07_02223CAC
	b _02223DC0
_02223DAE:
	bl ov07_02223CE4
	cmp r0, #0
	beq _02223DC0
	ldr r0, [r4, #0x48]
	add r0, r0, #1
	str r0, [r4, #0x48]
	b _02223DC0
_02223DBE:
	mov r5, #1
_02223DC0:
	add r0, r5, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov07_02223D28

	thumb_func_start ov07_02223DC4
ov07_02223DC4: ; 0x02223DC4
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0
	beq _02223DD6
	cmp r0, #1
	beq _02223DE8
	b _02223DFE
_02223DD6:
	add r0, r4, #0
	bl ov07_02223D28
	cmp r0, #0
	beq _02223DFE
	ldr r0, [r4, #0xc]
	add r0, r0, #1
	str r0, [r4, #0xc]
	b _02223DFE
_02223DE8:
	ldr r0, [r4, #0x14]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, pc}
_02223DFE:
	ldr r0, [r4, #8]
	bl SpriteSystem_DrawSprites
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_02223DC4

	thumb_func_start ov07_02223E08
ov07_02223E08: ; 0x02223E08
	push {r3, r4, r5, r6, r7, lr}
	add r7, r1, #0
	mov r1, #0x50
	add r5, r0, #0
	str r2, [sp]
	add r6, r3, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r5, [r4]
	ldr r0, [sp]
	str r7, [r4, #4]
	str r0, [r4, #8]
	str r6, [r4, #0x14]
	add r0, r6, #0
	mov r1, #0x64
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x14]
	mov r1, #1
	bl ManagedSprite_SetPriority
	ldr r0, [r4, #0x14]
	mov r1, #2
	bl ManagedSprite_SetAffineOverwriteMode
	ldr r0, [r4, #0x14]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	mov r0, #4
	str r0, [r4, #0x44]
	ldr r0, [r4]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_02222004
	str r0, [r4, #0x3c]
	add r0, r5, #0
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	mov r1, #0
	add r5, r0, #0
	bl Pokepic_GetAttr
	add r1, r4, #0
	add r1, #0x4c
	strh r0, [r1]
	add r0, r5, #0
	mov r1, #1
	bl Pokepic_GetAttr
	add r1, r4, #0
	add r1, #0x4e
	strh r0, [r1]
	ldr r0, [r4]
	ldr r1, _02223E90 ; =ov07_02223DC4
	add r2, r4, #0
	bl ov07_0221C410
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223E90: .word ov07_02223DC4
	thumb_func_end ov07_02223E08

	thumb_func_start ov07_02223E94
ov07_02223E94: ; 0x02223E94
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #4]
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _02223EB2
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
_02223EB2:
	pop {r3, r4, r5, pc}
	thumb_func_end ov07_02223E94

	thumb_func_start ov07_02223EB4
ov07_02223EB4: ; 0x02223EB4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	mov r1, #8
	add r5, r0, #0
	bl ov07_022324D8
	add r6, r0, #0
	str r5, [r6]
	add r0, r5, #0
	bl ov07_0221FA78
	str r0, [r6, #4]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	cmp r0, #0
	beq _02223EE2
	cmp r0, #1
	beq _02223EEE
	cmp r0, #2
	beq _02223EF4
	b _02223EFA
_02223EE2:
	add r0, r5, #0
	bl ov07_0221E6C8
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	b _02223EFE
_02223EEE:
	mov r4, #1
	lsl r4, r4, #8
	b _02223EFE
_02223EF4:
	mov r4, #2
	lsl r4, r4, #8
	b _02223EFE
_02223EFA:
	bl GF_AssertFail
_02223EFE:
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r7, r0, #0
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [sp, #0x10]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	ldr r1, [sp, #0xc]
	lsl r0, r0, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp]
	ldr r1, [sp, #0x10]
	lsl r3, r7, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r0, [r6, #4]
	mov r1, #1
	add r2, r4, #0
	asr r3, r3, #0x18
	bl PaletteData_BeginPaletteFade
	ldr r0, [r6]
	ldr r1, _02223F58 ; =ov07_02223E94
	add r2, r6, #0
	bl ov07_0221C410
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02223F58: .word ov07_02223E94
	thumb_func_end ov07_02223EB4

	thumb_func_start ov07_02223F5C
ov07_02223F5C: ; 0x02223F5C
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #5
	bhi _02224004
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02223F74: ; jump table
	.short _02223F80 - _02223F74 - 2 ; case 0
	.short _02223F98 - _02223F74 - 2 ; case 1
	.short _02223FAA - _02223F74 - 2 ; case 2
	.short _02223FBC - _02223F74 - 2 ; case 3
	.short _02223FD4 - _02223F74 - 2 ; case 4
	.short _02223FF6 - _02223F74 - 2 ; case 5
_02223F80:
	ldr r0, [r4, #0x14]
	mov r1, #0
	str r0, [sp]
	ldr r0, [r4, #4]
	ldr r2, [r4, #0x18]
	ldr r3, [r4, #0x10]
	bl Pokepic_StartPaletteFade
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
_02223F98:
	ldr r0, [r4, #4]
	bl Pokepic_ResumePaletteFade
	cmp r0, #0
	bne _02224004
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
_02223FAA:
	ldr r1, [r4, #0x1c]
	cmp r1, #0
	bne _02223FB6
	add r0, r0, #1
	str r0, [r4, #8]
	b _02223FBC
_02223FB6:
	sub r0, r1, #1
	str r0, [r4, #0x1c]
	pop {r3, r4, r5, pc}
_02223FBC:
	ldr r0, [r4, #0x14]
	mov r2, #0
	str r0, [sp]
	ldr r0, [r4, #4]
	ldr r1, [r4, #0x18]
	ldr r3, [r4, #0x10]
	bl Pokepic_StartPaletteFade
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
_02223FD4:
	ldr r0, [r4, #4]
	bl Pokepic_ResumePaletteFade
	cmp r0, #0
	bne _02224004
	ldr r0, [r4, #0xc]
	sub r0, r0, #1
	str r0, [r4, #0xc]
	cmp r0, #0
	bgt _02223FF0
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
_02223FF0:
	mov r0, #0
	str r0, [r4, #8]
	pop {r3, r4, r5, pc}
_02223FF6:
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
_02224004:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_02223F5C

	thumb_func_start ov07_02224008
ov07_02224008: ; 0x02224008
	push {r3, r4, r5, r6, r7, lr}
	mov r1, #0x20
	add r4, r0, #0
	bl ov07_022324D8
	add r6, r0, #0
	str r4, [r6]
	mov r1, #0
	add r0, r4, #0
	str r1, [r6, #4]
	bl ov07_0221C4A8
	ldr r2, _022241D0 ; =0x00000802
	cmp r0, r2
	bgt _02224042
	bge _022240C8
	cmp r0, #8
	bgt _0222403C
	cmp r0, #2
	blt _0222403A
	beq _0222405A
	cmp r0, #4
	beq _0222406C
	cmp r0, #8
	beq _02224092
_0222403A:
	b _02224180
_0222403C:
	cmp r0, #0x10
	beq _022240A4
	b _02224180
_02224042:
	add r1, r2, #6
	cmp r0, r1
	bgt _02224052
	bge _02224124
	add r1, r2, #2
	cmp r0, r1
	beq _022240F8
	b _02224180
_02224052:
	add r2, #0xe
	cmp r0, r2
	beq _02224154
	b _02224180
_0222405A:
	add r0, r4, #0
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_0222406C:
	add r0, r4, #0
	bl ov07_0221FAB0
	cmp r0, #1
	beq _02224078
	b _02224184
_02224078:
	ldr r0, [r6]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0223197C
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_02224092:
	add r0, r4, #0
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_022240A4:
	add r0, r4, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _02224184
	ldr r0, [r6]
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0223197C
	add r1, r0, #0
	ldr r0, [r6]
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_022240C8:
	mov r7, #0xff
	mov r5, #0
_022240CC:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #0
	beq _022240DC
	cmp r0, #2
	bne _022240E0
_022240DC:
	add r7, r5, #0
	b _022240E6
_022240E0:
	add r5, r5, #1
	cmp r5, #4
	blt _022240CC
_022240E6:
	cmp r7, #0xff
	bne _022240EC
	mov r7, #0
_022240EC:
	ldr r0, [r6]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_022240F8:
	mov r7, #0xff
	mov r5, #0
_022240FC:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #4
	bne _0222410C
	add r7, r5, #0
	b _02224112
_0222410C:
	add r5, r5, #1
	cmp r5, #4
	blt _022240FC
_02224112:
	cmp r7, #0xff
	bne _02224118
	mov r7, #0
_02224118:
	ldr r0, [r6]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_02224124:
	mov r7, #0xff
	mov r5, #0
_02224128:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #1
	beq _02224138
	cmp r0, #3
	bne _0222413C
_02224138:
	add r7, r5, #0
	b _02224142
_0222413C:
	add r5, r5, #1
	cmp r5, #4
	blt _02224128
_02224142:
	cmp r7, #0xff
	bne _02224148
	mov r7, #0
_02224148:
	ldr r0, [r6]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_02224154:
	mov r7, #0xff
	mov r5, #0
_02224158:
	add r0, r4, #0
	add r1, r5, #0
	bl ov07_0221FA04
	cmp r0, #5
	bne _02224168
	add r7, r5, #0
	b _0222416E
_02224168:
	add r5, r5, #1
	cmp r5, #4
	blt _02224158
_0222416E:
	cmp r7, #0xff
	bne _02224174
	mov r7, #0
_02224174:
	ldr r0, [r6]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r6, #4]
	b _02224184
_02224180:
	bl GF_AssertFail
_02224184:
	ldr r0, [r6, #4]
	cmp r0, #0
	bne _02224192
	add r0, r6, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
_02224192:
	add r0, r4, #0
	mov r1, #1
	bl ov07_0221C4A8
	str r0, [r6, #0x10]
	add r0, r4, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [r6, #0xc]
	add r0, r4, #0
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [r6, #0x14]
	add r0, r4, #0
	mov r1, #4
	bl ov07_0221C4A8
	str r0, [r6, #0x18]
	add r0, r4, #0
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r6, #0x1c]
	ldr r0, [r6]
	ldr r1, _022241D4 ; =ov07_02223F5C
	add r2, r6, #0
	bl ov07_0221C410
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022241D0: .word 0x00000802
_022241D4: .word ov07_02223F5C
	thumb_func_end ov07_02224008

	thumb_func_start ov07_022241D8
ov07_022241D8: ; 0x022241D8
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #4
	bls _022241E8
	b _0222433C
_022241E8:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022241F4: ; jump table
	.short _022241FE - _022241F4 - 2 ; case 0
	.short _0222424C - _022241F4 - 2 ; case 1
	.short _0222429E - _022241F4 - 2 ; case 2
	.short _022242C8 - _022241F4 - 2 ; case 3
	.short _0222432A - _022241F4 - 2 ; case 4
_022241FE:
	ldr r0, [r4, #0x4c]
	asr r0, r0, #0x10
	str r0, [sp]
	ldr r1, [r4, #0x3c]
	ldr r2, [r4, #0x44]
	ldr r3, [r4, #0x40]
	add r0, r4, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	add r0, #0x14
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222508
	add r0, r4, #0
	add r0, #0x14
	add r1, sp, #8
	add r2, sp, #4
	bl ov07_02222644
	ldr r0, [r4, #0x50]
	cmp r0, #1
	bne _0222423A
	ldr r1, [sp, #8]
	mov r0, #0
	bl _fsub
	str r0, [sp, #8]
_0222423A:
	ldr r0, [r4, #0x10]
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	bl ManagedSprite_SetAffineScale
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0222433C
_0222424C:
	add r0, r4, #0
	add r0, #0x14
	bl ov07_02222558
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0x14
	add r1, sp, #8
	add r2, sp, #4
	bl ov07_02222644
	ldr r0, [r4, #0x50]
	cmp r0, #1
	bne _02224272
	ldr r1, [sp, #8]
	mov r0, #0
	bl _fsub
	str r0, [sp, #8]
_02224272:
	ldr r0, [r4, #0x10]
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	bl ManagedSprite_SetAffineScale
	cmp r5, #0
	beq _02224296
	mov r0, #0
	str r0, [sp]
	mov r1, #0xc
	mov r2, #0xe
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	ldr r3, [r4, #0x28]
	bl ov07_022226FC
	b _0222433C
_02224296:
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0222433C
_0222429E:
	ldr r0, [r4, #0x4c]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r1, [r4, #0x40]
	ldr r2, [r4, #0x44]
	ldr r3, [r4, #0x3c]
	add r0, r4, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	add r0, #0x14
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222508
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0222433C
_022242C8:
	add r0, r4, #0
	add r0, #0x14
	bl ov07_02222558
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0x14
	add r1, sp, #8
	add r2, sp, #4
	bl ov07_02222644
	ldr r0, [r4, #0x50]
	cmp r0, #1
	bne _022242EE
	ldr r1, [sp, #8]
	mov r0, #0
	bl _fsub
	str r0, [sp, #8]
_022242EE:
	ldr r0, [r4, #0x10]
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	bl ManagedSprite_SetAffineScale
	cmp r5, #0
	beq _02224312
	mov r0, #0
	str r0, [sp]
	mov r1, #0xc
	mov r2, #0xe
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	ldr r3, [r4, #0x28]
	bl ov07_022226FC
	b _0222433C
_02224312:
	ldr r0, [r4, #0x48]
	sub r0, r0, #1
	str r0, [r4, #0x48]
	cmp r0, #0
	bgt _02224324
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0222433C
_02224324:
	mov r0, #0
	str r0, [r4, #8]
	b _0222433C
_0222432A:
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
	add sp, #0xc
	pop {r4, r5, pc}
_0222433C:
	ldr r0, [r4, #4]
	bl SpriteSystem_DrawSprites
	add sp, #0xc
	pop {r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_022241D8

	thumb_func_start ov07_02224348
ov07_02224348: ; 0x02224348
	push {r4, r5, r6, lr}
	mov r1, #0x54
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r5, [r4]
	add r0, r5, #0
	bl ov07_0221C514
	str r0, [r4, #4]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	cmp r0, #0
	bne _0222438A
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	add r6, r0, #0
	ldr r0, [r4]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FAA0
	b _022243A8
_0222438A:
	ldr r0, [r4]
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FA48
	add r6, r0, #0
	ldr r0, [r4]
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FAA0
_022243A8:
	strh r0, [r4, #0xe]
	add r0, r6, #0
	mov r1, #1
	bl Pokepic_GetAttr
	strh r0, [r4, #0xc]
	add r0, r6, #0
	mov r1, #0x29
	bl Pokepic_GetAttr
	mov r1, #0xc
	ldrsh r1, [r4, r1]
	sub r0, r1, r0
	strh r0, [r4, #0xc]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	str r0, [r4, #0x38]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [r4, #0x3c]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [r4, #0x40]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	str r0, [r4, #0x44]
	add r0, r5, #0
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r4, #0x48]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	str r0, [r4, #0x4c]
	add r0, r5, #0
	mov r1, #7
	bl ov07_0221C4A8
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221C4E8
	str r0, [r4, #0x10]
	mov r1, #0x64
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x10]
	mov r1, #1
	bl ManagedSprite_SetPriority
	ldr r0, [r4, #0x10]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r4, #0x10]
	mov r1, #2
	bl ManagedSprite_SetAffineOverwriteMode
	ldr r1, [r4, #0x38]
	mov r2, #0x10
	ldr r0, [r4]
	sub r2, r2, r1
	bl ov07_02231E08
	ldr r0, [r4]
	ldr r1, _02224468 ; =ov07_022241D8
	add r2, r4, #0
	bl ov07_0221C410
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	cmp r0, #0
	beq _02224456
	mov r1, #1
	b _02224458
_02224456:
	mov r1, #0
_02224458:
	ldr r0, [r4]
	bl ov07_0221FB78
	cmp r0, #1
	bne _02224466
	mov r0, #1
	str r0, [r4, #0x50]
_02224466:
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02224468: .word ov07_022241D8
	thumb_func_end ov07_02224348

	thumb_func_start ov07_0222446C
ov07_0222446C: ; 0x0222446C
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #4]
	cmp r0, #5
	bls _0222447C
	b _022245B8
_0222447C:
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02224488: ; jump table
	.short _02224494 - _02224488 - 2 ; case 0
	.short _022244D8 - _02224488 - 2 ; case 1
	.short _02224508 - _02224488 - 2 ; case 2
	.short _0222451C - _02224488 - 2 ; case 3
	.short _02224562 - _02224488 - 2 ; case 4
	.short _022245A4 - _02224488 - 2 ; case 5
_02224494:
	ldr r0, [r4, #0x40]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp]
	ldr r0, [r4, #0x44]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #4]
	ldr r0, [r4, #0x4c]
	asr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r1, [r4, #0x34]
	ldr r2, [r4, #0x38]
	ldr r3, [r4, #0x3c]
	add r0, r4, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	add r0, #0x10
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222590
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	add r0, #0x10
	bl ov07_02222768
	ldr r0, [r4, #4]
	add sp, #0xc
	add r0, r0, #1
	str r0, [r4, #4]
	pop {r4, r5, pc}
_022244D8:
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	add r0, #0x10
	bl ov07_02222788
	cmp r0, #0
	beq _022244FE
	mov r0, #0
	str r0, [sp]
	mov r1, #8
	mov r2, #0xa
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0xc]
	ldr r3, [r4, #0x24]
	bl ov07_022226C4
	add sp, #0xc
	pop {r4, r5, pc}
_022244FE:
	ldr r0, [r4, #4]
	add sp, #0xc
	add r0, r0, #1
	str r0, [r4, #4]
	pop {r4, r5, pc}
_02224508:
	ldr r1, [r4, #0x50]
	cmp r1, #0
	bne _02224514
	add r0, r0, #1
	str r0, [r4, #4]
	b _0222451C
_02224514:
	sub r0, r1, #1
	add sp, #0xc
	str r0, [r4, #0x50]
	pop {r4, r5, pc}
_0222451C:
	ldr r0, [r4, #0x3c]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp]
	ldr r0, [r4, #0x44]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #4]
	ldr r0, [r4, #0x4c]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r1, [r4, #0x38]
	ldr r2, [r4, #0x34]
	ldr r3, [r4, #0x40]
	add r0, r4, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	add r0, #0x10
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222590
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	add r0, #0x10
	bl ov07_02222788
	ldr r0, [r4, #4]
	add sp, #0xc
	add r0, r0, #1
	str r0, [r4, #4]
	pop {r4, r5, pc}
_02224562:
	add r0, r4, #0
	ldr r1, [r4, #0xc]
	add r0, #0x10
	bl ov07_02222788
	cmp r0, #0
	beq _02224588
	mov r0, #0
	str r0, [sp]
	mov r1, #8
	mov r2, #0xa
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0xc]
	ldr r3, [r4, #0x24]
	bl ov07_022226C4
	add sp, #0xc
	pop {r4, r5, pc}
_02224588:
	ldr r0, [r4, #0x48]
	sub r0, r0, #1
	str r0, [r4, #0x48]
	cmp r0, #0
	bgt _0222459C
	ldr r0, [r4, #4]
	add sp, #0xc
	add r0, r0, #1
	str r0, [r4, #4]
	pop {r4, r5, pc}
_0222459C:
	mov r0, #0
	add sp, #0xc
	str r0, [r4, #4]
	pop {r4, r5, pc}
_022245A4:
	ldr r0, [r4, #0xc]
	bl ov07_02223224
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
_022245B8:
	add sp, #0xc
	pop {r4, r5, pc}
	thumb_func_end ov07_0222446C

	thumb_func_start ov07_022245BC
ov07_022245BC: ; 0x022245BC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x54
	add r5, r0, #0
	mov r1, #0x54
	bl ov07_022324D8
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	str r5, [r4]
	bl ov07_0221C4A8
	ldr r2, _022247C0 ; =0x00000802
	cmp r0, r2
	bgt _02224600
	bge _0222466C
	mov r2, #0x42
	lsl r2, r2, #2
	cmp r0, r2
	bgt _022245F8
	sub r1, r2, #6
	cmp r0, r1
	blt _022245F6
	beq _02224618
	sub r1, r2, #4
	cmp r0, r1
	beq _02224618
	cmp r0, r2
	beq _02224642
_022245F6:
	b _02224722
_022245F8:
	add r2, #8
	cmp r0, r2
	beq _02224642
	b _02224722
_02224600:
	add r1, r2, #6
	cmp r0, r1
	bgt _02224610
	bge _022246C8
	add r1, r2, #2
	cmp r0, r1
	beq _0222469C
	b _02224722
_02224610:
	add r2, #0xe
	cmp r0, r2
	beq _022246F8
	b _02224722
_02224618:
	ldr r0, [r4]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FAA0
	strh r0, [r4, #0xa]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r1, r0, #0
	ldr r0, [r4]
	add r2, sp, #4
	add r3, sp, #0
	bl ov07_02232020
	ldr r0, [sp, #0xc]
	str r0, [r4, #0xc]
	b _02224722
_02224642:
	ldr r0, [r4]
	bl ov07_0221C470
	add r1, r0, #0
	ldr r0, [r4]
	bl ov07_0221FAA0
	strh r0, [r4, #0xa]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r1, r0, #0
	ldr r0, [r4]
	add r2, sp, #4
	add r3, sp, #0
	bl ov07_02232020
	ldr r0, [sp, #0xc]
	str r0, [r4, #0xc]
	b _02224722
_0222466C:
	mov r7, #0xff
	mov r6, #0
_02224670:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA04
	cmp r0, #0
	beq _02224680
	cmp r0, #2
	bne _02224684
_02224680:
	add r7, r6, #0
	b _0222468A
_02224684:
	add r6, r6, #1
	cmp r6, #4
	blt _02224670
_0222468A:
	cmp r7, #0xff
	bne _02224690
	mov r7, #0
_02224690:
	ldr r0, [r4]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r4, #0xc]
	b _02224722
_0222469C:
	mov r7, #0xff
	mov r6, #0
_022246A0:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA04
	cmp r0, #4
	bne _022246B0
	add r7, r6, #0
	b _022246B6
_022246B0:
	add r6, r6, #1
	cmp r6, #4
	blt _022246A0
_022246B6:
	cmp r7, #0xff
	bne _022246BC
	mov r7, #0
_022246BC:
	ldr r0, [r4]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r4, #0xc]
	b _02224722
_022246C8:
	mov r7, #0xff
	mov r6, #0
_022246CC:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA04
	cmp r0, #1
	beq _022246DC
	cmp r0, #3
	bne _022246E0
_022246DC:
	add r7, r6, #0
	b _022246E6
_022246E0:
	add r6, r6, #1
	cmp r6, #4
	blt _022246CC
_022246E6:
	cmp r7, #0xff
	bne _022246EC
	mov r7, #0
_022246EC:
	ldr r0, [r4]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r4, #0xc]
	b _02224722
_022246F8:
	mov r7, #0xff
	mov r6, #0
_022246FC:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA04
	cmp r0, #5
	bne _0222470C
	add r7, r6, #0
	b _02224712
_0222470C:
	add r6, r6, #1
	cmp r6, #4
	blt _022246FC
_02224712:
	cmp r7, #0xff
	bne _02224718
	mov r7, #0
_02224718:
	ldr r0, [r4]
	add r1, r7, #0
	bl ov07_0221FA48
	str r0, [r4, #0xc]
_02224722:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	bne _02224732
	add r0, r4, #0
	bl Heap_Free
	add sp, #0x54
	pop {r4, r5, r6, r7, pc}
_02224732:
	mov r1, #1
	bl Pokepic_GetAttr
	strh r0, [r4, #8]
	ldr r0, [r4, #0xc]
	mov r1, #0x29
	bl Pokepic_GetAttr
	strh r0, [r4, #0xa]
	mov r0, #0xa
	ldrsh r0, [r4, r0]
	mov r1, #1
	add r0, #0x10
	strh r0, [r4, #0xa]
	add r0, r5, #0
	bl ov07_0221C4A8
	str r0, [r4, #0x34]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [r4, #0x38]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [r4, #0x3c]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	str r0, [r4, #0x40]
	add r0, r5, #0
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r4, #0x44]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	str r0, [r4, #0x48]
	add r0, r5, #0
	mov r1, #7
	bl ov07_0221C4A8
	str r0, [r4, #0x4c]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [r4, #0x48]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	asr r0, r0, #0x10
	str r0, [r4, #0x50]
	ldr r0, [r4]
	ldr r1, _022247C4 ; =ov07_0222446C
	add r2, r4, #0
	bl ov07_0221C410
	add r1, r4, #0
	bl ov07_0222446C
	add sp, #0x54
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022247C0: .word 0x00000802
_022247C4: .word ov07_0222446C
	thumb_func_end ov07_022245BC

	thumb_func_start ov07_022247C8
ov07_022247C8: ; 0x022247C8
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	add r0, #8
	bl ov07_02222AF4
	cmp r0, #0
	beq _022247EA
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C448
	pop {r3, r4, r5, pc}
_022247EA:
	ldr r0, [r4, #4]
	bl SpriteSystem_DrawSprites
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_022247C8

	thumb_func_start ov07_022247F4
ov07_022247F4: ; 0x022247F4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	mov r1, #0x30
	add r7, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r7, [r4]
	add r0, r7, #0
	bl ov07_0221C514
	str r0, [r4, #4]
	add r0, r7, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r6, r0, #0
	mov r5, #0
_02224818:
	mov r0, #1
	lsl r0, r5
	tst r0, r6
	beq _0222482E
	ldr r0, [r4]
	add r1, r5, #0
	bl ov07_0221C4E8
	mov r1, #1
	bl ManagedSprite_SetOamMode
_0222482E:
	add r5, r5, #1
	cmp r5, #4
	blt _02224818
	add r0, r7, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r5, r0, #0
	add r0, r7, #0
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [sp, #8]
	ldr r0, [r4]
	ldr r2, [sp, #8]
	add r1, r5, #0
	bl ov07_02231E08
	add r0, r7, #0
	mov r1, #2
	bl ov07_0221C4A8
	add r6, r0, #0
	add r0, r7, #0
	mov r1, #4
	bl ov07_0221C4A8
	str r0, [sp, #0xc]
	add r0, r7, #0
	mov r1, #5
	bl ov07_0221C4A8
	ldr r1, [sp, #0xc]
	ldr r3, [sp, #8]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	str r1, [sp]
	str r0, [sp, #4]
	add r0, r4, #0
	lsl r1, r5, #0x10
	lsl r2, r6, #0x10
	lsl r3, r3, #0x10
	add r0, #8
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222AC4
	ldr r0, [r4]
	ldr r1, _0222489C ; =ov07_022247C8
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222489C: .word ov07_022247C8
	thumb_func_end ov07_022247F4

	thumb_func_start ov07_022248A0
ov07_022248A0: ; 0x022248A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r6, r0, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r5, r0, #0
	add r0, r6, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r1, r0, #0
	add r0, r6, #0
	add r2, sp, #4
	add r3, sp, #0
	bl ov07_02232020
	ldr r0, [sp]
	mov r4, #0
	cmp r0, #0
	ble _022248EE
	mov r7, #6
_022248CC:
	ldr r1, [sp, #0x14]
	add r0, r6, #0
	bl ov07_0221FAC8
	cmp r0, #1
	bne _022248DC
	cmp r5, #0
	beq _022248E6
_022248DC:
	ldr r0, [sp, #0xc]
	add r1, r7, #0
	add r2, r5, #0
	bl Pokepic_SetAttr
_022248E6:
	ldr r0, [sp]
	add r4, r4, #1
	cmp r4, r0
	blt _022248CC
_022248EE:
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov07_022248A0

	thumb_func_start ov07_022248F4
ov07_022248F4: ; 0x022248F4
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r1, [r4, #4]
	ldr r0, [r4, #8]
	cmp r1, r0
	blt _02224942
	ldr r0, [r4]
	cmp r0, #0
	ldr r0, [r4, #0x14]
	bne _02224922
	mov r1, #6
	mov r2, #0
	bl Pokepic_SetAttr
	add r0, r4, #0
	bl Heap_Free
	ldr r0, [r4, #0x3c]
	add r1, r5, #0
	bl ov07_0221C448
	pop {r3, r4, r5, pc}
_02224922:
	mov r1, #6
	bl Pokepic_GetAttr
	add r3, r0, #0
	mov r2, #1
	ldr r0, [r4, #0x14]
	mov r1, #6
	eor r2, r3
	bl Pokepic_SetAttr
	ldr r0, [r4]
	sub r0, r0, #1
	str r0, [r4]
	mov r0, #0
	str r0, [r4, #4]
	pop {r3, r4, r5, pc}
_02224942:
	add r0, r1, #1
	str r0, [r4, #4]
	pop {r3, r4, r5, pc}
	thumb_func_end ov07_022248F4

	thumb_func_start ov07_02224948
ov07_02224948: ; 0x02224948
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r1, #0x40
	bl ov07_022324D8
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	str r5, [r4, #0x3c]
	bl ov07_0221C4A8
	lsl r0, r0, #1
	str r0, [r4]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r1, r4, #0
	str r0, [r4, #8]
	add r0, r5, #0
	add r1, #0x20
	bl ov07_02231FE4
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0221FA48
	str r0, [r4, #0x14]
	ldr r0, [r4, #0x3c]
	ldr r1, _02224994 ; =ov07_022248F4
	add r2, r4, #0
	bl ov07_0221C410
	pop {r3, r4, r5, pc}
	nop
_02224994: .word ov07_022248F4
	thumb_func_end ov07_02224948

	thumb_func_start ov07_02224998
ov07_02224998: ; 0x02224998
	push {r4, lr}
	sub sp, #8
	add r4, r1, #0
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _022249B0
	cmp r0, #1
	beq _022249E4
	b _02224A0E
_022249B0:
	mov r0, #0x16
	ldrsh r3, [r4, r0]
	mov r0, #0x14
	ldrsh r1, [r4, r0]
	mov r2, #0xc
	str r3, [sp]
	ldr r0, [r4, #8]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	ldrsh r2, [r4, r2]
	add r0, r4, #0
	add r0, #0x40
	add r2, r1, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_02222268
	add r0, r4, #0
	add r0, #0x24
	ldrb r0, [r0]
	add r4, #0x24
	add sp, #8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, pc}
_022249E4:
	add r0, r4, #0
	add r0, #0x40
	bl ov07_022222B4
	cmp r0, #0
	bne _022249FE
	add r0, r4, #0
	add r0, #0x24
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x24
	strb r1, [r0]
_022249FE:
	mov r2, #0x40
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x18]
	mov r1, #0
	bl Pokepic_SetAttr
	add sp, #8
	pop {r4, pc}
_02224A0E:
	ldr r0, [r4, #0x64]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	add sp, #8
	pop {r4, pc}
	thumb_func_end ov07_02224998

	thumb_func_start ov07_02224A20
ov07_02224A20: ; 0x02224A20
	push {r4, r5, r6, lr}
	mov r1, #0x68
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r5, [r4, #0x64]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	str r0, [r4, #8]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	strh r0, [r4, #0xc]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	mov r6, #0xff
	add r1, r6, #0
	add r1, #9
	cmp r0, r1
	bgt _02224A6C
	add r1, r6, #3
	cmp r0, r1
	blt _02224AC2
	beq _02224A76
	add r1, r6, #5
	cmp r0, r1
	beq _02224A80
	add r1, r6, #0
	add r1, #9
	cmp r0, r1
	beq _02224A9C
	b _02224AC2
_02224A6C:
	add r1, r6, #0
	add r1, #0x11
	cmp r0, r1
	beq _02224AA6
	b _02224AC2
_02224A76:
	add r0, r5, #0
	bl ov07_0221C468
	add r6, r0, #0
	b _02224AC6
_02224A80:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _02224AC6
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _02224AC6
_02224A9C:
	add r0, r5, #0
	bl ov07_0221C470
	add r6, r0, #0
	b _02224AC6
_02224AA6:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _02224AC6
	add r0, r5, #0
	bl ov07_0221C470
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _02224AC6
_02224AC2:
	bl GF_AssertFail
_02224AC6:
	cmp r6, #0xff
	bne _02224AD2
	add r0, r4, #0
	bl Heap_Free
	pop {r4, r5, r6, pc}
_02224AD2:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x24
	bl ov07_02231FE4
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA48
	add r1, r4, #0
	add r1, #0x14
	str r0, [r4, #0x18]
	bl ov07_02231FA0
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_02222004
	cmp r0, #0
	bgt _02224B04
	mov r0, #0xc
	ldrsh r1, [r4, r0]
	sub r0, #0xd
	mul r0, r1
	strh r0, [r4, #0xc]
_02224B04:
	ldr r0, [r4, #0x64]
	ldr r1, _02224B10 ; =ov07_02224998
	add r2, r4, #0
	bl ov07_0221C410
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02224B10: .word ov07_02224998
	thumb_func_end ov07_02224A20

	thumb_func_start ov07_02224B14
ov07_02224B14: ; 0x02224B14
	push {r4, lr}
	sub sp, #8
	add r4, r1, #0
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _02224B2C
	cmp r0, #1
	beq _02224B6E
	b _02224B98
_02224B2C:
	ldr r0, [r4, #0x14]
	cmp r0, #0
	bne _02224B3C
	mov r0, #0xff
	add r4, #0x20
	add sp, #8
	strb r0, [r4]
	pop {r4, pc}
_02224B3C:
	mov r0, #0x12
	ldrsh r3, [r4, r0]
	mov r0, #0x10
	ldrsh r1, [r4, r0]
	mov r2, #0x64
	str r3, [sp]
	ldr r0, [r4, #8]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	ldrsh r2, [r4, r2]
	add r0, r4, #0
	add r0, #0x3c
	add r2, r1, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_02222268
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x20
	strb r1, [r0]
_02224B6E:
	add r0, r4, #0
	add r0, #0x3c
	bl ov07_022222B4
	cmp r0, #0
	bne _02224B88
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x20
	strb r1, [r0]
_02224B88:
	mov r2, #0x3c
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x14]
	mov r1, #0
	bl Pokepic_SetAttr
	add sp, #8
	pop {r4, pc}
_02224B98:
	ldr r0, [r4, #0x60]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	add sp, #8
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov07_02224B14

	thumb_func_start ov07_02224BAC
ov07_02224BAC: ; 0x02224BAC
	push {r3, r4, r5, r6, r7, lr}
	mov r1, #0x68
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r5, [r4, #0x60]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	str r0, [r4, #8]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r1, r4, #0
	add r1, #0x64
	strh r0, [r1]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	add r1, r4, #0
	add r7, r0, #0
	add r0, r5, #0
	add r1, #0x20
	bl ov07_02231FE4
	mov r6, #0xff
	add r0, r6, #0
	add r0, #9
	cmp r7, r0
	bgt _02224C08
	add r0, r6, #3
	cmp r7, r0
	blt _02224C5E
	beq _02224C12
	add r0, r6, #5
	cmp r7, r0
	beq _02224C1C
	add r0, r6, #0
	add r0, #9
	cmp r7, r0
	beq _02224C38
	b _02224C5E
_02224C08:
	add r0, r6, #0
	add r0, #0x11
	cmp r7, r0
	beq _02224C42
	b _02224C5E
_02224C12:
	add r0, r5, #0
	bl ov07_0221C468
	add r6, r0, #0
	b _02224C62
_02224C1C:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _02224C62
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _02224C62
_02224C38:
	add r0, r5, #0
	bl ov07_0221C470
	add r6, r0, #0
	b _02224C62
_02224C42:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _02224C62
	add r0, r5, #0
	bl ov07_0221C470
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _02224C62
_02224C5E:
	bl GF_AssertFail
_02224C62:
	cmp r6, #0xff
	bne _02224C6E
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
_02224C6E:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA48
	add r1, r4, #0
	add r1, #0x10
	str r0, [r4, #0x14]
	bl ov07_02231FA0
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_02222004
	cmp r0, #0
	bgt _02224C9C
	mov r0, #0x64
	ldrsh r1, [r4, r0]
	sub r0, #0x65
	add r2, r1, #0
	mul r2, r0
	add r0, r4, #0
	add r0, #0x64
	strh r2, [r0]
_02224C9C:
	add r0, r5, #0
	bl ov07_0221BFC0
	ldr r0, [r4, #0x60]
	ldr r1, _02224CB4 ; =ov07_02224B14
	add r2, r4, #0
	bl ov07_0221C410
	add r1, r4, #0
	bl ov07_02224B14
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02224CB4: .word ov07_02224B14
	thumb_func_end ov07_02224BAC

	thumb_func_start ov07_02224CB8
ov07_02224CB8: ; 0x02224CB8
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r4, r1, #0
	add r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #3
	bls _02224CC8
	b _02224E46
_02224CC8:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02224CD4: ; jump table
	.short _02224CDC - _02224CD4 - 2 ; case 0
	.short _02224D3E - _02224CD4 - 2 ; case 1
	.short _02224D88 - _02224CD4 - 2 ; case 2
	.short _02224DD6 - _02224CD4 - 2 ; case 3
_02224CDC:
	add r0, r4, #0
	add r0, #0x14
	bl ov07_02222558
	cmp r0, #1
	bne _02224D34
	add r0, r4, #0
	add r0, #0x38
	bl ov07_022227D8
	mov r2, #0x14
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x16
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r2, #0x38
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0
	bl Pokepic_AddAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x28]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #4
	pop {r3, r4, r5, r6, pc}
_02224D34:
	ldrb r0, [r4]
	add sp, #4
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02224D3E:
	ldrb r0, [r4, #9]
	add r0, r0, #1
	strb r0, [r4, #9]
	ldrb r1, [r4, #9]
	ldrb r0, [r4, #0xa]
	cmp r1, r0
	bhi _02224D4E
	b _02224E54
_02224D4E:
	ldr r0, [r4, #0xc]
	mov r1, #0
	bl ov07_0221C4A8
	add r5, r0, #0
	ldr r0, [r4, #0xc]
	mov r1, #1
	bl ov07_0221C4A8
	add r6, r0, #0
	ldr r0, [r4, #0xc]
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [sp]
	add r0, r4, #0
	lsl r1, r5, #0x10
	lsl r3, r6, #0x10
	add r0, #0x14
	asr r1, r1, #0x10
	mov r2, #0x64
	asr r3, r3, #0x10
	bl ov07_02222508
	ldrb r0, [r4]
	add sp, #4
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02224D88:
	add r0, r4, #0
	add r0, #0x14
	bl ov07_02222558
	cmp r0, #1
	bne _02224DCC
	mov r2, #0x14
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x16
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x28]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #4
	pop {r3, r4, r5, r6, pc}
_02224DCC:
	ldrb r0, [r4]
	add sp, #4
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02224DD6:
	add r0, r4, #0
	add r0, #0x14
	bl ov07_02222558
	cmp r0, #1
	ldr r0, [r4, #0x10]
	bne _02224E1A
	mov r2, #0x14
	ldrsh r2, [r4, r2]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x16
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x28]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #4
	pop {r3, r4, r5, r6, pc}
_02224E1A:
	mov r1, #0xc
	add r2, r1, #0
	add r2, #0xf4
	bl Pokepic_SetAttr
	mov r1, #0xd
	add r2, r1, #0
	ldr r0, [r4, #0x10]
	add r2, #0xf3
	bl Pokepic_SetAttr
	mov r2, #2
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x10]
	mov r1, #1
	bl Pokepic_SetAttr
	ldrb r0, [r4]
	add sp, #4
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02224E46:
	ldr r0, [r4, #0xc]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
_02224E54:
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	thumb_func_end ov07_02224CB8

	thumb_func_start ov07_02224E58
ov07_02224E58: ; 0x02224E58
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	bl ov07_0221BFD0
	mov r1, #0x5c
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	strb r0, [r4, #8]
	strb r0, [r4]
	str r5, [r4, #0xc]
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4, #0xc]
	bl ov07_0221FA48
	str r0, [r4, #0x10]
	mov r0, #0
	strb r0, [r4, #9]
	add r0, r5, #0
	mov r1, #4
	bl ov07_0221C4A8
	strb r0, [r4, #0xa]
	ldr r0, [r4, #0x10]
	mov r1, #1
	bl Pokepic_GetAttr
	strh r0, [r4, #2]
	ldr r0, [r4, #0xc]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4, #0xc]
	bl ov07_0221FAA0
	str r0, [r4, #4]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r6, r0, #0
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0x64
	lsl r3, r6, #0x10
	add r0, #0x14
	add r2, r1, #0
	asr r3, r3, #0x10
	bl ov07_02222508
	mov r0, #0xa
	str r0, [sp]
	add r0, r4, #0
	mov r2, #0
	add r0, #0x38
	mov r1, #2
	add r3, r2, #0
	bl ov07_022227A8
	ldr r0, [r4, #0xc]
	ldr r1, _02224EF0 ; =ov07_02224CB8
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	nop
_02224EF0: .word ov07_02224CB8
	thumb_func_end ov07_02224E58

	thumb_func_start ov07_02224EF4
ov07_02224EF4: ; 0x02224EF4
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #3
	bls _02224F04
	b _02225084
_02224F04:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02224F10: ; jump table
	.short _02224F18 - _02224F10 - 2 ; case 0
	.short _02224F7A - _02224F10 - 2 ; case 1
	.short _02224FC6 - _02224F10 - 2 ; case 2
	.short _02225014 - _02224F10 - 2 ; case 3
_02224F18:
	add r0, r4, #0
	add r0, #0x20
	bl ov07_0222260C
	cmp r0, #1
	bne _02224F70
	add r0, r4, #0
	add r0, #0x44
	bl ov07_022227D8
	mov r2, #0x20
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x22
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r2, #0x44
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0
	bl Pokepic_AddAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x34]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #0xc
	pop {r3, r4, pc}
_02224F70:
	ldrb r0, [r4]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, pc}
_02224F7A:
	ldrb r0, [r4, #9]
	add r0, r0, #1
	strb r0, [r4, #9]
	ldrb r1, [r4, #9]
	ldrb r0, [r4, #0xa]
	cmp r1, r0
	bhi _02224F8A
	b _02225092
_02224F8A:
	ldr r3, [r4, #0x10]
	ldr r2, [r4, #0xc]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp]
	asr r1, r2, #0x10
	mov r0, #0x64
	str r0, [sp, #4]
	ldr r0, [r4, #0x14]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	asr r3, r3, #0x10
	str r0, [sp, #8]
	add r0, r4, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	add r0, #0x20
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov07_02222590
	ldrb r0, [r4]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, pc}
_02224FC6:
	add r0, r4, #0
	add r0, #0x20
	bl ov07_0222260C
	cmp r0, #1
	bne _0222500A
	mov r2, #0x20
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x22
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x34]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #0xc
	pop {r3, r4, pc}
_0222500A:
	ldrb r0, [r4]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, pc}
_02225014:
	add r0, r4, #0
	add r0, #0x20
	bl ov07_0222260C
	cmp r0, #1
	ldr r0, [r4, #0x1c]
	bne _02225058
	mov r2, #0x20
	ldrsh r2, [r4, r2]
	mov r1, #0xc
	bl Pokepic_SetAttr
	mov r2, #0x22
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #0xd
	bl Pokepic_SetAttr
	mov r0, #2
	ldrsh r0, [r4, r0]
	ldr r1, [r4, #4]
	ldr r2, [r4, #0x34]
	bl ov07_02222674
	mov r2, #2
	add r3, r0, #0
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #1
	add r2, r2, r3
	bl Pokepic_SetAttr
	add sp, #0xc
	pop {r3, r4, pc}
_02225058:
	mov r1, #0xc
	add r2, r1, #0
	add r2, #0xf4
	bl Pokepic_SetAttr
	mov r1, #0xd
	add r2, r1, #0
	ldr r0, [r4, #0x1c]
	add r2, #0xf3
	bl Pokepic_SetAttr
	mov r2, #2
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x1c]
	mov r1, #1
	bl Pokepic_SetAttr
	ldrb r0, [r4]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, pc}
_02225084:
	ldr r0, [r4, #0x18]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
_02225092:
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end ov07_02224EF4

	thumb_func_start ov07_02225098
ov07_02225098: ; 0x02225098
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	bl ov07_0221BFD0
	mov r1, #0x68
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	strb r0, [r4, #8]
	strb r0, [r4]
	str r5, [r4, #0x18]
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4, #0x18]
	bl ov07_0221FA48
	str r0, [r4, #0x1c]
	mov r0, #0
	strb r0, [r4, #9]
	add r0, r5, #0
	mov r1, #6
	bl ov07_0221C4A8
	strb r0, [r4, #0xa]
	ldr r0, [r4, #0x18]
	mov r1, #3
	bl ov07_0221C4A8
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x18]
	mov r1, #4
	bl ov07_0221C4A8
	str r0, [r4, #0x10]
	ldr r0, [r4, #0x18]
	mov r1, #5
	bl ov07_0221C4A8
	str r0, [r4, #0x14]
	ldr r0, [r4, #0x18]
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	mov r2, #0
	add r3, r4, #2
	bl ov07_02221F38
	ldr r0, [r4, #0x18]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [r4, #0x18]
	bl ov07_0221FAA0
	str r0, [r4, #4]
	mov r1, #2
	ldrsh r1, [r4, r1]
	add r0, r1, r0
	strh r0, [r4, #2]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	asr r0, r0, #0x10
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #0x10]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r0, r0, #0x10
	asr r7, r0, #0x10
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	asr r0, r0, #0x10
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r0, r0, #0x10
	asr r6, r0, #0x10
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	str r6, [sp]
	mov r1, #0x64
	str r1, [sp, #4]
	str r0, [sp, #8]
	add r0, r4, #0
	ldr r1, [sp, #0x10]
	ldr r3, [sp, #0xc]
	add r0, #0x20
	add r2, r7, #0
	bl ov07_02222590
	mov r0, #0xa
	str r0, [sp]
	add r0, r4, #0
	mov r2, #0
	add r0, #0x44
	mov r1, #2
	add r3, r2, #0
	bl ov07_022227A8
	ldr r0, [r4, #0x18]
	ldr r1, _02225198 ; =ov07_02224EF4
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02225198: .word ov07_02224EF4
	thumb_func_end ov07_02225098

	thumb_func_start ov07_0222519C
ov07_0222519C: ; 0x0222519C
	push {r3, r4, r5, lr}
	mov r2, #1
	lsl r2, r2, #0x1a
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r2]
	ldr r0, _022251C8 ; =0xFFFF1FFF
	and r0, r1
	str r0, [r2]
	ldr r0, [r4, #8]
	mov r1, #2
	bl ov07_0221EC7C
	ldr r0, [r4, #8]
	add r1, r5, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, pc}
	nop
_022251C8: .word 0xFFFF1FFF
	thumb_func_end ov07_0222519C

	thumb_func_start ov07_022251CC
ov07_022251CC: ; 0x022251CC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r1, #0x28
	bl ov07_022324D8
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	add r0, r5, #0
	add r1, r4, #4
	bl ov07_02231FE4
	ldr r0, [r4, #8]
	mov r1, #2
	bl ov07_0221EB98
	ldr r0, [r4, #8]
	mov r1, #0
	bl ov07_0221C4E8
	str r0, [r4, #0x24]
	mov r1, #2
	bl ManagedSprite_SetOamMode
	ldr r0, [r4, #0x24]
	mov r1, #2
	bl ManagedSprite_SetAffineOverwriteMode
	ldr r1, _0222524C ; =0x3F99999A
	ldr r0, [r4, #0x24]
	add r2, r1, #0
	bl ManagedSprite_SetAffineScale
	mov r0, #1
	lsl r0, r0, #0x1a
	ldr r2, [r0]
	ldr r1, _02225250 ; =0xFFFF1FFF
	and r2, r1
	lsr r1, r0, #0xb
	orr r1, r2
	str r1, [r0]
	add r0, #0x4a
	ldrh r2, [r0]
	mov r1, #0x3f
	bic r2, r1
	mov r1, #0x1b
	orr r1, r2
	strh r1, [r0]
	ldrh r2, [r0]
	ldr r1, _02225254 ; =0xFFFFC0FF
	and r2, r1
	mov r1, #7
	lsl r1, r1, #8
	orr r1, r2
	strh r1, [r0]
	ldr r0, [r4, #8]
	ldr r1, _02225258 ; =ov07_0222519C
	add r2, r4, #0
	bl ov07_0221C410
	pop {r3, r4, r5, pc}
	nop
_0222524C: .word 0x3F99999A
_02225250: .word 0xFFFF1FFF
_02225254: .word 0xFFFFC0FF
_02225258: .word ov07_0222519C
	thumb_func_end ov07_022251CC

	thumb_func_start ov07_0222525C
ov07_0222525C: ; 0x0222525C
	push {r4, lr}
	sub sp, #8
	add r4, r1, #0
	add r2, r0, #0
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _02225274
	cmp r0, #1
	beq _022252B0
	b _022252E6
_02225274:
	mov r0, #0x12
	ldrsh r3, [r4, r0]
	mov r0, #0x10
	ldrsh r1, [r4, r0]
	mov r0, #0x66
	ldrsh r0, [r4, r0]
	mov r2, #0x64
	add r0, r3, r0
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp]
	ldr r0, [r4, #8]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	ldrsh r2, [r4, r2]
	add r0, r4, #0
	add r0, #0x3c
	add r2, r1, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_02222268
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x20
	strb r1, [r0]
_022252B0:
	add r0, r4, #0
	add r0, #0x3c
	bl ov07_022222B4
	cmp r0, #0
	bne _022252CA
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x20
	strb r1, [r0]
_022252CA:
	mov r2, #0x3c
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x14]
	mov r1, #0
	bl Pokepic_SetAttr
	mov r2, #0x3e
	ldrsh r2, [r4, r2]
	ldr r0, [r4, #0x14]
	mov r1, #1
	bl Pokepic_SetAttr
	add sp, #8
	pop {r4, pc}
_022252E6:
	ldr r0, [r4, #0x60]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl Heap_Free
	add sp, #8
	pop {r4, pc}
	thumb_func_end ov07_0222525C

	thumb_func_start ov07_022252F8
ov07_022252F8: ; 0x022252F8
	push {r3, r4, r5, r6, r7, lr}
	mov r1, #0x68
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	str r5, [r4, #0x60]
	add r0, r5, #0
	mov r1, #0
	bl ov07_0221C4A8
	str r0, [r4, #8]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221C4A8
	add r1, r4, #0
	add r1, #0x64
	strh r0, [r1]
	add r0, r5, #0
	mov r1, #2
	bl ov07_0221C4A8
	add r1, r4, #0
	add r1, #0x66
	strh r0, [r1]
	add r0, r5, #0
	mov r1, #3
	bl ov07_0221C4A8
	add r1, r4, #0
	add r7, r0, #0
	add r0, r5, #0
	add r1, #0x20
	bl ov07_02231FE4
	mov r6, #0xff
	add r0, r6, #0
	add r0, #9
	cmp r7, r0
	bgt _02225362
	add r0, r6, #3
	cmp r7, r0
	blt _022253B8
	beq _0222536C
	add r0, r6, #5
	cmp r7, r0
	beq _02225376
	add r0, r6, #0
	add r0, #9
	cmp r7, r0
	beq _02225392
	b _022253B8
_02225362:
	add r0, r6, #0
	add r0, #0x11
	cmp r7, r0
	beq _0222539C
	b _022253B8
_0222536C:
	add r0, r5, #0
	bl ov07_0221C468
	add r6, r0, #0
	b _022253BC
_02225376:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _022253BC
	add r0, r5, #0
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _022253BC
_02225392:
	add r0, r5, #0
	bl ov07_0221C470
	add r6, r0, #0
	b _022253BC
_0222539C:
	add r0, r5, #0
	bl ov07_0221FAB0
	cmp r0, #1
	bne _022253BC
	add r0, r5, #0
	bl ov07_0221C470
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_0223197C
	add r6, r0, #0
	b _022253BC
_022253B8:
	bl GF_AssertFail
_022253BC:
	cmp r6, #0xff
	bne _022253C8
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
_022253C8:
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_0221FA48
	str r0, [r4, #0x14]
	cmp r0, #0
	bne _022253DE
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
_022253DE:
	add r1, r4, #0
	add r1, #0x10
	bl ov07_02231FA0
	add r0, r5, #0
	add r1, r6, #0
	bl ov07_02222004
	cmp r0, #0
	bgt _02225412
	mov r0, #0x64
	ldrsh r1, [r4, r0]
	sub r0, #0x65
	add r2, r1, #0
	mul r2, r0
	add r0, r4, #0
	add r0, #0x64
	strh r2, [r0]
	mov r0, #0x66
	ldrsh r1, [r4, r0]
	sub r0, #0x67
	add r2, r1, #0
	mul r2, r0
	add r0, r4, #0
	add r0, #0x66
	strh r2, [r0]
_02225412:
	add r0, r5, #0
	bl ov07_0221BFC0
	cmp r0, #1
	bne _0222542C
	mov r0, #0x66
	ldrsh r1, [r4, r0]
	sub r0, #0x67
	add r2, r1, #0
	mul r2, r0
	add r0, r4, #0
	add r0, #0x66
	strh r2, [r0]
_0222542C:
	ldr r0, [r4, #0x60]
	ldr r1, _02225440 ; =ov07_0222525C
	add r2, r4, #0
	bl ov07_0221C410
	add r1, r4, #0
	bl ov07_0222525C
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02225440: .word ov07_0222525C
	thumb_func_end ov07_022252F8
