; program to calculate gcd of two inputs org 100h
section .data
prompt0: db "This is a program to caculate the GCD of two inputs $"
prompt1: db "Please enter integer X: $"
prompt2: db "Please enter integer Y: $"
prompt3: db "The GCD is:    $"
intX     dw 0
;intY     dw 0
;gcd      dw 0
section .text
    mov ah,9        ; print prompt
    mov dx,prompt0
    int     21h
    mov ah,9        ; print prompt
    mov dx,prompt1
    int     21h
    call    dec_in
    mov [intX], bx


dec_in: 
; save registers
    push ax
    push dx

    xor bx,bx
    mov ah, 1
    int 21h
while1: 
    cmp al,0Dh      ; char = CR?
    je  finis       ; if so, we are done
    push    ax      ; save the character read
    mov ax,10       ; set up for multiply
    mul bx      ; dx:ax <- bx * 10
    mov bx,ax       ; put 16-bit result back in bx (assume no overflow)
    pop ax      ; restore the char read
    and ax,000Fh    ; convert character '0'-'9' to value 0-9
    add bx,ax       ; add value to accumulated input
    mov ah,1        ; read char fcn
    int 21h     ; read next char into al
    jmp while1      ; loop until done
finis:  
    ; restore registers
    pop dx
    pop ax
    ret