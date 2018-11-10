
%define concat(x,y)  x %+ y

%assign limit 30
%define prime_bits 100b
%assign n 3
%rep limit 
    %assign current 1
    %assign i 1
    %rep n/2
        %assign i i+1
        %if n % i = 0
            %assign current 0
            %exitrep
        %endif
    %endrep
    %xdefine prime_bits concat(current, prime_bits)
    %assign n n+1
%endrep

section .data
is_prime: dd prime_bits

section .text
global _start

; this function checks if the number in rdi is a primer number
; returns 1 if it is, otherwise 0
is_prime_number:
    mov eax, [is_prime]
    mov rcx, rdi
    sar rax, cl
    and rax, 1
    ret

; These functions are used to print a null terminated string
print_string:
    push rdi
    call string_length
    pop rsi
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret
string_length:
    xor rax, rax
.loop:
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

; These functions are used to print an unsigned int
print_uint:
    mov rax, rdi
    mov rdi, rsp 
    push 0
    dec rdi
    sub rsp, 16
    mov r8, 10

.loop:
    xor rdx, rdx
    div r8
    or  dl, 0x30
    dec rdi
    mov [rdi], dl
    test rax, rax
    jnz .loop

    call print_string

    add rsp, 24

.end:
    ret

_start:
    mov rdi,11d
    call is_prime_number
    mov rdi, rax
    call print_uint

    mov rax, 60         ; use exit system call to shut down correctly
    xor rdi, rdi
    syscall
