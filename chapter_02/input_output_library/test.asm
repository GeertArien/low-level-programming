section .data

number: db '1', 0

section .text
global _start

string_length:
    xor rax, rax

.loop:
    cmp byte [rdi+rax],0
    je .end

    inc rax
    jmp .loop

.end:
    ret


; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
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
    ret


_start:
  mov rdi, number
  call parse_uint

  mov rax, 60
  xor rdi, rdi
  syscall
