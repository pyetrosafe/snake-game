/* MinhocaGLv1.CPP **********************************************************
 * ============                                                             *
 *              1 Implementa a bolinha(minhoca) se movimentando na tela     *
 *              2 Implementa Teclas de movimentação                         *
 ****************************************************************************/

#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Definicao de Constantes
#define HRES 800  //Resolucao Inicial da Window na Horizontal
#define VRES 600  //Resolucao Inicial da Window na Vertical
#define INC 10     //Incremento em pixel para X e Y da Minhoca
#define rastro 100 //Define qnts partes o corpo da minhoca
#define ON 1 //define constante ON ativado
#define OFF 0 //define constante OFF desativado
#define lifes 5
#define MAXFASE 100

//Estrutura pra guardar pontos da minhoca
struct minhocas{
       float xf, yf;
};

//Declaracao de Variaveis Globais
GLfloat head[rastro][2];
struct minhocas food;
long seed=0, conter=0;
char niveltxt[100];
int StatusGame, Stats, sizefont, getout=0, alive, FASE;
int i=0, pisca=0, contrastro=0, DELAY, timer, ztimer;
int winXmin=-(HRES/2), winXmax=(HRES/2);  //Tamanho minimo e maximo em X da Janela
int winYmin=-(VRES/2), winYmax=(VRES/2);  //Tamanho minimo e maximo em Y da Janela
int incX, incY, Pause=0;  //Fatores de increment em X e Y para movimentar a Minhoca
GLfloat Xfood, Yfood;
GLfloat moveToX, moveToY; //Indica se a minhoca vai andar horizontal ou vertical
GLfloat Vkey, Hkey; //Fator que define a direção da minhoca

// Função que recebe a fonte e um texto por parâmetro para ser exibido na 
// tela usando caracteres bitmap
void DesenhaTexto(void *font, char *string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) {
    // Exibe caractere a caractere

    glColor3f(red, green, blue);
    glRasterPos2f(x,y); 

    while(*string) {
        if(sizefont==10) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++);
        }
        if(sizefont==20) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
        }
    }
} //Fim DesenhaTexto

int getRandom(int min,int max) {
  int r;

  if(!seed) {
    time(&seed);  //Pega a hora atual
    srand((unsigned)seed);  //utiliza a hora como semente do gerador de nros.
  }
  r=min+rand()%(max-min+1);
  if(r%10!=0){
              r/=10;
              r*=10;
              }
  return r;
}//Fim getRandom

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
}//Fim DesenhaTela()

void GAMEON(){
    int j;
    char msg[120];
    //Desenha Minhoca (HEAD)
    glColor3f(1.0f, 0.4f, 0.4f);
    glPointSize(9.0f);
    glBegin(GL_POINTS);
    glVertex2f(head[0][0],head[0][1]);
    glEnd();

    //Desenha a comida
    glColor3f(1.0f, 1.0f, 0.7f);
    if(pisca%10!=0){
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(Xfood,Yfood);
        glEnd();}
        if(pisca>=99) {
            pisca=0;
        }

        // Posição no universo onde o texto bitmap será colocado
       sizefont=10;
       glColor3f(1.0f, 1.0f, 1.0f);
       glRasterPos2f(-290,240); 
       sprintf(msg,"Head - X = %1.f , Y = %1.f",head[0][0], head[0][1]);
       DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);

       glRasterPos2f(-290,230); 
       sprintf(msg,"Food - X = %1.f , Y = %1.f",food.xf, food.yf);
       DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);

       glRasterPos2f(-290,260); 
       sprintf(msg,"Partes do Corpo = %i ",contrastro);
       DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);

       glRasterPos2f(-290,270);
        sprintf(msg,"Vidas = %i",alive);
       DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);

       glRasterPos2f(-290,280); 
       sprintf(msg,"LEVEL = %i",FASE);
       DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);

       //Desenha o corpo da minhoca
       if(contrastro>0){
           for(j=1;j<contrastro+1;j++){
               glColor3f(1.0f, 0.7f, 0.7f);
               glPointSize(9.0f);
               glBegin(GL_POINTS);
               glVertex2f(head[j][0],head[j][1]);
               glEnd();
           }
       }
}//Fim Game ON

// Mensagens na tela!!!
void GAMEOFF() {
     char msg[120];
     sizefont=20;
     if(Stats==0){
         sprintf(msg,"!!! Snake Game !!!");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-110,200,1.0f,0.4f,0.4f);

         sprintf(msg,"Escolha a dificuldade!");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,160,1.0f,1.0f,1.0f);
/*
         glRasterPos2f(-100,120);
         sprintf(msg,"Pressione no teclado:");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg);
*/
         sprintf(msg,"Facil = 'E'  \  Dificil = 'H'");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,80);
         sprintf(msg,"Facil = 'E'  \  Dificil = 'H'");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,80);
         sprintf(msg,"Facil = 'E'  \  Dificil = 'H'");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,80);

         sprintf(msg,"Comandos:");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,0);

         sprintf(msg,"'S' = Sair");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-40);

         sprintf(msg,"'O' = Opcoes");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-80);

         sprintf(msg,"'P' = Pause");
         DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-100,-120);
       }//Fim Stats == 1

       if(Stats==2){
           sprintf(msg,"Voce Ainda tem: %i Vidas",alive);
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-130,120);
           
           sprintf(msg,"Timer: %i",ztimer);
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-130,90);
       }//Fim Stats == 2

       if(Stats==3){
           sprintf(msg,"Voce Perdeu");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-290,120);
                    
           sprintf(msg,"Para voltar as Opcoes do Menu Principal Pressione 'O'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-290,40);
           
           sprintf(msg,"Para Finalizar o jogo Pressione 'S'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-290,0);
       } //Fim Stats == 3

       if(Stats==4){
           sprintf(msg,"PARABENS, Voce Finalizou o Jogo!");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-160,100);
           
           sprintf(msg,"Pressione 'O' para Opcoes do Menu Principal!");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-200,40);
           
           sprintf(msg,"Pressione 'S' para Sair!");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-200,10);
       }//Fim Stats == 4

       if(Stats==5){
           sprintf(msg,"Para Sair pressione 'S'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-120,140);
           
           sprintf(msg,"Para Voltar pressione 'P'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-120,100);
           
           sprintf(msg,"Para voltar ao Menu pressione 'O'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-120,60);
       }//Fim Stats == 5

       if(Stats==6){
           sprintf(msg,"PAUSADO");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-60,140);
           
           printf(msg,"Para voltar pressione 'P'");
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-110,100);
       }//Fim Stats == 6

       if(Stats==7){
           sprintf(msg,"LEVEL %i",FASE);
           DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,msg,-40,140);
       }//Fim Stats == 7
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
   DesenhaTela();
   //Desenha se status do jogo = on
   if(StatusGame==ON){
  	                  GAMEON();
    }//Fim do IF StatusGame==ON
   if(StatusGame==OFF){
                       GAMEOFF();
   }//Encessa Else
   // Executa os comandos OpenGL
   glutSwapBuffers();
}//Fim Desenha()

//Fecha o jogo
void TerminaGame(int chama){
     int j;
     fflush(stdin);
     timer=clock()/CLOCKS_PER_SEC;
     StatusGame=OFF;
     Stats=chama;
     FASE=1;
     contrastro=0;
     alive=lifes;
     if(Stats!=6){
     head[0][0] = getRandom(winXmin+10,winXmax-10);
     head[0][1] = getRandom(winYmin+10,winYmax-10);
     food.xf = Xfood = getRandom(winXmin+10,winXmax-10);
     food.yf = Yfood = getRandom(winYmin+10,winYmax-10);
     moveToX = 1;
     moveToY = 0;
     if(head[0][0]>100||head[0][0]<-100){
                            if(head[0][0]>100){
                                           Hkey=1;
                                           }
                            if(head[0][0]<-100)
                                           Hkey=0;
                                           }
                                           if(head[0][0]>100||head[0][0]<-100){
                            if(head[0][1]>100){
                                           Vkey=1;
                                           }
                            if(head[0][1]<-100)
                                           Vkey=0;
                                           }
      //Rastros recomeçam com X Y da Head[0]
      for(j=1;j<contrastro;j++){
                                    head[j][0]=head[j-1][0]; //Coord x
                                    head[j][1]=head[j-1][1]; //Coord y
      }
     }//End IF Stats!=6
}

// Função callback chamada pela GLUT a cada intervalo de tempo
void Engine(int value) {
     int j, stimer, g;
     //Incrementa variavel pra piscar a comida!
     pisca++;

if(StatusGame==OFF && Stats==2){
                  stimer=clock()/CLOCKS_PER_SEC-timer;
                  ztimer=3;
                  if(stimer>0)
                              ztimer-=stimer;
                  if(stimer>3){
                               StatusGame=ON;                                
                  }
}

if(StatusGame==OFF && Stats==7){
                  stimer=clock()/CLOCKS_PER_SEC-timer;
                  ztimer=3-stimer;
                  if(stimer>3){
                               StatusGame=ON;                                
                  }
}

if(StatusGame==ON){
    if(Stats==0 || Stats==4){
                 alive=lifes;
    }
	// Verifica se a bola bateu nas barras
	if((head[0][0] >= winXmax-5) || (head[0][0] <= winXmin+5) || (head[0][1] >= winYmax-5) || (head[0][1] <= winYmin+5)){ //bateu nas bordas verticais
	alive--;
	        if(alive==0){
                alive=lifes;
              TerminaGame(3);
            }
            else{
	        TerminaGame(2);
            }
    }
    //Perde se bater no próprio corpo
    for(j=1;j<contrastro+1;j++){
        if((head[0][0]==head[j][0])&&(head[0][1]==head[j][1])){
            alive--;
            if(alive==0){
                alive=lifes;
              TerminaGame(3);
            }
            else{
	        TerminaGame(2);
            }
        }
    }
    //Verifica se a minhoca está completa
/*    switch(FASE){
             case 1:  if(contrastro==rastro-12){
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 2:  if(contrastro==rastro-10){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 3:  if(contrastro==rastro-6){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 4:  if(contrastro==rastro-4){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 5:  if(contrastro==rastro-3){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 6:  if(contrastro==rastro-2){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 7:  if(contrastro==rastro-1){
                      DELAY-=5;
                      contrastro=0;
                      FASE++;
                      TerminaGame(7);
                      }
                      break;
             case 8:  if(contrastro==rastro){
                      DELAY-=5;
                      contrastro=0;
                      alive=lifes;
                      FASE=1;
                      TerminaGame(4);
                      }
                      break;
    }//Fim Switch
    */
    for(i=MAXFASE;i>1;i--){
                           g=(i-MAXFASE)+1;
                           if(FASE==i){
                                       if(contrastro==rastro-g){
                                       contrastro=0;
                                       FASE++;
                                       TerminaGame(7);
                                       }
                           }
    }
    
    
    //Verifica se Head relou na comida e incrementa o corpo
    if((head[0][0]==food.xf)&&(head[0][1]==food.yf)){
                                                     food.xf=Xfood=getRandom(winXmin+10,winXmax-10);
                                                     food.yf=Yfood=getRandom(winYmin+10,winYmax-10);
                                                     if(contrastro<rastro){
                                                                  contrastro++;
                                                     }
    }
    
    //Guarda coordenadas do corpo da minhoca
    for(j=contrastro;j>0;j--){
                              head[j][0]=head[j-1][0]; //Coord x
                              head[j][1]=head[j-1][1]; //Coord y
    }

     
	// Move a minhoca (Head[0])
     if(moveToX == 1){
                if(Hkey == 0){
                        head[0][0] += incX;
                }                         
                if(Hkey == 1){
                        head[0][0] -= incX;
                }       
     }                                    
     if(moveToY == 1){                    
                if(Vkey == 0){
                        head[0][1] += incY;
                }                         
                if(Vkey == 1){
                        head[0][1] -= incY;
                }
     }
 }//Encerra IF StatusGame
 
	// Redesenha a Bola na nova posição
	glutPostRedisplay();

	glutTimerFunc(DELAY, Engine, 1);
}//fim Engine

// Teclado(): funcao chamada sempre que for pressionada uma tecla ASCII
void Teclado(unsigned char tecla, int x, int y){
    if(tecla == 'e' || tecla == 'E'){
             if(StatusGame==OFF && Stats==0){
           DELAY = 100;
           Stats=7;
             }
    }
    if(tecla == 'h' || tecla == 'H'){
             if(StatusGame==OFF && Stats==0){
           DELAY = 60;
           Stats=7;
             }
    }
    if(tecla == 'o' || tecla == 'O'){
             TerminaGame(0);
    }
     
    if(tecla == 's' || tecla == 'S'){
            if(Stats==5){
                        exit(0);
             }
             else{
             Pause=1;
             StatusGame=OFF;
             Stats=5;
             }
    }
    
    if(tecla == 'p' || tecla == 'P'){
             if(Pause==0){
                          StatusGame=OFF;
                          Stats=6;
                          Pause=1;
             }
             else{
                  Stats=0;
                  StatusGame=ON;
                  Pause=0;
             }
    }
} //Fim Teclado()

//Funcao para coordenadar a minhoca
void TeclasEspeciais(int tecla, int x, int y){
     switch(tecla){      
      case GLUT_KEY_UP : //seta para cima
           moveToX = 0;
           moveToY = 1;
           Vkey = 0;
      break;
      case GLUT_KEY_DOWN: //seta para cima
           moveToX = 0;
           moveToY = 1;
           Vkey = 1;
      break;
      case GLUT_KEY_LEFT : //seta para esquerda
           moveToX = 1;
           moveToY = 0;
           Hkey = 1;
      break;
      case GLUT_KEY_RIGHT: //seta para direita
           moveToX = 1;
           moveToY = 0;
           Hkey = 0;
      break;
      }
}//End Teclas Especiais

//AlteraTamanhoJanela(): funcao callback para redesenhar a janela
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
   GLsizei largura, altura;
    
 	// Evita a divisao por zero
	if(h == 0) h = 1;
   
   largura = w;
   altura = h;
   
   //Dimensoes da Viewport
   glViewport(0,0,largura,altura);
   
   //Inicia o sistema de coordenadas
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   //Estabelece a janela de selecao
   //Tambem atualiza as variaveis de dimensoes da janela
   if (largura <= altura) { 
	   gluOrtho2D (-HRES/2, HRES/2, -(VRES/2)*(altura/largura), (VRES/2)*(altura/largura));
		winXmin = -HRES/2;
		winXmax = HRES/2;
      winYmin = -(VRES/2)*(altura/largura);
      winYmax = (VRES/2)*(altura/largura);
	}else { 
        gluOrtho2D (-(HRES/2)*(largura/altura), (HRES/2)*(largura/altura), -VRES/2, VRES/2);
        winXmin = -(HRES/2)*(largura/altura);
        winXmax = (HRES/2)*(largura/altura);
        winYmin = -VRES/2;
        winYmax = VRES/2;
    }    
   glMatrixMode(GL_MODELVIEW);
}//Fim AlteraTamanhoJanela()

// Inicializa(): Funcao que inicializa o ambiente de trabalho
void Inicializa() {
   int j;  
   fflush(stdin);
   //Dimensoes da ViewPort
   glViewport(0,0,HRES,VRES);
     
   //Define a janela de visualizacao 2D
   glMatrixMode(GL_PROJECTION);
   gluOrtho2D(-HRES/2,HRES/2,-VRES/2,VRES/2);
   glMatrixMode(GL_MODELVIEW);
     
   //Inicializacao das variaveis globais  
   contrastro=0;
   incX = incY = INC;
   head[0][0] = getRandom(winXmin+10,winXmax-10);
   head[0][1] = getRandom(winYmin+10,winYmax-10);
   food.xf = Xfood = getRandom(winXmin+10,winXmax-10);
   food.yf = Yfood = getRandom(winYmin+10,winYmax-10);
   moveToX = 1;
   moveToY = 0;
   StatusGame=OFF;
   Stats=0;
   alive=lifes;
   FASE=1;
   if(head[0][0]>100||head[0][0]<-100){
                               if(head[0][0]>100){
                                              Hkey=1;
                                              }
                               if(head[0][0]<-100)
                                              Hkey=0;
   }
   if(head[0][0]>100||head[0][0]<-100){
                               if(head[0][0]>100){
                                              Vkey=1;
                                              }
                               if(head[0][0]<-100)
                                              Vkey=0;
   }
 
   //glutFullScreen();
} //Fim Inicializa()

// Programa Principal 
int main(void) {
         
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //Modo exibicao da GLUT    
   glutInitWindowSize(HRES,VRES); //Tamanho inicial em pixel da janela Glut
   glutInitWindowPosition(100,100);  //Posicao inicial da janela Glut na Tela
   //Cria a janela passando o Titulo
   glutCreateWindow("MinhocaGLv1 - Versão Modificada da Minhoquinha");  
   glutDisplayFunc(Desenha); //Registra Funcao de desenho da janela
   glutReshapeFunc(AlteraTamanhoJanela); //Registra funcao decredimensionamento  
   glutKeyboardFunc(Teclado);  //Registra Funcao para tratamento das teclas
   glutSpecialFunc(TeclasEspeciais); // Registra a função que trata teclas especiais
   // Registra função que será chamada a cada intervalo de tempo
	glutTimerFunc(50, Engine, 1);
   Inicializa(); //Faz as inicializacoes necessarias 
   glutMainLoop(); //Inicia processamento e aguarda interacoes do usuario
     
   return 0;
}//Fim main()
