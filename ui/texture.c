#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "texture.h"
#include "j.h"

#define J_JPG ".jpg"
/*
struct texture {
   unsigned char* tx;
   int w;
   int h;
   long ts;
   int rs;
   int c;
   int dpt;
   void *src;
};

typedef struct texture texture_t;
*/
/* unsigned char* jpg_load(const char* a, int* w, int*h, int* c, long *ts); */
texture_t* texture_from_file(char *fn) {
   assert(fn);
   char* ext;
   texture_t tex;
   texture_t* r;

   r = NULL;
   ext = strrchr(fn, '.');
   if (strcmp(ext, J_JPG) == 0) {
    tex.tx = jpg_load(fn, &tex.w, &tex.h, &tex.c, &tex.ts);
    if (tex.tx) {
     tex.rs = tex.c * tex.w;
     tex.dpt = tex.c * 8;
     tex.src = fn;
     r = malloc(sizeof(tex));
     *r = tex;
    }
   }

   return r;
}
