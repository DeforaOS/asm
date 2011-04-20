/* $Id$ */
.text
	aaa
	aad
	aad	$0x42
	aam
	aam	$0x42
	aas
	/* ADC */
	adc	[%ecx], %dh			/* 10 31		*/
	adc	[%ecx+$0x50], %dh		/* 10 71 50		*/
	adc	%bl, %bh			/* 10 fb		*/
	adc	%eax, %eax			/* 11 c0		*/
	adc	[%eax], %eax			/* 11 00		*/
#if 1 /* FIXME doesn't work at the moment */
	adc	%dh, [%ecx]			/* 12			*/
	adc	%bl, %bh			/* 12			*/
	adc	%edx, [%ecx]			/* 13			*/
	adc	%ebx, %ebx			/* 13			*/
#endif
	adc	%al, $0x40			/* 14 40		*/
	adc	%eax, $0x41424344		/* 15 44 43 42 41	*/
	adc	[%edx], $0x46			/* 80 12 46		*/
	adc	[%ebx + $0x15], $0x47		/* 80 53 15 47		*/
	adc	[%ebx + $0x16171819], $0x48	/* 80 93 19 18 17 16 48	*/
	adc	%cl, $0x45			/* 80 d1 45		*/
	adc	[%edx], $0x46474849		/* 81 12 46 47 48 49	*/
	adc	[%ebx + $0x14], $0x4748494a	/* 81 53 14 47 48 49 4a	*/
	adc	[%ebx + $0x16171819], $0x48494a4b
						/* 81 93 19 18 17 16 4b	*/
						/* 4a 49 48		*/
	adc	%ecx, $0x45464748		/* 81 d1 45 46 47 48	*/
	adc	[%eax], -$0x02			/* 83 10 fe		*/
	/* ADD */
	add	%al, $0x40			/* 04 40		*/
	add	%eax, $0x41424344		/* 05 44 43 42 41	*/
	add	[%edx], $0x46			/* 80 02 46		*/
	add	[%ebx + $0x15], $0x47		/* 80 43 15 47		*/
	add	[%ebx + $0x16171819], $0x48	/* 80 83 19 18 17 16 48	*/
	add	%cl, $0x45			/* 80 c1 45		*/
	add	[%edx], $0x46474849		/* 81 02 46 47 48 49	*/
	add	[%ebx + $0x14], $0x4748494a	/* 81 43 14 47 48 49 4a	*/
	add	[%ebx + $0x16171819], $0x48494a4b
						/* 81 83 19 18 17 16 4b	*/
						/* 4a 49 48		*/
	add	%ecx, $0x45464748		/* 81 c1 45 46 47 48	*/
	/* AND */
	and	%ecx, $0x45464748		/* 81 e1 45 46 47 48	*/
	/* BSF */
#if 1 /* FIXME doesn't work at the moment */
	bsf	%eax, [%eax]			/* 0f bc		*/
#endif
	/* BSR */
#if 1 /* FIXME doesn't work at the moment */
	bsr	%eax, [%eax]			/* 0f bd		*/
#endif
	/* BT */
	bt	%eax, %ecx			/* 0f a3 c8		*/
	bt	[%eax], %ecx			/* 0f a3 08		*/
	bt	%eax, $0x42			/* 0f ba e0 42		*/
	bt	[%eax], $0x42			/* 0f ba 20 42		*/
	/* BTC */
	btc	%eax, %ecx			/* 0f bb c8		*/
	btc	[%eax], %ecx			/* 0f bb 08		*/
	btc	%eax, $0x42			/* 0f ba f8 42		*/
	btc	[%eax], $0x42			/* 0f ba 38 42		*/
	/* BTR */
	btr	%eax, %ecx			/* 0f b3 c8		*/
	btr	[%eax], %ecx			/* 0f b3 08		*/
	btr	%eax, $0x42			/* 0f ba f0 42		*/
	btr	[%eax], $0x42			/* 0f ba 30 42		*/
	/* BTS */
	bts	%eax, %ecx			/* 0f ab c8		*/
	bts	[%eax], %ecx			/* 0f ab 08		*/
	bts	%eax, $0x42			/* 0f ba e8 42		*/
	bts	[%eax], $0x42			/* 0f ba 28 42		*/
	cwde					/* 98			*/
	clc					/* f8			*/
	cld					/* fc			*/
	cli					/* fa			*/
	clts					/* 0f 06		*/
	cmc					/* f5			*/
	cdq					/* 99			*/
	daa					/* 27			*/
	das					/* 2f			*/
	/* DEC */
	dec	%eax				/* 48			*/
	dec	%ecx				/* 49			*/
	dec	%edx				/* 4a			*/
	dec	%ebx				/* 4b			*/
	dec	%esp				/* 4c			*/
	dec	%ebp				/* 4d			*/
	dec	%esi				/* 4e			*/
	dec	%edi				/* 4f			*/
	decb	[%eax]				/* fe 08		*/
	decb	[%ecx]				/* fe 09		*/
	decb	[%edx]				/* fe 0a		*/
	decb	[%ebx]				/* fe 0b		*/
	dec	[%eax]				/* ff 08		*/
	dec	[%ecx]				/* ff 09		*/
	dec	[%edx]				/* ff 0a		*/
	dec	[%ebx]				/* ff 0b		*/
	/* DIV */
	divb	[%ecx]				/* f6 31		*/
	divb	[%eax + $0x02]			/* f6 70 02		*/
	divb	[%ebx + $0x4012]		/* f6 b3 12 40 00 00	*/
	div	%dl				/* f6 f2		*/
	div	[%ecx]				/* f7 31		*/
	div	[%eax + $0x02]			/* f7 70 02		*/
	div	[%ebx + $0x4012]		/* f7 b3 12 40 00 00	*/
	div	%edx				/* f7 f2		*/
	/* ENTER */
	enter	$0xabcd, $0x0			/* c8 cd ab 00		*/
	enter	$0xdcef, $0x1			/* c8 ef dc 01		*/
	enter	$0xfacd, $0x42			/* c8 cd fa 42		*/
	f2xm1
	fabs
	/* FADD */
	fadd	%st0, %st0
	fadd	%st0, %st1
	fadd	%st0, %st2
	fadd	%st0, %st3
	fadd	%st0, %st4
	fadd	%st0, %st5
	fadd	%st0, %st6
	fadd	%st0, %st7
	fadd	%st1, %st0
	fadd	%st2, %st0
	fadd	%st3, %st0
	fadd	%st4, %st0
	fadd	%st5, %st0
	fadd	%st6, %st0
	fadd	%st7, %st0
	/* FADDP */
	faddp	%st0, %st0
	faddp
	faddp	%st2, %st0
	faddp	%st3, %st0
	faddp	%st4, %st0
	faddp	%st5, %st0
	faddp	%st6, %st0
	faddp	%st7, %st0
	fchs
	fclex
	fcom					/* de d9		*/
	fcom	%st0
	fcom	%st1
	fcom	%st2
	fcom	%st3
	fcom	%st4
	fcom	%st5
	fcom	%st6
	fcom	%st7
	fcomp					/* de d9		*/
	fcomp	%st0
	fcomp	%st1
	fcomp	%st2
	fcomp	%st3
	fcomp	%st4
	fcomp	%st5
	fcomp	%st6
	fcomp	%st7
	fcompp					/* de d9		*/
	fcos
	fdecstp
	fdiv	%st0, %st0
	fdiv	%st0, %st1
	fdiv	%st0, %st2
	fdiv	%st0, %st3
	fdiv	%st0, %st4
	fdiv	%st0, %st5
	fdiv	%st0, %st6
	fdiv	%st0, %st7
	fdiv	%st0, %st0
	fdiv	%st1, %st0
	fdiv	%st2, %st0
	fdiv	%st3, %st0
	fdiv	%st4, %st0
	fdiv	%st5, %st0
	fdiv	%st6, %st0
	fdiv	%st7, %st0
	fdivp
	fdivp	%st0, %st0
	fdivp	%st1, %st0
	fdivp	%st2, %st0
	fdivp	%st3, %st0
	fdivp	%st4, %st0
	fdivp	%st5, %st0
	fdivp	%st6, %st0
	fdivp	%st7, %st0
	fdivr	%st0, %st0
	fdivr	%st0, %st1
	fdivr	%st0, %st2
	fdivr	%st0, %st3
	fdivr	%st0, %st4
	fdivr	%st0, %st5
	fdivr	%st0, %st6
	fdivr	%st0, %st7
	fdivr	%st0, %st0
	fdivr	%st1, %st0
	fdivr	%st2, %st0
	fdivr	%st3, %st0
	fdivr	%st4, %st0
	fdivr	%st5, %st0
	fdivr	%st6, %st0
	fdivr	%st7, %st0
	fdivrp
	fdivrp	%st0, %st0
	fdivrp	%st1, %st0
	fdivrp	%st2, %st0
	fdivrp	%st3, %st0
	fdivrp	%st4, %st0
	fdivrp	%st5, %st0
	fdivrp	%st6, %st0
	fdivrp	%st7, %st0
	ffree	%st0				/* dd c0		*/
	ffree	%st1				/* dd c1		*/
	ffree	%st2				/* dd c2		*/
	ffree	%st3				/* dd c3		*/
	ffree	%st4				/* dd c4		*/
	ffree	%st5				/* dd c5		*/
	ffree	%st6				/* dd c6		*/
	ffree	%st7				/* dd c7		*/
	finit
	fld	%st0				/* d9 c0		*/
	fld	%st1
	fld	%st2
	fld	%st3
	fld	%st4
	fld	%st5
	fld	%st6
	fld	%st7
	fld1					/* d9 e8		*/
	fld2e
	fld2t
	fldg2
	fldn2
	fldpi
	fldz
	fnclex
	fninit
	fwait
	/* IN */
	in	%al, $0x43			/* e4 43		*/
	in	%eax, $0x44			/* e5 44		*/
	in	%al, %dx			/* ec			*/
	in	%eax, %dx			/* ed			*/
	/* INC */
	inc	%eax				/* 40			*/
	inc	%ecx				/* 41			*/
	inc	%edx				/* 42			*/
	inc	%ebx				/* 43			*/
	inc	%esp				/* 44			*/
	inc	%ebp				/* 45			*/
	inc	%esi				/* 46			*/
	inc	%edi				/* 47			*/
	incb	[%eax]				/* fe 00		*/
	incb	[%ecx]				/* fe 01		*/
	incb	[%edx]				/* fe 02		*/
	incb	[%ebx]				/* fe 03		*/
	inc	[%eax]				/* ff 00		*/
	inc	[%ecx]				/* ff 01		*/
	inc	[%edx]				/* ff 02		*/
	inc	[%ebx]				/* ff 03		*/
	insb
	insd
	int	$0x3				/* cd 03		*/
	int3					/* cc			*/
	int	$0x4				/* cd 04		*/
	int	$0x42				/* cd 42		*/
	into					/* ce			*/
	invd
	iret
	iretd
	lahf
	leave
	lock
	lodsb					/* ac			*/
	lodsd					/* ad			*/
	loop	$0x41
	loope	$0x41
	loopne	$0x41
	loopnz	$0x41
	loopz	$0x41
	nop					/* 90			*/
	movsb
	movsd
	mulb	[%eax]
	mulb	[%eax + $0x42]
	mulb	[%eax + $0x11223344]
	mul	%ch
	mul	[%eax]
	mul	[%eax + $0x42]
	mul	[%eax + $0x11223344]
	mul	%eax
	negb	[%eax]
	negb	[%eax + $0x42]
	negb	[%eax + $0x11223344]
	neg	%ch
	neg	[%eax]
	neg	[%eax + $0x42]
	neg	[%eax + $0x11223344]
	neg	%eax
	notb	[%eax]
	notb	[%eax + $0x42]
	notb	[%eax + $0x11223344]
	not	%ch
	not	[%eax]
	not	[%eax + $0x42]
	not	[%eax + $0x11223344]
	not	%eax
	/* OR */
	or	[%ecx], %dh
	or	[%ecx+$0x50], %dh
	or	%bl, %bh
	or	%eax, %eax
	or	[%eax], %eax
#if 1 /* FIXME doesn't work at the moment */
	or	%dh, [%ecx]
	or	%bl, %bh
	or	%edx, [%ecx]
	or	%ebx, %ebx
#endif
	or	%al, $0x40
	or	%eax, $0x41424344
	or	[%edx], $0x46
	or	[%ebx + $0x15], $0x47
	or	[%ebx + $0x16171819], $0x48
	or	%cl, $0x45
	or	[%edx], $0x46474849
	or	[%ebx + $0x14], $0x4748494a
	or	[%ebx + $0x16171819], $0x48494a4b
	or	%ecx, $0x45464748
	or	[%eax], -$0x02
	out	$0xa8, %al
	out	$0xa8, %eax
	out	%dx, %al
	out	%dx, %eax
	outsb					/* 6e			*/
	outsd					/* 6f			*/
	pop	%ds				/* 1f			*/
	pop	%es				/* 07			*/
	pop	%ss				/* 17			*/
	pop	%fs				/* 0f a1		*/
	pop	%gs				/* 0f a9		*/
	pop	%eax
	pop	%ecx
	pop	%edx
	pop	%ebx
	pop	%esp
	pop	%ebp
	pop	%esi
	pop	%edi
	pop	[%ecx]
	pop	[%edx + $0x42]
	pop	[%ebx + $0x43424140]
	popa					/* 61			*/
	popad					/* 61			*/
	popf					/* 9d			*/
	popfd					/* 9d			*/
	push	%cs				/* 0e			*/
	push	%ss				/* 16			*/
	push	%ds				/* 1e			*/
	push	%es				/* 06			*/
	push	%fs				/* 0f a0		*/
	push	%gs				/* 0f a8		*/
	push	%eax
	push	%ecx
	push	%edx
	push	%ebx
	push	%esp
	push	%ebp
	push	%esi
	push	%edi
	push	$0x42
	push	$0x42424242
	push	[%eax]
	push	[%eax + $0x42]
	push	[%eax + $0x43424140]
	pusha					/* 60			*/
	pushad					/* 60			*/
	pushf					/* 9c			*/
	pushfd					/* 9c			*/
	ret
	ret	$0x3412
	rsm					/* 0f aa		*/
	sahf					/* 9e			*/
	scasb
	scasd
	sgdt	[%ecx]
	sidt	[%edx - $0x79]
	shld	%ecx, %edx, $0x31
	shld	%eax, %eax, %cl
	shrd	%eax, %eax, $0x31
	shrd	%eax, %eax, %cl
	stc
	std
	sti
	stosb
	stosd
	/* TEST */
	test	%al, $0xaa
	test	%eax, $0xccddeeff
	testb	[%ecx], $0x04
	testb	[%ecx + $0x12], $0x04
	testb	[%ecx + $0x2212], $0x04
	test	%bl, $0x04
	test	[%ecx], $0x04002000
	test	[%ecx + $0x22], $0x04002000
	test	[%ecx + $0x2221], $0x04002000
	test	%ebx, $0x04002000
#if 0 /* FIXME doesn't work */
	testb	[%ecx], %dh
	testb	[%ecx + $0x12], %ch
	testb	[%ecx + $0x2212], %dh
	test	%bl, %bh
	test	[%ecx], %eax
	test	[%ecx + $0x22], %ecx
	test	[%ecx + $0x2221], %edx
#endif
	test	%ebx, %ebx			/* 85 c3		*/
	ud2
	wait
	wbinvd
	wrmsr
	/* XADD */
	xadd	%eax, %ecx			/* 0f c1 c8		*/
	xadd	%ecx, %eax			/* 0f c1 c1		*/
	xadd	[%eax], %ebx			/* 0f c1 18		*/
	/* XCHG */
	xchg	%eax, %ecx			/* 91			*/
	xchg	%ecx, %eax			/* 91			*/
	xchg	[%eax], %ebx			/* 87 18		*/
	xchg	%eax, [%ebx]
	xlatb
	/* XOR */
	xor	[%ecx], %dh			/* 30 31		*/
	xor	[%ecx+$0x50], %dh		/* 30 71 50		*/
	xor	%bl, %bh			/* 30 fb		*/
	xor	%eax, %eax			/* 31 c0		*/
	xor	[%eax], %eax			/* 31 00		*/
#if 1 /* FIXME doesn't work at the moment */
	xor	%dh, [%ecx]			/* 32			*/
	xor	%bl, %bh			/* 32			*/
	xor	%edx, [%ecx]			/* 33			*/
	xor	%ebx, %ebx			/* 33			*/
#endif
	xor	%al, $0x40			/* 34 40		*/
	xor	%eax, $0x41424344		/* 35 44 43 42 41	*/
	xor	[%edx], $0x46			/* 80 32 46		*/
	xor	[%ebx + $0x15], $0x47		/* 80 73 15 47		*/
	xor	[%ebx + $0x16171819], $0x48	/* 80 b3 19 18 17 16 48	*/
	xor	%cl, $0x45			/* 80 f1 45		*/
	xor	[%edx], $0x46474849		/* 81 32 46 47 48 49	*/
	xor	[%ebx + $0x14], $0x4748494a	/* 81 73 14 47 48 49 4a	*/
	xor	[%ebx + $0x16171819], $0x48494a4b
						/* 81 b3 19 18 17 16 4b	*/
						/* 4a 49 48		*/
	xor	%ecx, $0x45464748		/* 81 f1 45 46 47 48	*/
	xor	[%eax], -$0x02			/* 83 30 fe		*/
