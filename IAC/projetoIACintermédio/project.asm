; ******************************************************************************
; * IST-UL
; * Alunos: Enzo Nunes 		ist1106336
;			João Ribeiro 	ist1107251
;			David Antunes	ist1107061
; * PROJETO
; * Descrição: Parte intermédia do projeto "Beyond Mars".
; ******************************************************************************


; ******************************************************************************
; * Constantes
; ******************************************************************************

DISPLAYS					EQU 0A000H			; endereço dos displays de 7 segmentos (periférico POUT-1)

; teclado
TEC_LIN						EQU 0C000H			; endereço das linhas do teclado (periférico POUT-2)
TEC_COL						EQU 0E000H			; endereço das colunas do teclado (periférico PIN)
LINHA						EQU 1				; primeira linha
MASCARA						EQU 0FH				; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
INCREMENT					EQU 4				; tecla que incrementa o valor dos displays
DECREMENT					EQU 5				; tecla que decrementa o valor dos displays 

; MediaCenter

APAGA_ECRA	 		     	EQU 6002H           ; endereço do comando para apagar todos os pixels já desenhados
DEFINE_LINHA    		 	EQU 600AH           ; endereço do comando para definir a linha
DEFINE_COLUNA   	 	 	EQU 600CH           ; endereço do comando para definir a coluna
DEFINE_PIXEL    	 	 	EQU 6012H           ; endereço do comando para escrever um pixel
APAGA_AVISO     		 	EQU 6040H           ; endereço do comando para apagar o aviso de nenhum cenário selecionado
SELECIONA_CENARIO_FUNDO		EQU 6042H           ; endereço do comando para selecionar uma imagem de fundo
	CENARIO					EQU 0
TOCA_SOM					EQU 605AH           ; endereço do comando para tocar um som

; cores
AMARELO						EQU 0FFE0H			; cor do pixel: amarelo
VERMELHO					EQU 0FF00H			; cor do pixel: vermelho
PRETO						EQU 0F000H			; cor do pixel: preto
BRANCO						EQU 0FFFFH			; cor do pixel: branco
CINZENTO					EQU 0FCCCH			; cor do pixel: cinzento
VERDE						EQU 0F5F0H			; cor do pixel: verde
LARANJA						EQU 0FF50H			; cor do pixel: laranja

; ******************************************************************************
; * Stacks
; ******************************************************************************

PLACE 1000H

; Reserva o espaço para as pilhas dos processos
	STACK 100H									; espaço reservado para a pilha do processo "programa principal"
SP_inicial:								; este é o endereço com que o SP deste processo deve ser inicializado

; ******************************************************************************
; * Tabelas
; ******************************************************************************

DEF_METEORO:									; boneco do meteoro
	WORD		5, 5							; largura e comprimento
	WORD		VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	WORD		0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD		VERMELHO , VERMELHO , 0		   , VERMELHO , VERMELHO
	WORD		0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD		VERMELHO , 0		, VERMELHO , 0		  , VERMELHO

; ******************************************************************************
; * Código Principal
; ******************************************************************************

PLACE	   0
inicializacoes:
    MOV SP, SP_inicial							; inicializacao do Stack Pointer
    MOV [APAGA_AVISO], R1						; apaga o aviso de nenhum cenário selecionado (R1 não é relevante)
    MOV [APAGA_ECRA], R1						; apaga todos os pixels já desenhados (R1 não é relevante)
	MOV R1, CENARIO
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário de fundo

CALL desenha_meteoro							; desenha o meteoro

CALL inicio_teclado

fim:
	JMP fim

; ******************************************************************************
; * Teclado
; ******************************************************************************

inicio_teclado:		
    MOV  R2, TEC_LIN							; endereço do periférico das linhas
    MOV  R3, TEC_COL							; endereço do periférico das colunas
    MOV  R4, DISPLAYS							; endereço do periférico dos displays
    MOV  R5, MASCARA							; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
	MOV  R6, 8									; constante para usar no CMP
	MOV  R7, 0						
	MOV  R8, 0									; onde � armazenada a tecla premida, entre 0 e FH
	MOV  R9, 0									; contador que vai ligar aos displays
	MOV SP, SP_inicial


MOVB [R4], R1									; escreve linha e coluna a zero nos displays
ciclo:						
    MOV  R1, 0									; inicializa a linha
	MOV  R0, 0         ; inicializa a coluna
	MOV  R1, LINHA     ; testar a linha 1

espera_tecla:          ; neste ciclo espera-se at� uma tecla ser premida
    MOVB [R2], R1      ; escrever no perif�rico de sa�da (linhas)
    MOVB R0, [R3]      ; ler do perif�rico de entrada (colunas)
    AND  R0, R5        ; elimina bits para al�m dos bits 0-3
    CMP  R0, 0         ; h� tecla premida?
    JZ   muda_linha    ; se nenhuma tecla premida na linha, muda de linha
	MOV  R7, R1        ; guarda a linha no R7 porque o R1 vai ser alterado 
	JMP  converte_num  ; caso contr�rio converte a tecla para um n�mero hexadecimal
	
muda_linha:
	CMP  R1, R6		   ; testa se a linha atual � a linha 4
	JZ   ciclo		   ; repete o ciclo
	SHL  R1, 1         ; muda de linha
	JMP  espera_tecla  ; testar a proxima linha

converte_num:          ; converte a tecla premida para um n�mero hexadecimal entre 0 e FH
	MOV  R8, 0         ; resultado final
	CALL converte_num_ciclo ; converte a linha        
	SHL  R8, 2         ; multiplica o n�mero correspondente � linha por quatro
	MOV  R1, R0         
	CALL converte_num_ciclo  ; converte a coluna
	JMP  verifica_tecla

converte_num_ciclo:    ; acumula o n�mero de SHR feitos at� o valor da linha/coluna ser 0    
	SHR  R1, 1         
	CMP  R1, 0         ; se o n�mero j� for 0
	JZ   converte_num_ret ; termina o ciclo
	ADD  R8, 1         ; adiciona ao resultado final
	JMP  converte_num_ciclo ; repete ciclo
	
converte_num_ret:
	RET

verifica_tecla:        ; verifica a tecla que foi premids para incrementar ou decrementar os displays          
	CMP R8, INCREMENT  ; verifica se a tecla � a tecla que incrementa
	JZ  incrementa_display
	CMP R8, DECREMENT  ; verifica se a tecla � a tecla que decrementa
	JZ  decrementa_display
	JMP ha_tecla       ; aguarda que a tecla deixe de ser premida

incrementa_display:
	ADD R9, 1          ; incrementa o contador
	MOVB [R4], R9      ; atualiza o display
	JMP ha_tecla       ; aguarda que a tecla deixe de ser premida

decrementa_display:
	SUB R9, 1          ; decrementa o contador
	MOVB [R4], R9       ; decrementa o contador
	JMP ha_tecla       ; aguarda que a tecla deixe de ser premida
	
ha_tecla:              ; neste ciclo espera-se at� NENHUMA tecla estar premida
    MOV  R1, R7        ; testar a �ltima linha usada (R1 tinha sido alterado)
    MOVB [R2], R1      ; escrever no perif�rico de sa�da (linhas)
    MOVB R0, [R3]      ; ler do perif�rico de entrada (colunas)
    AND  R0, R5        ; elimina bits para al�m dos bits 0-3
    CMP  R0, 0         ; h� tecla premida?
    JNZ  ha_tecla      ; se ainda houver uma tecla premida, espera at� n�o haver
	JMP  ciclo
	

; ******************************************************************************
; * Rotinas
; ******************************************************************************

; **********************************************************************
; desenha_meteoro - chama a funcao desenha_boneco com os argumentos
;                   necessarios para que esta desenhe o meteoro
;
; **********************************************************************

desenha_meteoro:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, 0
	MOV R2, 0
	MOV R3, DEF_METEORO
	CALL desenha_boneco
	POP R3
	POP R2
	POP R1
	RET

; **********************************************************************
; desenha_boneco - desenha um boneco respeitando os argumentos
; argumentos - 		R1 - linha
;					R2 - coluna
;					R3 - tabela
; **********************************************************************

desenha_boneco:									; desenha o boneco a partir da tabela
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    PUSH R6
    PUSH R7
	PUSH R8
	MOV R8, R2									; guarda a coluna inicial
	MOV	R5, [R3]								; obtém a largura do boneco
	MOV R7, R5									; decrementador da coluna
	ADD R3, 2									; endereço da tabela que define o comprimento do boneco
	MOV R6, [R3]								; obtém o comprimento do boneco
	ADD	R3, 2									; endereço da cor do 1º pixel (2 porque a largura é uma word)
	desenha_linha:       						; desenha os pixels do boneco a partir da tabela
		MOV	R4, [R3]							; obtém a cor do próximo pixel do boneco
		MOV  [DEFINE_LINHA], R1					; seleciona a linha
		MOV  [DEFINE_COLUNA], R2				; seleciona a coluna
		MOV  [DEFINE_PIXEL], R4					; altera a cor do pixel na linha e coluna selecionadas
		ADD	R3, 2								; endereço da cor do próximo pixel (2 porque cada cor de pixel é uma word)
		ADD  R2, 1              				; próxima coluna
		SUB R7, 1								; menos uma coluna para tratar
		JNZ desenha_linha						; continua até percorrer toda a largura do objeto
		MOV R2, R8								; volta á primeira linha
		ADD R1, 1
		MOV R7, R5
		SUB R6, 1
		JNZ desenha_linha
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	RET