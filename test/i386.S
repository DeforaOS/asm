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
	/* CWDE */
	cwde					/* 98			*/
	/* CLC */
	clc					/* f8			*/
	/* CLD */
	cld					/* fc			*/
	/* CLI */
	cli					/* fa			*/
	/* CLTS */
	clts					/* 0f 06		*/
	/* CMC */
	cmc					/* f5			*/
	/* NOP */
	nop
