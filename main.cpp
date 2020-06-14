#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1350
#define HEIGHT 690

// Controle da Translação
float Tx = 0.0f, Ty = 0.0f;

// Definição de limites
const float xMax = 2.5f, xMin = -1.0f, yMax = 2.0f, yMin = -3.0f;

// Velocidade da bola
static float vx = 2.0, vy = 0.01;

// Controle da direção da bola
float direcao = -1.0;

// Controle da cor
int cor = 0;

void init();
void display();
void makeRedTexture();
void makeGreenTexture();
void makeBlueTexture();
void genSphereVertex(float i, float j, float *vertex);
void drawSurface(void (*genVertex)(float, float, float*));
void makeTextureShadow();
void genShadowVertex(float i, float j, float *vertex);
void drawShadow(void (*genVertex)(float, float, float*));
void bouncingBall();
void planos();
void keyboard(unsigned char key, int x, int y);
void menuCores(int op);
void menuPrincipal(int op);
void criaMenu();

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("AMIGA - Bouncing Ball Demo");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    criaMenu();
    init();
    glutMainLoop ();

    return(0);
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
    gluPerspective(45.0f,
                  (float)WIDTH / (float)HEIGHT,
                   1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        if(cor == 0)
        {
            makeRedTexture();
        }

        if(cor == 1)
        {
            makeGreenTexture();
        }

        if(cor == 2)
        {
            makeBlueTexture();
        }

        bouncingBall();
    glPopMatrix();

    planos();

    //glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void makeRedTexture()
{
    GLubyte texture[16][16][3];
    int i, j;

    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 16; j++)
        {
            if((i + j) % 2 == 0)
            {
                // Branco
                texture[i][j][0] = 0xff;
                texture[i][j][1] = 0xff;
                texture[i][j][2] = 0xff;
            }
            else
            {
                // Vermelho
                texture[i][j][0] = 0xff;
                texture[i][j][1] = 0x00;
                texture[i][j][2] = 0x00;
            }
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}

void makeGreenTexture()
{
    GLubyte texture[16][16][3];
    int i, j;

    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 16; j++)
        {
            if((i + j) % 2 == 0)
            {
                // Branco
                texture[i][j][0] = 0xff;
                texture[i][j][1] = 0xff;
                texture[i][j][2] = 0xff;
            }
            else
            {
                // Verde
                texture[i][j][0] = 0x00;
                texture[i][j][1] = 0xff;
                texture[i][j][2] = 0x00;
            }
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}

void makeBlueTexture()
{
    GLubyte texture[16][16][3];
    int i, j;

    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 16; j++)
        {
            if((i + j) % 2 == 0)
            {
                // Branco
                texture[i][j][0] = 0xff;
                texture[i][j][1] = 0xff;
                texture[i][j][2] = 0xff;
            }
            else
            {
                // Azul
                texture[i][j][0] = 0x00;
                texture[i][j][1] = 0x00;
                texture[i][j][2] = 0xff;
            }
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}

void genSphereVertex(float i, float j, float *vertex)
{
	float y = -1.0 + (2.0 * j);
    float r = sqrt(1.0 - pow(y, 2));
	float x = r * cos(2.0 * M_PI * i);
	float z = r * sin(2.0 * M_PI * i);

	vertex[0] = x;
	vertex[1] = y;
	vertex[2] = z;
}

void drawSurface(void (*genVertex)(float, float, float*))
{
    int i;
    int j;
    float angle = 80 * (glutGet(GLUT_ELAPSED_TIME)/1000.0);
    const int h_div = 40;
    const int v_div = 40;

    glColor3f(1.0, 1.0, 1.0);

    // Rotação da Esfera
    // A variável direcao espefica a direção da rotação de acordo
    // com o movimento da Bouncing Ball
    glRotatef(angle, 0.0, direcao, 0.0);

    for(j = 0; j < v_div; j++)
    {
        for(i = 0; i < h_div; i++)
        {
            glBegin(GL_QUADS);
              float vertex[3];
              float u, v;
              u = (float)i/h_div;
              v = (float)j/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u); // Especifica coordenadas de textura
              glVertex3fv(vertex);

              u = (float)(i+1)/h_div;
              v = (float)j/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);

              u = (float)(i+1)/h_div;
              v = (float)(j+1)/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);

              u = (float)i/h_div;
              v = (float)(j+1)/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);
            glEnd();
    }
  }
}

void makeTextureShadow()
{
    GLubyte texture[16][16][3];
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            texture[j][i][0] = 0xff;
            texture[j][i][1] = 0xff;
            texture[j][i][2] = 0xff;
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}


void genShadowVertex(float i, float j, float *vertex)
{
	float y = -1.0 + (2.0 * j);
    float r = sqrt(1.0 - pow(y, 2));
	float x = r * cos(2.0 * M_PI * i);
	float z = r * sin(2.0 * M_PI * i);

	vertex[0] = x;
	vertex[1] = y;
	vertex[2] = z;
}

void drawShadow(void (*genVertex)(float, float, float*))
{
    int i;
    int j;
    const int h_div = 40;
    const int v_div = 40;

    glColor3f(0.6, 0.6, 0.6);

    for(j = 0; j < v_div; j++)
    {
        for(i = 0; i < h_div; i++)
        {
            glBegin(GL_QUADS);
              float vertex[3];
              float u, v;
              u = (float)i/h_div;
              v = (float)j/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u); // Especifica coordenadas de textura
              glVertex3fv(vertex);

              u = (float)(i+1)/h_div;
              v = (float)j/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);

              u = (float)(i+1)/h_div;
              v = (float)(j+1)/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);

              u = (float)i/h_div;
              v = (float)(j+1)/v_div;
              genVertex(u, v, vertex);
              glTexCoord2f(v, u);
              glVertex3fv(vertex);
            glEnd();
    }
  }
}

void bouncingBall()
{
    static float t = 0;
    float delta_t = glutGet(GLUT_ELAPSED_TIME)/1000.0 - t;
    t += delta_t;
    float g = -3.0;

    vy = vy + g * delta_t;
    //vx = vx + g * 0.001;

    glPushMatrix();

        if(Ty + vy * delta_t + 1.0 > yMax)
        {
            vy = -vy;
        }

        if(Ty + vy * delta_t - 1.0 < yMin)
        {
            vy = -vy;
        }

        if(Tx + vx * delta_t + 1.0 > xMax)
        {
            vx = -vx;
            direcao = -1.0;
        }

        if(Tx + vx * delta_t + 1.0 < xMin)
        {
            vx = -vx;
            direcao = 1.0;
        }

        // Bola
        glPushMatrix();
            glRotatef(-16.0f, 0.0f, 0.0f, 1.0f); // Inclinação da bola
            glTranslatef(Tx, Ty, -8.0);
            drawSurface(genSphereVertex);
        glPopMatrix();

        // Sombra
        glPushMatrix();
            makeTextureShadow();
            glRotatef(-16.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(Tx + 0.5, Ty, -9.0f);
            drawShadow(genShadowVertex);
        glPopMatrix();

        Tx = Tx + vx * delta_t;
        Ty = Ty + vy * delta_t;

        printf("Delta = %.2f\nTX = %.2f\nTY = %.2f\n", delta_t, Tx, Ty);
    glPopMatrix();
}

void planos()
{
    float y, x;

    glLineWidth(2.4f);
    glColor3f(1.0f, 0.0f, 1.0f);

    // Plano inferior e horizontal
    glPushMatrix();
        glTranslatef(0.0, -6.0, -13.0);
        glRotatef(95.0f, 1.0f, 0.0f, 0.0f);

        // Linhas horizontais
        y = 5.0f;
        for(int i = 0; i < 15; i++, y -= 0.671)
        {
          glBegin(GL_LINES);
            glVertex2f(-5.0f, y);
            glVertex2f(4.8f, y);
          glEnd();
        }

        // Linhas verticais
        x = -5.0f;
        for(int i = 0; i < 17; i++, x += 0.61)
        {
          glBegin(GL_LINES);
            glVertex2f(x, 5.0f);
            glVertex2f(x, -4.44f);
          glEnd();
        }
    glPopMatrix();

    // Plano traseiro e vertical
    glPushMatrix();
        glTranslatef(0.0, -1.2, -17.4);

        // Linhas verticais
        y = 5.0f;
        for(int i = 0; i < 16; i++, y -= 0.627)
        {
          glBegin(GL_LINES);
            glVertex2f(-5.0f, y);
            glVertex2f(4.8f, y);
          glEnd();
        }

        // Linhas horizontais
        x = -5.0f;
        for(int i = 0; i < 17; i++, x += 0.61)
        {
          glBegin(GL_LINES);
            glVertex2f(x, 5.0f);
            glVertex2f(x, -4.44f);
          glEnd();
        }
    glPopMatrix();
}

void keyboard(unsigned char key, int x, int y)
{
  printf("%d\n", key);
  switch(key) {
    case 27:
    exit(0);
    break;
  }
}

// Função para gerenciamento das primitivas do OpenGL
void menuCores(int op)
{
    //printf("**** Menu de Cores\n");

    switch(op)
    {
        case 0:
            //printf("Cor VERMELHA\n\n");
            cor = 0;
            break;
        case 1:
            //printf("Cor VERDE\n\n");
            cor = 1;
            break;
        case 2:
            //printf("Cor AZUL\n\n");
            cor = 2;
            break;
    }

    glutPostRedisplay();
}

void menuPrincipal(int op)
{

}

void criaMenu()
{
    int menu, submenuCores;

    // Cria menu para seleção da cor
    submenuCores = glutCreateMenu(menuCores);

    // Adição de itens ao menu corrente
    glutAddMenuEntry("VERMELHO", 0);
    glutAddMenuEntry("VERDE", 1);
    glutAddMenuEntry("AZUL", 2);

    // Cria menu principal
    menu = glutCreateMenu(menuPrincipal);
    glutAddSubMenu("Cores", submenuCores);

    // Associa o menu corrente ao botão direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
