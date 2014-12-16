.nolist
.macro frame currpars, locals
    .equ    frame_size, \locals*4
.endm

.equ local1, -4
.equ local2, -8
.equ local3, -12
.equ local4, -16
.equ local5, -20
.equ local6, -24
.equ local7, -28
.equ local8, -32
.equ local9, -36
.equ local10, -40
.equ local11, -44
.equ local12, -48
.equ local13, -52
.equ local14, -56
.equ local15, -60
.equ local16, -64
.equ local17, -68
.equ local18, -72
.equ local19, -76
.equ local20, -80
.equ local21, -84
.equ local22, -88
.equ local23, -92
.equ local24, -96
.equ local25, -100
.equ local26, -104
.equ local27, -108
.equ local28, -112
.equ local29, -116
.equ local30, -120
.equ local31, -124
.equ local32, -128
.equ local33, -132
.equ local34, -136
.equ local35, -140
.equ local36, -144
.equ local37, -148
.equ local38, -152
.equ local39, -156
.equ local40, -160
.equ local41, -164
.equ local42, -168
.equ local43, -172
.equ local44, -176
.equ local45, -180
.equ local46, -184
.equ local47, -188
.equ local48, -192
.equ local49, -196
.equ local50, -200
.equ local51, -204
.equ local52, -208
.equ local53, -212
.equ local54, -216
.equ local55, -220
.equ local56, -224
.equ local57, -228
.equ local58, -232
.equ local59, -236
.equ local60, -240
.equ local61, -244
.equ local62, -248
.equ local63, -252
.equ local64, -256
.equ local65, -260
.equ local66, -264
.equ local67, -268
.equ local68, -272
.equ local69, -276
.equ local70, -280
.equ local71, -284
.equ local72, -288
.equ local73, -292
.equ local74, -296
.equ local75, -300
.equ local76, -304
.equ local77, -308
.equ local78, -312
.equ local79, -316
.equ local80, -320
.equ local81, -324
.equ local82, -328
.equ local83, -332
.equ local84, -336
.equ local85, -340
.equ local86, -344
.equ local87, -348
.equ local88, -352
.equ local89, -356
.equ local90, -360
.equ local91, -364
.equ local92, -368
.equ local93, -372
.equ local94, -376
.equ local95, -380
.equ local96, -384
.equ local97, -388
.equ local98, -392
.equ local99, -396
.equ local100, -400
.equ currPar1 , 8
.equ currPar2 , 12
.equ currPar3 , 16
.equ currPar4 , 20
.equ currPar5 , 24
.equ currPar6 , 28
.equ currPar7 , 32
.equ currPar8 , 36
.equ currPar9 , 40
.equ currPar10 , 44
.equ currPar11 , 48
.equ currPar12 , 52
.equ currPar13 , 56
.equ currPar14 , 60
.equ currPar15 , 64
.equ currPar16 , 68
.equ currPar17 , 72
.equ currPar18 , 76
.equ currPar19 , 80
.equ currPar20 , 84
.equ currPar21 , 88
.equ currPar22 , 92
.equ currPar23 , 96
.equ currPar24 , 100
.equ currPar25 , 104
.equ currPar26 , 108
.equ currPar27 , 112
.equ currPar28 , 116
.equ currPar29 , 120
.equ currPar30 , 124
.equ currPar31 , 128
.equ currPar32 , 132
.equ currPar33 , 136
.equ currPar34 , 140
.equ currPar35 , 144
.equ currPar36 , 148
.equ currPar37 , 152
.equ currPar38 , 156
.equ currPar39 , 160
.equ currPar40 , 164
.equ currPar41 , 168
.equ currPar42 , 172
.equ currPar43 , 176
.equ currPar44 , 180
.equ currPar45 , 184
.equ currPar46 , 188
.equ currPar47 , 192
.equ currPar48 , 196
.equ currPar49 , 200
.equ currPar50 , 204
.equ currPar51 , 208
.equ currPar52 , 212
.equ currPar53 , 216
.equ currPar54 , 220
.equ currPar55 , 224
.equ currPar56 , 228
.equ currPar57 , 232
.equ currPar58 , 236
.equ currPar59 , 240
.equ currPar60 , 244
.equ currPar61 , 248
.equ currPar62 , 252
.equ currPar63 , 256
.equ currPar64 , 260
.equ currPar65 , 264
.equ currPar66 , 268
.equ currPar67 , 272
.equ currPar68 , 276
.equ currPar69 , 280
.equ currPar70 , 284
.equ currPar71 , 288
.equ currPar72 , 292
.equ currPar73 , 296
.equ currPar74 , 300
.equ currPar75 , 304
.equ currPar76 , 308
.equ currPar77 , 312
.equ currPar78 , 316
.equ currPar79 , 320
.equ currPar80 , 324
.equ currPar81 , 328
.equ currPar82 , 332
.equ currPar83 , 336
.equ currPar84 , 340
.equ currPar85 , 344
.equ currPar86 , 348
.equ currPar87 , 352
.equ currPar88 , 356
.equ currPar89 , 360
.equ currPar90 , 364
.equ currPar91 , 368
.equ currPar92 , 372
.equ currPar93 , 376
.equ currPar94 , 380
.equ currPar95 , 384
.equ currPar96 , 388
.equ currPar97 , 392
.equ currPar98 , 396
.equ currPar99 , 400
.equ currPar100 , 404
.list
