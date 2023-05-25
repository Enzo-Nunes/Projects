; ******************************************************************************
; * IST-UL
; * Alunos: Enzo Nunes 		ist1106336
;			João Ribeiro 	ist1107251
;			David Antunes	ist1107061
; * PROJETO
; * Descrição: Parte intermédia do projeto "Beyond Mars".
; ******************************************************************************


; ******************************************************************************
; * CONSTANTES
; ******************************************************************************

DISPLAYS					EQU 0A000H			; endereço dos displays de 7 segmentos (periférico POUT-1)

; Teclado
TEC_LIN						EQU 0C000H			; endereço das linhas do teclado (periférico POUT-2)
TEC_COL						EQU 0E000H			; endereço das colunas do teclado (periférico PIN)
LINHA						EQU 1				; primeira linha
MASCARA						EQU 0FH				; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
INCREMENT					EQU 0AH				; tecla que incrementa o valor dos displays
DECREMENT					EQU 0BH				; tecla que decrementa o valor dos displays
MEXE_ASTEROIDE				EQU 0EH				; tecla que mexe o asteroide
MEXE_SONDA					EQU 0FH

; MediaCenter

APAGA_ECRA	 		     	EQU 6002H			; endereço do comando para apagar todos os pixels já desenhados
DEFINE_LINHA    		 	EQU 600AH			; endereço do comando para definir a linha
DEFINE_COLUNA   	 	 	EQU 600CH			; endereço do comando para definir a coluna
DEFINE_PIXEL    	 	 	EQU 6012H			; endereço do comando para escrever um pixel
APAGA_AVISO     		 	EQU 6040H			; endereço do comando para apagar o aviso de nenhum cenário selecionado
SELECIONA_CENARIO_FUNDO		EQU 6042H			; endereço do comando para selecionar uma imagem de fundo
	CENARIO_JOGO			EQU 0				; cenário de jogo
TOCA_SOM					EQU 605AH			; endereço do comando para tocar um som
	SFX_AK_47				EQU 0				; audio de disparo

; Dimensões Objetos

ALTURA_ASTEROIDE			EQU 5				;altura do objeto asteroide
LARGURA_ASTEROIDE			EQU 5				;largura do objeto asteroide
LIN_ASTEROIDE_INIC			EQU 0				;linha referencia do asteroide
COL_ASTEROIDE_INIC			EQU 0				;coluna referencia do asteroide

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
; * STACKS
; ******************************************************************************

PLACE 1000H

; Reserva o espaço para as pilhas dos processos
	STACK 100H									; espaço reservado para a pilha do processo "programa principal"
SP_inicial:										; este é o endereço com que o SP deste processo deve ser inicializado


; ******************************************************************************
; * DADOS
; ******************************************************************************

VALOR_DISPLAYS: WORD 0							; "variável global" do valor dos displays

; ******************************************************************************
; * Pixeis de referência dos objetos, i.e. onde começam a ser desenhados
; ******************************************************************************
REF_ASTEROIDE:									; pixel de referencia do asteroide
	WORD 	LIN_ASTEROIDE_INIC, COL_ASTEROIDE_INIC

REF_SONDA:										; pixel de referencia da sonda
	WORD 	LIN_SONDA_INIC, COL_SONDA_INIC
	
REF_PAINEL:										; pixel de referencia do painel
	WORD 	LIN_PAINEL_INIC, COL_PAINEL_INIC

; ******************************************************************************
; * Tabelas de desenhos dos objetos
; ******************************************************************************
DEF_ASTEROIDE:									; tabela do asteroide
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide
	WORD	VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	WORD	0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD	VERMELHO , VERMELHO , 0		   , VERMELHO , VERMELHO
	WORD	0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD	VERMELHO , 0		, VERMELHO , 0		  , VERMELHO

DEF_SONDA:										; tabela da sonda. (1 pixel apenas)
	WORD 	ALTURA_SONDA, LARGURA_SONDA			; dimensões da sonda
	WORD 	LARANJA

DEF_PAINEL:										; tabela do painel de instrumentos
	WORD	ALTURA_PAINEL, LARGURA_PAINEL		; dimensões do painel
	WORD	0		 , 0		, VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , 0        , 0
	WORD	0		 , VERMELHO , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , VERMELHO , 0
	WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO	  , LARANJA  , VERDE    , VERMELHO , LARANJA  , VERDE    , VERMELHO , LARANJA  , BRANCO   , BRANCO   , BRANCO   , VERMELHO
    WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO   , VERMELHO , LARANJA  , VERDE    , VERMELHO , LARANJA  , VERDE    , VERMELHO , BRANCO   , BRANCO   , BRANCO   , VERMELHO
	WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , VERMELHO


; ******************************************************************************
; * CÓDIGO PRINCIPAL
; ******************************************************************************

PLACE	   0
inicializacoes:
    MOV SP, SP_inicial							; inicialização do Stack Pointer
    MOV [APAGA_AVISO], R1						; apaga o aviso de nenhum cenário selecionado (R1 não é relevante)
    MOV [APAGA_ECRA], R1						; apaga todos os pixels já desenhados (R1 não é relevante)
	MOV R1, CENARIO_JOGO
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário de fundo de jogo
	MOV R1, 0
	MOV [DISPLAYS], R1							; escreve linha e coluna a zero nos displays

CALL desenha_asteroide							; desenha o asteroide
CALL desenha_sonda								; desenha a sonda
CALL desenha_painel								; desenha o painel da nave

ciclo_teclado:									; ciclo que vai repetidamente esperar uma ação no teclado
	CALL inicio_teclado
	JMP verifica_tecla

fim:
	JMP fim

; ******************************************************************************
; * FUNÇÕES DO TECLADO
; ******************************************************************************

; ******************************************************************************
; * inicio_teclado - 	Função que lê do perférico e codifica em hexadecimal a
; *						tecla premida pelo utilizador.
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
	MOV  R7, 0									; onde é armazenada a linha da tecla premida
	MOV  R8, 0									; onde é armazenada a tecla premida, entre 0 e FH
	MOV  R9, 0									; contador que vai ligar aos displays
	ciclo:
		MOV  R1, 0								; inicializa a linha
		MOV  R0, 0								; inicializa a coluna
		MOV  R1, LINHA							; testar a linha 1
		espera_tecla:							; neste ciclo espera-se até uma tecla ser premida
			MOVB [R2], R1						; escrever no periférico de saída (linhas)
			MOVB R0, [R3]						; ler do periférico de entrada (colunas)
			AND  R0, R5							; elimina bits para além dos bits 0-3
			CMP  R0, 0							; há tecla premida?
			JZ   muda_linha						; se nenhuma tecla premida na linha, muda de linha
			MOV  R7, R1							; guarda a linha no R7 porque o R1 vai ser alterado 
			JMP  converte_num					; caso contrário converte a tecla para um n�mero hexadecimal
		muda_linha:
			CMP  R1, R6							; testa se a linha atual é a linha 4
			JZ   ciclo							; repete o ciclo
			SHL  R1, 1							; muda de linha
			JMP  espera_tecla					; testar a proxima linha
converte_num:									; converte a tecla premida para um número hexadecimal entre 0 e FH
	MOV  R8, 0									; resultado final
	CALL converte_num_ciclo						; converte a linha        
	SHL  R8, 2									; multiplica o número correspondente à linha por quatro
	MOV  R1, R0         
	CALL converte_num_ciclo						; converte a coluna
	POP R9
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET
converte_num_ciclo:								; acumula o número de SHR feitos até o valor da linha/coluna ser 0    
	SHR  R1, 1									
	CMP  R1, 0									; se o número já for 0
	JZ   converte_num_ret						; termina o ciclo
	ADD  R8, 1									; adiciona ao resultado final
	JMP  converte_num_ciclo						; repete ciclo
converte_num_ret:
	RET

; ******************************************************************************
; * verifica_tecla - 	Função que determina qual foi a tecla premida pelo
; * 					utilizador através do seu valor hexadecimal.
; ******************************************************************************

verifica_tecla:									; determina qual deve ser a ação consoante a tecla premida
	MOV	R1, INCREMENT
	CMP	R8, R1									; verifica se a tecla premida é a tecla que incrementa os displays
	JZ	incrementa_display
	MOV	R1, DECREMENT
	CMP	R8, R1									; verifica se a tecla premida é a tecla que decrementa os displays
	JZ	decrementa_display
	MOV	R1, MEXE_ASTEROIDE
	CMP	R8, R1									; verifica se a tecla premida é a tecla que move o asteroide
	JZ	mexer_asteroide
	MOV	R1, MEXE_SONDA
	CMP	R8, R1									; verifica se a tecla premida é a tecla que move a sonda
	JZ	mexer_sonda
	JMP	ha_tecla								; aguarda que a tecla deixe de ser premida

; ******************************************************************************
; * ha_tecla - 			Função que espera que o utilizador deixe de premir a
; * 					tecla.
; ******************************************************************************

ha_tecla:										; neste ciclo espera-se até NENHUMA tecla estar premida
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	ciclo_ha_tecla:
		MOV  R1, R7								; testar a última linha usada (R1 tinha sido alterado)
		MOV R2, TEC_LIN
		MOV R3, TEC_COL
		MOV R4, MASCARA
		MOVB [R2], R1							; escrever no periférico de saída (linhas)
		MOVB R0, [R3]							; ler do periférico de entrada (colunas)
		AND  R0, R4								; elimina bits para além dos bits 0-3
		CMP  R0, 0 								; há tecla premida?
		JNZ  ciclo_ha_tecla 					; se ainda houver uma tecla premida, espera até não haver
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	JMP ciclo_teclado							; volta ao ciclo inicial do programa

; ******************************************************************************
; * ROTINAS - funções gerais que são chamadas por outras funções.
; ******************************************************************************

; ******************************************************************************
; * incrementa_display- incrementa o valor dos displays por 1
; ******************************************************************************

incrementa_display:
	PUSH R9
	MOV R9, [VALOR_DISPLAYS]					; valor atual dos displays
	ADD R9, 1									; incrementa o valor em 1
	MOV [DISPLAYS], R9							; atualiza os displays
	MOV [VALOR_DISPLAYS], R9					; atualiza a variável do valor dos displays
	POP R9
	JMP ha_tecla								; aguarda que a tecla deixe de ser premida
	
; ******************************************************************************
; * decrementa_display - decrementa o valor dos displays por 1
; ******************************************************************************

decrementa_display:
	PUSH R9
	MOV R9, [VALOR_DISPLAYS]					; valor atual dos displays
	SUB R9, 1									; decrementa o valor em 1
	MOV [DISPLAYS], R9							; atualiza os displays
	MOV [VALOR_DISPLAYS], R9					; atualiza a variável do valor dos displays
	POP R9
	JMP ha_tecla								; aguarda que a tecla deixe de ser premida

; ******************************************************************************
; * mexer_asteroide - 	avança o asteroide para a próxima posição
; ******************************************************************************

mexer_asteroide:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_ASTEROIDE]						; linha do pixel de referência do asteroide
	MOV R2, [REF_ASTEROIDE + 2]					; coluna do pixel de referência do asteroide
	MOV R3, DEF_ASTEROIDE						; tabela do desenho do asteroide
	CALL apaga_objeto							; apaga o asteroide
	ADD R1, 1									; incrementa em 1 a linha
	ADD R2, 1									; incrementa em 1 a coluna
	MOV [REF_ASTEROIDE], R1						; atualiza a linha do pixel de referência
	MOV [REF_ASTEROIDE + 2], R2					; atualiza a coluna do pixel de referência
	CALL desenha_asteroide						; desenha novamente o asteroide
	MOV R1, SFX_AK_47							; seleciona o efeito sonoro
	MOV [TOCA_SOM], R1							; reproduz o efeito sonoro
	POP R3
	POP R2
	POP R1
	JMP ha_tecla								; aguarda que a tecla deixe de ser premida
	
; ******************************************************************************
; * mexer_sonda - 		apaga a sonda, decrementa por 1 a lin do pixel de
; *						referencia e desenha-a outra vez
; ******************************************************************************

mexer_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_SONDA]							; linha do pixel de referência da sonda
	MOV R2, [REF_SONDA + 2]						; coluna do pixel de referência da sonda
	MOV R3, DEF_SONDA							; tabela do desenho da sonda
	CALL apaga_objeto							; apaga a sonda
	SUB R1, 1									; decrementa em 1 a linha
	MOV [REF_SONDA], R1							; atualiza a linha do pixel de referência
	CALL desenha_sonda							; desenha novamente a sonda
	POP R3
	POP R2
	POP R1
	JMP ha_tecla								; aguarda que a tecla deixe de ser premida


; ******************************************************************************
; * desenha_asteroide - chama a funcao desenha_objeto com os argumentos
; *                 	para desenhar o asteroide.
; ******************************************************************************

desenha_asteroide:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [REF_ASTEROIDE]
	MOV R2, [REF_ASTEROIDE + 2]
	MOV R3, DEF_ASTEROIDE
	CALL desenha_objeto
	POP R3
	POP R2
	POP R1
	RET
	
; ******************************************************************************
; * desenha_sonda - 	chama a funcao desenha_objeto com os argumentos
; *						necessarios para desenhar a sonda.
; ******************************************************************************

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
	
; ******************************************************************************
; * desenha_painel - 	chama a funcao desenha_objeto com os argumentos
; *                 	necessarios para desenhar o painel da nave.
; ******************************************************************************

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


; ******************************************************************************
; * desenha_objeto - 	desenha um objeto respeitando os argumentos:
; *						R1 - linha do pixel de referência
; *						R2 - coluna do pixel de referência
; *						R3 - tabela do objeto
; ******************************************************************************

desenha_objeto:									; desenha o objeto a partir da tabela
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    PUSH R6
    PUSH R7
	PUSH R8
	MOV  R8, R2									; guarda a coluna inicial
	MOV	 R5, [R3]								; obtém a altura do objeto
	ADD  R3, 2									; endereço da tabela que define o comprimento do objeto
	MOV  R6, [R3]								; obtém o largura do objeto
	MOV  R7, R6									; decrementador da coluna
	ADD	 R3, 2									; endereço da cor do 1º pixel (2 porque a largura é uma word)
	desenha_linha:								; desenha os pixels do objeto a partir da tabela
		MOV	R4, [R3]							; obtém a cor do próximo pixel do objeto
		MOV [DEFINE_LINHA], R1					; seleciona a linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R4					; altera a cor do pixel na linha e coluna selecionadas
		ADD	R3, 2								; endereço da cor do próximo pixel (2 porque cada cor de pixel é uma word)
		ADD  R2, 1              				; próxima coluna
		SUB R7, 1								; menos uma coluna para tratar
		JNZ desenha_linha						; continua até percorrer toda a largura do objeto
		MOV R2, R8								; volta á primeira linha
		ADD R1, 1								; próxima linha
		MOV R7, R6								; reinicia o decrementador da coluna
		SUB R5, 1								; menos uma linha para tratar
		JNZ desenha_linha						; continua até percorrer toda a altura do objeto
	POP  R8
	POP  R7
	POP  R6
	POP  R5
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	RET
	
; ******************************************************************************
; *	apaga_objeto - 		apaga um objeto respeitando os argumentos:
; *						R1 - linha do pixel de referência
; *						R2 - coluna do pixel de referência
; *						R3 - tabela do objeto
; ******************************************************************************
	
apaga_objeto:									; apaga o objeto a partir da tabela
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    PUSH R6
    PUSH R7
	PUSH R8
	MOV  R8, R2									; guarda a coluna inicial
	MOV	 R5, [R3]								; obtém a altura do objeto
	ADD  R3, 2									; endereço da tabela que define o comprimento do objeto
	MOV  R6, [R3]								; obtém o largura do objeto
	MOV  R7, R6									; decrementador da coluna
	MOV  R3, 0									; valor do pixel transparente ( irá substituir todos os outros)
	apaga_linha:       							; desenha os pixels do objeto a partir da tabela
		MOV [DEFINE_LINHA], R1					; seleciona a linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R3					; altera a cor do pixel na linha e coluna selecionadas
		ADD R2, 1              					; próxima coluna
		SUB R7, 1								; menos uma coluna para tratar
		JNZ apaga_linha							; continua até percorrer toda a largura do objeto
		MOV R2, R8								; volta á primeira coluna
		ADD R1, 1								; próxima linha
		MOV R7, R6								; reinicia o decrementador da coluna
		SUB R5, 1								; menos uma linha para tratar
		JNZ apaga_linha							; continua até percorrer toda a altura do objeto
	POP  R8
	POP  R7
	POP  R6
	POP  R5
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	RET
