	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 1
	.globl	_configfile                     ; -- Begin function configfile
	.p2align	2
_configfile:                            ; @configfile
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	_coinr
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function coinr
_coinr:                                 ; @coinr
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x19, _max_index@PAGE
Lloh0:
	adrp	x20, _charset@PAGE
Lloh1:
	add	x20, x20, _charset@PAGEOFF
LBB1_1:                                 ; =>This Inner Loop Header: Depth=1
	bl	_rand
	ldr	w8, [x19, _max_index@PAGEOFF]
	sdiv	w9, w0, w8
	msub	w8, w9, w8, w0
	add	x1, x20, w8, sxtw
	mov	x0, #0
	mov	w2, #1
	bl	_strncpy
	b	LBB1_1
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	x0, #0
	bl	_time
	bl	_configfile
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

.subsections_via_symbols
