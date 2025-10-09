# Diagrama de Estados

```mermaid
stateDiagram-v2
    [*] --> MenuPrincipal

    MenuPrincipal --> Jogando: Enter
    MenuPrincipal --> TelaExit: S
    MenuPrincipal --> [*]: S (confirma)

    Jogando --> Pausado: P
    Jogando --> Hit: Colis�o
    Jogando --> NovoNivel: Objetivo Completo
    Jogando --> GameOver: Sem Vidas

    Pausado --> Jogando: P
    Pausado --> MenuPrincipal: M

    Hit --> Jogando: Timer
    Hit --> GameOver: Sem Vidas

    NovoNivel --> Jogando: Timer
    NovoNivel --> Finalizado: �ltimo N�vel

    GameOver --> MenuPrincipal: M
    GameOver --> TelaExit: S

    TelaExit --> MenuPrincipal: M
    TelaExit --> [*]: S

    Finalizado --> MenuPrincipal: M
    Finalizado --> TelaExit: S

    state Jogando {
        [*] --> Movimento
        Movimento --> VerificaColisao
        VerificaColisao --> VerificaComida
        VerificaComida --> AtualizaPontos
        AtualizaPontos --> Movimento
    }
```

# Detalhamento dos Estados

## Estados Principais

1. **MenuPrincipal** (SCREEN_MENU)
   - Estado inicial do jogo
   - Permite sele��o de dificuldade
   - Mostra controles e op��es

2. **Jogando** (statusGame == ON)
   - Loop principal do jogo
   - Controle da cobra
   - Atualiza��o de f�sica

3. **Pausado** (SCREEN_PAUSE)
   - Jogo em pausa
   - Mant�m estado atual
   - Permite retorno ou sa�da

4. **Hit** (SCREEN_HIT)
   - Ap�s colis�o
   - Mostra vidas restantes
   - Timer de recupera��o

5. **NovoNivel** (SCREEN_LEVEL)
   - Transi��o entre n�veis
   - Mostra progresso
   - Ajusta dificuldade

6. **GameOver** (SCREEN_GAMEOVER)
   - Fim de jogo
   - Mostra pontua��o
   - Op��es de rein�cio

7. **TelaExit** (SCREEN_EXIT)
   - Confirma��o de sa�da
   - Op��o de retorno

8. **Finalizado** (SCREEN_FINISHED)
   - Jogo completado
   - Congratula��es
   - Pontua��o final

## Sub-estados do Estado "Jogando"

1. **Movimento**
   - Atualiza��o de posi��o
   - Input do usu�rio
   - F�sica da cobra

2. **VerificaColisao**
   - Colis�o com bordas
   - Colis�o com pr�prio corpo
   - Determina perda de vida

3. **VerificaComida**
   - Detec��o de coleta
   - Crescimento da cobra
   - Nova posi��o da comida

4. **AtualizaPontos**
   - C�lculo de pontua��o
   - Multiplicadores
   - Progresso do n�vel

# Transi��es

## Transi��es por Tecla
- **P**: Alterna entre Jogando e Pausado
- **M**: Retorna ao Menu Principal
- **S**: Acessa tela de sa�da
- **Enter**: Inicia jogo
- **I**: Toggle informa��es

## Transi��es Autom�ticas
- Timer em Hit ? Jogando
- Timer em NovoNivel ? Jogando
- Colis�o em Jogando ? Hit
- Sem vidas em Hit ? GameOver
- Objetivo completo em Jogando ? NovoNivel
- �ltimo n�vel completo ? Finalizado

# Estados de Debug

Em modo debug (-show ou -log):
- Informa��es adicionais em cada estado
- Logging de transi��es
- Dados de performance
