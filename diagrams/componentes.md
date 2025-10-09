# Diagrama de Componentes

```mermaid
graph TB
    subgraph "Sistema Principal"
        Main[Main System]
        GameEngine[Game Engine]
        RenderSystem[Render System]
        AudioSystem[Audio System]
        InputSystem[Input System]
        DebugSystem[Debug System]
    end

    subgraph "Bibliotecas Externas"
        OpenGL[OpenGL/GLUT]
        FMOD[FMOD Sound]
        CPP[C++ STD]
    end

    subgraph "Recursos"
        Sounds[Arquivos de Som]
        Config[Configura��es]
        Logs[Arquivos de Log]
    end

    Main --> GameEngine
    Main --> RenderSystem
    Main --> AudioSystem
    Main --> InputSystem
    Main --> DebugSystem

    GameEngine --> OpenGL
    RenderSystem --> OpenGL
    AudioSystem --> FMOD
    DebugSystem --> CPP

    AudioSystem --> Sounds
    DebugSystem --> Logs
    Main --> Config
```

# Arquitetura do Sistema

## 1. Componentes Principais

### 1.1 Main System
- Controle principal do programa
- Inicializa��o de subsistemas
- Gerenciamento de estados
- Loop principal do jogo

### 1.2 Game Engine
- L�gica do jogo
- F�sica
- Colis�es
- Regras do jogo

### 1.3 Render System
- Interface OpenGL
- Renderiza��o de objetos
- Gerenciamento de texturas
- Interface do usu�rio

### 1.4 Audio System
- Interface FMOD
- Gerenciamento de sons
- Controle de volume
- Efeitos sonoros

### 1.5 Input System
- Captura de entrada
- Mapeamento de teclas
- Controle de estados

### 1.6 Debug System
- Logging
- Informa��es em tempo real
- Ferramentas de desenvolvimento

## 2. Depend�ncias Externas

### 2.1 OpenGL/GLUT
- Vers�o: 3.7.6
- Respons�vel por:
  - Renderiza��o 2D
  - Janela do jogo
  - Eventos de sistema

### 2.2 FMOD Sound
- Vers�o: 3.75
- Respons�vel por:
  - Sistema de �udio
  - Efeitos sonoros
  - M�sica de fundo

### 2.3 C++ Standard Library
- Usado para:
  - I/O de arquivos
  - Gerenciamento de mem�ria
  - Estruturas de dados

## 3. Recursos

### 3.1 Arquivos de Som
- `SomMenu.mp3`
- `SomComeu.wav`
- `SomChoque2.wav`
- `SomParabens1.mp3`
- `SomPerdeu.wav`

### 3.2 Configura��es
- Par�metros do jogo
- Estados salvos
- Configura��es de debug

### 3.3 Arquivos de Log
- Registro de eventos
- Informa��es de debug
- Dados de performance

# Intera��es entre Componentes

## 1. Fluxo de Dados

```mermaid
flowchart TD
    A[Input System] -->|Eventos| B[Game Engine]
    B -->|Estado| C[Render System]
    B -->|Triggers| D[Audio System]
    B -->|Logs| E[Debug System]
```

## 2. Ciclo de Vida

```mermaid
stateDiagram-v2
    [*] --> Inicializa��o
    Inicializa��o --> Loop
    Loop --> Atualiza��o
    Atualiza��o --> Renderiza��o
    Renderiza��o --> Loop
    Loop --> [*]
```

## 3. Depend�ncias de Compila��o

```mermaid
graph TB
    Main -->|Inclui| GameEngine
    GameEngine -->|Requer| OpenGL
    GameEngine -->|Requer| FMOD
    RenderSystem -->|Requer| OpenGL
    AudioSystem -->|Requer| FMOD
```

# Considera��es de Design

## 1. Modularidade
- Componentes independentes
- Interfaces bem definidas
- Baixo acoplamento

## 2. Performance
- Renderiza��o otimizada
- Gerenciamento de mem�ria
- Controle de recursos

## 3. Manutenibilidade
- C�digo organizado
- Documenta��o clara
- Ferramentas de debug

## 4. Extensibilidade
- Arquitetura modular
- Pontos de extens�o
- Configura��o flex�vel
