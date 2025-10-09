# Snake Game OpenGL

## Descri��o do Projeto

Snake Game � uma implementa��o moderna do cl�ssico jogo da cobra usando OpenGL. Desenvolvido inicialmente como trabalho de TCC para a universidade UNIMAR em 2011, o jogo foi refatorado e melhorado ao longo do tempo.

## Documenta��o e Diagramas

- [Diagrama de Fluxo do Jogo](./diagrams/fluxo_jogo.md) - Fluxo de execu��o e estados do jogo
- [Diagrama de Estados](./diagrams/estados.md) - Estados e transi��es do jogo
- [Diagrama de Classes e Estruturas](./diagrams/estruturas.md) - Estrutura do c�digo e rela��es
- [Diagrama de Sequ�ncia](./diagrams/sequencia.md) - Intera��es entre componentes
- [Diagrama de Componentes](./diagrams/componentes.md) - Arquitetura do sistema
- [Diagrama de Casos de Uso](./diagrams/casos_uso.md) - Funcionalidades e intera��es

## Tecnologias Utilizadas

- C++ como linguagem principal
- OpenGL com GLUT para renderiza��o gr�fica
- FMOD Sound System para �udio
- Dev-C++ 5.11 como IDE de desenvolvimento

## Requisitos do Sistema

### Hardware
- Processador: qualquer x86 ou x64
- Mem�ria RAM: 512MB m�nimo
- Placa de v�deo: compat�vel com OpenGL 1.1+
- Espa�o em disco: 10MB

### Software
- Windows XP ou superior
- Dev-C++ 5.11 (para compila��o)
- OpenGL Runtime
- FMOD Sound System

## Instala��o e Configura��o

1. **Prepara��o do Ambiente**:
   ```bash
   # 1. Instale o Dev-C++ 5.11
   # Baixe de: https://sourceforge.net/projects/orwelldevcpp/

   # 2. Configure as bibliotecas
   # Copie os arquivos DLL para a pasta do projeto:
   - glut32.dll
   - fmod.dll
   ```

2. **Estrutura de Diret�rios**:
   ```
   snake-game/
   ??? Snake.cpp            # C�digo fonte principal
   ??? SnakeGame.dev        # Arquivo de projeto Dev-C++
   ??? Sons/                # Arquivos de �udio
   ??? inc/                 # Arquivos de cabe�alho (*.h)
   ?   ??? GL/              # Cabe�alhos OpenGL
   ??? lib/                 # Arquivos de bibliotecas est�ticas (.a)
   ??? Libraries/           # Bibliotecas externas
   ?   ??? glut-3.7/
   ?   ??? fmodapi375win/
   ??? README.md
   ```

## Compila��o

1. **Usando Dev-C++**:
   - Abra o arquivo `SnakeGame.dev`
   - Pressione F11 ou use o menu Executar > Compilar

2. **Via Linha de Comando**:
   ```bash
   g++ Snake.cpp -o SnakeGame.exe -lglut32 -lglu32 -lopengl32 -lfmod
   ```

## Execu��o

1. **Modo Normal**:
   ```bash
   ./SnakeGame.exe
   ```

2. **Modo Debug**:
   ```bash
   ./SnakeGame.exe -log -show
   ```

### Par�metros de Linha de Comando

- `-log`: Ativa o logging em arquivo
- `-show`: Mostra informa��es de debug na tela
- `-maxlevel`: Inicia no n�vel m�ximo
- `-endscreen`: Testa a tela de finaliza��o

## Controles do Jogo

- **Setas direcionais**: Controlam a dire��o da cobra
- **P**: Pausa o jogo
- **M**: Menu principal
- **S**: Sair do jogo
- **I**: Mostrar/ocultar informa��es
- **+/-**: Controle de volume

## Como Contribuir

1. Fork o projeto
2. Crie uma branch para sua feature
3. Commit suas mudan�as
4. Push para a branch
5. Abra um Pull Request

## Licen�a

Este projeto est� sob a licen�a MIT. Veja o arquivo LICENSE para mais detalhes.

## Autor

Pyetro S. F. da Costa
- Desenvolvido como TCC para UNIMAR (2011)
- Refatorado e melhorado posteriormente