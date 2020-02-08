%include "lib.inc"
%include "macros.inc"

%define pc r15
%define w r14
%define rstack r13

section .data
%include "words.inc"

global _start

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

section .rodata
error_msg: db "Error: unknown word.", 0 

section .text

next:
    mov w, [pc]
    add pc, 8
    jmp [w]

_start:
    jmp i_init

