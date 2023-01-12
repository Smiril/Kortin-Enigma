	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 1
	.globl	_configfile                     ; -- Begin function configfile
	.p2align	2
_configfile:                            ; @configfile
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #112
	stp	x24, x23, [sp, #48]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	mov	w21, #0
Lloh0:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh1:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh2:
	ldr	x8, [x8]
	str	x8, [sp, #40]
	adrp	x19, _max_index@PAGE
Lloh3:
	adrp	x20, _charset@PAGE
Lloh4:
	add	x20, x20, _charset@PAGEOFF
LBB0_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_2 Depth 2
	mov	w22, #26
LBB0_2:                                 ;   Parent Loop BB0_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	bl	_rand
	ldr	w23, [x19, _max_index@PAGEOFF]
	sdiv	w8, w0, w23
	msub	w8, w8, w23, w0
	add	x1, x20, w8, sxtw
	mov	x0, #0
	bl	_strcpy
	sub	w8, w23, #1
	str	w8, [x19, _max_index@PAGEOFF]
	subs	w22, w22, #1
	b.ne	LBB0_2
; %bb.3:                                ;   in Loop: Header=BB0_1 Depth=1
	add	x0, sp, #14
	bl	_puts
	add	w21, w21, #1
	cmp	w21, #6
	b.ne	LBB0_1
; %bb.4:
	mov	w21, #26
LBB0_5:                                 ; =>This Inner Loop Header: Depth=1
	bl	_rand
	ldr	w22, [x19, _max_index@PAGEOFF]
	sdiv	w8, w0, w22
	msub	w8, w8, w22, w0
	add	x1, x20, w8, sxtw
	mov	x0, #0
	bl	_strcpy
	sub	w8, w22, #1
	str	w8, [x19, _max_index@PAGEOFF]
	subs	w21, w21, #1
	b.ne	LBB0_5
; %bb.6:
	add	x0, sp, #14
	bl	_puts
	ldr	x8, [sp, #40]
Lloh5:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh6:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh7:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB0_8
; %bb.7:
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #112
	ret
LBB0_8:
	bl	___stack_chk_fail
	.loh AdrpAdd	Lloh3, Lloh4
	.loh AdrpLdrGotLdr	Lloh0, Lloh1, Lloh2
	.loh AdrpLdrGotLdr	Lloh5, Lloh6, Lloh7
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x0, #0
	bl	_time
	mov	x19, x0
	bl	_configfile
	mov	x0, #0
	bl	_time
	stp	x19, x0, [sp]
Lloh8:
	adrp	x0, l_.str.1@PAGE
Lloh9:
	add	x0, x0, l_.str.1@PAGEOFF
	bl	_printf
	mov	w0, #0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpAdd	Lloh8, Lloh9
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
	.globl	_charset                        ; @charset
_charset:
	.asciz	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

	.section	__DATA,__data
	.globl	_max_index                      ; @max_index
	.p2align	2
_max_index:
	.long	26                              ; 0x1a

	.section	__TEXT,__cstring,cstring_literals
l_.str.1:                               ; @.str.1
	.asciz	"Time elapsed : begin %ld - end %ld \n"

.subsections_via_symbols
