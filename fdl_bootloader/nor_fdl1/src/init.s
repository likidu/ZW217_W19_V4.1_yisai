;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and
; interrupts for each mode, and finally branches to __main in the C library (which
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

        PRESERVE8
        AREA    Init, CODE, READONLY

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_SVC_NO_IRQ EQU     0xD3 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled
	IF (:DEF:PLATFORM_UWS6121E)
SVC_STACK_TEMP  EQU     0x50813FFC ;// SVC temp stack
	ELSE
SVC_STACK_TEMP  EQU     0x40002000 ;// SVC temp stack
	ENDIF

	ENTRY

	IMPORT  Chip_Init
	IMPORT  stack_bottom
	IMPORT  __main

	EXPORT  Reset_Handler
	CODE32
	EXPORT  __vectors
__vectors
    LDR     pc,=Reset_Handler            ; Reset goes to startup function
    LDR     pc,=__undefined                 ; Undefined handler
    LDR     pc,=__swi_interrupt             ; Software interrupt handler
    LDR     pc,=__prefetch_handler       ; Prefetch exeception handler
    LDR     pc,=__abort_handler           ; Abort exception handler
    LDR     pc,=__reserved_handler       ; Reserved exception handler
    LDR     pc,=__irq_handler               ; IRQ interrupt handler
    LDR     pc,=__fiq_handler               ; FIQ interrupt handler

Reset_Handler
; Disable MMU,cache and transfer to big_endian mode.
	MRC p15,0,r0,c1,c0,0
	BIC r0,r0,#1                            ; disable MMU
	LDR r1,=0x1004
	BIC r0,r0,r1                            ; disable cache
	MCR p15,0,r0,c1,c0,0

; --- Initialise Hardware
; --- Initialise ARM common registers
	MOV		r0, #0
	MOV		r1, #0
	MOV		r2, #0
	MOV		r3, #0
	MOV		r4, #0
	MOV		r5, #0
	MOV		r6, #0
	MOV		r7, #0
	MOV		r8, #0
	MOV		r9, #0
	MOV		r10,#0
	MOV		r11,#0
	MOV		r12,#0
	MOV		sp, #0
	MOV		lr, #0

; --- Initialise stack pointer registers
; Enter SVC mode and set up the SVC stack pointer
	MOV     a1, #Mode_SVC_NO_IRQ     ; Build SVC mode CPSR
	MSR     CPSR_c, a1                      ; Enter SVC mode

	LDR    sp, =SVC_STACK_TEMP

; Initialize the pSRAM
	BL     Chip_Init

	LDR     a2, [pc, #SVC_STACK-.-8]      ; Pickup SVC mode stack address
	LDR     a2, [a2,#0]
	MOV    sp, a2                                  ; Setup SVC MODE stack pointer

; --- Now enter the C code
      B       __main   ; note use B not BL, because an application will never return this way

    EXPORT  __undefined
__undefined
    b __undefined

    EXPORT  __swi_interrupt
__swi_interrupt
    b __swi_interrupt

    EXPORT  __prefetch_handler
__prefetch_handler
    b __prefetch_handler

    EXPORT  __abort_handler
__abort_handler
    b __abort_handler

    EXPORT  __reserved_handler
__reserved_handler
    b __reserved_handler

    EXPORT  __irq_handler
__irq_handler
    b __irq_handler

    EXPORT  __fiq_handler
__fiq_handler
    b __fiq_handler

; --- Now enter the FDL code
; jump to target address
; on entry
; r0 = int startaddr
; no exit

	EXPORT __user_initial_stackheap
__user_initial_stackheap
	LDR		r0, =SVC_STACK
	MOV		pc, lr

	EXPORT  JumpToTarget
JumpToTarget
	MOV  pc, r0

SVC_STACK
    DCD     stack_bottom

    END
