/* $Id$ */
.section text
	ldd	[%r4 + %r7], %r11		/* 0xd6190007 */
	ldub	[%r23], %r19			/* 0xe60dc000 */
	sub	%r16, %r26, %r27		/* 0xb624001a */
	smulcc	%r29, -$23, %r19		/* 0xa6df7fe9 */

	add	%r16, %r26, %r27
	add	%r29, -$23, %r19
	addcc	%r16, %r26, %r27
	addcc	%r29, -$23, %r19
	and	%r16, %r26, %r27
	and	%r29, -$23, %r19
	andcc	%r16, %r26, %r27
	andcc	%r29, -$23, %r19
	andn	%r16, %r26, %r27
	andn	%r29, -$23, %r19
	andncc	%r16, %r26, %r27
	andncc	%r29, -$23, %r19
	ba	$0x3
	be	$0x3
	bg	$0x3
	bge	$0x3
	bl	$0x3
	ble	$0x3
	bne	$0x3				/* 0x12800003 */
	bz	$0x3
	ld	[%r4 + %r7], %r11
	ld	[%r23], %r19
	ldd	[%r4 + %r7], %r11
	ldd	[%r23], %r19
	ldsb	[%r4 + %r7], %r11
	ldsb	[%r23], %r19
	ldsh	[%r4 + %r7], %r11
	ldsh	[%r23], %r19
	ldub	[%r4 + %r7], %r11
	ldub	[%r23], %r19
	lduh	[%r4 + %r7], %r11
	lduh	[%r23], %r19
	nop
	or	%r16, %r26, %r27
	or	%r29, -$23, %r19
	orcc	%r16, %r26, %r27
	orcc	%r29, -$23, %r19
	orn	%r16, %r26, %r27
	orn	%r29, -$23, %r19
	orncc	%r16, %r26, %r27
	orncc	%r29, -$23, %r19
	sdiv	%r16, %r26, %r27
	sdiv	%r29, -$23, %r19
	sdivcc	%r16, %r26, %r27
	sdivcc	%r29, -$23, %r19
	sethi	$0x87654321, %r2		/* 0x0521d950 */
	smul	%r16, %r26, %r27
	smul	%r29, -$23, %r19
	smulcc	%r16, %r26, %r27
	smulcc	%r29, -$23, %r19
	st	%r11, [%r4 + %r7]
	st	%r19, [%r23]
	stb	%r11, [%r4 + %r7]
	stb	%r19, [%r23]
	std	%r11, [%r4 + %r7]
	std	%r19, [%r23]
	sth	%r11, [%r4 + %r7]
	sth	%r19, [%r23]
	udiv	%r16, %r26, %r27
	udiv	%r29, -$23, %r19
	udivcc	%r16, %r26, %r27
	udivcc	%r29, -$23, %r19
	umul	%r16, %r26, %r27
	umul	%r29, -$23, %r19
	umulcc	%r16, %r26, %r27
	umulcc	%r29, -$23, %r19
	xnor	%r16, %r26, %r27
	xnor	%r29, -$23, %r19
	xnorcc	%r16, %r26, %r27
	xnorcc	%r29, -$23, %r19
	xor	%r16, %r26, %r27
	xor	%r29, -$23, %r19
	xorcc	%r16, %r26, %r27
	xorcc	%r29, -$23, %r19
