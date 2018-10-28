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
; returns rax: number, rdx : length
parse_uint:
    push rbx
    call string_length
    mov rdx, rax

    xor rsi, rsi            ; final value
    mov r11, 2              ; value for division
    mov r10, 10             ; value for multiplication
    mov r8, 0               ; char position

.next_char:
    xor rcx, rcx            ; bit position
    xor rbx, rbx              ; number

    mov r9b, [rdi + r8]
    sub r9b, '0'
    cmp r9b, 10
    ja .end
    mov rax, rsi
    push rdx
    mul r10                 ; multiply final value with 10
    pop rdx
    mov rsi, rax
    xor rax, rax
    mov al, r9b

.char_division:
    div r11b
    sal ah, cl
    or bl, ah
    cmp cl, 3
    je .add_value
    inc cl
    xor ah, ah
    jmp .char_division

.add_value:
    add rsi, rbx
    inc r8
    cmp r8, rdx
    je .end
    jmp .next_char

.end:
    mov rax, rsi
    mov rdx, r8
    pop rbx
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
  cmp byte [rdi], '-'
  je .parse_neg
  call parse_uint
  jmp .end

.parse_neg:
  inc rdi
  call parse_uint
  neg rax
  inc rdx

.end:
  ret

; rdi contains an unsigned int
; return rax: factorial
calc_factorial:
  test rdi, rdi
  mov rcx, 2
  mov rax, 1

.loop:
  cmp rcx, rdi
  ja .end
  mul rcx
  inc rcx
  jmp .loop

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
call parse_uint

mov rdi, rax
call calc_factorial

mov rdi, rax
call print_uint

mov rax, 60         ; use exit system call to shut down correctly
xor rdi, rdi
syscall