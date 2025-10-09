# Diagrama de Sequ�ncia

```mermaid
sequenceDiagram
    participant Main
    participant Init
    participant GameLoop
    participant Input
    participant Physics
    participant Render
    participant Audio
    participant Debug

    Main->>Init: Inicializa()
    Init->>Audio: FSOUND_Init()
    Init->>Debug: SetupDebug()
    Init->>Render: SetupOpenGL()

    loop Game Loop
        GameLoop->>Input: ProcessInput()
        Input-->>GameLoop: Input Events

        alt Game Active
            GameLoop->>Physics: engineSnake()
            Physics->>Physics: checkCollisions()
            Physics->>Physics: updatePositions()

            alt Collision Detected
                Physics->>Audio: playHitSound()
                Physics->>GameLoop: updateLives()
            end

            alt Food Collected
                Physics->>Audio: playEatSound()
                Physics->>Physics: growSnake()
                Physics->>Physics: updateScore()
            end

            Physics-->>GameLoop: Game State
        end

        GameLoop->>Render: Desenha()
        Render->>Render: DesenhaCobra()
        Render->>Render: DesenhaTextos()

        opt Debug Mode
            GameLoop->>Debug: StoreDebugLog()
            GameLoop->>Debug: UpdateDebugDisplay()
        end

        GameLoop->>GameLoop: Timer(delay)
    end
```

# Detalhamento das Sequ�ncias

## 1. Inicializa��o

### Sequ�ncia Principal
1. `Main` inicia o programa
2. `Init` configura sistemas
3. Setup de �udio (FMOD)
4. Configura��o OpenGL
5. Inicializa��o de vari�veis

### Componentes Envolvidos
- Sistema principal
- Sistema de �udio
- Sistema de renderiza��o
- Sistema de debug

## 2. Loop Principal do Jogo

### Ciclo de Vida
1. Processamento de Input
2. Atualiza��o de f�sica
3. Renderiza��o
4. Debug (se ativo)

### Input
- Teclas direcionais
- Comandos de controle
- Estado do jogo

### F�sica
1. Verifica��o de colis�es
2. Atualiza��o de posi��es
3. Crescimento da cobra
4. Pontua��o

### Renderiza��o
1. Limpeza do buffer
2. Desenho da cobra
3. Desenho da comida
4. Interface do usu�rio

## 3. Eventos do Jogo

### Colis�es
1. Detec��o
2. Efeito sonoro
3. Atualiza��o de vidas
4. Reset de posi��o

### Coleta de Comida
1. Verifica��o de posi��o
2. Som de coleta
3. Crescimento
4. Nova posi��o da comida

### Mudan�a de N�vel
1. Verifica��o de objetivo
2. Atualiza��o de estado
3. Ajuste de dificuldade
4. Reset de posi��es

## 4. Sistema de Debug

### Logging
1. Registro de eventos
2. Estado do jogo
3. Posi��es
4. Performance

### Display
1. Informa��es na tela
2. Estado atual
3. Vari�veis importantes
4. Contadores

# Fluxos Especiais

## 1. Modo Teste

```mermaid
sequenceDiagram
    participant Main
    participant TestMode
    participant GameLoop
    participant Debug

    Main->>TestMode: InitTestMode()
    TestMode->>GameLoop: SetMaxLevel()
    TestMode->>GameLoop: SetInitialConditions()

    loop Test Execution
        GameLoop->>Debug: LogTestData()
        Debug-->>TestMode: ValidationResults
    end
```

## 2. Recupera��o de Colis�o

```mermaid
sequenceDiagram
    participant Physics
    participant GameState
    participant Audio
    participant Timer

    Physics->>GameState: DetectCollision()
    GameState->>Audio: PlayHitSound()
    GameState->>Timer: StartRecoveryTimer()
    Timer-->>GameState: TimerComplete
    GameState->>Physics: ResumeGame()
```

## 3. Transi��o de N�vel

```mermaid
sequenceDiagram
    participant GameState
    participant Physics
    participant Audio
    participant Display

    GameState->>Physics: CheckLevelComplete()
    Physics->>GameState: ConfirmCompletion()
    GameState->>Audio: PlayLevelUpSound()
    GameState->>Display: ShowLevelTransition()
    Display-->>GameState: TransitionComplete
```

# Notas de Implementa��o

1. **Timing**
   - Uso de Timer para controle de FPS
   - Separa��o entre f�sica e render
   - Controle de estados temporais

2. **Estado do Jogo**
   - M�quina de estados clara
   - Transi��es bem definidas
   - Persist�ncia de dados

3. **Debug**
   - Logging n�o-bloqueante
   - Informa��es em tempo real
   - Rastreamento de eventos
