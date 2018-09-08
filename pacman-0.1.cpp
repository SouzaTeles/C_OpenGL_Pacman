/*
 * Pacman - Criado para a Disciplina de Computação Grafica
 * By: Lucas Teles
 * Criado em: maio e junho de 2018
*/

#include <iostream>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#define MAX_L 60
#define MAX_C 60
#define G90 4.71239
#define WINSIZE 600
using namespace std;

//int habilita3d = 0;
int blue = 0;
float DELTA = 0.1; //Velocidade
float fantSpeed = 0.5;
float altura = 0;
int habilita3d = 0;
float dx; float dz;
int var = 0;

//** Declarações Globais ** //

struct Personagem
{
    GLfloat x, z, angulo;
} personagem;

typedef struct Fantasma
{
	GLfloat x, z, angulo, r, g, b;
} fantasma;

fantasma fant[4];// fLaranja, fVermelho, fAzul, fant[2];

struct Labirinto
{
    int mat[MAX_L][MAX_C];
    int L, C;
} lab;

int centroX = 512;
int centroY = 384;

void myReshape(int w, int h); 
void lerLabirinto(char *nomeArq);
void fantInit();
void fantColorInit();
void persoInit();
void mover(int t);
void teclou(unsigned char c, int x, int y);
void myDisplay(void); 
void moveu(int x, int y);
void mudaPerspectiva(int t);

float a, b;
int c = 0;

void fantasmas(fantasma *fCor){
	float dx = cos(fCor->angulo) * fantSpeed; //Proxima Posição
	float dz = sin(fCor->angulo) * fantSpeed; //Proxima Posição
	
	int c = floor(fCor->x + dx );
    int l = floor(fCor->z + dz );
	
    if (c < 0 || l < 0 || lab.mat[l][c] == 0 || lab.mat[l][c] == 3 || lab.mat[l][c] == 4)
    {
        fCor->x += dx;
        fCor->z += dz;       
    }
    else if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 6 ){
		fCor->x -= 26;	
			if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
	}
	
	else if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 7 ){
		fCor->x += 26;
			if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
	}
    
	else{
			fCor->angulo += G90;
			printf("%f", fCor->angulo);
	}       
}

void idle(int t){
	fantasmas(&fant[0]);
	fantasmas(&fant[1]);
	fantasmas(&fant[3]);
	fantasmas(&fant[2]);
	glutPostRedisplay();
	glutTimerFunc(80, idle, 1);
}

void descida(int t){
	if (altura > 0){
	altura --;
	glutPostRedisplay();
	glutTimerFunc(100, descida, 1);
	}
}

void subida(int t){
	if (altura < 20){
	altura++;
	glutPostRedisplay();
	glutTimerFunc(100, subida, 1);
	}
}

void bluePill(int t){
	if(blue == 0){
		blue = 1;
		glutTimerFunc(8000, bluePill, 1);
	}else if (blue == 1){
		blue = 0;
		fantColorInit();
	}
}

int main(int argc, char** argv) { 
    glutInit(&argc, argv); // OpenGL initializations
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);// double buffering and RGB
    glutInitWindowSize(WINSIZE, WINSIZE); 
    glutInitWindowPosition(300, 0); // ...in the upper left
    glutCreateWindow("Computação Grafica - Pacman"); // create the window

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(teclou);
    
    lerLabirinto("lab.txt");
    persoInit();
	fantInit();
	fantColorInit();
	glutTimerFunc(80, idle, 1);
    SetCursorPos(centroX, centroY);
    glutTimerFunc(100, mudaPerspectiva, 1); 
    glutFullScreen();
    glutMainLoop(); // start it running
    return 0; // ANSI C expects this
}

void myReshape(int w, int h) { 
    glViewport (0, 0, w, h); 
    glMatrixMode(GL_PROJECTION); // update projection
    glLoadIdentity();
    gluPerspective( 65.0, (GLdouble) w / h, 0.1, 50.0 );
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay(); // request redisplay
}

void lerLabirinto(char *nomeArq){
    FILE *fid = fopen(nomeArq, "rt");

    fscanf(fid, "%d %d", &lab.L, &lab.C);

    for (int l = 0; l < lab.L; l++)
        for (int c = 0; c < lab.C; c++)
            fscanf(fid, "%d", &lab.mat[l][c]);

    fclose(fid);
    
    for (int l = 0; l < lab.L; l++){
    	for(int c =0; c<lab.C; c++){
    		printf("%d ", lab.mat[l][c]);
		}
		printf("\n");
	}
}


void fantInit()
{
	//Laranja
	fant[0].x = 15.5;
	fant[0].z = 13.5;
	fant[0].angulo = 0;
	
	//Azul
	fant[1].x = 13.5;
	fant[1].z = 13.5;
	fant[1].angulo = 0;
	
	//Lilas
	fant[2].x = 12.5;
	fant[2].z = 14.5;
	fant[2].angulo = 0;
	
	//Vermelho
	fant[3].x = 14.5;
	fant[3].z = 14.5;
	fant[3].angulo = 0;	
}

void fantColorInit(){
			fant[0].r = .19;
			fant[0].g = .24;
			fant[0].b = .84; // Laranja
			
			fant[1].r = .29;
			fant[1].g = .87;
			fant[1].b = .79; // Azul
			
			fant[2].r = .98;
			fant[2].g = .23;
			fant[2].b = .07; // Vermelho
			
			fant[3].r = .99;
			fant[3].g = .76;
			fant[3].b = .83; // Lilas
}

void persoInit(){
	personagem.x = 15;
    personagem.z = 23.5;
    personagem.angulo = 4.71239;
}

void mover(int t)
{
	
	int c = floor(personagem.x + dx + .25);
    int l = floor(personagem.z + dz + .25);

    if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 0 )
    {
        personagem.x += dx;
        personagem.z += dz;
        lab.mat[l][c] = 3;
        if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
    }
    
    else if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 3 )
    {
        personagem.x += dx;
        personagem.z += dz;
        lab.mat[l][c] = 3;
        if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
    }
    
    else if(c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 5 ){
    	personagem.x += dx;
        personagem.z += dz;
        lab.mat[l][c] = 3;
    	glutTimerFunc(0, bluePill, 1);
    		if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
	}
	
	else if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 6 ){
		personagem.x -= 26;	
			if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
	}
	
	else if (c < 0 || l < 0 || c >= lab.C || l >= lab.L || lab.mat[l][c] == 7 ){
		personagem.x += 26;
			if(habilita3d == 1){
        	glutTimerFunc(80, mover, 1);        	
		}
	}
}

void teclou(unsigned char c, int x, int y)
{
		switch (c){
	        case 'w': case 'W':
						        dx = cos(personagem.angulo) * DELTA;
						        dz = sin(personagem.angulo) * DELTA;
						        glutTimerFunc(25, mover, 1);
	        break;
	        
			case 's': case 'S':
								dx = -cos(personagem.angulo) * DELTA;
								dz = -sin(personagem.angulo) * DELTA;
								glutTimerFunc(25, mover, 1);
	        break;
	        
			case 'a': case 'A': 
								dx = cos(personagem.angulo - M_PI_2) * DELTA;
								dz = sin(personagem.angulo - M_PI_2) * DELTA;
								glutTimerFunc(25, mover, 1);
	        break;
	        
			case 'd': case 'D': 
								dx = cos(personagem.angulo + M_PI_2) * DELTA;
								dz = sin(personagem.angulo + M_PI_2) * DELTA;
								glutTimerFunc(25, mover, 1);
	        break;
	        
			case 'l': case 'L': mudaPerspectiva(1);
	        break;
	        
			case 'm': case 'M': glutTimerFunc(100, descida, 1);
	        break;  
			
			case 'n': case 'N': glutTimerFunc(100, subida, 1);
			break; 
			
			case 'y': case 'Y': var = 1;
			break;
			
			case 't': idle(1);
			break;
	    }
			
    glutPostRedisplay();
}

void myDisplay(void)
{ 
    glClearColor(0, 0, 0, 1.0); // background is gray
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
    glLoadIdentity();    
    
    gluLookAt(personagem.x , altura, personagem.z ,
              personagem.x + cos(personagem.angulo), 0, personagem.z + sin(personagem.angulo),
              0, 1, 0);    

    for (int l = 0; l < lab.L -1; l++)
        for (int c = 0; c < lab.C; c++)
        	switch(lab.mat[l][c]){
        		case 1: 
                glPushMatrix();
                    glTranslatef(c + 0.5, 0, l + 0.5);
                    glColor3f(.19, .24, .84);
                    glutSolidCube(1);
                    glColor3f(0.05, 0.07, 0.24);
                    glutWireCube(1.0001);
                glPopMatrix();
                break;
			
				case 0:
            	glPushMatrix();
            		glTranslatef(c + 0.5, 0, l + 0.5);
            		glColor3f(1, .7, .65);
            		glutSolidSphere(.1,10, 10 );
				glPopMatrix();	
				break;
				
				case 5:	
			    glPushMatrix();
            		glTranslatef(c + 0.5, 0, l + 0.5);
            		glColor3f(1, .7, .65);
            		glutSolidSphere(.3,10, 10 );
				glPopMatrix();
				break;
				
				case 4:
				glPushMatrix();
                    glTranslatef(c + 0.5, 0, l + 0.5);
                    glColor4f(1, 1, 1, 0.5 );
                    glutSolidCube(1);
                    glColor3f(0.2, 0.2, 1);
                    glutWireCube(1.0001);
                glPopMatrix();
				break;
        }
    glPushMatrix();
    	glTranslatef(personagem.x, 1, personagem.z);
    	glColor3f(255, 255, 0);
    	glutSolidSphere(.5, 10, 10);
    glPopMatrix();
    
    /* .:: Fantasmas ::. */
    for(int i = 0; i < 4; i++)
	{
	    glPushMatrix();
	    	glTranslatef(fant[i].x, -.5, fant[i].z);
	    	if(blue)
	    		glColor3f(0, .24, 0.215);
			else
			glColor3f(fant[i].r, fant[i].g, fant[i].b);		
			glRotatef(270, 1, 0, 0);	
			glRotatef(fant[i].angulo, 0, 0, 1);
			glutSolidCone(0.5, 1, 10,10);	
	    glPopMatrix();
    }
	glutSwapBuffers(); // swap buffers
}

void moveu(int x, int y)
{
    POINT pos;
    GetCursorPos(&pos);    
    if(pos.x != centroX || pos.y != centroY)
	{
    	SetCursorPos(centroX, centroY);
	}    
    int deltaX = pos.x - centroX;
    personagem.angulo += deltaX * 0.01;
    glutPostRedisplay();
}

void mudaPerspectiva(int t){
	if(habilita3d == 1)
	{
	DELTA = 0.1;
	glutPassiveMotionFunc(moveu);
	
	descida(1);
	habilita3d =  0;
	}
	else {
		DELTA = 0.5;
		//altura = 20;
		glutPassiveMotionFunc(NULL);
		personagem.angulo = 4.71239;
		subida(1);
		habilita3d = 1;
	}
}
