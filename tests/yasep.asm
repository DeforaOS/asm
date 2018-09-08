/* $Id$ */
.section .text
	add	$0x0e, %r1
	add	%r0, %r1
	and	$0x0d, %r1
	and	%r2, %r3
	andn	$0x0c, %r1
	andn	%r4, %r5
	cmps	$0x0b, %r1
	cmps	%r6, %r7
	cmpu	$0x0a, %r1
	cmpu	%r8, %r9
	get	$0x09, %r1
	get	%r10, %r11
	lsb	%r12, %r13
#ifdef __yasep__
	lsh	%r14, %r15
#endif
	lzb	%npc, %d5
#ifdef __yasep__
	lzh	%a5, %d4
#endif
	mov	$0x04, %a4
	mov	%d3, %a3
	mul8h	$0x03, %d2
	mul8h	%a2, %d1
	mul8l	$0x02, %a1
	mul8l	%r0, %r1
	muli	$0x01, %r1
	muli	%r0, %r1
	nand	$0x00, %r1
	nand	%r0, %r1
	nor	$0x01, %r1
	nor	%r0, %r1
	or	$0x02, %r1
	or	%r0, %r1
	orn	$0x03, %r1
	orn	%r0, %r1
	put	%r0, %r1
	rol	$0x05, %r1
	rol	%r0, %r1
	ror	$0x06, %r1
	ror	%r0, %r1
	sar	$0x07, %r1
	sar	%r0, %r1
	sb	$0x08, %r1
	sb	%r0, %r1
#ifdef __yasep__
	sh	$0x09, %r1
	sh	%r0, %r1
	shh	$0x0a, %r1
	shh	%r0, %r1
#endif
	shl	$0x0b, %r1
	shl	%r0, %r1
	shr	$0x0c, %r1
	shr	%r0, %r1
	smax	$0x0d, %r1
	smax	%r0, %r1
	smin	$0x0e, %r1
	smin	%r0, %r1
	sub	$0x0f, %r1
	sub	%r0, %r1
	umax	$0x0e, %r1
	umax	%r0, %r1
	umin	$0x0d, %r1
	umin	%r0, %r1
	xor	$0x0c, %r1
	xor	%r0, %r1
	xorn	$0x0b, %r1
	xorn	%r0, %r1
