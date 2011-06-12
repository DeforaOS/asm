/* $Id$ */
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
	bic	%r0, %r0, %r0
	biceq	%r2, %r0, %r1
	cmn	%r5, %r4, %r3
	cmneq	%r5, %r4, %r3
	cmp	%r5, %r4, %r3
	cmpeq	%r5, %r4, %r3
	eor	%r5, %r4, %r3
	eoreq	%r5, %r4, %r3
	ldm	%r0, %r0, %r0
	ldmeq	%r0, %r0, %r0
	ldr	%r0, %r0, %r0
	ldreq	%r0, %r0, %r0
	mov	%r5, %r4, %r3
	moveq	%r5, %r4, %r3
	mrs	%r0, %cpsr
	msr	%r0, %cpsr
	mul	%r0, %r1, %r2
	muls	%r0, %r1, %r2
	mvn	%r5, %r4, %r3
	mvneq	%r5, %r4, %r3
	nop
	orr	%r5, %r4, %r3
	orreq	%r5, %r4, %r3
	rsb	%r5, %r4, %r3
	rsbeq	%r5, %r4, %r3
	rsc	%r5, %r4, %r3
	rsceq	%r5, %r4, %r3
	sbc	%r5, %r4, %r3
	sbceq	%r5, %r4, %r3
	stm	%r0, %r0, %r0
	stmeq	%r0, %r0, %r0
	str	%r0, %r0, %r0
	streq	%r0, %r0, %r0
	sub	%r5, %r4, %r3
	subeq	%r5, %r4, %r3
	swi	$0x0
	swine	$0x0
	swp	%r5, %r4, %r3
	swpeq	%r5, %r4, %r3
