/* $Id$ */
.text
	aaload
	aastore
	aconst_null
	aload		$0x01
	aload_0
	aload_1
	aload_2
	aload_3
	areturn
	arraylength
	astore		$0x01
	astore_0
	astore_1
	astore_2
	astore_3
	athrow
	baload
	bastore
	bipush		$0x01
	caload
	castore
	checkcast	$0x01
	d2f
	d2i
	d2l
	dadd
	daload
	dastore
	dcmpg
	dcmpl
	dconst_0
	dconst_1
	ddiv
	dload		$0x01
	dload_0
	dload_1
	dload_2
	dload_3
	dmul
	dneg
	drem
	dreturn
	dstore		$0x01
	dstore_0
	dstore_1
	dstore_2
	dstore_3
	dsub
	dup
	dup_x1
	dup_x2
	dup2
	dup2_x1
	dup2_x2
	f2d
	f2i
	f2l
	fadd
	faload
	fastore
	fcmpg
	fcmpl
	fconst_0
	fconst_1
	fconst_2
	fdiv
	fload		$0x01
	fload_0
	fload_1
	fload_2
	fload_3
	fmul
	fneg
	frem
	freturn
	fstore_0
	fstore_1
	fstore_2
	fstore_3
	fsub
	getfield	$0x01
	getstatic	$0x01
	goto		$0x01
	goto_w		$0x01
	i2b
	i2c
	i2d
	i2f
	i2l
	i2s
	iadd
	iaload
	iand
	iastore
	iconst_m1
	iconst_0
	iconst_1
	iconst_2
	iconst_3
	iconst_4
	iconst_5
	idiv
	if_acmpeq	$0x01
	if_acmpne	$0x01
	if_icmpeq	$0x01
	if_icmpne	$0x01
	if_icmplt	$0x01
	if_icmpge	$0x01
	if_icmpgt	$0x01
	if_icmple	$0x01
	ifeq		$0x01
	ifne		$0x01
	iflt		$0x01
	ifge		$0x01
	ifgt		$0x01
	ifle		$0x01
	ifnonnull	$0x01
	ifnull		$0x01
	iinc		$0x01, $0x02
	iload		$0x01
	iload_0
	iload_1
	iload_2
	iload_3
	impdep1
	impdep2
	imul
	ineg
	instanceof	$0x01
	invokeinterface	$0x01, $0x02
	invokespecial	$0x01
	invokestatic	$0x01
	invokevirtual	$0x01
	ior
	irem
	ireturn
	ishl
	ishr
	istore		$0x01
	istore_0
	istore_0
	istore_0
	istore_3
	isub
	iushr
	ixor
	jsr		$0x01
	jsr_w		$0x01
	l2d
	l2f
	l2i
	ladd
	laload
	land
	lastore
	lcmp
	lconst_0
	lconst_1
	ldc		$0x01
	ldc_w		$0x01
	ldc2_w		$0x01
	ldiv
	lload		$0x01
	lload_0
	lload_1
	lload_2
	lload_3
	lmul
	lneg
	lor
	lrem
	lreturn
	lookupswitch
	lshl
	lshr
	lstore		$0x01
	lstore_0
	lstore_1
	lstore_2
	lstore_3
	lsub
	lushr
	lxor
	monitorenter
	monitorexit
	multianewarray	$0x01, $0x02
	new		$0x01
	newarray	$0x01
	nop
	pop
	pop2
	putfield	$0x01
	putstatic	$0x01
	ret		$0x01
	return
	saload
	sastore
	sipush		$0x01
	swap
	tableswitch	$0x0, $0x0, $0x0
	wide		$0x01, $0x02
	wide		$0x01, $0x02, $0x03
	xxxunusedxxx
#if 0 /* XXX not supported again yet */
	/* db */
	db		$0x04
#endif
