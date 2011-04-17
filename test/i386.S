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
	cpuid					/* 0f a2		*/
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
	fnclex
	nop					/* 0x90			*/
