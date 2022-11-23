; To compile: nasm -gstabs -felf64 -l <file>.lst <file>.asm
; To link object file: ld <file>.o -o <executable>

; Prompt for positive integer number 1
; Prompt for positive integer number 2

; Print out GCD between num 1 and 2

section .data
prompt1: db "Enter the first positive integer number: "
prompt2: db "Enter the second positive integer number: "
num1: resb 100
num2: resb 100
    section .text
    global _start
_start:
    mov rax, 4 ; write
    mov rbx, 1 ; stdout
    mov rcx, prompt1 ; where characters start
    mov rdx, 41 ; 41 characters
    int 0x80
    mov rax, 3 ; read
    mov rbx, 0 ; stdin
    mov rcx, num1 ; first number
    mov rdx, 100 ; no more than 0x64 chars
    int 0x80
    mov rax, 4 ; write
    mov rbx, 1 ; stdout
    mov rcx, prompt2 ; where characters start
    mov rdx, 42 ; 42 characters
    int 0x80
    mov rax, 3 ; read
    mov rbx, 0 ; stdin
    mov rcx, num2 ; first number
    mov rdx, 100 ; no more than 0x64 chars
    int 0x80
    ; put the nums on stack and call function
    pushl num1
    pushl num2
    call gcd
    ; Return to Linux
    mov rax, 1
    mov rbx, 0
    int 0x80

gcd:

    cmp b, 0 ; compare b to 0
    je