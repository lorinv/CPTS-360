.global main, mymain, myprintf
.data
	FMT:	.asciz "main() in assembly call mymain() in C\n"
	a:	.long 1234
	b:	.long 5678
	fmt:	.asciz "a=%d b=%d\n"

.text

main:	 
	pushl	%ebp
	movl	%esp, %ebp
		
	
	call myprintf
		
