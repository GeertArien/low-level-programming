%include "lib.inc"
%include "macros.inc"

%define pc r15
%define w r14
%define rstack r13

section .data
%include "words.inc"

global _start
global w_create
extern find_word
extern cfa


section .bss
stack_base: resq 1
input_buf: resb 1024
user_mem: resq 65536
dict_ext: resq 65536
resq 65535
rstack_start: resq 1

section .data
state: db 0         ; 0 = interpeter, 1 = compiler
here: dq dict_ext
last_word: dq last_item
error_msg: db "Error: unknown word.", 0 

program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop

section .text

next:
    mov w, [pc]
    add pc, 8
    jmp [w]

interpreter_loop:
    mov sil, [state]
    test sil, sil
    jnz .compiler_loop

    mov rdi, input_buf
    mov rsi, 1024
    call read_word

    mov rdi, input_buf
    call string_length
    test rax, rax
    jz .end

    mov rdi, input_buf
    mov rsi, last_word
.find_int:
    call find_word
    test rax, rax
    jz .invalid_word
    mov rdi, rax
    call cfa
    mov [program_stub], rax
    mov pc, program_stub
    jmp next
    
.invalid_word:
    mov rdi, input_buf
    call parse_int
    test rdx, rdx
    jz .invalid_input
    push rax
    jmp interpreter_loop

.invalid_input:
    mov rdi, error_msg
    call print_error

.end:
    mov rax, 60         ; use exit system call to shut down correctly
    xor rdi, rdi
    syscall

.compiler_loop:
    mov rdi, input_buf
    mov rsi, 1024
    call read_word

    mov rdi, input_buf
    call string_length
    test rax, rax
    jz .end

    mov rdi, input_buf
    mov rsi, last_word
    call find_word
    test rax, rax
    jz .not_word
    mov rdi, rax
    call cfa
    mov rdi, rax
    dec rdi             ; decrease by 1 for flag address
    mov sil, [rdi]
    test sil, sil
    jz .add_word

    mov [program_stub], rax             ; interpret word
    mov pc, program_stub
    jmp next

.add_word:
    mov rdi, [here]
    mov [rdi], rax
    add rdi, 8
    mov [here], rdi
    jmp .compiler_loop

.not_word:
    mov rdi, input_buf
    call parse_int
    test rdx, rdx
    jz .invalid_input

    mov rdi, [here]
    sub rdi, 8
    mov rdi, [rdi]
    mov rsi, xt_branch
    cmp rdi, rsi
    je .add_number
    mov rsi, xt_0branch
    cmp rdi, rsi
    je .add_number

    mov rdi, [here]
    mov qword[rdi], xt_lit
    add rdi, 8
    mov [here], rdi

.add_number:
    mov rdi, [here]
    mov [rdi], rax
    add rdi, 8
    mov [here], rdi
    jmp .compiler_loop

_start:
    mov [stack_base], rsp   ; save stack base address
    mov rstack, rstack_start
    mov rdi, [xt_interpreter]
    mov [program_stub], rdi
    mov pc, program_stub
    jmp next

