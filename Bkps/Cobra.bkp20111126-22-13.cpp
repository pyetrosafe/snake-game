/******************************* Cobra GLv1.CPP *****************************
 *    *
 *  1 Implementa a bolinha(cobra) se movimentando na tela    *
 *  2 Implementa Teclas de movimentação    *
 ****************************************************************************/

#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <winuser.h>

//Definicao de Constantes
//#define HRES 800    //Resolucao Inicial da Window na Horizontal
//#define VRES 600    //Resolucao Inicial da Window na Vertical
#define HRES 480      //Resolucao Inicial da Window na Horizontal
#define VRES 320      //Resolucao Inicial da Window na Vertical
#define INC 10        //Incremento em pixel para X e Y da Cobra
#define rastro 48     //Define maximo de partes o corpo da cobra
#define ON 1          //define constante ON ativado
#define OFF 0         //define constante OFF desativado
#define lifes 5
#define MAXFASE 30
#define TEMPO_ESPERA1 5
#define TEMPO_ESPERA2 3

//Estrutura pra guardar pontos da cobra
struct cobra{
    float xf, yf;
};

//Declaracao de Variaveis Globais
GLfloat head[rastro][2];
GLfloat start_time, end_time, clock_time;
GLfloat Xfood, Yfood;
GLfloat moveToX, moveToY; //Indica se a cobra vai andar horizontal ou vertical
GLfloat Vkey, Hkey; //Fator que define a direção da cobra
struct cobra food;
long seed=0, counter=0;
char niveltxt[100];
int StatusGame, Stats, sizefont, getout=0, alive, FASE, infos=ON;
int i=0, pisca=0, contrastro=0, nivel=0, DELAY;
int winXmin=-(HRES/2), winXmax=(HRES/2);  //Tamanho minimo e maximo em X da Janela
int winYmin=-(VRES/2), winYmax=(VRES/2);  //Tamanho minimo e maximo em Y da Janela
int incX, incY, Pause=0;  //Fatores de increment em X e Y para movimentar a Cobra

// Função que recebe como parametros: font, texto, coord X e Y e cores em RGB
// Exibe na tela usando caracteres bitmap
void DesenhaTexto(void *font, char *string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) {

    glColor3f(red, green, blue);
    glRasterPos2f(x,y); 

    // Exibe caractere a caractere
    while(*string) {
      if(sizefont==10)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++);
      if(sizefont==20)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
    }
} //Fim DesenhaTexto

int getRandom(int min,int max) {
    int r;

    if(!seed) {
      time(&seed);  //Pega a hora atual
      srand((unsigned)seed);  //utiliza a hora como semente do gerador de nros.
    }
    r=min+rand()%(max-min+1);
    if(r%10!=0) {
      r/=10;
      r*=10;
    }
    return r;
}//Fim getRandom


//Função Debug - Mostrar informações para o jogador e das variaveis na tela para analise (quando sendo desenvolvido)
void Debug(){
    char msg[120];
    // Posição no universo onde o texto bitmap será colocado
    sizefont=10;
    if (StatusGame == ON) {
      sprintf(msg,"LEVEL: %i",FASE);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-20);
      sprintf(msg,"Vidas: %i",alive);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-30);
      sprintf(msg,"Tamanho: %i ",contrastro);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-40);
      sprintf(msg,"Tempo: %.0f",(clock()-clock_time)/CLOCKS_PER_SEC);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-50);
/*
      sprintf(msg,"Head X: %.0f , Y: %.0f",head[0][0], head[0][1]);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-60);
      sprintf(msg,"Food - X = %1.f , Y = %1.f",food.xf, food.yf);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-290,230);
      sprintf(msg,"STATS = %i,    StatusGame = %i",Stats, StatusGame);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-290,220);
      sprintf(msg,"wXx/3: %i  wXx: %i",(winXmax/3),winXmax-10);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,(HRES/2)-110,(VRES/2)-70);
*/
    }
}

//Desenha as barras
void DesenhaTela() {
    //Barra horizontal superior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmax-5);
    glVertex2f(winXmin, winYmax-5);
    glEnd();

    //Barra horizontal inferior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmax, winYmin);
    glVertex2f(winXmax, winYmin+5);
    glVertex2f(winXmin, winYmin+5);
    glEnd();

    //Barra vertical esquerda
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmin+5, winYmax);
    glVertex2f(winXmin+5, winYmin);
    glVertex2f(winXmin, winYmin);
    glEnd();

    //Barra vertical direita
    glBegin(GL_QUADS);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax-5, winYmax);
    glVertex2f(winXmax-5, winYmin);
    glVertex2f(winXmax, winYmin);
    glEnd();
/*  // Linhas abaixo para analise de correção de valores de posição da cobra e dos textos
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
    //int i;
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

}//Fim DesenhaTela()

//void GAMEON(){
void GameScreen() {
    int j;
    char msg[120];

    if(StatusGame == ON) {
      //Stats = 0;
      //Desenha Cobra (HEAD)
      glColor3f(1.0f, 0.4f, 0.4f);
      glPointSize(9.0f);
      glBegin(GL_POINTS);
      glVertex2f(head[0][0],head[0][1]);
      glEnd();

      //Desenha a comida
      glColor3f(1.0f, 1.0f, 0.7f);
      if(pisca%10!=0) {
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(Xfood,Yfood);
        glEnd();
      }
      if(pisca>=99)
        pisca=0;

      //Desenha o corpo da cobra
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
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-110,VRES/3,1.0f,0.4f,0.4f);

      // Desenha flecha nivel
      sprintf(msg,">>");
      if(nivel == 0) {
        DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-80,(VRES/5)+10,0.3f,1.0f,0.5f);
      } else if (nivel == 1) {
        DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-80,(VRES/5)-15,0.3f,1.0f,0.5f);
      } else {
        DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-80,(VRES/5)-40,0.3f,1.0f,0.5f);
      }

      sprintf(msg," Facil");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-50,(VRES/5)+10);
      sprintf(msg," Medio ");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-50,(VRES/5)-15);
      sprintf(msg," Dificil ");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-50,(VRES/5)-40);

      sprintf(msg,"Comandos:");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,-10);

      sprintf(msg,"'M'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-40);
      sprintf(msg,"= Menu Principal");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,-40);

      sprintf(msg,"'I'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-60);
      sprintf(msg,"= Mostrar Informacoes");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,-60);

      sprintf(msg,"'P'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-80);
      sprintf(msg,"= Pause");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,-80);

      sprintf(msg,"'S'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-100);
      sprintf(msg,"= Sair");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,-100);
    }//Fim Stats == 1

    // Stats 2 - Tela Sair
    if(Stats==2){
      sprintf(msg,"Para Sair do jogo pressione 'S'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-150,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-220,(VRES/4)-40);
    }//Fim Stats == 2

    // Stats 3 - Tela Pausa
    if(Stats==3){
      sprintf(msg,"PAUSADO");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,(VRES/4));
      sprintf(msg,"Para voltar pressione 'P'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-110,(VRES/4)-40);
    }//Fim Stats == 3

    // Stats 4 - Tela Nivel
    if(Stats==4){
      sprintf(msg,"LEVEL %i",FASE);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-40,(VRES/4));
    }//Fim Stats == 4

    // Stats 5 - Tela de Choque
    if(Stats==5) {
      sprintf(msg,"Restam: %i vidas",alive);
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-75,(VRES/4));
      sprintf(msg,"%0.f",(TEMPO_ESPERA1-(start_time-end_time)/CLOCKS_PER_SEC));
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-10,(VRES/4)-40);
    }//Fim Stats == 5

    // Stats 6 - Game Over
    if(Stats==6) {
      sprintf(msg,"VOCE PERDEU");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-70,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-220,50-(VRES/4));
      sprintf(msg,"Para Sair do jogo Pressione 'S'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-150,20-(VRES/4));
    } //Fim Stats == 6

    // Stats 7 - Jogo Finalizado
    if(Stats==7) {
      sprintf(msg,"PARABENS, Voce Finalizou o Jogo!");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-170,(VRES/4));
      sprintf(msg,"Para voltar ao Menu Principal Pressione 'M'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-220,50-(VRES/4));
      sprintf(msg,"Para Sair do jogo Pressione 'S'");
      DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-150,20-(VRES/4));
    } //Fim Stats == 7
}//Fim GAME OFF

// Desenha(): funcao chamada sempre que for preciso redesenhar a tela
void Desenha(void) {

    // Muda para o sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa a matriz de transformação corrente
    glLoadIdentity();

    // Limpa janela de visualizacao
    glClearColor(0.0f,0.15f,0.2f,0.0f); //especifica a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT); //pinta o fundo com a cor acima 

    glColor3f(1.0f, 1.0f, 1.0f); //Cor de desenho branco

    // Chama Visualização do Debug
    if(infos==ON)
      Debug();

    DesenhaTela();

    //Desenha de acordo com status do jogo
    GameScreen();
    if(StatusGame==ON)
      Stats=0;

    // Executa os comandos OpenGL
    glutSwapBuffers();
}//Fim Desenha()

void Score() {
  
}

//Fecha o jogo
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
      // FASE = 30; DELAY-30; // Testar configurações de ultimo level
      alive = lifes;
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

    //Rastros recomeçam com X Y da Head[0]
    for(j=1;j<contrastro;j++) {
      head[j][0]=head[j-1][0]; //Coord x
      head[j][1]=head[j-1][1]; //Coord y
    }
}

// Função callback chamada pela GLUT a cada intervalo de tempo
void Engine(int value) {
    int j, g;
    float t=0;
    //Incrementa variavel pra piscar a comida!
    pisca++;

    //Atualiza o tempo jogado e de espera
    start_time = clock();
    t  = ((start_time-end_time)/CLOCKS_PER_SEC);

    if(StatusGame==OFF && (Stats==4 || Stats==5)) // Contador para tela 4 e 5(Level e Choque)
      if ( (Stats==5 && (t >= TEMPO_ESPERA1-1)) || (Stats==4 && (t >= TEMPO_ESPERA2-1)) ) {
        StatusGame=ON;
      //Atualiza o tempo jogado e de espera
      clock_time = clock();
      }

    if(StatusGame==ON) {
      // Verifica se a bola bateu nas barras
      if((head[0][0] >= winXmax-5) || (head[0][0] <= winXmin+5) || (head[0][1] >= winYmax-5) || (head[0][1] <= winYmin+5)) {
        alive--;
        if(alive<=0)
          TerminaGame(2); // Tela GameOver
        else
          TerminaGame(1); // Tela Choque
      }
      //Perde se bater no próprio corpo
      for(j=1;j<contrastro+1;j++) {
        //if((head[0][0]==head[j][0])&&(head[0][1]==head[j][1])&&(((clock()-clock_time)/CLOCKS_PER_SEC)>1)) { // Usado para testar ultimo level sem bugar a vida !
        if((head[0][0]==head[j][0])&&(head[0][1]==head[j][1])) {
          alive--;
          if(alive==0)
            TerminaGame(2); // Tela GameOver
          else
            TerminaGame(1); // Tela Choque
        }
      }
      //Verifica se Head relou na comida e incrementa o corpo
      if((head[0][0]==food.xf)&&(head[0][1]==food.yf)) {
        food.xf=Xfood=getRandom(winXmin+10,winXmax-10);
        food.yf=Yfood=getRandom(winYmin+10,winYmax-10);
        if(contrastro<rastro)
          contrastro++;
      }
      //Guarda coordenadas do corpo da cobra
      for(j=contrastro;j>0;j--) {
        head[j][0]=head[j-1][0]; //Coord x
        head[j][1]=head[j-1][1]; //Coord y
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
      // Muda Level (FASE) se objetivo, rastros, estão completos !
      // Calculo que determina tamanho do corpo a atingir no level
      if(FASE<=10)
        g=FASE*2-2;
      else
        g=FASE*2-(FASE-8);
      g=10+g;
      //Verifica se tamanho do corpo igual tamanho a ser completado
      if((contrastro==g) && (FASE<MAXFASE))//(contrastro<rastro))
        TerminaGame(3);
      else if((contrastro==g) && (FASE=MAXFASE))//(contrastro==rastro)
        TerminaGame(4);
    } //Encerra IF StatusGame

    // Redesenha a Bola na nova posição
    glutPostRedisplay();
    glutTimerFunc(DELAY, Engine, 1);
}//fim Engine

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
      } else if(Pause==1 and StatusGame==OFF) {
        Stats=0;
        StatusGame=ON;
        Pause=0;
      }
    }

    if(tecla == 'i' || tecla == 'I') {
      if(infos==ON)
        infos=OFF;
      else
        infos=ON;
    }
} //Fim Teclado()

//Funcao para coordenadar a cobra
void TeclasEspeciais(int tecla, int x, int y) {
    switch(tecla) {
    case GLUT_KEY_UP : //seta para cima
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
    case GLUT_KEY_DOWN: //seta para cima
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
    case GLUT_KEY_LEFT : //seta para esquerda
      if (moveToY==1 && StatusGame==ON) {
        moveToX = 1;
        moveToY = 0;
        Hkey = 1;
      }
      break;
    case GLUT_KEY_RIGHT: //seta para direita
      if (moveToY==1 && StatusGame==ON) {
        moveToX = 1;
        moveToY = 0;
        Hkey = 0;
      }
      break;
    }
}//End Teclas Especiais

//AlteraTamanhoJanela(): funcao callback para redesenhar a janela
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    GLsizei x, y;
    // Evita a divisao por zero
    if(h == 0)
      h = 1;

    //Dimensoes da Viewport
    if (w > HRES)
      x = (w-HRES)/2;
    else {
      glutReshapeWindow(HRES,VRES);
      x = 0;
    }
    if (h > VRES)
      y = (h-VRES)/2;
    else {
      glutReshapeWindow(HRES,VRES);
      y = 0;
    }
    glViewport(x,y,HRES,VRES);

    //Inicia o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-HRES/2,HRES/2,-VRES/2,VRES/2);
    glMatrixMode(GL_MODELVIEW);
} //Fim AlteraTamanhoJanela()

// Inicializa(): Funcao que inicializa o ambiente de trabalho
void Inicializa() {
    HWND hWnd=FindWindow(TEXT("GLUT"), TEXT("Cobra Game"));
    LONG gwl=GetWindowLong(hWnd, GWL_STYLE);
    gwl&=(~WS_MAXIMIZEBOX)&(~WS_THICKFRAME);
    //SetWindowLong(hWnd, GWL_STYLE, gwl);

    // Chama função primaria
    TerminaGame();
    //glutFullScreen();
} //Fim Inicializa()

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
}//Fim main()
