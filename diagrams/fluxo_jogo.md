# Diagrama de Fluxo do Jogo

```mermaid
graph TD
    A[Início do Programa] --> B[Inicialização]
    B --> C[Menu Principal]

    C --> |Nova Partida| D[Iniciar Jogo]
    C --> |Sair| E[Encerrar]

    D --> F[Loop Principal]

    F --> G{Estado do Jogo}
    G --> |Jogando| H[Atualizar Posições]
    G --> |Pausado| I[Tela de Pausa]
    G --> |Game Over| J[Tela Game Over]
    G --> |Nível Completo| K[Próximo Nível]

    H --> L{Verificar Colisões}
    L --> |Sem Colisão| M[Verificar Comida]
    L --> |Colisão| N[Perder Vida]

    M --> |Pegou Comida| O[Aumentar Cobra]
    M --> |Não Pegou| F

    O --> P[Atualizar Pontuação]
    P --> F

    N --> Q{Vidas Restantes?}
    Q --> |Sim| F
    Q --> |Não| J

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
- Azul claro: Inicialização
- Verde claro: Estados principais

## Tipos de Nós
- Retângulos: Processos
- Losangos: Decisões
- Círculos: Conectores

## Fluxos
- Setas sólidas: Fluxo principal
- Setas tracejadas: Fluxo alternativo

# Descrição Detalhada

1. **Início do Programa**
   - Carregamento das bibliotecas
   - Verificação de parâmetros de linha de comando

2. **Inicialização**
   - Setup do OpenGL
   - Carregamento de recursos de áudio
   - Inicialização de variáveis

3. **Menu Principal**
   - Interface do usuário
   - Seleção de dificuldade
   - Opções de jogo

4. **Loop Principal**
   - Gerenciamento de estados
   - Atualização de física
   - Renderização

5. **Sistema de Colisões**
   - Verificação de bordas
   - Verificação de auto-colisão
   - Gerenciamento de vidas

6. **Sistema de Níveis**
   - Progressão de dificuldade
   - Objetivos por nível
   - Condições de vitória
