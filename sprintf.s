.include "cbm_petscii_charmap.inc"
;
; int __fastcall__ vsprintf (char* Buf, const char* Format, va_list ap);
;
; Ullrich von Bassewitz, 2009-09-26
;

        .export         _vsprintf
        .import         pushw0sp, staxysp
        .import         vsnprintf


; ----------------------------------------------------------------------------
; vsprintf - formatted output into a buffer
;
; int __fastcall__ vsprintf (char* buf, const char* format, va_list ap);
;


_vsprintf:
        pha                     ; Save low byte of ap
        txa
        pha                     ; Save high byte of op

; Build a stackframe for vsnprintf. To do that, we move format one word down,
; and store 0x7FFF (INT_MAX) as size.

        jsr     pushw0sp        ; Get format and push it
        lda     #$FF
        ldx     #$7F            ; INT_MAX
        ldy     #2
        jsr     staxysp

; Continue by jumping to vsnprintf, which expects ap on the CPU stack and will
; cleanup the C stack

        jmp     vsnprintf
        
;
; int sprintf (char* buf, const char* Format, ...);
;
; Ullrich von Bassewitz, 1.12.2000
;

        .export         _sprintf
        .import         pushax, addysp, decsp4
        .importzp       sp, ptr1

        .macpack        generic

; ----------------------------------------------------------------------------
; Data

.bss

ParamSize:      .res    1               ; Number of parameter bytes

; ----------------------------------------------------------------------------
; Code

.code


_sprintf:
        sty     ParamSize               ; Number of param bytes passed in Y

; We have to push buf and format, both in the order they already have on stack.
; To make this somewhat more efficient, we will create space on the stack and
; then do a copy of the complete block instead of pushing each parameter
; separately. Since the size of the arguments passed is the same as the size
; of the fixed arguments, this will allow us to calculate the pointer to the
; fixed size arguments easier (they're just ParamSize bytes away).

        jsr     decsp4

; Calculate a pointer to the Format argument

        lda     ParamSize
        add     sp
        sta     ptr1
        ldx     sp+1
        bcc     @L1
        inx
@L1:    stx     ptr1+1

; Now copy both, buf and format

        ldy     #4-1
@L2:    lda     (ptr1),y
        sta     (sp),y
        dey
        bpl     @L2

; Load va_list (last and __fastcall__ parameter to vsprintf)

        lda     ptr1
        ldx     ptr1+1

; Call vsprintf

        jsr     _vsprintf

; Cleanup the stack. We will return what we got from vsprintf

        ldy     ParamSize
        jmp     addysp
