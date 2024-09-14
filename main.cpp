#pragma hdrstop

// Se voc� estiver usando o Builder defina BUILDER para desabilitar
// excess�es em ponto flutuante
#ifdef BUILDER
#include <float.h>
#endif

#pragma argsused
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define ESCAPE 27
#define TAMANHO_ESFERA 0.04

#define ALT_QUADRIL 0.4
#define LARG_QUADRIL 0.4
#define COMP_QUADRIL 0.11

#define LARG_TRONCO 2.0
#define ALT_TRONCO 0.8
#define COMP_TRONCO 0.5

#define ALT_FEMUR 0.5
#define LARG_FEMUR 0.2
#define COMP_FEMUR 0.1

#define ALT_CANELA 0.35
#define LARG_CANELA 0.1
#define COMP_CANELA 0.08

#define ALT_PATA 0.15
#define BASE_PATA 0.1

#define ALT_PESCOCO 0.2
#define LARG_PESCOCO 0.2
#define COMP_PESCOCO 0.2

#define ALT_PESCOCO 0.4
#define LARG_PESCOCO 1.4
#define COMP_PESCOCO 0.2

#define ALT_CABECA 0.3
#define LARG_CABECA 0.4
#define COMP_CABECA 0.15

#define ESQUERDA_ANTERIOR 0
#define DIREITA_ANTERIOR 1
#define ESQUERDA_POSTERIOR 2
#define DIREITA_POSTERIOR 3

int window;
int Width;
int Height;

#define QUADRIL 0
#define FEMUR 1
#define CANELA 2
#define PATA 3
// angulos[estagio][posicao][quadril/femur/canela/pata]
float angulosCaminhada[6][4][4];
float angulosTrote[6][4][4];
float anguloCabeca;

float anguloTronco;
float deslocamento = 0.0;
float anguloRabo = 0.0;

int estagio = 0;
int passo = 0; // 0 - 10
int caminhando = 1;
float deslocamentoYTronco = 0.0;
int movimentarCavalo = 1;
float anguloCavalo = 0.0;
int passoRabo = 0;
int passoRaboSubindo = 1;
int iluminacao = 1;
int arvores = 1;
float anguloPescoco = 0.0;
int anguloPescocoSubindo = 1;

float xCavalo = 0.0;
float zCavalo = 1.5;

// Prototipo das funcoes
void desenhaEsfera();
void desenhaFemur(int posicao);
void desenhaCanela(int posicao);
void desenhaPata(int posicao);
void desenhaCabeca();
void desenhaCorpo();
void inicializaAngulos();
void renderScene();
void timer(int value);
GLUquadricObj *params = gluNewQuadric();

static float angle = 0.0, ratio;
static float x = 0.0f, y = 0.75f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;

//---------------------------------------------------------------------------
void changeSize(int w, int h)
{
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;

  ratio = 1.0f * w / h;
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the clipping volume
  gluPerspective(45, ratio, 1, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//---------------------------------------------------------------------------
void desenhaArvore()
{
  glColor3f(0.54, 0.4, 0.3);
  glRotatef(-90, 1, 0, 0);
  gluCylinder(params, 0.2, 0.2, 2, 15, 2);
  glTranslatef(0, 0, 2);
  glColor3f(0.14, 0.42, 0.13);
  gluCylinder(params, 0.8, 0.0, 2, 15, 2);
}
//---------------------------------------------------------------------------
void initScene()
{
  glEnable(GL_DEPTH_TEST);
}
//---------------------------------------------------------------------------
void renderScene(void)
{

  int i;
  int j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpe a tela e o buffer

  GLfloat diffuseLight[] = {1, 1, 1, 1};
  GLfloat ambientLight[] = {1, 1, 1, 1};
  GLfloat specularLight[] = {0.2, 0.3, 0.3, 1};
  GLfloat lightPos[] = {300.0f, 2000.0f, -20.0f, 1.0f};
  if (iluminacao)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuseLight);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);

  // deseha ch�o
  glPushMatrix();
  glColor3f(0.05f, 0.25f, 0.05f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f, 0.1f, -100.0f);
  glVertex3f(-100.0f, 0.1f, 100.0f);
  glVertex3f(100.0f, 0.1f, 100.0f);
  glVertex3f(100.0f, 0.1f, -100.0f);
  glEnd();
  glPopMatrix();

  if (arvores)
  {
    // Desenha 64  �rvores
    for (i = -4; i < 4; i++)
      for (j = -4; j < 4; j++)
      {
        glPushMatrix();
        glTranslatef(i * 10.0, 0, j * 10.0);
        desenhaArvore();
        glPopMatrix();
      }
  }
  glPushMatrix();
  glTranslatef(xCavalo, 0.945, zCavalo);
  glRotatef(anguloCavalo, 0, 1, 0);
  desenhaCorpo();
  glPopMatrix();
  glutSwapBuffers();
}
//---------------------------------------------------------------------------
void orientMe(float ang)
{
  lx = sin(ang);
  lz = -cos(ang);
  glLoadIdentity();
  gluLookAt(x, y, z,
            x + lx, y + ly, z + lz,
            0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
void moveMeFlat(int i)
{
  x = x + i * (lx) * 0.1;
  z = z + i * (lz) * 0.1;
  glLoadIdentity();
  gluLookAt(x, y, z,
            x + lx, y + ly, z + lz,
            0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
void processNormalKeys(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
    break;
  case 'b':
  {
    float maiorAngulo = caminhando ? 20.0 : 15.0;
    if (anguloPescoco > maiorAngulo || anguloPescoco < 0.0)
      anguloPescocoSubindo = !anguloPescocoSubindo;
    float incremento = caminhando ? 1.5 : 3.0;
    anguloPescoco = anguloPescocoSubindo ? anguloPescoco + incremento : anguloPescoco - incremento;
    if (passo < 10)
    {
      caminhando ? passo += 2 : passo += 3;
      if (estagio == 0 || estagio == 2)
        deslocamentoYTronco += 0.01;
      else if (estagio == 1 || estagio == 3)
        deslocamentoYTronco -= 0.01;
    }
    else
    {
      passo = 0;
      int estagioFinal = 5;
      if (!caminhando)
        estagioFinal = 3;
      if (estagio < estagioFinal)
        estagio++;
      else
        estagio = 0;
    }
    if (movimentarCavalo)
    {
      float deslocamento = caminhando ? 0.03 : 0.12;
      float anguloGraus = anguloCavalo * (M_PI / 180);
      xCavalo += deslocamento * cos(anguloGraus);
      zCavalo -= deslocamento * sin(anguloGraus);
    }
    break;
  }
  case ',':
    anguloCavalo += 5;
    break;
  case '.':
    anguloCavalo -= 5;
    break;
  }
  renderScene();
}
//---------------------------------------------------------------------------

void inputKey(int key, int x, int y)
{

  switch (key)
  {
  case GLUT_KEY_LEFT:
    angle -= 0.05f;
    orientMe(angle);
    break;
  case GLUT_KEY_RIGHT:
    angle += 0.05f;
    orientMe(angle);
    break;
  case GLUT_KEY_UP:
    moveMeFlat(5);
    break;
  case GLUT_KEY_DOWN:
    moveMeFlat(-5);
    break;
  case GLUT_KEY_F1:
    deslocamentoYTronco = 0.0;
    caminhando = !caminhando;
    break;
  case GLUT_KEY_F5:
    glutFullScreen();
    break;
  case GLUT_KEY_F6:
    glutReshapeWindow(640, 360);
    break;
  case GLUT_KEY_F7:
    iluminacao = !iluminacao;
    break;
  case GLUT_KEY_F8:
    arvores = !arvores;
    break;
  case GLUT_KEY_F11:
    movimentarCavalo = !movimentarCavalo;
    break;
  }
  renderScene();
}

//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
#ifdef BUILDER
  _control87(MCW_EM, MCW_EM); // Desabilitar excessoes em ponto flutuante (builder)
#endif

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 360);
  glutCreateWindow("Trabalho de computa��o gr�fica");

  initScene();

  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(inputKey);

  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutTimerFunc(100, timer, 0);

  inicializaAngulos();
  glutMainLoop();

  return (0);
}
//---------------------------------------------------------------------------
float pegaAngulo(int posicao, int parte, bool posicaoAtual)
{
  int estagioDoAngulo = estagio;
  int estagioFinal = caminhando ? 5 : 3;
  if (!posicaoAtual)
  {
    if (estagioDoAngulo == estagioFinal)
      estagioDoAngulo = 0;
    else
      estagioDoAngulo++;
  }

  float(*vetor)[4][4] = caminhando ? angulosCaminhada : angulosTrote;

  int estagioDeTroca = caminhando ? 2 : 1;
  if (estagioDoAngulo > estagioDeTroca)
  {
    switch (posicao)
    {
    case ESQUERDA_ANTERIOR:
      return vetor[estagioDoAngulo - estagioDeTroca - 1][DIREITA_ANTERIOR][parte];
    case DIREITA_ANTERIOR:
      return vetor[estagioDoAngulo - estagioDeTroca - 1][ESQUERDA_ANTERIOR][parte];
    case ESQUERDA_POSTERIOR:
      return vetor[estagioDoAngulo - estagioDeTroca - 1][DIREITA_POSTERIOR][parte];
    case DIREITA_POSTERIOR:
      return vetor[estagioDoAngulo - estagioDeTroca - 1][ESQUERDA_POSTERIOR][parte];
    }
  }
  else
  {
    return vetor[estagioDoAngulo][posicao][parte];
  }
  return 0.0;
}
//---------------------------------------------------------------------------
float pegaAngulo(int posicao, int parte)
{
  float fatorPasso = passo / 10.0;
  float anguloAtual = pegaAngulo(posicao, parte, true);
  float proximoAngulo = pegaAngulo(posicao, parte, false);

  return anguloAtual + (proximoAngulo - anguloAtual) * fatorPasso;
}
//---------------------------------------------------------------------------
void inicializaAngulos()
{
  angulosCaminhada[0][ESQUERDA_POSTERIOR][FEMUR] = 7;
  angulosCaminhada[0][ESQUERDA_POSTERIOR][CANELA] = -33;
  angulosCaminhada[0][ESQUERDA_POSTERIOR][PATA] = 0;
  angulosCaminhada[0][DIREITA_POSTERIOR][FEMUR] = 4;
  angulosCaminhada[0][DIREITA_POSTERIOR][CANELA] = -4;
  angulosCaminhada[0][DIREITA_POSTERIOR][PATA] = 0;
  angulosCaminhada[0][ESQUERDA_ANTERIOR][QUADRIL] = 15;
  angulosCaminhada[0][ESQUERDA_ANTERIOR][FEMUR] = -35;
  angulosCaminhada[0][ESQUERDA_ANTERIOR][CANELA] = 70;
  angulosCaminhada[0][ESQUERDA_ANTERIOR][PATA] = 0;
  angulosCaminhada[0][DIREITA_ANTERIOR][QUADRIL] = -20;
  angulosCaminhada[0][DIREITA_ANTERIOR][FEMUR] = -26;
  angulosCaminhada[0][DIREITA_ANTERIOR][CANELA] = 39;
  angulosCaminhada[0][DIREITA_ANTERIOR][PATA] = 0;

  angulosCaminhada[1][ESQUERDA_POSTERIOR][FEMUR] = 43;
  angulosCaminhada[1][ESQUERDA_POSTERIOR][CANELA] = -81;
  angulosCaminhada[1][ESQUERDA_POSTERIOR][PATA] = 0;
  angulosCaminhada[1][DIREITA_POSTERIOR][FEMUR] = -8;
  angulosCaminhada[1][DIREITA_POSTERIOR][CANELA] = -6;
  angulosCaminhada[1][DIREITA_POSTERIOR][PATA] = -4;
  angulosCaminhada[1][ESQUERDA_ANTERIOR][QUADRIL] = 5;
  angulosCaminhada[1][ESQUERDA_ANTERIOR][FEMUR] = -20;
  angulosCaminhada[1][ESQUERDA_ANTERIOR][CANELA] = 38;
  angulosCaminhada[1][ESQUERDA_ANTERIOR][PATA] = 0;
  angulosCaminhada[1][DIREITA_ANTERIOR][QUADRIL] = -27;
  angulosCaminhada[1][DIREITA_ANTERIOR][FEMUR] = -25;
  angulosCaminhada[1][DIREITA_ANTERIOR][CANELA] = 30;
  angulosCaminhada[1][DIREITA_ANTERIOR][PATA] = 0;

  angulosCaminhada[2][ESQUERDA_POSTERIOR][FEMUR] = 35;
  angulosCaminhada[2][ESQUERDA_POSTERIOR][CANELA] = -30;
  angulosCaminhada[2][ESQUERDA_POSTERIOR][PATA] = 0;
  angulosCaminhada[2][DIREITA_POSTERIOR][FEMUR] = -20;
  angulosCaminhada[2][DIREITA_POSTERIOR][CANELA] = 0;
  angulosCaminhada[2][DIREITA_POSTERIOR][PATA] = 0;
  angulosCaminhada[2][ESQUERDA_ANTERIOR][QUADRIL] = 20;
  angulosCaminhada[2][ESQUERDA_ANTERIOR][FEMUR] = -60;
  angulosCaminhada[2][ESQUERDA_ANTERIOR][CANELA] = 38;
  angulosCaminhada[2][ESQUERDA_ANTERIOR][PATA] = 0;
  angulosCaminhada[2][DIREITA_ANTERIOR][QUADRIL] = 5;
  angulosCaminhada[2][DIREITA_ANTERIOR][FEMUR] = -70;
  angulosCaminhada[2][DIREITA_ANTERIOR][CANELA] = 40;
  angulosCaminhada[2][DIREITA_ANTERIOR][PATA] = 0;

  angulosTrote[0][ESQUERDA_POSTERIOR][FEMUR] = 70;
  angulosTrote[0][ESQUERDA_POSTERIOR][CANELA] = -70;
  angulosTrote[0][ESQUERDA_POSTERIOR][PATA] = -50;
  angulosTrote[0][DIREITA_POSTERIOR][FEMUR] = -15;
  angulosTrote[0][DIREITA_POSTERIOR][CANELA] = 0;
  angulosTrote[0][DIREITA_POSTERIOR][PATA] = -10;
  angulosTrote[0][ESQUERDA_ANTERIOR][QUADRIL] = -5;
  angulosTrote[0][ESQUERDA_ANTERIOR][FEMUR] = -25;
  angulosTrote[0][ESQUERDA_ANTERIOR][CANELA] = 30;
  angulosTrote[0][ESQUERDA_ANTERIOR][PATA] = -20;
  angulosTrote[0][DIREITA_ANTERIOR][QUADRIL] = 33;
  angulosTrote[0][DIREITA_ANTERIOR][FEMUR] = -75;
  angulosTrote[0][DIREITA_ANTERIOR][CANELA] = 95;
  angulosTrote[0][DIREITA_ANTERIOR][PATA] = -05;

  angulosTrote[1][ESQUERDA_POSTERIOR][FEMUR] = 45;
  angulosTrote[1][ESQUERDA_POSTERIOR][CANELA] = 0;
  angulosTrote[1][ESQUERDA_POSTERIOR][PATA] = 0;
  angulosTrote[1][DIREITA_POSTERIOR][FEMUR] = 25;
  angulosTrote[1][DIREITA_POSTERIOR][CANELA] = -75;
  angulosTrote[1][DIREITA_POSTERIOR][PATA] = 15;
  angulosTrote[1][ESQUERDA_ANTERIOR][QUADRIL] = -25;
  angulosTrote[1][ESQUERDA_ANTERIOR][FEMUR] = -35;
  angulosTrote[1][ESQUERDA_ANTERIOR][CANELA] = 75;
  angulosTrote[1][ESQUERDA_ANTERIOR][PATA] = 0;
  angulosTrote[1][DIREITA_ANTERIOR][QUADRIL] = 10;
  angulosTrote[1][DIREITA_ANTERIOR][FEMUR] = -5;
  angulosTrote[1][DIREITA_ANTERIOR][CANELA] = 45;
  angulosTrote[1][DIREITA_ANTERIOR][PATA] = 0;

  anguloCabeca = 90.0;
  anguloTronco = -3.75;
}
//---------------------------------------------------------------------------
void desenhaCabeca()
{
  glPushMatrix();
  glRotatef(45 - anguloPescoco, 0, 0, 1);
  glPushMatrix();
  glScalef(LARG_PESCOCO, ALT_PESCOCO, COMP_PESCOCO);
  glutSolidCube(0.5);
  glPopMatrix();
  glTranslatef(LARG_PESCOCO * 0.19, -ALT_PESCOCO * 0.4, 0);
  glRotatef(anguloCabeca, 0, 0, 1);
  glScalef(LARG_CABECA, ALT_CABECA, COMP_CABECA);
  glutSolidCube(0.5);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaRabo()
{
  float angulo = caminhando ? 0.0 : 5.0;
  float angulo2 = caminhando ? passoRabo : passoRabo / 2;

  // primeiro segmento
  glColor3f(0.6, 0.5, 0.3);
  glutSolidSphere(TAMANHO_ESFERA / 1.25, 8, 8);
  glColor3f(1.0, 0.8, 0);
  glRotatef(90 - angulo2, 0, 1, 0);
  glRotatef(-45 + angulo, 1, 0, 0);
  glTranslatef(0, 0, -0.20);
  gluCylinder(params, 0.020, 0.03, 0.20, 15, 2);

  // segundo segmento
  glColor3f(0.6, 0.5, 0.3);
  glutSolidSphere(TAMANHO_ESFERA / 1.5, 8, 8);
  glColor3f(1.0, 0.8, 0);
  glRotatef(-angulo2 * 2, 0, 1, 0);
  glRotatef(-24 + angulo / 2, 1, 0, 0);
  glTranslatef(0, 0, -0.15);
  gluCylinder(params, 0.015, 0.020, 0.15, 15, 2);

  // terceiro segmento
  glColor3f(0.6, 0.5, 0.3);
  glutSolidSphere(TAMANHO_ESFERA / 2, 8, 8);
  glRotatef(-angulo2 * 3, 0, 1, 0);
  glColor3f(1.0, 0.8, 0);
  glTranslatef(0, 0, -0.25);
  gluCylinder(params, 0.0, 0.015, 0.25, 15, 2);
}
//---------------------------------------------------------------------------
void desenhaQuadril(int posicao)
{
  glPushMatrix();
  glRotatef(pegaAngulo(posicao, QUADRIL), 0, 0, 1);
  glTranslatef(0.0, -TAMANHO_ESFERA, 0.0);
  //  glTranslatef(0.0,-ALT_QUADRIL*0.25,0.0);
  glPushMatrix();
  glScalef(LARG_QUADRIL, ALT_QUADRIL, COMP_QUADRIL);

  glRotatef(90, 1, 0, 0);
  gluCylinder(params, 0.3, 0.3, 0.5, 15, 2);

  //  glutSolidCube(0.5);
  glPopMatrix();
  glTranslatef(0.0, -ALT_QUADRIL * 0.5 - TAMANHO_ESFERA, 0.0);
  desenhaEsfera();
  desenhaFemur(posicao);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaFemur(int posicao)
{
  glPushMatrix();
  glRotatef(pegaAngulo(posicao, FEMUR), 0, 0, 1);
  glTranslatef(0.0, -TAMANHO_ESFERA, 0.0);
  //  glTranslatef(0.0,-ALT_FEMUR*0.25,0.0);
  glPushMatrix();
  glScalef(LARG_FEMUR, ALT_FEMUR, COMP_FEMUR);

  glRotatef(90, 1, 0, 0);
  gluCylinder(params, 0.3, 0.3, 0.5, 15, 2);

  //  glutSolidCube(0.5);
  glPopMatrix();
  glTranslatef(0.0, -ALT_FEMUR * 0.5 - TAMANHO_ESFERA, 0.0);
  desenhaEsfera();
  desenhaCanela(posicao);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaCanela(int posicao)
{
  glPushMatrix();
  glRotatef(pegaAngulo(posicao, CANELA), 0, 0, 1);
  glTranslatef(0.0, -TAMANHO_ESFERA, 0.0);
  //  glTranslatef(0.0,-ALT_CANELA*0.25,0.0);
  glPushMatrix();
  glScalef(LARG_CANELA, ALT_CANELA, COMP_CANELA);

  glRotatef(90, 1, 0, 0);
  gluCylinder(params, 0.3, 0.3, 0.5, 15, 2);

  //  glutSolidCube(0.5);
  glPopMatrix();
  glTranslatef(0.0, -ALT_CANELA * 0.5 - TAMANHO_ESFERA, 0.0);
  desenhaEsfera();
  desenhaPata(posicao);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaPata(int posicao)
{
  glPushMatrix();
  glRotatef(pegaAngulo(posicao, PATA), 0, 0, 1);
  glTranslatef(0.0, -TAMANHO_ESFERA, 0.0);
  glTranslatef(0.0, -ALT_PATA * 0.35, 0.0);
  glScalef(BASE_PATA, ALT_PATA, BASE_PATA);
  glRotatef(-90, 1, 0, 0);
  glutSolidCone(0.5, 0.6, 8, 6);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaTronco()
{
  if (!caminhando)
    glTranslatef(0.0, deslocamentoYTronco, 0.0);

  glPushMatrix();
  glColor3f(0.6, 0.4, 0.1);
  glPushMatrix();
  glTranslatef(-LARG_TRONCO * 0.2, 0, 0.0);
  glPushMatrix();
  glScalef(1, ALT_TRONCO * 1.5, 1);
  glutSolidSphere(COMP_TRONCO * 0.335, 8, 8);
  glPopMatrix();
  glScalef(LARG_TRONCO, ALT_TRONCO, COMP_TRONCO);
  glRotatef(90, 0, 1, 0);
  gluCylinder(params, 0.25, 0.25, 0.4, 15, 2);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(LARG_TRONCO * 0.2, 0, 0.0);
  glScalef(1, ALT_TRONCO * 1.5, 0.75);
  glutSolidSphere(COMP_TRONCO * 0.335, 8, 8);
  glPopMatrix();

  //  glutSolidCube(0.5);
  glColor3f(0.6, 0.25, 0.1);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaPerna(int posicao)
{
  glPushMatrix();
  if (posicao == ESQUERDA_ANTERIOR || posicao == DIREITA_ANTERIOR)
    desenhaQuadril(posicao);
  else
    desenhaFemur(posicao);
  glPopMatrix();
}
//---------------------------------------------------------------------------
void desenhaEsfera()
{
  glColor3f(0.6, 0.5, 0.3);
  glutSolidSphere(TAMANHO_ESFERA, 8, 8);
  glColor3f(1.0, 0.8, 0);
}
//---------------------------------------------------------------------------
void desenhaCorpo()
{
  desenhaTronco();

  glPushMatrix();
  glTranslatef(-LARG_TRONCO * 0.17, ALT_TRONCO * 0.1, COMP_TRONCO * 0.3);
  desenhaEsfera();
  desenhaPerna(ESQUERDA_ANTERIOR);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(LARG_TRONCO * 0.22, -ALT_TRONCO * 0.2, COMP_TRONCO * 0.2);
  desenhaEsfera();
  desenhaPerna(ESQUERDA_POSTERIOR);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(LARG_TRONCO * 0.22, -ALT_TRONCO * 0.2, -COMP_TRONCO * 0.2);
  desenhaEsfera();
  desenhaPerna(DIREITA_POSTERIOR);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-LARG_TRONCO * 0.17, ALT_TRONCO * 0.1, -COMP_TRONCO * 0.3);
  desenhaEsfera();
  desenhaPerna(DIREITA_ANTERIOR);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(LARG_TRONCO * 0.25, ALT_TRONCO * 0.2, 0.0);
  desenhaCabeca();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-LARG_TRONCO * 0.28, ALT_TRONCO * 0.1, 0.0);
  desenhaRabo();
  glPopMatrix();
}
//---------------------------------------------------------------------------
void timer(int value)
{
  if (passoRabo > 15)
  {
    passoRaboSubindo = 0;
    if (caminhando)
      passoRabo--;
    else
      passoRabo -= 2;
  }
  else if (passoRabo < -15)
  {
    passoRaboSubindo = 1;
    if (caminhando)
      passoRabo++;
    else
      passoRabo += 2;
  }
  else
  {
    if (caminhando)
      passoRaboSubindo ? passoRabo++ : passoRabo--;
    else
      passoRaboSubindo ? passoRabo += 2 : passoRabo -= 2;
  }

  renderScene();
  glutTimerFunc(50, timer, 0);
}
//---------------------------------------------------------------------------
