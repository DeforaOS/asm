/* $Id$ */
.text
	emms					/* 0f 77		*/
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
