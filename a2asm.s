; ---------------------------------------------------------
; void DrawCharASM(byte index, byte xpos, byte ypos)
; cc65 calling convention:
;   A = index
;   X = xpos
;   Y = ypos
; ---------------------------------------------------------

.export _DrawCharASM
.import charset, RowsHGR, HGR

.segment "CODE"

_DrawCharASM:
    ; Save parameters
    sta index
    stx xpos
    sty ypos

    ; Compute src = charset + (index << 3)
    lda index
    asl
    asl
    asl
    tax                     ; X = index*8

    lda #<charset
    sta srcptr
    lda #>charset
    sta srcptr+1

    clc
    lda srcptr
    adc #0
    sta srcptr
    bcc :+
    inc srcptr+1
:

    ; Compute row = ypos << 3
    lda ypos
    asl
    asl
    asl
    tay                     ; Y = ypos*8

    ; Compute dst = HGR + RowsHGR[y] + xpos
    lda RowsHGR,y
    clc
    adc xpos
    sta dstptr
    lda RowsHGR+1,y
    adc #0
    sta dstptr+1

    ; Add HGR base
    lda dstptr
    clc
    adc #<HGR
    sta dstptr
    lda dstptr+1
    adc #>HGR
    sta dstptr+1

    ; -----------------------------------------------------
    ; Unrolled 8‑row blit
    ; -----------------------------------------------------

    ; Row 0
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 1
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 2
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 3
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 4
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 5
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 6
    lda charset,x
    sta (dstptr)
    inx
    lda dstptr
    clc
    adc #$00
    sta dstptr
    lda dstptr+1
    adc #$04
    sta dstptr+1

    ; Row 7
    lda charset,x
    sta (dstptr)

    rts

