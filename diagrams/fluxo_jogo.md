# Diagrama de Fluxo do Jogo

```mermaid
graph TD
    A[In�cio do Programa] --> B[Inicializa��o]
    B --> C[Menu Principal]

    C --> |Nova Partida| D[Iniciar Jogo]
    C --> |Sair| E[Encerrar]

    D --> F[Loop Principal]

    F --> G{Estado do Jogo}
    G --> |Jogando| H[Atualizar Posi��es]
    G --> |Pausado| I[Tela de Pausa]
    G --> |Game Over| J[Tela Game Over]
    G --> |N�vel Completo| K[Pr�ximo N�vel]

    H --> L{Verificar Colis�es}
    L --> |Sem Colis�o| M[Verificar Comida]
    L --> |Colis�o| N[Perder Vida]

    M --> |Pegou Comida| O[Aumentar Cobra]
    M --> |N�o Pegou| F

    O --> P[Atualizar Pontua��o]
    P --> F

    N --> Q{Vidas Restantes?}
    Q --> |Sim| F
    Q --> |N�o| J

    I --> |Despausar| F
    J --> C
    K --> F

    style A fill:#f9f,stroke:#333,stroke-width:4px
    style B fill:#bbf,stroke:#333,stroke-width:2px
    style C fill:#bfb,stroke:#333,stroke-width:2px
```

# Legenda

## Cores
- Rosa: Entrada do programa
- Azul claro: Inicializa��o
- Verde claro: Estados principais

## Tipos de N�s
- Ret�ngulos: Processos
- Losangos: Decis�es
- C�rculos: Conectores

## Fluxos
- Setas s�lidas: Fluxo principal
- Setas tracejadas: Fluxo alternativo

# Descri��o Detalhada

1. **In�cio do Programa**
   - Carregamento das bibliotecas
   - Verifica��o de par�metros de linha de comando

2. **Inicializa��o**
   - Setup do OpenGL
   - Carregamento de recursos de �udio
   - Inicializa��o de vari�veis

3. **Menu Principal**
   - Interface do usu�rio
   - Sele��o de dificuldade
   - Op��es de jogo

4. **Loop Principal**
   - Gerenciamento de estados
   - Atualiza��o de f�sica
   - Renderiza��o

5. **Sistema de Colis�es**
   - Verifica��o de bordas
   - Verifica��o de auto-colis�o
   - Gerenciamento de vidas

6. **Sistema de N�veis**
   - Progress�o de dificuldade
   - Objetivos por n�vel
   - Condi��es de vit�ria
