/* $Id$ */
.text
	emms					/* 0f 77		*/
#if 1 /* FIXME doesn't work properly */
	movd	%mm1, [%edx]
	movd	%mm2, [%edx + $0x56]
	movd	%mm3, [%edx + $0x789abc]
	movd	%mm4, %mm5
#endif
	movd	[%edx], %mm2
	movd	[%edx + $0x56], %mm3
	movd	[%edx + $0x789abc], %mm4
#if 1 /* FIXME doesn't work properly */
	movd	%mm5, %mm6
#endif
#if 1 /* FIXME doesn't work properly */
	movq	%mm1, [%edx]
	movq	%mm2, [%edx + $0x56]
	movq	%mm3, [%edx + $0x789abc]
	movq	%mm4, %mm5
#endif
	movq	[%edx], %mm2
	movq	[%edx + $0x56], %mm3
	movq	[%edx + $0x789abc], %mm4
#if 1 /* FIXME doesn't work properly */
	movq	%mm5, %mm6
#endif
#if 1 /* FIXME doesn't work properly */
	paddb	%mm1, [%edx]
	paddb	%mm2, [%edx + $0x56]
	paddb	%mm3, [%edx + $0x789abc]
	paddb	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	paddd	%mm1, [%edx]
	paddd	%mm2, [%edx + $0x56]
	paddd	%mm3, [%edx + $0x789abc]
	paddd	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	paddw	%mm1, [%edx]
	paddw	%mm2, [%edx + $0x56]
	paddw	%mm3, [%edx + $0x789abc]
	paddw	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	paddsb	%mm1, [%edx]
	paddsb	%mm2, [%edx + $0x56]
	paddsb	%mm3, [%edx + $0x789abc]
	paddsb	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	paddsw	%mm1, [%edx]
	paddsw	%mm2, [%edx + $0x56]
	paddsw	%mm3, [%edx + $0x789abc]
	paddsw	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	pand	%mm1, [%edx]
	pand	%mm2, [%edx + $0x56]
	pand	%mm3, [%edx + $0x789abc]
	pand	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	pandn	%mm1, [%edx]
	pandn	%mm2, [%edx + $0x56]
	pandn	%mm3, [%edx + $0x789abc]
	pandn	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	por	%mm1, [%edx]
	por	%mm2, [%edx + $0x56]
	por	%mm3, [%edx + $0x789abc]
	por	%mm4, %mm5
#endif
#if 1 /* FIXME doesn't work properly */
	pxor	%mm1, [%edx]
	pxor	%mm2, [%edx + $0x56]
	pxor	%mm3, [%edx + $0x789abc]
	pxor	%mm4, %mm5
#endif
