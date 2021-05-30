#include <assert.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "xdraw_plugin.h"
#include "gi.h"

static struct xdraw_plugin* plug;
static void* gi;
static tbl_t* tbl;

static void am_clean(void) {
   gi_free(gi);
   gi = 0;
   tbl = 0;
}

static void am_kpress(unsigned char k,int x,int y) {
	gi_on_kbd(gi, k);
}

static void am_krelease(unsigned char k, int x, int y){
}

static void am_mpress(int k, int x, int y){
	gi_on_md(gi,x,y);
}
static void am_mrelease(int k, int x, int y){
	gi_on_mu(gi,x,y);
}
static void am_mmove(int k, int x, int y){
   gi_on_mo (gi, x, y);
}
static void am_mbmove(int k, int x, int y) {
   gi_on_mo (gi, x, y);
}

static void am_draw_rect(rect_t* r) {
  int w,h;
  w = r->w; h = r->h;
  w/=2; h/=2;
  glBegin(GL_QUADS);
  glVertex3i(-w, -w, w);    glTexCoord2f (0, 0);
  glVertex3i(w, -w, w);     glTexCoord2f (1, 0);
  glVertex3i(w, w, w);      glTexCoord2f (1, 1);
  glVertex3i(-w, w, w);     glTexCoord2f (0, 1);
  glEnd();
}

void DrawCubeFace(float fSize)
{
  fSize /= 2.0;
  glBegin(GL_QUADS);
  glVertex3f(-fSize, -fSize, fSize);    glTexCoord2f (0, 0);
  glVertex3f(fSize, -fSize, fSize);     glTexCoord2f (1, 0);
  glVertex3f(fSize, fSize, fSize);      glTexCoord2f (1, 1);
  glVertex3f(-fSize, fSize, fSize);     glTexCoord2f (0, 1);
  glEnd();
}

void DrawCubeWithTextureCoords (float fSize)
{
  glPushMatrix();
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 1, 0, 0);
  DrawCubeFace (fSize);
  glRotatef (90, 0, 1, 0);
  DrawCubeFace (fSize);
  glRotatef (180, 0, 1, 0);
  DrawCubeFace (fSize);
  glPopMatrix();
}

void RenderObjects(void)
{
  float colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
  float colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };
  float colorBlue[4]       = { 0.0, 0.2, 1.0, 1.0 };
  float colorNone[4]       = { 0.0, 0.0, 0.0, 0.0 };

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Main object (cube) ... transform to its coordinates, and render
  glRotatef(15, 1, 0, 0);
  glRotatef(45, 0, 1, 0);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colorNone);
  glColor4fv(colorBlue);
  glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_CUBE);
  DrawCubeWithTextureCoords(1.0);

  glPopMatrix();
}

static void am_draw() {
   glLoadIdentity();
   gluLookAt(0, 0, -9, 0, 0, -1, 0, 1, 0);
   assert(0);
   RenderObjects();
}


static void am_init() {
 gi = gi_open(0, 0, 512, 512, 9, 4);
 tbl = gi_tbl(gi);
}

void libam_so(void* pd) {
	if (pd != NULL) {
		struct xdraw_plugin* p;
		*(void**) (&p) = pd;
		p->display = am_draw;
		p->cleanup = am_clean;
		p->init = am_init;
		p->ctrls->kpress = am_kpress;
		p->ctrls->krelease = am_krelease;
		p->ctrls->mpress = am_mpress;
		p->ctrls->mrelease = am_mrelease;
		p->ctrls->mmove = am_mmove;
		p->ctrls->mbmove = am_mbmove;



		plug = p;
	}
}

