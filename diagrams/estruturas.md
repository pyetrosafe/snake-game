# Diagrama de Classes e Estruturas

```mermaid
classDiagram
    class Posicao {
        +GLint x
        +GLint y
    }

    class GameState {
        +GLint statusGame
        +GLint currentScreen
        +GLint fase
        +GLint lifes
        +GLfloat ScorePoints
        +GLint delay
    }

    class Snake {
        +GLint coordinates[MAX_TAIL+1][2]
        +GLint countTail
        +GLint moveToX
        +GLint moveToY
        +GLint Vkey
        +GLint Hkey
    }

    class AudioSystem {
        +FSOUND_STREAM* menuMusic
        +FSOUND_SAMPLE* eatSound
        +FSOUND_SAMPLE* hitSound
        +FSOUND_SAMPLE* winSound
        +FSOUND_SAMPLE* loseSound
    }

    class RenderSystem {
        +GLfloat winXmin
        +GLfloat winXmax
        +GLfloat winYmin
        +GLfloat winYmax
        +void Desenha()
        +void DesenhaCobra()
        +void DesenhaTextos()
    }

    class GameEngine {
        +void Timer()
        +void engineSnake()
        +void ResetValues()
        +bool checkCollision()
    }

    class InputHandler {
        +void Teclado()
        +void TeclasEspeciais()
    }

    class DebugSystem {
        +bool storeLogEnabled
        +bool showLogEnabled
        +FILE* logFile
        +void StoreDebugLog()
        +void Debug()
    }

    GameEngine -- GameState
    GameEngine -- Snake
    GameEngine -- AudioSystem
    GameEngine -- RenderSystem
    GameEngine -- InputHandler
    GameEngine -- DebugSystem
    RenderSystem -- Snake
    RenderSystem -- Posicao
```

# Descri��o das Classes e Estruturas

## Estruturas de Dados B�sicas

### Posicao
- Estrutura para armazenar coordenadas 2D
- Usada principalmente para a posi��o da comida
- Membros:
  - `x`: Coordenada horizontal
  - `y`: Coordenada vertical

### Snake
- Representa a cobra e seus segmentos
- Armazena posi��es e estado de movimento
- Membros:
  - `coordinates`: Array de posi��es dos segmentos
  - `countTail`: Tamanho atual da cobra
  - `moveToX/Y`: Dire��o do movimento
  - `Vkey/Hkey`: Estado das teclas

## Sistemas Principais

### GameState
- Controla o estado geral do jogo
- Gerencia pontua��o e progress�o
- Membros:
  - `statusGame`: Estado ativo/inativo
  - `currentScreen`: Tela atual
  - `fase`: N�vel atual
  - `lifes`: Vidas restantes
  - `ScorePoints`: Pontua��o
  - `delay`: Velocidade do jogo

### AudioSystem
- Gerencia todos os efeitos sonoros
- Controla m�sica de fundo
- Membros:
  - Diferentes tipos de som (FMOD)
  - Controles de volume
  - Estados de reprodu��o

### RenderSystem
- Sistema de renderiza��o OpenGL
- Gerencia a interface gr�fica
- M�todos:
  - `Desenha()`: Renderiza��o principal
  - `DesenhaCobra()`: Renderiza a cobra
  - `DesenhaTextos()`: Renderiza UI

### GameEngine
- N�cleo da l�gica do jogo
- Controla f�sica e regras
- M�todos:
  - `Timer()`: Loop principal
  - `engineSnake()`: L�gica da cobra
  - `ResetValues()`: Reinicializa��o
  - `checkCollision()`: Detec��o de colis�es

## Sistemas de Suporte

### InputHandler
- Processa entrada do usu�rio
- Gerencia controles
- M�todos:
  - `Teclado()`: Teclas normais
  - `TeclasEspeciais()`: Teclas especiais

### DebugSystem
- Sistema de debugging e logging
- Auxilia desenvolvimento
- Membros:
  - `storeLogEnabled`: Ativa logging
  - `showLogEnabled`: Mostra debug
  - `logFile`: Arquivo de log
- M�todos:
  - `StoreDebugLog()`: Registra eventos
  - `Debug()`: Mostra informa��es

# Rela��es entre Componentes

1. **GameEngine ? GameState**
   - Atualiza estado do jogo
   - Controla progress�o

2. **GameEngine ? Snake**
   - Atualiza posi��es
   - Gerencia crescimento

3. **GameEngine ? AudioSystem**
   - Dispara efeitos sonoros
   - Controla m�sica

4. **RenderSystem ? Snake**
   - Renderiza segmentos
   - Atualiza visual

5. **GameEngine ? DebugSystem**
   - Registra eventos
   - Monitora estado

# Notas de Implementa��o

1. **Otimiza��es**
   - Uso de arrays fixos para performance
   - Minimiza��o de aloca��o din�mica
   - Cache de posi��es

2. **Manutenibilidade**
   - Separa��o de responsabilidades
   - Interfaces claras
   - Logging extensivo

3. **Extensibilidade**
   - Sistema modular
   - F�cil adi��o de features
   - Debug incorporado
