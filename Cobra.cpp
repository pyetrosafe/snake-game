/******************************* Cobra GL v1.09 CPP *************************
 *                                                                          *
 *  1 Implementado GLPoint(cobra) se movimentando na tela                   *
 *  2 Implementado Teclas de movimentação Horizontal/Vertical               *
 *  3 Implementado Comida da cobra                                          *
 *  4 Implementado Funções de Apresentação de textos                        *
 *  5 Implementado Rotinas de Regras para cumprir objetivo da Fase          *
 *  6 Implementado Rotinas que determinam quando cobra morre ou come        *
 *  7 Melhorado Rotinas de inicialização                                    *
 *  8 Implementado Cronometros                                              *
 *  9 Implementado Sons                                                     *

 http://www.ilovewavs.com/Effects/Music/Music.htm
 http://www.secit.at/doc/fmod-3.74/html/HTML
****************************************************************************/

#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <winuser.h>
#include <fmod.h>

// Definicao de Constantes
#define HRES 480      // Resolucao Inicial da Window na Horizontal
#define VRES 320      // Resolucao Inicial da Window na Vertical
#define INC 10        // Incremento em pixel para X e Y da Cobra
#define rastro 48     // Define maximo de partes o corpo da cobra
#define ON 1          // define constante ON ativado
#define OFF 0         // define constante OFF desativado
#define lifes 5
#define MAXFASE 30
#define TEMPO_ESPERA1 5
#define TEMPO_ESPERA2 3

// Estrutura pra guardar pontos da cobra
struct cobra{
    float xf, yf;
};

// Declaracao de Variaveis Globais
GLfloat head[rastro][2];
GLfloat start_time, end_time, clock_time, pause_time, paused_time, showsound_time=0;
GLfloat Xfood, Yfood;
GLfloat moveToX, moveToY; // Indica se a cobra vai andar horizontal ou vertical
GLfloat Vkey, Hkey; // Fator que define a direção da cobra
struct cobra food;
long seed, ScorePoints;
int StatusGame, Stats, sizefont, alive, FASE, showVol=OFF, infos=ON;
int pisca=0, contrastro=0, nivel=0, DELAY;
int winXmin=-(HRES/2), winXmax=(HRES/2);  // Tamanho minimo e maximo em X da Janela
int winYmin=-(VRES/2), winYmax=(VRES/2);  // Tamanho minimo e maximo em Y da Janela
int incX, incY, Pause=0;  // Fatores de increment em X e Y para movimentar a Cobra
// unsigned char teclas; // Usado para mostrar tecla do teclado que foi pressionada

FSOUND_SAMPLE* somComeu;  // Som quando come
FSOUND_STREAM* somMenu;  // Som Inicial
FSOUND_SAMPLE* somChoque;  // Som Choque
FSOUND_SAMPLE* somParabens;  // Som Ganhou
FSOUND_SAMPLE* somPerdeu;  // Som Perdeu

//  Função que recebe como parametros: font, texto, coord X e Y e cores em RGB
//  Exibe na tela usando caracteres bitmap
//void DesenhaTexto(void *font, char *string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) { // modo antigo
void DesenhaTexto(char *string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) {

    glColor3f(red, green, blue);
    glRasterPos2f(x,y); 

    //  Exibe caractere a caractere
    while(*string) {
      if(sizefont==10)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++);
      if(sizefont==20)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
    }
} // Fim DesenhaTexto
/*
void mostraTeclas() {
    char msg[20];
    sprintf(msg,"%c",teclas);
    DesenhaTexto(msg, -(HRES/2)+40,(VRES/2)-20);
}
*/
int getRandom(int min,int max) {
    int r;

    if(!seed) {
      time(&seed);  // Pega a hora atual
      srand((unsigned)seed);  // utiliza a hora como semente do gerador de nros.
    }
    r=min+rand()%(max-min+1);
    if(r%10!=0) {
      r/=10;
      r*=10;
    }
    return r;
}// Fim getRandom

// Função Debug - Mostrar informações para o jogador e das variaveis na tela para analise (quando sendo desenvolvido)
void Debug(){
    char msg[120];
    //  Posição no universo onde o texto bitmap será colocado
    sizefont=10;
    if ((StatusGame == ON) && (infos==ON)) {
      sprintf(msg,"LEVEL: %i",FASE);
      DesenhaTexto(msg, -(HRES/2)+40,(VRES/2)-20);
      sprintf(msg,"Pontos: %d",ScorePoints);
      DesenhaTexto(msg, -(HRES/2)+40,(VRES/2)-30);
      sprintf(msg,"Vidas: %i",alive);
      DesenhaTexto(msg, (HRES/2)-90,(VRES/2)-20);
      sprintf(msg,"Tamanho: %i ",contrastro);
      DesenhaTexto(msg, (HRES/2)-90,(VRES/2)-30);
      sprintf(msg,"Tempo: %.0f",((clock()-clock_time)/CLOCKS_PER_SEC)-paused_time);
      DesenhaTexto(msg, (HRES/2)-90,(VRES/2)-40);
/*    sprintf(msg,"Head X: %.0f , Y: %.0f",head[0][0], head[0][1]);
      DesenhaTexto(msg, (HRES/2)-110,(VRES/2)-60);
      sprintf(msg,"Food - X = %1.f , Y = %1.f",food.xf, food.yf);
      DesenhaTexto(msg, -290,230);
      sprintf(msg,"STATS = %i,    StatusGame = %i",Stats, StatusGame);
      DesenhaTexto(msg, -290,220);
      sprintf(msg,"wXx/3: %i  wXx: %i",(winXmax/3),winXmax-10);
      DesenhaTexto(msg, (HRES/2)-110,(VRES/2)-70); */
    }
}

void MostraVolume() {
    char msg[120];
    float sound_time = ((clock()-showsound_time)/CLOCKS_PER_SEC);
    if ((sound_time <= 3) && (showVol==ON)) {
      sizefont=10;
      sprintf(msg,"Volume: %i%%", (FSOUND_GetSFXMasterVolume()*100/255));
      DesenhaTexto(msg, -35,(VRES/2)-20);
    } else {
      showsound_time=0;
      showVol=OFF;
    }
}

// Desenha as barras
void DesenhaTela() {

    // Fundo Tela Grande
    //glColor3f(.1f, .1f, .1f); //  Cor de desenho branco
    glColor3f(.0f, .15f, .2f); //  Cor de desenho branco

    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmin);
    glEnd();

    // Barras Laterais
    glColor3f(1.0f, 1.0f, 1.0f); //  Cor de desenho branco

    // Barra horizontal superior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmax-5);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmax-5);
    glEnd();

    // Barra horizontal inferior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmin+5);
    glVertex2f(winXmax, winYmin+5);
    glVertex2f(winXmax, winYmin);
    glEnd();

    // Barra vertical esquerda
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmin+5, winYmax);
    glVertex2f(winXmin+5, winYmin);
    glEnd();

    // Barra vertical direita
    glBegin(GL_QUADS);
    glVertex2f(winXmax-5, winYmin);
    glVertex2f(winXmax-5, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmin);
    glEnd();

/*  //  Linhas abaixo para analise de correção de valores de posição da cobra e dos textos
    int i= -(HRES/2);
    while(i < HRES/2) {
    glBegin(GL_LINES);
    if (i==0)
    glColor3f(1,1,1);
    else if ((i > -90) && (i < 90))
    glColor3f(1,1,0);
    else
    glColor3f(1,0,0);
    glVertex3f(i,-(VRES/2),0);
    glVertex3f(i,VRES,0);
    glEnd();
    i += 10;
    }
    // int i;
    i = (((HRES/2)-(HRES/3))/10)*10;
    glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex2f(i,-VRES/2);
      glVertex2f(i,VRES/2);

      glVertex2f(-i,-VRES/2);
      glVertex2f(-i,VRES/2);

    i = (((VRES/2)-(VRES/3))/10)*10;
      glVertex2f(-HRES/2,i);
      glVertex2f(HRES/2,i);

      glVertex2f(-HRES/2,-i);
      glVertex2f(HRES/2,-i);
    glEnd();
*/

}// Fim DesenhaTela()

// void GAMEON(){
void GameScreen() {
    int j;
    char msg[120];

    if(StatusGame == ON) {

      // Stats = 0;
      // Desenha Cobra (HEAD)
      glColor3f(1.0f, 0.4f, 0.4f);
      glPointSize(9.0f);
      glBegin(GL_POINTS);
      glVertex2f(head[0][0],head[0][1]);
      glEnd();

      // Desenha a comida
      glColor3f(1.0f, 1.0f, 0.7f);
      if(pisca%10!=0) {
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(Xfood,Yfood);
        glEnd();
      }
      if(pisca>=99)
        pisca=0;

      // Desenha o corpo da cobra
      if(contrastro>0) {
        for(j=1;j<contrastro+1;j++) {
          glColor3f(1.0f, 0.7f, 0.7f);
          glPointSize(9.0f);
          glBegin(GL_POINTS);
          glVertex2f(head[j][0],head[j][1]);
          glEnd();
        }
      }
    }

    // Mensagens na tela!!!
    sizefont=20;

    // Stats 1 - Menu Principal
    if(Stats==1) {
      sprintf(msg,"\\_/\\_Cobra Game_/\\_/");
      DesenhaTexto(msg, -110,VRES/3,1.0f,0.4f,0.4f);

      // Desenha flecha nivel
      sprintf(msg,">>");
      if(nivel == 0) {
        DesenhaTexto(msg, -80,(VRES/5)+10,0.3f,1.0f,0.5f);
      } else if (nivel == 1) {
        DesenhaTexto(msg, -80,(VRES/5)-15,0.3f,1.0f,0.5f);
      } else {
        DesenhaTexto(msg, -80,(VRES/5)-40,0.3f,1.0f,0.5f);
      }

      sprintf(msg," Facil");
      DesenhaTexto(msg, -50,(VRES/5)+10);
      sprintf(msg," Medio ");
      DesenhaTexto(msg, -50,(VRES/5)-15);
      sprintf(msg," Dificil ");
      DesenhaTexto(msg, -50,(VRES/5)-40);

      sprintf(msg,"Comandos:");
      DesenhaTexto(msg, -60,-10);

      sprintf(msg,"'M'");
      DesenhaTexto(msg, -100,-40);
      sprintf(msg,"= Menu Principal");
      DesenhaTexto(msg, -50,-40);

      sprintf(msg,"'I'");
      DesenhaTexto(msg, -100,-60);
      sprintf(msg,"= Mostrar Informacoes");
      DesenhaTexto(msg, -50,-60);

      sprintf(msg,"'P'");
      DesenhaTexto(msg, -100,-80);
      sprintf(msg,"= Pause");
      DesenhaTexto(msg, -50,-80);

      sprintf(msg,"'S'");
      DesenhaTexto(msg, -100,-100);
      sprintf(msg,"= Sair");
      DesenhaTexto(msg, -50,-100);

      sprintf(msg,"'+/-'");
      DesenhaTexto(msg, -100,-120);
      sprintf(msg,"= Volume");
      DesenhaTexto(msg, -50,-120);
    }// Fim Stats == 1

    // Stats 2 - Tela Sair
    if(Stats==2){
      sprintf(msg,"Para Sair do jogo pressione 'S'");
      DesenhaTexto(msg, -150,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(msg, -220,(VRES/4)-40);
    }// Fim Stats == 2

    // Stats 3 - Tela Pausa
    if(Stats==3){
      sprintf(msg,"PAUSADO");
      DesenhaTexto(msg, -60,(VRES/4));
      sprintf(msg,"Para voltar pressione 'P'");
      DesenhaTexto(msg, -110,(VRES/4)-40);
    }// Fim Stats == 3

    // Stats 4 - Tela Nivel
    if(Stats==4){
      sprintf(msg,"LEVEL %i",FASE);
      DesenhaTexto(msg, -40,(VRES/4));
    }// Fim Stats == 4

    // Stats 5 - Tela de Choque
    if(Stats==5) {
      sprintf(msg,"Restam: %i vidas",alive);
      DesenhaTexto(msg, -75,(VRES/4));
      sprintf(msg,"%0.f",(TEMPO_ESPERA1-(start_time-end_time)/CLOCKS_PER_SEC));
      DesenhaTexto(msg, -10,(VRES/4)-40);
    }// Fim Stats == 5

    // Stats 6 - Game Over
    if(Stats==6) {
      sprintf(msg,"VOCE PERDEU");
      DesenhaTexto(msg, -70,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(msg, -220,50-(VRES/4));
      sprintf(msg,"Para Sair do jogo Pressione 'S'");
      DesenhaTexto(msg, -150,20-(VRES/4));
    } // Fim Stats == 6

    // Stats 7 - Jogo Finalizado
    if(Stats==7) {
      sprintf(msg,"PARABENS, Voce Finalizou o Jogo!");
      DesenhaTexto(msg, -170,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(msg, -220,50-(VRES/4));
      sprintf(msg,"Para Sair do jogo Pressione 'S'");
      DesenhaTexto(msg, -150,20-(VRES/4));
    } // Fim Stats == 7
}// Fim GAME OFF

// Desenha(): funcao chamada sempre que for preciso redesenhar a tela
void Desenha(void) {

    // Muda para o sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa a matriz de transformação corrente
    glLoadIdentity();

    // Limpa janela de visualizacao
    //glClearColor(0.0f,0.15,0.2f,0.0f); // especifica a cor de fundo
    glClearColor(.1f,0.1,0.1f,0.0f); // especifica a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT); // pinta o fundo com a cor acima 

    // Desenha barras e fundo pra tela grande
    DesenhaTela();
    //mostraTeclas();

    // Chama Visualização do Debug (informações)
    Debug();
    // Chama Visualização do volume do áudio
    MostraVolume();

    // Desenha de acordo com status do jogo
    GameScreen();
    if(StatusGame==ON)
      Stats=0;

    // Executa os comandos OpenGL
    glutSwapBuffers();
}// Fim Desenha()

// Fecha o jogo
void TerminaGame(int chama=0){
    int j;

    fflush(stdin);
    StatusGame = OFF;
    incX = incY = INC;
    contrastro = 0;
    // contrastro = 45; // Testar configurações de ultimo level
    end_time = start_time = clock();

    if(chama==0) {
      nivel=0;
      Stats = FASE = 1;
      ScorePoints = 0;
      // FASE = 30; DELAY-30; // Testar configurações de ultimo level
      alive = lifes;
      FSOUND_StopSound(FSOUND_ALL);
      FSOUND_Stream_Play(0,somMenu);
    } else if(chama==1) {
      Stats = 5;
    } else if(chama==2) {
      Stats = 6;
    } else if(chama==3) {
      Stats = 4;
      FASE++;
      DELAY--;
    } else if(chama==4) {
      Stats = 7;
    }

    // Cobra começa nos cantos da tela
    head[0][0] = getRandom((winXmax/3)+10,winXmax-10);
    head[0][1] = getRandom((winYmax/3)+10,winYmax-10);
    food.xf = Xfood = getRandom(winXmin+10,winXmax-10);
    food.yf = Yfood = getRandom(winYmin+10,winYmax-10);
    // Define qual canto nascer!
    j = getRandom(-HRES,HRES);
    if(j<0)
      j = -1;
    else
      j = 1;
    head[0][0] *= j;
    j = getRandom(-HRES,HRES);
    if(j<0)
      j = -1;
    else
      j = 1;
    head[0][1] *= j;

    if(head[0][0] > 0)
        Hkey=1;
    else
      Hkey=0;

    if(head[0][1] > 0)
      Vkey=1;
    else
      Vkey=0;

    j = getRandom(-HRES,HRES);
    if (j<0) {
      moveToX = 1;
      moveToY = 0;
    } else {
      moveToX = 0;
      moveToY = 1;
    }

    // Rastros recomeçam com X Y da Head[0]
    for(j=1;j<contrastro;j++) {
      head[j][0]=head[j-1][0]; // Coord x
      head[j][1]=head[j-1][1]; // Coord y
    }
}

// Função callback chamada pela GLUT a cada intervalo de tempo
void Engine(int value) {
    int j, g;
    float t=0;
    // Incrementa variavel pra piscar a comida!
    pisca++;

    // Atualiza o tempo jogado e de espera
    start_time = clock();
    t  = ((start_time-end_time)/CLOCKS_PER_SEC);

    if(StatusGame==OFF && (Stats==4 || Stats==5)) {// Contador para tela 4 e 5(Level e Choque)
      FSOUND_Stream_Stop(somMenu); // Para a execução da musica inicial (id 2)
      if ( (Stats==5 && (t >= TEMPO_ESPERA1-1)) || (Stats==4 && (t >= TEMPO_ESPERA2-1)) ) {
        StatusGame=ON;
      // Atualiza o tempo jogado e de espera
      clock_time = clock();
      pause_time = paused_time = 0;
      }
    }

    if(StatusGame==ON) {

      // Calculo que determina tamanho do corpo a atingir no level
      if(FASE<=10)
        g=FASE*2-2;
      else
        g=FASE*2-(FASE-8);
      g+=10;

      // Verifica se a bola bateu nas barras
      if((head[0][0] >= winXmax-5) || (head[0][0] <= winXmin+5) || (head[0][1] >= winYmax-5) || (head[0][1] <= winYmin+5)) {
        alive--;
          if (ScorePoints > (alive*400)*g)
            ScorePoints -= (alive*400)*g;
          else
            ScorePoints = 0;
        if(alive<=0) {
          FSOUND_PlaySound (5,somPerdeu);
          TerminaGame(2); // Tela GameOver
        } else {
          FSOUND_PlaySound (3,somChoque);
          TerminaGame(1); // Tela Choque
        }
      }
      // Perde se bater no próprio corpo
      for(j=1;j<contrastro+1;j++) {
        //if((head[0][0]==head[j][0])&&(head[0][1]==head[j][1])&&(((clock()-clock_time)/CLOCKS_PER_SEC)>1)) { // Usado para testar ultimo level sem bugar a vida !
        if((head[0][0]==head[j][0])&&(head[0][1]==head[j][1])) {
          alive--;
          if (ScorePoints > (alive*400)*g)
            ScorePoints -= (alive*400)*g;
          else
            ScorePoints = 0;
          if(alive<=0) {
            FSOUND_PlaySound (5,somPerdeu);
            TerminaGame(2); // Tela GameOver
          } else {
            FSOUND_PlaySound (3,somChoque);
            TerminaGame(1); // Tela Choque
          }
        }
      }
      // Verifica se Head relou na comida e incrementa o corpo
      if((head[0][0]==food.xf)&&(head[0][1]==food.yf)) {
        food.xf=Xfood=getRandom(winXmin+10,winXmax-10);
        food.yf=Yfood=getRandom(winYmin+10,winYmax-10);

        FSOUND_PlaySound (1,somComeu);
        if ((clock()-clock_time)/CLOCKS_PER_SEC<90) {
          ScorePoints += (alive*400);
        } else if ((clock()-clock_time)/CLOCKS_PER_SEC<180) {
          ScorePoints += (alive*300);
        } else
          ScorePoints += (alive*150);

        if(contrastro<rastro)
          contrastro++;
      }
      // Guarda coordenadas do corpo da cobra
      for(j=contrastro;j>0;j--) {
        head[j][0]=head[j-1][0]; // Coord x
        head[j][1]=head[j-1][1]; // Coord y
      }
      // Move a cobra (Head[0])
      if(moveToX == 1) {
        if(Hkey == 0)
          head[0][0] += incX;
        if(Hkey == 1)
          head[0][0] -= incX;
      }
      if(moveToY == 1) {
        if(Vkey == 0)
          head[0][1] += incY;
        if(Vkey == 1)
          head[0][1] -= incY;
      }
//      FSOUND_PlaySound (0,somAndou); // Som da cobra andando

      // Muda Level (FASE) se objetivo, rastros, estão completos !
      // Verifica se tamanho do corpo igual tamanho a ser completado
      if((contrastro==g) && (FASE<MAXFASE))// (contrastro<rastro))
        TerminaGame(3);
      else if((contrastro==g) && (FASE=MAXFASE)) {// (contrastro==rastro)
        TerminaGame(4);
        FSOUND_PlaySound (4,somParabens);
      }
    } // Encerra IF StatusGame

    // Redesenha a Bola na nova posição
    glutPostRedisplay();
    glutTimerFunc(DELAY, Engine, 1);
}// fim Engine

// Teclado(): funcao chamada sempre que for pressionada uma tecla ASCII
void Teclado(unsigned char tecla, int x, int y){
    if(tecla == 13) {
      if(StatusGame==OFF && Stats==1) {
        if(nivel==0)
          DELAY = 100;
        else if(nivel==1)
          DELAY = 80;
        else
          DELAY = 60;
        Stats=4;
        end_time = start_time;
      }
    }

    if(tecla == 'm' || tecla == 'M')
      TerminaGame(0);

    if(tecla == 's' || tecla == 'S') {
      if(StatusGame==OFF && Stats==2)
        exit(0);
      else if(StatusGame==OFF && (Stats==1 || Stats==6 || Stats==7))
        Stats=2;
    }

    if(tecla == 'p' || tecla == 'P') {
      if(Pause==0 and StatusGame==ON) {
        StatusGame=OFF;
        Stats=3;
        Pause=1;
        pause_time = (clock()-clock_time)/CLOCKS_PER_SEC;
      } else if(Pause==1 and StatusGame==OFF) {
        Stats=0;
        StatusGame=ON;
        Pause=0;
        pause_time = ((clock()-clock_time)/CLOCKS_PER_SEC)-pause_time;
        paused_time += pause_time;
      }
    }

    if(tecla == 'i' || tecla == 'I') {
      if(infos==ON)
        infos=OFF;
      else
        infos=ON;
    }

    if (tecla == '+' || tecla == '=') {
      if (FSOUND_GetSFXMasterVolume() > 250)
        FSOUND_SetSFXMasterVolume(256);
      else
        FSOUND_SetSFXMasterVolume(FSOUND_GetSFXMasterVolume()+6);
      showsound_time = clock();
      showVol=ON;
    }

    if (tecla == '-') {
      if (FSOUND_GetSFXMasterVolume() < 6)
        FSOUND_SetSFXMasterVolume(0);
      else
        FSOUND_SetSFXMasterVolume(FSOUND_GetSFXMasterVolume()-6);
      showsound_time = clock();
      showVol=ON;
    }

} // Fim Teclado()

// Funcao para coordenadar a cobra
void TeclasEspeciais(int tecla, int x, int y) {
    switch(tecla) {
    case GLUT_KEY_UP : // seta para cima
      if(moveToX==1 && StatusGame==ON) {
        moveToX = 0;
        moveToY = 1;
        Vkey = 0;
      }
      if(StatusGame==OFF && Stats==1) {
        if(nivel>0)
          nivel--;
        else
          nivel = 2;
      }
      break;
    case GLUT_KEY_DOWN: // seta para cima
      if (moveToX==1 && StatusGame==ON) {
        moveToX = 0;
        moveToY = 1;
        Vkey = 1;
      }
      if(StatusGame==OFF && Stats==1) {
        if(nivel<2)
          nivel++;
        else
          nivel = 0;
      }
      break;
    case GLUT_KEY_LEFT : // seta para esquerda
      if (moveToY==1 && StatusGame==ON) {
        moveToX = 1;
        moveToY = 0;
        Hkey = 1;
      }
      break;
    case GLUT_KEY_RIGHT: // seta para direita
      if (moveToY==1 && StatusGame==ON) {
        moveToX = 1;
        moveToY = 0;
        Hkey = 0;
      }
      break;
    }
}// End Teclas Especiais

// AlteraTamanhoJanela(): funcao callback para redesenhar a janela
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    GLsizei x, y;
    // Evita a divisao por zero
    if(h == 0)
      h = 1;

    // Dimensoes da Viewport
    if (w < HRES)
      glutReshapeWindow(HRES,VRES);
    if (h < VRES)
      glutReshapeWindow(HRES,VRES);

    // glViewport(x,y,HRES,VRES);
    glViewport(0,0,w,h);

    // Inicia o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-w/2,w/2,-h/2,h/2);
    glMatrixMode(GL_MODELVIEW);
} // Fim AlteraTamanhoJanela()

// Inicializa(): Funcao que inicializa o ambiente de trabalho
void Inicializa() {
    HWND hWnd=FindWindow(TEXT("GLUT"), TEXT("Cobra Game"));
    LONG gwl=GetWindowLong(hWnd, GWL_STYLE);
    gwl&=(~WS_MAXIMIZEBOX)&(~WS_THICKFRAME);
    // SetWindowLong(hWnd, GWL_STYLE, gwl);

    // Inicializa sistema de som da FMOD
    FSOUND_Init(44100, 32, 0);
   //Carrega os arquivos com os sons
//    somAndou=FSOUND_Sample_Load (0,"Sons/SomAndou3.wav",0, 0, 0);
//    FSOUND_Sample_SetMode(somAndou, FSOUND_LOOP_OFF);  

    somComeu=FSOUND_Sample_Load (1,"Sons/SomComeu.wav",0, 0, 0);
    FSOUND_Sample_SetMode(somComeu, FSOUND_LOOP_OFF);

    somMenu=FSOUND_Stream_Open ("Sons/SomMenu.mp3",0, 0, 0);
    FSOUND_Stream_SetMode(somMenu, FSOUND_LOOP_NORMAL);

    somChoque=FSOUND_Sample_Load (3,"Sons/SomChoque2.wav",0, 0, 0);
    FSOUND_Sample_SetMode(somChoque, FSOUND_LOOP_OFF);  

    somParabens=FSOUND_Sample_Load (4,"Sons/SomParabens1.mp3",0, 0, 0);
    FSOUND_Sample_SetMode(somParabens, FSOUND_LOOP_OFF);

    somPerdeu=FSOUND_Sample_Load (5,"Sons/SomPerdeu.wav",0, 0, 0);
    FSOUND_Sample_SetMode(somPerdeu, FSOUND_LOOP_OFF);

    FSOUND_SetSFXMasterVolume(52);

    // Chama função primaria
    TerminaGame();
    // glutFullScreen();
} // Fim Inicializa()

// Programa Principal 
int main(void) {
    size_t width = (size_t)GetSystemMetrics(SM_CXSCREEN);
    size_t height = (size_t)GetSystemMetrics(SM_CYSCREEN);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);                    // Modo exibicao da GLUT    
    glutInitWindowSize(HRES,VRES);                                  // Tamanho inicial em pixel da janela Glut
    glutInitWindowPosition((width/2)-(HRES/2),(height/2)-(VRES/2)); // Posicao inicial da janela Glut na Tela
    glutCreateWindow("Cobra Game");                                 // Cria a janela passando o Titulo
    glutDisplayFunc(Desenha);                                       // Registra Funcao de desenho da janela
    glutReshapeFunc(AlteraTamanhoJanela);                           // Registra funcao decredimensionamento
    glutKeyboardFunc(Teclado);                                      // Registra Funcao para tratamento das teclas
    glutSpecialFunc(TeclasEspeciais);                               // Registra a função que trata teclas especiais
    glutTimerFunc(50, Engine, 1);                                   // Registra função que será chamada a cada intervalo de tempo
    Inicializa();                                                   // Faz as inicializacoes necessarias 
    glutMainLoop();                                                 // Inicia processamento e aguarda interacoes do usuario
    return 0;
}// Fim main()
