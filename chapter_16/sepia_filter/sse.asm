section .text
global sse

; rdi = x, rsi = y

sse:
    movdqa xmm0, [rdi]
    movdqa xmm1, [rdi + 16]
    movdqa xmm2, [rdi + 32]
    mulps xmm0, [rsi]
    mulps xmm1, [rsi + 16]
    mulps xmm2, [rsi + 32]
    addps xmm0, xmm1
    addps xmm0, xmm2
    movdqa [rdi], xmm0
    ret
