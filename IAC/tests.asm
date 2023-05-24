N	EQU 15

start:
	MOV R1, N
	MOV R2, N

cycle:
	SUB R1, 1
	MUL R2, R1
	JV ERR
	CMP R1, 1
	JGT cycle

end:
	JMP end