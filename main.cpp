
#include <GL/gl.h>
#include <GL/GLUT.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <GL/glext.h>
#include "getbmp.h"

//Defines
#define SENS_ROT	5.0
#define SENS_OBS	10.0
#define SENS_TRANSL	10.0
//Constantes
const double PI = 3.141592654;

//Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
double ang;

//Globais.
char texto[30];
int frameNumber = 0;
int x_ini,y_ini,bot;
static unsigned int texture[2]; //Array para os ids da textura
static int id = 0; //Textura atual

// Carrega Textura
void loadExternalTextures(){
	// cria um espaco para alocar a imagem bmp localmente
	BitMapFile *image[1];
	
	// Carrega a imagem
	image[0] = getbmp("pluton-projeto.bmp"); 
	
	// Cria o objeto de textura, no id 0
	glBindTexture(GL_TEXTURE_3D, texture[0]); 
	
	// Especifica os dados da textura
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	
	// Configura os parametros de wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Configura os parâmetros de filtro de textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

//Função para desenhar a LUA, para poder aplicar a TEXTURA, GlutSolidSphere não 
//mapeia coordenadas de textura por padrão. 
void solidSphere(int radius, int stacks, int columns){
	GLUquadric* quadObj = gluNewQuadric();  
	gluQuadricDrawStyle(quadObj, GLU_FILL);  
	gluQuadricNormals(quadObj, GLU_SMOOTH);  
	gluQuadricTexture(quadObj, GL_TRUE); //Esse e o comando que falta
	gluSphere(quadObj, 80, stacks, columns);  
gluDeleteQuadric(quadObj);
}

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao (void){
	
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	GLfloat posicaoLuz[4]={20.0, 80.0, 50.0, 0.3};
	
	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 05;
	
	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
	
	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void disco(double raio){
	int d;
	glBegin(GL_POLYGON);
	for (d = 0; d < 32; d++) {
		double angulo = 2*PI/32 * d;
		glVertex2d( raio*cos(angulo), raio*sin(angulo));
	}
	glEnd();
}

void desenhaCirculo(){
	int i;
	glColor3f(0,0,0);
	
	for (i = 0; i < 2; i++) { // Desenha 2 linhas para o pedal da bicicleta
		glRotatef( 360 / 2, 0, 0, 1 ); // Acumula rotações para movimento
		glBegin(GL_LINES);                                
		glVertex2f(0, 0);
		glVertex2f(0.35f, 0);
		glEnd();
	}
	disco(0.2);
	glColor3f(0,0,0);
}

void desenhaRoda(){
    int i;
    GLfloat circle_points = 100;
	glColor3f(0,0,0);
	disco(0.2); //desenha centro da roda
	glRotatef(frameNumber*20,0,0,1);
	
	for (i = 0; i < 7; i++) { // desenha 'aro'
		glLineWidth(1.0f);
		glBegin(GL_LINES);
			glVertex2f(0,0);
			glVertex2d(cos(i*2*PI/7), sin(i*2*PI/7));
		glEnd();
	}
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP); // desenha 'Pneu'
		for (i = 0; i < circle_points; i++) {
			angle = 2*PI*i/circle_points;
			glVertex2f(cos(angle), sin(angle));
		}
	glEnd();  
	
}

// Modelagem da bicicleta
void desenhaBike(){
	
	glPushMatrix();
		glTranslatef(-1.5f, -0.1f, 0);
		glScalef(0.8f,0.8f,1);
		desenhaRoda();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(1.5f, -0.1f, 0);
		glScalef(0.8f,0.8f,1);
		desenhaRoda();
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(0,0,0);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			glVertex2f(-1.6f,0);
			glVertex2f(0,0);
			glVertex2f(0,0);
			glVertex2f(1.4f,1);
			glVertex2f(1.4f,1);
			glVertex2f(0,1);
			glVertex2f(0,1);
			glVertex2f(-1.6f,0);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glBegin(GL_LINES);
			glVertex2f(1.6f,0);
			glVertex2f(1.4f,1.2);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glBegin(GL_LINES);
			glVertex2f(1.4f,1.2);
			glVertex2f(1.9f,1.5);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glBegin(GL_LINES);
			glVertex2f(0,0);
			glVertex2f(0,1.1);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glBegin(GL_TRIANGLES);
			glVertex2f(0,1.1);
			glVertex2f(0.5,1.1);
			glVertex2f(0,1.35);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glRotated(-frameNumber*0.7,0,0,1);
		desenhaCirculo();
	glPopMatrix();
}

// Modelagem da árvore
void DesenhaArvore(){
	glPushMatrix();	
		glTranslatef(-50.0, -10.0, 50.0);
		glScalef(15.0,10.0,1.0);
		glBegin(GL_QUADS);
			glColor3f(0, 0, 0);
			glVertex2f(5,0);
			glVertex2f(5.2,0);
			glVertex2f(5.2,1);
			glVertex2f(5,1);
		glEnd();
		glBegin(GL_TRIANGLES);
			glColor3f(0, 0, 0);
			glVertex2f(4.5,1);
			glVertex2f(5.7,1);
			glVertex2f(5.1,3);
		glEnd();
	glPopMatrix();
}

// Função callback de redesenho
void Desenha(void){
	float xt, yt;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Chama a função que especifica os parâmetros de iluminação
    DefineIluminacao();
       
	//desenha lua
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(10.0, 0.0, -60.0);
		glRotatef(-90, 1, 0, 0);
		solidSphere(65.0, 35, 20);
	glPopMatrix();
	
    //desenha chao
	glPushMatrix();	
		glTranslatef(-50.0, -20.0, 50.0);
		glScalef(15.0,10.0,1.0);
		glBegin(GL_POLYGON);
			glColor3f(0, 0, 0);
			glVertex2f(-3,-4);
			glVertex2f(7,-4);
			glVertex2f(7,0.6f);
			glVertex2f(-3,0.6f);
		glEnd();
		glBegin(GL_POLYGON);
			glColor3f(0, 0, 0);
			glVertex2f(-3,0.6f);
			glVertex2f(7,0.6f);
			glVertex2f(7,1.1);
			glVertex2f(-3,1.1);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		DesenhaArvore();
		glTranslatef(-3,-7,0);
		glScalef(0.7,0.7,0.7);
		DesenhaArvore();
	glPopMatrix();
	
	//desenha bicicleta
	glPushMatrix();
		xt= -90 + (50*(frameNumber % 300)/100.0);
		yt= -10 + (10*(frameNumber % 300)/100.0)+(frameNumber % 300)/100.0;
		if (yt < 25) 
		glTranslatef(xt, yt ,50.0); //Sobe a bicicleta	
		glScaled(2.2,2.2,1.0);
		desenhaBike();
	glPopMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glutSwapBuffers();
}

void doFrame(int v){
	frameNumber++;
	glutPostRedisplay();
	glutTimerFunc(30,doFrame,0);
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void){
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);

	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	DefineIluminacao();

	// Posiciona e orienta o observador
	glTranslatef(-obsX,-obsY,-obsZ);
	glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
}

// Função usada para especificar a visualização
void EspecificaParametrosVisualizacao(void){
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle,fAspect,0.5,500);
	
	PosicionaObservador();
}

// Função callback chamada para gerenciar eventos de teclas normais e esc
void keyInput(unsigned char key, int x, int y){
	switch(key)
	{
		case 'z':obsZ++;
			break;
		case 'Z':obsZ++;
			break;
		case 'x':obsZ--;
			break;
		case 'X':obsZ--;
			break;				
		case 27:
		    exit(0);
			break;
		default:
		    break;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y){
	switch (tecla)
	{
		case GLUT_KEY_LEFT:rotY--;
			break;
		case GLUT_KEY_RIGHT:rotY++;
			break;
		case GLUT_KEY_UP:rotX++;
			break;
		case GLUT_KEY_DOWN:rotX--;
			break;
		case GLUT_KEY_F10:// "camera reset"
			rotX=0;
			rotY=0;
			obsZ=150;
			angle=45;
			EspecificaParametrosVisualizacao();
			break;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

// Gerenciamento do menu com as opções de cores           
void MenuInteracoes(int op){
	glutPostRedisplay();	
	switch(op){
		case 0:
			glutPostRedisplay();
			rotX=0;
			rotY=0;
			obsZ=150;
			angle=45;
			EspecificaParametrosVisualizacao();
			glutPostRedisplay();
		break;
		
		case 1:
			BitMapFile *image[3];
		   
		   // Load the image.
		   image[0] = getbmp("foliage.bmp"); 
		   
		   // Create texture object texture[0]. 
		   glBindTexture(GL_TEXTURE_3D, texture[0]); 
		
		   // Specify image data for currently active texture object.
		   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
			            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
		
		   // Set texture parameters for wrapping.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		   // Set texture parameters for filtering.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
		   solidSphere(65.0, 35, 20);
		   glutPostRedisplay();
		break;
		
		case 2:
		   
		   // Load the image.
		   image[0] = getbmp("terra.bmp"); 
		   
		   // Create texture object texture[0]. 
		   glBindTexture(GL_TEXTURE_3D, texture[0]); 
		
		   // Specify image data for currently active texture object.
		   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
			            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
		
		   // Set texture parameters for wrapping.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		   // Set texture parameters for filtering.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   			
			glPushMatrix();
			solidSphere(65.0, 35, 20);
			glPopMatrix();
		   glutPostRedisplay();
		break;
		
		case 3:
		   
		   // Load the image.
		   image[0] = getbmp("pluton-projeto.bmp"); 
		   
		   // Create texture object texture[0]. 
		   glBindTexture(GL_TEXTURE_3D, texture[0]); 
		
		   // Specify image data for currently active texture object.
		   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
			            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
		
		   // Set texture parameters for wrapping.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		   // Set texture parameters for filtering.
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   			
			glPushMatrix();
			solidSphere(65.0, 35, 20);
			glPopMatrix();
		   glutPostRedisplay();
		break;
	}
glutPostRedisplay();
}         

// Gerenciamento do menu principal           
void MenuPrincipal(int op){}
              
// Criacao do Menu
void CriaMenu(){
    int menu,submenu1,submenu2;

	submenu1 = glutCreateMenu(MenuInteracoes);
	glutAddMenuEntry("Reseta visão da cena (F10)",0);
	
	
	submenu2 = glutCreateMenu(MenuInteracoes);
	glutAddMenuEntry("Textura - Marte",1);
	glutAddMenuEntry("Textura - Terra",2);
	glutAddMenuEntry("Textura - Lua",3);
	
    menu = glutCreateMenu(MenuPrincipal);
	glutAddSubMenu("Interações",submenu1);
	glutAddSubMenu("Muda textura",submenu2);
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPostRedisplay();
}

// Função callback para eventos de botões do mouse
void GerenciaMouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(state==GLUT_DOWN)
		{
			// Salva os parâmetros atuais
			x_ini = x;
			y_ini = y;
			obsX_ini = obsX;
			obsY_ini = obsY;
			obsZ_ini = obsZ;
			rotX_ini = rotX;
			rotY_ini = rotY;
			bot = button;
	    }
		else bot = -1;
	}
	if(button == GLUT_RIGHT_BUTTON){
		if(state == GLUT_DOWN){
			CriaMenu();
		}
	}
	glutPostRedisplay();
}

// Função callback para eventos de movimento do mouse

void GerenciaMovim(int x, int y){
	// Botão esquerdo ?
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica ângulos
		rotY = rotY_ini - deltax/SENS_ROT;
		rotX = rotX_ini - deltay/SENS_ROT;
	}
	// Botão direito ?
	else if(bot==GLUT_RIGHT_BUTTON)
	{
		// Calcula diferença
		int deltaz = y_ini - y;
		// E modifica distância do observador
		obsZ = obsZ_ini + deltaz/SENS_OBS;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h){
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;
	
	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);
	
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
	
	EspecificaParametrosVisualizacao();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void){
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(0.1f, 0.1f, 0.44f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);
	
	// Inicializa a variável que especifica o ângulo da projeção perspectiva
	angle=45;
	
	// Inicializa as variáveis usadas para alterar a posição dO observador virtual
	rotX = 0;
	rotY = 0;
	obsX = obsY = 0;
	obsZ = 150;
	
	// CRIA os ids da textura
	glGenTextures(1, texture);
	
	// Carrega uma textura externa 
	loadExternalTextures();
	//Especifica como os valores da textura vao combinar com os valores de cor da superfície
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);	
}
    
// Programa Principal
int main(void){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(5,5);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Projeto - Cena Filme ET");
	glutTimerFunc(200,doFrame,0);
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutSpecialFunc (TeclasEspeciais);
	glutMouseFunc(GerenciaMouse);
	glutMotionFunc(GerenciaMovim);
	Inicializa();
	glutKeyboardFunc(keyInput);
	glutMainLoop();
	
	return 0;
}

