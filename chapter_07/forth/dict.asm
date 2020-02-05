section .text
global find_word
global cfa
extern string_equals
extern string_length

; rdi points to string we try to find
; rsi points to last value of list
find_word:
.loop:
    test rsi, rsi
    jz .no_match
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .match
    mov rsi, [rsi]
    jmp .loop

.no_match:
    xor rax, rax
    jmp .end

.match:
    mov rax, rsi

.end:
    ret

; rdi points to word header start
cfa:
    add rdi, 8
    push rdi
    call string_length
    pop rdi
    add rax, rdi
    add rax, 2     ; 1b for 0 of zero terminated str and 1b for header flags
    ret 