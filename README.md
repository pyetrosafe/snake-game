# Snake Game OpenGL

## Descrição do Projeto

Snake Game é uma implementação moderna do clássico jogo da cobra usando OpenGL. Desenvolvido inicialmente como trabalho de TCC para a universidade UNIMAR em 2011, o jogo foi refatorado e melhorado ao longo do tempo.

## Documentação e Diagramas

- [Diagrama de Fluxo do Jogo](./diagrams/fluxo_jogo.md) - Fluxo de execução e estados do jogo
- [Diagrama de Estados](./diagrams/estados.md) - Estados e transições do jogo
- [Diagrama de Classes e Estruturas](./diagrams/estruturas.md) - Estrutura do código e relações
- [Diagrama de Sequência](./diagrams/sequencia.md) - Interações entre componentes
- [Diagrama de Componentes](./diagrams/componentes.md) - Arquitetura do sistema
- [Diagrama de Casos de Uso](./diagrams/casos_uso.md) - Funcionalidades e interações

## Tecnologias Utilizadas

- C++ como linguagem principal
- OpenGL com GLUT para renderização gráfica
- FMOD Sound System para áudio
- Dev-C++ 5.11 como IDE de desenvolvimento

## Requisitos do Sistema

### Hardware
- Processador: qualquer x86 ou x64
- Memória RAM: 512MB mínimo
- Placa de vídeo: compatível com OpenGL 1.1+
- Espaço em disco: 10MB

### Software
- Windows XP ou superior
- Dev-C++ 5.11 (para compilação)
- OpenGL Runtime
- FMOD Sound System

## Instalação e Configuração

1. **Preparação do Ambiente**:
   ```bash
   # 1. Instale o Dev-C++ 5.11
   # Baixe de: https://sourceforge.net/projects/orwelldevcpp/

   # 2. Configure as bibliotecas
   # Copie os arquivos DLL para a pasta do projeto:
   - glut32.dll
   - fmod.dll
   ```

2. **Estrutura de Diretórios**:
   ```
   snake-game/
   ??? Snake.cpp            # Código fonte principal
   ??? SnakeGame.dev        # Arquivo de projeto Dev-C++
   ??? Sons/                # Arquivos de áudio
   ??? inc/                 # Arquivos de cabeçalho (*.h)
   ?   ??? GL/              # Cabeçalhos OpenGL
   ??? lib/                 # Arquivos de bibliotecas estáticas (.a)
   ??? Libraries/           # Bibliotecas externas
   ?   ??? glut-3.7/
   ?   ??? fmodapi375win/
   ??? README.md
   ```

## Compilação

1. **Usando Dev-C++**:
   - Abra o arquivo `SnakeGame.dev`
   - Pressione F11 ou use o menu Executar > Compilar

2. **Via Linha de Comando**:
   ```bash
   g++ Snake.cpp -o SnakeGame.exe -lglut32 -lglu32 -lopengl32 -lfmod
   ```

## Execução

1. **Modo Normal**:
   ```bash
   ./SnakeGame.exe
   ```

2. **Modo Debug**:
   ```bash
   ./SnakeGame.exe -log -show
   ```

### Parâmetros de Linha de Comando

- `-log`: Ativa o logging em arquivo
- `-show`: Mostra informações de debug na tela
- `-maxlevel`: Inicia no nível máximo
- `-endscreen`: Testa a tela de finalização

## Controles do Jogo

- **Setas direcionais**: Controlam a direção da cobra
- **P**: Pausa o jogo
- **M**: Menu principal
- **S**: Sair do jogo
- **I**: Mostrar/ocultar informações
- **+/-**: Controle de volume

## Como Contribuir

1. Fork o projeto
2. Crie uma branch para sua feature
3. Commit suas mudanças
4. Push para a branch
5. Abra um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

## Autor

Pyetro S. F. da Costa
- Desenvolvido como TCC para UNIMAR (2011)
- Refatorado e melhorado posteriormente