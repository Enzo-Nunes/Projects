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

;Perifericos

DISPLAYS					EQU 0A000H			; endereço dos displays de 7 segmentos (periférico POUT-1)

; Teclado
TEC_LIN						EQU 0C000H			; endereço das linhas do teclado (periférico POUT-2)
TEC_COL						EQU 0E000H			; endereço das colunas do teclado (periférico PIN)
LINHA						EQU 1				; primeira linha
MASCARA						EQU 0FH				; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
INCREMENT					EQU 0AH				; tecla que incrementa o valor dos displays
DECREMENT					EQU 0BH				; tecla que decrementa o valor dos displays
MEXE_ASTEROIDE				EQU 0EH				; tecla que mexe o asteroide
MEXE_SONDA					EQU 0FH				; tecla que mexe a sonda


; MediaCenter
OBTEM_COR_PIXEL				EQU 6010H			; endereço do comando para obter a cor do pixel selecionado
APAGA_ECRA	 				EQU 6002H			; endereço do comando para apagar todos os pixels já desenhados
DEFINE_LINHA				EQU 600AH			; endereço do comando para definir a linha
DEFINE_COLUNA				EQU 600CH			; endereço do comando para definir a coluna
DEFINE_PIXEL				EQU 6012H			; endereço do comando para escrever um pixel
APAGA_AVISO					EQU 6040H			; endereço do comando para apagar o aviso de nenhum cenário selecionado
SELECIONA_CENARIO_FUNDO		EQU 6042H			; endereço do comando para selecionar uma imagem de fundo
	CENARIO_JOGO			EQU 0				; cenário de jogo
TOCA_SOM					EQU 605AH			; endereço do comando para tocar um som
	SFX_AK_47				EQU 0				; audio de disparo
	
; Mascaras
MASCARA_0AH					EQU 0AH
MASCARA_0A0H				EQU 0A0H
MASCARA_0FH					EQU 0FH
MASCARA_0F0H				EQU 0F0H

; Dimensões, coordenadas e movimento dos Objetos
ALTURA_ASTEROIDE			EQU 5				; altura do objeto asteroide
LARGURA_ASTEROIDE			EQU 5				; largura do objeto asteroide
LIN_ASTEROIDE_INICIAL		EQU 0				; linha referencia inicial dos asteroides
COL_ASTEROIDE_ESQ			EQU 0				; coluna referencia do asteroide que aparece á esquerda
COL_ASTEROIDE_MEIO			EQU 30				; coluna referencia do asteroide que aparece no meio
COL_ASTEROIDE_DIR			EQU 59				; coluna referencia do asteroide que aparece á direita

MEXER_DIREITA				EQU 1
MEXER_ESQUERDA				EQU -1
MEXER_VERTICAL				EQU 0

ALTURA_SONDA				EQU 1				; altura do objeto sonda
LARGURA_SONDA				EQU 1				; largura do objeto sonda
LIN_SONDA_INIC				EQU 26				; linha referencia da sonda
COL_SONDA_INIC				EQU 32				; coluna referencia da sonda	

ALTURA_PAINEL				EQU 5				; altura do objeto painel
LARGURA_PAINEL				EQU 15				; largura do objeto painel
LIN_PAINEL_INIC				EQU 27				; linha referencia do painel
COL_PAINEL_INIC				EQU 25				; linha referencia do painel
LIN_ECRA_PAINEL				EQU 29				; linha referencia do ecra do painel
COL_ECRA_PAINEL				EQU 29				; linha referencia do ecra do painel
COL_ECRA_FINAL				EQU 35

; Cores
AMARELO						EQU 0FFE0H			; cor do pixel: amarelo
VERMELHO					EQU 0FF00H			; cor do pixel: vermelho
PRETO						EQU 0F000H			; cor do pixel: preto
BRANCO						EQU 0FFFFH			; cor do pixel: branco
CINZENTO					EQU 0FCCCH			; cor do pixel: cinzento
VERDE						EQU 0F5F0H			; cor do pixel: verde
LARANJA						EQU 0FF50H			; cor do pixel: laranja
AZUL						EQU 0F00FH			; cor do pixel: azul
CIANO 						EQU 0FFFFH			; cor do pixel: ciano

; Constantes do jogo
ENERGIA_INICIAL 			EQU 100H			; energia inicial da nave

; ******************************************************************************
; * STACKS
; ******************************************************************************

PLACE 1000H

; Reserva do espaço para as pilhas dos processos
	STACK 100H			; espaço reservado para a pilha do processo "programa principal"
SP_inicial_prog_princ:		; este é o endereço com que o SP deste processo deve ser inicializado
							
	STACK 100H			; espaço reservado para a pilha do processo "teclado"
SP_inicial_teclado:			; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "energia"
SP_inicial_energia:
	
	STACK 100H			; espaço reservado para a pilha do processo "painel"
SP_inicial_painel:


; SP inicial de cada processo "asteroide"
	STACK 100H			; espaço reservado para a pilha do processo "asteroide", instância 0
SP_inicial_asteroide_0:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "asteroide", instância 1
SP_inicial_asteroide_1:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "asteroide", instância 2
SP_inicial_asteroide_2:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "asteroide", instância 3
SP_inicial_asteroide_3:		; este é o endereço com que o SP deste processo deve ser inicializado

tab_stack_asteroides:
	WORD SP_inicial_asteroide_0
	WORD SP_inicial_asteroide_1
	WORD SP_inicial_asteroide_2
	WORD SP_inicial_asteroide_3
	

TECLA_CARREGADA: LOCK 0				; LOCK para o teclado comunicar aos restantes processos que tecla detetou
DECREMENTA_ENERGIA: LOCK 0			; LOCK para a rotina de interrupcao energia comunicar com a rotina de decrementa_passivo
ATUALIZA_PAINEL: LOCK 0			; LOCK para a rotina de interrupcao painel comunicar com a rotina de altera_painel


; ******************************************************************************
; * INTERRUPCOES
; ******************************************************************************

tab_interrupcoes:
	WORD 0
	WORD 0
	WORD rot_energia
	WORD rot_painel

; ******************************************************************************
; * DADOS
; ******************************************************************************

VALOR_DISPLAYS: WORD ENERGIA_INICIAL			; "variável global" do valor dos displays

; ******************************************************************************
; * Pixeis de referência dos objetos, i.e. onde começam a ser desenhados
; ******************************************************************************
POSSIBILIDADES_ASTEROIDE:									; tabela das possiveis caracteristicas de spawn dos asteroides
	WORD 	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_ESQ, MOVER_DIREITA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_ESQUERDA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_MEIO
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_DIREITA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_DIR, MOVER_ESQUERDA
	
REFERENCIAS_ASTEROIDES:										; tabela para guardar os pixeis de referencia dos asteroides
	WORD 0,0												;linha, coluna
	WORD 0,0
	WORD 0,0
	WORD 0,0
	

REF_SONDA:										; pixel de referencia da sonda
	WORD 	LIN_SONDA_INIC, COL_SONDA_INIC
	
REF_PAINEL:										; pixel de referencia do painel
	WORD 	LIN_PAINEL_INIC, COL_PAINEL_INIC
REF_ECRA:										; pixel de referencia do painel
	WORD 	COL_ECRA_PAINEL

; ******************************************************************************
; * Tabelas de desenhos dos objetos
; ******************************************************************************
DEF_ASTEROIDE_MAU:								; tabela do asteroide mau
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide
	WORD	VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	WORD	0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD	VERMELHO , VERMELHO , 0		   , VERMELHO , VERMELHO
	WORD	0		 , VERMELHO , VERMELHO , VERMELHO , 0
	WORD	VERMELHO , 0		, VERMELHO , 0		  , VERMELHO
	
DEF_ASTEROIDE_BOM:								; tabela do asteroide bom
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide
	WORD	0     , VERDE , VERDE , VERDE , 0
	WORD	VERDE , VERDE , VERDE , VERDE , VERDE
	WORD	VERDE , VERDE , VERDE , VERDE , VERDE
	WORD	VERDE , VERDE , VERDE , VERDE , VERDE
	WORD	0     , VERDE , VERDE , VERDE , 0

DEF_ESPLOSAO_ASTEROIDE_MAU:						; tabela da explosão do asteroide mau
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide
	WORD	0     , CIANO , 0     , CIANO , 0
	WORD	CIANO , 0     , CIANO , 0     , CIANO
	WORD	0     , CIANO , 0     , CIANO , 0
	WORD	CIANO , 0     , CIANO , 0     , CIANO
	WORD	0     , CIANO , 0     , CIANO , 0

DEF_EXPLOSAO_ASTEROIDE_BOM:						; tabela da explosão do asteroide bom
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide 
	WORD	0 , 0     , 0     , 0     , 0		; dimensões iguais as outras para facilitar a manipulação de pixeis
	WORD	0 , 0     , VERDE , 0     , 0
	WORD	0 , VERDE , VERDE , VERDE , 0
	WORD	0 , 0     , VERDE , 0     , 0
	WORD	0 , 0     , 0     , 0     , 0

DEF_SONDA:										; tabela da sonda. (1 pixel apenas)
	WORD 	ALTURA_SONDA, LARGURA_SONDA			; dimensões da sonda
	WORD 	AZUL

DEF_PAINEL:										; tabela do painel de instrumentos
	WORD	ALTURA_PAINEL, LARGURA_PAINEL		; dimensões do painel
	WORD	0		 , 0		, VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , VERMELHO , 0        , 0
	WORD	0		 , VERMELHO , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , VERMELHO , 0
	WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO	  , AZUL     , VERDE    , VERMELHO , AZUL     , VERDE    , VERMELHO , AZUL     , BRANCO   , BRANCO   , BRANCO   , VERMELHO
    WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO   , VERMELHO , AZUL     , VERDE    , VERMELHO , AZUL     , VERDE    , VERMELHO , BRANCO   , BRANCO   , BRANCO   , VERMELHO
	WORD	VERMELHO , BRANCO	, BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , BRANCO   , VERMELHO


; ******************************************************************************
; * CÓDIGO PRINCIPAL
; ******************************************************************************

PLACE	   0
inicializacoes:
    MOV SP, SP_inicial_prog_princ				; inicialização do Stack Pointer
    MOV [APAGA_AVISO], R1						; apaga o aviso de nenhum cenário selecionado (R1 não é relevante)
    MOV [APAGA_ECRA], R1						; apaga todos os pixels já desenhados (R1 não é relevante)
	MOV R1, CENARIO_JOGO
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário de fundo de jogo
	MOV R1, ENERGIA_INICIAL
	MOV [DISPLAYS], R1							; escreve linha e coluna a zero nos displays
	MOV BTE, tab_interrupcoes
    EI2
	EI3
	EI      ; permite interrupções (geral)
	
CALL inicio_teclado								; inicia o processo do teclado
CALL desenha_asteroide							; desenha o asteroide
CALL desenha_sonda								; desenha a sonda
CALL desenha_painel								; desenha o painel da nave
CALL energia									; inicio o processo de energia
CALL painel

fim:
	YIELD
	JMP fim

; ******************************************************************************
; * PROCESSOS
; ******************************************************************************

; ******************************************************************************
; * Processo: teclado - Processo responsável por ler o teclado e devolver a 
; * 					tecla no LOCK TECLA_CARREGADA.
; ******************************************************************************

PROCESS SP_inicial_teclado	; indicação de que a rotina que se segue é um processo,
							; com indicação do valor para inicializar o SP
inicio_teclado:
    MOV  R2, TEC_LIN							; endereço do periférico das linhas
    MOV  R3, TEC_COL							; endereço do periférico das colunas
    MOV  R4, DISPLAYS							; endereço do periférico dos displays
    MOV  R5, MASCARA							; para isolar os 4 bits de menor peso, ao ler as colunas do teclado
	MOV  R6, 8									; constante para usar no CMP
	MOV  R7, 0									; onde é armazenada a linha da tecla premida
	MOV  R8, 0									; onde é armazenada a tecla premida, entre 0 e FH
	ciclo:
		YIELD
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
			CALL  converte_num					; caso contrário converte a tecla para um n�mero hexadecimal
			MOV	[TECLA_CARREGADA], R8			; informa quem estiver bloqueado neste LOCK que uma tecla foi carregada
			JMP ha_tecla						; verifica se a tecla ainda está a ser premida
		muda_linha:
			CMP  R1, R6							; testa se a linha atual é a linha 4
			JZ   ciclo							; repete o ciclo
			SHL  R1, 1							; muda de linha
			JMP  espera_tecla					; testar a proxima linha
			
	ha_tecla:									; neste ciclo espera-se até NENHUMA tecla estar premida
		ciclo_ha_tecla:
			YIELD
			MOV  R1, R7							; testar a última linha usada (R1 tinha sido alterado)
			MOVB [R2], R1						; escrever no periférico de saída (linhas)
			MOVB R0, [R3]						; ler do periférico de entrada (colunas)
			AND  R0, R5							; elimina bits para além dos bits 0-3
			CMP  R0, 0							; há tecla premida?
			JNZ  ciclo_ha_tecla					; se ainda houver uma tecla premida, espera até não haver
		JMP ciclo
			
; ******************************************************************************
; * converte_num : converte a linha e coluna do teclado no valor hexadecimal  																	!!! COLOCAR NAS ROTINAS
; *					da tecla primida
; *					R0 - coluna
; *					R1 - linha
; *					R8 - Tecla no seu valor hexadecimal
; ******************************************************************************			
			
converte_num:									; converte a tecla premida para um número hexadecimal entre 0 e FH
	PUSH R0
	PUSH R1
	MOV  R8, 0									; resultado final
	CALL converte_num_ciclo						; converte a linha        
	SHL  R8, 2									; multiplica o número correspondente à linha por quatro
	MOV  R1, R0									; argumento usado no converte_num_ciclo ( agora a coluna)
	CALL converte_num_ciclo						; converte a coluna
	JMP converte_num_ret
		converte_num_ciclo:						; acumula o número de SHR feitos até o valor da linha/coluna ser 0    
			SHR  R1, 1									
			CMP  R1, 0							; se o número já for 0
			JZ   converte_num_ciclo_ret			; termina o ciclo
			ADD  R8, 1							; adiciona ao resultado final
			JMP  converte_num_ciclo				; repete ciclo
		converte_num_ciclo_ret:
			RET
converte_num_ret:
	POP R1
	POP R0
	RET
	

; ******************************************************************************
; * Processo: energia - Processo responsável por decrementar a energia da nave 
; * 					passivamente, através da interrupção rot_energia
; ******************************************************************************
PROCESS SP_inicial_energia
	energia:
		MOV R0, [DECREMENTA_ENERGIA]
		CALL decrementa_display
		JMP energia

; ******************************************************************************
; * Processo: painel - Processo responsável por alterar o painel da nave 
; * 					passivamente, através da interrupção rot_painel
; ******************************************************************************
PROCESS SP_inicial_painel
	painel:
		MOV R0, [ATUALIZA_PAINEL]
		CALL altera_painel
		JMP painel
		
; ******************************************************************************
; * Processo: asteroide - Processo responsável por decidir a posicao, mover e 
; * 					verificar colisões do asteroide
; *						Argumentos: R11 - instancia do meteoro
; ******************************************************************************		
PROCESS SP_inicial_asteroide_0
	asteroide:
		MOV  R10, R11       				; cópia do nº de instância do processo
	    SHL  R10, 1     					; multiplica por 2 porque as tabelas são de WORDS
	    MOV  R9, tab_stack_asteroides       ; tabela com os SPs iniciais das várias instâncias deste processo
	    MOV	 SP, [R9+R10]        			; re-inicializa o SP deste processo, de acordo com o nº de instância
	cria_asteroide:
		 MOV 


; ******************************************************************************
; * ROTINAS - funções gerais que são chamadas por outras funções.
; ******************************************************************************

; ******************************************************************************
; * incrementa_display - incrementa o valor dos displays por 1
; ******************************************************************************
incrementa_display:
	PUSH R2
	PUSH R3
	PUSH R4
	MOV R2, [VALOR_DISPLAYS]					; valor atual dos displays
	ADD R2, 1									; incrementa o valor em 1
	MOV R3, MASCARA_0AH							; mascara para verificar se o ultimo digito é A
	MOV R4, R2									; copia do valor dos displays usado para a transformação em decimal
	AND R4, R3									; caso o ultimo digito é A, isola esse digito
	CMP R4, R3									; verificar se o ultimo digito é A
	JNZ fim_incrementa							; caso não seja, atualiza o display
	SUB R2, R3									; coloca o ultimo digito a 0
	MOV R3, 10H
	ADD R2, R3									; soma 1 ao segundo digito ( carry in da soma anterior)
	MOV R4, R2									; guardar o novo valor do display ( R4 vai ser destruido a seguir)
	MOV R3, MASCARA_0A0H								; mascara para verificar se o penultimo digito é A
	AND R4, R3									; caso o penultimo digito é A, isola esse digito
	CMP R4, R3									; verificar se o penultimo digito é A
	JNZ fim_incrementa							; caso não seja, atualiza o display
	SUB R2, R3									; coloca o penultimo digito a 0
	MOV R3, 100H
	ADD R2, R3									; soma 1 ao terceiro digito ( carry in da soma anterior)
	fim_incrementa:
		MOV [VALOR_DISPLAYS], R2				; atualiza a variável do valor dos displays
		MOV [DISPLAYS], R2
	POP R4
	POP R3
	POP R2
	RET
	
; ******************************************************************************
; * decrementa_display - decrementa o valor dos displays por 1
; ******************************************************************************

decrementa_display:
	PUSH R2
	PUSH R3
	PUSH R4
	MOV  R2, [VALOR_DISPLAYS]					; valor atual dos displays
	SUB  R2, 1									; decrementa o valor em 1
	MOV R3, MASCARA_0FH							; mascara para verificar se o ultimo digito e um F
	MOV R4, R2									; copia do valor dos displays usado para a transformação em decimal
	AND R4, R3									; caso o ultimo digito F, isola esse digito
	CMP R4, R3									; verificar se o ultimo digito é F
	JNZ fim_decrementa							; caso não seja, atualiza o display
	SUB R2, R3									; coloca o ultimo digito a 0
	MOV R3, 9H
	ADD R2, R3									; soma 9 ao ultimo digito
	MOV R4, R2									; guardar o novo valor do display ( R4 vai ser destruido a seguir)
	MOV R3, MASCARA_0F0H						; mascara para verificar se o penultimo digito é A
	AND R4, R3									; caso o penultimo digito é F, isola esse digito
	CMP R4, R3									; verificar se o penultimo digito é F
	JNZ fim_decrementa							; caso não seja, atualiza o display
	SUB R2, R3									; coloca o penultimo digito a 0
	MOV R3, 90H
	ADD R2, R3									; soma 9 ao penultimo digito
	fim_decrementa:
		MOV  [VALOR_DISPLAYS], R2					; atualiza a variável do valor dos displays
		MOV [DISPLAYS], R2
	POP R4
	POP R3
	POP R2
	RET

; ******************************************************************************
; * altera_painel -	altera os pixels do painel da nave
; ******************************************************************************
altera_painel:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	MOV  R0, COL_ECRA_FINAL
	MOV  R1, LIN_ECRA_PAINEL
	MOV  R5, R1
	ADD R5, 1
	MOV  R2, [REF_ECRA]
	MOV  R3, VERDE
	MOV  R4, PRETO
	MOV [DEFINE_LINHA], R1					; seleciona a linha
	MOV [DEFINE_COLUNA], R2					; seleciona a coluna
	MOV [DEFINE_PIXEL], R4
	MOV [DEFINE_LINHA], R5
	MOV [DEFINE_PIXEL], R4
	CMP R2, R0
	JZ  ultima_col_painel
	ADD R2, 1
	MOV [REF_ECRA], R2
	MOV [DEFINE_LINHA], R1					; seleciona a linha
	MOV [DEFINE_COLUNA], R2					; seleciona a coluna
	MOV [DEFINE_PIXEL], R3
	MOV [DEFINE_LINHA], R5
	MOV [DEFINE_PIXEL], R3
	JMP altera_painel_ret
	
	ultima_col_painel:
		MOV R2, COL_ECRA_PAINEL
		MOV [REF_ECRA], R2
		MOV [DEFINE_LINHA], R1					; seleciona a linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R3
		MOV [DEFINE_LINHA], R5
		MOV [DEFINE_PIXEL], R3
		JMP altera_painel_ret
	
	altera_painel_ret:
		POP R5
		POP R4
		POP R3
		POP R2
		POP R1
		POP R0
		RET

; ******************************************************************************
; * mexer_asteroide -	avança o asteroide para a próxima posição
;						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
;									R8 -> DEF_ASTEROIDE
;									R7 -> MOVIMENTAÇÃO
; ******************************************************************************

mexer_asteroide:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R7
	PUSH R8
	PUSH R9
	MOV  R1, [R9]								; linha do pixel de referência do asteroide
	MOV  R2, [R9 + 2]							; coluna do pixel de referência do asteroide
	MOV  R3, R8									; tabela do desenho do asteroide
	CALL apaga_objeto							; apaga o asteroide
	ADD  R1, R7									; incrementa em 1 a linha
	ADD  R2, 1									; incrementa em 1 a coluna
	MOV  [R9], R1								; atualiza a linha do pixel de referência
	MOV  [R9 + 2], R2							; atualiza a coluna do pixel de referência
	CALL desenha_asteroide						; desenha novamente o asteroide
	MOV  R1, SFX_AK_47							; seleciona o efeito sonoro
	MOV  [TOCA_SOM], R1							; reproduz o efeito sonoro
	PUSH R9
	PUSH R8
	PUSH R7
	POP  R3
	POP  R2
	POP  R1
	JMP  ha_tecla								; aguarda que a tecla deixe de ser premida
	
; ******************************************************************************
; * mexer_sonda -		apaga a sonda, decrementa por 1 a lin do pixel de
; *						referencia e desenha-a outra vez
; *						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
; *									R8 -> MOVIMENTAÇÃO
; ******************************************************************************

mexer_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R8
	PUSH R9
	MOV  R1, [REF_SONDA]						; linha do pixel de referência da sonda
	MOV  R2, [REF_SONDA + 2]					; coluna do pixel de referência da sonda
	MOV  R3, DEF_SONDA							; tabela do desenho da sonda
	CALL apaga_objeto							; apaga a sonda
	SUB  R1, 1									; decrementa em 1 a linha
	SUB  R2, R8
	MOV  [REF_SONDA], R1						; atualiza a linha do pixel de referência
	MOV  [REF_SONDA + 2] R2						; atualiza a coluna do pixel de referência
	CALL desenha_sonda							; desenha novamente a sonda
	PUSH R9
	PUSH R8
	POP  R3
	POP  R2
	POP  R1
	JMP  ha_tecla								; aguarda que a tecla deixe de ser premida


; ******************************************************************************
; * desenha_asteroide -	chama a funcao desenha_objeto com os argumentos
; *						para desenhar o asteroide.
; *						ARGUMENTOS R9 - PIXEL DE REFERENCIA
; *								   R8 - DEF_ASTEROIDE
; ******************************************************************************

desenha_asteroide:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R8
	PUSH R9
	MOV  R1, [R9]
	MOV  R2, [R9 + 2]
	MOV  R3, R8
	CALL desenha_objeto
	POP R9
	POP R8
	POP  R3
	POP  R2
	POP  R1
	RET
	
; ******************************************************************************
; * desenha_sonda -		chama a funcao desenha_objeto com os argumentos
; *						necessarios para desenhar a sonda.
; *						ARGUMENTOS R9 - PIXEL DE REFERENCIA
; ******************************************************************************

desenha_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R9
	MOV  R1, [R9]
	MOV  R2, [R9 + 2]
	MOV  R3, DEF_SONDA
	CALL desenha_objeto
	POP R9
	POP  R3
	POP  R2
	POP  R1
	RET
	
; ******************************************************************************
; * desenha_painel -	chama a funcao desenha_objeto com os argumentos
; * 					necessarios para desenhar o painel da nave.
; ******************************************************************************

desenha_painel:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV  R1, [REF_PAINEL]
	MOV  R2, [REF_PAINEL + 2]
	MOV  R3, DEF_PAINEL
	CALL desenha_objeto
	POP  R3
	POP  R2
	POP  R1
	RET


; ******************************************************************************
; * desenha_objeto -	desenha um objeto respeitando os argumentos:
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
		ADD  R2, 1								; próxima coluna
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
; *	apaga_objeto -		apaga um objeto respeitando os argumentos:
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
	apaga_linha:								; desenha os pixels do objeto a partir da tabela
		MOV [DEFINE_LINHA], R1					; seleciona a linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R3					; altera a cor do pixel na linha e coluna selecionadas
		ADD R2, 1								; próxima coluna
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



; ******************************************************************************
; *	ROTINAS INTERRUPCOES
; ******************************************************************************

rot_energia:
	PUSH R0
	MOV R0, 1
	MOV [DECREMENTA_ENERGIA], R0
	POP R0
	RFE
	
rot_painel:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV  R1, LIN_ECRA_PAINEL
	MOV  R2, COL_ECRA_PAINEL
	MOV [DEFINE_LINHA], R1					; seleciona a linha
	MOV [DEFINE_COLUNA], R2					; seleciona a coluna
	MOV R3, [OBTEM_COR_PIXEL]
	MOV [ATUALIZA_PAINEL], R3
	POP R3
	POP R2
	POP R1
	RFE
	