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
PIN							EQU 0E000H			; endereço do periférico PIN 

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
OBTEM_COR_PIXEL				EQU 6010H            ; endereço do comando para obter a cor do pixel selecionado
APAGA_ECRA					EQU 6002H            ; endereço do comando para apagar todos os pixels já desenhados
APAGA_ECRA_ESP				EQU 6000H            ; endereço do comando para apagar os pixeis desenhados no ecrã especificado
DEFINE_LINHA				EQU 600AH            ; endereço do comando para definir a linha
DEFINE_COLUNA				EQU 600CH            ; endereço do comando para definir a coluna
DEFINE_PIXEL				EQU 6012H            ; endereço do comando para escrever um pixel
APAGA_AVISO					EQU 6040H            ; endereço do comando para apagar o aviso de nenhum cenário selecionado
SELECIONA_ECRA				EQU 6004H            ; endereço do comando para selecionar o ecra
SELECIONA_CENARIO_FUNDO		EQU 6042H            ; endereço do comando para selecionar uma imagem de fundo
    CENARIO_JOGO            EQU 0                ; cenário de jogo
    CENARIO_COMECO            EQU 1                ; cenário do ecra de começo
    CENARIO_SEM_ENERGIA        EQU 2                ; cenário de derrota por falta de energia
	CENARIO_GAME_OVER		EQU 4				; cenário de fim de jogo
APAGA_CENARIO_FRONTAL		EQU 6044H			; endereço do comando para apagar o foreground de jogo
SELECIONA_CENARIO_FRONTAL	EQU 6046H			; endereço do comando para selecionar o foreground de jogo
	CENARIO_PAUSA			EQU 3					; foreground de pausa do jogo
TOCA_LOOP					EQU 605CH            ; endereço do comando para tocar um som em loop
PAUSA_LOOP					EQU 605EH            ; endereço do comando para pausar o som em loop
RESUME_LOOP					EQU 6060H            ; endereço do comando para resumir o som em loop
TOCA_VIDEO_SOM                EQU 605AH            ; endereço do comando para tocar um som
    SOM_DISPARO                EQU 0                ; audio de disparo
    SOM_COLISAO_BOM            EQU 1                ; audio de colisão com um asteroide bom
    SOM_COLISAO_MAU            EQU 2                ; audio de colisão com um asteroide mau
    VIDEO_COLISAO_NAVE        EQU 3                ; video de colisão com a nave
    SOM_COLISAO_NAVE        EQU 4                ; audio de colisão com a nave
    SOM_SEM_ENERGIA            EQU 5                ; audio de derrota por falta de energia
	SOM_CONTROLO			EQU 6					; audio de comandos de controlo
	SOM_FUNDO				EQU 7					; audio de fundo
ECRA_PRINCIPAL                EQU 4                ; ecra onde a sonda e painel localizam-se
	
; Mascaras
MASCARA_0AH					EQU 0AH				; Mascaras para a conversão de hexa para decimal
MASCARA_0A0H				EQU 0A0H
MASCARA_0FH					EQU 0FH
MASCARA_0F0H				EQU 0F0H
MASCARA_ALEATORIO			EQU 3H				; para isolar os 2 bits de menor peso, ao ler o periférico PIN

; Dimensões, coordenadas e movimento dos Objetos
ALTURA_ASTEROIDE			EQU 5				; altura do objeto asteroide
LARGURA_ASTEROIDE			EQU 5				; largura do objeto asteroide
LIN_ASTEROIDE_INICIAL		EQU -1				; linha referencia inicial dos asteroides
COL_ASTEROIDE_ESQ			EQU 0				; coluna referencia do asteroide que aparece á esquerda
COL_ASTEROIDE_MEIO			EQU 29				; coluna referencia do asteroide que aparece no meio
COL_ASTEROIDE_DIR			EQU 59				; coluna referencia do asteroide que aparece á direita

MOVER_DIREITA				EQU 1				; valores a somar no pixel da coluna
MOVER_ESQUERDA				EQU -1
MOVER_VERTICAL				EQU 0

ALTURA_SONDA				EQU 1				; altura do objeto sonda
LARGURA_SONDA				EQU 1				; largura do objeto sonda
LIN_SONDA_INICIAL			EQU 26				; linha referencia da sonda
COL_SONDA_ESQ				EQU 26				; coluna referencia da sonda da esquerda
COL_SONDA_MEIO				EQU 32				; coluna referencia da sonda do meio
COL_SONDA_DIREITA			EQU 38				; coluna referencia da sonda	

ALTURA_PAINEL				EQU 5				; altura do objeto painel
LARGURA_PAINEL				EQU 15				; largura do objeto painel
LIN_PAINEL_INIC				EQU 27				; linha referencia do painel
COL_PAINEL_INIC				EQU 25				; linha referencia do painel
LIN_ECRA_PAINEL				EQU 29				; linha referencia do ecra do painel
COL_ECRA_PAINEL				EQU 29				; coluna referencia do ecra do painel
COL_ECRA_FINAL				EQU 35				; última coluna do ecra do painel

VERIFICA_LADOS				EQU 6				; pixeis que vai verificar para cada lado do asteroide

; Cores
AMARELO						EQU 0FFE0H			; cor do pixel: amarelo
VERMELHO					EQU 0FF00H			; cor do pixel: vermelho
PRETO						EQU 0F000H			; cor do pixel: preto
BRANCO						EQU 0FFFFH			; cor do pixel: branco
CINZENTO					EQU 0F555H			; cor do pixel: cinzento
VERDE						EQU 0F5F0H			; cor do pixel: verde
LARANJA						EQU 0FF50H			; cor do pixel: laranja
AZUL						EQU 0F00FH			; cor do pixel: azul
CIANO                       EQU 0F0FFH        	; cor do pixel: ciano
ROXO						EQU 0F90FH			; cor do pixel: roxo

; Constantes do jogo
ENERGIA_INICIAL 			EQU 100H			; energia inicial da nave
N_ASTEROIDES				EQU 4				; numero de asteroides no jogo
ENERGIA_PASSIVA				EQU 3				; energia a decrementar passivamente
N_SONDAS					EQU 3				; numero de sondas no jogo
LIMITE_SONDA				EQU 15				; limite da movimentação da sonda
LIMITE_ASTEROIDE			EQU 32				; limite de movimentação do asteroide
MINERAR_BOM					EQU 25				; quantidade de energia ganha ao minerar um asteroide bom
CUSTO_SONDA					EQU 5
ULTIMO_ECRA_ASTEROIDE		EQU 3				; ultimo ecra onde é desenhado um asteroide

; Teclas do jogo
TECLA_START					EQU 0CH				; tecla para começar o jogo
TECLA_PAUSE					EQU 0DH				; tecla para parar e continuar o jogo
TECLA_QUIT					EQU 0EH				; tecla para terminar o jogo
TECLA_SONDA_ESQ				EQU 0				; tecla para disparar a sonda da esquerda
TECLA_SONDA_MEIO			EQU 1				; tecla para disparar a sonda do meio
TECLA_SONDA_DIREITA			EQU 2				; tecla para disparar a sonda da direita

ATRASO			         	EQU 4FFFH			; para o ciclo de atraso

; ******************************************************************************
; * STACKS
; ******************************************************************************

PLACE 1000H

; Reserva do espaço para as pilhas dos processos
	STACK 100H			; espaço reservado para a pilha do processo "programa principal"
SP_inicial_prog_princ:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "controlo"
SP_inicial_controlo:		; este é o endereço com que o SP deste processo deve ser inicializado
							
	STACK 100H			; espaço reservado para a pilha do processo "teclado"
SP_inicial_teclado:			; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "energia"
SP_inicial_energia:
	
	STACK 100H			; espaço reservado para a pilha do processo "painel"
SP_inicial_painel:


; SP inicial de cada processo "sonda"
	STACK 100H			; espaço reservado para a pilha do processo "sonda", instância 0
SP_inicial_sonda_0:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "sonda", instância 1
SP_inicial_sonda_1:		; este é o endereço com que o SP deste processo deve ser inicializado

	STACK 100H			; espaço reservado para a pilha do processo "sonda", instância 2
SP_inicial_sonda_2:		; este é o endereço com que o SP deste processo deve ser inicializado


tab_stack_sondas:
	WORD SP_inicial_sonda_0
	WORD SP_inicial_sonda_1
	WORD SP_inicial_sonda_2

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


; ******************************************************************************
; * LOCKS
; ******************************************************************************	

START:				LOCK 0			; LOCK para o controlo desbloquear quando o jogo começa
PAUSE:				LOCK 0			; LOCK para o controlo desbloquear quando o jogo começa
TECLA_CARREGADA: 	LOCK 0			; LOCK para o teclado comunicar aos restantes processos que tecla detetou
ATUALIZA_ASTEROIDE: LOCK 0			; LOCK para a rotina de interrupcao energia comunicar com o processo asteroide
ATUALIZA_SONDA:		LOCK 0			; LOCK para a rotina de interrupcao energia comunicar com o processo sonda
DECREMENTA_ENERGIA: LOCK 0			; LOCK para a rotina de interrupcao energia comunicar com o processo energia
ATUALIZA_PAINEL: 	LOCK 0			; LOCK para a rotina de interrupcao painel comunicar com o processo painel
GAME_OVER: 			LOCK 0			; LOCK para o controlo saber o estado do jogo


; ******************************************************************************
; * INTERRUPCOES
; ******************************************************************************

tab_interrupcoes:
	WORD rot_asteroides
	WORD rot_sondas
	WORD rot_energia
	WORD rot_painel

; ******************************************************************************
; * DADOS
; ******************************************************************************

VALOR_DISPLAYS: WORD ENERGIA_INICIAL			; "variável global" do valor dos displays
EM_PAUSA: WORD 0

; ******************************************************************************
; * Pixeis de referência dos objetos, i.e. onde começam a ser desenhados
; ******************************************************************************
POSSIBILIDADES_ASTEROIDE:													; tabela das possiveis caracteristicas de spawn dos asteroides
	WORD 	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_ESQ, MOVER_DIREITA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_ESQUERDA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_VERTICAL
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_MEIO, MOVER_DIREITA
	WORD	LIN_ASTEROIDE_INICIAL, COL_ASTEROIDE_DIR, MOVER_ESQUERDA
	
REF_ASTEROIDES:																; tabela para guardar os pixeis de referencia dos asteroides
	WORD 0,0																; linha, coluna
	WORD 0,0
	WORD 0,0
	WORD 0,0

BOM_OU_MAU:																	; 0 é mau, 1 é bom (75% mau, 25% bom)
	WORD 0
	WORD 0
	WORD 1
	WORD 0
	
TECLA_DISPARAR:
	WORD TECLA_SONDA_ESQ													;teclas para disparar
	WORD TECLA_SONDA_MEIO
	WORD TECLA_SONDA_DIREITA
	

CAR_SONDA_INICIAL:															; pixel de referencia da sonda
	WORD 	LIN_SONDA_INICIAL, COL_SONDA_ESQ, MOVER_ESQUERDA
	WORD	LIN_SONDA_INICIAL, COL_SONDA_MEIO, MOVER_VERTICAL
	WORD	LIN_SONDA_INICIAL, COL_SONDA_DIREITA, MOVER_DIREITA
	
REF_SONDA:																	; tabela para guardar os pixeis de referencia das sondas
	WORD 0,0
	WORD 0,0
	WORD 0,0
	
ESTADO_COLISAO_ASTEROIDE:														; tabela para o processo sonda comunicar com o asteroide que colidiu com uma sonda
	WORD 0
	WORD 0
	WORD 0
	WORD 0

	
REF_PAINEL:										; pixel de referencia do painel
	WORD 	LIN_PAINEL_INIC, COL_PAINEL_INIC
REF_ECRA:										; pixel de referencia do painel
	WORD 	COL_ECRA_PAINEL

; ******************************************************************************
; * Tabelas de desenhos dos objetos
; ******************************************************************************
DEF_ASTEROIDE_MAU:									; tabela do asteroide
	WORD	ALTURA_ASTEROIDE, LARGURA_ASTEROIDE	; dimensões do asteroide
	WORD	0, CINZENTO, CINZENTO, VERMELHO, 0 
	WORD	CINZENTO, VERMELHO, CINZENTO, CINZENTO, CINZENTO
	WORD	CINZENTO, CINZENTO, VERMELHO, CINZENTO, VERMELHO
	WORD	VERMELHO, CINZENTO, CINZENTO, CINZENTO, CINZENTO
	WORD	0, CINZENTO, VERMELHO, CINZENTO, 0
	
DEF_ASTEROIDE_BOM:                                ; tabela do asteroide bom
    WORD    ALTURA_ASTEROIDE, LARGURA_ASTEROIDE    ; dimensões do asteroide
    WORD	0, CINZENTO, CINZENTO, VERDE, 0 
	WORD	CINZENTO, VERDE, CINZENTO, CINZENTO, CINZENTO
	WORD	CINZENTO, CINZENTO, VERDE, CINZENTO, VERDE
	WORD	VERDE, CINZENTO, CINZENTO, CINZENTO, CINZENTO
	WORD	0, CINZENTO, VERDE, CINZENTO, 0
	
DEF_ESPLOSAO_ASTEROIDE_MAU:                        ; tabela da explosão do asteroide mau
    WORD    ALTURA_ASTEROIDE, LARGURA_ASTEROIDE    ; dimensões do asteroide
    WORD    0, 0, AMARELO, 0, CINZENTO 
    WORD    AMARELO, LARANJA, 0, AMARELO, 0
    WORD    0, CINZENTO, LARANJA, 0, 0
    WORD    AMARELO, 0, LARANJA, CINZENTO, AMARELO
    WORD    0, 0, AMARELO, 0, 0

DEF_EXPLOSAO_ASTEROIDE_BOM:                                ; tabela da explosão do asteroide bom
    WORD    ALTURA_ASTEROIDE, LARGURA_ASTEROIDE    ; dimensões do asteroide 
    WORD    0 , 0     , 0     , 0     , 0        ; dimensões iguais as outras para facilitar a manipulação de pixeis
    WORD    0 , 0     , VERDE , 0     , 0
    WORD    0 , VERDE , VERDE , VERDE , 0
    WORD    0 , 0     , VERDE , 0     , 0
    WORD    0 , 0     , 0     , 0     , 0

DEF_SONDA:										; tabela da sonda. (1 pixel apenas)
	WORD 	ALTURA_SONDA, LARGURA_SONDA			; dimensões da sonda
	WORD 	CIANO

DEF_PAINEL:										; tabela do painel de instrumentos
	WORD	ALTURA_PAINEL, LARGURA_PAINEL		; dimensões do painel
	WORD	0		 , 0		, ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , ROXO , 0        , 0
	WORD	0		 , ROXO , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , ROXO , 0
	WORD	ROXO , CINZENTO	, CINZENTO   , CINZENTO	  , PRETO     , PRETO    , PRETO , PRETO     , PRETO    , PRETO , PRETO     , CINZENTO   , CINZENTO   , CINZENTO   , ROXO
    WORD	ROXO , CINZENTO	, CINZENTO   , CINZENTO   , PRETO , PRETO     , PRETO    , PRETO , PRETO     , PRETO    , PRETO , CINZENTO   , CINZENTO   , CINZENTO   , ROXO
	WORD	ROXO , CINZENTO	, CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , CINZENTO   , ROXO


; ******************************************************************************
; * CÓDIGO PRINCIPAL
; ******************************************************************************

PLACE	   0
inicializacoes:
    MOV SP, SP_inicial_prog_princ				; inicialização do Stack Pointer
    MOV [APAGA_AVISO], R1						; apaga o aviso de nenhum cenário selecionado (R1 não é relevante)
    MOV [APAGA_ECRA], R1						; apaga todos os pixels já desenhados (R1 não é relevante)
	MOV R1, ENERGIA_INICIAL
	MOV [DISPLAYS], R1							; escreve linha e coluna a zero nos displays
	MOV [VALOR_DISPLAYS], R1
	MOV BTE, tab_interrupcoes
	EI0
	EI1
    EI2
	EI3
	EI      ; permite interrupções (geral)

MOV R11, N_ASTEROIDES	
CALL controlo
CALL inicio_teclado								; inicia o processo do teclado
CALL inicio_energia									; inicio o processo de energia
CALL inicio_painel
chama_processo_asteroides:
	SUB R11, 1
	CALL asteroide
	CMP R11, 0
	JNZ chama_processo_asteroides
MOV R11, N_SONDAS
chama_processo_sondas:
	SUB R11, 1
	CALL sonda
	CMP R11, 0
	JNZ chama_processo_sondas

fim:
	YIELD
	JMP fim

; ******************************************************************************
; * PROCESSOS
; ******************************************************************************

PROCESS SP_inicial_controlo
	controlo:
	MOV R1, CENARIO_COMECO
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário do main menu
	main_menu:
	MOV R0, [TECLA_CARREGADA]
	MOV R1, TECLA_START
	CMP R0, R1
	JNZ main_menu
	inicializar:
	CALL toca_som_controlo
	MOV R1, SOM_FUNDO
	MOV [TOCA_LOOP], R1
	MOV R1, CENARIO_JOGO
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário de fundo de jogo
	MOV R1, ENERGIA_INICIAL
	MOV [DISPLAYS], R1							; escreve linha e coluna a zero nos displays
	MOV [VALOR_DISPLAYS], R1
	CALL desenha_painel
	MOV [START], R0
	jogo:
	MOV R3, [GAME_OVER]
	MOV R4, 1
	CMP R3, R4
	JZ nave_explodiu
	MOV R4, 2
	CMP R3, R4
	JZ  sem_energia
	MOV R4, 3
	CMP R3, R4
	JZ pausa
	MOV R4, 4
	CMP R3, R4
	JZ  quit_jogo
	JMP jogo
	pausa:
	CALL toca_som_controlo
	CALL pausa_som_fundo
	MOV R1, CENARIO_PAUSA
	MOV [SELECIONA_CENARIO_FRONTAL], R1
	MOV R2, 1
	MOV [EM_PAUSA], R2
	ciclo_pausa:
	MOV R0, [TECLA_CARREGADA]
	MOV R1, TECLA_PAUSE
	CMP R0, R1
	JZ resume
	MOV R1, TECLA_QUIT
	CMP R0, R1
	JZ quit_jogo
	JMP ciclo_pausa
	resume:
	CALL toca_som_controlo
	MOV R1, SOM_FUNDO
	MOV [RESUME_LOOP], R1
	MOV [APAGA_CENARIO_FRONTAL], R1
	MOV [PAUSE], R2
	MOV R2, 0
	MOV [EM_PAUSA], R2
	JMP jogo
	quit_jogo:
	CALL toca_som_controlo
	CALL pausa_som_fundo
	MOV R2, 1
	MOV [APAGA_ECRA], R0
	MOV R1, CENARIO_GAME_OVER
	MOV [SELECIONA_CENARIO_FUNDO], R1			; coloca o cenário de fundo de jogo
	MOV [APAGA_CENARIO_FRONTAL], R1
	JMP fim_jogo
	nave_explodiu:
	CALL pausa_som_fundo
	MOV [APAGA_ECRA], R0
	MOV R1, VIDEO_COLISAO_NAVE
	MOV [TOCA_VIDEO_SOM], R1			; coloca o cenário de fundo de jogo
	MOV R1, SOM_COLISAO_NAVE
	MOV [TOCA_VIDEO_SOM], R1
	JMP fim_jogo
	sem_energia:
	CALL pausa_som_fundo
	MOV R0, SOM_SEM_ENERGIA
	MOV [TOCA_VIDEO_SOM], R0
	MOV R0, 0
	MOV [DISPLAYS], R0
	MOV [APAGA_ECRA], R0
	MOV R1, CENARIO_SEM_ENERGIA			; coloca o cenário de fundo de jogo
	MOV [SELECIONA_CENARIO_FUNDO], R1
	fim_jogo:
	JMP fim_jogo
	
	
	
	

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
			verificar_se_houve_pausa:
            MOV R1, TECLA_PAUSE
            CMP R8, R1     ; verifica se a tecla pausa foi premida
            JNZ verificar_se_houve_terminar_jogo        ; se nao foi salta
            MOV R1, 3							; estado estiver pausado
            MOV [GAME_OVER], R1
            JMP ha_tecla
        verificar_se_houve_terminar_jogo:
            MOV R1, TECLA_QUIT
            CMP R8, R1         ; verifica se a tecla quit foi premida 
            JNZ ha_tecla        ; se nao, salta para ha_tecla
            MOV R1, 4			; estado para terminar o jogo
            MOV [GAME_OVER], R1
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
; * Processo: energia - Processo responsável por decrementar a energia da nave 
; * 					passivamente, através da interrupção rot_energia
; ******************************************************************************
PROCESS SP_inicial_energia
	inicio_energia:
	MOV R0, [START]
	energia:
		CALL verifica_pausa
		MOV R0, [DECREMENTA_ENERGIA]
		MOV R1, ENERGIA_PASSIVA
		ciclo_decrementa:
			CALL decrementa_display
			SUB R1,1
			JNZ ciclo_decrementa
		MOV R1, [VALOR_DISPLAYS]
		MOV [DISPLAYS], R1
		JMP energia

; ******************************************************************************
; * Processo: painel - Processo responsável por alterar o painel da nave 
; * 					passivamente, através da interrupção rot_painel
; ******************************************************************************
PROCESS SP_inicial_painel
	inicio_painel:
		MOV R0, [START]
	painel:
		CALL verifica_pausa
		MOV R11, ECRA_PRINCIPAL
		MOV R0, [ATUALIZA_PAINEL]
		MOV [SELECIONA_ECRA], R11
		CALL altera_painel
		JMP painel
		
; ******************************************************************************
; * Processo: asteroide - Processo responsável por decidir a posicao, mover e 
; * 					verificar colisões do asteroide
; *						Argumentos: R11 - instancia do meteoro
; *						Registos Gerais : R10 - estado de colição
; *										  R9 - pixel de referencia do asteroide
; *										  R8 - tabela de desenho do asteroide
; *										  R7 - movimentação
; *										  R6 - bom ou mau
; *										  R4 - tabela de colisão
; ******************************************************************************		
PROCESS SP_inicial_asteroide_0
	asteroide:
		MOV R0, [START]
		MOV  R10, R11       				; cópia do nº de instância do processo
	    SHL  R10, 1     					; multiplica por 2 porque as tabelas são de WORDS
	    MOV  R9, tab_stack_asteroides       ; tabela com os SPs iniciais das várias instâncias deste processo
	    MOV	 SP, [R9+R10]        			; re-inicializa o SP deste processo, de acordo com o nº de instância
		MOV  R4, ESTADO_COLISAO_ASTEROIDE	; tabela com o estado de colisão do asteroide
		ADD  R4, R10						; ajustar o endereço segundo a instancia do processo
		SHL  R10, 1							; tabela de referencia tem 2 words para cada instancia (linha, coluna)
		MOV  R9, REF_ASTEROIDES				; tabela com os pixeis de referencia dos asteroides
		ADD  R9, R10						; R9 fica com o pixel de referencia do asteroide da instacia do R11

	reset_asteroide:						; determina todos as caracteristicas do asteroide
		CALL gera_asteroide					; gera as carateristicas do asteroide ( bom ou mau e onde spawna/movimenta)
		MOV [R9], R1						; atualizamos a linha de referencia para a inicial
		MOV [R9 + 2], R2					; atualizamos a coluna de referencia para a inicial
		MOV R10 , 0							; estado da colisão com a 0 ( não colidiu com nada)
		MOV [R4], R10						; resetar a tabela de colisão
		CMP	 R6 , 0
		JZ asteroide_mau
		asteroide_bom:
			MOV R8, DEF_ASTEROIDE_BOM
			JMP ciclo_asteroide
		asteroide_mau:
			MOV R8, DEF_ASTEROIDE_MAU
	ciclo_asteroide:
		CALL verifica_pausa
		MOV R0, [ATUALIZA_ASTEROIDE]
		verifica_limite_asteroide:
			MOV R0, [R9]					; obetemos a linha atual do asteroide
			MOV R1, LIMITE_ASTEROIDE		; obetemos a linha limite do asteroide
			CMP R0, R1						; verificamos se está no limite
			JZ acabou_asteroide
		verifica_colisoes:
			CALL verifica_colisão_nave		; devolve para o R10 com o que colidiu (0 - nada, 2 - nave)
			CMP R10, 2
			JZ bateu_nave				
			MOV R10, [R4]
			CMP R10, 1						; verificar se colidiu com a sonda
			JZ morreu_asteroide
		continua_ciclo:
		CALL mexer_asteroide
		JMP ciclo_asteroide
	acabou_asteroide:
		CALL apaga_asteroide
		JMP reset_asteroide
	morreu_asteroide:
		CALL apaga_asteroide
		CMP R6, 1							; verificar se é bom para decidir qual a animação de explosão
		JZ morreu_bom
		morreu_mau:
			MOV R8, SOM_COLISAO_MAU
			MOV [TOCA_VIDEO_SOM], R8			; toca o som de explosao
			MOV R8, DEF_ESPLOSAO_ASTEROIDE_MAU		; tabela de desenho da explosão do asteroide mau
			CALL desenha_asteroide
			MOV R0, [ATUALIZA_ASTEROIDE]			; atraso para a animação com a interrupcao
			CALL apaga_asteroide
			JMP reset_asteroide
		morreu_bom:
			MOV R8, SOM_COLISAO_BOM
			MOV [TOCA_VIDEO_SOM], R8			
			MOV R8, DEF_EXPLOSAO_ASTEROIDE_BOM		; tabela de desenho da explosão do asteroide bom
			CALL desenha_asteroide
			MOV R0, [ATUALIZA_ASTEROIDE]			; atraso para a animação com a interrupcao
			CALL apaga_asteroide
			CALL incrementa_25						; energia ganha por minerar o asteroide bom
			MOV R5, [VALOR_DISPLAYS]
			MOV [DISPLAYS], R5						; atualizar o display
			JMP reset_asteroide
	bateu_nave:
		CALL apaga_asteroide
		MOV R0, 1
		MOV [GAME_OVER], R0							; para comunicar com o processo controlo ( 1 para terminar o jogo)
		JMP reset_asteroide
		
	
		
; ******************************************************************************
; * Processo: sonda - Processo responsável por disparar a sonda na posição e 
; *					  movimentação de acordo com a tecla premida (escolhida pelo
; *					  numero da sua instancia).
; *						Argumentos: R11 - instancia do sonda
; *						Registos Gerais : R9 - pixel de referencia da sonda
; *										  R7 - movimentação
; *										  R6 - tecla para disparar a sonda
; ******************************************************************************
PROCESS SP_inicial_sonda_0
	sonda:
		MOV R0, [START]
		MOV  R10, R11       				; cópia do nº de instância do processo
	    SHL  R10, 1     					; multiplica por 2 porque as tabelas são de WORDS
	    MOV  R9, tab_stack_sondas	        ; tabela com os SPs iniciais das várias instâncias deste processo
	    MOV	 SP, [R9+R10]        			; re-inicializa o SP deste processo, de acordo com o nº de instância
		SHL  R10, 1							; tabela de referencia tem 2 words para cada instancia (linha, coluna)
		MOV  R0, TECLA_DISPARAR				; tabela com as teclas para disparar cada sonda	
		MOV  R10, R11       				; cópia do nº de instância do processo
		SHL  R10, 1     					; multiplica por 2 porque as tabelas são de WORDS
		ADD  R0, R10						; acertar a tabela da tecla de disparar
		MOV R6, [R0]
		
	reset_sonda:
		MOV R0, [TECLA_CARREGADA]			; tecla pressionada no teclado
		CMP R0, R6							; verificar se a tecla é a certa para disparar
		JNZ reset_sonda						; se não for volta a pedir a tecla
		MOV R1, SOM_DISPARO
		MOV [TOCA_VIDEO_SOM], R1			; toca o som de disparo
		CALL decrementa_5					; energia gasta por tiro
		MOV R1, [VALOR_DISPLAYS]
		MOV [DISPLAYS], R1					; atualizar os displays
		MOV R1, CAR_SONDA_INICIAL			; tabela com os valores iniciais da sonda ( diferente para cada instancia da sonda)
		MOV R2, R11							; cópia do nº de instância do processo
		MOV R0, 6
		MUL R2, R0							; multiplicar por 6 porque a tabela são 3 WORDS para cada pixel/movimentação
		ADD R1, R2							; acertar a tabela para o pixel da nossa sonda
		MOV R9, REF_SONDA					; tabela com os valores atuais da sonda ( diferente para cada instancia da sonda)
		MOV R2, R11
		SHL R2, 2							; multiplicar por 4 porque a tabela de pixeis são 2 WORDS para cada sonda
		ADD R9, R2							; acertar a tabela para o pixel da nossa sonda
		MOV R2, [R1]						; linha inicial da sonda
		MOV R3, [R1 + 2]					; coluna inicial da sonda
		MOV R7, [R1 + 4]					; movimentação
		MOV [R9], R2						; atualizar o pixel de referencia da sonda (linha)
		MOV [R9 + 2], R3					; (coluna)
	ciclo_sonda:
		CALL verifica_pausa
		MOV R0, [ATUALIZA_SONDA]
		verifica_limite_sonda:
			MOV R0, [R9]					; obetemos a linha atual da sonda
			MOV R1, LIMITE_SONDA			; obetemos a linha limite da sonda
			CMP R0, R1						; verificamos se está no limite
			JZ morreu_sonda
		verifica_colisao_sonda:
			CALL colisoes
			CMP R8, 1
			JZ morreu_sonda
		MOV R4, ECRA_PRINCIPAL
		MOV [SELECIONA_ECRA] , R4
		CALL mexer_sonda
		JMP ciclo_sonda
	morreu_sonda:							; apagar e resetar a sonda
		MOV R8, 0							; estado de colisão a 0
		CALL apaga_sonda
		JMP reset_sonda
	
; ******************************************************************************
; * ROTINAS - funções gerais que são chamadas por outras funções.
; ******************************************************************************

; ******************************************************************************
; * converte_num : converte a linha e coluna do teclado no valor hexadecimal  			
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
		CMP R2, 0
		JGT retorna_decrementa					; caso ainda haja energia retorna
		MOV R2, 2								; caso contrario atualiza o estado
		MOV [GAME_OVER], R2						; estado sem energia
	retorna_decrementa:						
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
	ADD R5, 1									; linha de baixo do ecrã do painel
	MOV  R2, [REF_ECRA]							; obtém a coluna que está a verde
	MOV  R3, VERDE
	MOV  R4, PRETO
	MOV [DEFINE_LINHA], R1						; seleciona a primeira linha
	MOV [DEFINE_COLUNA], R2						; seleciona a coluna
	MOV [DEFINE_PIXEL], R4						; muda o pixel para preto
	MOV [DEFINE_LINHA], R5						; seleciona a segunda linha
	MOV [DEFINE_PIXEL], R4						; muda o pixel para preto
	CMP R2, R0									; verifica se a coluna atual é a última
	JZ  ultima_col_painel						; se for, a coluna atual passa a ser a primeira
	ADD R2, 1									; passa para a próxima coluna
	MOV [REF_ECRA], R2							; atualiza a referência da coluna
	MOV [DEFINE_LINHA], R1						; seleciona a primeira linha
	MOV [DEFINE_COLUNA], R2						; seleciona a coluna
	MOV [DEFINE_PIXEL], R3						; muda o pixel para verde
	MOV [DEFINE_LINHA], R5						; seleciona a segunda linha
	MOV [DEFINE_PIXEL], R3						; muda o pixel para verde
	JMP altera_painel_ret
	
	ultima_col_painel:
		MOV R2, COL_ECRA_PAINEL					; põe a primeira coluna no R2
		MOV [REF_ECRA], R2						; atualiza a referência da coluna
		MOV [DEFINE_LINHA], R1					; seleciona a primeira linha
		MOV [DEFINE_COLUNA], R2					; seleciona a coluna
		MOV [DEFINE_PIXEL], R3					; muda o pixel para verde
		MOV [DEFINE_LINHA], R5					; seleciona a segunda linha 
		MOV [DEFINE_PIXEL], R3					; muda o pixel para verde
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
; * mexer_asteroide - avança o asteroide para a próxima posição
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
	MOV [SELECIONA_ECRA], R11					; selecionar o ecrã a desenhar
	MOV  R1, [R9]								; linha do pixel de referência do asteroide
	MOV  R2, [R9 + 2]							; coluna do pixel de referência do asteroide
	MOV  R3, R8									; tabela do desenho do asteroide
	CALL apaga_objeto							; apaga o asteroide
	ADD  R1, 1									; incrementa em 1 a linha
	ADD  R2, R7									; incrementa em 1 a coluna
	MOV  [R9], R1								; atualiza a linha do pixel de referência
	MOV  [R9 + 2], R2							; atualiza a coluna do pixel de referência
	CALL desenha_asteroide						; desenha novamente o asteroide
	MOV R1, ECRA_PRINCIPAL
	MOV [SELECIONA_ECRA], R1					;voltar para o ecrã principal
	POP R9
	POP R8
	POP R7
	POP  R3
	POP  R2
	POP  R1
	RET
	
; ******************************************************************************
; * apaga_asteroide - apaga o asteroide no pixel de referencia
; *						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
; *								R8 -> DEF_ASTEROIDE
; ******************************************************************************

apaga_asteroide:
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R1, [R9]						; linha de referencia do asteroide
	MOV R2, [R9 + 2]					; coluna de referencia do asteroide
	MOV [SELECIONA_ECRA], R11			; selecionar o ecrã para apagar os pixeis
	MOV R3, R8							; a tabela do asteroide para as dimensões
	CALL apaga_objeto
	POP R3
	POP R2
	POP R1
	RET
	
; ******************************************************************************
; * mexer_sonda -		apaga a sonda, decrementa por 1 a lin do pixel de
; *						referencia e desenha-a outra vez
; *						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
; *									R7 -> MOVIMENTAÇÃO
; ******************************************************************************

mexer_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R7
	PUSH R9
	MOV R1, ECRA_PRINCIPAL
	MOV [SELECIONA_ECRA], R1					;voltar para o ecrã principal
	MOV  R1, [R9]						; linha do pixel de referência da sonda
	MOV  R2, [R9 + 2]					; coluna do pixel de referência da sonda
	MOV  R3, DEF_SONDA							; tabela do desenho da sonda
	CALL apaga_objeto							; apaga a sonda
	SUB  R1, 1									; decrementa em 1 a linha
	ADD  R2, R7									
	MOV  [R9], R1						; atualiza a linha do pixel de referência
	MOV  [R9 + 2], R2						; atualiza a coluna do pixel de referência
	CALL desenha_sonda							; desenha novamente a sonda
	POP  R9
	POP  R7
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	RET

; ******************************************************************************
; * apaga_sonda -		apaga a sonda no pixel de referencia
; *						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
; ******************************************************************************

apaga_sonda:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	MOV  R1, [R9]						; linha do pixel de referência da sonda
	MOV  R2, [R9 + 2]					; coluna do pixel de referência da sonda
	MOV  R3, DEF_SONDA					; tabela do desenho da sonda
	MOV R4, ECRA_PRINCIPAL
	MOV [SELECIONA_ECRA], R4			; seleciona o ecra onde a sonda está desenhada ( ecra principal)
	CALL apaga_objeto					; apaga a sonda
	POP R4
	POP R3
	POP R2
	POP R1
	RET

; ******************************************************************************
; * colisoes -		deteta as colisoes da sonda com os asteroides
; *						ARGUMENTOS: R9 -> PIXEL DE REFERENCIA
; *						RETORNO:	R8 -> 1 ou 0, conforme tenha havido colisao
; ******************************************************************************


colisoes:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R4
	PUSH R5
	PUSH R7
	MOV R7, CIANO							; valor a por nas tables de colisao
	MOV R5, 0							; ecra a ser testado
	MOV R1, [R9]
	MOV R2, [R9 + 2]
	ciclo_ecras:
	MOV [SELECIONA_ECRA], R5			; seleciona o ecrã a ser testado
	MOV [DEFINE_LINHA], R1
	MOV [DEFINE_COLUNA], R2
	MOV R0, [OBTEM_COR_PIXEL]			; avalia a cor do pixel da sonda no ecra do asteroide
	CMP R0, 0							; verifica se o pixel está preenchido pelo asteroide
	JZ proximo_ecra
	colisao_detetada:
		CMP R0, R7
		JZ  proximo_ecra
		MOV R8, 1						; valor a colocar na tabela de colisao
		SHL R5, 1						; multiplica a instancia do asteroide por 2
		MOV R4, ESTADO_COLISAO_ASTEROIDE;
		ADD R4, R5
		MOV [R4], R8						; atualiza a table de colisões dos asteroides
		SHR R5, 1						; 5 volta a ser a instancia do asteroide
		JMP colisoes_ret
	proximo_ecra:
		CMP R5, ULTIMO_ECRA_ASTEROIDE
		JZ colisoes_ret				; se já for o último asteroide retorna
		ADD R5, 1						; passa para o próximo ecrã
		JMP ciclo_ecras
	colisoes_ret:
		POP R7
		POP R5
		POP R4
		POP R2
		POP R1
		POP R0
		RET

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
	MOV [SELECIONA_ECRA], R11
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
	MOV R1, ECRA_PRINCIPAL
	MOV [SELECIONA_ECRA], R1
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
; *						R11 - ecra a desenhar
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
		ADD R2, 1								; próxima coluna
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
; * gera_aleatorio - gera um número aleatório de 0 a 3 no registo R4 e um número
;					de 0 a 4 no R5 
; ******************************************************************************
gera_aleatorio:
	PUSH R0
	PUSH R1
	MOV R0, [PIN]
	SHR  R0, 4								; isola os bits 4 a 7 do PIN
	MOV  R4, R0								; onde fica o número de 0 a 3
	MOV  R1, MASCARA_ALEATORIO
	AND  R4, R1 							; isola os 2 bits de menor peso 			
	MOV  R5, R0								; onde fica o número de 0 a 4
	MOV  R1, 5
	MOD  R5, R1								; faz o resto da divisão por 5 para obter um número de 0 a 4
	POP R1
	POP R0
	RET

; ******************************************************************************
; * gera_asteroide - atualiza na tabela a direção e o pixel onde o asteroide 
; *					aparece conforme o número de 0 a 4 obtido
; *					ARGUMENTO: R11 - instancia do asteroide
; *					RETORNO R1 - linha do asteroide
; *							R2 - coluna do asteroide
; *							R6 - Bom ou mau
; *							R7 - movimentação do asteroide
; ******************************************************************************
gera_asteroide:
	PUSH R0
	PUSH R4
	PUSH R5
	PUSH R11
	CALL gera_aleatorio						; coloca o número de 0 a 3 no R4 e o de 0 a 4 no R5
	MOV R0, POSSIBILIDADES_ASTEROIDE		; coloca a table das possibilidades de asteroides no R0
	MOV R11, 6								; intervalo entre possibilidades (3 dados x 2 bytes)
	MUL R11, R5								; obtém o intervalo a adicionar ao valor inicial da table
	ADD R0, R11								; obtém a possibilidade
	MOV R1, [R0]							; obtém a linha inicial
	ADD R0, 2								; avança na table
	MOV R2, [R0]							; obtém a coluna inicial
	ADD R0, 2								; avança na table
	MOV R7, [R0]							; obtém a direção do asteroide
	MOV R0, BOM_OU_MAU						; coloca a tabela das chances de ser bom ou mau no R0
	SHL R4, 1								; tabela de words
	ADD R0, R4
	MOV R6 , [R0]
	
	POP R11
	POP R5
	POP R4
	POP R0
	RET
	

; ******************************************************************************
; * verifica_colisão_asteroide - verifica se o asteroide colidiu com a sonda (devolve 1
; *								ou a nave (devolve 2), se não colidiu com nada devolve 0
; *								Verifica á volta do asteroide ( menos na borda de cima pois é impossivel colisão em tal lado 
; *								procurando por um pixel colorido da sonda/nave.
; *					ARGUMENTO: R9 - Pixel de referencia
; *					RETORNO: R10 - estado colisão
; ******************************************************************************
verifica_colisão_nave:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R9
	PUSH R11
	MOV R5, ROXO							; cor da borda da nave
	MOV R1, [R9]							; obter a linha do pixel de referencia do asteroide
	MOV R2, [R9 + 2]						; obter a coluna do pixel de referencia do asteroide
	ADD R1, LARGURA_ASTEROIDE				; obter a linha abaixo do asteroide
	MOV R0, ECRA_PRINCIPAL					; selecionamos o ecrã principal onde estão as sondas e nave
	MOV [SELECIONA_ECRA], R0
	MOV R6, LARGURA_ASTEROIDE				; quantidade de pixeis na borda de baixo do asteroide
		ciclo_baixo:						; igual ao ciclo esquerda mas move-se para a direita verificando os pixeis por debaixo do asteroide
			MOV [DEFINE_LINHA], R1
			MOV [DEFINE_COLUNA], R2
			MOV R3, [OBTEM_COR_PIXEL]
			CMP R3, R5
			JZ colidiu_nave
			ADD R2, 1
			SUB R6, 1
			CMP R6, 0
			JNZ ciclo_baixo
	MOV R10, 0
	JMP retorna_verifica
	colidiu_nave:
		MOV R10, 2								; estado a 2 para comunicar com o processo asteroide que colidiu com uma nave
	retorna_verifica:
		POP R11
		POP R9
		POP R6
		POP R5
		POP R4
		POP R3
		POP R2
		POP R1
		POP R0
		RET


; ******************************************************************************
; * verifica_sonda - verifica qual sonda colidiu com o asteroide e retorna a sua instancia.
; *					ARGUMENTO: R1 - linha do pixel
; *							   R2 - coluna do pixel
; *					RETORNO: R0 - instancia da sonda q colidiu
; ******************************************************************************
verifica_sonda:
	PUSH R3
	PUSH R10
	PUSH R11
	MOV R0, 0
	MOV R11, REF_SONDA
		verifica_linha:
			MOV R10, [R11]
			CMP R10, R1
			JZ verifica_coluna
			JMP proximo_pixel
		verifica_coluna:
			ADD R11, 2
			MOV R10, [R11]
			CMP R10, R2
			JZ retorna_verifica_sonda
			JMP proximo_pixel
	proximo_pixel:
		ADD R0,1
		MOV R11, REF_SONDA
		MOV R3, R0
		SHL R3, 2
		ADD R11, R3
		JMP verifica_linha
	retorna_verifica_sonda:
	POP R11
	POP R10
	POP R3
	RET
	
; ******************************************************************************
; * incrementa_25 - incrementa o valor do display em 25
; ******************************************************************************
incrementa_25:
	PUSH R0
	MOV R0, MINERAR_BOM
	ciclo_minerar:
		CALL incrementa_display
		SUB R0, 1
		CMP R0, 0
		JNZ ciclo_minerar
	POP R0
	RET
	
; ******************************************************************************
; * decrementa_5 - decrementa o valor do display em 5
; ******************************************************************************
decrementa_5:
	PUSH R0
	MOV R0, CUSTO_SONDA
	ciclo_custo:
		CALL decrementa_display
		SUB R0, 1
		CMP R0, 0
		JNZ ciclo_custo
	POP R0
	RET

; ******************************************************************************
; verifica_pausa -	verifica se o jogo está em pausa. Se estiver,
;					espera que o utilizador carregue no botão de resumir.
; ******************************************************************************

verifica_pausa:
PUSH R0
PUSH R1
MOV R0, [EM_PAUSA]
MOV R1, 1
CMP R0, R1
JNZ verifica_pausa_ret
MOV R1, [PAUSE]
verifica_pausa_ret:
	POP R1
	POP R0
	RET

; **********************************************************************
; toca_som_controlo - reproduz o audio das teclas de controlo.
; **********************************************************************

toca_som_controlo:
	PUSH R0
	MOV R0, SOM_CONTROLO
	MOV [TOCA_VIDEO_SOM], R0
	POP R0
	RET

; **********************************************************************
; pausa_som_fundo - pausa o audio de fundo.
; **********************************************************************
pausa_som_fundo:
	PUSH R0
	MOV R0, SOM_FUNDO
	MOV [PAUSA_LOOP], R0
	POP R0
	RET

; ******************************************************************************
; *	ROTINAS INTERRUPCOES
; ******************************************************************************
rot_asteroides:
	PUSH R0
	MOV R0, 1
	MOV [ATUALIZA_ASTEROIDE], R0
	POP R0
	RFE
	
rot_sondas:
	PUSH R0
	MOV R0, 1
	MOV [ATUALIZA_SONDA], R0
	POP R0
	RFE

rot_energia:
	PUSH R0
	MOV R0, 1
	MOV [DECREMENTA_ENERGIA], R0
	POP R0
	RFE
	
rot_painel:
	PUSH R0
	MOV R0, 1
	MOV [ATUALIZA_PAINEL], R0
	POP R0
	RFE
	