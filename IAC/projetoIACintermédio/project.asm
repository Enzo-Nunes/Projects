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

; Teclado
TEC_LIN						EQU 0C000H			; endereço das linhas do teclado (periférico POUT-2)
TEC_COL						EQU 0E000H			; endereço das colunas do teclado (periférico PIN)
LINHA						EQU 1				; primeira linha
MASCARA						EQU 0FH				; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
INCREMENT					EQU 0AH				; tecla que incrementa o valor dos displays
DECREMENT					EQU 0BH				; tecla que decrementa o valor dos displays
MEXE_METEORO				EQU 0EH				; tecla que mexe o meteoro
MEXE_SONDA					EQU 0FH

; MediaCenter

APAGA_ECRA	 		     	EQU 6002H           ; endereço do comando para apagar todos os pixels já desenhados
DEFINE_LINHA    		 	EQU 600AH           ; endereço do comando para definir a linha
DEFINE_COLUNA   	 	 	EQU 600CH           ; endereço do comando para definir a coluna
DEFINE_PIXEL    	 	 	EQU 6012H           ; endereço do comando para escrever um pixel
APAGA_AVISO     		 	EQU 6040H           ; endereço do comando para apagar o aviso de nenhum cenário selecionado
SELECIONA_CENARIO_FUNDO		EQU 6042H           ; endereço do comando para selecionar uma imagem de fundo
	CENARIO					EQU 0
TOCA_SOM					EQU 605AH           ; endereço do comando para tocar um som
	METEORO_MEXE			EQU 0

; Dimensões Objetos

ALTURA_METEORO				EQU 5				;altura do objeto meteoro
LARGURA_METEORO				EQU 5				;largura do objeto meteoro
LIN_METEORO_INIC			EQU 0				;linha referencia do meteoro
COL_METEORO_INIC			EQU 0				;coluna referencia do meteoro

ALTURA_SONDA				EQU 1				;altura do objeto sonda
LARGURA_SONDA				EQU 1				;largura do objeto sonda
LIN_SONDA_INIC				EQU 26				;linha referencia da sonda
COL_SONDA_INIC				EQU 32				;coluna referencia da sonda	

ALTURA_PAINEL				EQU 5				;altura do objeto painel
LARGURA_PAINEL				EQU 15				;largura do objeto painel
LIN_PAINEL_INIC				EQU 27				;linha referencia do painel
COL_PAINEL_INIC				EQU 25				;linha referencia do painel

; Cores

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
SP_inicial:										; este é o endereço com que o SP deste processo deve ser inicializado

; ******************************************************************************
; * Tabelas
; ******************************************************************************

VALOR_DISPLAYS: WORD 0

DEF_METEORO:									; objeto do meteoro
	WORD		ALTURA_METEORO, LARGURA_METEORO	; dimensões do meteoro
	WORD		VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	WORD		0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD		VERMELHO , VERMELHO , 0		   , VERMELHO , VERMELHO
	WORD		0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD		VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	
REF_METEORO:									; pixel de referencia do meteoro
	WORD LIN_METEORO_INIC, COL_METEORO_INIC

DEF_SONDA:										; objeto da sonda
	WORD ALTURA_SONDA, LARGURA_SONDA			; dimensões da sonda
	WORD LARANJA

REF_SONDA:										; pixel de referencia da sonda
	WORD LIN_SONDA_INIC, COL_SONDA_INIC
	
DEF_PAINEL:										; tabela que define o objeto (cor, largura, pixels)
	WORD        ALTURA_PAINEL, LARGURA_PAINEL
	WORD		0, 0, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, 0, 0
	WORD		0, VERMELHO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, VERMELHO, 0
	WORD		VERMELHO, BRANCO, BRANCO, BRANCO, LARANJA, VERDE, VERMELHO, LARANJA, VERDE, VERMELHO, LARANJA, BRANCO, BRANCO, BRANCO, VERMELHO
    WORD		VERMELHO, BRANCO, BRANCO, BRANCO, VERMELHO, LARANJA, VERDE, VERMELHO, LARANJA, VERDE, VERMELHO, BRANCO, BRANCO, BRANCO, VERMELHO
	WORD        VERMELHO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, BRANCO, VERMELHO

REF_PAINEL:										; pixel de referencia do painel
	WORD LIN_PAINEL_INIC, COL_PAINEL_INIC

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
	MOV R1, 0
	MOV [DISPLAYS], R1								; escreve linha e coluna a zero nos displays

CALL desenha_meteoro							; desenha o meteoro
CALL desenha_sonda								; desenha a sonda
CALL desenha_painel

ciclo_teclado:
	CALL inicio_teclado
	JMP verifica_tecla

fim:
	JMP fim

; ******************************************************************************
; * Teclado
; ******************************************************************************

inicio_teclado:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R9
    MOV  R2, TEC_LIN							; endereço do periférico das linhas
    MOV  R3, TEC_COL							; endereço do periférico das colunas
    MOV  R4, DISPLAYS							; endereço do periférico dos displays
    MOV  R5, MASCARA							; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
	MOV  R6, 8									; constante para usar no CMP
	MOV  R7, 0						
	MOV  R8, 0									; onde � armazenada a tecla premida, entre 0 e FH
	MOV  R9, 0									; contador que vai ligar aos displays
ciclo:						
    MOV  R1, 0									; inicializa a linha
	MOV  R0, 0         							; inicializa a coluna
	MOV  R1, LINHA     							; testar a linha 1

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
	POP R9
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

converte_num_ciclo:				; acumula o n�mero de SHR feitos at� o valor da linha/coluna ser 0    
	SHR  R1, 1         
	CMP  R1, 0					; se o n�mero j� for 0
	JZ   converte_num_ret		; termina o ciclo
	ADD  R8, 1					; adiciona ao resultado final
	JMP  converte_num_ciclo		; repete ciclo
	
converte_num_ret:
	RET
	
; ******************************************************************************
; * Funções das teclas do teclado
; ******************************************************************************

verifica_tecla:					; verifica a tecla que foi premids para incrementar ou decrementar os displays
	MOV R1, INCREMENT
	CMP R8, R1					; verifica se a tecla � a tecla que incrementa
	JZ  incrementa_display
	MOV R1, DECREMENT
	CMP R8, R1					; verifica se a tecla � a tecla que decrementa
	JZ  decrementa_display
	MOV R1, MEXE_METEORO
	CMP R8, R1
	JZ mexer_meteoro
	MOV R1, MEXE_SONDA
	CMP R8, R1
	JZ mexer_sonda
	JMP ha_tecla				; aguarda que a tecla deixe de ser premida
	
ha_tecla:						; neste ciclo espera-se at� NENHUMA tecla estar premida
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	ciclo_ha_tecla:
		MOV  R1, R7				; testar a �ltima linha usada (R1 tinha sido alterado)
		MOV R2, TEC_LIN
		MOV R3, TEC_COL
		MOV R4, MASCARA
		MOVB [R2], R1			; escrever no perif�rico de sa�da (linhas)
		MOVB R0, [R3]			; ler do perif�rico de entrada (colunas)
		AND  R0, R4				; elimina bits para al�m dos bits 0-3
		CMP  R0, 0 				; h� tecla premida?
		JNZ  ciclo_ha_tecla 	; se ainda houver uma tecla premida, espera at� n�o haver
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	JMP ciclo_teclado

; ******************************************************************************
; * Rotinas
; ******************************************************************************

; ******************************************************************************
; * incrementa_display- incrementa o valor dos displays por 1
; ******************************************************************************

incrementa_display:
	PUSH R9
	MOV R9, [VALOR_DISPLAYS]
	ADD R9, 1					; incrementa o contador
	MOV [DISPLAYS], R9			; atualiza o display
	MOV [VALOR_DISPLAYS], R9
	POP R9
	JMP ha_tecla				; aguarda que a tecla deixe de ser premida
	
; ******************************************************************************
; * decrementa_display- decrementa o valor dos displays por 1
; ******************************************************************************

decrementa_display:
	PUSH R9
	MOV R9, [VALOR_DISPLAYS]
	SUB R9, 1					; decrementa o contador
	MOV [DISPLAYS], R9			; decrementa o contador
	MOV [VALOR_DISPLAYS], R9
	POP R9
	JMP ha_tecla				; aguarda que a tecla deixe de ser premida
	

; ******************************************************************************
; * mexer_meteoro- apaga o meteoro, incrementa por 1 a col e lin do pixel de referencia
;					e desenha-o outra vez
; ******************************************************************************

mexer_meteoro:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_METEORO]
	MOV R2, [REF_METEORO + 2]
	MOV R3, DEF_METEORO
	CALL apaga_objeto
	ADD R1, 1
	ADD R2, 1
	MOV [REF_METEORO], R1
	MOV [REF_METEORO + 2], R2
	CALL desenha_meteoro
	MOV R1, METEORO_MEXE
	MOV [TOCA_SOM], R1
	POP R3
	POP R2
	POP R1
	JMP ha_tecla
	
; ******************************************************************************
; * mexer_sonda- apaga a sonda, decrementa por 1 a lin do pixel de referencia
;					e desenha-a outra vez
; ******************************************************************************

mexer_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_SONDA]
	MOV R2, [REF_SONDA + 2]
	MOV R3, DEF_SONDA
	CALL apaga_objeto
	SUB R1, 1
	MOV [REF_SONDA], R1
	CALL desenha_sonda
	POP R3
	POP R2
	POP R1
	JMP ha_tecla


; **********************************************************************
; desenha_meteoro - chama a funcao desenha_objeto com os argumentos
;                   necessarios para que esta desenhe o meteoro
;
; **********************************************************************

desenha_meteoro:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_METEORO]
	MOV R2, [REF_METEORO + 2]
	MOV R3, DEF_METEORO
	CALL desenha_objeto
	POP R3
	POP R2
	POP R1
	RET
	
; **********************************************************************
; desenha_sonda - chama a funcao desenha_objeto com os argumentos
;                   necessarios para que esta desenhe a sonda
;
; **********************************************************************

desenha_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_SONDA]
	MOV R2, [REF_SONDA + 2]
	MOV R3, DEF_SONDA
	CALL desenha_objeto
	POP R3
	POP R2
	POP R1
	RET
	
; **********************************************************************
; desenha_painel - chama a funcao desenha_objeto com os argumentos
;                   necessarios para que esta desenhe o painel
;
; **********************************************************************

desenha_painel:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_PAINEL]
	MOV R2, [REF_PAINEL + 2]
	MOV R3, DEF_PAINEL
	CALL desenha_objeto
	POP R3
	POP R2
	POP R1
	RET


; **********************************************************************
; desenha_objeto - desenha um objeto respeitando os argumentos
; argumentos - 		R1 - linha
;					R2 - coluna
;					R3 - tabela
; **********************************************************************

desenha_objeto:									; desenha o objeto a partir da tabela
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    PUSH R6
    PUSH R7
	PUSH R8
	MOV R8, R2									; guarda a coluna inicial
	MOV	R5, [R3]								; obtém a altura do objeto
	ADD R3, 2									; endereço da tabela que define o comprimento do objeto
	MOV R6, [R3]								; obtém o largura do objeto
	MOV R7, R6									; decrementador da coluna
	ADD	R3, 2									; endereço da cor do 1º pixel (2 porque a largura é uma word)
	desenha_linha:       						; desenha os pixels do objeto a partir da tabela
		MOV	R4, [R3]							; obtém a cor do próximo pixel do objeto
		MOV  [DEFINE_LINHA], R1					; seleciona a linha
		MOV  [DEFINE_COLUNA], R2				; seleciona a coluna
		MOV  [DEFINE_PIXEL], R4					; altera a cor do pixel na linha e coluna selecionadas
		ADD	R3, 2								; endereço da cor do próximo pixel (2 porque cada cor de pixel é uma word)
		ADD  R2, 1              				; próxima coluna
		SUB R7, 1								; menos uma coluna para tratar
		JNZ desenha_linha						; continua até percorrer toda a largura do objeto
		MOV R2, R8								; volta á primeira linha
		ADD R1, 1
		MOV R7, R6
		SUB R5, 1
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
	
; **********************************************************************
; apaga_objeto - apaaga um objeto respeitando os argumentos
; argumentos - 		R1 - linha
;					R2 - coluna
;					R3 - tabela
; **********************************************************************	
	
apaga_objeto:									; apaga o objeto a partir da tabela
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    PUSH R6
    PUSH R7
	PUSH R8
	MOV R8, R2									; guarda a coluna inicial
	MOV	R5, [R3]								; obtém a altura do objeto
	ADD R3, 2									; endereço da tabela que define o comprimento do objeto
	MOV R6, [R3]								; obtém o largura do objeto
	MOV R7, R6									; decrementador da coluna
	MOV R3, 0									; valor do pixel transparente ( irá substituir todos os outros)
	apaga_linha:       							; desenha os pixels do objeto a partir da tabela
		MOV [DEFINE_LINHA], R1					; seleciona a linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R3					; altera a cor do pixel na linha e coluna selecionadas
		ADD R2, 1              					; próxima coluna
		SUB R7, 1								; menos uma coluna para tratar
		JNZ apaga_linha							; continua até percorrer toda a largura do objeto
		MOV R2, R8								; volta á primeira linha
		ADD R1, 1
		MOV R7, R6
		SUB R5, 1
		JNZ apaga_linha
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	RET