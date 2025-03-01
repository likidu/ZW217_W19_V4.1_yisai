;/*****************************************/
; arm_mc_chroma4x4_dx2dy2_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_CHROMA_SIZE	EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pt0				RN		3
pt1				RN		4
pt2				RN		5
pt3				RN		6
pt4				RN		7

pb0				RN		8
pb1				RN		9
pb2				RN		8
pb3				RN		9
pb4				RN		8

tmp				RN		10
par3			RN		11
pRef_b			RN		0

srcPitch_sub4	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $pt0, $pt1,$pb0,$pb1, $dst_add
		ldrb	$pb1, [pRef_b], $src_add
		add		$pt0, $pt0, $pt1
		add  	$pt0, $pt0, $pb0
		add		$pt0, $pt0, $pb1
		add		$pt0, $pt0, #1
		mov		$pt0, $pt0, asr #2
		strb	$pt0, [pPred], #$dst_add
		mov		$pt0, $pb0
		
		MEND   

;/***************************************************/
;arm_mc_chroma4x4_dx2dy2_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma4x4_dx2dy2_rv9 	
		
arm_mc_chroma4x4_dx2dy2_rv9

		stmfd	sp!, {r4 - r11, r14} 

		mov		par3, #3		
		sub		srcPitch_sub4, srcPitch, #4
				
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], #1
		ldrb	pt4, [pRef], srcPitch_sub4
		
		mov		i, #4
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pb0, [pRef_b], #1
		
		;//pix0
		ldrb	pb1, [pRef_b], #1
		add		pt0, pt0, pt1
		add		pt0, pt0, pb0
		add		pt0, pt0, pb1
		add		pt0, pt0, #1
		mov		pt0, pt0, asr #2
		strb	pt0, [pPred], #1
		mov		pt0, pb0
 			
		BILINEAR_FILTER_1PIX	#1, pt1, pt2, pb1, pb2, 1 ;//pix1
		BILINEAR_FILTER_1PIX	#1, pt2, pt3, pb2, pb3, 1;//pix2
		BILINEAR_FILTER_1PIX	srcPitch_sub4, pt3, pt4, pb3, pb4, 5;//pix3
		mov		pt4, pb4
		
		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r11, pc}	
				
		END
