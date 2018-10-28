; These macrodefinitions are copied from linux sources
; Linux is written in C, so the definitions looked a bit
; different there.
; We could have just looked up their values and use
; them directly inright places
; However it would have made the code much less legible

%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

section .data
; This is the file name. You are free to change it.
fname: db 'int.txt', 0

section .text
global _start

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


; rdi points to a string
; returns sum of all number's digits
add_digits:
    call string_length
    mov rdx, rax            ; rdx contains string length

    xor rax, rax            ; final value
    test rdx, rdx           ; if string is empty jump to end
    jz .end  

    xor rcx, rcx            ; char position
    xor r9, r9

    mov r9b, [rdi]
    cmp r9b, '-'            ; if first char is minus
    jne .next_char          

    inc rcx                 ; got to next char

.next_char:
    cmp rcx, rdx
    jae .end
    mov r9b, [rdi + rcx]    ; r9b now contains char
    sub r9b, '0'            ; convert char number to unsigned int
    cmp r9b, 10             ; if char is not a number
    ja .invalid_input
    add rax, r9             ; add unsigned int to final value (rsi)
    inc rcx
    jmp .next_char

.invalid_input:
    mov rax, 0

.end:
    ret


_start:
; call open
mov rax, 2
mov rdi, fname
mov rsi, O_RDONLY   ; Open file read only
mov rdx, 0          ; We are not creating a file
                    ; so this argument has no meaning
syscall

; mmap
mov r8, rax         ; rax holds opened file descriptor
                    ; it is the fourth argument of mmap
mov rax, 9          ; mmap number
mov rdi, 0          ; operating system will choose mapping destination
mov rsi, 4096       ; page size
mov rdx, PROT_READ  ; new memory region will be marked read only
mov r10, MAP_PRIVATE; pages will not be shared

mov r9, 0           ; offset inside test.txt
syscall             ; now rax will point to mapped location

mov rdi, rax
call add_digits

mov rdi, rax
call print_uint

mov rax, 60         ; use exit system call to shut down correctly
xor rdi, rdi
syscall