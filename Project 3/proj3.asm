; Name: CJ Hess
; Date: 11/23/ 2022
; File proj3.asm
; Calculate GCD of 2 Numbers
section		.data
prompt1:	db "Enter first number: "
prompt2:	db "Enter second number: "
gcdTxt:		db "The GCD is: "
const10:    dd 10
section 	.bss
lbl: resb	0x1		;trying to print 1 char as a string
intA: resb 64
intB: resb 64
section		.text		; this is a comment
	global		_start
_start:
	; print prompt
	mov		rdx, 20
	mov		rcx, prompt1
	mov		rax, 4
	mov		rbx, 1
	int		0x80

	; read from keyboard (stdin=0), no more than 64 bytes (chars)
	mov		rax, 3
	mov		rbx, 0
	mov		rcx, intA
	mov		rdx, 64
	int		0x80

	; these lines give the size of string without newline
	; size is in rcx
	mov rcx, rax
	dec rcx

	call convertIntA
	mov r10, rax

	; print prompt
	mov		rdx, 21
	mov		rcx, prompt2
	mov		rax, 4
	mov		rbx, 1
	int		0x80

	; read from keyboard (stdin=0), no more than 100 bytes (chars)
	mov		rax, 3
	mov		rbx, 0
	mov		rcx, intB
	mov		rdx, 100
	int		0x80

	; these lines give the size of string without newline
	; size is in rcx
	mov rcx, rax
	dec rcx

	call convertIntB
	mov r11, rax

	; Print out label for GCD
	mov		rdx, 12
	mov		rcx, gcdTxt
	mov		rax, 4
	mov		rbx, 1
	int		0x80

	mov rax, r10
	mov rbx, r11
	; Find the GCD and print it
	call gcd
	call printNumber

	;  return to linux
	mov		rax, 1
	mov		rbx, 0
	int		0x80

; Use sumDigit as a reference
; Converts the first input and store into intA
; Input:
; RCX: Length of string
; Output:
; RAX: Integer Value
convertIntA:
	;  want 	rdx+= al
	mov	rax, 0	; clear out rax before proceeding
	mov rbx, 0  ; position in string
	mov	rdx, 0	; sum

	cmp rcx, 0
	jne addNextDigitA

addNextDigitA:
	mov rax, 0
	mov	al, [intA+rbx] ;pull next CHAR from memory
	sub	al, '0'	       ;convert char to numeric equivalent
	jmp adjustPower10A

adjustPower10A:
	cmp rcx, 0
	je endConvertA
	mov rdi, rcx
	push rdx
	call calculatePowerTen
	pop rdx
	add	rdx, rax
	inc rbx
	dec rcx
	jmp addNextDigitA

endConvertA:
	mov rax, rdx
	ret
	

; Use sumDigit as a reference
; Converts the second input and store into intB
; Input:
; RCX: Length of string
; Output:
; RAX: Integer Value
convertIntB:
	;  want 	rdx+= al
	mov	rax, 0	; clear out rax before proceeding
	mov rbx, 0  ; position in string
	mov	rdx, 0	; sum

	cmp rcx, 0
	jne addNextDigitB

addNextDigitB:
	mov rax, 0
	mov	al, [intB+rbx] ;pull next CHAR from memory
	sub	al, '0'	       ;convert char to numeric equivalent
	jmp adjustPower10B

adjustPower10B:
	cmp rcx, 0
	je endConvertB
	mov rdi, rcx
	push rdx
	call calculatePowerTen
	pop rdx
	add	rdx, rax
	inc rbx
	dec rcx
	jmp addNextDigitB

endConvertB:
	mov rax, rdx
	ret	

; Calculates the power of ten given a power
; Used to apply the actual value of the number based on position in string
; Input
; RAX = Base
; RDI = Power
; Output
; RAX = The result
calculatePowerTen:
	cmp RDI, 1
	jg mul10
	jmp done
mul10:
	mov r9, 10
	mul r9
	sub rdi, 1
	jmp calculatePowerTen
done:
	ret

; Used to print a character
; Which is used as an assistance for printNumber
printChar:

	;;  back up all the registers we want to use here in this function call
	push 	rax
	push	rbx
	push	rcx
	push	rdx
	
	;;  get AL into the string starting at lbl
	mov	[lbl], al

	;; print out string
	mov	eax, 4		;Linux syscall #4 is print string
	mov	ebx, 1		;tell Linux syscall where to print (1=stdout)
	mov	ecx, lbl	;store address of string for Linux syscall
	mov	edx, 1		; print 1 character
	int	0x80 		;ask Linux to do something for me

	;; holy printing, batman! We're done!

	;; restore all registers from stack
	pop	rdx
	pop	rcx
	pop	rbx
	pop	rax
	
	ret

; Used to print number
; Input
; RAX = Number to print
printNumber:
    push rax
    push rdx
    xor rdx,rdx          ;edx:eax = number
    div dword [const10]  ;eax = quotient, edx = remainder
    test rax,rax         ;Is quotient zero?
    je l1               ; yes, don't display it
    call printNumber     ;Display the quotient
l1:
    lea rax,[rdx+'0']
    call printChar  ;Display the remainder
    pop rdx
    pop rax
    ret


; Input
; RAX = int A
; RBX = int B
; Output
; RAX = gcd
gcd:
	mov r9, rax
	mov r10, rbx
	cmp r10, 0
	je baseCase
	cmp r9, r10
	jl switchCase
	jmp modCase
modCase:
	mov rax, r9
	mov rdx, 0

	mov rbx, r10
	div rbx

	mov rax, rbx
	mov rbx, rdx
	call gcd
switchCase:
	mov rax, r10
	mov rbx, r9
	call gcd
baseCase:
	mov rax, r9
	jmp end
end:
	ret