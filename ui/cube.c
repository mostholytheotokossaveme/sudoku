#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include "gi.h"

typedef struct {
	void* gi;
	tbl_t* tbl;
} game_t;

game_t game;

void init(void) 
{
   glClearColor (0.2, 0.2, 0.2, 0.2);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 1024.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
         /* modeling transformation */ 
   rect_t vrect = { .x=10, .y = 500, .w = 60, .h = 60 };
   int sz,r,c;
   for (sz=9, r=0; r<sz; r++) {
	   for(c=0; c<sz;c++) {
		rect_t sq = vrect;
		sq.x = sq.x + sq.w * c;
		sq.y = sq.y - r * sq.h;
		glPushMatrix();
		glTranslatef(sq.x, sq.y, 2.);		
		glBegin(GL_QUADS);
		glVertex3i(2,1,0);
		glVertex3i(1,2,0);
		glVertex3i(1,1,0);
		glVertex3i(2,2,0);
		glEnd();
		glPopMatrix();	   
	   }
   }
   
   
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (0.0, 1.0, 0.0, 1.0, 1.5, 2000.0);
   glMatrixMode (GL_MODELVIEW);
}
void MouseHover(int x, int y) {
	printf ("mhover %d %d\n",x,y);
}
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (512, 512); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   glutPassiveMotionFunc(MouseHover);
   /*game.gi = gi_open(-,0, g_Width, g_Height, 9, 4);
   game.tbl = gi_tbl(game.gi); */
   init ();
   glutSetCursor(GLUT_CURSOR_INFO);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);   
   glutMainLoop();
   return 0;
}
