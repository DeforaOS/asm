.text
	adc	%r0, %r1, %r2
	adceq	%r0, %r1, %r2
	add	%r2, %r1, %r0
	addeq	%r2, %r1, %r0
	and	%r3, %r4, %r5
	andeq	%r3, %r4, %r5
	b	$0xa0
	beq	$0xa0
	bl	$0xa0
	bleq	$0xa0
	bx	%r0
	bxeq	%r0
	bic	%r0, %r0, $0x1f
	mrs	%r0, %cpsr
	msr	%r0, %cpsr
	nop
