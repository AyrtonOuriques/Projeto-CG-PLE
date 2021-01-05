#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

static int eixo;
float angle = 0.0;
static float eixoporta1 = 1.6 , eixoporta2 = 0.1 , eixoporta3 = 5.0, eixoporta4 = -1.0;
static float eixojanela1 = 1.6 , eixojanela2 = 0.1 , eixojanela3 = -3, eixojanela4 = 0.6;

GLuint texture[8];

void drawHalfSphere(int scaley, int scalex, GLfloat r)
{
   int i, j;
   GLfloat v[scalex*scaley][3];

   for (i=0; i<scalex; ++i) {
     for (j=0; j<scaley; ++j) {
       v[i*scaley+j][0]=r*cos(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
       v[i*scaley+j][1]=r*sin(i*M_PI/(2*scalex));
       v[i*scaley+j][2]=r*sin(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
     }
   }

   glBegin(GL_QUADS);
     for (i=0; i<scalex-1; ++i) {
       for (j=0; j<scaley; ++j) {
         glVertex3fv(v[i*scaley+j]);
         glVertex3fv(v[i*scaley+(j+1)%scaley]);
         glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);
         glVertex3fv(v[(i+1)*scaley+j]);
       }
     }
   glEnd();
 }

static void drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(1, 1);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(1, 0);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(0.3, 0);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(0.3, 1);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void LoadTexture( const char * filename , int aux, int width, int height)
{
  unsigned char * data;
  FILE * file;
  file = fopen( filename, "rb" );

  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;
  }

      glBindTexture( GL_TEXTURE_2D, texture[aux] );
      glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );

      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );

      glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
      free(data);

}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { 1.8, 1, -2.2, 1.0 };
    GLfloat spot_direction[] = { -0.3, -1.0, -0.2 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);


    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

    glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

  //quadro
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
        glVertex3f(2.999, 0.5, -2);
        glTexCoord2f(1, 1);
        glVertex3f(2.999, 1.5, -2);
        glTexCoord2f(0.2, 1);
        glVertex3f(2.999, 1.5, -3);
        glTexCoord2f(0.2, 0);
        glVertex3f(2.999, 0.5, -3);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glVertex3f(3.001, 0.5, -2);
        glVertex3f(3.001, 1.5, -2);
        glVertex3f(3.001, 1.5, -3);
        glVertex3f(3.001, 0.5, -3);
    glEnd();

  //paredes
  glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
	glVertex3f(-3, 2, -5);
	glNormal3f(0, 1, 0);
	glVertex3f(-3, 2, 5);
	glNormal3f(0, 1, 0);
	glVertex3f(3, 2, 5);
	glNormal3f(0, 1, 0);
	glVertex3f(3, 2, -5);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[6]);
  glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-3, -2, -5);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-3, -2, 5);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(3, -2, 5);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(3, -2, -5);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.6, 0.2, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[7]);
  glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(1, 0);
	glVertex3f(-3, -2, 5);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-3, 2, 5);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-3, 2, -5);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-3, -2, -5);
  glEnd();
  glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 0);
	glVertex3f(3, -2, 5);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(3, 2, 5);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(3, 2, -5);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(3, -2, -5);
  glEnd();
  glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(1, 0);
	glVertex3f(-3, -2, -5);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-3, 2, -5);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 1);
	glVertex3f(3, 2, -5);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0);
	glVertex3f(3, -2, -5);
  glEnd();
  glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 0);
	glVertex3f(-3, -2, 5);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1);
	glVertex3f(-3, 2, 5);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(3, 2, 5);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(3, -2, 5);
  glEnd();
  glDisable(GL_TEXTURE_2D);

//  glPushMatrix();
//  glScalef (6.0, 4.0, 10.0);
//  glutWireCube(1);
//  glPopMatrix();


  glColor3f(1, 1, 1);
  //cadeira
   glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture[2]);
    glPushMatrix();
        glTranslatef(2.75, -1.725, 0.0);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1, GL_QUADS);
	glPopMatrix();

    glPushMatrix();
        glTranslatef(2.75, -1.725, -0.5);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(2.25, -1.725, -0.5);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(2.25, -1.725, 0);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1, GL_QUADS);;
	glPopMatrix();

	glPushMatrix();
        glTranslatef(2.5, -1.3, -0.25);
        glScalef (1, 0.15, 0.75);
        drawBox(1, GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(3, -0.75, -0.25);
        glScalef (0.1, 1, 0.75);
        drawBox(1, GL_QUADS);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

    //mesa
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
        glTranslatef(2, -0.5, -3.1);
        glScalef (2, 0.15, 3.75);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(1.5, -1.3, -2);
        glScalef (0.125, 1.5, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(1.5, -1.3, -4.3);
        glScalef (0.125, 1.5, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//cama
	glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture[5]);
    glPushMatrix();
        glTranslatef(-2, -1.2, -2.85);
        glScalef (2, 0.5, 4.3);
        drawBox(1,GL_QUADS);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

	glPushMatrix();
        glTranslatef(-2.8, -1.65, -4.8);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-1.3, -1.65, -4.8);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-1.3, -1.65, -1);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-2.8, -1.65, -1);
        glScalef (0.125, 0.75, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//travesseiro
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
        glTranslatef(-2, -0.8, -4.5);
        glScalef (1.5, 0.35, 1);
        drawBox(1,GL_QUADS);
	glPopMatrix();

	//porta
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
        glTranslatef(eixoporta4, -0.5, eixoporta3);
        glScalef (eixoporta1, 3, eixoporta2);
        drawBox(1,GL_QUADS);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
        glVertex3f(-0.3, -2, 4.999);
        glVertex3f(-0.3, 1, 4.999);
        glVertex3f(-1.8, 1, 4.999);
        glVertex3f(-1.8, -2, 4.999);
    glEnd();

	//janela
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor3f(1, 1, 1);
	glPushMatrix();
        glTranslatef(eixojanela3, eixojanela4, -3);
        glScalef (eixojanela2, eixojanela1, 1.3);
        drawBox(1,GL_QUADS);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
        glVertex3f(-2.999, -0.2, -2.35);
        glVertex3f(-2.999, 1.4, -2.35);
        glVertex3f(-2.999, 1.4, -3.65);
        glVertex3f(-2.999, -0.2, -3.65);
    glEnd();

	//ventilador
	glEnable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glPushMatrix();
	    glTranslatef(2, 0.3, -4);
	    glRotatef (120, 1, 0, 0.0);
	    glRotatef (angle, 1, 0, 0.0);
        glScalef (0.01, 1, 0.125);
        drawBox(1,GL_QUADS);
    glPopMatrix();

    glPushMatrix();
	    glTranslatef(2, 0.3, -4);
	    glRotatef (angle, 1, 0, 0.0);
        glScalef (0.01, 1, 0.125);
        drawBox(1,GL_QUADS);
    glPopMatrix();

    glPushMatrix();
	    glTranslatef(2, 0.3, -4);
	    glRotatef (-120, 1, 0, 0.0);
	    glRotatef (angle, 1, 0, 0.0);
        glScalef (0.01, 1, 0.125);
        drawBox(1,GL_QUADS);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2.1, -0.1, -4);
        glScalef (0.125, 1, 0.125);
        drawBox(1,GL_QUADS);
	glPopMatrix();

    //luminaria
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glPushMatrix();
	    glTranslatef(2, 0.3, -2);
	    glRotatef (140, 1, 0, 0.0);
	    glRotatef (-30, 0, 0, 1);
        glScalef (0.1, 1, 0.125);
        drawBox(0.8, GL_QUADS);
    glPopMatrix();
    glPushMatrix();
	    glTranslatef(2, -0.2, -2);
	    glRotatef (-140, 1, 0, 0.0);
	    glRotatef (30, 0, 0, 1);
        glScalef (0.1, 1, 0.125);
        drawBox(0.8, GL_QUADS);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(1.7, 0.5, -2.3);
        glRotatef (50, 0, 1, 0);
        glRotatef (40, 1, 0, 0);
        glScalef (0.1, 0.1, 0.1);
        drawHalfSphere(10,10,2);
    glPopMatrix();

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'r':
    eixo = (eixo + 1) % 10;
    glRotatef ((GLfloat) eixo, 0.0, 1.0, 0.0);
    glutPostRedisplay();
    break;
  case 'p':
    {
    if ( eixoporta3 == 5.0)
    {
        eixoporta3 = 5.7;
        eixoporta4 = -0.3;
    }
    else
    {
        eixoporta3 = 5.0;
        eixoporta4 = -1.0;
    }
    float aux = eixoporta1;
    eixoporta1 = eixoporta2;
    eixoporta2 = aux;
    glutPostRedisplay();
    break;
    }
  case 'j':
    if ( eixojanela3 == -3.0)
    {
        eixojanela3 = -2.4;
        eixojanela4 = 1.4;
    }
    else
    {
        eixojanela3 = -3.0;
        eixojanela4 = 0.6;
    }
    float aux2 = eixojanela1;
    eixojanela1 = eixojanela2;
    eixojanela2 = aux2;
    glutPostRedisplay();
    break;
  }
}

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 0.01, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 // glTranslatef (0.0, 0.0, -10.0);
}

void init(void)
{

    glClearColor (0, 0, 0, 0);

    glShadeModel(GL_SMOOTH);

    glGenTextures( 8, texture );

     LoadTexture( "van.bmp" , 0 , 756,598);
     LoadTexture( "vidro.bmp" , 1 , 238, 282);
     LoadTexture( "madeira.bmp" , 2 , 800, 800);
     LoadTexture( "metal.bmp" , 3 , 350, 350 );
     LoadTexture( "porta.bmp" , 4 , 236, 501 );
     LoadTexture( "cama.bmp" , 5 , 400, 400 );
     LoadTexture( "chao.bmp" , 6 , 900, 900 );
     LoadTexture( "parede.bmp" , 7 , 521, 506 );

    glDepthFunc(GL_LESS);

    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);



}

void update(int value)
{
 angle += 8;
 if (angle > 360)
 {
  angle -= 360;
 }

 glutPostRedisplay();
 glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutCreateWindow (argv[0]);
  glEnable(GL_DEPTH_TEST);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  update(0);
  glutMainLoop();
  return 0;
}
