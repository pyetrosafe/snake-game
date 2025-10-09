# Diagrama de Casos de Uso

```mermaid
graph TB
    subgraph "Casos de Uso - Snake Game"
        Player[Jogador]

        subgraph "A��es Principais"
            Start[Iniciar Jogo]
            Move[Mover Cobra]
            Pause[Pausar Jogo]
            Exit[Sair do Jogo]
        end

        subgraph "Controle de Jogo"
            SelectDiff[Selecionar Dificuldade]
            ToggleInfo[Alternar Informa��es]
            AdjustVol[Ajustar Volume]
        end

        subgraph "Desenvolvimento"
            Debug[Modo Debug]
            Test[Modo Teste]
            Log[Visualizar Logs]
        end

        Player --> Start
        Player --> Move
        Player --> Pause
        Player --> Exit
        Player --> SelectDiff
        Player --> ToggleInfo
        Player --> AdjustVol

        Developer[Desenvolvedor]
        Developer --> Debug
        Developer --> Test
        Developer --> Log
    end
```

# Detalhamento dos Casos de Uso

## 1. A��es do Jogador

### 1.1 Iniciar Jogo
- **Ator**: Jogador
- **Descri��o**: Inicia uma nova partida
- **Fluxo Principal**:
  1. Selecionar dificuldade
  2. Pressionar Enter
  3. Jogo come�a no n�vel 1

### 1.2 Mover Cobra
- **Ator**: Jogador
- **Descri��o**: Controla dire��o da cobra
- **Controles**:
  - Seta para cima
  - Seta para baixo
  - Seta para esquerda
  - Seta para direita

### 1.3 Pausar Jogo
- **Ator**: Jogador
- **Descri��o**: Pausa/despausa o jogo
- **A��o**: Pressionar 'P'

### 1.4 Sair do Jogo
- **Ator**: Jogador
- **Descri��o**: Encerra o programa
- **Fluxo**:
  1. Pressionar 'S'
  2. Confirmar sa�da

## 2. Configura��es

### 2.1 Selecionar Dificuldade
- **Ator**: Jogador
- **Op��es**:
  - F�cil
  - M�dio
  - Dif�cil
- **Impacto**:
  - Velocidade inicial
  - Progress�o de dificuldade

### 2.2 Alternar Informa��es
- **Ator**: Jogador
- **Descri��o**: Mostra/oculta HUD
- **Informa��es**:
  - Pontua��o
  - N�vel
  - Vidas
  - Tamanho da cobra

### 2.3 Ajustar Volume
- **Ator**: Jogador
- **Controles**: +/-
- **Elementos**:
  - M�sica de fundo
  - Efeitos sonoros

## 3. Desenvolvimento

### 3.1 Modo Debug
- **Ator**: Desenvolvedor
- **Ativa��o**: `-show`
- **Funcionalidades**:
  - Informa��es em tempo real
  - Estado do jogo
  - Posi��es
  - Performance

### 3.2 Modo Teste
- **Ator**: Desenvolvedor
- **Op��es**:
  - `-maxlevel`: �ltimo n�vel
  - `-endscreen`: Tela final
- **Prop�sito**: Testes espec�ficos

### 3.3 Visualizar Logs
- **Ator**: Desenvolvedor
- **Ativa��o**: `-log`
- **Dados**:
  - Eventos do jogo
  - Estados
  - Erros
  - M�tricas

# Fluxos Alternativos

## 1. Game Over
```mermaid
graph TB
    A[Colis�o] --> B{Vidas Restantes?}
    B -->|Sim| C[Continuar]
    B -->|N�o| D[Game Over]
    D --> E[Menu Principal]
    E --> F[Nova Partida]
```

## 2. Progress�o de N�vel
```mermaid
graph TB
    A[Objetivo Completo] --> B{�ltimo N�vel?}
    B -->|N�o| C[Pr�ximo N�vel]
    B -->|Sim| D[Tela Final]
    D --> E[Menu Principal]
```

# Restri��es e Regras

1. **Movimento**
   - N�o pode reverter dire��o
   - Movimento cont�nuo
   - Velocidade aumenta por n�vel

2. **Colis�es**
   - Com paredes
   - Com pr�prio corpo
   - Com comida (positivo)

3. **Pontua��o**
   - Base por comida
   - Multiplicador por vidas
   - B�nus por tempo

4. **Progress�o**
   - 30 n�veis totais
   - Aumento gradual de dificuldade
   - Objetivos espec�ficos por n�vel
